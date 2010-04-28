import Image
import ImageChops
import ImageFilter
import sys
import os
import traceback
from athana import getTALstr
import random
import md5

filenames = []
directories = ["pdfs"]

SWFRENDER="swfrender"
PDFTOPPM="./pdftoppm"
CONVERT="convert"
PDF2SWF="pdf2swf"

COMPARE=["xpdf", "xpdf -C"]
OUTPUTDIR = "results.clip/"

#COMPARE=["xpdf", PDF2SWF+" -s convertgradients"]
#OUTPUTDIR = "results.pdf2swf/"

#COMPARE=[PDF2SWF, PDF2SWF+" --flatten"]
#OUTPUTDIR = "results.flatten/"

counter = 1

def randstr():
    return md5.md5(str(random.random())).hexdigest()[0:8]

def unlink(file):
    try:
        os.unlink(file)
    except:
        pass

def system(command):
    if ">" not in command:
        if os.system(command + " > /tmp/log.txt 2>&1") & 0xff00:
            error = open("/tmp/log.txt", "rb").read()
            print error
            return error
    else:
        if os.system(command) & 0xff00:
            return "Unknown error in "+command

class ConversionError:
    def __init__(self, msg):
        self.msg = msg
    def __str__(self):
        return self.msg

class TooComplexError:
    def __init__(self, msg):
        self.msg = msg
    def __str__(self):
        return self.msg

class BadMatch:
    def __init__(self, msg):
        self.msg = msg
    def __str__(self):
        return self.msg

def formatException():
    s = "Exception "+str(sys.exc_info()[0])
    info = sys.exc_info()[1]
    if info:
        s += " "+str(info)
    s += "\n"
    for l in traceback.extract_tb(sys.exc_info()[2]):
        s += "  File \"%s\", line %d, in %s\n" % (l[0],l[1],l[2])
        s += "    %s\n" % l[3]
    return s


class PDFPage:
    def __init__(self, filename, page, width, height):
        self.filename = filename
        self.page = page
        self.rating = None
        self.message = None
        self.htmlmessage = None
        self.file1 = None
        self.file2 = None
        self.file12 = None
        self.html12 = None
        self.htmldiff = None
        self.width,self.height = width,height

    def runtool(self, cmd, filename, page, file):
        if cmd.startswith("xpdf"):
            unlink("/tmp/test-%06d.ppm" % page)
            args = cmd[4:]
            error = system(PDFTOPPM + "%s -r 72 -f %d -l %d '%s' /tmp/test" % (args, page, page, filename))
            if error:
                raise ConversionError(error)
            unlink(file)
            error = system(CONVERT + " /tmp/test-%06d.ppm  %s" % (page, file))
            if error:
                raise ConversionError(error)
            unlink("/tmp/test-%06d.ppm" % page)
        else:
            unlink("/tmp/test.swf")
            unlink("svp.ps")
            error = system(COMPARE[0]+ " -Q 300 -p%d '%s' -o /tmp/test.swf" % (page, filename))
            #system("mv svp.ps %s.ps" % randstr())
            if error and "supports 65536" in error:
                raise TooComplexError(error)
            if error:
                raise ConversionError(error)
            unlink(file)
            error = system(SWFRENDER + " /tmp/test.swf -o %s" % file)
            if error:
                raise ConversionError(error)
            unlink("/tmp/test.swf")
        
    def runtools(self, filename, page, file1, file2, file12):

        badness = 0.0

        self.runtool(COMPARE[0], filename, page, file2)
        self.runtool(COMPARE[1], filename, page, file1)

        unlink(file12)

        pic1 = Image.open(file1)
        pic1.load()
        self.width1 = pic1.size[0]
        self.height1 = pic1.size[1]
        
        pic2 = Image.open(file2)
        pic2.load()
        self.width2 = pic2.size[0]
        self.height2 = pic2.size[1]

        if abs(self.width1-self.width2)>5 or abs(self.height1!=self.height2)>5:
            badness += 65536*abs(self.width2-self.width1)*max(self.height1,self.height2)+65536*abs(self.height2-self.height1)*max(self.width1,self.width2)

        minx = min(self.width1,self.width2)
        miny = min(self.height1,self.height2)

        pic1 = pic1.crop((0,0,minx,miny))
        pic1 = pic1.convert("RGB")
        pic1 = pic1.filter(ImageFilter.BLUR)
        pic2 = pic2.crop((0,0,minx,miny))
        pic2 = pic2.convert("RGB")
        pic2 = pic2.filter(ImageFilter.BLUR)

        diffimage = ImageChops.difference(pic1,pic2)
        diffimage.save(file12, "PNG")
        
        # compute quadratical difference
        diff = diffimage.histogram()
        for i in range(1,128):
            badness += (diff[i] + diff[256-i])*float(i*i)
            badness += (diff[256+i] + diff[256+256-i])*float(i*i)
            badness += (diff[512+i] + diff[512+256-i])*float(i*i)

        badness /= (minx*miny)*3

        return badness

    def compare(self):
        try:
            global counter
            self.file1 = str(counter) + ".png"
            counter = counter + 1
            self.file2 = str(counter) + ".png"
            counter = counter + 1
            self.file12 = str(counter) + ".png"
            counter = counter + 1
            self.rating = self.runtools(self.filename, self.page, OUTPUTDIR + self.file1, OUTPUTDIR + self.file2, OUTPUTDIR + self.file12)
        except BadMatch:
            self.rating = 65534.0
            self.message = formatException()
            print self.message
        except ConversionError:
            self.rating = 65535.0
            self.message = formatException()
            print self.message
        except TooComplexError:
            self.rating = 65536.0
            self.message = formatException()
            print self.message
        except:
            self.rating = 65537.0
            self.message = formatException()
            print self.message

    def getsizes(self):
        if self.message:
            return ""
        if abs(self.width1 - self.width2) > 5 or \
           abs(self.height1 - self.height2) > 5:
               return '<font color="red">%dx%d <-> %dx%d</font>' % (self.width1, self.height1, self.width2, self.height2)
        else:
               return '%dx%d,%dx%d' % (self.width1, self.height1, self.width2, self.height2)

    def generatehtml(self):
        global OUTPUTDIR
        global counter
        self.html12 = str(counter) + ".html"
        counter = counter + 1
        self.htmldiff = str(counter) + ".html"
        counter = counter + 1
        fi = open(OUTPUTDIR + self.html12, "wb")
        fi.write(getTALstr("""
<html><head></head>
<body>
<tal:block tal:replace="python:'File: '+self.filename"/><br>
<tal:block tal:replace="python:'Page: '+str(self.page)"/><br>
<tal:block tal:replace="python:'Rating: '+str(self.rating)"/><br>
<pre tal:condition="python:self.message" tal:replace="python:'Message: '+str(self.message)"/><br>
<hr>
<table cellspacing="0" cellpadding="0">
<tr><td><img tal:attributes="src python:self.file1"/></td><td><img tal:attributes="src python:self.file2"/></td></tr>
<tr><td>pdf2swf Version</td><td>pdftoppm Version</td></tr>
</table>
<hr>
</body>
</html>""", {"self": self}))
        fi.close()
        
        fi = open(OUTPUTDIR + self.htmldiff, "wb")
        fi.write(getTALstr("""
<html><head></head>
<body>
<tal:block tal:replace="python:'File: '+self.filename"/><br>
<tal:block tal:replace="python:'Page: '+str(self.page)"/><br>
<tal:block tal:replace="python:'Rating: '+str(self.rating)"/><br>
<pre tal:condition="python:self.message" tal:replace="python:'Message: '+str(self.message)"/><br>
<hr>
<img tal:attributes="src python:self.file12"/>
<hr>
</body>
</html>""", {"self": self}))
        fi.close()
        
        if self.message:
            self.htmlmessage = str(counter) + ".html"
            counter = counter + 1
            fi = open(OUTPUTDIR + self.htmlmessage, "wb")
            fi.write(getTALstr("""
<html><head></head>
<body>
<pre tal:content="raw python:self.message">
</pre>
</body>
</html>""", {"self": self}))
            fi.close()
        


def compare_pages(page1,page2):
    if page1.rating < page2.rating:
        return 1
    elif page1.rating > page2.rating:
        return -1
    else:
        return 0


def add_directory(directory):
    if not os.path.isdir(directory):
        print "bad directory:",directory
        return
    for file in os.listdir(directory):
        global filenames
        filename = os.path.join(directory, file)
        if file.lower().endswith(".pdf"):
            filenames += [filename]
            print "+",filename
        elif os.path.isdir(filename):
            add_directory(filename)

pages = []
try:
    os.mkdir(OUTPUTDIR)
except: pass

for file in filenames:
    print "+",file

for dir in directories:
    add_directory(dir)

for filename in filenames:
    try:
        unlink("/tmp/test.txt")
        error = system(PDF2SWF + " -I %s -o /tmp/test.txt" % filename)
        if error:
            raise ConversionError(error)
        fi = open("/tmp/test.txt", "rb")
        for line in fi.readlines():
            p = {}
            for param in line.split(" "):
                key,value = param.split("=")
                p[key] = value
            page = int(p["page"])
            width = int(float(p["width"]))
            height = int(float(p["height"]))
            print filename, page, "%dx%d" % (width, height)
            pdfpage = PDFPage(filename, page, width, height)
            pdfpage.compare()

            if width < 2000 and height < 2000:
                pages += [pdfpage]

            # only consider the first 3 pages
            if page > 3:
                break
        fi.close()
    except KeyboardInterrupt:
        break
    except:
        pdfpage = PDFPage(filename, -1, -1, -1)
        pdfpage.rating = 65536.0
        pdfpage.message = formatException()
        pages += [pdfpage]

pages.sort(compare_pages)

position = 1
for page in pages:
    page.generatehtml()
    page.position = position
    position = position + 1

fi = open(OUTPUTDIR + "index.html", "wb")
fi.write(getTALstr("""<html>
<head></head>
<body>
<table border="1"><tr><th>Position</th><th>Rating</th><th>File</th><th>Size</th><th>Page</th><th>Images</th><th>Diff</th><th>Further Info</th></tr>
<tal:block tal:repeat="page pages">
<tr>
<td tal:content="python:page.position"/>
<td tal:content="python:page.rating"/>
<td tal:content="python:page.filename"/>
<td tal:content="raw python:page.getsizes()"/>
<td tal:content="python:page.page"/>
<td><a tal:attributes="href python:page.html12">Side by Side</a></td>
<td><a tal:attributes="href python:page.htmldiff">Difference</a></td>
<td><a tal:condition="python:page.message" tal:attributes="href python:page.htmlmessage">Error message</a></td>
</tr>
</tal:block>
</table>
</body>
</html>""", {"pages": pages}))
fi.close()

