#ifndef __bbox_h__
#define __bbox_h__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ibbox {
    int xmin,ymin,xmax,ymax;
    struct _ibbox*next;
} ibbox_t;

ibbox_t* ibbox_new(int x1, int y1, int x2, int y2);
void ibbox_destroy(ibbox_t*b);
ibbox_t*get_bitmap_bboxes(unsigned char*alpha, int width, int height);

#ifdef __cplusplus
}
#endif

#endif //__bbox_h__
