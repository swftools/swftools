#ifndef __utf8_h__
#define __utf8_h__

#ifdef __cplusplus
extern "C" {
#endif

int writeUTF8(unsigned int charnum, char*dest);
char* getUTF8(unsigned int charnum);

#ifdef __cplusplus
}
#endif

#endif
