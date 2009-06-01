#!/usr/bin/env python

from distutils import ccompiler
from distutils.core import CompileError

try:
    from setuptools import setup, Extension
except ImportError:
    from distutils.core import setup, Extension

cc = ccompiler.new_compiler()

# leave it to gcc to detect c/c++ files. we'll be linking against -lstdc++
# later on to ensure we do have all the c++ libraries, so we don't need to
# compile *everything* as c++ just because of the few .cc files.
cc.language_map[".cc"] = "c"

import os
import sys
import stat
import re

DATADIR = "/usr/share/swftools/"

def update_dirs(list1, list2):
    for v in list2:
        if v not in list1 and os.path.isdir(v):
            list1.append(v)

def update_list(list1, list2):
    for v in list2:
        if v not in list1:
            list1.append(v)

COMPILER_OPTIONS = ['-fPIC', '-Wparentheses', '-Wimplicit', '-Wreturn-type',
                    '-O3', '-fomit-frame-pointer', '-Winline', 
                    '-Wno-unused', '-Wno-strict-prototypes','-DHAVE_CONFIG_H',
                    '-DSWFTOOLS_DATADIR="'+DATADIR+'"']
INCLUDE=['lib/lame/', 'lib/pdf/xpdf', 'lib/pdf']

if 'library_dirs' in os.environ:
    update_dirs(cc.library_dirs, [path.strip() for path in os.environ.get('library_dirs').strip().split(":")])
else:
    update_dirs(cc.library_dirs, [os.path.join(sys.prefix, 'lib'), 
                                  os.path.join(sys.prefix, 'local/lib')])
    if sys.platform == "darwin":
        update_list(cc.library_dirs, ["/sw/lib", "/opt/local/lib"])

if 'include_dirs' in os.environ:
    update_dirs(cc.include_dirs, [path.strip() for path in os.environ.get('include_dirs').strip().split(":")])
else:
    update_dirs(cc.include_dirs, [os.path.join(sys.prefix, 'include'), 
                                  os.path.join(sys.prefix, 'include/freetype2'),
                                  os.path.join(sys.prefix, 'include/fontconfig')])
    if sys.platform == "darwin":
        update_dirs(cc.include_dirs, ["/sw/include/freetype2", "/sw/lib/freetype2/include", "/sw/include", "/opt/local/include"])


class ConfigScript:
    def __init__(self):
        self.defines = []
        self.includefiles = []
        self.libraries = []

    def unlink(self, filename):
        try:
            os.unlink(filename)
        except OSError: 
            pass

    def test_code(self, head, body, stdh=1):
        fi = open("setup_test.c", "wb")
        if stdh:
            fi.write("#include <stdlib.h>\n")
            fi.write("#include <stdio.h>\n")
            for i in self.includefiles:
                fi.write("#include <%s>\n" % i)
        for d in self.defines:
            fi.write("%s;\n" % d)
        fi.write(head)
        fi.write("\nint main()\n")
        fi.write("{\n")
        fi.write(body)
        fi.write("\n}\n")
        fi.close()
        #fi = open("setup_test.c", "rb")
        #print fi.read()
        #fi.close()

        quiet = 1
        if quiet:
            try:
                # redirect stderr
                sys.stderr.flush()
                err = open('/dev/null', 'a+', 0)
                olderr = open('/dev/null', 'a+', 0)
                os.dup2(2, olderr.fileno())
                os.dup2(err.fileno(), 2)
            except:
                pass

        error = 0
        try:
            cc.compile(["setup_test.c"], include_dirs=cc.include_dirs)
        except CompileError:
            error = 1

        if quiet:
            try:
                # unredirect stderr
                sys.stderr.flush()
                err = open('/dev/null', 'a+', 0)
                os.dup2(olderr.fileno(), 2)
                olderr.close()
                err.close()
            except:
                pass

        self.unlink("setup_test.o")
        self.unlink("setup_test.obj")
        self.unlink("setup_test.c")
        if error:
            return None
        else:
            return 1

    def system_has_property(self,name):
        if name.startswith("STD"):
            return 1
        elif name.startswith("INTERNAL"):
            return 1
        elif name == "PACKAGE":
            return "\"swftools\""
        elif name == "VERSION":
            fi = open("configure.in", "rb")
            for line in fi.readlines():
                if line.startswith("VERSION="):
                    return '"'+line[8:].strip()+'"'
            return "unknown"
        elif "POPPLER" in name:
            return None
        elif name.startswith("HAVE_") and name.endswith("_H"):
            header = name[5:].lower()
            c = header.rfind("_")
            if c>=0:
                header = header[0:c]+"."+header[c+1]
            header = header.replace("_","/")
            ok = 0
            for dir in cc.include_dirs:
                if os.path.isfile(os.path.join(dir,header)):
                    ok = 1
                    break
            if ok and self.test_code("#include <"+header+">", ""):
                if header.startswith("sys"):
                    self.includefiles += [header]
                return 1
        elif name.startswith("HAVE_LIB") \
          or name.startswith("HAVE_FONTCONFIG") \
          or name.startswith("HAVE_FREETYPE"):
            if name.startswith("HAVE_LIB"):
                libname = name[8:].lower()
            else:
                libname = name[5:].lower()
            if cc.find_library_file(cc.library_dirs, libname):
                update_list(self.libraries, [libname])
                return 1
        elif name.startswith("HAVE_LAME"):
            return None
        elif name.startswith("O_BINARY"):
            if sys.platform.startswith("win"):
                return None
            else:
                return 0
        elif name == "boolean":
            if self.test_code("", "boolean b;"):
                return None
            else:
                self.defines += ["typedef int boolean"]
                return "int"
        elif name.startswith("SIZEOF_"):
            t = name[7:].lower().replace("_", " ")
            if t == "voidp":
                t = "void*"
            for i in [1,2,4,8]:
                s = "static int test_array [%d+1-sizeof(%s)*2];\ntest_array [0] = 0;" % (i*2,t)
                if self.test_code("", s):
                    return i
            return None
        elif name.startswith("USE_FREEETYPE"):
            # TODO: run a test here?
            return 1 
        elif name.startswith("HAVE_"):
            function = name[5:].lower()
            params=""
            if function=="bcopy":
                params = "0,0,0"
            elif function=="bzero":
                params = "0,0"
            return self.test_code("", "%s(%s);" % (function,params), stdh=0)
        return None

    def create_config(self):
        print "Creating config.h..."

        fi = open("config.h.in", "rb")
        fo = open("config.h", "wb")
        for line in fi.readlines():
            if line.startswith("#undef "):
                line = line.strip()
                name = line[7:]
                if " " in name:
                    name = name.split(" ")[0]
                value = self.system_has_property(name)
                if value is None:
                    fo.write("// %s is not set\n" % (name))
                else:
                    fo.write("#define %s %s\n" % (name, value))
            else:
                fo.write(line)
        fo.write("// python:lib "+(" ".join(self.libraries))+"\n")
        fo.close()
        fi.close()

    def load(self):
        if not os.path.isfile("config.h"):
            return 0
        fi = open("config.h", "rb")
        for line in fi.readlines():
            if line.startswith("// python: lib "):
                self.libraries = line[15:].split(" ")
        fi.close()
        return

config = ConfigScript()
# if either setup.py or config.h.in were modfied, rebuild config.h
if not config.load() or \
   os.stat("setup.py")[stat.ST_MTIME] > os.stat("config.h")[stat.ST_MTIME] or \
   os.stat("config.h.in")[stat.ST_MTIME] > os.stat("config.h")[stat.ST_MTIME]:
    config.create_config()
#print dir(cc)
#sys.exit(0)

if "jpeg" not in config.libraries:
    print "Error: Couldn't find jpeg library on your system"
    sys.exit(1)

if "freetype" not in config.libraries:
    print "Error: Couldn't find freetype library on your system"
    sys.exit(1)

base_sources = [
"lib/q.c", "lib/utf8.c", "lib/png.c", "lib/jpeg.c", "lib/wav.c", "lib/mp3.c", "lib/os.c", "lib/bitio.c", "lib/log.c", "lib/mem.c",
]
rfxswf_sources = [
"lib/modules/swfaction.c", "lib/modules/swfbits.c", "lib/modules/swfbutton.c",
"lib/modules/swfcgi.c", "lib/modules/swfdraw.c", "lib/modules/swfdump.c", "lib/modules/swffilter.c",
"lib/modules/swffont.c", "lib/modules/swfobject.c", "lib/modules/swfrender.c", "lib/modules/swfshape.c",
"lib/modules/swfsound.c", "lib/modules/swftext.c", "lib/modules/swftools.c",
"lib/rfxswf.c", "lib/drawer.c", "lib/MD5.c", "lib/h.263/dct.c", "lib/h.263/h263tables.c",
"lib/h.263/swfvideo.c", "lib/action/assembler.c", "lib/action/compile.c",
"lib/action/lex.swf4.c", "lib/action/lex.swf5.c", "lib/action/libming.c",
"lib/action/swf4compiler.tab.c", "lib/action/swf5compiler.tab.c", "lib/action/actioncompiler.c",
"lib/as3/abc.c", "lib/as3/code.c", "lib/as3/pool.c", "lib/as3/files.c", "lib/as3/opcodes.c", 
"lib/as3/scripts.c", "lib/as3/common.c", "lib/as3/builtin.c", "lib/as3/compiler.c", "lib/as3/expr.c", "lib/as3/import.c",
"lib/as3/initcode.c", "lib/as3/parser.tab.c", "lib/as3/registry.c", "lib/as3/tokenizer.yy.c",
]
libpdf_sources = [
"lib/pdf/GFXOutputDev.cc", "lib/pdf/InfoOutputDev.cc", "lib/pdf/BitmapOutputDev.cc",
"lib/pdf/FullBitmapOutputDev.cc", "lib/pdf/pdf.cc", "lib/pdf/fonts.c", "lib/pdf/xpdf/GHash.cc",
"lib/pdf/xpdf/GList.cc", "lib/pdf/xpdf/GString.cc", "lib/pdf/xpdf/gmem.cc", "lib/pdf/xpdf/gfile.cc",
"lib/pdf/xpdf/FoFiTrueType.cc", "lib/pdf/xpdf/FoFiType1.cc", "lib/pdf/xpdf/FoFiType1C.cc",
"lib/pdf/xpdf/FoFiBase.cc", "lib/pdf/xpdf/FoFiEncodings.cc", "lib/pdf/xpdf/OutputDev.cc", "lib/pdf/xpdf/PDFDoc.cc",
"lib/pdf/xpdf/Error.cc", "lib/pdf/xpdf/Stream.cc", "lib/pdf/xpdf/Object.cc", "lib/pdf/xpdf/Decrypt.cc",
"lib/pdf/xpdf/Array.cc", "lib/pdf/xpdf/XRef.cc", "lib/pdf/xpdf/Dict.cc", "lib/pdf/xpdf/Parser.cc",
"lib/pdf/xpdf/Lexer.cc", "lib/pdf/xpdf/Outline.cc", "lib/pdf/xpdf/PDFDocEncoding.cc", "lib/pdf/xpdf/Catalog.cc",
"lib/pdf/xpdf/Link.cc", "lib/pdf/xpdf/GlobalParams.cc", "lib/pdf/xpdf/JBIG2Stream.cc", "lib/pdf/xpdf/Page.cc",
"lib/pdf/xpdf/JPXStream.cc", "lib/pdf/xpdf/JArithmeticDecoder.cc", "lib/pdf/xpdf/Gfx.cc", "lib/pdf/xpdf/GfxFont.cc",
"lib/pdf/xpdf/CMap.cc", "lib/pdf/xpdf/CharCodeToUnicode.cc", "lib/pdf/xpdf/PSTokenizer.cc", "lib/pdf/xpdf/FontEncodingTables.cc",
"lib/pdf/xpdf/BuiltinFont.cc", "lib/pdf/xpdf/BuiltinFontTables.cc", "lib/pdf/xpdf/GfxState.cc", "lib/pdf/xpdf/Function.cc",
"lib/pdf/xpdf/Annot.cc", "lib/pdf/xpdf/NameToCharCode.cc", "lib/pdf/xpdf/UnicodeMap.cc", "lib/pdf/xpdf/SecurityHandler.cc",
"lib/pdf/xpdf/SplashOutputDev.cc", "lib/pdf/xpdf/SplashFont.cc", "lib/pdf/xpdf/SplashState.cc", "lib/pdf/xpdf/Splash.cc",
"lib/pdf/xpdf/SplashBitmap.cc", "lib/pdf/xpdf/SplashClip.cc", "lib/pdf/xpdf/SplashPattern.cc", "lib/pdf/xpdf/SplashFontEngine.cc",
"lib/pdf/xpdf/SplashFontFile.cc", "lib/pdf/xpdf/SplashFontFileID.cc", "lib/pdf/xpdf/SplashScreen.cc", "lib/pdf/xpdf/SplashPath.cc",
"lib/pdf/xpdf/SplashXPath.cc", "lib/pdf/xpdf/SplashXPathScanner.cc", "lib/pdf/xpdf/SplashFTFontEngine.cc",
"lib/pdf/xpdf/SplashFTFontFile.cc", "lib/pdf/xpdf/SplashFTFont.cc"]

libgfx_sources = [
"lib/gfxtools.c", "lib/gfxfont.c", "lib/gfxpoly.c",
"lib/devices/dummy.c", "lib/devices/file.c", "lib/devices/render.c", "lib/devices/text.c", "lib/devices/record.c",
"lib/devices/ops.c", "lib/devices/polyops.c", "lib/devices/bbox.c", "lib/devices/rescale.c",
"lib/art/art_affine.c", "lib/art/art_alphagamma.c", "lib/art/art_bpath.c", "lib/art/art_gray_svp.c",
"lib/art/art_misc.c", "lib/art/art_pixbuf.c", "lib/art/art_rect.c", "lib/art/art_rect_svp.c",
"lib/art/art_rect_uta.c", "lib/art/art_render.c", "lib/art/art_render_gradient.c", "lib/art/art_render_mask.c",
"lib/art/art_render_svp.c", "lib/art/art_rgb.c", "lib/art/art_rgb_a_affine.c", "lib/art/art_rgb_affine.c",
"lib/art/art_rgb_affine_private.c", "lib/art/art_rgb_bitmap_affine.c", "lib/art/art_rgb_pixbuf_affine.c",
"lib/art/art_rgb_rgba_affine.c", "lib/art/art_rgb_svp.c", "lib/art/art_rgba.c", "lib/art/art_svp.c",
"lib/art/art_svp_intersect.c", "lib/art/art_svp_ops.c", "lib/art/art_svp_point.c",
"lib/art/art_svp_render_aa.c", "lib/art/art_svp_vpath.c", "lib/art/art_svp_vpath_stroke.c",
"lib/art/art_svp_wind.c", "lib/art/art_uta.c", "lib/art/art_uta_ops.c", "lib/art/art_uta_rect.c",
"lib/art/art_uta_svp.c", "lib/art/art_uta_vpath.c", "lib/art/art_vpath.c", "lib/art/art_vpath_bpath.c",
"lib/art/art_vpath_dash.c", "lib/art/art_vpath_svp.c", "lib/gocr/box.c", 
"lib/gocr/database.c", "lib/gocr/detect.c", "lib/gocr/job.c", "lib/gocr/lines.c", "lib/gocr/list.c", 
"lib/gocr/ocr0.c", "lib/gocr/ocr0n.c", "lib/gocr/ocr1.c", "lib/gocr/otsu.c", "lib/gocr/output.c", 
"lib/gocr/pgm2asc.c", "lib/gocr/pixel.c", "lib/gocr/progress.c", "lib/gocr/remove.c", "lib/gocr/unicode.c",
"lib/devices/ocr.c",
]
gfxswf_sources = ["lib/devices/swf.c", "lib/readers/swf.c", "lib/readers/image.c"]

swf_module = Extension(
    name='SWF', 
    include_dirs=cc.include_dirs+INCLUDE,
    sources=['lib/python/SWF.c', 'lib/python/taglist.c', 'lib/python/tag.c', 'lib/python/image.c', 
             'lib/python/tags.c', 'lib/python/tagmap.c', 'lib/python/action.c', 'lib/python/primitives.c', 
             'lib/python/pyutils.c'] + base_sources + rfxswf_sources,
    library_dirs=cc.library_dirs,
    libraries=config.libraries,
    extra_objects=['%s/lib/python%s/site-packages/PIL/_imaging.so' % (sys.prefix, sys.version[:3])],
    extra_compile_args=COMPILER_OPTIONS,
    )

gfx_module = Extension(
    name='gfx',
    sources=['lib/python/gfx.c'] + base_sources + libgfx_sources + rfxswf_sources + libpdf_sources + gfxswf_sources,
    include_dirs=cc.include_dirs+INCLUDE,
    library_dirs=cc.library_dirs,
    libraries=config.libraries + ["stdc++"],
    extra_objects=[],
    extra_compile_args=COMPILER_OPTIONS,
    )    

setup(name='SWFTools',
      version='0.9dev',
      description='Python wrapper for SWFTools',
      author='Matthias Kramm',
      author_email='kramm@quiss.org',
      url='http://www.swftools.org/',
      ext_modules=[swf_module, gfx_module],
)
