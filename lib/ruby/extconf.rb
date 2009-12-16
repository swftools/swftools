require 'mkmf'
require 'rbconfig'

$LDFLAGS << " -L.. "
$LIBS << " -lstdc++ "

dir_config 'gfx'

have_library("z", "deflate")
have_library("m", "sqrt")
have_library("jpeg", "jpeg_write_raw_data")
have_library("fftw3", "fftw_plan_dft_r2c_2d")
have_library("freetype", "FT_Init_FreeType")
have_library("fontconfig", "FcInit")
have_library("base", "ringbuffer_init")
have_library("gfx", "gfxfontlist_create")
have_library("gfxpdf", "gfxsource_pdf_create")
have_library("rfxswf", "swf_ReadSWF")
have_library("gfxswf", "gfxdevice_swf_init")

create_makefile('gfx')
