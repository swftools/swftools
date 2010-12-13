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
from gui.plugin import Plugin
import os, sys

order = 2
type = "custom"
name = u"Flex Paper Viewer"
if os.path.isfile(os.path.join(GPDF2SWF_BASEDIR, 'viewers', 'flexpaper', 'FlexPaperViewer.swf')):

    desc = u"""Provides a light weight document viewer with:
        Printing, Searching, Fit width, Fit height, Flowbox mode
        Zooming, Scrolling with Page Up/Down, Arrows, Drag,
        Full screen mode, Navigation buttons (next/prev),
        Print range. See http://flexpaper.devaldi.com/"""
    
    swf_options = [
        Choose("flashversion", u"Flash version:",
               (u"9", 9,), 0),
    ]
    
    viewer_options = [
        Choose("scale", u"Scale:",
               (u"0.1", 0.1,
                u"0.2", 0.2,
                u"0.3", 0.3,
                u"0.4", 0.4,
                u"0.5", 0.5,
                u"0.6", 0.6,
                u"0.7", 0.7,
                u"0.8", 0.8,
                u"0.9", 0.9,
                u"1.0", 1.0,
               ), 5),
        Choose("zoomtransition", u"Zoom transition:",
               ('easenone', 'easenone',
                'easeout', 'easeout',
                'linear', 'linear',
                'easeoutquad', 'easeoutquad',
               ), 1),
        Choose("zoomtime", u"Zoom time:",
               (u"0.0", '0.0',
                u"0.1", '0.1',
                u"0.2", '0.2',
                u"0.3", '0.3',
                u"0.4", '0.4',
                u"0.5", '0.5',
                u"0.6", '0.6',
                u"0.7", '0.7',
                u"0.8", '0.8',
                u"0.9", '0.9',
                u"1.0", '1.0',
               ), 5),
        Choose("zoominterval", u"Zoom interval:",
               (u"0.1", '0.1',
                u"0.2", '0.2',
                u"0.3", '0.3',
                u"0.4", '0.4',
                u"0.5", '0.5',
                u"0.6", '0.6',
                u"0.7", '0.7',
                u"0.8", '0.8',
                u"0.9", '0.9',
                u"1.0", '1.0',
               ), 0),
        Choose("fitpageonload", u"Fit page on load:",
               (u"no", 'false', u"yes", 'true', ), 0),
        Choose("localechain", u"Locale chain:",
               (
                'English',             'en_US',
                'French',              'fr_FR',
                'Chinese, Simple',     'zh_CN',
                'Spanish',             'es_ES',
                'Brazilian Portugese', 'pt_BR',
                'Russian',             'ru_RU',
                'Finnish',             'fi_FN',
                'German',              'de_DE',
                'Netherlands',         'nl_NL',
                'Turkish',             'tr_TR',
                'Swedish',             'se_SE',
                'Portugese',           'pt_PT',
                'Greek',               'el_EL',
                'Danish',              'da_DN',
                'Czech',               'cz_CS',
                'Italian',             'it_IT',
                'Polish',              'pl_PL',
                'Finnish',             'pv_FN',
                'Hungarian',           'hu_HU',
               ), 0),
    ]
    
    
    html_template = open('viewers/flexpaper/FlexPaperViewer.html').read()

    class FlexPaper(Plugin):
        def __init__(self, swf, filename):
            self.__swf = swf
            self.__filename = filename
    
            for opt in swf_options:
                setattr(self, "_%s" % opt.name, opt.value)
    
            for opt in viewer_options:
                setattr(self, "_%s" % opt.name, opt.value)
    
        def after_save(self, page):
            template_vars = {
                             'swffilename': os.path.basename(self.__filename),
                             'scale': self._scale,
                             'zoomtransition': self._zoomtransition,
                             'zoomtime': self._zoomtime,
                             'zoominterval': self._zoominterval,
                             'fitpageonload': self._fitpageonload,
                             'localechain': self._localechain,
                            }
    
            htmlfilename = self._swap_extension(self.__filename, "html")
            f = open(htmlfilename, "wb")
            f.write(html_template % template_vars)
            f.close()
    
    
            import shutil
            opj = os.path.join
            basedir = GPDF2SWF_BASEDIR
            src = opj(basedir, 'viewers', 'flexpaper')
            dst = os.path.dirname(self.__filename)
            shutil.copy(opj(src, 'FlexPaperViewer.swf'), dst)
    
            try:
                os.mkdir(opj(dst, 'js'))
            except OSError:
                pass
            shutil.copy(opj(src, 'js', 'jquery.js'), opj(dst,'js'))
    
            try:
                os.mkdir(opj(dst, 'js', 'swfobject'))
            except OSError:
                pass
            shutil.copy(opj(src, 'js', 'swfobject', 'expressInstall.swf'), opj(dst,'js', 'swfobject'))
            shutil.copy(opj(src, 'js', 'swfobject', 'swfobject.js'), opj(dst,'js', 'swfobject'))
    
            #shutil.copytree(src+'/js', dst+'/js')

else:
    desc = u"""You can use devaldi FlexPaper Viewer.
1. Just go to http://flexpaper.devaldi.com/
   and download the flash version.
   This was tested with FlexPaper 1.2.4 only.
2. Unzip it and find FlexPaperViewer.swf file.
3. Copy it into your swftools instalation directory under viewers.

It should end up in, for eg:
c:\\program files\\swftools\\viewers\\flexpaper\\FlexPaperViewer.swf

For now this is just a placeholder and it is identical to "No Viewer".
"""
    swf_options = [
        Choose("flashversion", u"Flash version:",
               (u"4", 4, u"5", 5, u"6", 6, u"7", 7, u"8", 8, u"9", 9), 2),
        Choose("insertstop", u"Insert stop after each frame:",
               (u"no", 0, u"yes", 1), 0),
        Choose("transparent", u"Make SWF file transparent:",
               (u"no", 0, u"yes", 1), 0),
        ]

    viewer_options = []

    class FlexPaper(Plugin):
        one_page_per_file = False
        def __init__(self, swf, filename):
            for opt in swf_options:
                swf.setparameter(opt.name, str(opt.value))

def init(swf, filename):
    return FlexPaper(swf, filename)

from lib.embeddedimage import PyEmbeddedImage

preview = PyEmbeddedImage(
    "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")
getpreviewData = preview.GetData
getpreviewImage = preview.GetImage
getpreviewBitmap = preview.GetBitmap
getpreviewIcon = preview.GetIcon

