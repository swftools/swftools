#!/usr/bin/env python
# -*- coding: UTF-8 -*-
#
# gpdf2swf.py
# graphical user interface for pdf2swf
#
# Part of the swftools package.
# 
# Copyright (c) 2008,2009 Matthias Kramm <kramm@quiss.org> 
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

from gui.fields import Choose, ChooseAndInt

order = 0
default = True
type = "custom"
name = u"rfx Viewer"
desc = u"A more sophisticated viewer with zooming and scrolling."

swf_options = [
    Choose("flashversion", u"Flash version:",
           (u"7", 7, u"8", 8), 1),
]

viewer_options = [
    Choose("html", u"Save HTML:",
           (u"no", False, u"yes", True, ), True),
    ChooseAndInt('width', 'Width:',
                  ('same as PDF', -2, 'fullscreen', -1, 'custom', 0), 1, 2, 600),
    ChooseAndInt('height', 'Height:',
                  ('same as PDF', -2, 'fullscreen', -1, 'custom', 0), 1, 2, 800),
    Choose('zoomtype', 'Initial zoom level:',
            ('Original resolution', 1,
             'Show all', 2,
             'Maximum width/height', 3), 2),
]


html_template = """<html>
<body style="padding: 0px; margin: 0px">
<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"
        width="%(width)s"
        height="%(height)s"
        codebase="http://active.macromedia.com/flash5/cabs/swflash.cab#version=%(version)d,0,0,0">
        <param name="MOVIE" value="%(swffilename)s">
        <param name="PLAY" value="true">
        <param name="LOOP" value="true">
        <param name="QUALITY" value="high">
        <param name="FLASHVARS" value="%(flashvars)s">
          <embed src="%(swffilename)s" width="%(width)s" height="%(height)s"
                 play="true" ALIGN="" loop="true" quality="high"
                 type="application/x-shockwave-flash"
                 flashvars="%(flashvars)s"
                 pluginspage="http://www.macromedia.com/go/getflashplayer">
          </embed>
</object>
</body>
</html>
"""

from gui.plugin import Plugin
import os, sys

class Rfx(Plugin):
    def __init__(self, swf, filename):
        self.__swf = swf
        self.__filename = filename

        for opt in swf_options:
            swf.setparameter(opt.name, str(opt.value))
            setattr(self, "_%s" % opt.name, opt.value)

        for opt in viewer_options:
            setattr(self, "_%s" % opt.name, opt.value)

    def after_save(self, page):
        basedir = GPDF2SWF_BASEDIR
        viewerfilename = os.path.join(basedir,
                                      "viewers",
                                      "rfx%s.swf" % self._flashversion)

        self.swfcombine(
               u"%s" % viewerfilename,
               u"viewport=%s" % self.__filename,
               u"-o", u"%s" % self.__filename,
        )

        if not self._html:
            return

        if self._width == -1:
            width = "100%"
        elif self._width == -2:
            width = page.width+40
        else:
            width = self._width

        if self._height == -1:
            height = "100%"
        elif self._height == -2:
            height = page.height+70
        else:
            height = self._height

        template_vars = {
                         'width': width,
                         'height': height,
                         'version': self._flashversion,
                         'flashvars': "zoomtype=%s" % self._zoomtype,
                         'swffilename': os.path.basename(self.__filename),
                        }

        htmlfilename = self._swap_extension(self.__filename, "html")
        f = open(htmlfilename, "wb")
        f.write(html_template % template_vars)
        f.close()


def init(swf, filename):
    return Rfx(swf, filename)

from lib.embeddedimage import PyEmbeddedImage

preview = PyEmbeddedImage(
    "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")
getpreviewData = preview.GetData
getpreviewImage = preview.GetImage
getpreviewBitmap = preview.GetBitmap
getpreviewIcon = preview.GetIcon

