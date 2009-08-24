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

import wx
from wx.lib.wordwrap import wordwrap

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
        btnsizer = wx.BoxSizer(wx.HORIZONTAL)

        btn = wx.Button(self, wx.ID_CLOSE)
        self.SetAffirmativeId(wx.ID_CLOSE)
        btnsizer.Add(btn)

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


class AboutDialog:
    def __init__(self, parent):
        info = wx.AboutDialogInfo()
        # no need to get app name
        #info.Name = wx.GetApp().GetAppName()
        info.Version = u"0.9.0"
        info.Copyright = (u"Copyright (c) 2008,2009 "
                          u"Matthias Kramm <kramm@quiss.org>")
        info.Description = u"graphical user interface for pdf2swf"
        info.Developers = [
                           u"Matthias Kramm <kramm@quiss.org>",
                           u"Ricardo Pedroso <rmdpedroso@gmail.com>",
                          ]

        if 'wxGTK' in wx.PlatformInfo:
            info.WebSite = (u"http://www.swftools.org/", u"swftools home page")
            licenseText = [
                u"%(name)s is free software; you can redistribute "
                u"it and/or modify it under the terms of the GNU General "
                u"Public License as published by the Free Software "
                u"Foundation; either version 2 of the License, or (at "
                u"your option) any later version."
            ]
            licenseText.append(
                u"%(name)s is distributed in the hope that it will "
                u"be useful, but WITHOUT ANY WARRANTY; without even the "
                u"implied warranty of MERCHANTABILITY or FITNESS FOR A "
                u"PARTICULAR PURPOSE. See the GNU General Public License "
                u"for more details."
            )
            licenseText.append(
                u"You should have received a copy of the GNU General "
                u"Public License along with %(name)s; if not, "
                u"write to the Free Software Foundation, Inc., 51 "
                u"Franklin St, Fifth Floor, Boston, MA  02110-1301 USA"
            )
            lic = (os.linesep*2).join(licenseText) % {'name': info.Name}
            info.License = wordwrap(lic, 350, wx.ClientDC(parent))

        wx.AboutBox(info)

