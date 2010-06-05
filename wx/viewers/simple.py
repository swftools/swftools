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

from gui.fields import Hidden, Choose

order = 3
type = "core"
name = u"Simple Viewer"
desc = (u"A tiny viewer, which attaches directly to the SWF, "
        u"and provides small previous/next buttons in the upper left corner")

swf_options = [
    Hidden('simpleviewer', u'1'),
    Choose('flashversion', u'Flash version:',
              (u'4', 4, u'5', 5, u'6', 6, u'7', 7, u'8', 8), 2
          ),
    Choose('transparent', u'Make SWF file transparent:', (u'no', 0, u'yes', 1), 0),
    ]

viewer_options = []

from gui.plugin import Plugin
class Simple(Plugin):
    def __init__(self, swf, filename):

        for opt in swf_options:
            swf.setparameter(opt.name, str(opt.value))


def init(swf, filename):
    return Simple(swf, filename)


from lib.embeddedimage import PyEmbeddedImage
preview = PyEmbeddedImage(
    "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")
getpreviewData = preview.GetData
getpreviewImage = preview.GetImage
getpreviewBitmap = preview.GetBitmap
getpreviewIcon = preview.GetIcon
