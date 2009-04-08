#ifndef __as3_common_h__
#define __as3_common_h__


extern int as3_verbosity;
extern int as3_pass;
#define syntaxerror as3_error
void as3_error(const char*format, ...) __attribute__ ((__noreturn__));
void as3_warning(const char*format, ...);
void as3_softwarning(const char*format, ...);
#endif
