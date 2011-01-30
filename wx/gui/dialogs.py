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

from __future__ import division
import os
import gfx
import wx
from lib.wordwrap import wordwrap

from gui.options import Quality, ViewerBook


class ProgressDialog(wx.ProgressDialog):
    def __init__(self, title, message, maximum=100, parent=None,
                 style=wx.PD_AUTO_HIDE|wx.PD_APP_MODAL):
        wx.ProgressDialog.__init__(self, title, message, maximum=maximum,
                                   parent=parent, style=style)

class OptionsDialog(wx.Dialog):
    def __init__(self, parent):
        wx.Dialog.__init__(self, parent)

        app_name = wx.GetApp().GetAppName()
        self.SetTitle(u"%s options" % app_name)

        p = wx.Notebook(self)
        self.__quality = Quality(p)
        self.__viewers = ViewerBook(p)
        p.AddPage(self.__quality, u"Quality")
        p.AddPage(self.__viewers, u"Viewer")

        sizer = wx.BoxSizer(wx.VERTICAL)
        btnsizer = wx.StdDialogButtonSizer()

        btn = wx.Button(self, wx.ID_OK)
        btn.SetDefault()
        btnsizer.AddButton(btn)
        btnsizer.Realize()


        sizer.Add(p, 0, wx.EXPAND|wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5)
        sizer.Add(btnsizer, 0, wx.ALIGN_RIGHT|wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5)

        self.SetSizer(sizer)
        sizer.Fit(self)

    def __get_quality(self):
        return self.__quality
    quality_panel = property(__get_quality)

    def __get_viewers(self):
        return self.__viewers
    viewers_panel = property(__get_viewers)

    def __get_quality_options(self):
        return self.__quality.options
    quality = property(__get_quality_options)

    def __get_viewers_options(self):
        return self.__viewers.options
    viewers = property(__get_viewers_options)


from gui.info import InfoList
class InfoDialog(wx.Dialog):
    def __init__(self, parent):
        wx.Dialog.__init__(self, parent,
                           style=wx.DEFAULT_DIALOG_STYLE
                                |wx.RESIZE_BORDER
                          )

        app_name = wx.GetApp().GetAppName()
        self.SetTitle(u"Document info - %s" % app_name)

        self.info = InfoList(self)

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.info, 1, wx.EXPAND, 0)


        btnsizer = wx.BoxSizer(wx.HORIZONTAL)

        btn = wx.Button(self, wx.ID_OK)
        btn.SetDefault()
        #self.SetAffirmativeId(wx.ID_CLOSE)
        btnsizer.Add(btn)
        sizer.Add(btnsizer, 0, wx.ALIGN_RIGHT|wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5)

        self.SetSizer(sizer)


class AboutDialog:
    def __init__(self, parent):
        appname = wx.GetApp().GetAppName()
        version = " - part of SWFTools %s" % gfx.VERSION
        copyright = (u"Copyright (c) 2008,2009,2010\n"
                     u"Matthias Kramm <kramm@quiss.org>")
        description = u"A graphical user interface for pdf2swf"
        developers = (u"Developers:\nMatthias Kramm <kramm@quiss.org>\n"
                      u"Ricardo Pedroso <rmdpedroso@gmail.com>")


        message = ("%(appname)s %(version)s\n\n"
                   "%(description)s\n\n"
                   "%(developers)s\n\n"
                   "%(copyright)s" % locals())
        caption = "About %s" % appname

        #wx.MessageBox(message, caption)

        wx.MessageDialog(parent, message, caption, style=wx.OK | wx.CENTRE).ShowModal()

