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
import  wx.lib.intctrl

class ChoiceInt(wx.Panel):
    def __init__(self, parent, choices=[], editselection=None):
        wx.Panel.__init__(self, parent)
        choices = choices
        self.editselection = editselection

        s = wx.BoxSizer(wx.HORIZONTAL)

        self.choice = choice = wx.Choice(self, choices=choices)
        self.text = text = wx.lib.intctrl.IntCtrl(self)
        s.Add(choice, 1, wx.EXPAND)
        s.Add(text, 1, wx.EXPAND)

        self.SetSizer(s)

        choice.Bind(wx.EVT_CHOICE, self.__OnChoice)

    def IsEditableSelection(self, n):
        return n == self.editselection
 
    def GetValue(self):
        return self.text.GetValue()

    def SetValue(self, value):
        self.text.SetValue(value)

    def GetSelectionAndValue(self):
        return self.choice.GetSelection(), self.text.GetValue()

    def SetSelectionAndValue(self, n, value):
        self.SetSelection(n)
        self.text.SetValue(value)

    def GetSelection(self):
        return self.choice.GetSelection()

    def SetSelection(self, n):
        self.choice.SetSelection(n)
        self.EnableText(self.IsEditableSelection(n))

    def EnableText(self, enable):
        self.text.Enable(enable)

    def __OnChoice(self, event):
        self.EnableText(self.IsEditableSelection(event.GetSelection()))

