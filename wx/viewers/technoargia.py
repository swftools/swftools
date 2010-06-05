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

from gui.fields import Choose, ChooseAndInt, Hidden

order = 1
type = "custom"
name = u"technoargia Viewer"
desc = (u"Simple viewer with home link from Laurent Lalanne"
        u" (http://technoargia.free.fr/swftools/examples/viewer_home/viewer_home.html)")

swf_options = [
    Hidden("flashversion", 6),
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

class Technoargia(Plugin):
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
                                      "technoargia.swf")

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
    return Technoargia(swf, filename)


from lib.embeddedimage import PyEmbeddedImage

preview = PyEmbeddedImage(
    "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"
    "AABJRU5ErkJggg==")
getpreviewData = preview.GetData
getpreviewImage = preview.GetImage
getpreviewBitmap = preview.GetBitmap
getpreviewIcon = preview.GetIcon

