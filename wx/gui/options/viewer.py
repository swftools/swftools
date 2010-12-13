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

import os
import wx
import operator
import imp
from lib.wordwrap import wordwrap
from gui.boldstatictext import BoldStaticText
import viewers
import gui.fields

# this two are only to satisfy some plugins requirements
import gui.plugin
import shutil

import wx.html
#import viewers.raw
#import viewers.simple
#import viewers.rfx
#import viewers.flexpaper
#import viewers.technoargia

class Viewers:
    def __init__(self):
        #self.viewers = [
        #    'raw',
        #    'simple',
        #    'rfx',
        #    'flexpaper',
        #    'technoargia',
        #]
        self.viewers = []
        #self.modules = [
        #    viewers.raw,
        #    viewers.simple,
        #    viewers.rfx,
        #    viewers.flexpaper,
        #    viewers.technoargia,
        #]
        self.modules = []
 
        self.list_viewers()
        self.import_viewers()

    def list_viewers(self):
        for file in os.listdir('viewers'):
            if (file.startswith('.') or file.startswith('_')
                or file.endswith(".pyc") or not file.endswith('.py')):
                continue
            self.viewers.append(os.path.splitext(file)[0])

    def import_viewers(self):
        for file in self.viewers:
            try:
                _temp = imp.load_source("viewers.%s" % file, os.path.join(os.getcwdu(), "viewers/%s.py" % file))
                self.modules.append(_temp)
            except Exception, e:
                print "Could not load %s (%s)" % (file, e)


class HtmlWindow(wx.html.HtmlWindow):
    def __init__(self, parent, text=""):
        wx.html.HtmlWindow.__init__(self, parent, size=(-1, 100))
        self.SetFonts("arial", "courier", [7,8,9,10,11,12,13])
        self.SetPage(text)
        #self.Bind(wx.html.EVT_HTML_LINK_CLICKED, self.OnLinkClicked)

    def OnLinkClicked(self, event):
        if 'gtk2' in wx.PlatformInfo:
            os.system('xdg-open "%s"' % event.GetHref())
        elif 'msw' in wx.PlatformInfo():
            os.system('start "%s"' % event.GetHref())
        else: # MAC ???
            pass

class ViewerBook(wx.Listbook):
    def __init__(self, parent):
        wx.Listbook.__init__(self, parent, wx.ID_ANY, style=
                            wx.LB_DEFAULT
                            )

        self.__mod = []
        self.__viewers = viewers = Viewers()

        # make an image list
        il = wx.ImageList(102, 102)
        self.AssignImageList(il)

        # Sort viewers by attribute order.
        # The attribute order must be an int from 0..n
        viewers.modules.sort(key=operator.attrgetter('order'))

        # Make a bunch of panels for the list book
        for idx, mod in enumerate(viewers.modules):
            bmp = mod.preview.GetBitmap()
            il.Add(bmp)

            win = self.makePanel(mod)
            self.AddPage(win, mod, imageId=idx)
            if hasattr(mod, "default"):
                self.SetSelection(idx)


    def makePanel(self, mod):
        p = wx.Panel(self)
        sizer = wx.BoxSizer(wx.VERTICAL)

        nameCtrl = BoldStaticText(p, label=mod.name)
        sizer.Add(nameCtrl, 0, wx.EXPAND|wx.TOP, 5)

        descCtrl = HtmlWindow(p, mod.desc)
        sizer.Add(descCtrl, 0, wx.EXPAND|wx.TOP|wx.BOTTOM, 5)

        #swf_options = []

        # Avoid showing an empty StaticBox
        if (len(mod.swf_options) == 1
               and isinstance(mod.swf_options[0], gui.fields.Hidden)
           ):
            hidden = True
        else:
            hidden = False

        if mod.swf_options and not hidden:
            box = wx.StaticBox(p, label=u"SWF")
            bsizer = wx.StaticBoxSizer(box, wx.VERTICAL)

            flex = wx.FlexGridSizer(rows=len(mod.swf_options), cols=2, hgap=0, vgap=0)
            flex.AddGrowableCol(1)

            for option in mod.swf_options:
                #swf_options.append(option)
                label, ctrl = option.draw(p)
                if label and ctrl:
                    flex.Add(label, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 4)
                    flex.Add(ctrl, 0, wx.EXPAND|wx.ALL, 4)

            bsizer.Add(flex, 0, wx.EXPAND)
            sizer.Add(bsizer, 0, wx.EXPAND)

        #vie_options = []
        if mod.viewer_options:
            box = wx.StaticBox(p, label=u"Viewer")
            bsizer = wx.StaticBoxSizer(box, wx.VERTICAL)

            flex = wx.FlexGridSizer(rows=len(mod.viewer_options),
                                    cols=2, hgap=0, vgap=0)
            flex.AddGrowableCol(1)

            for option in mod.viewer_options:
                #vie_options.append(option)
                label, ctrl = option.draw(p)
                if label and ctrl:
                    flex.Add(label, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 4)
                    flex.Add(ctrl, 0, wx.EXPAND|wx.ALL, 4)

            bsizer.Add(flex, 0, wx.EXPAND)
            sizer.Add(bsizer, 0, wx.EXPAND)

        p.SetSizer(sizer)
        return p

    def AddPage(self, win, mod, select=False, imageId=-1):
        wx.Listbook.AddPage(self, win, mod.name, select, imageId)
        self.__mod.append(mod)

    def __get_options(self):
        page = self.GetSelection()
        return self.__mod[page]

    options = property(__get_options)

    def pickle(self):
        data = {}
        data['selected_viewer'] = self.GetSelection()

        for viewer, module in zip(self.__viewers.viewers, self.__viewers.modules):
            data[viewer] = {}
            for opt in module.swf_options:
                data[viewer][opt.name] = opt.value
            for opt in module.viewer_options:
                data[viewer][opt.name] = opt.value

        return data

    def unpickle(self, data):
        if not data:
            return

        selected_viewer = data.pop('selected_viewer')
        self.SetSelection(selected_viewer)

        _fields = {}
        for viewer, module in zip(self.__viewers.viewers, self.__viewers.modules):
            _fields[viewer] = {}

            all_opts = module.swf_options + module.viewer_options
            for field in all_opts:
                _fields[viewer][field.name] = field

        for modname, opts in data.items():
            for k, v in opts.items():
                _fields[modname][k].SetValue(v)

