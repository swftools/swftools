/* Define if you have alloca, as a function or macro.  */
#define HAVE_ALLOCA 1

/* Define if you have <alloca.h> and it should be used (not on Ultrix).  */
#define HAVE_ALLOCA_H 1

/* Define if the `long double' type works.  */
#define HAVE_LONG_DOUBLE 1

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
#define TIME_WITH_SYS_TIME 1

/* Define if your processor stores words with the most significant
   byte first (like Motorola and SPARC, unlike Intel and VAX).  */
/* #undef WORDS_BIGENDIAN */

/* #undef  int8_t */
#define uint8_t unsigned char
/* #undef  int18_t */
#define uint16_t unsigned short
/* #undef  int32_t */
#define uint32_t unsigned int
/* #undef  int64_t */
#define uint64_t unsigned long long
#define ieee854_float80_t long double
#define ieee754_float64_t double
#define ieee754_float32_t float

/* The number of bytes in a double.  */
#define SIZEOF_DOUBLE 8

/* The number of bytes in a float.  */
#define SIZEOF_FLOAT 4

/* The number of bytes in a int.  */
#define SIZEOF_INT 4

/* The number of bytes in a long.  */
#define SIZEOF_LONG 4

/* The number of bytes in a long double.  */
/* #undef SIZEOF_LONG_DOUBLE */

/* The number of bytes in a long long.  */
#define SIZEOF_LONG_LONG 8

/* The number of bytes in a short.  */
#define SIZEOF_SHORT 2

/* The number of bytes in a unsigned int.  */
#define SIZEOF_UNSIGNED_INT 4

/* The number of bytes in a unsigned long.  */
#define SIZEOF_UNSIGNED_LONG 4

/* The number of bytes in a unsigned long long.  */
#define SIZEOF_UNSIGNED_LONG_LONG 8

/* The number of bytes in a unsigned short.  */
#define SIZEOF_UNSIGNED_SHORT 2

/* Define if you have the <errno.h> header file.  */
#define HAVE_ERRNO_H 1

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <limits.h> header file.  */
#define HAVE_LIMITS_H 1

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <termcap.h> header file.  */
#define HAVE_TERMCAP_H 1

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Name of package */
#define PACKAGE "lame"

/* Version number of package */
#define VERSION "3.91"

/* Define if compiler has function prototypes */
#define PROTOTYPES 1

/* system has 80 bit floats */
#define HAVE_IEEE854_FLOAT80 1

/* requested by Frank, seems to be temporary needed for a smooth transition */
#define LAME_LIBRARY_BUILD 1

/* have working GTK */
#define HAVE_GTK 1

/* build with layer 1 decoding */
#define USE_LAYER_1 1

/* build with layer 2 decoding */
#define USE_LAYER_2 1

/* enable VBR bitrate histogram */
#define BRHIST 1

/* have termcap */
#define HAVE_TERMCAP 1

/* IEEE754 compatible machine */
#define TAKEHIRO_IEEE754_HACK 1

/* no debug build */
#define NDEBUG 1

