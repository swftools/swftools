# coding: utf-8
import wx

def blank_bmp(w, h, color="white"):
    bmp = wx.EmptyBitmap(max(w,1) , max(h,1))
    clear_bmp(bmp, color)
    return bmp

def clear_bmp(bmp, color):
    dc = wx.MemoryDC()
    dc.SelectObject(bmp)
    dc.SetBackground(wx.Brush(color))
    dc.Clear()

encodings = [
'UTF-8',        #
#'US-ASCII',     # (basic English)
#'ISO-8859-1',   # (Western Europe)
#'ISO-8859-2',   # (Central Europe)
#'ISO-8859-3',   # (Southern Europe)
#'ISO-8859-4',   # (Baltic)
#'ISO-8859-5',   # (Cyrillic)
#'ISO-8859-6',   # (Arabic)
#'ISO-8859-7',   # (Greek)
#'ISO-8859-8',   # (Hebrew)
#'ISO-8859-9',   # (Turkish)
'ISO-8859-15',  # (Latin 9)
#'Windows-1250', # (Central Europe)
#'Windows-1251', # (Cyrillic)
'Windows-1252', # (Western Europe)
#'Windows-1253', # (Greek)
#'Windows-1254', # (Turkish)
#'Windows-1255', # (Hebrew)
#'Windows-1256', # (Arabic)
#'Windows-1257', # (Baltic Rim)
#'Windows-1258', # (Vietnam)
#'SHIFT_JIS',    # (Japanese, Win/Mac)
#'BIG5',         # (Traditional Chinese)
#'GB18030',      # (Simplified Chinese)
]

def force_unicode(s):
    if isinstance(s, unicode):
        return s

    sane = repr(s)[1:-2]
    for enc in encodings:
        try:
            sane = s.decode(enc)
            break
        except UnicodeDecodeError:
            continue
    return sane
