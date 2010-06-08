#ifndef __bbox_h__
#define __bbox_h__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ibbox {
    int xmin,ymin,xmax,ymax;
    struct _ibbox*next;
} ibbox_t;

ibbox_t ibbox_clip(ibbox_t* outer, ibbox_t* inner);

ibbox_t* ibbox_new(int x1, int y1, int x2, int y2, int rowsize);
void ibbox_destroy(ibbox_t*b);
ibbox_t*get_bitmap_bboxes(unsigned char*alpha, int width, int height, int rowsize);

#ifdef __cplusplus
}
#endif

#endif //__bbox_h__
