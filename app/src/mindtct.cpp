#include <stdio.h>
#include <sys/param.h>

#include <QImage>

#include <an2k.h>
#include <lfs.h>
#include <imgdecod.h>
#include <imgboost.h>
#include <img_io.h>
#include <version.h>

#include "mindtct.h"
#include "canvas.h"

//void procargs(int, char **, int *, int *, char **, char **);

/*int debug = 0;*/
extern int debug;

MinutiaeDetect::MinutiaeDetect(Canvas* canvas)
{
    _canvas = canvas;
    //_file = file;
    //nist_flag = 1;
    //iafis_flag = 0;
}

void MinutiaeDetect::run()
{
    /*int boostflag, m1flag;
    char *ifile, *oroot, ofile[MAXPATHLEN];*/
    unsigned char *idata, *bdata;
    /*int img_type;*/
    int /*ilen,*/ iw, ih, id, ippi, bw, bh, bd;
    double ippmm;
    /*int img_idc, img_imp;*/
    int *direction_map, *low_contrast_map, *low_flow_map;
    int *high_curve_map, *quality_map;
    int map_w, map_h;
    int ret;
    MINUTIAE *minutiae;
    /*ANSI_NIST *ansi_nist;
    RECORD *imgrecord;*/
    /*int imgrecord_i;*/

#if 0

    /* 1. READ FINGERPRINT IMAGE FROM FILE INTO MEMORY. */

    /* Is input file in ANSI/NIST format? */
    if((ret = is_ANSI_NIST_file(ifile)) < 0) {
       /* If error ... */
       exit(ret);
    }

    /* If file is ANSI/NIST format ... */
    if(ret){
       img_type = ANSI_NIST_IMG;
       /* Parse ANSI/NIST file into memory structure */
       if((ret = read_ANSI_NIST_file(ifile, &ansi_nist)))
          exit(ret);
       /* Get first grayscale fingerprint record in ANSI/NIST file. */
       if((ret = get_first_grayprint(&idata, &iw, &ih, &id,
                                 &ippmm, &img_idc, &img_imp,
                                 &imgrecord, &imgrecord_i, ansi_nist)) < 0){
          /* If error ... */
          free_ANSI_NIST(ansi_nist);
          exit(ret);
       }
       /* If grayscale fingerprint not found ... */
       if(!ret){
          free_ANSI_NIST(ansi_nist);
          fprintf(stderr, "ERROR : main : ");
          fprintf(stderr, "grayscale image record not found in %s\n", ifile);
          exit(-2);
       }
    }
    /* Otherwise, not an ANSI/NIST file */
    else{
       /* Read the image data from file into memory */
       if((ret = read_and_decode_grayscale_image(ifile, &img_type,
                     &idata, &ilen, &iw, &ih, &id, &ippi))){
          exit(ret);
       }
       /* If image ppi not defined, then assume 500 */
       if(ippi == UNDEFINED)
          ippmm = DEFAULT_PPI / (double)MM_PER_INCH;
       else
          ippmm = ippi / (double)MM_PER_INCH;
    }

    /* 2. ENHANCE IMAGE CONTRAST IF REQUESTED */
    if(boostflag)
       trim_histtails_contrast_boost(idata, iw, ih);
#endif

    QImage image = _canvas->image();
    ippi = _canvas->ppi();
    ippmm = ippi / (double)MM_PER_INCH;
    idata = const_cast<uchar *>(image.constBits());
    iw = image.width();
    ih = image.height();
    id = 8;

    /* 3. GET MINUTIAE & BINARIZED IMAGE. */
    if((ret = get_minutiae(&minutiae, &quality_map, &direction_map,
                          &low_contrast_map, &low_flow_map, &high_curve_map,
                          &map_w, &map_h, &bdata, &bw, &bh, &bd,
                          idata, iw, ih, id, ippmm, &lfsparms_V2))){
#if 0
       if(img_type == ANSI_NIST_IMG)
          free_ANSI_NIST(ansi_nist);
       free(idata);
#endif
       return;
    }

    for (int i = 0; i < minutiae->num; ++i)
    {
        auto m = minutiae->list[i];
        _canvas->addMinutiae(Minutiae(m->x, m->y));
    }

#if 0
    /* Done with input image data */
    free(idata);

    /* 4. WRITE MINUTIAE & MAP RESULTS TO TEXT FILES */
    if((ret = write_text_results(oroot, m1flag, bw, bh,
                                minutiae, quality_map,
                                direction_map, low_contrast_map,
                                low_flow_map, high_curve_map, map_w, map_h))){
       if(img_type == ANSI_NIST_IMG)
          free_ANSI_NIST(ansi_nist);
       free_minutiae(minutiae);
       free(quality_map);
       free(direction_map);
       free(low_contrast_map);
       free(low_flow_map);
       free(high_curve_map);
       free(bdata);
       exit(ret);
    }

    /* Done with minutiae detection maps. */
    free(quality_map);
    free(direction_map);
    free(low_contrast_map);
    free(low_flow_map);
    free(high_curve_map);

    /* 5. WRITE ADDITIONAL RESULTS */

    /* If input is ANSI/NIST ... */
    if(img_type == ANSI_NIST_IMG){

       /* Update ansi/nist structure with results. */
       if((ret = update_ANSI_NIST_lfs_results(ansi_nist, minutiae,
                                             bdata, bw, bh, bd,
                                             ippmm, img_idc, img_imp))){
          if(img_type == ANSI_NIST_IMG)
             free_ANSI_NIST(ansi_nist);
          free_minutiae(minutiae);
          free(bdata);
          exit(ret);
       }

       /* Write updated ANSI/NIST structure to output file */
       sprintf(ofile, "%s.%s", oroot, AN2K_OUT_EXT);
       if((ret = write_ANSI_NIST_file(ofile, ansi_nist))){
          if(img_type == ANSI_NIST_IMG)
             free_ANSI_NIST(ansi_nist);
          free_minutiae(minutiae);
          free(bdata);
          exit(ret);
       }

       /* Done with ANSI/NIST structure */
       free_ANSI_NIST(ansi_nist);
    }
    /* Otherwise, input image is not ANSI/NIST ... */
    else{
       sprintf(ofile, "%s.%s", oroot, BINARY_IMG_EXT);
       if((ret = write_raw_from_memsize(ofile, bdata, bw*bh))){
          free_minutiae(minutiae);
          free(bdata);
          exit(ret);
       }
    }

    /* Done with minutiae and binary image results */
    free_minutiae(minutiae);
    free(bdata);

    /* Exit normally. */
    /* exit(0); */

#endif
}
