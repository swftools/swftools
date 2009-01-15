#!/usr/bin/env python
# -*- coding: ISO-8859-15 -*-

import sys
import wx
import os
sys.path+=["../lib/python"]
import gfx
import images

basedir = os.getcwd()

gfx.verbose(3)

#try:
#    gfx.setparameter("wxwindowparams", "1")
#except:
#    gfx.setoption("wxwindowparams", "1")

class StaticData:
    def __init__(self):
        self.simpleviewer_bitmap = wx.BitmapFromImage(wx.ImageFromData(images.simpleviewer_width,images.simpleviewer_height,images.simpleviewer_data))
        self.raw_bitmap = wx.BitmapFromImage(wx.ImageFromData(images.raw_width,images.raw_height,images.raw_data))
        self.motionpaper_bitmap = wx.BitmapFromImage(wx.ImageFromData(images.motionpaper_width,images.motionpaper_height,images.motionpaper_data))
        self.rfxview_bitmap = wx.BitmapFromImage(wx.ImageFromData(images.rfxview_width,images.rfxview_height,images.rfxview_data))
staticdata = None

HTMLTEMPLATE = """<html>
<body style="padding: 0px; margin: 0px">
<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"
        width="%(width)s"
        height="%(height)s"
        codebase="http://active.macromedia.com/flash5/cabs/swflash.cab#version=%(version)d,0,0,0">
        <param name="MOVIE" value="%(swffilename)s">
        <param name="PLAY" value="true">
        <param name="LOOP" value="true">
        <param name="QUALITY" value="high">
        <param name="FLASHVARS" value="%(flashvars)s">
          <embed src="%(swffilename)s" width="%(width)s" height="%(height)s"
                 play="true" ALIGN="" loop="true" quality="high"
                 type="application/x-shockwave-flash"
                 flashvars="%(flashvars)s"
                 pluginspage="http://www.macromedia.com/go/getflashplayer">
          </embed>
</object>
</body>
</html>
"""

def error(msg):
    dlg = wx.MessageDialog(None, msg, "Error", style=wx.OK, pos=wx.DefaultPosition)
    dlg.ShowModal()
    dlg.Destroy()

def savefilestatus(msg):
    dlg = wx.MessageDialog(None, msg, "Save file status", style=wx.OK, pos=wx.DefaultPosition)
    dlg.ShowModal()
    dlg.Destroy()

def swfcombine(params):
    exe = "swfcombine"
    if os.path.sep == '/':
        locations = [os.path.join(basedir, "swfcombine"), 
                     "/usr/local/bin/swfcombine",
                     "/usr/bin/swfcombine"
                    ]
    else:
        locations = [os.path.join(basedir, "swfcombine.exe"), 
                     "c:\\swftools\\swfcombine.exe"]
    for e in locations:
        if os.path.isfile(e):
            exe = e
            break

    if hasattr(os,"spawnv"):
        print "spawnv",exe,params
        ret = os.spawnv(os.P_WAIT, exe, [exe]+params)
        if not ret:
            return

    cmd = exe + " " + (" ".join(params))
    print "system",cmd
    ret = os.system(cmd)
    if ret&0xff00:
        error("Couldn't execute swfcombine.exe- error code "+str(ret))

ICON_SIZE = 64

EVENT_PAGE_CHANGE = 1
EVENT_FILE_CHANGE = 2
EVENT_STATUS_TEXT = 4

class ProgressFrame(wx.Dialog):
    def __init__(self, parent, message=""):
        wx.Dialog.__init__(self, parent, -1, "Progress", size=(350, 150))
        panel = wx.Panel(self, -1)
        self.count = 0
        
        self.msg = wx.StaticText(panel, -1, message, (20,25))
        self.gauge = wx.Gauge(panel, -1, 100, (20, 50), (250, 25))

        self.gauge.SetBezelFace(3)
        self.gauge.SetShadowWidth(3)

        self.Bind(wx.EVT_WINDOW_DESTROY, self.close, id=wx.ID_CLOSE)

    def setProgress(self, num):
        self.gauge.SetValue(int(num))

    def close(self, event):
        print "close"


def swapextension(filename,newext):
    basename,ext = os.path.splitext(filename)
    return basename + "." + newext

def has_different_size_pages(doc):
    width,height = 0,0
    for i in range(1,doc.pages+1):
        page = doc.getPage(i)
        if i==1:
            width,height = page.width,page.height
        else:
            if abs(width-page.width)>2 or \
               abs(height-page.height)>2:
                   return 1
    return 0


options = []
gfx_options = {}

class Option:
    def __init__(self, parameter, text, options, default, mapping=None):
        self.parameter = parameter
        self.text = text
        self.options = options
        self.default = default
        self.mapping = mapping
        self.control = None
        self.enabled = 1
        self.register()

    def generateControl(self, panel):
        if type(self.options) == type((0,)):
            control = wx.Choice(panel, -1, choices=self.options)
            control.SetSelection(self.default)
        elif self.options == "slider":
            control = wx.Slider(panel, -1, self.default, 0, 100, size=(100, -1), style=wx.SL_HORIZONTAL|wx.SL_LABELS|wx.SL_TOP)
        elif self.options == "spinner":
            control = wx.SpinCtrl(panel, -1, str(self.default))
        else:
            control = wx.Choice(panel, -1, choices=["broken"])
            control.SetSelection(0)

        self.control = control
        return self.control

    def getSettings(self):
        value = ""
        if type(self.options) == type((0,)):
            value = self.options[self.control.GetCurrentSelection()]
            if self.mapping and value in self.mapping:
                value = str(self.mapping[value])
            if value == "yes": 
                value = "1"
            elif value == "no":
                value = "0"
            return {self.parameter:value}
        elif self.options == "slider" or self.options == "spinner":
            value = str(self.control.GetValue())
            return {self.parameter:value}

    def register(self):
        global options
        options += [self]

class Option2(Option):

    def __init__(self, parameter, text, options, default, mapping=None):
        Option.__init__(self, parameter, text, options, default, mapping)
        self.enabled = 0

    def generateControl(self, panel):
        p = wx.Panel(panel, -1)
        #p.SetOwnBackgroundColour('#ff0000')
        h = wx.BoxSizer(wx.HORIZONTAL)
        control = wx.Choice(p, -1, choices=self.options)
        control.SetSelection(self.default)
        text = wx.StaticText(p, -1, self.text)
        h.Add(text,1,wx.EXPAND|wx.ALIGN_LEFT|wx.TOP, 5)
        h.Add(control,1,wx.EXPAND|wx.ALIGN_RIGHT|wx.ALIGN_TOP)
        self.control = control
        if self.enabled:
            control.Enable()
        else:
            control.Disable()
        p.SetSizer(h)
        p.Fit()
        return p 
    
    def Disable(self):
        self.enabled=0
        if self.control:
            self.control.Disable()

    def Enable(self):
        self.enabled=1
        if self.control:
            self.control.Enable()
    
    def getSettings(self):
        if not self.enabled:
            return {}
        return Option.getSettings(self)

class ChooseAndText(Option):
    def __init__(self, parameter, text, options, default, editselection, textvalue=""):
        Option.__init__(self, parameter, text, options, default)
        self.editselection = editselection
        self.selection = default
        self.textvalue = textvalue
        self.enabled = 0
        self.choice = None

    def generateControl(self, panel):
        p = wx.Panel(panel, -1)
        h = wx.BoxSizer(wx.HORIZONTAL)
        control = wx.Choice(p, -1, choices=self.options)
        p.Bind(wx.EVT_CHOICE, self.OnChoice, control)
        control.SetSelection(self.default)
        text = wx.StaticText(p, -1, self.text)
        if self.selection == self.editselection:
            edittext = wx.TextCtrl(p, -1, self.textvalue)
            self.textvalue = ""
        else:
            edittext = wx.TextCtrl(p, -1, "")
            edittext.Disable()
        p.Bind(wx.EVT_TEXT, self.OnText, edittext)
        h.Add(text,1,wx.EXPAND|wx.ALIGN_LEFT|wx.TOP, 5)
        h.Add(control,1,wx.EXPAND|wx.ALIGN_RIGHT)
        h.Add(edittext,1,wx.EXPAND|wx.ALIGN_RIGHT)
        self.choice = control
        self.edittext = edittext
        if self.enabled:
            control.Enable()
        else:
            control.Disable()
        p.SetSizer(h)
        p.Fit()
        return p 

    def OnText(self, event):
        text = self.edittext.GetValue()
        text2 = "".join(c for c in text if c.isdigit())
        if text2!=text:
            self.edittext.SetValue(text2)

    def OnChoice(self, event):
        self.selection = self.choice.GetCurrentSelection()
        if self.selection != self.editselection:
            if not self.textvalue and self.edittext.GetValue():
                self.textvalue = self.edittext.GetValue()
            self.edittext.SetValue("")
            self.edittext.Disable()
        else:
            if self.textvalue and not self.edittext.GetValue():
                self.edittext.SetValue(self.textvalue)
                self.textvalue = ""
            self.edittext.Enable()
    
    def Disable(self):
        self.enabled=0
        if not self.choice:
            return
        self.choice.Disable()
        self.edittext.Disable()

    def Enable(self):
        self.enabled=1
        if not self.choice:
            return
        self.choice.Enable()
        if self.choice.GetCurrentSelection() == self.editselection:
            if self.textvalue and not self.edittext.GetValue():
                self.edittext.SetValue(self.textvalue)
                self.textvalue = ""
            self.edittext.Enable()
        else:
            self.edittext.Disable()
    
    def getSettings(self):
        if not self.enabled:
            return {}
        if self.choice.GetCurrentSelection() != self.editselection:
            value = self.options[self.choice.GetCurrentSelection()]
        else:
            value = self.edittext.GetValue().strip()
        return {self.parameter:value}

class TextOption:
    def __init__(self, parameter, label, default=""):
        self.parameter = parameter
        self.label = label
        self.default = default
        self.register()

    def generateControl(self, panel):
        v = wx.BoxSizer(wx.VERTICAL)
        self.control = wx.TextCtrl(panel, -1, self.default, size=(250, -1))
        self.control.Fit()
        return self.control

    def getSettings(self):
        settings = {}
        for items in self.control.GetValue().split(" "):
            if "=" in items:
                l = items.split("=")
                if len(l) == 2:
                    settings[l[0]] = l[1]
        return settings

    def register(self):
        global options
        options += [self]

class RadioOption(Option):
    def __init__(self, text, options):
        self.text = text
        self.options = options
        self.selected = "==nothing=="
        self.radios = []
        self.register()
        
    def generateControl(self, panel):
        control = wx.Panel(panel, -1)
        vsplit = wx.BoxSizer(wx.VERTICAL)
        for i in range(len(self.options)/2):
            text = self.options[i*2]
            if i == 0:
                c = wx.RadioButton(control, -1, text, style=wx.RB_GROUP)
            else:
                c = wx.RadioButton(control, -1, text)
            control.Bind(wx.EVT_RADIOBUTTON, self.OnRadio, c)
            self.radios += [c]
            vsplit.Add(c)
        control.SetSizer(vsplit)
        control.Fit()
        self.control = control
        return control

    def OnRadio(self, event):
        self.selected = event.GetEventObject().GetLabel()

    def getSettings(self):
        for i in range(len(self.options)/2):
            if self.options[i*2] == self.selected:
                return self.options[i*2+1]
        return self.options[1]

class BitmapWindow(wx.Window):
    def __init__(self, parent, image):
        wx.Window.__init__(self, parent, -1)
        self.image = image
        self.SetMinSize((image.GetWidth()+2, image.GetHeight()+2))
        self.SetMaxSize((image.GetWidth()+2, image.GetHeight()+2))
        self.SetSize((image.GetWidth()+2, image.GetHeight()+2))
        self.Bind(wx.EVT_PAINT, self.OnPaint)
        self.Update()
    def OnPaint(self, event):
        dc = wx.PaintDC(self)
        self.Draw(dc)
    def Draw(self,dc=None):
        if not dc:
            dc = wx.ClientDC(self)
        dc.DrawRectangleRect((0, 0, self.image.GetWidth()+2, self.image.GetHeight()+2))
        dc.DrawBitmap(self.image, 1, 1, False)

class ImageRadioOption(Option):
    def __init__(self, text, options):
        self.text = text
        self.options = options
        self.selected = "==nothing=="
        self.radios = []
        self.register()
        self.ids = []
        
    def generateControl(self, panel):
        control = wx.Panel(panel, -1)
        vsplit = wx.BoxSizer(wx.VERTICAL)
        first = 1
        for image,text,params,selected,extraoptions in self.options:
            hsplit = wx.BoxSizer(wx.HORIZONTAL)

            v = wx.BoxSizer(wx.VERTICAL)

            name,text = text.split("- ")

            c = wx.CheckBox(control, -1, name)
            control.Bind(wx.EVT_CHECKBOX, self.OnRadio, c)

            # radio buttons crash windows when clicked on- even without event bindings.
            # This is caused by the subpanel which is created for extra options
            # (I tried this with a empty Panel(), and even that crashed)
            #if first:
            #    c = wx.RadioButton(control, -1, name, style=wx.RB_GROUP)
            #else:
            #    c = wx.RadioButton(control, -1, name)
            #control.Bind(wx.EVT_RADIOBUTTON, self.OnRadio, c)

            self.ids += [c.GetId()]

            first = 0

            if "disable" in text:
                c.Enable(False)
            if selected:
                self.selected = c.GetId()
                c.SetValue(True)
            else:
                c.SetValue(False)
            self.radios += [c]

            bitmap = BitmapWindow(control, image)
            t = wx.StaticText(control, -1, text, size=(400,50))
            
            v.Add(c, 0, wx.EXPAND)
            v.Add(t, 0, wx.EXPAND|wx.LEFT, 20)
           
            for o in extraoptions:
                cx = o.generateControl(control)
                if selected:
                    o.Enable()
                else:
                    o.Disable()
                v.Add(cx, 0, wx.EXPAND|wx.LEFT, 20)
            
            v.SetMinSize((330,170))
            
            hsplit.Add(bitmap, 0, wx.LEFT|wx.RIGHT|wx.BOTTOM|wx.ALIGN_TOP, 5)
            hsplit.Add(v, 0, wx.EXPAND)
            vsplit.Add(hsplit, 0, wx.EXPAND)

        control.SetSizer(vsplit)
        control.Fit()
        self.control = control
        return vsplit

    def OnRadio(self, event):
        self.selected = event.GetEventObject().GetId()
        for c in self.radios:
            if c.GetId() == self.selected:
                c.SetValue(1)
            else:
                c.SetValue(0)
        i = 0
        for image,text,params,selected,extraoptions in self.options:
            if self.ids[i] == self.selected:
                for xo in extraoptions:
                    xo.Enable()
                pass
            else:
                for xo in extraoptions:
                    xo.Disable()
                pass
            i = i + 1
        event.ResumePropagation(0)

    def getSettings(self):
        i = 0
        for image,text,params,s,extraoptions in self.options:
            id = self.ids[i]
            i = i + 1
            if id == self.selected:
                return params
        return {}


class OptionFrame(wx.Dialog):

    def __init__(self, parent):
        wx.Dialog.__init__(self, parent, -1, "Options")

        #self.nb = wx.Notebook(self, -1)#, wx.Point(0,0), wx.Size(0,0), wxNB_FIXEDWIDTH)
        self.nb = wx.Notebook(self, -1)

        self.needreload = 0
        
        options0 = [RadioOption('Rendering mode', 
                        ["Convert polygons to polygons and fonts to fonts", {},
                         "Convert fonts to fonts, everything else to bitmaps", {"poly2bitmap":"1"},
                         "Convert everthing to bitmaps", {"poly2bitmap":"1", "bitmapfonts":"1"}
                        ])]

        mp_options = []
        sv_options = [Option2('flashversion', 'Flash version:', ('4','5','6','7','8'), 2),
                      Option2('transparent', 'Make SWF file transparent:', ('no','yes'), 0),
                     ]

        raw_options = [Option2('flashversion', 'Flash version:', ('4','5','6','7','8','9'), 2),
                       Option2('insertstop', 'Insert stop after each frame:', ('no','yes'), 0),
                       Option2('transparent', 'Make SWF file transparent:', ('no','yes'), 0),
                      ]
        rfxview_options = [ChooseAndText('rfxwidth', 'Width:', ('same as PDF','fullscreen','custom'),1,2,"600"),
                           ChooseAndText('rfxheight', 'Height:', ('same as PDF','fullscreen','custom'),1,2,"800"),
                           Option2('rfxzoomtype', 'Initial zoom level:', ('Original resolution','Show all','Maximum width/height'),2),
                          ]

        options4 = [ImageRadioOption('Select Paging GUI', 
                        [(staticdata.raw_bitmap, "No Viewer- The SWF will be in \"raw\" format, with each page a seperate frame. Use this if you want to add a viewer yourself afterwards.", {}, 0, raw_options),
                         (staticdata.simpleviewer_bitmap, "SimpleViewer- A tiny viewer, which attaches directly to the SWF, and provides small previous/next buttons in the upper left corner", {"simpleviewer":"1", "insertstop":"1"}, 0, sv_options),
                         (staticdata.rfxview_bitmap, "rfxView- A more sophisticated viewer with zooming and scrolling.", {"rfxview":"1", "flashversion":"8"}, 1, rfxview_options),
                         #(staticdata.motionpaper_bitmap, "MotionPaper- A highly sophisticated viewer with page flipping. (disabled in this evaluation version)", {}, 0, mp_options),
                         #(staticdata.motionpaper_bitmap, "Your advertisement here- Are you are company who developed a viewer for pdf2swf, or who offers commercial PDF hosting service? Place your advertisement or demo viewer here, or allow pdf2swf to upload SWFs directly to your site! contact sales@swftools.org for details.", {}, 0, mp_options),
                        ])]

        options1 = [Option('zoom', 'Resolution (in dpi):', "spinner", 72),
                    Option('fontquality', 'Font quality:', "slider", 20),
                    Option('storeallcharacters', 'Insert full fonts in SWF file:', ('no','yes'), 0),
                    Option('splinequality', 'Polygon quality:', "slider", 100),
                    Option('jpegquality', 'JPEG quality:', "slider", 75),
                    Option('jpegsubpixels', 'JPEG image resolution:', ('same as in PDF', '1x', '2x', '4x'), 0, {"same as in PDF": 0, "1x": 1, "2x": 2, "3x": 3}),
                    Option('ppmsubpixels', 'non-JPEG image resolution:', ('same as in PDF', '1x', '2x', '4x'), 0, {"same as in PDF": 0, "1x": 1, "2x": 2, "3x": 3}),
                   ]
        
        
        options3 = [TextOption('_additional_', 'Additional options')]

        panel1 = [('Rendering options', options0,''),
                  ('Quality',options1,'v')]
        panel3 = [('Select paging GUI', options4,'')]
        panel4 = [('Additional options', options3,'')]

        panels = [('Quality', panel1),
                  ('Viewer', panel3),
                  ('Advanced', panel4)]

        for name,poptions in panels:
            panel = wx.Panel(self.nb, -1)
            self.nb.AddPage(panel, name)
        
            vsplit = wx.BoxSizer(wx.VERTICAL)
      
            for name,options,align in poptions:
                optiongroup = wx.StaticBox(panel, -1, name)
                optiongroupsizer= wx.StaticBoxSizer(optiongroup, wx.VERTICAL)
                optiongroup.SetSizer(optiongroupsizer)

                if align == 'v':
                    grid = wx.GridSizer(rows=len(options), cols=2, hgap=3, vgap=3)
                    optiongroupsizer.Add(grid, 1, wx.EXPAND, 0)
                else:
                    grid = wx.GridSizer(rows=len(options), cols=1, hgap=3, vgap=3)
                    optiongroupsizer.Add(grid, 1, wx.EXPAND, 0)

                for option in options:
                    if align=='v':
                        t = wx.StaticText(panel, -1, option.text)
                        grid.Add(t, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_LEFT)
                    optionbox = option.generateControl(panel)
                    grid.Add(optionbox, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_LEFT)

                vsplit.Add(optiongroupsizer, 0, wx.EXPAND, 0)

            #hs = wx.BoxSizer(wx.HORIZONTAL)
            #hs.Add(gobutton, 0, wx.ALIGN_CENTER, 0)
            gobutton = wx.Button(panel, -1, "Apply")
            self.Bind(wx.EVT_BUTTON, self.Apply, gobutton)

            vsplit.Add(gobutton, 0, wx.ALIGN_CENTER|wx.ALL, 0)
            
            panel.SetSizer(vsplit)
            panel.Fit()

        self.nb.Fit()

        self.Fit()
        

    def updateOptions(self):
        global options,gfx_options
        a = []

        # FIXME: we clear *our* options- but gfx will still have
        #        stored the old ones. Critical for options in the "imageradio" section.
        gfx_options.clear()
        i = 0
        print "----- options ------"
        for option in options:
            for k,v in option.getSettings().items():
                gfx_options[k] = v
                gfx.setparameter(k,v)
                print k,v
            i = i + 1

        # TODO: filter out "global" options, and do this only if
        # pdf layer is affected
    
    def Apply(self, event):
        self.updateOptions()
        self.Hide()
        self.needreload = 1


class State:
    def __init__(self):
        self.pdf = None
        self.page = None
        self.pagenr = 1
        self.pagebitmap = None
        self.bitmap_width = 0
        self.bitmap_height = 0
        self.bitmap_page = 0
        self.filename = None
        self.status_text = None
        self.lastsavefile = "output.swf"
        self.lasthtmlfile = "index.html"

        self.listeners = []

    def onEvent(self,event_type, function):
        self.listeners += [(event_type,function)]
    def loadPDF(self,filename):
        self.filename = filename
        self.lastsavefile = swapextension(filename,"swf")
        self.lasthtmlfile = swapextension(filename,"html")

        self.pdf = gfx.open("pdf",filename)
        if(has_different_size_pages(self.pdf)):
            # just let the user know- for now, we can't handle this properly
            dlg = wx.MessageDialog(app.frame, """In this PDF, width or height are not the same for each page. This might cause problems if you export pages of different dimensions into the same SWF file.""", "Notice", style=wx.OK, pos=wx.DefaultPosition)
            dlg.ShowModal()
            dlg.Destroy()

        self.changePage(1)

        for type,f in self.listeners:
            if type&EVENT_PAGE_CHANGE or type&EVENT_FILE_CHANGE:
                f()
        self.setStatus("File loaded successfully.")

    def saveSWF(self, filename, progress, pages=None, html=0):
        if html:
            basename,ext = os.path.splitext(filename)
            if not ext:
                html = basename + ".html"
                filename = basename + ".swf"
            elif ext.lower() != ".swf":
                html = filename
                filename = basename + ".swf"
            else:
                html = basename + ".html"
                filename = filename

        steps = 100.0 / (self.pdf.pages*2 + 3)
        pos = [0]
        
        self.lastsavefile = filename
        if html:
            self.lasthtmlfile = html

        swf = gfx.SWF()
        for k,v in gfx_options.items():
            swf.setparameter(k,v)
        if pages is None:
            pages = range(1,self.pdf.pages+1)
        pdfwidth,pdfheight=0,0
        for pagenr in pages:
            page = self.pdf.getPage(pagenr)
            pdfwidth = page.width
            pdfheight = page.height
            swf.startpage(page.width, page.height)
            page.render(swf)
            swf.endpage()
        swf.save(filename)

        if gfx_options.get("rfxview",None):
            rfxview = os.path.join(basedir, "rfxview.swf")
            if not os.path.isfile(rfxview):
                error("File rfxview.swf not found in working directory")
            else:
                swfcombine([rfxview,"viewport="+filename,"-o",filename])
        
        if html:
            version = int(gfx_options.get("flashversion", "8"))
            swf = gfx.open("swf", filename)
            page1 = swf.getPage(1)

            width,height = str(page1.width),str(page1.height)


            w = gfx_options.get("rfxwidth","")
            if w == "fullscreen":    width = "100%"
            elif w == "same as PDF": width = pdfwidth+40
            elif w.isdigit():        width = w
            else:                    width = pdfwidth
            
            h = gfx_options.get("rfxheight","")
            if h == "fullscreen":    height = "100%"
            elif h == "same as PDF": height = pdfheight+70
            elif h.isdigit():        height = h
            else:                    height = pdfwidth

            flashvars = ""
            zoomtype = gfx_options.get("rfxzoomtype","")
            if zoomtype=="Original resolution":
                flashvars = "zoomtype=1"
            elif zoomtype=="Show all":
                flashvars = "zoomtype=2"
            elif zoomtype=="Maximum width/height":
                flashvars = "zoomtype=3"

            swffilename = os.path.basename(filename)
            fi = open(html, "wb")
            fi.write(HTMLTEMPLATE % locals())
            fi.close()


    def changePage(self,page):
        self.pagenr = page
        self.page = self.pdf.getPage(self.pagenr)
        for type,f in self.listeners:
            if type&EVENT_PAGE_CHANGE:
                f()

    def getPageIcon(self,pagenr):
        page = self.pdf.getPage(pagenr)
        return wx.BitmapFromImage(wx.ImageFromData(ICON_SIZE,ICON_SIZE,page.asImage(ICON_SIZE,ICON_SIZE)))
        #return wx.BitmapFromImage(wx.ImageFromData(8,8,"0"*(64*3)))

    def getPageImage(self, width, height):
        if self.bitmap_width == width and self.bitmap_height == height and self.bitmap_page == self.pagenr:
            return self.pagebitmap
        else:
            self.bitmap_width = width
            self.bitmap_height = height
            self.bitmap_page = self.pagenr
            self.pagebitmap = wx.BitmapFromImage(wx.ImageFromData(width,height,self.page.asImage(width,height)))
            #self.pagebitmap = wx.BitmapFromImage(wx.ImageFromData(8,8,"0"*(64*3)))
            return self.pagebitmap

    def setStatus(self,text):
        self.status_text = text
        for type,f in self.listeners:
            if type&EVENT_STATUS_TEXT:
                f()

state = State()

class PageListWidget(wx.ListCtrl):
    def __init__(self,parent):
        wx.ListCtrl.__init__(self,parent,style=wx.LC_ICON|wx.LC_AUTOARRANGE)
        #self.SetMinSize((ICON_SIZE+8,-1))
        #self.SetMaxSize((ICON_SIZE+8,-1))
        #self.SetSize((ICON_SIZE+8,-1))
        self.Bind(wx.EVT_LIST_ITEM_SELECTED, self.SelectItem)
        state.onEvent(EVENT_FILE_CHANGE, self.reload)
        state.onEvent(EVENT_PAGE_CHANGE, self.switchPage)
        self.reload()
        self.dontcare = 0
        #self.Bind(wx.EVT_IDLE, self.OnIdle)
        #print dir(self)

    def processFiles(self):
        if self.filepos >= 0 and self.filepos < state.pdf.pages:
            icon = state.getPageIcon(self.filepos+1)
            self.imglist.Add(icon)
            self.InsertImageStringItem(self.filepos, str(self.filepos+1), self.filepos)
            self.filepos = self.filepos + 1
        self.Update()

    def OnIdle(self,event):
        self.processFiles()
        event.ResumePropagation(0)

    def reload(self):
        self.filepos = -1
        self.DeleteAllItems()
        self.imglist = wx.ImageList(ICON_SIZE,ICON_SIZE,mask=False)
        self.AssignImageList(self.imglist,wx.IMAGE_LIST_NORMAL)
        self.filepos = 0
        while state.pdf and self.filepos < state.pdf.pages:
            self.processFiles()

    def switchPage(self):
        if self.dontcare:
            self.dontcare = 0
            return
        for i in range(0,self.GetItemCount()):
            self.Select(i, False)
        self.Select(state.pagenr-1, True)
        self.Focus(state.pagenr-1)
        self.Update()

    def SelectItem(self,event):
        self.dontcare = 1 #ignore next change event
        state.changePage(event.GetIndex()+1)


helptxt = """
This is the SWF preview window.
Here, you will see how the SWF file generated from
the PDF file will look like. Changing parameters in
the configuration which affect the appeareance of
the final SWF will affect this preview, too, so you
can always evaluate the final output beforehand.
"""

        
class OnePageWidget(wx.Window):
    def __init__(self,parent):
        wx.Window.__init__(self, parent)
        self.SetSize((160,100))
        self.SetMinSize((160,100))
        self.Fit()
        self.Bind(wx.EVT_PAINT, self.OnPaint)
        self.Bind(wx.EVT_SIZE, self.OnSize)
        self.Bind(wx.EVT_KEY_DOWN, self.key_down)
        state.onEvent(EVENT_PAGE_CHANGE, self.OnPageChange)
    
    def key_down(self, event):
        if state.pdf:
            if event.GetKeyCode() == 312 and state.pagenr>1:
                state.changePage(state.pagenr-1)
            elif event.GetKeyCode() == 313 and state.pagenr<state.pdf.pages:
                state.changePage(state.pagenr+1)

    def OnPageChange(self):
        self.Refresh()

    def OnSize(self, event):
        self.Refresh()

    def Draw(self,dc=None):
        global bitmap
        if not dc:
            dc = wx.ClientDC(self)
        posx = 0 
        posy = 0
        window_width,window_height = self.GetSize()
        dc.Clear()

        if not state.pdf or not state.page:
            return

        if state.page.width * window_height > state.page.height * window_width:
            width = window_width
            height = window_width * state.page.height / state.page.width
            posy = (window_height - height) / 2
        else:
            width = window_height * state.page.width / state.page.height
            height = window_height
            posx = (window_width - width) / 2

        dc.DrawBitmap(state.getPageImage(width,height), posx,posy, False)
        #state.getPageImage(

    def OnPaint(self, event):
        dc = wx.PaintDC(self)
        self.Draw(dc)

class Pdf2swfFrame(wx.Frame):
    #def __init__(self):
        #wx.Window.__init__(self, None, id=-1, pos=wx.DefaultPosition, size=wx.DefaultSize)
    def __init__(self,application):
        wx.Frame.__init__(self, None, -1, style = wx.DEFAULT_FRAME_STYLE)
        self.application = application
        
        self.SetTitle("pdf2swf")
        self.createMenu()
        self.createToolbar()
        self.createStatusBar()
        self.createMainFrame()
        
        self.SetSize((800,600))

        self.options = OptionFrame(None)
        self.options.Show(False)
        self.options.updateOptions()

        state.onEvent(EVENT_STATUS_TEXT, self.status_change)
        self.html = 0
       
        #self.table = wx.AcceleratorTable([(wx.ACCEL_ALT,  ord('X'), 333),])
        #self.SetAcceleratorTable(self.table)

        self.Bind(wx.EVT_IDLE, self.OnIdle)
        self.Bind(wx.EVT_CLOSE, self.menu_exit)
        return

    def menu_open(self,event):
        global state
        if state.filename:
            dlg = wx.FileDialog(self, "Choose PDF File:", style = wx.DD_DEFAULT_STYLE, defaultFile = state.filename, wildcard = "PDF files (*.pdf)|*.pdf|all files (*.*)|*.*")
        else:
            dlg = wx.FileDialog(self, "Choose PDF File:", style = wx.DD_DEFAULT_STYLE, wildcard = "PDF files (*.pdf)|*.pdf|all files (*.*)|*.*")

        if dlg.ShowModal() == wx.ID_OK:
            self.filename = dlg.GetFilename() 
            state.loadPDF(self.filename)

    def menu_save(self,event,pages=None):
        html,self.html = self.html,0
        global state
        if not state.pdf:
            return
        print "html",html
        if not html:
            defaultFile = state.lastsavefile
        else:
            defaultFile = state.lasthtmlfile
        dlg = wx.FileDialog(self, "Choose Save Filename:", style = wx.SAVE | wx.OVERWRITE_PROMPT, defaultFile = defaultFile, wildcard = "all files (*.*)|*.*|SWF files (*.swf)|*.swf|HTML template (*.html)|*.html")
        
        if dlg.ShowModal() == wx.ID_OK:
            filename = os.path.join(dlg.GetDirectory(),dlg.GetFilename())
        
            #progress = ProgressFrame(self, "Saving %s File '%s'..." % (html and "HTML" or "SWF", filename))
            #progress.Show(True)
            progress = None
            state.saveSWF(filename, progress, pages, html)
            #progress.Destroy()
    
    def menu_save_selected(self,event):
        if not state.pdf:
            return
        p = []
        for i in range(0,self.pagelist.GetItemCount()):
            if self.pagelist.IsSelected(i):
                p += [i+1]
        self.menu_save(event, pages=p)

    def menu_save_html(self,event):
        self.html = 1
        return self.menu_save(event)

    def menu_save_selected_html(self,event):
        self.html = 1
        return self.menu_save_selected(event)

    def menu_exit(self,event):
        self.application.Exit()

    def menu_selectall(self,event):
        for i in range(0,self.pagelist.GetItemCount()):
            self.pagelist.Select(i, True)
    def menu_options(self,event):
        self.options.Show(True)

    def status_change(self):
        self.statusbar.SetStatusText(state.status_text)

    def OnIdle(self,event):
        if self.options.needreload:
            self.options.needreload = 0
            if state.pdf:
                # reload
                state.loadPDF(state.filename)

    def createMenu(self):
        menubar = wx.MenuBar()

        menu = wx.Menu();menubar.Append(menu, "&File")
        menu.Append(wx.ID_OPEN, "Open PDF\tCTRL-O");self.Bind(wx.EVT_MENU, self.menu_open, id=wx.ID_OPEN)
        menu.AppendSeparator()
        menu.Append(wx.ID_SAVE, "Save SWF (all pages)\tCTRL-W");self.Bind(wx.EVT_MENU, self.menu_save, id=wx.ID_SAVE)
        menu.Append(wx.ID_SAVEAS, "Save SWF (selected pages)\tCTRL-S");self.Bind(wx.EVT_MENU, self.menu_save_selected, id=wx.ID_SAVEAS)
        menu.AppendSeparator()
        menu.Append(2001, "Save HTML template (all pages)\tCTRL-H");self.Bind(wx.EVT_MENU, self.menu_save_html, id=2001)
        menu.Append(2002, "Save HTML template (selected pages)");self.Bind(wx.EVT_MENU, self.menu_save_selected_html, id=2002)
        menu.AppendSeparator()
        menu.Append(wx.ID_EXIT, "Exit\tCTRL-Q");self.Bind(wx.EVT_MENU, self.menu_exit, id=wx.ID_EXIT)

        menu = wx.Menu();menubar.Append(menu, "&Edit")
        menu.Append(wx.ID_SELECTALL, "Select All\tCTRL-A");self.Bind(wx.EVT_MENU, self.menu_selectall, id=wx.ID_SELECTALL)
        menu.AppendSeparator()
        menu.Append(wx.ID_PREFERENCES, "Options\tCTRL-R");self.Bind(wx.EVT_MENU, self.menu_options, id=wx.ID_PREFERENCES)

        menu = wx.Menu();menubar.Append(menu, "&Help")

        self.SetMenuBar(menubar)


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

    def createStatusBar(self):
        self.statusbar = self.CreateStatusBar(1)

    def createMainFrame(self):
        
        if 0:
            self.pagelist = PageListWidget(self)
            self.onepage = OnePageWidget(self)
            hsplit = wx.BoxSizer(wx.HORIZONTAL)
            pagelistbox = wx.StaticBox(self, -1, "Pages")
            pagelistboxsizer= wx.StaticBoxSizer(pagelistbox, wx.VERTICAL)
            pagelistboxsizer.Add(self.pagelist, proportion=1, flag=wx.EXPAND)
            onepagebox = wx.StaticBox(self, -1, "Page 1")
            onepageboxsizer= wx.StaticBoxSizer(onepagebox, wx.VERTICAL)
            onepageboxsizer.Add(self.onepage, proportion=1, flag=wx.EXPAND)
            hsplit.Add(pagelistboxsizer, 0, wx.EXPAND, 0)
            hsplit.Add(onepageboxsizer, 1, wx.EXPAND, 0)
            self.SetAutoLayout(True)
            self.SetSizer(hsplit)
            hsplit.Fit(self)
            hsplit.SetSizeHints(self)
        else:
            hsplit = wx.SplitterWindow(self, style=wx.SP_3D|wx.SP_LIVE_UPDATE)
            #p1 = wx.Panel(hsplit,-1, style=wx.SUNKEN_BORDER)
            #p2 = wx.Panel(hsplit,-1, style=wx.SUNKEN_BORDER)
            self.pagelist = PageListWidget(hsplit)
            self.onepage = OnePageWidget(hsplit)
            #hsplit.SplitVertically(p1,p2, sashPosition=64)
            hsplit.SplitVertically(self.pagelist, self.onepage, sashPosition=ICON_SIZE*3/2)
            hsplit.SetMinimumPaneSize(10)

#class TestWindow(wx.SplitterWindow):
#    def __init__(self,parent):
#        wx.SplitterWindow.__init__(self, parent, -1)
#        panel = wx.Panel(self, -1)
#        self.txt = wx.StaticText(panel, -1, "bla bla bla", (100,10), (160,-1), wx.ALIGN_CENTER|wx.ALIGN_CENTER_VERTICAL)
#        self.SetMinSize((320,200))
#        self.txt.SetForegroundColour("blue")
#        self.txt.SetBackgroundColour("green")
#        self.Fit()
#        self.Initialize(panel)
#
#class TestFrame(wx.Frame):
#    def __init__(self,parent):
#        wx.Frame.__init__(self, None, -1)
#        panel = wx.Panel(self, -1)
#        window = TestWindow(panel)
#        window.Show()

class MyApp(wx.App):
    def __init__(self):
        wx.App.__init__(self, redirect=False, filename=None, useBestVisual=False)
        
        #state.loadPDF("sitis2007.pdf")
        #state.loadPDF("wxPython-Advanced-OSCON2004.pdf")
        global staticdata
        staticdata = StaticData()

        self.frame = Pdf2swfFrame(self)
        self.SetTopWindow(self.frame)
        self.frame.Show(True)

        #self.frame = TestFrame(self)
        #self.frame.Show(True)

    def OnInit(self):
        return True

app = MyApp()
app.MainLoop()
        
