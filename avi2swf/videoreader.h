#ifndef __videoreader_t__
#define __videoreader_t__

#define videoreader_getsamples(v, buffer, num) ((v)->getsamples((v),(buffer),(num)))
#define videoreader_getimage(v, buffer) ((v)->getimage((v),(buffer)))
#define videoreader_eof(v) ((v)->eof(v))
#define videoreader_setparameter(v,name,value) ((v)->setparameter((v),(name),(value)))
#define videoreader_close(v) ((v)->close(v))

typedef struct _videoreader_t
{
    void*internal;

    /* video */
    int width;
    int height;
    double fps;

    /* audio */
    int channels;
    int samplerate;

    /* progress */
    int frame;

    void (*setparameter) (struct _videoreader_t*, char*name, char*value);
    int (*getsamples) (struct _videoreader_t*, void*buffer, int num);
    int (*getimage) (struct _videoreader_t*, void*buffer); // buffer must hold width*height*4 bytes
    void (*close) (struct _videoreader_t*);

} videoreader_t;


#endif
