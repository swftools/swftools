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

from gui.fields import Choose

order = 4
type = "core"
name = u"No Viewer"
desc = (u'The SWF will be in "raw" format, with each page a seperate frame. '
        u"Use this if you want to add a viewer yourself afterwards or "
        u"if you will use the `One Page Per File`.")

swf_options = [
    Choose("flashversion", u"Flash version:",
           (u"4", 4, u"5", 5, u"6", 6, u"7", 7, u"8", 8, u"9", 9), 2),
    Choose("insertstop", u"Insert stop after each frame:",
           (u"no", 0, u"yes", 1), 0),
    Choose("transparent", u"Make SWF file transparent:",
           (u"no", 0, u"yes", 1), 0),
    ]

viewer_options = []

from gui.plugin import Plugin
class Raw(Plugin):
    one_page_per_file = True
    def __init__(self, swf, filename):
        for opt in swf_options:
            swf.setparameter(opt.name, str(opt.value))

def init(swf, filename):
    return Raw(swf, filename)

from lib.embeddedimage import PyEmbeddedImage
preview = PyEmbeddedImage(
    "iVBORw0KGgoAAAANSUhEUgAAAGYAAABmCAIAAAC2vXM1AAAAAXNSR0IArs4c6QAADGNJREFU"
    "eNrtXX1MFEcU36/74LwDvByNnIqaFL9ORU2MEUNtE6AVIxWjFuN30ESNTWtiIhpBTVDSNk2j"
    "Jq3RfvxhNPEPg2kaVFo1ChgNKQo5vURRMIBIICdwX9zt7Gz/eHZc9/aWQ45DcN8fl7nZudmZ"
    "t++9+b33ZucoSqN3IPFtwhhjjKFAPiOR9GooFBJFsaCgYGyzi4t5j8FgcNOmTTt37kQIMQxD"
    "URRN0/BgaJom/KUoimEYuCR9eLIaqIQC/DxSA7hKWkZqI72LtDK8K+ntBEFgWbaxsXH37t2x"
    "ZxnG+OOPP87MzBx78mU2m4dFymiaDgQCgiAghFiWlT1bQRDu3bvX09MjCEJmZqbVagVxE0WR"
    "YRhBEMhDhkqMMRSgHmPMsizIKZGOuElZX1/fsLAMWAATk7IM7vr8+fOtW7du27YtFArNmzfv"
    "o48+Ar6wLBsKhfR6vUw9WZaFq+QrQojjuDgrJtwa7AwnNRYy2wEF2dVIhoYsI7L1RCp9oijy"
    "PL9w4cJDhw5RFNXW1rZz506DwZCbm9vX11dZWWmxWIqLi5OSkn744Qe/39/Z2bl+/fp//vmH"
    "oqiSkpLk5OSff/65trZ2woQJpaWl48ePVxxwNOOM1D7S3KVzYeJsDoxGY0NDw9dff11eXt7S"
    "0nLz5s29e/euWLEiJyenrKwsFAqdPn163Lhxf/75544dOwoKCg4cOFBaWmqz2a5evXrnzp0r"
    "V678+uuvHMedOHEC7GacocVrKQsXY1Ip/VQxXqRA2hOSqUMwGJwxY0ZpaSnHcU+fPs3Kypo6"
    "dWogEPjxxx9tNpvdbne73R6PZ8mSJbNmzfL5fNnZ2Varddq0aZ2dnRjjQCDwxx9/6PX6GTNm"
    "yMYmHWSkAUfZOHzu0unEXjFlJLuKEMIYp6SkwDi8Xi/G2O1219fXnzt3rqyszOv1iqLo8/l4"
    "ng8Gg4FAQBTFQCAQDAYXLlx4/vz5vLy8cePGQW+wMsRNMYdLymQke1BWq3X58uWgUDabLScn"
    "h6Zpu92+bdu2Y8eOTZ06ddKkSXq9Pjs7m2XZ1NTUTz75hKIoh8PR2dnpcDi2bt1aXl5uNps3"
    "b96cmpoK62mcpYwOtwgqq4bi0yNXeZ7X6XR5eXkLFiw4duyYIsiAMtwRZqvePzRWbCmTiDiA"
    "jNra2qysrEFImeLcpDUcx9E0zXEcy7JQiPQwiWiEjzj8FopypCgyKqxXHEkk32NgWxY9RgUr"
    "oy5lPT09vb29Xq9XUcrIZyRZGCmHyWQyMQyDMVYX+WhZBuixpqZmy5YtixcvJhhdNkkoG43G"
    "Z8+e7dq1SzaB95llCCGfz1dRUSEV/yGxDLru6ekxGAwXLlyIFAuR4oxRR4WFhX6/32KxqCOE"
    "aFkGXGBZ1mAwkLVCZtQJpxBCpBJCQ+BqCIIAPib4UhhjMHYkgiTV4nhKGZBOp5P2ELPgD/BL"
    "2jU4gL/88svdu3cpilq0aNGePXtIe+IbiqIIjiGpkWoi8RnjjOZlZlo2tdiwLBxtgbBcvXq1"
    "sLAwKyuLZdnm5ma/39/R0bFgwYJbt26JopiTk2OxWDo6OjweT2NjY25urtvtbmxszMnJMRqN"
    "NE1XVVUFAoEvvvhCr9cDQFUR9gEx12Bx2WDtCTf050NRVFJSUnNzs9VqXbRo0TfffENR1Lp1"
    "67q6utxu97Nnz65du3bmzJmTJ08+fvx4/vz5v//++7Jly168eOF0Og8ePPjdd995PB6apuvr"
    "6w8fPhwNUhtZio1bLghCc3Ozy+Xq7u5mWbaoqGjNmjWpqakTJ06cM2dOXV2dz+djGCYvL6+k"
    "pARj/Omnn544ceLBgwetra0VFRXz5s1zOBw1NTVtbW2w0r/vgewBfcxwtzE8KrJ9+/bFixdD"
    "5bRp0xBCJ0+eRAh99dVXHMf5fD6TyWS32xFCM2fONJlMwBeSXrDb7cePH7fZbMRzVHcPVQb8"
    "bj6m4gSH5GOqe44Mw/h8PkEQGIZhGAZwnNVqbW1tvXv3rtfr1el0oijCcsnzPKgeQigtLW31"
    "6tX19fUGg4G4DZEUc7htWfjUFNF/VBkmmGRlZWVGRoYs7SQIgiiKLpfr1atXUPPo0SOIRiCE"
    "Ll++fPv2bZfL1d/f39LS0t3djTF+8uSJx+PBGDudTujn2rVrZ8+eraurU8xp4f8pUrpLepWU"
    "I7WRdiJtv3HjRo/HI8urkZaAnGpqamJm/mfOnElqZs2aRXzDL7/8ktRPmTKFJFOg4HA44IHl"
    "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"
    "gg==")
getpreviewData = preview.GetData
getpreviewImage = preview.GetImage
getpreviewBitmap = preview.GetBitmap
getpreviewIcon = preview.GetIcon
