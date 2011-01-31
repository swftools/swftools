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
import os, sys
import wx
import time
import pickle

from lib.wordwrap import wordwrap
from wx.lib.pubsub import Publisher

from document import Document
from gui.dialogs import (ProgressDialog, OptionsDialog, AboutDialog, InfoDialog)
from gui.gmain import (PdfFrame,
                      ID_INVERT_SELECTION, ID_SELECT_ODD,
                      ID_ONE_PAGE_PER_FILE,
                      ID_SELECT_EVEN, ID_DOC_INFO,
                     )
from lib import error
from lib import utils


def GetDataDir():
    """
    Return the standard location on this platform for application data
    """
    sp = wx.StandardPaths.Get()
    return sp.GetUserDataDir()

def GetConfig():
    if not os.path.exists(GetDataDir()):
        os.makedirs(GetDataDir())

    config = wx.FileConfig(
        localFilename=os.path.join(GetDataDir(), "options"))
    return config


class Pdf2Swf:
    def __init__(self):
        self.__doc = Document()

        self.__threads = {}

        self.__busy = None
        self.__progress = None

        self.__can_save = False
        self.__can_viewinfo = False

        self.view = PdfFrame()
        wx.GetApp().SetTopWindow(self.view)
        sys.excepthook = error.ErrorFrame(self.view)

        # Call Show after the current and pending event
        # handlers have been completed. Otherwise on MSW
        # we see the frame been draw and after that we saw
        # the menubar appear
        wx.CallAfter(self.Show)

        self.options = OptionsDialog(self.view)
        self.__ReadConfigurationFile()

        self.view.toolbar_preview_type.SetSelection(0)

        Publisher.subscribe(self.OnPageChanged, "PAGE_CHANGED")
        Publisher.subscribe(self.OnFileLoaded, "FILE_LOADED")
        Publisher.subscribe(self.OnFileNotLoaded, "FILE_NOT_LOADED")
        Publisher.subscribe(self.OnDiffSizes, "DIFF_SIZES")
        Publisher.subscribe(self.OnThumbnailAdded, "THUMBNAIL_ADDED")
        Publisher.subscribe(self.OnThumbnailDone, "THUMBNAIL_DONE")
        Publisher.subscribe(self.OnProgressBegin, "SWF_BEGIN_SAVE")
        Publisher.subscribe(self.OnProgressUpdate, "SWF_PAGE_SAVED")
        Publisher.subscribe(self.OnProgressDone, "SWF_FILE_SAVED")
        Publisher.subscribe(self.OnCombineError, "SWF_COMBINE_ERROR")
        Publisher.subscribe(self.OnFileDroped, "FILE_DROPED")
        Publisher.subscribe(self.OnFilesDroped, "FILES_DROPED")
        Publisher.subscribe(self.OnPluginOnePagePerFileNotSupported,
                                    "PLUGIN_ONE_PAGE_PER_FILE_NOT_SUPPORTED")
        Publisher.subscribe(self.OnPluginError, "PLUGIN_ERROR")

        self.view.Bind(wx.EVT_MENU, self.OnMenuOpen, id=wx.ID_OPEN)
        self.view.Bind(wx.EVT_MENU, self.OnMenuSave, id=wx.ID_SAVE)
        self.view.Bind(wx.EVT_MENU, self.OnMenuSaveSelected, id=wx.ID_SAVEAS)
        self.view.Bind(wx.EVT_MENU, self.OnMenuExit, id=wx.ID_EXIT)
        self.view.Bind(wx.EVT_MENU_RANGE, self.OnFileHistory,
                       id=wx.ID_FILE1, id2=wx.ID_FILE9)

        self.view.Bind(wx.EVT_UPDATE_UI, self.OnUpdateUI, id=wx.ID_SAVE)
        self.view.Bind(wx.EVT_UPDATE_UI, self.OnUpdateUI, id=wx.ID_SAVEAS)

        self.view.Bind(wx.EVT_MENU, self.OnMenuSelectAll, id=wx.ID_SELECTALL)
        self.view.Bind(wx.EVT_MENU,
                       self.OnMenuInvertSelection, id=ID_INVERT_SELECTION)
        self.view.Bind(wx.EVT_MENU, self.OnMenuSelectOdd, id=ID_SELECT_ODD)
        self.view.Bind(wx.EVT_MENU, self.OnMenuSelectEven, id=ID_SELECT_EVEN)
        self.view.Bind(wx.EVT_MENU, self.OnMenuOptions, id=wx.ID_PREFERENCES)

        self.view.Bind(wx.EVT_UPDATE_UI, self.OnUpdateUI, id=wx.ID_SELECTALL)
        self.view.Bind(wx.EVT_UPDATE_UI, self.OnUpdateUI, id=ID_INVERT_SELECTION)
        self.view.Bind(wx.EVT_UPDATE_UI, self.OnUpdateUI, id=ID_SELECT_ODD)
        self.view.Bind(wx.EVT_UPDATE_UI, self.OnUpdateUI, id=ID_SELECT_EVEN)

        self.view.Bind(wx.EVT_MENU, self.OnAbout, id=wx.ID_ABOUT)

        self.view.Bind(wx.EVT_MENU, self.OnZoom, id=wx.ID_ZOOM_IN)
        self.view.Bind(wx.EVT_MENU, self.OnZoom, id=wx.ID_ZOOM_OUT)
        self.view.Bind(wx.EVT_MENU, self.OnZoom, id=wx.ID_ZOOM_100)
        self.view.Bind(wx.EVT_MENU, self.OnFit, id=wx.ID_ZOOM_FIT)
        self.view.Bind(wx.EVT_MENU, self.OnShowDocInfo, id=ID_DOC_INFO)

        self.view.Bind(wx.EVT_UPDATE_UI, self.OnUpdateUI, id=wx.ID_ZOOM_IN)
        self.view.Bind(wx.EVT_UPDATE_UI, self.OnUpdateUI, id=wx.ID_ZOOM_OUT)
        self.view.Bind(wx.EVT_UPDATE_UI, self.OnUpdateUI, id=wx.ID_ZOOM_100)
        self.view.Bind(wx.EVT_UPDATE_UI, self.OnUpdateUI, id=wx.ID_ZOOM_FIT)
        self.view.Bind(wx.EVT_UPDATE_UI, self.OnUpdateUIInfo, id=ID_DOC_INFO)

        self.view.page_list.Bind(wx.EVT_LIST_ITEM_SELECTED, self.OnSelectItem)
        self.view.Bind(wx.EVT_CLOSE, self.OnMenuExit)

        self.view.toolbar_preview_type.Bind(wx.EVT_CHOICE,
                                            self.OnPreviewType)

        # statusbar cancel thumbanails generation button
        self.view.statusbar.btn_cancel.Bind(wx.EVT_BUTTON,
                                            self.OnThumbnailCancel)

        # Don't know where the problem is (python/xpython or wxwidgets/wxpython)
        # but I found that this hack was necessary to avoid the app enter in a
        # idle state. We must, for example, move the mouse inside the app
        # for threads continue their job.
        # There is no need for this when freezing with other utils, like
        # py2exe, pyinstaller, cxfreeze
        if "wxMSW" in wx.PlatformInfo:
            self.timer = wx.Timer(self.view)
            self.view.Bind(wx.EVT_TIMER, lambda evt: None)
            self.timer.Start(50)

    def OnFilesDroped(self, evt):
        dlg = wx.MessageDialog(self.view,
                      u"You must drop only one file.",
                      u"Notice",
                      style=wx.OK, pos=wx.DefaultPosition)
        dlg.ShowModal()
        dlg.Destroy()

    def OnFileDroped(self, message):
        self.__Load(message.data["filename"])

    def OnPluginOnePagePerFileNotSupported(self, message):
        self.Message(u"Selected viewer does not support "
                     u"one page per file. ")

    def OnPluginError(self, message):
        self.Message(u"Error applying selected viewer")

    def OnFileHistory(self, evt):
        # get the file based on the menu ID
        fileNum = evt.GetId() - wx.ID_FILE1
        filename = self.view.filehistory.GetHistoryFile(fileNum)

        self.__Load(filename)

    def OnProgressBegin(self, message):
        pages = message.data["pages"]
        style = (
                 wx.PD_APP_MODAL|wx.PD_ELAPSED_TIME|
                 wx.PD_REMAINING_TIME|wx.PD_CAN_ABORT|
                 wx.PD_AUTO_HIDE
                )
        if self.__progress:
            self.__progress.Destroy()
        self.__progress = ProgressDialog(u"Saving...",
                                       u"Start saving SWF pages",
                                       maximum=pages,
                                       parent=self.view, style=style)
        self.__progress.Show()
        self.view.SetStatusText(u"Saving document...")

    def OnProgressUpdate(self, message):
        pagenr = message.data["pagenr"]
        pages = message.data["pages"]

        keep_running = self.__progress.Update(
                                 pagenr,
                                 u"Saving SWF page %d of %d" % (pagenr, pages)
                             )

        if not keep_running and self.__threads.has_key("progress"):
            self.view.SetStatusText(u"Cancelling...")
            self.__threads.pop("progress").Stop()
            self.__progress.Hide()


    def OnProgressDone(self, message):
        if self.__threads.has_key("progress"): # it goes all the way?
            self.__threads.pop("progress")
            self.view.SetStatusText(u"SWF document saved successfully.")
        else:
            self.view.SetStatusText(u"")

    def OnCombineError(self, message):
        from wx.lib.dialogs import ScrolledMessageDialog
        ScrolledMessageDialog(self.view, message.data, u"Notice").ShowModal()


    def OnThumbnailAdded(self, message):
        self.view.statusbar.SetGaugeValue(message.data['pagenr'])
        tot = self.view.page_list.GetItemCount()
        self.view.SetStatusText(u"Generating thumbnails %s/%d" %
                                (message.data['pagenr'], tot), 0)

    def OnThumbnailDone(self, message):
        self.view.statusbar.SetGaugeValue(0)
        self.view.SetStatusText(u"", 0)
        if self.__threads.has_key("thumbnails"):
            self.__threads.pop("thumbnails")
        self.view.SendSizeEvent()

    def OnThumbnailCancel(self, event):
        if self.__threads.has_key("thumbnails"):
            self.__threads["thumbnails"].Stop()

    def OnSelectItem(self, event):
        self.__doc.ChangePage(event.GetIndex() + 1)

    def OnPreviewType(self, event):
        filename = self.__doc.filename
        if filename:
            self.__Load(filename)

    def OnPageChanged(self, message):
        # ignore if we have more than one item selected
        if self.view.page_list.GetSelectedItemCount() > 1:
            return

        self.view.page_preview.DisplayPage(message.data)

    def SetTitle(self):
        name = wx.GetApp().GetAppName()
        filename = os.path.basename(self.__doc.filename)
        if self.__doc.title != "n/a":
            t = u"%s - %s (%s)" % (name, filename, self.__doc.title)
        else:
            t = u"%s - %s" % (name, filename)
        self.view.SetTitle(t)

    def OnFileLoaded(self, message):
        if self.__progress:
            self.__progress.Destroy()
            self.__progress = None

        self.__can_viewinfo = True
        del self.__busy

        self.SetTitle()

        if self.__doc.oktocopy == 'no':
            self.__can_save = False
            self.view.page_list.DisplayEmptyThumbnails(0)
            self.view.page_preview.Clear()
            self.view.SetStatusText(u"")
            self.Message(
                    u"This PDF disallows copying, cannot be converted."
                    )
            return

        #if not self.__doc.oktoprint:
        self.view.SetStatusText(u"Document loaded successfully.")

        self.view.page_list.DisplayEmptyThumbnails(message.data["pages"])
        thumbs = self.__doc.GetThumbnails()
        t = self.view.page_list.DisplayThumbnails(thumbs)
        self.__threads["thumbnails"] = t
        self.view.statusbar.SetGaugeRange(message.data["pages"])
        #del self.__busy

    def OnFileNotLoaded(self, message):
        self.__can_save = False
        self.__can_viewinfo = False
        del self.__busy
        self.view.SetStatusText(u"")
        self.Message(
                    u"Could not open file %s" % message.data['filename']
                    )
        
    def OnDiffSizes(self, message):
        # just let the user know- for now, we can't handle this properly
        self.Message(
                    u"In this PDF, width or height are not the same for "
                    u"each page. This might cause problems if you export "
                    u"pages of different dimensions into the same SWF file."
                    )

    def OnMenuOpen(self, event):
        dlg = wx.FileDialog(self.view, u"Choose PDF File:",
                     style=wx.OPEN|wx.CHANGE_DIR,
                     wildcard = u"PDF files (*.pdf)|*.pdf|all files (*.*)|*.*")

        if dlg.ShowModal() == wx.ID_OK:
            filename = dlg.GetPath()
            self.__Load(filename)

    def OnMenuSave(self, event, pages=None):
        defaultFile = self.__doc.lastsavefile
        if "wxMSW" in wx.PlatformInfo:
            allFiles = "*.*"
        else:
            allFiles = "*"
        self.view.SetStatusText(u"")
        dlg = wx.FileDialog(self.view, u"Choose Save Filename:",
                       style = wx.SAVE | wx.OVERWRITE_PROMPT,
                       defaultFile=os.path.basename(defaultFile),
                       wildcard=u"SWF files (*.swf)|*.swf"
                                 "|all files (%s)|%s" % (allFiles, allFiles))


        if dlg.ShowModal() == wx.ID_OK:
            menubar = self.view.GetMenuBar()
            one_file_per_page = menubar.IsChecked(ID_ONE_PAGE_PER_FILE)

            self.__threads["progress"] = self.__doc.SaveSWF(dlg.GetPath(),
                                                         one_file_per_page,
                                                         pages, self.options)

    def OnUpdateUI(self, event):
        menubar = self.view.GetMenuBar()
        menubar.Enable(event.GetId(), self.__can_save)

        self.view.GetToolBar().EnableTool(event.GetId(), self.__can_save)

    def OnUpdateUIInfo(self, event):
        menubar = self.view.GetMenuBar()
        menubar.Enable(event.GetId(), self.__can_viewinfo)

        self.view.GetToolBar().EnableTool(event.GetId(), self.__can_viewinfo)

    def OnMenuSaveSelected(self, event):
        pages = []
        page = self.view.page_list.GetFirstSelected()
        pages.append(page+1)

        while True:
            page = self.view.page_list.GetNextSelected(page)
            if page == -1:
                break
            pages.append(page+1)

        self.OnMenuSave(event, pages)

    def OnMenuExit(self, event):
        self.view.SetStatusText(u"Cleaning up...")

        # Stop any running thread
        self.__StopThreads()

        config = GetConfig()
        self.view.filehistory.Save(config)
        config.Flush()
        # A little extra cleanup is required for the FileHistory control
        del self.view.filehistory

        # Save quality options
        dirpath = GetDataDir()
        data = self.options.quality_panel.pickle()
        try:
            f = file(os.path.join(dirpath, 'quality.pkl'), 'wb')
            pickle.dump(data, f)
            f.close()
        except Exception:
            pass

        # Save viewer options
        try:
            f = file(os.path.join(dirpath, 'viewers.pkl'), 'wb')
            data = self.options.viewers_panel.pickle()
            pickle.dump(data, f)
            f.close()
        except Exception:
            pass

        self.view.Destroy()

    def OnMenuSelectAll(self, event):
        for i in range(0, self.view.page_list.GetItemCount()):
            self.view.page_list.Select(i, True)

    def OnMenuInvertSelection(self, event):
        for i in range(0, self.view.page_list.GetItemCount()):
            self.view.page_list.Select(i, not self.view.page_list.IsSelected(i))

    def OnMenuSelectOdd(self, event):
        for i in range(0, self.view.page_list.GetItemCount()):
            self.view.page_list.Select(i, not bool(i%2))

    def OnMenuSelectEven(self, event):
        for i in range(0, self.view.page_list.GetItemCount()):
            self.view.page_list.Select(i, bool(i%2))

    def OnMenuOptions(self, event):
        self.options.ShowModal()

    def OnFit(self, event):
        self.__doc.Fit(self.view.page_preview.GetClientSize())

    def OnZoom(self, event):
        zoom = {
              wx.ID_ZOOM_IN: .1,
              wx.ID_ZOOM_OUT: -.1,
              wx.ID_ZOOM_100: 1,
        }
        self.__doc.Zoom(zoom[event.GetId()])

    def OnShowDocInfo(self, event):
        info = InfoDialog(self.view)
        info.info.display(self.__doc)
        info.Show()

    def OnAbout(self, evt):
        AboutDialog(self.view)

    def __Load(self, filename):
        self.__can_save = True
        self.__StopThreads()
        self.view.SetStatusText(u"Loading document...")
        self.__busy = wx.BusyInfo(u"One moment please, "
                                  u"opening pdf document...")

        self.view.filehistory.AddFileToHistory(filename)
        try:
            # I dont care if this, for some reason,
            # give some error. I just swallow it
            os.chdir(os.path.dirname(filename))
        except:
            pass

        # Need to delay the file load a little bit
        # for the BusyInfo get a change to repaint itself
        #wx.FutureCall(150, self.__doc.Load, filename)
        sel = self.view.toolbar_preview_type.GetSelection()
        #print sel
        PREV_TYPE = {
            0 : [('bitmap', '1'), ('poly2bitmap', '0'), ('bitmapfonts', '1'),
                ('textonly', '0')],
            1 : [('bitmap', '0'), ('poly2bitmap', '1'), ('bitmapfonts', '0'),
                ('textonly', '0')],
            2 : [('bitmap', '0'), ('poly2bitmap', '0'), ('bitmapfonts', '0'),
                ('textonly', '0')],
            3 : [('bitmap', '0'), ('poly2bitmap', '0'), ('bitmapfonts', '0'),
                ('textonly', '1')],
        }
        self.__doc.preview_parameters = PREV_TYPE[sel]
        wx.CallAfter(self.__doc.Load, filename)

    def __StopThreads(self):
        for n, t in self.__threads.items():
            t.Stop()

        running = True
        while running:
            running = False
            for n, t in self.__threads.items():
                running = running + t.IsRunning()
            time.sleep(0.1)

    def __ReadConfigurationFile(self):
        config = GetConfig()
        self.view.filehistory.Load(config)

        dirpath = GetDataDir()
        try:
            f = file(os.path.join(dirpath, 'quality.pkl'), 'rb')
            #try:
            if 1:
                data = pickle.load(f)
                self.options.quality_panel.unpickle(data)
            #except:
            #    self.Message(
            #          u"Error loading quality settings. "
            #          u"They will be reset to defaults. ")
            f.close()
        except Exception:
            pass

        try:
            f = file(os.path.join(dirpath, 'viewers.pkl'), 'rb')
            #try:
            if 1:
                data = pickle.load(f)
                self.options.viewers_panel.unpickle(data)
            #except:
            #    self.Message(
            #          u"Error loading viewers settings. "
            #          u"They will be reset to defaults. ")
            f.close()
        except Exception:
            pass
        #d = pickle.load(f)

    def Show(self):
        self.view.Show()
        if len(sys.argv) == 2:
            self.__Load(utils.force_unicode(sys.argv[1]))

    def Message(self, message):
        dlg = wx.MessageDialog(self.view,
                      message,
                      style=wx.OK, pos=wx.DefaultPosition)
        dlg.ShowModal()
        dlg.Destroy()

