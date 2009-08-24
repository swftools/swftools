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
from wx.lib.pubsub import Publisher
from wx.lib.embeddedimage import PyEmbeddedImage
import thread
import time

ICON_SIZE = 64

ID_INVERT_SELECTION = wx.NewId()
ID_SELECT_ODD = wx.NewId()
ID_SELECT_EVEN = wx.NewId()

# TODO: move into images.py
blank = PyEmbeddedImage(
    "iVBORw0KGgoAAAANSUhEUgAAAEAAAABACAYAAACqaXHeAAAAAXNSR0IArs4c6QAAAAZiS0dE"
    "AP8A/wD/oL2nkwAAAAlwSFlzAAALEwAACxMBAJqcGAAAAAd0SU1FB9kHDAscKjCK/4UAAABo"
    "SURBVHja7dABAcBAEAIgXf/Ofo8dRKDblqPa5stxAgQIECBAgAABAgQIECBAgAABAgQIECBA"
    "gAABAgQIECBAgAABAgQIECBAgAABAgQIECBAgAABAgQIECBAgAABAgQIEPALTbLLAQ8OIAV9"
    "8WNeKwAAAABJRU5ErkJggg==")
getblankData = blank.GetData
getblankImage = blank.GetImage
getblankBitmap = blank.GetBitmap

stop_width=8
stop_height=8
stop_data="""\xd5\x07\x07\xd5\x08\x08\xd5\x09\x09\xd4\x0a\x0a\xd4\x0a\x0a\xd5\x09\x09\xd5\x08\x08\xd5\x07\x07\xd5\x08\x08\xd4\x12\x12\xd4<<\xd1--\xd1,-\xd4;;\xd5\x12\x12\xd6\x08\x08\xd5\x0a\x0a\xd2@@\xd4\xc6\xc7\xd5kk\xd2gi\xd2\xc4\xc6\xd4<<\xd6\x0a\x0a\xd4\x0c\x0c\xcb66\xd6gi\xe3\xeb\xed\xe1\xeb\xed\xd4bc\xd3++\xd5\x0c\x0c\xd4\x0b\x0b\xd0--\xd3hi\xe2\xeb\xed\xe2\xed\xee\xd5cd\xd3++\xd5\x0c\x0c\xd5\x09\x09\xd4:;\xd3\xc5\xc7\xd3ij\xd3jj\xd3\xc5\xc7\xd4<<\xd6\x0a\x0a\xd5\x08\x08\xd4\x12\x12\xd5<=\xd2..\xd1,,\xd4;;\xd5\x12\x12\xd6\x08\x08\xd5\x07\x07\xd6\x08\x08\xd6\x0a\x0a\xd5\x0b\x0b\xd4\x0b\x0b\xd5\x09\x09\xd5\x08\x08\xd5\x07\x07"""


class _AppendThumbnailThread:
    def __init__(self, win, thumbs):
        self.__win = win
        self.__thumbs = thumbs

    def Start(self):
        self.__keep_running = self.running = True
        thread.start_new_thread(self.Run, ())

    def IsRunning(self):
        return self.running

    def Stop(self):
        self.__keep_running = False

    def Run(self):
        thumbs = self.__thumbs
        different_sizes = False
        for pos, thumb in enumerate(thumbs):
            if pos == 0:
                width, height = thumb.width, thumb.height
            else:
                if abs(width - thumb.width) > 2 or \
                   abs(height - thumb.height) > 2:
                       different_sizes = True

            wx.CallAfter(self.__win.AppendThumbnail, pos,
                         thumb.asImage(ICON_SIZE, ICON_SIZE))
            wx.CallAfter(Publisher.sendMessage, "THUMBNAIL_ADDED",
                                                {'pagenr':pos+1,})
            time.sleep(.05)
            if not self.__keep_running:
                break

        else:
            if different_sizes:
                wx.CallAfter(Publisher.sendMessage, "DIFF_SIZES")
                time.sleep(.10)
        wx.CallAfter(Publisher.sendMessage, "THUMBNAIL_DONE")
        time.sleep(.10)

        self.running = False

class FileDropTarget(wx.FileDropTarget):
    def __init__(self, window):
        wx.FileDropTarget.__init__(self)
        self.window = window

    def OnDropFiles(self, x, y, filenames):
        if len(filenames) == 1:
            Publisher.sendMessage("FILE_DROPED",
                                   {'filename': filenames[0]}
                                 )
        else:
            Publisher.sendMessage("FILES_DROPED",
                                   {'filenames': filenames}
                                 )

class PagePreviewWindow(wx.ScrolledWindow):
    def __init__(self, parent):
        wx.ScrolledWindow.__init__(self, parent)
        self.SetBackgroundColour('grey')
        self.SetScrollRate(20, 20)

        self.__buffer = wx.EmptyBitmap(1, 1)

        self.Bind(wx.EVT_PAINT, self.__OnPaint)

    def DisplayPage(self, page):
        thread.start_new_thread(self.__DisplayPageThread, (page,))

    def __OnPaint(self, event):
        dc = wx.BufferedPaintDC(self, self.__buffer, wx.BUFFER_VIRTUAL_AREA)

    def __DisplayPage(self, w, h, page):
        self.SetVirtualSize((w, h))
        self.__buffer = wx.EmptyBitmap(w+2, h+2)
        dc = wx.BufferedDC(None, self.__buffer)
        dc.Clear()
        dc.DrawRectangle(0, 0, w+2, h+2)
        dc.DrawBitmap(wx.BitmapFromBuffer(w, h, page), 1, 1, True)
        self.Refresh()

    def __DisplayPageThread(self, page):
        w = page['width']
        h = page['height']
        time.sleep(.02)
        page = page["page"].asImage(w, h)
        wx.CallAfter(self.__DisplayPage, w, h, page)


class PageListCtrl(wx.ListView):
    def __init__(self, parent):
        wx.ListView.__init__(self, parent, style=wx.LC_ICON)
        self.Bind(wx.EVT_CONTEXT_MENU, self.OnContextMenu)

    def OnContextMenu(self, event):
        menu = wx.Menu()
        menu.Append(wx.ID_SELECTALL, u"Select All\tCTRL-A")
        menu.AppendSeparator()
        menu.Append(wx.ID_SAVE, u"Save SWF (all pages)\tCTRL-W")
        menu.Append(wx.ID_SAVEAS, u"Save SWF (selected pages)\tCTRL-S")
        self.PopupMenu(menu)
        menu.Destroy()

    def DisplayEmptyThumbnails(self, pages):
        self.DeleteAllItems()
        self.imglist = wx.ImageList(ICON_SIZE, ICON_SIZE, mask=True)
        self.AssignImageList(self.imglist, wx.IMAGE_LIST_NORMAL)
        bmp = getblankBitmap()
        for pos in range(pages):
            self.imglist.Add(bmp)
            self.InsertImageStringItem(pos, u"Page %s" % (pos+1), pos)

    def DisplayThumbnails(self, thumbs):
        t = _AppendThumbnailThread(self, thumbs)
        t.Start()
        return t

    def AppendThumbnail(self, pos, thumb):
        bmp = wx.BitmapFromBuffer(ICON_SIZE, ICON_SIZE, thumb)
        self.imglist.Replace(pos, bmp)
        if pos == 0:
            wx.CallAfter(self.Select, 0)
        self.Refresh()


class StatusBar(wx.StatusBar):
    def __init__(self, parent):
        wx.StatusBar.__init__(self, parent, -1)

        self.sizeChanged = False
        self.Bind(wx.EVT_SIZE, self.OnSize)
        self.Bind(wx.EVT_IDLE, self.OnIdle)

        self.gauge = wx.Gauge(self)

        bmp = wx.BitmapFromImage(wx.ImageFromData(stop_width,stop_height,stop_data))
        self.btn_cancel = wx.BitmapButton(self, bitmap=bmp,
                                       style = wx.NO_BORDER)
        self.gauge.Hide()
        self.btn_cancel.Hide()
        
        self.Reposition()

    def SetGaugeValue(self, value):
        if value == 0:
            self.gauge.Hide()
            self.btn_cancel.Hide()
        self.gauge.SetValue(value)

    def SetGaugeRange(self, pages):
        self.gauge.Show()
        self.btn_cancel.Show()
        self.gauge.SetRange(pages)

    def OnSize(self, evt):
        self.Reposition()
        self.sizeChanged = True

    def OnIdle(self, evt):
        if self.sizeChanged:
            self.Reposition()

    def Reposition(self):
        rect = self.GetFieldRect(0)
        of = rect.width // 1.5
        self.gauge.SetPosition((rect.x+of , rect.y+2))
        self.gauge.SetSize((rect.width-of-24, rect.height-4))
        self.btn_cancel.SetPosition((rect.width-22, rect.y+1))
        self.btn_cancel.SetSize((22, rect.height-2))
        self.sizeChanged = False


class PdfFrame(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, None, size=(750,550), title=u"gpdf2swf")

        icon = self.__MakeIcon(os.path.join("images", "pdf2swf_gui.ico"))
        self.SetIcon(icon)
        self.__CreateMenu()
        self.__CreateToolbar()
        self.statusbar = StatusBar(self)
        self.SetStatusBar(self.statusbar)

        dt = FileDropTarget(self)
        self.SetDropTarget(dt)

        hsplit = wx.SplitterWindow(self, style=wx.SP_3D|wx.SP_LIVE_UPDATE)
        self.page_list = PageListCtrl(hsplit)
        self.page_preview = PagePreviewWindow(hsplit)
        hsplit.SplitVertically(self.page_list, self.page_preview,
                               sashPosition=ICON_SIZE*2)
        hsplit.SetMinimumPaneSize(ICON_SIZE*2)

    def __MakeIcon(self, filename):
        # TODO: Probably include the icon on a .py file
        img = wx.Bitmap(filename).ConvertToImage()
        if "wxMSW" in wx.PlatformInfo:
            img = img.Scale(16, 16)
        #elif "wxGTK" in wx.PlatformInfo:
        #    img = img.Scale(22, 22)
        # wxMac and wxGTK???? can be any size up to 128x128,
        # so leave the source img alone....
        icon = wx.IconFromBitmap(img.ConvertToBitmap())
        return icon

    def __CreateMenu(self):
        menubar = wx.MenuBar()

        menu = wx.Menu()
        menu.Append(wx.ID_OPEN, u"Open PDF\tCTRL-O", u"Open a PDF document")
        menu.AppendSeparator()
        menu.Append(wx.ID_SAVE, u"Save SWF (all pages)\tCTRL-W",
                                u"Save all pages")
        menu.Append(wx.ID_SAVEAS, u"Save SWF (selected pages)\tCTRL-S",
                                  u"Save selected pages")
        menu.AppendSeparator()
        menu.Append(wx.ID_EXIT, u"Exit\tCTRL-Q")
        menubar.Append(menu, u"&File")

        self.filehistory = wx.FileHistory()
        self.filehistory.UseMenu(menu)

        menu = wx.Menu()
        menu.Append(wx.ID_SELECTALL, u"Select All\tCTRL-A",
                                     u"Select all pages")
        menu.Append(ID_INVERT_SELECTION, u"Invert Selection",
                                         u"Invert current selection")
        menu.Append(ID_SELECT_ODD, u"Select Odd",
                                   u"Select odd pages")
        menu.Append(ID_SELECT_EVEN, u"Select Even",
                                    u"Select even pages")
        menu.AppendSeparator()
        menu.Append(wx.ID_PREFERENCES, u"Options\tCTRL-R",
                                       u"Show options dialog")
        menubar.Append(menu, u"&Edit")

        menu = wx.Menu()
        menu.Append(wx.ID_ZOOM_IN, u"Zoom In\tCTRL-+")
        menu.Append(wx.ID_ZOOM_OUT, u"Zoom Out\tCTRL--")
        menu.Append(wx.ID_ZOOM_100, u"Normal Size\tCTRL-0")
        menu.Append(wx.ID_ZOOM_FIT, u"Fit\tCTRL-1")
        menubar.Append(menu, u"&View")

        menu = wx.Menu()
        menu.Append(wx.ID_ABOUT, u"About")
        menubar.Append(menu, u"&Help")
        self.SetMenuBar(menubar)

    def __CreateToolbar(self):
        tsize = (16,16)
        wxart = wx.ArtProvider.GetBitmap
        self.toolbar = self.CreateToolBar(wx.TB_HORIZONTAL |
                                          wx.NO_BORDER | wx.TB_FLAT)
        self.toolbar.AddSimpleTool(wx.ID_OPEN,
                                   wxart(wx.ART_FILE_OPEN,
                                         wx.ART_TOOLBAR, tsize),
                                   u"Open")
        self.toolbar.AddSimpleTool(wx.ID_SAVE,
                                   wxart(wx.ART_FILE_SAVE,
                                         wx.ART_TOOLBAR, tsize),
                                   u"Save SWF (all pages)")
        self.toolbar.AddSimpleTool(wx.ID_PREFERENCES,
                                   wxart(wx.ART_LIST_VIEW,
                                         wx.ART_TOOLBAR, tsize),
                                   u"Options")
        self.toolbar.Realize()

