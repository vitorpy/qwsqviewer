#ifndef AN2KLOADER_H
#define AN2KLOADER_H

#include <QRunnable>
#include "dpyan2k.h"
class Canvas;

class An2kLoader : public QRunnable
{
public:
    An2kLoader(QString file, Canvas* canvas);

    virtual void run();
private:
    Canvas* _canvas;
    QString _file;
    int nist_flag;
    int iafis_flag;

    int dpyan2k_record(const int imgrecord_i, const ANSI_NIST *ansi_nist);
    int get_pix_per_mm(float *ohpix_per_mm, float *ovpix_per_mm, const int imgrecord_i, const ANSI_NIST *ansi_nist);
    int find_matching_minutiae(RECORD **ominrecord, int *ominrecord_i,
                      const int imgrecord_i, const ANSI_NIST *ansi_nist);
    int get_minutiae_pixel_coords(int **oxs, int **oys, int *onpts,
                     const float hpix_per_mm, const float vpix_per_mm,
                     const int iw, const int ih,
                     const int minrecord_i, const ANSI_NIST *ansi_nist);
    int get_nist_minutiae_pixel_coords(int **oxs, int **oys, int *onpts,
                     const float hpix_per_mm, const float vpix_per_mm,
                     const int iw, const int ih,
                     const int minrecord_i, const ANSI_NIST *ansi_nist);
    int get_iafis_minutiae_pixel_coords(int **oxs, int **oys, int *onpts,
                     const float hpix_per_mm, const float vpix_per_mm,
                     const int minrecord_i, const ANSI_NIST *ansi_nist);
    int is_COF_zero(FIELD *coffield);
    int get_segmentation_data(const RECORD *const imgrecord,
                     SEGMENTS **segments);
    int dpyimagepts(char *fname, unsigned char *data,
             unsigned int image_w, unsigned int image_h,
                     unsigned int d, unsigned int whitepix, int align, int *done,
             int *xs, int *ys, int npts, const SEGMENTS *const segs);

};

#endif // AN2KLOADER_H
