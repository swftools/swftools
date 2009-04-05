#ifndef __as3_compiler_h__
#define __as3_compiler_h__

void as3_setverbosity(int level);
void as3_add_include_dir(char*dir);

void as3_set_define(const char*c);
void as3_set_option(const char*key, const char*value);

void as3_parse_file(const char*filename);
void as3_parse_bytearray(const char*name, const void*mem, int length);
void as3_parse_directory(const char*dir);

char as3_schedule_directory(const char*dir);
void as3_schedule_package(const char*package);
void as3_schedule_class(const char*package, const char*cls);
void as3_schedule_class_noerror(const char*package, const char*cls);

void as3_warning(const char*format, ...);
char* as3_getglobalclass();
void* as3_getcode();
void as3_destroy();

#endif //__as3_compiler_h__
