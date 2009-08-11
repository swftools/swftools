import wx
import wx.lib.scrolledpanel as scrolled
import os
import re
import sys
import time
import thread
import traceback
import math

class Check:
    def __init__(self, x,y):
        self.x = x
        self.y = y
    def left(self):
        return "pixel at (%d,%d)" % (self.x,self.y)
    def right(self):
        return ""

class PixelColorCheck(Check):
    def __init__(self, x,y, color):
        Check.__init__(self,x,y)
        self.color = color
    def right(self):
        return "is of color 0x%06x" % self.color

class TwoPixelCheck(Check): 
    def __init__(self, x,y, x2,y2):
        Check.__init__(self,x,y)
        self.x2,self.y2 = x2,y2
    def right(self):
        return "pixel at (%d,%d)" % (self.x2,self.y2)

class PixelBrighterThan(TwoPixelCheck):
    pass

class PixelDarkerThan(TwoPixelCheck):
    pass

class PixelEqualTo(TwoPixelCheck):
    pass

class AreaCheck(Check):
    def __init__(self, x,y, x2,y2):
        Check.__init__(self,x,y)
        self.x2,self.y2 = x2,y2
    def left(self):
        return "area at (%d,%d,%d,%d)" % (self.x,self.y,self.x2,self.y2)

class AreaPlain(AreaCheck):
    pass

class AreaNotPlain(AreaCheck):
    pass

class Model:
    def __init__(self, filename, checks):
        self.filename = filename
        self.checks = checks

    @staticmethod
    def load(filename):
        path = os.path.splitext(filename)[0]+".rb"
        fi = open(path, "rb")
        r_file = re.compile(r"^convert_file \"([^\"]*)\"")
        r_pixelcolor = re.compile(r"^pixel_at\(([0-9]+),([0-9]+)\).should_be_of_color (0x[0-9a-fA-F]+)")
        r_pixelbrighter = re.compile(r"^pixel_at\(([0-9]+),([0-9]+)\).should_be_brighter_than pixel_at\(([0-9]+),([0-9]+)\)")
        r_pixeldarker = re.compile(r"^pixel_at\(([0-9]+),([0-9]+)\).should_be_darker_than pixel_at\(([0-9]+),([0-9]+)\)")
        r_pixelequalto = re.compile(r"^pixel_at\(([0-9]+),([0-9]+)\).should_be_the_same_as pixel_at\(([0-9]+),([0-9]+)\)")
        r_areaplain = re.compile(r"^area_at\(([0-9]+),([0-9]+),([0-9]+),([0-9]+)\).should_be_plain_colored")
        r_areanotplain = re.compile(r"^area_at\(([0-9]+),([0-9]+),([0-9]+),([0-9]+)\).should_not_be_plain_colored")
        r_width = re.compile(r"^width.should be ([0-9]+)")
        r_height = re.compile(r"^height.should be ([0-9]+)")
        r_describe = re.compile(r"^describe \"pdf conversion\"")
        r_header = re.compile(r"^require File.dirname")
        r_end = re.compile(r"^end$")
        filename = None
        checks = []
        for nr,line in enumerate(fi.readlines()):
            line = line.strip()
            if not line:
                continue
            m = r_file.match(line)
            if m: 
                if filename:
                    raise Exception("can't load multi-file specs (in line %d)" % (nr+1))
                filename = m.group(1);
                continue
            m = r_pixelcolor.match(line)
            if m: checks += [PixelColorCheck(int(m.group(1)),int(m.group(2)),int(m.group(3),16))];continue
            m = r_pixelbrighter.match(line)
            if m: checks += [PixelBrighterThan(int(m.group(1)),int(m.group(2)),int(m.group(3)),int(m.group(4)))];continue
            m = r_pixeldarker.match(line)
            if m: checks += [PixelDarkerThan(int(m.group(1)),int(m.group(2)),int(m.group(3)),int(m.group(4)))];continue
            m = r_pixelequalto.match(line)
            if m: checks += [PixelEqualTo(int(m.group(1)),int(m.group(2)),int(m.group(3)),int(m.group(4)))];continue
            m = r_areaplain.match(line)
            if m: checks += [AreaPlain(int(m.group(1)),int(m.group(2)),int(m.group(3)),int(m.group(4)))];continue
            m = r_areanotplain.match(line)
            if m: checks += [AreaNotPlain(int(m.group(1)),int(m.group(2)),int(m.group(3)),int(m.group(4)))];continue
            if r_width.match(line) or r_height.match(line):
                continue # compatibility
            if r_describe.match(line) or r_end.match(line) or r_header.match(line):
                continue
            print line
            raise Exception("invalid file format: can't load this file (in line %d)" % (nr+1))

        fi.close()
        return Model(filename, checks)

    def save(self):
        path = os.path.splitext(self.filename)[0]+".rb"
        fi = open(path, "wb")
        fi.write("require File.dirname(__FILE__) + '/spec_helper'\n\ndescribe \"pdf conversion\" do\n")
        fi.write("    convert_file \"%s\" do\n" % self.filename)
        for check in self.checks:
            c = check.__class__
            if c == PixelColorCheck:
                fi.write("        pixel_at(%d,%d).should_be_of_color 0x%06x\n" % (check.x,check.y,check.color))
            elif c == PixelBrighterThan:
                fi.write("        pixel_at(%d,%d).should_be_brighter_than pixel_at(%d,%d)\n" % (check.x,check.y,check.x2,check.y2))
            elif c == PixelDarkerThan:
                fi.write("        pixel_at(%d,%d).should_be_darker_than pixel_at(%d,%d)\n" % (check.x,check.y,check.x2,check.y2))
            elif c == PixelEqualTo:
                fi.write("        pixel_at(%d,%d).should_be_the_same_as pixel_at(%d,%d)\n" % (check.x,check.y,check.x2,check.y2))
            elif c == AreaPlain:
                fi.write("        area_at(%d,%d,%d,%d).should_be_plain_colored\n" % (check.x,check.y,check.x2,check.y2))
            elif c == AreaNotPlain:
                fi.write("        area_at(%d,%d,%d,%d).should_not_be_plain_colored\n" % (check.x,check.y,check.x2,check.y2))
        fi.write("    end\n")
        fi.write("end\n")
        fi.close()


class ImageFrame(wx.Frame):
    def __init__(self, application, model):
        wx.Frame.__init__(self, None, -1, style = wx.DEFAULT_FRAME_STYLE, pos=(50,50))
        self.application = application
        self.model = model
        self.Bind(wx.EVT_PAINT, self.OnPaint)
        self.Bind(wx.EVT_MOUSE_EVENTS, self.OnMouse)
        self.bitmap = wx.Bitmap(model.filename)
        self.width = self.bitmap.GetWidth()
        self.height = self.bitmap.GetHeight()
        self.bitmap_x = 500
        self.bitmap_y = 32
        self.SetSize((self.width+self.bitmap_x+32, max(self.height+self.bitmap_y, 15*32)))
        self.image = wx.ImageFromBitmap(self.bitmap)
        self.x = 0
        self.y = 0
        self.createToolbar()

    def createToolbar(self):
        tsize = (16,16)
        self.toolbar = self.CreateToolBar(wx.TB_HORIZONTAL | wx.NO_BORDER | wx.TB_FLAT)
        self.toolbar.AddSimpleTool(wx.ID_OPEN,
                                   wx.ArtProvider.GetBitmap(wx.ART_FILE_OPEN, wx.ART_TOOLBAR, tsize),
                                   "Open")
        self.toolbar.AddSimpleTool(wx.ID_SAVE,
                                   wx.ArtProvider.GetBitmap(wx.ART_FILE_SAVE, wx.ART_TOOLBAR, tsize),
                                   "Save selected pages")
        self.toolbar.AddSimpleTool(wx.ID_PREFERENCES,
                                   wx.ArtProvider.GetBitmap(wx.ART_LIST_VIEW, wx.ART_TOOLBAR, tsize),
                                   "Options")
        #self.toolbar.AddSeparator()
        self.toolbar.Realize()

    def OnMouseClick(self, event):
        x = min(max(event.X  - self.bitmap_x, 0), self.width-1)
        y = min(max(event.Y  - self.bitmap_y, 0), self.height-1)
        if y not in self.model.pixels:
            self.model.pixels[y] = {}
        if y in self.model.pixels and x in self.model.pixels[y]:
            del self.model.pixels[y][x]
        else:
            color = self.image.GetRed(x,y)<<16 | self.image.GetGreen(x,y)<<8 | self.image.GetBlue(x,y)
            self.model.pixels[y][x] = color
        self.Refresh()

    def OnMouse(self, event):
        if event.LeftIsDown():
            return self.OnMouseClick(event)
        lastx = self.x
        lasty = self.y
        self.x = min(max(event.X  - self.bitmap_x, 0), self.width-1)
        self.y = min(max(event.Y  - self.bitmap_y, 0), self.height-1)
        if lastx!=self.x or lasty!=self.y:
            self.Refresh()

    def OnPaint(self, event):
        dc = wx.PaintDC(self)
        self.Draw(dc)

    def Draw(self,dc=None):
        if not dc:
            dc = wx.ClientDC(self)
      
        dc.SetBackground(wx.Brush((0,0,0)))
        dc.DrawBitmap(self.bitmap, self.bitmap_x, self.bitmap_y, False)

        for yy,row in self.model.pixels.items():
            for xx in row.keys():
                x = xx+self.bitmap_x
                y = yy+self.bitmap_y
                l = 0
                for r in range(10):
                    r = (r+1)*3.141526/5
                    dc.DrawLine(x+10*math.sin(l), y+10*math.cos(l), x+10*math.sin(r), y+10*math.cos(r))
                    l = r
                dc.DrawLine(x,y,x+1,y)

        color = (0,255,0)
        for yy in range(15):
            y = self.y+yy-8
            marked = self.model.pixels.get(y, {})
            for xx in range(15):
                x = self.x+xx-8
                if 0<=x<self.width and 0<=y<self.height:
                    color = (self.image.GetRed(x,y), self.image.GetGreen(x,y), self.image.GetBlue(x,y))
                else:
                    color = (0,0,0)
                dc.SetBrush(wx.Brush(color))
                dc.SetPen(wx.Pen(color))
                m = x in marked
                dc.DrawRectangle(32*xx, 32*yy, 32, 32)

                if (xx==8 and yy==8) or m:
                    dc.SetPen(wx.Pen((0, 0, 0)))
                    dc.DrawRectangleRect((32*xx, 32*yy, 32, 32))
                    dc.DrawRectangleRect((32*xx+2, 32*yy+2, 28, 28))

                    if (xx==8 and yy==8):
                        dc.SetPen(wx.Pen((255, 255, 255)))
                    else:
                        dc.SetPen(wx.Pen((255, 255, 0)))
                    dc.DrawRectangleRect((32*xx+1, 32*yy+1, 30, 30))
                    #dc.SetPen(wx.Pen((0, 0, 0)))
                    #dc.SetPen(wx.Pen(color))

class EntryPanel(scrolled.ScrolledPanel):
    def __init__(self, parent, model):
        self.model = model
        scrolled.ScrolledPanel.__init__(self, parent, -1)
        vbox = wx.BoxSizer(wx.VERTICAL)

        for check in model.checks:
            hbox = wx.BoxSizer(wx.HORIZONTAL) 

            desc = wx.StaticText(self, -1, check.left())
            hbox.Add(desc, 0, wx.ALIGN_LEFT|wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5)
           
            if isinstance(check,AreaCheck):
                choices = ["is plain","is not plain"]
                lb = wx.Choice(self, -1, (100, 50), choices = choices)
                hbox.Add(lb, 0, wx.ALIGN_LEFT|wx.ALL, 5)
            elif isinstance(check,TwoPixelCheck):
                choices = ["is the same as","is brighter than","is darker than"]
                lb = wx.Choice(self, -1, (100, 50), choices = choices)
                hbox.Add(lb, 0, wx.ALIGN_LEFT|wx.ALL, 5)
            elif isinstance(check,PixelColorCheck):
                # TODO: color control
                pass
            
            desc = wx.StaticText(self, -1, check.right())
            hbox.Add(desc, 0, wx.ALIGN_LEFT|wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5)

            vbox.Add(hbox)
            vbox.Add(wx.StaticLine(self, -1, size=(360,-1)), 0, wx.ALL, 5)
            #vbox.Add((20,20))

        self.SetSizer(vbox)
        self.SetAutoLayout(1)
        self.SetupScrolling()

class MainFrame(wx.Frame):
    def __init__(self, application, model):
        wx.Frame.__init__(self, None, -1, style = wx.DEFAULT_FRAME_STYLE, pos=(50,50))
        self.application = application
        self.entries = EntryPanel(self, model)
        self.entries.Show()


#class ScrollFrame(wx.Frame):

def getpixels(filename, p):
    model = Model(filename, p)
    app = wx.PySimpleApp()
    main = MainFrame(app, model)
    main.Show()
    app.MainLoop()
    return model.pixels

if __name__ == "__main__":
    model = Model.load(sys.argv[1])
    app = wx.PySimpleApp()
    main = MainFrame(app, model)
    main.Show()
    app.MainLoop()
    model.save()
