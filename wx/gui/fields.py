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
from choicetext import ChoiceInt

class Option(object):
    def __init__(self, name, label, data, selection):
        self.name = name
        self.label = label
        self.data = data
        self.selection = selection

    def _Update(self):
        self.value = self.ctrl.GetValue()

    def _SetControlValue(self, value):
        self.value = value
        self.ctrl.SetValue(value)

    def _OnUpdate(self, event):
        self._Update()
        event.Skip()

    def SetValue(self, value):
        self._SetControlValue(value)

class Hidden:
    def __init__(self, name, value):
        self.name = name
        self.value = value

    def draw(self, parent):
        return None, None

    def SetValue(self, value):
        self.value = value

class Choose(Option):
    klass = wx.Choice
    def __init__(self, name, label, data, selection):
        Option.__init__(self, name, label, data, selection)
        self.choices = data[0::2]
        self.data = data[1::2]

    def draw(self, parent):
        label = wx.StaticText(parent, label=self.label)
        self.ctrl = ctrl = self.klass(parent, choices=self.choices)
        ctrl.Bind(wx.EVT_CHOICE, self._OnUpdate)
        self._SetControlValue(self.selection)
        return label, ctrl

    def _Update(self):
        n = self.ctrl.GetSelection()
        self.value = self.data[n]

    def _SetControlValue(self, n):
        self.ctrl.SetSelection(n)
        self.value = self.data[n]

    def SetValue(self, value):
        # in python < 2.6 tuples doesnt have the index method
        # be sure that we are using a list
        tmp = list(self.data)
        try:
            n = tmp.index(value)
        except ValueError:
            n = self.selection
        self._SetControlValue(n)

class ChooseAndInt(Choose):
    klass = ChoiceInt
    def __init__(self, name, label, data, selection, editselection, editvalue):
        Choose.__init__(self, name, label, data, selection)
        self.editselection = editselection
        self.editvalue = editvalue

    def draw(self, parent):
        label = wx.StaticText(parent, label=self.label)
        self.ctrl = ctrl = self.klass(parent, choices=self.choices,
                                      editselection=self.editselection)
        ctrl.choice.Bind(wx.EVT_CHOICE, self._OnUpdate)
        ctrl.text.Bind(wx.EVT_TEXT, self._OnUpdate)
        self._SetControlValue(self.selection)
        return label, ctrl

    def _Update(self):
        n = self.ctrl.GetSelection()
        if  self.ctrl.IsEditableSelection(n):
            self.value = self.ctrl.GetValue()
        else:
            self.value = self.data[n]

    def _SetControlValue(self, n):
        self.ctrl.SetSelectionAndValue(n, self.editvalue)
        self.value = self.data[n]

    def SetValue(self, value):
        # in python < 2.6 tuples doesnt have the index method
        # be sure that we are using a list
        tmp = list(self.data)
        try:
            n = tmp.index(value)
        except ValueError:
            n = self.editselection
            self.editvalue = value
        self._SetControlValue(n)

class Radio(Option):
    klass = wx.RadioBox
    def __init__(self, name, label, data, selection):
        Option.__init__(self, name, label, data, selection)
        self.choices = data[0::2]
        self.data = data[1::2]
        self.selection = selection

    def draw(self, parent):
        self.ctrl = ctrl = self.klass(parent, label=self.label,
                                      choices=self.choices,
                                      majorDimension=1,
                                      style=wx.RA_SPECIFY_COLS)
        ctrl.Bind(wx.EVT_RADIOBOX, self._OnUpdate)
        self._SetControlValue(self.selection)
        return ctrl

    def _Update(self):
        n = self.ctrl.GetSelection()
        self.value = self.data[n]

    def _SetControlValue(self, n):
        self.ctrl.SetSelection(n)
        self.value = self.data[n]

    def SetValue(self, value):
        # in python < 2.6 tuples doesnt have the index method
        # be sure that we are using a list
        tmp = list(self.data)
        try:
            n = tmp.index(value)
        except ValueError:
            n = self.selection
        self._SetControlValue(n)

class Spinner(Option):
    klass = wx.SpinCtrl
    def __init__(self, name, label, data, selection):
        Option.__init__(self, name, label, data, selection)
        self.min = data[0]
        self.max = data[1]

    def draw(self, parent):
        label = wx.StaticText(parent, label=self.label)
        self.ctrl = ctrl = self.klass(parent, min=self.min, max=self.max,
                                      initial=self.selection)
        ctrl.Bind(wx.EVT_SPINCTRL, self._OnUpdate)
        self._SetControlValue(self.selection)
        return label, ctrl

class Slider(Option):
    klass = wx.Slider
    def __init__(self, name, label, data, selection):
        Option.__init__(self, name, label, data, selection)
        self.min = data[0]
        self.max = data[1]

    def draw(self, parent):
        label = wx.StaticText(parent, label=self.label)
        self.ctrl = ctrl = self.klass(parent, minValue=self.min,
                                      maxValue=self.max,
                                      value=self.selection,
                                      style=wx.SL_LABELS)
        ctrl.Bind(wx.EVT_SCROLL, self._OnUpdate)
        self._SetControlValue(self.selection)
        return label, ctrl
