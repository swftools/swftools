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
import time
import wx
from wx.lib.pubsub import Publisher
from subprocess import Popen, PIPE

class Plugin:
    def before_render(self):
        pass

    def before_save(self, page):
        pass

    def after_save(self, page):
        pass

    def _swap_extension(self, filename, newext, flashversion=""):
        name, ext = os.path.splitext(filename)
        return u"%s%s.%s" % (name, flashversion, newext)

    def __find_swfcombine(self):
        found = False
        prog = "swfcombine.exe" if "wxMSW" in wx.PlatformInfo else "swfcombine"
        basedir = os.path.dirname(__file__)

        opj = os.path.join
        locations = [opj(basedir, prog)]
        if "wxMSW" in wx.PlatformInfo:
            locations.extend([
                              opj("c:", "swftools", prog),
                              opj("c:", "Program Files", "SWFTools", prog)
                             ])
        else:
            locations.extend([
                              opj(os.sep, "usr", "local", "bin", prog),
                              opj(os.sep, "usr", "bin", prog),
                             ])

        exe = prog
        for e in locations:
            if os.path.isfile(e):
                exe = e
                found = True
                break
        return exe, found

    def swfcombine(self, *args):
        try:
            self.__swfcombine(*args)
        except Exception, e:
            wx.CallAfter(Publisher.sendMessage,
                         "SWF_COMBINE_ERROR", unicode(e))
            time.sleep(0.05)

    def __swfcombine(self, *args):
        exe, found = self.__find_swfcombine()
        # uncoment to test a failed swfcombine find
        #found = False

        if not found:
            raise Exception(u"Could not execute %s: %s not found" % (exe, exe))

        # Create a command line
        cmd = [exe,]
        cmd.extend(args)

        output = Popen(cmd, stdout=PIPE).communicate()[0]

        # Check the process output
        if output:
            raise Exception(u"Error executing %s:%s%s" %
                             (u" ".join(cmd), os.linesep, output))

