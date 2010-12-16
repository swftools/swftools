import os, sys
import wx
import wx.lib.mixins.listctrl as listmix
import traceback


class BoldedStaticText(wx.StaticText):
    def __init__(self, *args, **kwargs):
        wx.StaticText.__init__(self, *args, **kwargs)
        font = self.GetFont()
        font.SetWeight(wx.BOLD)
        self.SetFont(font) 


class ListCtrl(wx.ListCtrl, listmix.ListCtrlAutoWidthMixin):
    def __init__(self, parent, size=wx.DefaultSize, style=0):
        wx.ListCtrl.__init__(self, parent, size=size, style=style)
        listmix.ListCtrlAutoWidthMixin.__init__(self)


class ErrorPanel(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)

        self.box = wx.BoxSizer(wx.VERTICAL)

        # Main Label
        self.box.Add(BoldedStaticText(self, label="An error has occurred")
                     , 0, wx.ALIGN_CENTER | wx.ALL, 10)

        # Exception Information
        boxInfoGrid  = wx.FlexGridSizer(2, 2, 0, 0)
        textFlags    = wx.ALIGN_LEFT | wx.LEFT | wx.RIGHT | wx.TOP

        boxInfoGrid.Add(BoldedStaticText(self, label="Type: "), 0, textFlags, 5 )
        self.error_type = wx.StaticText(self, label="")
        boxInfoGrid.Add(self.error_type, 0, textFlags, 5 )

        textFlags = wx.ALIGN_LEFT | wx.ALL
        boxInfoGrid.Add(BoldedStaticText(self, label="Value: ") , 0, textFlags, 5 )
        self.error_details = wx.StaticText(self, label="")
        boxInfoGrid.Add(self.error_details , 0, textFlags, 5 )

        bbox = wx.BoxSizer(wx.HORIZONTAL)
        self.copy = wx.Button(self, label="Copy to Clipboard")
        self.close = wx.Button(self, label="&Close")
        bbox.Add(self.copy)
        bbox.Add(self.close)

        # Set up the traceback list
        # This one automatically resizes last column to take up remaining space
        self.__list = ListCtrl(self, style=wx.LC_REPORT  | wx.SUNKEN_BORDER)
        self.__list.InsertColumn(0, "Filename")
        self.__list.InsertColumn(1, "Line", wx.LIST_FORMAT_RIGHT)
        self.__list.InsertColumn(2, "Function")
        self.__list.InsertColumn(3, "Code")

        self.box.Add(boxInfoGrid)
        self.box.Add((-1,10))
        self.box.Add(BoldedStaticText(self, label="Traceback:")
                     , 0, wx.ALIGN_LEFT | wx.LEFT | wx.TOP, 5)
        self.box.Add(self.__list, 1, wx.EXPAND | wx.ALIGN_CENTER | wx.ALL, 5)
        self.box.Add(bbox, 0, wx.ALIGN_RIGHT | wx.ALL, 5)

        self.box.Fit(self)
        self.SetSizer(self.box)

        self.tb_info = []

        self.copy.Bind(wx.EVT_BUTTON, self.OnCopy)


    def repr_(self, val):
        return repr(val)[1:-1]

    def show(self, type_, value, tb):
        sys.__excepthook__(type_, value, tb)

        self.error_type.SetLabel(self.repr_(type_))
        self.error_details.SetLabel(repr(value))

        self.tb_info = traceback.format_exception(type_, value, tb)
        x = 0
        self.__list.DeleteAllItems()
        for filename, line, func, text in traceback.extract_tb(tb):
            self.__list.InsertStringItem(x,
                    self.repr_(os.path.basename(filename))) # Filename
            self.__list.SetStringItem(x, 1, unicode(line))    # Line
            self.__list.SetStringItem(x, 2, unicode(func))    # Function
            self.__list.SetStringItem(x, 3, self.repr_(text)) # Code

            x += 1
        self.Show()

    def OnCopy(self, event):
        data = wx.TextDataObject()
        data.SetText(''.join(self.tb_info))
        if wx.TheClipboard.Open():
            wx.TheClipboard.SetData(data)
            wx.TheClipboard.Close()
            event.GetEventObject().Enable(False)
        else:
            wx.MessageBox("Unable to open the clipboard", "Error")


class ErrorFrame:
    def __init__(self, parent=None):
        self.frame = None
        self.parent = parent

    def __call__(self, type_, value, tb):
        if not self.frame:
            self.frame = wx.Frame(self.parent, size=(600,400))
            self.error_panel = ErrorPanel(self.frame)

            self.error_panel.close.Bind(wx.EVT_BUTTON, self.OnClose)
            self.frame.Bind(wx.EVT_CLOSE, self.OnClose)

        self.error_panel.show(type_, value, tb)

        self.frame.CenterOnParent()
        self.frame.Show()

    def OnClose(self, event):
        self.frame.Hide()
        #event.Skip()


if __name__ == '__main__':
    def raise_():
        raise Exception("Teste")

    app = wx.App()
    f = wx.Frame(None)
    sys.excepthook = ErrorFrame(f)
    f.Show()
    wx.FutureCall(500, raise_)
    app.MainLoop()
