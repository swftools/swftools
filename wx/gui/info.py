import wx
from lib.document import PDF_INFO
import lib.utils as utils
#from wx.lib.mixins import listctrl as listmix

#class InfoList(wx.ListCtrl, listmix.ListCtrlAutoWidthMixin):
class InfoList(wx.ListCtrl):
    def __init__(self, parent):
        wx.ListCtrl.__init__(self, parent,
                                 style=wx.LC_REPORT
                                 | wx.LC_VIRTUAL
                                 | wx.BORDER_SUNKEN
                                 #| wx.BORDER_NONE
                                 #| wx.LC_EDIT_LABELS
                                 #| wx.LC_SORT_ASCENDING
                                 #| wx.LC_NO_HEADER
                                 | wx.LC_VRULES
                                 | wx.LC_HRULES
                                 | wx.LC_SINGLE_SEL
                             )
        #listmix.ListCtrlAutoWidthMixin.__init__(self)
        #self.setResizeColumn("LAST")

        self.InsertColumn(0, "Property")
        self.InsertColumn(1, "Value", wx.LIST_FORMAT_LEFT)
        self.SetColumnWidth(0, 120)
        self.SetColumnWidth(1, 400)

        self.__data = []

    def append(self, data):
        self.__data.append(data)
        self.SetItemCount(len(self.__data))
        self.RefreshItem(len(self.__data)-1)

    def OnGetItemText(self, item, col):
        data_row = self.__data[item]
        return data_row[col]

    def display(self, doc):
        self.__data = []
        for item in PDF_INFO:
            val = getattr(doc, item)
            sane_val = utils.force_unicode(val)
            self.append([item, sane_val])

