#include <QString>
#include "inc/an2kloader.h"
//#include "dpyan2k.h"
#include "defs.h"
#include <sys/param.h>

An2kLoader::An2kLoader(Canvas* canvas)
{
    _canvas = canvas;
    //_file = file;
    nist_flag = 1;
    iafis_flag = 0;
}

void An2kLoader::run()
{
    if (_file.isEmpty() || !_canvas)
        return;

    int ret;
    REC_SEL *rec_sel = 0; /*TODO*/

    ANSI_NIST *ansi_nist;
    if ((ret = read_ANSI_NIST_file(_file.toLocal8Bit().data(), &ansi_nist))){
       return;
    }

    /* Initialize image displayed counter. */
    int num_images = 0;
    for(int record_i = 1; record_i < ansi_nist->num_records; record_i++){
       /* Set current record. */
       RECORD *record = ansi_nist->records[record_i];

       if (image_record(record->type) && select_ANSI_NIST_record(record, rec_sel)){
           if((ret = dpyan2k_record(record_i, ansi_nist))){
              free_ANSI_NIST(ansi_nist);
              return;
           }
           /* Bump image displayed counter. */
           num_images++;
       }
    }

    free_ANSI_NIST(ansi_nist);
}

/*****************************************************************
Joe Konczal combined dpyan2k_binary_record() and dpyan2k_tagged_record(),
which were very similar, in preparation for adding the ability to
display segmentation information in one place instead of two.
*****************************************************************/
int An2kLoader::dpyan2k_record(const int imgrecord_i, const ANSI_NIST *ansi_nist)
{
   int ret;
   RECORD *imgrecord, *minrecord;
   int minrecord_i;
   unsigned char *imgdata;
   float hpix_per_mm, vpix_per_mm;
   int iw, ih, id;
   double ppmm;
   unsigned int whitepix = 0, align;
   int done = 0;
   char wintitle[2 * MAXPATHLEN];
   int *xs, *ys, npts;
   SEGMENTS *segments;

   imgrecord = ansi_nist->records[imgrecord_i];

  /* Set pixel aligment to single byte boundaries. */
   align = 8;

   /* Initialize number of points to 0. */
   npts = 0;

   /* Get and decode (if necessary) image field data. */
   ret = decode_ANSI_NIST_image(&imgdata, &iw, &ih, &id, &ppmm,
                ansi_nist, imgrecord_i, 1 /*interleave flag*/);
   /* If ERROR or IGNORE ... */
   if(ret <= 0)
      return(ret);

   /* New code - believes image attributes returned by the decode process. */
#if 0
      fprintf(stderr, "%s:\n", _file.toLocal8Bit().data());
      fprintf(stderr, "\timage size: %d x %d\n", iw, ih);
      fprintf(stderr, "\tdepth: %d\n", id);
      fprintf(stderr, "\tpoints: %d\n", npts);
#endif

   if(id == 1)
      whitepix = 0;
   else if(id == 8 || id == 24)
      whitepix = 255;
   else{
      fprintf(stderr, "ERROR : dpyan2k_record : "
          "image in record index [%d] [Type-%u] has depth %d != 1 or 8\n",
          imgrecord_i+1, imgrecord->type, id);
      return(-5);
   }


   /* Get image scan frequency. */
   ret = get_pix_per_mm(&hpix_per_mm, &vpix_per_mm, imgrecord_i, ansi_nist);
   /* If ERROR getting pix/mm ... */
   if(ret < 0)
      return(ret);
   /* If pix/mm set AND matching minutiae record found ... */
   if(ret != IGNORE){
      ret = find_matching_minutiae(&minrecord, &minrecord_i,
                                   imgrecord_i, ansi_nist);
      /* If ERROR ... */
      if(ret < 0)
         return(ret);
      /* If matching minutiae record found ... */
      if(ret){
         /* Extract minutiae coordinates converting from micrometer */
         /* units to pixels. */
         if((ret = get_minutiae_pixel_coords(&xs, &ys, &npts,
                         hpix_per_mm, vpix_per_mm, iw, ih,
                         minrecord_i, ansi_nist)))
            return(ret);
      }
   }

   /* Retrieve segmentation data from SEG or ASEG records. */
   if (TYPE_14_ID == imgrecord->type) {
      ret = get_segmentation_data(imgrecord, &segments);
      if (ret < 0)
     return ret;
   }

   fflush(stdout);
   fflush(stderr);

#if 0
   switch(fork()){
      /* On ERROR ... */
      case -1:
           fprintf(stderr, "ERROR : dpyan2k_record : fork failed\n");
           return(-8);
      /* Child process. */
      case 0:
#endif
           /* Compose title for window. */
       if (sprintf(wintitle, "%s : Record %d : Type-%u",
               _file.toLocal8Bit().data(), imgrecord_i+1, imgrecord->type)
           >= sizeof(wintitle)) {
          fprintf(stderr, "ERROR : dpyan2k_record : "
              "sprintf result overflows %lu byte buffer\n",
              (unsigned long)sizeof(wintitle));
          return(-81);
       }
           /* Display the image record's contents. */
           dpyimagepts(wintitle, imgdata, (unsigned int)iw, (unsigned int)ih,
               (unsigned int)id, whitepix, (int)align, &done,
                       xs, ys, npts, segments);
#if 0
           free(imgdata);
           if(npts > 0){
              free(xs);
              free(ys);
           }
           if(verbose)
              fprintf(stderr, "Child: %s exiting\n", wintitle);
           _exit(0);
      /* Parent process. */
      default:
           /* Bump next window offsets. */
           wx += WIN_XY_INCR;
           wy += WIN_XY_INCR;
           break;
   }
#endif

   return(0);
}

/***************************************************************/
int An2kLoader::get_pix_per_mm(float *ohpix_per_mm, float *ovpix_per_mm,
              const int imgrecord_i, const ANSI_NIST *ansi_nist)
{
   RECORD *imgrecord;
   FIELD *field;
   int field_i;
   float hpix_per_mm, vpix_per_mm;
   int slc, hps, vps;
   char *nep;

   *ohpix_per_mm = 0.0;
   *ovpix_per_mm = 0.0;
   imgrecord = ansi_nist->records[imgrecord_i];

   if(binary_image_record(imgrecord->type)){
      /* Get pix/mm from NTR Type-1.012 field. */
      if(!lookup_ANSI_NIST_field(&field, &field_i, NTR_ID,
                                 ansi_nist->records[0])){
         fprintf(stderr, "ERROR : get_pix_per_mm : "
         "NTR field [Type-1.%03d] not found\n", NTR_ID);
         return(-2);
      }

      /* Beware of strtof(), it doesn't seem to work right on Fedora
     Core 7, but (float)strtod() works. -- jck */
      errno = 0;
      hpix_per_mm =
     (float)strtod((char *)field->subfields[0]->items[0]->value, &nep);
      if (errno) {
     fprintf(stderr, "ERROR : get_pix_per_mm : "
         "converting NTR value '%s' to floating point: %s.\n",
         (char *)field->subfields[0]->items[0]->value, strerror(errno));
     return(-23);

      } else if (nep == (char *)field->subfields[0]->items[0]->value
         || *nep != 0) {
     fprintf(stderr, "ERROR : gep_pix_per_mm : "
         "NTR value '%s' is not a floating point number.\n",
         (char *)field->subfields[0]->items[0]->value);
     return(-24);
      }
      vpix_per_mm = hpix_per_mm;
   }
   else if(tagged_image_record(imgrecord->type)){
      /* Get SLC field (ID=008) in tagged image record. */
      if(!lookup_ANSI_NIST_field(&field, &field_i, SLC_ID, imgrecord)){
         fprintf(stderr, "ERROR : get_pix_per_mm : "
         "SLC field [Type-%u.%03d] not found\n",
         imgrecord->type, SLC_ID);
         return(-3);
      }
      slc = (int)strtol((char *)field->subfields[0]->items[0]->value, &nep, 10);
      if (nep == (char *)field->subfields[0]->items[0]->value
      || *nep != '\0') {
     fprintf(stderr, "ERROR : get_pix_per_mm : "
         "SLC field [Type-%u.%03d] : invalid integer value '%s'\n",
         imgrecord->type, SLC_ID,
         (char *)field->subfields[0]->items[0]->value);
     return(-34);
      }
      /* If not scale specified ... then, can't plot minutiae. */
      if(slc == 0){
         fprintf(stderr, "WARNING : get_pix_per_mm : "
         "SLC field [Type-%u.%03d] = 0, "
         "so scanning frequency not available\n",
         imgrecord->type, SLC_ID);
         return(IGNORE);
      }

      /* Get HPS field in tagged image record. */
      if(!lookup_ANSI_NIST_field(&field, &field_i, HPS_ID, imgrecord)){
         fprintf(stderr, "ERROR : get_pix_per_mm : "
         "HPS field [Type-%u.%03d] not found\n",
                 imgrecord->type, HPS_ID);
         return(-4);
      }
      hps = (int)strtol((char *)field->subfields[0]->items[0]->value, &nep, 10);
      if (nep == (char *)field->subfields[0]->items[0]->value
      || *nep != '\0') {
     fprintf(stderr, "ERROR : get_pix_per_mm : "
         "HPS field [Type-%u.%03d] : invalid integer value '%s'\n",
         imgrecord->type, HPS_ID,
         (char *)field->subfields[0]->items[0]->value);
     return(-45);
      }

      /* Get VPS field in tagged image record. */
      if(!lookup_ANSI_NIST_field(&field, &field_i, VPS_ID, imgrecord)){
         fprintf(stderr, "ERROR : get_pix_per_mm : "
         "VPS field [Type-%u.%03d] not found\n",
                 imgrecord->type, VPS_ID);
         return(-5);
      }
      vps = (int)strtol((char *)field->subfields[0]->items[0]->value, &nep, 10);
      if (nep == (char *)field->subfields[0]->items[0]->value
      || *nep != '\0') {
     fprintf(stderr, "ERROR : get_pix_per_mm : "
         "VPS field [Type-%u.%03d] : invalid integer value '%s'\n",
         imgrecord->type, VPS_ID,
         (char *)field->subfields[0]->items[0]->value);
     return(-56);
      }

      /* If hps and vps are pix/inch ... */
      if(slc == 1){
         /* Compute pix/mm. */
         hpix_per_mm = hps / MM_PER_INCH;
         vpix_per_mm = vps / MM_PER_INCH;
      }
      /* If hps and vps are pix/cm ... */
      else if(slc == 2){
         /* Compute pix/mm. */
         hpix_per_mm = hps / 10.0;
         vpix_per_mm = vps / 10.0;
      }
      else{
         fprintf(stderr, "ERROR : get_pix_per_mm : SLC field ");
         fprintf(stderr, "[Type-%u.%03d] has illegal value, ",
                 imgrecord->type, SLC_ID);
         fprintf(stderr, "%d != {0,1,2}\n", slc);
         return(-6);
      }
   }
   else{
      fprintf(stderr, "ERROR : get_pix_per_mm : ");
      fprintf(stderr, "record Type-%u not supported\n",
              imgrecord->type);
      return(-7);
   }

   /* Assign output pointers. */
   *ohpix_per_mm = hpix_per_mm;
   *ovpix_per_mm = vpix_per_mm;

   /* Return normally. */
   return(0);
}

/***************************************************************/
int An2kLoader::find_matching_minutiae(RECORD **ominrecord, int *ominrecord_i,
                  const int imgrecord_i, const ANSI_NIST *ansi_nist)
{
   RECORD *imgrecord, *record;
   int record_i;
   int img_idc, t9_idc;
   char *valstr, *nep;

   imgrecord = ansi_nist->records[imgrecord_i];

   /* Get image record's IDC. */
   if(imgrecord->fields[IDC_ID-1]->field_int != IDC_ID){
      fprintf(stderr, "ERROR : find_matching_minutiae : "
          "IDC field [Type-%u.%03d] not found\n", imgrecord->type, IDC_ID);
      return(-2);
   }
   valstr = (char *)imgrecord->fields[IDC_ID-1]->subfields[0]->items[0]->value;
   img_idc = (int)strtol(valstr, &nep, 10);
   if (nep == valstr || *nep != '\0') {
      fprintf(stderr, "ERROR : find_matching_minutiae : "
          "IDC field [Type-%u.%03d] : invalid integer value '%s'\n",
          imgrecord->type, IDC_ID, valstr);
      return(-23);
   }

   /* Search for 1st Type-9 record with matching IDC. */
   for(record_i = 1; record_i < ansi_nist->num_records; record_i++){
      /* If we are not on the input image record ... */
      if(record_i != imgrecord_i){
         record = ansi_nist->records[record_i];
         if(record->type == TYPE_9_ID){
            /* Get Type-9 record's IDC. */
            if(record->fields[IDC_ID-1]->field_int != IDC_ID){
               fprintf(stderr, "ERROR : find_matching_minutiae : "
               "IDC field [Type-%u.%03d] not found\n",
                       record->type, IDC_ID);
               return(-3);
            }
        valstr =
           (char *)record->fields[IDC_ID-1]->subfields[0]->items[0]->value;
        t9_idc = (int)strtol(valstr, &nep, 10);
        if (nep == valstr || *nep != '\0') {
           fprintf(stderr, "ERROR : find_matching_minutiae : "
               "IDC field [Type-%u.%03d] : invalid integer value '%s'.\n",
               record->type, IDC_ID, valstr);
           return(-34);
        }

            /* Do we have an IDC match? */
            if(img_idc == t9_idc){
               *ominrecord = record;
               *ominrecord_i = record_i;
               return(TRUE);
            } /* Else, IDCs don't match. */
         } /* Else, record not Type-9. */
      } /* Else, on input record. */
   } /* END For ... */

   /* If we get here, then no matching minutiae found ... */
   return(FALSE);
}

/***************************************************************/
int An2kLoader::get_minutiae_pixel_coords(int **oxs, int **oys, int *onpts,
                 const float hpix_per_mm, const float vpix_per_mm,
                 const int iw, const int ih,
                 const int minrecord_i, const ANSI_NIST *ansi_nist)
{
   int ret;

   if(nist_flag){
      if((ret = get_nist_minutiae_pixel_coords(oxs, oys, onpts,
                      hpix_per_mm, vpix_per_mm,
                      iw, ih, minrecord_i, ansi_nist)))
         return(ret);
   }
   else if (iafis_flag){
      if((ret = get_iafis_minutiae_pixel_coords(oxs, oys, onpts,
                      hpix_per_mm, vpix_per_mm, minrecord_i, ansi_nist)))
         return(ret);
   }
   else{
      fprintf(stderr, "ERROR : get_minutiae_pixel_coords : ");
      fprintf(stderr, "neither NIST or IAFIS field flags set\n");
      return(-2);
   }

   return(0);
}

/***************************************************************/
int An2kLoader::get_nist_minutiae_pixel_coords(int **oxs, int **oys, int *onpts,
                 const float hpix_per_mm, const float vpix_per_mm,
                 const int /*iw*/, const int ih,
                 const int minrecord_i, const ANSI_NIST *ansi_nist)
{
   RECORD *minrecord;
   FIELD *field;
   ITEM *item;
   int field_i, subfield_i;
   int *xs, *ys, npts, micro_mm;
   unsigned char c;
   char *valstr, *nep;

   minrecord = ansi_nist->records[minrecord_i];

   /* Find minutiae count field ... */
   if(!lookup_ANSI_NIST_field(&field, &field_i, MIN_ID, minrecord)){
      fprintf(stderr, "ERROR : get_nist_minutiae_pixel_coords : "
          "MIN field in record index [%d] [Type-%u.%03d] not found\n",
              minrecord_i+1, minrecord->type, MIN_ID);
      return(-2);
   }
   valstr = (char *)field->subfields[0]->items[0]->value;
   npts = (int)strtol(valstr, &nep, 10);
   if (nep == valstr || *nep != '\0') {
      fprintf(stderr, "ERROR : get_nist_minutiae_pixel_coord : "
          "IDC field in record index [%d] [Type-%u.%03d] : "
          "invalid integer value '%s'.\n",
          minrecord_i, minrecord->type, MIN_ID, valstr);
      return(-20);
   }


   /* !!!! No minutiea test added 09-13-04 !!! */
   /* If no minutiea recorded in Type-9 record ... */
   if(npts == 0){
      *oxs = (int *)NULL;
      *oys = (int *)NULL;
      *onpts = npts;
      return(0);
   }

   xs = (int *)malloc(npts * sizeof(int));
   if(xs == (int *)NULL){
      fprintf(stderr,
              "ERROR : get_nist_minutiae_pixel_coords : malloc : xs\n");
      return(-3);
   }
   ys = (int *)malloc(npts * sizeof(int));
   if(ys == (int *)NULL){
      fprintf(stderr,
              "ERROR : get_nist_minutiae_pixel_coords : malloc : ys\n");
      free(xs);
      return(-4);
   }
   /* Find minutiae attributes field ... */
   if(!lookup_ANSI_NIST_field(&field, &field_i, MRC_ID, minrecord)){
      fprintf(stderr, "ERROR : get_nist_minutiae_pixel_coords : ");
      fprintf(stderr, "MRC field in record index [%d] [Type-%u.%03d] ",
              minrecord_i+1, minrecord->type, MRC_ID);
      fprintf(stderr, "not found\n");
      free(xs);
      free(ys);
      return(-5);
   }
   /* If wrong number of subfields ... */
   if(field->num_subfields != npts){
      fprintf(stderr, "ERROR : get_nist_minutiae_pixel_coords : ");
      fprintf(stderr, "number of subfields %d != %d ",
              field->num_subfields, npts);
      fprintf(stderr, "in MRC field index [%d.%d] [Type-%u.%03d]\n",
              minrecord_i+1, field_i+1, minrecord->type, MRC_ID);
      free(xs);
      free(ys);
      return(-6);
   }

   /* For each subfield in MRC field ... */
   for(subfield_i = 0; subfield_i < npts; subfield_i++){
      /* Set location/direction item. */
      item = field->subfields[subfield_i]->items[1];
      /* If location/direction wrong length ... */
      if(strlen((char *)item->value) != 11){
         fprintf(stderr, "ERROR : get_nist_minutiae_pixel_coords : "
         "location/direction value \"%s\" has wrong length in "
         "MRC subfield index [%d.%d.%d] [Type-%u.%03d]\n",
         (char *)item->value, minrecord_i+1, field_i+1, subfield_i+1,
         minrecord->type, MRC_ID);
         free(xs);
         free(ys);
         return(-7);
      }
      /* Get x-coord. */
      c = item->value[4];
      item->value[4] = '\0';
      micro_mm = (int)strtol((char *)item->value, &nep, 10);
      if (nep == (char *)item->value || *nep != '\0') {
     fprintf(stderr, "ERROR : get_nist_minutiae_pixel_coords : "
         "MRC subfield [%d.%d.%d] [Type-%u.%03d] : "
         "x-coord is not a valid integer value '%s'\n.",
         minrecord_i+1, field_i+1, subfield_i+1,
         minrecord->type, MRC_ID, (char *)item->value);
     return(-78);
      }
      xs[subfield_i] = sround(micro_mm * hpix_per_mm / 100.0);
      item->value[4] = c;
      /* Get y-coord. */
      c = item->value[8];
      item->value[8] = '\0';

      micro_mm = (int)strtol((char *)&(item->value[4]), &nep, 10);
      if (nep == (char *)item->value || *nep != '\0') {
     fprintf(stderr, "ERROR : get_nist_minutiae_pixel_coords : "
         "MRC subfield [%d.%d.%d] [Type-%u.%03d] : "
         "y-coord is not a valid integer value '%s'\n.",
         minrecord_i+1, field_i+1, subfield_i+1,
         minrecord->type, MRC_ID, (char *)item->value);
     return(-79);
      }
      ys[subfield_i] = sround(micro_mm * vpix_per_mm / 100.0);
      /* Flip y-coord. */
      ys[subfield_i] = ih - ys[subfield_i] - 1;
      item->value[8] = c;
   }

   /* Set output pointers. */
   *oxs = xs;
   *oys = ys;
   *onpts = npts;

   /* Return normally. */
   return(0);
}

/***************************************************************/
int An2kLoader::get_iafis_minutiae_pixel_coords(int **oxs, int **oys, int *onpts,
                 const float hpix_per_mm, const float vpix_per_mm,
                 const int minrecord_i, const ANSI_NIST *ansi_nist)
{
   int ret;
   RECORD *minrecord;
   FIELD *field;
   ITEM *item;
   int field_i, subfield_i;
   int *xs, *ys, npts, micro_mm;
   char c;

   minrecord = ansi_nist->records[minrecord_i];

   /* Find minutiae COF field ... */
   if(lookup_ANSI_NIST_field(&field, &field_i, COF_ID, minrecord)){
      ret = is_COF_zero(field);
      /* If ERROR ... */
      if(ret < 0)
         return(ret);
      /* IF COF field not zero ... */
      if(!ret){
         fprintf(stderr, "WARNING : get_iafis_minutiae_pixel_coords : ");
         fprintf(stderr, "COF field non-zero in record index ");
         fprintf(stderr, "[%d] [Type-%d.%03d], so minutiae ignored\n",
                 minrecord_i+1, minrecord->type, COF_ID);
         *onpts = 0;
         /* Return normally. */
         return(0);
      }
   }

   /* Find minutiae count field ... */
   if(!lookup_ANSI_NIST_field(&field, &field_i, NMN_ID, minrecord)){
      fprintf(stderr, "ERROR : get_iafis_minutiae_pixel_coords : ");
      fprintf(stderr, "NMN field in record index [%d] [Type-%d.%03d] ",
              minrecord_i+1, minrecord->type, NMN_ID);
      fprintf(stderr, "not found\n");
      return(-2);
   }
   npts = atoi((char *)field->subfields[0]->items[0]->value);
   xs = (int *)malloc(npts * sizeof(int));
   if(xs == (int *)NULL){
      fprintf(stderr,
              "ERROR : get_iafis_minutiae_pixel_coords : malloc : xs\n");
      return(-3);
   }
   ys = (int *)malloc(npts * sizeof(int));
   if(ys == (int *)NULL){
      fprintf(stderr,
              "ERROR : get_iafis_minutiae_pixel_coords : malloc : ys\n");
      free(xs);
      return(-4);
   }
   /* Find minutiae attributes field ... */
   if(!lookup_ANSI_NIST_field(&field, &field_i, MAT_ID, minrecord)){
      fprintf(stderr, "ERROR : get_iafis_minutiae_pixel_coords : ");
      fprintf(stderr, "MAT field in record index [%d] [Type-%d.%03d] ",
              minrecord_i+1, minrecord->type, MAT_ID);
      fprintf(stderr, "not found\n");
      free(xs);
      free(ys);
      return(-5);
   }
   /* If wrong number of subfields ... */
   if(field->num_subfields != npts){
      fprintf(stderr, "ERROR : get_iafis_minutiae_pixel_coords : ");
      fprintf(stderr, "number of subfields %d != %d ",
              field->num_subfields, npts);
      fprintf(stderr, "in MAT field index [%d.%d] [Type-%d.%03d]\n",
              minrecord_i+1, field_i+1, minrecord->type, MAT_ID);
      free(xs);
      free(ys);
      return(-6);
   }

   /* For each subfield in MAT field ... */
   for(subfield_i = 0; subfield_i < npts; subfield_i++){
      /* Set location/direction item. */
      item = field->subfields[subfield_i]->items[1];
      /* If location/direction wrong length ... */
      if(strlen((char *)item->value) != 11){
         fprintf(stderr, "ERROR : get_iafis_minutiae_pixel_coords : ");
         fprintf(stderr, "location/direction value \"%s\" has ", item->value);
         fprintf(stderr, "wrong length in MAT subfield ");
         fprintf(stderr, "index [%d.%d.%d] [Type-%d.%03d]\n",
                          minrecord_i+1, field_i+1, subfield_i+1,
                          minrecord->type, MAT_ID);
         free(xs);
         free(ys);
         return(-7);
      }
      /* Get x-coord. */
      c = item->value[4];
      item->value[4] = '\0';
      micro_mm = atoi((char *)item->value);
      xs[subfield_i] = sround(micro_mm * hpix_per_mm / 100.0);
      item->value[4] = c;
      /* Get y-coord. */
      c = item->value[8];
      item->value[8] = '\0';
      micro_mm = atoi((char *)&(item->value[4]));
      ys[subfield_i] = sround(micro_mm * vpix_per_mm / 100.0);
      item->value[8] = c;
   }

   /* Set output pointers. */
   *oxs = xs;
   *oys = ys;
   *onpts = npts;

   /* Return normally. */
   return(0);
}

/***************************************************************/
int An2kLoader::is_COF_zero(FIELD *coffield)
{
   ITEM *item;

   if((coffield->num_subfields != 1) ||
      (coffield->subfields[0]->num_items < 1)){
      fprintf(stderr, "ERROR : is_COF_zero : field format error\n");
      return(-2);
   }
   else{
      item = coffield->subfields[0]->items[0];
      if(strcmp((char *)item->value, "00000000") != 0)
         return(FALSE);
   }

   if(coffield->subfields[0]->num_items >= 2){
      item = coffield->subfields[0]->items[1];
      if(strcmp((char *)item->value, "00000000") != 0)
         return(FALSE);
   }

   if(coffield->subfields[0]->num_items >= 3){
      item = coffield->subfields[0]->items[2];
      if(strcmp((char *)item->value, "000.0000") != 0)
         return(FALSE);
   }

   if(coffield->subfields[0]->num_items >= 4){
      item = coffield->subfields[0]->items[3];
      if(strcmp((char *)item->value, "00000000") != 0)
         return(FALSE);
   }

   if(coffield->subfields[0]->num_items == 5){
      item = coffield->subfields[0]->items[4];
      if(strcmp((char *)item->value, "00000000") != 0)
         return(FALSE);
   }

   return(TRUE);
}

/***************************************************************/
int An2kLoader::get_segmentation_data(const RECORD *const imgrecord,
                 SEGMENTS **segments)
{
   FIELD *sfield, *afield;
   SUBFIELD *subf;
   int num_polygons = 0, space_needed = 0;
   int sfield_i, afield_i, subf_i, item_i, *next;
   SEGMENTS *seg;
   POLYGON *pol;

   /* First determine how much space is needed. */
   if (lookup_ANSI_NIST_field(&sfield, &sfield_i, SEG_ID, imgrecord)) {
      num_polygons += sfield->num_subfields;
      space_needed +=
     (sfield->num_subfields * (sizeof(POLYGON) + 2 * 4 * sizeof(int)));
   } else {
      sfield = NULL;
   }

   if (lookup_ANSI_NIST_field(&afield, &afield_i, ASEG_ID, imgrecord)) {
      num_polygons += afield->num_subfields;
      space_needed += (afield->num_subfields * sizeof(POLYGON));
      for (subf_i = 0; subf_i < afield->num_subfields; subf_i++) {
     subf = afield->subfields[subf_i];
     space_needed += ((subf->num_items - 1) * sizeof(int));
      }
   } else {
      afield = NULL;
   }

   /* If no segmentation was found, return empty-handed. */
   if (0 == space_needed) {
      *segments = NULL;
      return IGNORE;
   }

   /* Allocate the space. */
   space_needed += sizeof(SEGMENTS);
   if (NULL == (seg = (SEGMENTS*)malloc(space_needed))) {
      fprintf(stderr, "ERROR : get_segmentation_data : "
          "cannot allocate %d bytes for segmentation data\n",
          space_needed);
      return -1;
   }

   /* Initial initialization. */
   seg->num_polygons = 0;
   seg->polygons = (POLYGON*)(seg + 1);
   next = (int*)(seg->polygons + num_polygons);

   /* Retrieve the data. */
   if (NULL != sfield) {
      for (subf_i = 0; subf_i < sfield->num_subfields; subf_i++) {
     subf = sfield->subfields[subf_i];

     pol = seg->polygons + seg->num_polygons++;
     pol->num_points = 4;
     pol->x = next;
     next += pol->num_points;
     pol->y = next;
     next += pol->num_points;

     if (subf->num_items != 5) {
        fprintf(stderr, "ERROR : get_segmentation_data : "
            "expected 5 items, found %d\n", subf->num_items);
        return -2;
     }
     pol->fgp = atoi((char *)subf->items[0]->value);
     pol->x[0] = pol->x[3] = atoi((char *)subf->items[1]->value);
     pol->x[1] = pol->x[2] = atoi((char *)subf->items[2]->value);
     pol->y[0] = pol->y[1] = atoi((char *)subf->items[3]->value);
     pol->y[2] = pol->y[3] = atoi((char *)subf->items[4]->value);
      }
   }
   if (NULL != afield) {
      for (subf_i = 0; subf_i < afield->num_subfields; subf_i++) {
     subf = afield->subfields[subf_i];

     pol = seg->polygons + seg->num_polygons++;
     pol->fgp = atoi((char *)subf->items[0]->value);
     pol->num_points = atoi((char *)subf->items[1]->value);
     pol->x = next;
     next += pol->num_points;
     pol->y = next;
     next += pol->num_points;

     for (item_i = 2; item_i < subf->num_items; item_i++) {
        if (0 == (item_i % 2))    /* even */
           pol->x[(item_i-2)/2] = atoi((char *)subf->items[item_i]->value);
        else		      /* odd */
           pol->y[(item_i-2)/2] = atoi((char *)subf->items[item_i]->value);
     }
      }
   }

   *segments = seg;
   return 0;
}

int An2kLoader::dpyimagepts(char *fname, unsigned char *data, unsigned int image_w, unsigned int image_h, unsigned int d, unsigned int whitepix, int align, int *done, int *xs, int *ys, int npts, const SEGMENTS * const segs)
{
    /* Open a new MainWindow, show the image, add the minutiae */
}
