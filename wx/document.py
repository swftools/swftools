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
import time
import thread
import gfx
import wx
from wx.lib.pubsub import Publisher
from subprocess import Popen, PIPE

class _SaveSWFThread:
    def __init__(self, filename, doc, pages, options):
        self.__doc = doc
        self.__filename = filename
        self.__pages = pages or range(1, doc.pages+1)
        self.__options = options

    def Start(self):
        self.__keep_running = self.__running = True
        thread.start_new_thread(self.Run, ())
        #self.Run()

    def Stop(self):
        self.__keep_running = False

    def IsRunning(self):
        return self.__running

    def Run(self):
        pages = len(self.__pages)
        wx.CallAfter(Publisher.sendMessage, "SWF_BEGIN_SAVE",
                                            {'pages': pages,})

        time.sleep(0.05)

        swf = gfx.SWF()
        self.set_swf_parameters(swf)

        plugin = self.__options.viewers.init(swf, self.__filename)

        plugin.before_render()
        for i, pagenr in enumerate(self.__pages):
            page = self.__doc.getPage(pagenr)
            swf.startpage(page.width, page.height)
            page.render(swf)
            swf.endpage()
            wx.CallAfter(Publisher.sendMessage, "SWF_PAGE_SAVED",
                                                {'pagenr': i+1,
                                                 'pages': pages,})
            time.sleep(0.05)
            if not self.__keep_running:
                break
        else:
            # This will not run if we break the for loop
            plugin.before_save(page)
            swf.save(self.__filename)
            plugin.after_save(page)

        # No need. But to be sure that it's clean up
        # as soon as possible
        del swf

        wx.CallAfter(Publisher.sendMessage, "SWF_FILE_SAVED")
        time.sleep(0.05)

        self.__running = False

    def set_swf_parameters(self, swf):
        for opt in self.__options.quality:
            if type(opt.name) in (tuple, list):
                for name, value in (
                    # Example to better understand the list comprehension:
                    # opt.name = ['a', 'b', 'c']
                    # opt.value = [1, 2, 3]
                    # zip them = [('a',1), ('b', 2), ('c', 3)]
                    # pair will be in this example ('a', 1) due to
                    # the if pair[1] condition
                    pair for pair in zip(opt.name, opt.value) if pair[1] == 1
                ):
                    #print "1.swf.setparameter(%s, %s)" % (name, value)
                    swf.setparameter(str(name), str(value))
            else:
                #print "2.swf.setparameter(%s, %s)" % (opt.name, str(opt.value))
                swf.setparameter(opt.name, str(opt.value))


class Document:
    def __init__(self):
        self.__page = None
        self.__zoom = 1
        self.__lastsavefile = "output.swf"

    def __get_lastsavefile(self):
        return self.__lastsavefile
    def __set_lastsavefile(self, lastsavefile):
        self.__lastsavefile = lastsavefile
    lastsavefile = property(__get_lastsavefile, __set_lastsavefile)

    def __SwapExtension(self, filename, newext):
        basename, ext = os.path.splitext(filename)
        return "%s.%s" % (basename, newext)

    def __Reload(self):
        Publisher.sendMessage("PAGE_CHANGED",
                              {'page': self.__page,
                               'width': int(self.__page.width * self.__zoom),
                               'height': int(self.__page.height * self.__zoom)})

    def Load(self, filename):
        self.__lastsavefile = self.__SwapExtension(filename, "swf")
        #self.__htmlfilename = self.__SwapExtension(filename, "html")
        self.__pdf = gfx.open("pdf", filename)
        Publisher.sendMessage("FILE_LOADED", {'pages': self.__pdf.pages})

    def ChangePage(self, pagenr=1, size=None):
        self.__page = page = self.__pdf.getPage(pagenr)
        self.__Reload()

    def Fit(self, size):
        w = size[0] / self.__page.width
        h = size[1] / self.__page.height
        self.__zoom = min(w, h)
        self.__Reload()

    def Zoom(self, zoom):
        self.__zoom = 1 if zoom == 1 else self.__zoom + zoom
        self.__Reload()

    def GetThumbnails(self):
        for pagenr in range(1, self.__pdf.pages + 1):
            page = self.__pdf.getPage(pagenr)
            yield page

    def SaveSWF(self, filename, pages, options):
        self.__lastsavefile = filename
        t = _SaveSWFThread(filename, self.__pdf, pages, options)
        t.Start()
        return t
        

