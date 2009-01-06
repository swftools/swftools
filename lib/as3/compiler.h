#ifndef __as3_compiler_h__
#define __as3_compiler_h__

void as3_setverbosity(int level);
void as3_add_include_dir(char*dir);
void as3_parse_file(char*filename);
void as3_warning(const char*format, ...);
char* as3_getglobalclass();
void* as3_getcode();
void as3_destroy();

#endif //__as3_compiler_h__
