#!/usr/bin/env python
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
    def verifies(self, model):
        return True

class PixelColorCheck(Check):
    def __init__(self, x,y, color):
        Check.__init__(self,x,y)
        self.color = color
    def right(self):
        return "is of color 0x%06x" % self.color
    def verifies(self, model):
        p = model.getPixel(self.x,self.y)
        val = p[0]<<16 | p[1]<<8 | p[2]
        return val == self.color

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

checktypes = [PixelColorCheck,PixelBrighterThan,PixelDarkerThan,PixelEqualTo,AreaPlain,AreaNotPlain]

global TESTMODE

def convert_to_ppm(pdf):
    print pdf
    f = os.popen("pdfinfo "+pdf, "rb")
    info = f.read()
    f.close()
    width,heigth = re.compile(r"Page size:\s*([0-9]+) x ([0-9]+) pts").findall(info)[0]
    dpi = int(72.0 * 612 / int(width))
    if TESTMODE:
        os.system("pdf2swf -s zoom="+str(dpi)+" -p1 "+pdf+" -o test.swf")
        os.system("swfrender --legacy test.swf -o test.png")
        os.unlink("test.swf")
        return "test.png"
    else:
        os.system("pdftoppm -r "+str(dpi)+" -f 1 -l 1 "+pdf+" test")
    return "test-000001.ppm"


class Model:
    def __init__(self, specfile, docfile, checks):
        self.specfile = specfile
        self.docfile = docfile
        self.imgfilename = convert_to_ppm(self.docfile)
        self.bitmap = wx.Bitmap(self.imgfilename)
        self.image = wx.ImageFromBitmap(self.bitmap)
        self.width = self.bitmap.GetWidth()
        self.height = self.bitmap.GetHeight()
        self.checks = checks
        self.xy2check = {}
        self.appendListeners = []
        self.drawModeListeners = []
        self.drawmode = PixelColorCheck

    def close(self):
        try: os.unlink(self.imgfilename)
        except: pass

    def getPixel(self,x,y):
        return (self.image.GetRed(x,y), self.image.GetGreen(x,y), self.image.GetBlue(x,y))
        
    def setdrawmode(self, mode):
        self.drawmode = mode
        for f in self.drawModeListeners:
            f()

    def find(self, x, y):
        return self.xy2check.get((x,y),None)

    def delete(self, check):
        i = self.checks.index(check)
        del self.checks[i]
        del self.xy2check[(check.x,check.y)]
        for f in self.appendListeners:
            f(check)

    def append(self, check):
        self.checks += [check]
        self.xy2check[(check.x,check.y)] = check
        for f in self.appendListeners:
            f(check)

    @staticmethod
    def load(filename):
        # convenience, allow to do "edit_spec.py file.pdf"
        p,ext = os.path.splitext(filename)
        if ext!=".rb":
            path = p+".rb"
            if not os.path.isfile(path):
                path = p+".spec.rb"
                if not os.path.isfile(path):
                    print "No file %s found, creating new..." % path
                    return Model(path, filename, [])
        else:
            path = filename

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
                model = Model(path, filename, [])
                continue
            m = r_pixelcolor.match(line)
            if m: model.append(PixelColorCheck(int(m.group(1)),int(m.group(2)),int(m.group(3),16)));continue
            m = r_pixelbrighter.match(line)
            if m: model.append(PixelBrighterThan(int(m.group(1)),int(m.group(2)),int(m.group(3)),int(m.group(4))));continue
            m = r_pixeldarker.match(line)
            if m: model.append(PixelDarkerThan(int(m.group(1)),int(m.group(2)),int(m.group(3)),int(m.group(4))));continue
            m = r_pixelequalto.match(line)
            if m: model.append(PixelEqualTo(int(m.group(1)),int(m.group(2)),int(m.group(3)),int(m.group(4))));continue
            m = r_areaplain.match(line)
            if m: model.append(AreaPlain(int(m.group(1)),int(m.group(2)),int(m.group(3)),int(m.group(4))));continue
            m = r_areanotplain.match(line)
            if m: model.append(AreaNotPlain(int(m.group(1)),int(m.group(2)),int(m.group(3)),int(m.group(4))));continue
            if r_width.match(line) or r_height.match(line):
                continue # compatibility
            if r_describe.match(line) or r_end.match(line) or r_header.match(line):
                continue
            print line
            raise Exception("invalid file format: can't load this file (in line %d)" % (nr+1))

        fi.close()
        return model

    def save(self):
        path = self.specfile
        fi = open(path, "wb")
        fi.write("require File.dirname(__FILE__) + '/spec_helper'\n\ndescribe \"pdf conversion\" do\n")
        fi.write("    convert_file \"%s\" do\n" % self.docfile)
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

class ZoomWindow(wx.Window):
    def __init__(self, parent, model):
        wx.Window.__init__(self, parent, pos=(0,0), size=(15*32,15*32))
        self.model = model
        self.Bind(wx.EVT_PAINT, self.OnPaint)
        self.x = 0
        self.y = 0

    def setpos(self,x,y):
        self.x = x
        self.y = y
        self.Refresh()
    
    def OnPaint(self, event):
        dc = wx.PaintDC(self)
        self.Draw(dc)
    
    def Draw(self,dc=None):
        if not dc:
            dc = wx.ClientDC(self)
        dc.SetBackground(wx.Brush((0,0,0)))
        color = (0,255,0)
        for yy in range(15):
            y = self.y+yy-8
            for xx in range(15):
                x = self.x+xx-8
                if 0<=x<self.model.width and 0<=y<self.model.height:
                    color = self.model.getPixel(x,y)
                else:
                    color = (0,0,0)
                dc.SetPen(wx.Pen(color))
                m = self.model.find(x,y)
                dc.SetBrush(wx.Brush(color))
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

class ImageWindow(wx.Window):
    def __init__(self, parent, model, zoom):
        wx.Window.__init__(self, parent)
        self.model = model
        self.Bind(wx.EVT_PAINT, self.OnPaint)
        self.Bind(wx.EVT_MOUSE_EVENTS, self.OnMouse)
        self.SetSize((model.width, model.height))
        self.zoom = zoom
        self.x = 0
        self.y = 0
        self.lastx = 0
        self.lasty = 0
        self.firstclick = None
        self.model.drawModeListeners += [self.reset]

    def reset(self):
        self.firstclick = None

    def OnMouseClick(self, event):
        x = min(max(event.X, 0), self.model.width-1)
        y = min(max(event.Y, 0), self.model.height-1)
        if self.model.drawmode == PixelColorCheck:
            check = self.model.find(x,y)
            if check:
                self.model.delete(check)
            else:
                p = slef.model.GetPixel(x,y)
                color = p[0]<<16|p[1]<<8|p[2]
                self.model.append(PixelColorCheck(x,y,color))
        else:
            if not self.firstclick:
                self.firstclick = (x,y)
            else:
                x1,y1 = self.firstclick
                self.model.append(self.model.drawmode(x1,y1,x,y))
                self.firstclick = None

        self.Refresh()

    def OnMouse(self, event):
        if event.LeftIsDown():
            return self.OnMouseClick(event)
        lastx = self.x
        lasty = self.y
        self.x = min(max(event.X, 0), self.model.width-1)
        self.y = min(max(event.Y, 0), self.model.height-1)
        if lastx!=self.x or lasty!=self.y:
            self.zoom.setpos(self.x,self.y)
            self.Refresh()

    def OnPaint(self, event):
        dc = wx.PaintDC(self)
        self.Draw(dc)

    def Draw(self,dc=None):
        if not dc:
            dc = wx.ClientDC(self)
      
        dc.SetBackground(wx.Brush((0,0,0)))
        dc.DrawBitmap(self.model.bitmap, 0, 0, False)

        red = wx.Pen((192,0,0),2)

        if self.firstclick:
            x,y = self.firstclick
            if AreaCheck in self.model.drawmode.__bases__:
                dc.SetBrush(wx.TRANSPARENT_BRUSH)
                dc.DrawRectangle(x,y,self.x-x,self.y-y)
                dc.SetBrush(wx.WHITE_BRUSH)
            elif TwoPixelCheck in self.model.drawmode.__bases__:
                x,y = self.firstclick
                dc.DrawLine(x,y,self.x,self.y)

        for check in self.model.checks:
            if TESTMODE and not check.verifies(model):
                dc.SetPen(red)
            else:
                dc.SetPen(wx.BLACK_PEN)
            if AreaCheck in check.__class__.__bases__:
                dc.SetBrush(wx.TRANSPARENT_BRUSH)
                dc.DrawRectangle(check.x,check.y,check.x2-check.x,check.y2-check.y)
                dc.SetBrush(wx.WHITE_BRUSH)
            else:
                x = check.x
                y = check.y
                l = 0
                for r in range(10):
                    r = (r+1)*3.141526/5
                    dc.DrawLine(x+10*math.sin(l), y+10*math.cos(l), x+10*math.sin(r), y+10*math.cos(r))
                    l = r
                dc.DrawLine(x,y,x+1,y)
                if TwoPixelCheck in check.__class__.__bases__:
                    dc.DrawLine(x,y,check.x2,check.y2)
            dc.SetPen(wx.BLACK_PEN)

class EntryPanel(scrolled.ScrolledPanel):
    def __init__(self, parent, model):
        self.model = model
        scrolled.ScrolledPanel.__init__(self, parent, -1, size=(480,10*32), pos=(0,16*32))
        self.id2check = {}
        self.append(None)

    def delete(self, event):
        self.model.delete(self.id2check[event.Id])

    def append(self, check):
        self.vbox = wx.BoxSizer(wx.VERTICAL)
        self.vbox.Add(wx.StaticLine(self, -1, size=(500,-1)), 0, wx.ALL, 5)
        for nr,check in enumerate(model.checks):
            hbox = wx.BoxSizer(wx.HORIZONTAL) 
            
            button = wx.Button(self, label="X", size=(32,32))
            hbox.Add(button, 0, wx.ALIGN_CENTER_VERTICAL)
            hbox.Add((16,16))
            self.id2check[button.Id] = check
            self.Bind(wx.EVT_BUTTON, self.delete, button)

            def setdefault(lb,nr):
                lb.Select(nr);self.Bind(wx.EVT_CHOICE, lambda lb:lb.EventObject.Select(nr), lb)

            desc = wx.StaticText(self, -1, check.left())

            hbox.Add(desc, 0, wx.ALIGN_LEFT|wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5)
            if isinstance(check,AreaCheck):
                choices = ["is plain","is not plain"]
                lb = wx.Choice(self, -1, (100, 50), choices = choices)
                if isinstance(check,AreaPlain):
                    setdefault(lb,0)
                else:
                    setdefault(lb,1)
                hbox.Add(lb, 0, wx.ALIGN_LEFT|wx.ALL, 5)
            elif isinstance(check,TwoPixelCheck):
                choices = ["is the same as","is brighter than","is darker than"]
                lb = wx.Choice(self, -1, (100, 50), choices = choices)
                hbox.Add(lb, 0, wx.ALIGN_LEFT|wx.ALL, 5)
                if isinstance(check, PixelEqualTo):
                    setdefault(lb,0)
                elif isinstance(check, PixelBrighterThan):
                    setdefault(lb,1)
                elif isinstance(check, PixelDarkerThan):
                    setdefault(lb,2)
            elif isinstance(check,PixelColorCheck):
                # TODO: color control
                pass
            
            desc2 = wx.StaticText(self, -1, check.right())
            hbox.Add(desc2, 0, wx.ALIGN_LEFT|wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5)
            
            self.vbox.Add(hbox)
            self.vbox.Add(wx.StaticLine(self, -1, size=(500,-1)), 0, wx.ALL, 5)
        self.end = wx.Window(self, -1, size=(1,1))
        self.vbox.Add(self.end)
        self.SetSizer(self.vbox)
        self.SetAutoLayout(1)
        self.SetupScrolling(scrollToTop=False)
        self.ScrollChildIntoView(self.end)

class ToolChoiceWindow(wx.Choice):
    def __init__(self, parent, model):
        self.model = model
        self.choices = [c.__name__ for c in checktypes]
        wx.Choice.__init__(self, parent, -1, (100,50), choices = self.choices)
        self.Bind(wx.EVT_CHOICE, self.choice)
    def choice(self, event):
        self.model.setdrawmode(eval(self.choices[self.GetCurrentSelection()]))

class MainFrame(wx.Frame):
    def __init__(self, application, model):
        wx.Frame.__init__(self, None, -1, style = wx.DEFAULT_FRAME_STYLE, pos=(50,50))
        self.application = application
      
        self.toolchoice = ToolChoiceWindow(self, model)
        self.toolchoice.Show()
        self.zoom = ZoomWindow(self, model)
        self.zoom.Show()
        self.image = ImageWindow(self, model, self.zoom)
        self.image.Show()
        self.entries = EntryPanel(self, model)
        self.entries.Show()
        self.createToolbar()
        model.appendListeners += [self.append]
        
        hbox = wx.BoxSizer(wx.HORIZONTAL)
        hbox.Add(self.zoom)
        hbox.Add((16,16))
        vbox = wx.BoxSizer(wx.VERTICAL)
        vbox.Add(self.toolchoice)
        vbox.Add(self.image)
        hbox.Add(vbox)
        #vbox.Add(self.entries)
        self.SetSizer(hbox)
        self.SetAutoLayout(True)
        hbox.Fit(self)

    def append(self, new):
        self.entries.Hide()
        e = self.entries
        del self.entries
        e.Destroy()
        self.entries = EntryPanel(self, model)
        self.entries.Show()

    def createToolbar(self):
        tsize = (16,16)
        self.toolbar = self.CreateToolBar(wx.TB_HORIZONTAL | wx.NO_BORDER | wx.TB_FLAT)
        self.toolbar.AddSimpleTool(wx.ID_CUT,
                                   wx.ArtProvider.GetBitmap(wx.ART_CROSS_MARK, wx.ART_TOOLBAR, tsize),
                                   "Remove")
        self.toolbar.AddSimpleTool(wx.ID_SETUP,
                                   wx.ArtProvider.GetBitmap(wx.ART_TIP, wx.ART_TOOLBAR, tsize),
                                   "Add")
        self.toolbar.AddSimpleTool(wx.ID_SETUP,
                                   wx.ArtProvider.GetBitmap(wx.ART_GO_UP, wx.ART_TOOLBAR, tsize),
                                   "Add")
        #self.toolbar.AddSeparator()
        self.toolbar.Realize()


if __name__ == "__main__":
    from optparse import OptionParser
    global TESTMODE
    parser = OptionParser()
    parser.add_option("-t", "--test", dest="test", help="Test checks against swf", action="store_true")
    (options, args) = parser.parse_args()

    if options.test:
        TESTMODE = True

    app = wx.PySimpleApp()
    model = Model.load(args[0])

    main = MainFrame(app, model)
    main.Show()
    app.MainLoop()
    model.save()
    model.close()
