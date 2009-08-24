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

import wx
from gui import fields

class Quality(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        self.__options = []

        sizer = wx.BoxSizer(wx.VERTICAL)
        obj = fields.Radio(
                        ("bitmap", "poly2bitmap", "bitmapfonts"),
                         u"Rendering mode",
                        [u"Convert polygons to polygons and fonts to fonts", (0, 0, 0),
                         u"Convert fonts to fonts, everything else to bitmaps", (0, 1, 0),
                         u"Convert everthing to bitmaps", (1, 0, 1),
                        ], 0)

        ra = obj.draw(self)
        self.__options.append(obj)
        sizer.Add(ra, 0, wx.EXPAND|wx.ALL, 5)

        quality = [
            ('Spinner', 'zoom', u'Resolution (in dpi):', (0, 100), 72),
            ('Slider', 'fontquality', u'Font quality:', (0, 100), 20),
            ('Choose', 'storeallcharacters', u'Insert full fonts in SWF file:',
                       (u'no', 0, u'yes', 1), 0),
            ('Slider', 'splinequality', u'Polygon quality:', (0, 100), 100),
            ('Slider', 'jpegquality', u'JPEG quality:', (0, 100), 75),
            ('Choose', 'jpegsubpixels', u'JPEG image resolution:',
                       (u'same as in PDF', 0, u'1x', 1, u'2x', 2, u'4x', 4), 0),
            ('Choose', 'ppmsubpixels', u'non-JPEG image resolution:',
                       (u'same as in PDF', 0, u'1x', 1, u'2x', 2, u'4x', 4), 0),
        ]

        box = wx.StaticBox(self, label=u"Quality")
        bsizer = wx.StaticBoxSizer(box, wx.VERTICAL)

        flex = wx.FlexGridSizer(rows=1, cols=2, hgap=0, vgap=0)
        flex.AddGrowableCol(0)
        flex.AddGrowableCol(1)
        for ctrl, opt, label, range, value in quality:
            wxobj = getattr(fields, ctrl)
            optobj = wxobj(opt, label, range, value)
            lb, sp = optobj.draw(self)
            flex.Add(lb, 0, wx.TOP|wx.ALIGN_CENTER_VERTICAL, 5)
            flex.Add(sp, 0, wx.TOP|wx.EXPAND, 5)
            self.__options.append(optobj)

        bsizer.Add(flex, 0, wx.EXPAND)
        sizer.Add(bsizer, 0, wx.EXPAND|wx.ALL, 5)

        self.SetSizer(sizer)

    def __get_options(self):
        return self.__options
    options = property(__get_options)

    def pickle(self):
        data = {}
        for opt in self.__options:
            data[opt.name] = opt.value
        return data

    def unpickle(self, data):
        fields = {}
        for opt in self.__options:
            fields[opt.name] = opt

        for k, v in data.items():
            fields[k].SetValue(v)
