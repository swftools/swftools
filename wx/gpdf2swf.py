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
import __builtin__
import os
import sys
import imp

def main_is_frozen():
    return (hasattr(sys, "frozen") or # new py2exe
            hasattr(sys, "importers") # old py2exe
            or imp.is_frozen("__main__")) # tools/freeze

#if not main_is_frozen():
#    try:
#        import wxversion
#        wxversion.select("2.6")
#    except:
#        wxversion.select("2.8")

def get_main_dir():
    if main_is_frozen():
        return os.path.dirname(sys.executable)
    return os.path.dirname(os.path.abspath(__file__))
__builtin__.get_main_dir = get_main_dir
__builtin__.GPDF2SWF_BASEDIR = get_main_dir()

pyver = "".join(map(str, sys.version_info[0:2]))
#print >>sys.stderr, pyver
if main_is_frozen():
    sys.path.insert(0, os.path.join("..", "python%s" % pyver))
else:
    sys.path.insert(0, os.path.join("..", "lib", "python"))
    sys.path.insert(1, os.path.join("..", "python%s" % pyver))

import wx
#print >>sys.stderr, wx.VERSION
from lib.app import Pdf2Swf

if __name__ == "__main__":
    app = wx.App(False)
    app.SetAppName(u"gpdf2swf")
    Pdf2Swf()
    app.MainLoop()

