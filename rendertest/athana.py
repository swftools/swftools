#!/usr/bin/python
"""
 Athana - standalone web server including the TAL template language

 Copyright (C) 2007 Matthias Kramm <kramm@in.tum.de>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

#===============================================================
#
# Athana
#
# A standalone webserver based on Medusa and the Zope TAL Parser
#
# This file is distributed under the GPL, see file COPYING for details.
#
#===============================================================
"""
Parse HTML and compile to TALInterpreter intermediate code.
"""

RCS_ID =  '$Id: athana.py,v 1.15 2007/11/23 10:13:32 kramm Exp $'

import sys

from HTMLParser import HTMLParser, HTMLParseError

BOOLEAN_HTML_ATTRS = [
    "compact", "nowrap", "ismap", "declare", "noshade", "checked",
    "disabled", "readonly", "multiple", "selected", "noresize",
    "defer"
    ]

EMPTY_HTML_TAGS = [
    "base", "meta", "link", "hr", "br", "param", "img", "area",
    "input", "col", "basefont", "isindex", "frame",
    ]

PARA_LEVEL_HTML_TAGS = [
    "h1", "h2", "h3", "h4", "h5", "h6", "p",
    ]

BLOCK_CLOSING_TAG_MAP = {
    "tr": ("tr", "td", "th"),
    "td": ("td", "th"),
    "th": ("td", "th"),
    "li": ("li",),
    "dd": ("dd", "dt"),
    "dt": ("dd", "dt"),
    }

BLOCK_LEVEL_HTML_TAGS = [
    "blockquote", "table", "tr", "th", "td", "thead", "tfoot", "tbody",
    "noframe", "ul", "ol", "li", "dl", "dt", "dd", "div",
    ]

TIGHTEN_IMPLICIT_CLOSE_TAGS = (PARA_LEVEL_HTML_TAGS
                               + BLOCK_CLOSING_TAG_MAP.keys())


class NestingError(HTMLParseError):
    """Exception raised when elements aren't properly nested."""

    def __init__(self, tagstack, endtag, position=(None, None)):
        self.endtag = endtag
        if tagstack:
            if len(tagstack) == 1:
                msg = ('Open tag <%s> does not match close tag </%s>'
                       % (tagstack[0], endtag))
            else:
                msg = ('Open tags <%s> do not match close tag </%s>'
                       % ('>, <'.join(tagstack), endtag))
        else:
            msg = 'No tags are open to match </%s>' % endtag
        HTMLParseError.__init__(self, msg, position)

class EmptyTagError(NestingError):
    """Exception raised when empty elements have an end tag."""

    def __init__(self, tag, position=(None, None)):
        self.tag = tag
        msg = 'Close tag </%s> should be removed' % tag
        HTMLParseError.__init__(self, msg, position)

class OpenTagError(NestingError):
    """Exception raised when a tag is not allowed in another tag."""

    def __init__(self, tagstack, tag, position=(None, None)):
        self.tag = tag
        msg = 'Tag <%s> is not allowed in <%s>' % (tag, tagstack[-1])
        HTMLParseError.__init__(self, msg, position)

class HTMLTALParser(HTMLParser):


    def __init__(self, gen=None):
        HTMLParser.__init__(self)
        if gen is None:
            gen = TALGenerator(xml=0)
        self.gen = gen
        self.tagstack = []
        self.nsstack = []
        self.nsdict = {'tal': ZOPE_TAL_NS,
                       'metal': ZOPE_METAL_NS,
                       'i18n': ZOPE_I18N_NS,
                       }

    def parseFile(self, file):
        f = open(file)
        data = f.read()
        f.close()
        try:
            self.parseString(data)
        except TALError, e:
            e.setFile(file)
            raise

    def parseString(self, data):
        self.feed(data)
        self.close()
        while self.tagstack:
            self.implied_endtag(self.tagstack[-1], 2)
        assert self.nsstack == [], self.nsstack

    def getCode(self):
        return self.gen.getCode()

    def getWarnings(self):
        return ()


    def handle_starttag(self, tag, attrs):
        self.close_para_tags(tag)
        self.scan_xmlns(attrs)
        tag, attrlist, taldict, metaldict, i18ndict \
             = self.process_ns(tag, attrs)
        if tag in EMPTY_HTML_TAGS and taldict.get("content"):
            raise TALError(
                "empty HTML tags cannot use tal:content: %s" % `tag`,
                self.getpos())
        self.tagstack.append(tag)
        self.gen.emitStartElement(tag, attrlist, taldict, metaldict, i18ndict,
                                  self.getpos())
        if tag in EMPTY_HTML_TAGS:
            self.implied_endtag(tag, -1)

    def handle_startendtag(self, tag, attrs):
        self.close_para_tags(tag)
        self.scan_xmlns(attrs)
        tag, attrlist, taldict, metaldict, i18ndict \
             = self.process_ns(tag, attrs)
        if taldict.get("content"):
            if tag in EMPTY_HTML_TAGS:
                raise TALError(
                    "empty HTML tags cannot use tal:content: %s" % `tag`,
                    self.getpos())
            self.gen.emitStartElement(tag, attrlist, taldict, metaldict,
                                      i18ndict, self.getpos())
            self.gen.emitEndElement(tag, implied=-1)
        else:
            self.gen.emitStartElement(tag, attrlist, taldict, metaldict,
                                      i18ndict, self.getpos(), isend=1)
        self.pop_xmlns()

    def handle_endtag(self, tag):
        if tag in EMPTY_HTML_TAGS:
            raise EmptyTagError(tag, self.getpos())
        self.close_enclosed_tags(tag)
        self.gen.emitEndElement(tag)
        self.pop_xmlns()
        self.tagstack.pop()

    def close_para_tags(self, tag):
        if tag in EMPTY_HTML_TAGS:
            return
        close_to = -1
        if BLOCK_CLOSING_TAG_MAP.has_key(tag):
            blocks_to_close = BLOCK_CLOSING_TAG_MAP[tag]
            for i in range(len(self.tagstack)):
                t = self.tagstack[i]
                if t in blocks_to_close:
                    if close_to == -1:
                        close_to = i
                elif t in BLOCK_LEVEL_HTML_TAGS:
                    close_to = -1
        elif tag in PARA_LEVEL_HTML_TAGS + BLOCK_LEVEL_HTML_TAGS:
            i = len(self.tagstack) - 1
            while i >= 0:
                closetag = self.tagstack[i]
                if closetag in BLOCK_LEVEL_HTML_TAGS:
                    break
                if closetag in PARA_LEVEL_HTML_TAGS:
                    if closetag != "p":
                        raise OpenTagError(self.tagstack, tag, self.getpos())
                    close_to = i
                i = i - 1
        if close_to >= 0:
            while len(self.tagstack) > close_to:
                self.implied_endtag(self.tagstack[-1], 1)

    def close_enclosed_tags(self, tag):
        if tag not in self.tagstack:
            raise NestingError(self.tagstack, tag, self.getpos())
        while tag != self.tagstack[-1]:
            self.implied_endtag(self.tagstack[-1], 1)
        assert self.tagstack[-1] == tag

    def implied_endtag(self, tag, implied):
        assert tag == self.tagstack[-1]
        assert implied in (-1, 1, 2)
        isend = (implied < 0)
        if tag in TIGHTEN_IMPLICIT_CLOSE_TAGS:
            white = self.gen.unEmitWhitespace()
        else:
            white = None
        self.gen.emitEndElement(tag, isend=isend, implied=implied)
        if white:
            self.gen.emitRawText(white)
        self.tagstack.pop()
        self.pop_xmlns()

    def handle_charref(self, name):
        self.gen.emitRawText("&#%s;" % name)

    def handle_entityref(self, name):
        self.gen.emitRawText("&%s;" % name)

    def handle_data(self, data):
        self.gen.emitRawText(data)

    def handle_comment(self, data):
        self.gen.emitRawText("<!--%s-->" % data)

    def handle_decl(self, data):
        self.gen.emitRawText("<!%s>" % data)

    def handle_pi(self, data):
        self.gen.emitRawText("<?%s>" % data)


    def scan_xmlns(self, attrs):
        nsnew = {}
        for key, value in attrs:
            if key.startswith("xmlns:"):
                nsnew[key[6:]] = value
        if nsnew:
            self.nsstack.append(self.nsdict)
            self.nsdict = self.nsdict.copy()
            self.nsdict.update(nsnew)
        else:
            self.nsstack.append(self.nsdict)

    def pop_xmlns(self):
        self.nsdict = self.nsstack.pop()

    def fixname(self, name):
        if ':' in name:
            prefix, suffix = name.split(':', 1)
            if prefix == 'xmlns':
                nsuri = self.nsdict.get(suffix)
                if nsuri in (ZOPE_TAL_NS, ZOPE_METAL_NS, ZOPE_I18N_NS):
                    return name, name, prefix
            else:
                nsuri = self.nsdict.get(prefix)
                if nsuri == ZOPE_TAL_NS:
                    return name, suffix, 'tal'
                elif nsuri == ZOPE_METAL_NS:
                    return name, suffix,  'metal'
                elif nsuri == ZOPE_I18N_NS:
                    return name, suffix, 'i18n'
        return name, name, 0

    def process_ns(self, name, attrs):
        attrlist = []
        taldict = {}
        metaldict = {}
        i18ndict = {}
        name, namebase, namens = self.fixname(name)
        for item in attrs:
            key, value = item
            key, keybase, keyns = self.fixname(key)
            ns = keyns or namens # default to tag namespace
            if ns and ns != 'unknown':
                item = (key, value, ns)
            if ns == 'tal':
                if taldict.has_key(keybase):
                    raise TALError("duplicate TAL attribute " +
                                   `keybase`, self.getpos())
                taldict[keybase] = value
            elif ns == 'metal':
                if metaldict.has_key(keybase):
                    raise METALError("duplicate METAL attribute " +
                                     `keybase`, self.getpos())
                metaldict[keybase] = value
            elif ns == 'i18n':
                if i18ndict.has_key(keybase):
                    raise I18NError("duplicate i18n attribute " +
                                    `keybase`, self.getpos())
                i18ndict[keybase] = value
            attrlist.append(item)
        if namens in ('metal', 'tal'):
            taldict['tal tag'] = namens
        return name, attrlist, taldict, metaldict, i18ndict
"""
Generic expat-based XML parser base class.
"""


class XMLParser:

    ordered_attributes = 0

    handler_names = [
        "StartElementHandler",
        "EndElementHandler",
        "ProcessingInstructionHandler",
        "CharacterDataHandler",
        "UnparsedEntityDeclHandler",
        "NotationDeclHandler",
        "StartNamespaceDeclHandler",
        "EndNamespaceDeclHandler",
        "CommentHandler",
        "StartCdataSectionHandler",
        "EndCdataSectionHandler",
        "DefaultHandler",
        "DefaultHandlerExpand",
        "NotStandaloneHandler",
        "ExternalEntityRefHandler",
        "XmlDeclHandler",
        "StartDoctypeDeclHandler",
        "EndDoctypeDeclHandler",
        "ElementDeclHandler",
        "AttlistDeclHandler"
        ]

    def __init__(self, encoding=None):
        self.parser = p = self.createParser()
        if self.ordered_attributes:
            try:
                self.parser.ordered_attributes = self.ordered_attributes
            except AttributeError:
                print "Can't set ordered_attributes"
                self.ordered_attributes = 0
        for name in self.handler_names:
            method = getattr(self, name, None)
            if method is not None:
                try:
                    setattr(p, name, method)
                except AttributeError:
                    print "Can't set expat handler %s" % name

    def createParser(self, encoding=None):
        global XMLParseError
        try:
            from Products.ParsedXML.Expat import pyexpat
            XMLParseError = pyexpat.ExpatError
            return pyexpat.ParserCreate(encoding, ' ')
        except ImportError:
            from xml.parsers import expat
            XMLParseError = expat.ExpatError
            return expat.ParserCreate(encoding, ' ')

    def parseFile(self, filename):
        f = open(filename)
        self.parseStream(f)
        #self.parseStream(open(filename))

    def parseString(self, s):
        self.parser.Parse(s, 1)

    def parseURL(self, url):
        import urllib
        self.parseStream(urllib.urlopen(url))

    def parseStream(self, stream):
        self.parser.ParseFile(stream)

    def parseFragment(self, s, end=0):
        self.parser.Parse(s, end)
"""Interface that a TALES engine provides to the METAL/TAL implementation."""

try:
    from Interface import Interface
    from Interface.Attribute import Attribute
except:
    class Interface: pass
    def Attribute(*args): pass


class ITALESCompiler(Interface):
    """Compile-time interface provided by a TALES implementation.

    The TAL compiler needs an instance of this interface to support
    compilation of TALES expressions embedded in documents containing
    TAL and METAL constructs.
    """

    def getCompilerError():
        """Return the exception class raised for compilation errors.
        """

    def compile(expression):
        """Return a compiled form of 'expression' for later evaluation.

        'expression' is the source text of the expression.

        The return value may be passed to the various evaluate*()
        methods of the ITALESEngine interface.  No compatibility is
        required for the values of the compiled expression between
        different ITALESEngine implementations.
        """


class ITALESEngine(Interface):
    """Render-time interface provided by a TALES implementation.

    The TAL interpreter uses this interface to TALES to support
    evaluation of the compiled expressions returned by
    ITALESCompiler.compile().
    """

    def getCompiler():
        """Return an object that supports ITALESCompiler."""

    def getDefault():
        """Return the value of the 'default' TALES expression.

        Checking a value for a match with 'default' should be done
        using the 'is' operator in Python.
        """

    def setPosition((lineno, offset)):
        """Inform the engine of the current position in the source file.

        This is used to allow the evaluation engine to report
        execution errors so that site developers can more easily
        locate the offending expression.
        """

    def setSourceFile(filename):
        """Inform the engine of the name of the current source file.

        This is used to allow the evaluation engine to report
        execution errors so that site developers can more easily
        locate the offending expression.
        """

    def beginScope():
        """Push a new scope onto the stack of open scopes.
        """

    def endScope():
        """Pop one scope from the stack of open scopes.
        """

    def evaluate(compiled_expression):
        """Evaluate an arbitrary expression.

        No constraints are imposed on the return value.
        """

    def evaluateBoolean(compiled_expression):
        """Evaluate an expression that must return a Boolean value.
        """

    def evaluateMacro(compiled_expression):
        """Evaluate an expression that must return a macro program.
        """

    def evaluateStructure(compiled_expression):
        """Evaluate an expression that must return a structured
        document fragment.

        The result of evaluating 'compiled_expression' must be a
        string containing a parsable HTML or XML fragment.  Any TAL
        markup cnotained in the result string will be interpreted.
        """

    def evaluateText(compiled_expression):
        """Evaluate an expression that must return text.

        The returned text should be suitable for direct inclusion in
        the output: any HTML or XML escaping or quoting is the
        responsibility of the expression itself.
        """

    def evaluateValue(compiled_expression):
        """Evaluate an arbitrary expression.

        No constraints are imposed on the return value.
        """

    def createErrorInfo(exception, (lineno, offset)):
        """Returns an ITALESErrorInfo object.

        The returned object is used to provide information about the
        error condition for the on-error handler.
        """

    def setGlobal(name, value):
        """Set a global variable.

        The variable will be named 'name' and have the value 'value'.
        """

    def setLocal(name, value):
        """Set a local variable in the current scope.

        The variable will be named 'name' and have the value 'value'.
        """

    def setRepeat(name, compiled_expression):
        """
        """

    def translate(domain, msgid, mapping, default=None):
        """
        See ITranslationService.translate()
        """


class ITALESErrorInfo(Interface):

    type = Attribute("type",
                     "The exception class.")

    value = Attribute("value",
                      "The exception instance.")

    lineno = Attribute("lineno",
                       "The line number the error occurred on in the source.")

    offset = Attribute("offset",
                       "The character offset at which the error occurred.")
"""
Common definitions used by TAL and METAL compilation an transformation.
"""

from types import ListType, TupleType


TAL_VERSION = "1.5"

XML_NS = "http://www.w3.org/XML/1998/namespace" # URI for XML namespace
XMLNS_NS = "http://www.w3.org/2000/xmlns/" # URI for XML NS declarations

ZOPE_TAL_NS = "http://xml.zope.org/namespaces/tal"
ZOPE_METAL_NS = "http://xml.zope.org/namespaces/metal"
ZOPE_I18N_NS = "http://xml.zope.org/namespaces/i18n"

NAME_RE = "[a-zA-Z_][-a-zA-Z0-9_]*"

KNOWN_METAL_ATTRIBUTES = [
    "define-macro",
    "use-macro",
    "define-slot",
    "fill-slot",
    "slot",
    ]

KNOWN_TAL_ATTRIBUTES = [
    "define",
    "condition",
    "content",
    "replace",
    "repeat",
    "attributes",
    "on-error",
    "omit-tag",
    "tal tag",
    ]

KNOWN_I18N_ATTRIBUTES = [
    "translate",
    "domain",
    "target",
    "source",
    "attributes",
    "data",
    "name",
    ]

class TALError(Exception):

    def __init__(self, msg, position=(None, None)):
        assert msg != ""
        self.msg = msg
        self.lineno = position[0]
        self.offset = position[1]
        self.filename = None

    def setFile(self, filename):
        self.filename = filename

    def __str__(self):
        result = self.msg
        if self.lineno is not None:
            result = result + ", at line %d" % self.lineno
        if self.offset is not None:
            result = result + ", column %d" % (self.offset + 1)
        if self.filename is not None:
            result = result + ', in file %s' % self.filename
        return result

class METALError(TALError):
    pass

class TALESError(TALError):
    pass

class I18NError(TALError):
    pass


class ErrorInfo:

    __implements__ = ITALESErrorInfo

    def __init__(self, err, position=(None, None)):
        if isinstance(err, Exception):
            self.type = err.__class__
            self.value = err
        else:
            self.type = err
            self.value = None
        self.lineno = position[0]
        self.offset = position[1]



import re
_attr_re = re.compile(r"\s*([^\s]+)\s+([^\s].*)\Z", re.S)
_subst_re = re.compile(r"\s*(?:(text|raw|structure)\s+)?(.*)\Z", re.S)
del re

def parseAttributeReplacements(arg, xml):
    dict = {}
    for part in splitParts(arg):
        m = _attr_re.match(part)
        if not m:
            raise TALError("Bad syntax in attributes: " + `part`)
        name, expr = m.group(1, 2)
        if not xml:
            name = name.lower()
        if dict.has_key(name):
            raise TALError("Duplicate attribute name in attributes: " + `part`)
        dict[name] = expr
    return dict

def parseSubstitution(arg, position=(None, None)):
    m = _subst_re.match(arg)
    if not m:
        raise TALError("Bad syntax in substitution text: " + `arg`, position)
    key, expr = m.group(1, 2)
    if not key:
        key = "text"
    return key, expr

def splitParts(arg):
    arg = arg.replace(";;", "\0")
    parts = arg.split(';')
    parts = [p.replace("\0", ";") for p in parts]
    if len(parts) > 1 and not parts[-1].strip():
        del parts[-1] # It ended in a semicolon
    return parts

def isCurrentVersion(program):
    version = getProgramVersion(program)
    return version == TAL_VERSION

def getProgramMode(program):
    version = getProgramVersion(program)
    if (version == TAL_VERSION and isinstance(program[1], TupleType) and
        len(program[1]) == 2):
        opcode, mode = program[1]
        if opcode == "mode":
            return mode
    return None

def getProgramVersion(program):
    if (len(program) >= 2 and
        isinstance(program[0], TupleType) and len(program[0]) == 2):
        opcode, version = program[0]
        if opcode == "version":
            return version
    return None

import re
_ent1_re = re.compile('&(?![A-Z#])', re.I)
_entch_re = re.compile('&([A-Z][A-Z0-9]*)(?![A-Z0-9;])', re.I)
_entn1_re = re.compile('&#(?![0-9X])', re.I)
_entnx_re = re.compile('&(#X[A-F0-9]*)(?![A-F0-9;])', re.I)
_entnd_re = re.compile('&(#[0-9][0-9]*)(?![0-9;])')
del re

def attrEscape(s):
    """Replace special characters '&<>' by character entities,
    except when '&' already begins a syntactically valid entity."""
    s = _ent1_re.sub('&amp;', s)
    s = _entch_re.sub(r'&amp;\1', s)
    s = _entn1_re.sub('&amp;#', s)
    s = _entnx_re.sub(r'&amp;\1', s)
    s = _entnd_re.sub(r'&amp;\1', s)
    s = s.replace('<', '&lt;')
    s = s.replace('>', '&gt;')
    s = s.replace('"', '&quot;')
    return s
"""
Code generator for TALInterpreter intermediate code.
"""

import re
import cgi



I18N_REPLACE = 1
I18N_CONTENT = 2
I18N_EXPRESSION = 3

_name_rx = re.compile(NAME_RE)


class TALGenerator:

    inMacroUse = 0
    inMacroDef = 0
    source_file = None

    def __init__(self, expressionCompiler=None, xml=1, source_file=None):
        if not expressionCompiler:
            expressionCompiler = AthanaTALEngine()
        self.expressionCompiler = expressionCompiler
        self.CompilerError = expressionCompiler.getCompilerError()
        self.program = []
        self.stack = []
        self.todoStack = []
        self.macros = {}
        self.slots = {}
        self.slotStack = []
        self.xml = xml
        self.emit("version", TAL_VERSION)
        self.emit("mode", xml and "xml" or "html")
        if source_file is not None:
            self.source_file = source_file
            self.emit("setSourceFile", source_file)
        self.i18nContext = TranslationContext()
        self.i18nLevel = 0

    def getCode(self):
        assert not self.stack
        assert not self.todoStack
        return self.optimize(self.program), self.macros

    def optimize(self, program):
        output = []
        collect = []
        cursor = 0
        if self.xml:
            endsep = "/>"
        else:
            endsep = " />"
        for cursor in xrange(len(program)+1):
            try:
                item = program[cursor]
            except IndexError:
                item = (None, None)
            opcode = item[0]
            if opcode == "rawtext":
                collect.append(item[1])
                continue
            if opcode == "endTag":
                collect.append("</%s>" % item[1])
                continue
            if opcode == "startTag":
                if self.optimizeStartTag(collect, item[1], item[2], ">"):
                    continue
            if opcode == "startEndTag":
                if self.optimizeStartTag(collect, item[1], item[2], endsep):
                    continue
            if opcode in ("beginScope", "endScope"):
                output.append(self.optimizeArgsList(item))
                continue
            if opcode == 'noop':
                opcode = None
                pass
            text = "".join(collect)
            if text:
                i = text.rfind("\n")
                if i >= 0:
                    i = len(text) - (i + 1)
                    output.append(("rawtextColumn", (text, i)))
                else:
                    output.append(("rawtextOffset", (text, len(text))))
            if opcode != None:
                output.append(self.optimizeArgsList(item))
            collect = []
        return self.optimizeCommonTriple(output)

    def optimizeArgsList(self, item):
        if len(item) == 2:
            return item
        else:
            return item[0], tuple(item[1:])

    def optimizeStartTag(self, collect, name, attrlist, end):
        if not attrlist:
            collect.append("<%s%s" % (name, end))
            return 1
        opt = 1
        new = ["<" + name]
        for i in range(len(attrlist)):
            item = attrlist[i]
            if len(item) > 2:
                opt = 0
                name, value, action = item[:3]
                attrlist[i] = (name, value, action) + item[3:]
            else:
                if item[1] is None:
                    s = item[0]
                else:
                    s = '%s="%s"' % (item[0], attrEscape(item[1]))
                attrlist[i] = item[0], s
                new.append(" " + s)
        if opt:
            new.append(end)
            collect.extend(new)
        return opt

    def optimizeCommonTriple(self, program):
        if len(program) < 3:
            return program
        output = program[:2]
        prev2, prev1 = output
        for item in program[2:]:
            if ( item[0] == "beginScope"
                 and prev1[0] == "setPosition"
                 and prev2[0] == "rawtextColumn"):
                position = output.pop()[1]
                text, column = output.pop()[1]
                prev1 = None, None
                closeprev = 0
                if output and output[-1][0] == "endScope":
                    closeprev = 1
                    output.pop()
                item = ("rawtextBeginScope",
                        (text, column, position, closeprev, item[1]))
            output.append(item)
            prev2 = prev1
            prev1 = item
        return output

    def todoPush(self, todo):
        self.todoStack.append(todo)

    def todoPop(self):
        return self.todoStack.pop()

    def compileExpression(self, expr):
        try:
            return self.expressionCompiler.compile(expr)
        except self.CompilerError, err:
            raise TALError('%s in expression %s' % (err.args[0], `expr`),
                           self.position)

    def pushProgram(self):
        self.stack.append(self.program)
        self.program = []

    def popProgram(self):
        program = self.program
        self.program = self.stack.pop()
        return self.optimize(program)

    def pushSlots(self):
        self.slotStack.append(self.slots)
        self.slots = {}

    def popSlots(self):
        slots = self.slots
        self.slots = self.slotStack.pop()
        return slots

    def emit(self, *instruction):
        self.program.append(instruction)

    def emitStartTag(self, name, attrlist, isend=0):
        if isend:
            opcode = "startEndTag"
        else:
            opcode = "startTag"
        self.emit(opcode, name, attrlist)

    def emitEndTag(self, name):
        if self.xml and self.program and self.program[-1][0] == "startTag":
            self.program[-1] = ("startEndTag",) + self.program[-1][1:]
        else:
            self.emit("endTag", name)

    def emitOptTag(self, name, optTag, isend):
        program = self.popProgram() #block
        start = self.popProgram() #start tag
        if (isend or not program) and self.xml:
            start[-1] = ("startEndTag",) + start[-1][1:]
            isend = 1
        cexpr = optTag[0]
        if cexpr:
            cexpr = self.compileExpression(optTag[0])
        self.emit("optTag", name, cexpr, optTag[1], isend, start, program)

    def emitRawText(self, text):
        self.emit("rawtext", text)

    def emitText(self, text):
        self.emitRawText(cgi.escape(text))

    def emitDefines(self, defines):
        for part in splitParts(defines):
            m = re.match(
                r"(?s)\s*(?:(global|local)\s+)?(%s)\s+(.*)\Z" % NAME_RE, part)
            if not m:
                raise TALError("invalid define syntax: " + `part`,
                               self.position)
            scope, name, expr = m.group(1, 2, 3)
            scope = scope or "local"
            cexpr = self.compileExpression(expr)
            if scope == "local":
                self.emit("setLocal", name, cexpr)
            else:
                self.emit("setGlobal", name, cexpr)

    def emitOnError(self, name, onError, TALtag, isend):
        block = self.popProgram()
        key, expr = parseSubstitution(onError)
        cexpr = self.compileExpression(expr)
        if key == "text":
            self.emit("insertText", cexpr, [])
        elif key == "raw":
            self.emit("insertRaw", cexpr, [])
        else:
            assert key == "structure"
            self.emit("insertStructure", cexpr, {}, [])
        if TALtag:
            self.emitOptTag(name, (None, 1), isend)
        else:
            self.emitEndTag(name)
        handler = self.popProgram()
        self.emit("onError", block, handler)

    def emitCondition(self, expr):
        cexpr = self.compileExpression(expr)
        program = self.popProgram()
        self.emit("condition", cexpr, program)

    def emitRepeat(self, arg):

        
        m = re.match("(?s)\s*(%s)\s+(.*)\Z" % NAME_RE, arg)
        if not m:
            raise TALError("invalid repeat syntax: " + `arg`,
                           self.position)
        name, expr = m.group(1, 2)
        cexpr = self.compileExpression(expr)
        program = self.popProgram()
        self.emit("loop", name, cexpr, program)


    def emitSubstitution(self, arg, attrDict={}):
        key, expr = parseSubstitution(arg)
        cexpr = self.compileExpression(expr)
        program = self.popProgram()
        if key == "text":
            self.emit("insertText", cexpr, program)
        elif key == "raw":
            self.emit("insertRaw", cexpr, program)
        else:
            assert key == "structure"
            self.emit("insertStructure", cexpr, attrDict, program)

    def emitI18nVariable(self, stuff):
        varname, action, expression = stuff
        m = _name_rx.match(varname)
        if m is None or m.group() != varname:
            raise TALError("illegal i18n:name: %r" % varname, self.position)
        key = cexpr = None
        program = self.popProgram()
        if action == I18N_REPLACE:
            program = program[1:-1]
        elif action == I18N_CONTENT:
            pass
        else:
            assert action == I18N_EXPRESSION
            key, expr = parseSubstitution(expression)
            cexpr = self.compileExpression(expr)
        self.emit('i18nVariable',
                  varname, program, cexpr, int(key == "structure"))

    def emitTranslation(self, msgid, i18ndata):
        program = self.popProgram()
        if i18ndata is None:
            self.emit('insertTranslation', msgid, program)
        else:
            key, expr = parseSubstitution(i18ndata)
            cexpr = self.compileExpression(expr)
            assert key == 'text'
            self.emit('insertTranslation', msgid, program, cexpr)

    def emitDefineMacro(self, macroName):
        program = self.popProgram()
        macroName = macroName.strip()
        if self.macros.has_key(macroName):
            raise METALError("duplicate macro definition: %s" % `macroName`,
                             self.position)
        if not re.match('%s$' % NAME_RE, macroName):
            raise METALError("invalid macro name: %s" % `macroName`,
                             self.position)
        self.macros[macroName] = program
        self.inMacroDef = self.inMacroDef - 1
        self.emit("defineMacro", macroName, program)

    def emitUseMacro(self, expr):
        cexpr = self.compileExpression(expr)
        program = self.popProgram()
        self.inMacroUse = 0
        self.emit("useMacro", expr, cexpr, self.popSlots(), program)

    def emitDefineSlot(self, slotName):
        program = self.popProgram()
        slotName = slotName.strip()
        if not re.match('%s$' % NAME_RE, slotName):
            raise METALError("invalid slot name: %s" % `slotName`,
                             self.position)
        self.emit("defineSlot", slotName, program)

    def emitFillSlot(self, slotName):
        program = self.popProgram()
        slotName = slotName.strip()
        if self.slots.has_key(slotName):
            raise METALError("duplicate fill-slot name: %s" % `slotName`,
                             self.position)
        if not re.match('%s$' % NAME_RE, slotName):
            raise METALError("invalid slot name: %s" % `slotName`,
                             self.position)
        self.slots[slotName] = program
        self.inMacroUse = 1
        self.emit("fillSlot", slotName, program)

    def unEmitWhitespace(self):
        collect = []
        i = len(self.program) - 1
        while i >= 0:
            item = self.program[i]
            if item[0] != "rawtext":
                break
            text = item[1]
            if not re.match(r"\A\s*\Z", text):
                break
            collect.append(text)
            i = i-1
        del self.program[i+1:]
        if i >= 0 and self.program[i][0] == "rawtext":
            text = self.program[i][1]
            m = re.search(r"\s+\Z", text)
            if m:
                self.program[i] = ("rawtext", text[:m.start()])
                collect.append(m.group())
        collect.reverse()
        return "".join(collect)

    def unEmitNewlineWhitespace(self):
        collect = []
        i = len(self.program)
        while i > 0:
            i = i-1
            item = self.program[i]
            if item[0] != "rawtext":
                break
            text = item[1]
            if re.match(r"\A[ \t]*\Z", text):
                collect.append(text)
                continue
            m = re.match(r"(?s)^(.*)(\n[ \t]*)\Z", text)
            if not m:
                break
            text, rest = m.group(1, 2)
            collect.reverse()
            rest = rest + "".join(collect)
            del self.program[i:]
            if text:
                self.emit("rawtext", text)
            return rest
        return None

    def replaceAttrs(self, attrlist, repldict):
        if not repldict:
            return attrlist
        newlist = []
        for item in attrlist:
            key = item[0]
            if repldict.has_key(key):
                expr, xlat, msgid = repldict[key]
                item = item[:2] + ("replace", expr, xlat, msgid)
                del repldict[key]
            newlist.append(item)
        for key, (expr, xlat, msgid) in repldict.items():
            newlist.append((key, None, "insert", expr, xlat, msgid))
        return newlist

    def emitStartElement(self, name, attrlist, taldict, metaldict, i18ndict,
                         position=(None, None), isend=0):
        if not taldict and not metaldict and not i18ndict:
            self.emitStartTag(name, attrlist, isend)
            self.todoPush({})
            if isend:
                self.emitEndElement(name, isend)
            return

        self.position = position
        for key, value in taldict.items():
            if key not in KNOWN_TAL_ATTRIBUTES:
                raise TALError("bad TAL attribute: " + `key`, position)
            if not (value or key == 'omit-tag'):
                raise TALError("missing value for TAL attribute: " +
                               `key`, position)
        for key, value in metaldict.items():
            if key not in KNOWN_METAL_ATTRIBUTES:
                raise METALError("bad METAL attribute: " + `key`,
                                 position)
            if not value:
                raise TALError("missing value for METAL attribute: " +
                               `key`, position)
        for key, value in i18ndict.items():
            if key not in KNOWN_I18N_ATTRIBUTES:
                raise I18NError("bad i18n attribute: " + `key`, position)
            if not value and key in ("attributes", "data", "id"):
                raise I18NError("missing value for i18n attribute: " +
                                `key`, position)
        todo = {}
        defineMacro = metaldict.get("define-macro")
        useMacro = metaldict.get("use-macro")
        defineSlot = metaldict.get("define-slot")
        fillSlot = metaldict.get("fill-slot")
        define = taldict.get("define")
        condition = taldict.get("condition")
        repeat = taldict.get("repeat")
        content = taldict.get("content")
        replace = taldict.get("replace")
        attrsubst = taldict.get("attributes")
        onError = taldict.get("on-error")
        omitTag = taldict.get("omit-tag")
        TALtag = taldict.get("tal tag")
        i18nattrs = i18ndict.get("attributes")
        msgid = i18ndict.get("translate")
        varname = i18ndict.get('name')
        i18ndata = i18ndict.get('data')

        if varname and not self.i18nLevel:
            raise I18NError(
                "i18n:name can only occur inside a translation unit",
                position)

        if i18ndata and not msgid:
            raise I18NError("i18n:data must be accompanied by i18n:translate",
                            position)

        if len(metaldict) > 1 and (defineMacro or useMacro):
            raise METALError("define-macro and use-macro cannot be used "
                             "together or with define-slot or fill-slot",
                             position)
        if replace:
            if content:
                raise TALError(
                    "tal:content and tal:replace are mutually exclusive",
                    position)
            if msgid is not None:
                raise I18NError(
                    "i18n:translate and tal:replace are mutually exclusive",
                    position)

        repeatWhitespace = None
        if repeat:
            repeatWhitespace = self.unEmitNewlineWhitespace()
        if position != (None, None):
            self.emit("setPosition", position)
        if self.inMacroUse:
            if fillSlot:
                self.pushProgram()
                if self.source_file is not None:
                    self.emit("setSourceFile", self.source_file)
                todo["fillSlot"] = fillSlot
                self.inMacroUse = 0
        else:
            if fillSlot:
                raise METALError("fill-slot must be within a use-macro",
                                 position)
        if not self.inMacroUse:
            if defineMacro:
                self.pushProgram()
                self.emit("version", TAL_VERSION)
                self.emit("mode", self.xml and "xml" or "html")
                if self.source_file is not None:
                    self.emit("setSourceFile", self.source_file)
                todo["defineMacro"] = defineMacro
                self.inMacroDef = self.inMacroDef + 1
            if useMacro:
                self.pushSlots()
                self.pushProgram()
                todo["useMacro"] = useMacro
                self.inMacroUse = 1
            if defineSlot:
                if not self.inMacroDef:
                    raise METALError(
                        "define-slot must be within a define-macro",
                        position)
                self.pushProgram()
                todo["defineSlot"] = defineSlot

        if defineSlot or i18ndict:

            domain = i18ndict.get("domain") or self.i18nContext.domain
            source = i18ndict.get("source") or self.i18nContext.source
            target = i18ndict.get("target") or self.i18nContext.target
            if (  domain != DEFAULT_DOMAIN
                  or source is not None
                  or target is not None):
                self.i18nContext = TranslationContext(self.i18nContext,
                                                      domain=domain,
                                                      source=source,
                                                      target=target)
                self.emit("beginI18nContext",
                          {"domain": domain, "source": source,
                           "target": target})
                todo["i18ncontext"] = 1
        if taldict or i18ndict:
            dict = {}
            for item in attrlist:
                key, value = item[:2]
                dict[key] = value
            self.emit("beginScope", dict)
            todo["scope"] = 1
        if onError:
            self.pushProgram() # handler
            if TALtag:
                self.pushProgram() # start
            self.emitStartTag(name, list(attrlist)) # Must copy attrlist!
            if TALtag:
                self.pushProgram() # start
            self.pushProgram() # block
            todo["onError"] = onError
        if define:
            self.emitDefines(define)
            todo["define"] = define
        if condition:
            self.pushProgram()
            todo["condition"] = condition
        if repeat:
            todo["repeat"] = repeat
            self.pushProgram()
            if repeatWhitespace:
                self.emitText(repeatWhitespace)
        if content:
            if varname:
                todo['i18nvar'] = (varname, I18N_CONTENT, None)
                todo["content"] = content
                self.pushProgram()
            else:
                todo["content"] = content
        elif replace:
            if varname:
                todo['i18nvar'] = (varname, I18N_EXPRESSION, replace)
            else:
                todo["replace"] = replace
            self.pushProgram()
        elif varname:
            todo['i18nvar'] = (varname, I18N_REPLACE, None)
            self.pushProgram()
        if msgid is not None:
            self.i18nLevel += 1
            todo['msgid'] = msgid
        if i18ndata:
            todo['i18ndata'] = i18ndata
        optTag = omitTag is not None or TALtag
        if optTag:
            todo["optional tag"] = omitTag, TALtag
            self.pushProgram()
        if attrsubst or i18nattrs:
            if attrsubst:
                repldict = parseAttributeReplacements(attrsubst,
                                                              self.xml)
            else:
                repldict = {}
            if i18nattrs:
                i18nattrs = _parseI18nAttributes(i18nattrs, attrlist, repldict,
                                                 self.position, self.xml,
                                                 self.source_file)
            else:
                i18nattrs = {}
            for key, value in repldict.items():
                if i18nattrs.get(key, None):
                    raise I18NError(
                      ("attribute [%s] cannot both be part of tal:attributes" +
                      " and have a msgid in i18n:attributes") % key,
                    position)
                ce = self.compileExpression(value)
                repldict[key] = ce, key in i18nattrs, i18nattrs.get(key)
            for key in i18nattrs:
                if not repldict.has_key(key):
                    repldict[key] = None, 1, i18nattrs.get(key)
        else:
            repldict = {}
        if replace:
            todo["repldict"] = repldict
            repldict = {}
        self.emitStartTag(name, self.replaceAttrs(attrlist, repldict), isend)
        if optTag:
            self.pushProgram()
        if content and not varname:
            self.pushProgram()
        if msgid is not None:
            self.pushProgram()
        if content and varname:
            self.pushProgram()
        if todo and position != (None, None):
            todo["position"] = position
        self.todoPush(todo)
        if isend:
            self.emitEndElement(name, isend)

    def emitEndElement(self, name, isend=0, implied=0):
        todo = self.todoPop()
        if not todo:
            if not isend:
                self.emitEndTag(name)
            return

        self.position = position = todo.get("position", (None, None))
        defineMacro = todo.get("defineMacro")
        useMacro = todo.get("useMacro")
        defineSlot = todo.get("defineSlot")
        fillSlot = todo.get("fillSlot")
        repeat = todo.get("repeat")
        content = todo.get("content")
        replace = todo.get("replace")
        condition = todo.get("condition")
        onError = todo.get("onError")
        repldict = todo.get("repldict", {})
        scope = todo.get("scope")
        optTag = todo.get("optional tag")
        msgid = todo.get('msgid')
        i18ncontext = todo.get("i18ncontext")
        varname = todo.get('i18nvar')
        i18ndata = todo.get('i18ndata')

        if implied > 0:
            if defineMacro or useMacro or defineSlot or fillSlot:
                exc = METALError
                what = "METAL"
            else:
                exc = TALError
                what = "TAL"
            raise exc("%s attributes on <%s> require explicit </%s>" %
                      (what, name, name), position)

        if content:
            self.emitSubstitution(content, {})
        if msgid is not None:
            if (not varname) or (
                varname and (varname[1] == I18N_CONTENT)):
                self.emitTranslation(msgid, i18ndata)
            self.i18nLevel -= 1
        if optTag:
            self.emitOptTag(name, optTag, isend)
        elif not isend:
            if varname:
                self.emit('noop')
            self.emitEndTag(name)
        if replace:
            self.emitSubstitution(replace, repldict)
        elif varname:
            assert (varname[1]
                    in [I18N_REPLACE, I18N_CONTENT, I18N_EXPRESSION])
            self.emitI18nVariable(varname)
        if msgid is not None: 
            if varname and (varname[1] <> I18N_CONTENT):
                self.emitTranslation(msgid, i18ndata)
        if repeat:
            self.emitRepeat(repeat)
        if condition:
            self.emitCondition(condition)
        if onError:
            self.emitOnError(name, onError, optTag and optTag[1], isend)
        if scope:
            self.emit("endScope")
        if i18ncontext:
            self.emit("endI18nContext")
            assert self.i18nContext.parent is not None
            self.i18nContext = self.i18nContext.parent
        if defineSlot:
            self.emitDefineSlot(defineSlot)
        if fillSlot:
            self.emitFillSlot(fillSlot)
        if useMacro:
            self.emitUseMacro(useMacro)
        if defineMacro:
            self.emitDefineMacro(defineMacro)

def _parseI18nAttributes(i18nattrs, attrlist, repldict, position,
                         xml, source_file):

    def addAttribute(dic, attr, msgid, position, xml):
        if not xml:
            attr = attr.lower()
        if attr in dic:
            raise TALError(
                "attribute may only be specified once in i18n:attributes: "
                + attr,
                position)
        dic[attr] = msgid

    d = {}
    if ';' in i18nattrs:
        i18nattrlist = i18nattrs.split(';')
        i18nattrlist = [attr.strip().split() 
                        for attr in i18nattrlist if attr.strip()]
        for parts in i18nattrlist:
            if len(parts) > 2:
                raise TALError("illegal i18n:attributes specification: %r"
                                % parts, position)
            if len(parts) == 2:
                attr, msgid = parts
            else:
                attr = parts[0]
                msgid = None
            addAttribute(d, attr, msgid, position, xml)
    else:
        i18nattrlist = i18nattrs.split()
        if len(i18nattrlist) == 1:
            addAttribute(d, i18nattrlist[0], None, position, xml)
        elif len(i18nattrlist) == 2:
            staticattrs = [attr[0] for attr in attrlist if len(attr) == 2]
            if (not i18nattrlist[1] in staticattrs) and (
                not i18nattrlist[1] in repldict):
                attr, msgid = i18nattrlist
                addAttribute(d, attr, msgid, position, xml)    
            else:
                import warnings
                warnings.warn(I18N_ATTRIBUTES_WARNING
                % (source_file, str(position), i18nattrs)
                , DeprecationWarning)
                msgid = None
                for attr in i18nattrlist:
                    addAttribute(d, attr, msgid, position, xml)    
        else:    
            import warnings
            warnings.warn(I18N_ATTRIBUTES_WARNING
            % (source_file, str(position), i18nattrs)
            , DeprecationWarning)
            msgid = None
            for attr in i18nattrlist:
                addAttribute(d, attr, msgid, position, xml)    
    return d

I18N_ATTRIBUTES_WARNING = (
    'Space separated attributes in i18n:attributes'
    ' are deprecated (i18n:attributes="value title"). Please use'
    ' semicolon to separate attributes'
    ' (i18n:attributes="value; title").'
    '\nFile %s at row, column %s\nAttributes %s')

"""Interpreter for a pre-compiled TAL program.

"""
import cgi
import sys
import getopt
import re
from cgi import escape

from StringIO import StringIO



class ConflictError:
    pass

class MessageID:
    pass



BOOLEAN_HTML_ATTRS = [
    "compact", "nowrap", "ismap", "declare", "noshade", "checked",
    "disabled", "readonly", "multiple", "selected", "noresize",
    "defer"
]

def _init():
    d = {}
    for s in BOOLEAN_HTML_ATTRS:
        d[s] = 1
    return d

BOOLEAN_HTML_ATTRS = _init()

_nulljoin = ''.join
_spacejoin = ' '.join

def normalize(text):
    return _spacejoin(text.split())


NAME_RE = r"[a-zA-Z][a-zA-Z0-9_]*"
_interp_regex = re.compile(r'(?<!\$)(\$(?:%(n)s|{%(n)s}))' %({'n': NAME_RE}))
_get_var_regex = re.compile(r'%(n)s' %({'n': NAME_RE}))

def interpolate(text, mapping):
    """Interpolate ${keyword} substitutions.

    This is called when no translation is provided by the translation
    service.
    """
    if not mapping:
        return text
    to_replace = _interp_regex.findall(text)
    for string in to_replace:
        var = _get_var_regex.findall(string)[0]
        if mapping.has_key(var):
            subst = ustr(mapping[var])
            try:
                text = text.replace(string, subst)
            except UnicodeError:
                subst = `subst`[1:-1]
                text = text.replace(string, subst)
    return text


class AltTALGenerator(TALGenerator):

    def __init__(self, repldict, expressionCompiler=None, xml=0):
        self.repldict = repldict
        self.enabled = 1
        TALGenerator.__init__(self, expressionCompiler, xml)

    def enable(self, enabled):
        self.enabled = enabled

    def emit(self, *args):
        if self.enabled:
            TALGenerator.emit(self, *args)

    def emitStartElement(self, name, attrlist, taldict, metaldict, i18ndict,
                         position=(None, None), isend=0):
        metaldict = {}
        taldict = {}
        i18ndict = {}
        if self.enabled and self.repldict:
            taldict["attributes"] = "x x"
        TALGenerator.emitStartElement(self, name, attrlist,
                                      taldict, metaldict, i18ndict,
                                      position, isend)

    def replaceAttrs(self, attrlist, repldict):
        if self.enabled and self.repldict:
            repldict = self.repldict
            self.repldict = None
        return TALGenerator.replaceAttrs(self, attrlist, repldict)


class TALInterpreter:

    def __init__(self, program, macros, engine, stream=None,
                 debug=0, wrap=60, metal=1, tal=1, showtal=-1,
                 strictinsert=1, stackLimit=100, i18nInterpolate=1):
        self.program = program
        self.macros = macros
        self.engine = engine # Execution engine (aka context)
        self.Default = engine.getDefault()
        self.stream = stream or sys.stdout
        self._stream_write = self.stream.write
        self.debug = debug
        self.wrap = wrap
        self.metal = metal
        self.tal = tal
        if tal:
            self.dispatch = self.bytecode_handlers_tal
        else:
            self.dispatch = self.bytecode_handlers
        assert showtal in (-1, 0, 1)
        if showtal == -1:
            showtal = (not tal)
        self.showtal = showtal
        self.strictinsert = strictinsert
        self.stackLimit = stackLimit
        self.html = 0
        self.endsep = "/>"
        self.endlen = len(self.endsep)
        self.macroStack = []
        self.position = None, None  # (lineno, offset)
        self.col = 0
        self.level = 0
        self.scopeLevel = 0
        self.sourceFile = None
        self.i18nStack = []
        self.i18nInterpolate = i18nInterpolate
        self.i18nContext = TranslationContext()

    def StringIO(self):
        return FasterStringIO()

    def saveState(self):
        return (self.position, self.col, self.stream,
                self.scopeLevel, self.level, self.i18nContext)

    def restoreState(self, state):
        (self.position, self.col, self.stream,
         scopeLevel, level, i18n) = state
        self._stream_write = self.stream.write
        assert self.level == level
        while self.scopeLevel > scopeLevel:
            self.engine.endScope()
            self.scopeLevel = self.scopeLevel - 1
        self.engine.setPosition(self.position)
        self.i18nContext = i18n

    def restoreOutputState(self, state):
        (dummy, self.col, self.stream,
         scopeLevel, level, i18n) = state
        self._stream_write = self.stream.write
        assert self.level == level
        assert self.scopeLevel == scopeLevel

    def pushMacro(self, macroName, slots, entering=1):
        if len(self.macroStack) >= self.stackLimit:
            raise METALError("macro nesting limit (%d) exceeded "
                             "by %s" % (self.stackLimit, `macroName`))
        self.macroStack.append([macroName, slots, entering, self.i18nContext])

    def popMacro(self):
        return self.macroStack.pop()

    def __call__(self):
        assert self.level == 0
        assert self.scopeLevel == 0
        assert self.i18nContext.parent is None
        self.interpret(self.program)
        assert self.level == 0
        assert self.scopeLevel == 0
        assert self.i18nContext.parent is None
        if self.col > 0:
            self._stream_write("\n")
            self.col = 0

    def interpretWithStream(self, program, stream):
        oldstream = self.stream
        self.stream = stream
        self._stream_write = stream.write
        try:
            self.interpret(program)
        finally:
            self.stream = oldstream
            self._stream_write = oldstream.write

    def stream_write(self, s,
                     len=len):
        self._stream_write(s)
        i = s.rfind('\n')
        if i < 0:
            self.col = self.col + len(s)
        else:
            self.col = len(s) - (i + 1)

    bytecode_handlers = {}

    def interpret(self, program):
        oldlevel = self.level
        self.level = oldlevel + 1
        handlers = self.dispatch
        try:
            if self.debug:
                for (opcode, args) in program:
                    s = "%sdo_%s(%s)\n" % ("    "*self.level, opcode,
                                           repr(args))
                    if len(s) > 80:
                        s = s[:76] + "...\n"
                    sys.stderr.write(s)
                    handlers[opcode](self, args)
            else:
                for (opcode, args) in program:
                    handlers[opcode](self, args)
        finally:
            self.level = oldlevel

    def do_version(self, version):
        assert version == TAL_VERSION
    bytecode_handlers["version"] = do_version

    def do_mode(self, mode):
        assert mode in ("html", "xml")
        self.html = (mode == "html")
        if self.html:
            self.endsep = " />"
        else:
            self.endsep = "/>"
        self.endlen = len(self.endsep)
    bytecode_handlers["mode"] = do_mode

    def do_setSourceFile(self, source_file):
        self.sourceFile = source_file
        self.engine.setSourceFile(source_file)
    bytecode_handlers["setSourceFile"] = do_setSourceFile

    def do_setPosition(self, position):
        self.position = position
        self.engine.setPosition(position)
    bytecode_handlers["setPosition"] = do_setPosition

    def do_startEndTag(self, stuff):
        self.do_startTag(stuff, self.endsep, self.endlen)
    bytecode_handlers["startEndTag"] = do_startEndTag

    def do_startTag(self, (name, attrList),
                    end=">", endlen=1, _len=len):
        self._currentTag = name
        L = ["<", name]
        append = L.append
        col = self.col + _len(name) + 1
        wrap = self.wrap
        align = col + 1
        if align >= wrap/2:
            align = 4  # Avoid a narrow column far to the right
        attrAction = self.dispatch["<attrAction>"]
        try:
            for item in attrList:
                if _len(item) == 2:
                    name, s = item
                else:
                    if item[2] in ('metal', 'tal', 'xmlns', 'i18n'):
                        if not self.showtal:
                            continue
                        ok, name, s = self.attrAction(item)
                    else:
                        ok, name, s = attrAction(self, item)
                    if not ok:
                        continue
                slen = _len(s)
                if (wrap and
                    col >= align and
                    col + 1 + slen > wrap):
                    append("\n")
                    append(" "*align)
                    col = align + slen
                else:
                    append(" ")
                    col = col + 1 + slen
                append(s)
            append(end)
            col = col + endlen
        finally:
            self._stream_write(_nulljoin(L))
            self.col = col
    bytecode_handlers["startTag"] = do_startTag

    def attrAction(self, item):
        name, value, action = item[:3]
        if action == 'insert':
            return 0, name, value
        macs = self.macroStack
        if action == 'metal' and self.metal and macs:
            if len(macs) > 1 or not macs[-1][2]:
                return 0, name, value
            macs[-1][2] = 0
            i = name.rfind(":") + 1
            prefix, suffix = name[:i], name[i:]
            if suffix == "define-macro":
                name = prefix + "use-macro"
                value = macs[-1][0] # Macro name
            elif suffix == "define-slot":
                name = prefix + "fill-slot"
            elif suffix == "fill-slot":
                pass
            else:
                return 0, name, value

        if value is None:
            value = name
        else:
            value = '%s="%s"' % (name, attrEscape(value))
        return 1, name, value

    def attrAction_tal(self, item):
        name, value, action = item[:3]
        ok = 1
        expr, xlat, msgid = item[3:]
        if self.html and name.lower() in BOOLEAN_HTML_ATTRS:
            evalue = self.engine.evaluateBoolean(item[3])
            if evalue is self.Default:
                if action == 'insert': # Cancelled insert
                    ok = 0
            elif evalue:
                value = None
            else:
                ok = 0
        elif expr is not None:
            evalue = self.engine.evaluateText(item[3])
            if evalue is self.Default:
                if action == 'insert': # Cancelled insert
                    ok = 0
            else:
                if evalue is None:
                    ok = 0
                value = evalue
        else:
            evalue = None

        if ok:
            if xlat:
                translated = self.translate(msgid or value, value, {})
                if translated is not None:
                    value = translated
            if value is None:
                value = name
            elif evalue is self.Default:
                value = attrEscape(value)
            else:
                value = escape(value, quote=1)
            value = '%s="%s"' % (name, value)
        return ok, name, value
    bytecode_handlers["<attrAction>"] = attrAction

    def no_tag(self, start, program):
        state = self.saveState()
        self.stream = stream = self.StringIO()
        self._stream_write = stream.write
        self.interpret(start)
        self.restoreOutputState(state)
        self.interpret(program)

    def do_optTag(self, (name, cexpr, tag_ns, isend, start, program),
                  omit=0):
        if tag_ns and not self.showtal:
            return self.no_tag(start, program)

        self.interpret(start)
        if not isend:
            self.interpret(program)
            s = '</%s>' % name
            self._stream_write(s)
            self.col = self.col + len(s)

    def do_optTag_tal(self, stuff):
        cexpr = stuff[1]
        if cexpr is not None and (cexpr == '' or
                                  self.engine.evaluateBoolean(cexpr)):
            self.no_tag(stuff[-2], stuff[-1])
        else:
            self.do_optTag(stuff)
    bytecode_handlers["optTag"] = do_optTag

    def do_rawtextBeginScope(self, (s, col, position, closeprev, dict)):
        self._stream_write(s)
        self.col = col
        self.position = position
        self.engine.setPosition(position)
        if closeprev:
            engine = self.engine
            engine.endScope()
            engine.beginScope()
        else:
            self.engine.beginScope()
            self.scopeLevel = self.scopeLevel + 1

    def do_rawtextBeginScope_tal(self, (s, col, position, closeprev, dict)):
        self._stream_write(s)
        self.col = col
        engine = self.engine
        self.position = position
        engine.setPosition(position)
        if closeprev:
            engine.endScope()
            engine.beginScope()
        else:
            engine.beginScope()
            self.scopeLevel = self.scopeLevel + 1
        engine.setLocal("attrs", dict)
    bytecode_handlers["rawtextBeginScope"] = do_rawtextBeginScope

    def do_beginScope(self, dict):
        self.engine.beginScope()
        self.scopeLevel = self.scopeLevel + 1

    def do_beginScope_tal(self, dict):
        engine = self.engine
        engine.beginScope()
        engine.setLocal("attrs", dict)
        self.scopeLevel = self.scopeLevel + 1
    bytecode_handlers["beginScope"] = do_beginScope

    def do_endScope(self, notused=None):
        self.engine.endScope()
        self.scopeLevel = self.scopeLevel - 1
    bytecode_handlers["endScope"] = do_endScope

    def do_setLocal(self, notused):
        pass

    def do_setLocal_tal(self, (name, expr)):
        self.engine.setLocal(name, self.engine.evaluateValue(expr))
    bytecode_handlers["setLocal"] = do_setLocal

    def do_setGlobal_tal(self, (name, expr)):
        self.engine.setGlobal(name, self.engine.evaluateValue(expr))
    bytecode_handlers["setGlobal"] = do_setLocal

    def do_beginI18nContext(self, settings):
        get = settings.get
        self.i18nContext = TranslationContext(self.i18nContext,
                                              domain=get("domain"),
                                              source=get("source"),
                                              target=get("target"))
    bytecode_handlers["beginI18nContext"] = do_beginI18nContext

    def do_endI18nContext(self, notused=None):
        self.i18nContext = self.i18nContext.parent
        assert self.i18nContext is not None
    bytecode_handlers["endI18nContext"] = do_endI18nContext

    def do_insertText(self, stuff):
        self.interpret(stuff[1])

    def do_insertText_tal(self, stuff):
        text = self.engine.evaluateText(stuff[0])
        if text is None:
            return
        if text is self.Default:
            self.interpret(stuff[1])
            return
        if isinstance(text, MessageID):
            text = self.engine.translate(text.domain, text, text.mapping)
        s = escape(text)
        self._stream_write(s)
        i = s.rfind('\n')
        if i < 0:
            self.col = self.col + len(s)
        else:
            self.col = len(s) - (i + 1)
    bytecode_handlers["insertText"] = do_insertText
    
    def do_insertRawText_tal(self, stuff):
        text = self.engine.evaluateText(stuff[0])
        if text is None:
            return
        if text is self.Default:
            self.interpret(stuff[1])
            return
        if isinstance(text, MessageID):
            text = self.engine.translate(text.domain, text, text.mapping)
        s = text
        self._stream_write(s)
        i = s.rfind('\n')
        if i < 0:
            self.col = self.col + len(s)
        else:
            self.col = len(s) - (i + 1)

    def do_i18nVariable(self, stuff):
        varname, program, expression, structure = stuff
        if expression is None:
            state = self.saveState()
            try:
                tmpstream = self.StringIO()
                self.interpretWithStream(program, tmpstream)
                if self.html and self._currentTag == "pre":
                    value = tmpstream.getvalue()
                else:
                    value = normalize(tmpstream.getvalue())
            finally:
                self.restoreState(state)
        else:
            if structure:
                value = self.engine.evaluateStructure(expression)
            else:
                value = self.engine.evaluate(expression)

            if isinstance(value, MessageID):
                value = self.engine.translate(value.domain, value,
                                              value.mapping)

            if not structure:
                value = cgi.escape(ustr(value))

        i18ndict, srepr = self.i18nStack[-1]
        i18ndict[varname] = value
        placeholder = '${%s}' % varname
        srepr.append(placeholder)
        self._stream_write(placeholder)
    bytecode_handlers['i18nVariable'] = do_i18nVariable

    def do_insertTranslation(self, stuff):
        i18ndict = {}
        srepr = []
        obj = None
        self.i18nStack.append((i18ndict, srepr))
        msgid = stuff[0]
        currentTag = self._currentTag
        tmpstream = self.StringIO()
        self.interpretWithStream(stuff[1], tmpstream)
        default = tmpstream.getvalue()
        if not msgid:
            if self.html and currentTag == "pre":
                msgid = default
            else:
                msgid = normalize(default)
        self.i18nStack.pop()
        if len(stuff) > 2:
            obj = self.engine.evaluate(stuff[2])
        xlated_msgid = self.translate(msgid, default, i18ndict, obj)
        assert xlated_msgid is not None
        self._stream_write(xlated_msgid)
    bytecode_handlers['insertTranslation'] = do_insertTranslation

    def do_insertStructure(self, stuff):
        self.interpret(stuff[2])

    def do_insertStructure_tal(self, (expr, repldict, block)):
        structure = self.engine.evaluateStructure(expr)
        if structure is None:
            return
        if structure is self.Default:
            self.interpret(block)
            return
        text = ustr(structure)
        if not (repldict or self.strictinsert):
            self.stream_write(text)
            return
        if self.html:
            self.insertHTMLStructure(text, repldict)
        else:
            self.insertXMLStructure(text, repldict)
    bytecode_handlers["insertStructure"] = do_insertStructure

    def insertHTMLStructure(self, text, repldict):
        gen = AltTALGenerator(repldict, self.engine.getCompiler(), 0)
        p = HTMLTALParser(gen) # Raises an exception if text is invalid
        p.parseString(text)
        program, macros = p.getCode()
        self.interpret(program)

    def insertXMLStructure(self, text, repldict):
        gen = AltTALGenerator(repldict, self.engine.getCompiler(), 0)
        p = TALParser(gen)
        gen.enable(0)
        p.parseFragment('<!DOCTYPE foo PUBLIC "foo" "bar"><foo>')
        gen.enable(1)
        p.parseFragment(text) # Raises an exception if text is invalid
        gen.enable(0)
        p.parseFragment('</foo>', 1)
        program, macros = gen.getCode()
        self.interpret(program)

    def do_loop(self, (name, expr, block)):
        self.interpret(block)

    def do_loop_tal(self, (name, expr, block)):
        iterator = self.engine.setRepeat(name, expr)
        while iterator.next():
            self.interpret(block)
    bytecode_handlers["loop"] = do_loop

    def translate(self, msgid, default, i18ndict, obj=None):
        if obj:
            i18ndict.update(obj)
        if not self.i18nInterpolate:
            return msgid
        return self.engine.translate(self.i18nContext.domain,
                                     msgid, i18ndict, default=default)

    def do_rawtextColumn(self, (s, col)):
        self._stream_write(s)
        self.col = col
    bytecode_handlers["rawtextColumn"] = do_rawtextColumn

    def do_rawtextOffset(self, (s, offset)):
        self._stream_write(s)
        self.col = self.col + offset
    bytecode_handlers["rawtextOffset"] = do_rawtextOffset

    def do_condition(self, (condition, block)):
        if not self.tal or self.engine.evaluateBoolean(condition):
            self.interpret(block)
    bytecode_handlers["condition"] = do_condition

    def do_defineMacro(self, (macroName, macro)):
        macs = self.macroStack
        if len(macs) == 1:
            entering = macs[-1][2]
            if not entering:
                macs.append(None)
                self.interpret(macro)
                assert macs[-1] is None
                macs.pop()
                return
        self.interpret(macro)
    bytecode_handlers["defineMacro"] = do_defineMacro

    def do_useMacro(self, (macroName, macroExpr, compiledSlots, block)):
        if not self.metal:
            self.interpret(block)
            return
        macro = self.engine.evaluateMacro(macroExpr)
        if macro is self.Default:
            macro = block
        else:
            if not isCurrentVersion(macro):
                raise METALError("macro %s has incompatible version %s" %
                                 (`macroName`, `getProgramVersion(macro)`),
                                 self.position)
            mode = getProgramMode(macro)
            #if mode != (self.html and "html" or "xml"):
            #    raise METALError("macro %s has incompatible mode %s" %
            #                     (`macroName`, `mode`), self.position)

        self.pushMacro(macroName, compiledSlots)
        prev_source = self.sourceFile
        self.interpret(macro)
        if self.sourceFile != prev_source:
            self.engine.setSourceFile(prev_source)
            self.sourceFile = prev_source
        self.popMacro()
    bytecode_handlers["useMacro"] = do_useMacro

    def do_fillSlot(self, (slotName, block)):
        self.interpret(block)
    bytecode_handlers["fillSlot"] = do_fillSlot

    def do_defineSlot(self, (slotName, block)):
        if not self.metal:
            self.interpret(block)
            return
        macs = self.macroStack
        if macs and macs[-1] is not None:
            macroName, slots = self.popMacro()[:2]
            slot = slots.get(slotName)
            if slot is not None:
                prev_source = self.sourceFile
                self.interpret(slot)
                if self.sourceFile != prev_source:
                    self.engine.setSourceFile(prev_source)
                    self.sourceFile = prev_source
                self.pushMacro(macroName, slots, entering=0)
                return
            self.pushMacro(macroName, slots)
        self.interpret(block)
    bytecode_handlers["defineSlot"] = do_defineSlot

    def do_onError(self, (block, handler)):
        self.interpret(block)

    def do_onError_tal(self, (block, handler)):
        state = self.saveState()
        self.stream = stream = self.StringIO()
        self._stream_write = stream.write
        try:
            self.interpret(block)
        except ConflictError:
            raise
        except:
            exc = sys.exc_info()[1]
            self.restoreState(state)
            engine = self.engine
            engine.beginScope()
            error = engine.createErrorInfo(exc, self.position)
            engine.setLocal('error', error)
            try:
                self.interpret(handler)
            finally:
                engine.endScope()
        else:
            self.restoreOutputState(state)
            self.stream_write(stream.getvalue())
    bytecode_handlers["onError"] = do_onError

    bytecode_handlers_tal = bytecode_handlers.copy()
    bytecode_handlers_tal["rawtextBeginScope"] = do_rawtextBeginScope_tal
    bytecode_handlers_tal["beginScope"] = do_beginScope_tal
    bytecode_handlers_tal["setLocal"] = do_setLocal_tal
    bytecode_handlers_tal["setGlobal"] = do_setGlobal_tal
    bytecode_handlers_tal["insertStructure"] = do_insertStructure_tal
    bytecode_handlers_tal["insertText"] = do_insertText_tal
    bytecode_handlers_tal["insertRaw"] = do_insertRawText_tal
    bytecode_handlers_tal["loop"] = do_loop_tal
    bytecode_handlers_tal["onError"] = do_onError_tal
    bytecode_handlers_tal["<attrAction>"] = attrAction_tal
    bytecode_handlers_tal["optTag"] = do_optTag_tal


class FasterStringIO(StringIO):
    """Append-only version of StringIO.

    This let's us have a much faster write() method.
    """
    def close(self):
        if not self.closed:
            self.write = _write_ValueError
            StringIO.close(self)

    def seek(self, pos, mode=0):
        raise RuntimeError("FasterStringIO.seek() not allowed")

    def write(self, s):
        self.buflist.append(s)
        self.len = self.pos = self.pos + len(s)


def _write_ValueError(s):
    raise ValueError, "I/O operation on closed file"
"""
Parse XML and compile to TALInterpreter intermediate code.
"""


class TALParser(XMLParser):

    ordered_attributes = 1

    def __init__(self, gen=None): # Override
        XMLParser.__init__(self)
        if gen is None:
            gen = TALGenerator()
        self.gen = gen
        self.nsStack = []
        self.nsDict = {XML_NS: 'xml'}
        self.nsNew = []

    def getCode(self):
        return self.gen.getCode()

    def getWarnings(self):
        return ()

    def StartNamespaceDeclHandler(self, prefix, uri):
        self.nsStack.append(self.nsDict.copy())
        self.nsDict[uri] = prefix
        self.nsNew.append((prefix, uri))

    def EndNamespaceDeclHandler(self, prefix):
        self.nsDict = self.nsStack.pop()

    def StartElementHandler(self, name, attrs):
        if self.ordered_attributes:
            attrlist = []
            for i in range(0, len(attrs), 2):
                key = attrs[i]
                value = attrs[i+1]
                attrlist.append((key, value))
        else:
            attrlist = attrs.items()
            attrlist.sort() # For definiteness
        name, attrlist, taldict, metaldict, i18ndict \
              = self.process_ns(name, attrlist)
        attrlist = self.xmlnsattrs() + attrlist
        self.gen.emitStartElement(name, attrlist, taldict, metaldict, i18ndict)

    def process_ns(self, name, attrlist):
        taldict = {}
        metaldict = {}
        i18ndict = {}
        fixedattrlist = []
        name, namebase, namens = self.fixname(name)
        for key, value in attrlist:
            key, keybase, keyns = self.fixname(key)
            ns = keyns or namens # default to tag namespace
            item = key, value
            if ns == 'metal':
                metaldict[keybase] = value
                item = item + ("metal",)
            elif ns == 'tal':
                taldict[keybase] = value
                item = item + ("tal",)
            elif ns == 'i18n':
                i18ndict[keybase] = value
                item = item + ('i18n',)
            fixedattrlist.append(item)
        if namens in ('metal', 'tal', 'i18n'):
            taldict['tal tag'] = namens
        return name, fixedattrlist, taldict, metaldict, i18ndict

    def xmlnsattrs(self):
        newlist = []
        for prefix, uri in self.nsNew:
            if prefix:
                key = "xmlns:" + prefix
            else:
                key = "xmlns"
            if uri in (ZOPE_METAL_NS, ZOPE_TAL_NS, ZOPE_I18N_NS):
                item = (key, uri, "xmlns")
            else:
                item = (key, uri)
            newlist.append(item)
        self.nsNew = []
        return newlist

    def fixname(self, name):
        if ' ' in name:
            uri, name = name.split(' ')
            prefix = self.nsDict[uri]
            prefixed = name
            if prefix:
                prefixed = "%s:%s" % (prefix, name)
            ns = 'x'
            if uri == ZOPE_TAL_NS:
                ns = 'tal'
            elif uri == ZOPE_METAL_NS:
                ns = 'metal'
            elif uri == ZOPE_I18N_NS:
                ns = 'i18n'
            return (prefixed, name, ns)
        return (name, name, None)

    def EndElementHandler(self, name):
        name = self.fixname(name)[0]
        self.gen.emitEndElement(name)

    def DefaultHandler(self, text):
        self.gen.emitRawText(text)

"""Translation context object for the TALInterpreter's I18N support.

The translation context provides a container for the information
needed to perform translation of a marked string from a page template.

"""

DEFAULT_DOMAIN = "default"

class TranslationContext:
    """Information about the I18N settings of a TAL processor."""

    def __init__(self, parent=None, domain=None, target=None, source=None):
        if parent:
            if not domain:
                domain = parent.domain
            if not target:
                target = parent.target
            if not source:
                source = parent.source
        elif domain is None:
            domain = DEFAULT_DOMAIN

        self.parent = parent
        self.domain = domain
        self.target = target
        self.source = source
"""
Dummy TALES engine so that I can test out the TAL implementation.
"""

import re
import sys
import stat
import os
import traceback

class _Default:
    pass
Default = _Default()

name_match = re.compile(r"(?s)(%s):(.*)\Z" % NAME_RE).match

class CompilerError(Exception):
    pass

class AthanaTALEngine:

    position = None
    source_file = None

    __implements__ = ITALESCompiler, ITALESEngine

    def __init__(self, macros=None, context=None, webcontext=None, language=None, request=None):
        if macros is None:
            macros = {}
        self.macros = macros
        dict = {'nothing': None, 'default': Default}
        if context is not None:
            dict.update(context)

        self.locals = self.globals = dict
        self.stack = [dict]
        self.webcontext = webcontext
        self.language = language
        self.request = request

    def compilefile(self, file, mode=None):
        assert mode in ("html", "xml", None)
        #file =  join_paths(GLOBAL_ROOT_DIR,join_paths(self.webcontext.root, file))
        if mode is None:
            ext = os.path.splitext(file)[1]
            if ext.lower() in (".html", ".htm"):
                mode = "html"
            else:
                mode = "xml"
        if mode == "html":
            p = HTMLTALParser(TALGenerator(self))
        else:
            p = TALParser(TALGenerator(self))
        p.parseFile(file)
        return p.getCode()

    def getCompilerError(self):
        return CompilerError

    def getCompiler(self):
        return self

    def setSourceFile(self, source_file):
        self.source_file = source_file

    def setPosition(self, position):
        self.position = position

    def compile(self, expr):
        return "$%s$" % expr

    def uncompile(self, expression):
        assert (expression.startswith("$") and expression.endswith("$"),
            expression)
        return expression[1:-1]

    def beginScope(self):
        self.stack.append(self.locals)

    def endScope(self):
        assert len(self.stack) > 1, "more endScope() than beginScope() calls"
        self.locals = self.stack.pop()

    def setLocal(self, name, value):
        if self.locals is self.stack[-1]:
            self.locals = self.locals.copy()
        self.locals[name] = value

    def setGlobal(self, name, value):
        self.globals[name] = value

    def evaluate(self, expression):
        assert (expression.startswith("$") and expression.endswith("$"),
            expression)
        expression = expression[1:-1]
        m = name_match(expression)
        if m:
            type, expr = m.group(1, 2)
        else:
            type = "path"
            expr = expression
        if type in ("string", "str"):
            return expr
        if type in ("path", "var", "global", "local"):
            return self.evaluatePathOrVar(expr)
        if type == "not":
            return not self.evaluate(expr)
        if type == "exists":
            return self.locals.has_key(expr) or self.globals.has_key(expr)
        if type == "python":
            try:
                return eval(expr, self.globals, self.locals)
            except:
                print "Error in python expression"
                print sys.exc_info()[0], sys.exc_info()[1]
                traceback.print_tb(sys.exc_info()[2])
                raise TALESError("evaluation error in %s" % `expr`)

        if type == "position":
            if self.position:
                lineno, offset = self.position
            else:
                lineno, offset = None, None
            return '%s (%s,%s)' % (self.source_file, lineno, offset)
        raise TALESError("unrecognized expression: " + `expression`)

    def evaluatePathOrVar(self, expr):
        expr = expr.strip()
        _expr=expr
        _f=None
        if expr.rfind("/")>0:
            pos=expr.rfind("/")
            _expr = expr[0:pos]
            _f = expr[pos+1:]
        if self.locals.has_key(_expr):
            if _f:
                return getattr(self.locals[_expr],_f)
            else:
                return self.locals[_expr]
        elif self.globals.has_key(_expr):
            if _f:
                return getattr(self.globals[_expr], _f)
            else:
                return self.globals[_expr]
        else:
            raise TALESError("unknown variable: %s" % `_expr`)

    def evaluateValue(self, expr):
        return self.evaluate(expr)

    def evaluateBoolean(self, expr):
        return self.evaluate(expr)

    def evaluateText(self, expr):
        text = self.evaluate(expr)
        if text is not None and text is not Default:
            text = ustr(text)
        return text

    def evaluateStructure(self, expr):
        return self.evaluate(expr)

    def evaluateSequence(self, expr):
        return self.evaluate(expr)

    def evaluateMacro(self, macroName):
        assert (macroName.startswith("$") and macroName.endswith("$"),
            macroName)
        macroName = macroName[1:-1]
        file, localName = self.findMacroFile(macroName)
        if not file:
            macro = self.macros[localName]
        else:
            program, macros = self.compilefile(file)
            macro = macros.get(localName)
            if not macro:
                raise TALESError("macro %s not found in file %s" %
                                 (localName, file))
        return macro

    def findMacroDocument(self, macroName):
        file, localName = self.findMacroFile(macroName)
        if not file:
            return file, localName
        doc = parsefile(file)
        return doc, localName

    def findMacroFile(self, macroName):
        if not macroName:
            raise TALESError("empty macro name")
        i = macroName.rfind('/')
        if i < 0:
            print "NO Macro"
            return None, macroName
        else:
            fileName = getMacroFile(macroName[:i])
            localName = macroName[i+1:]
            return fileName, localName

    def setRepeat(self, name, expr):
        seq = self.evaluateSequence(expr)
        self.locals[name] = Iterator(name, seq, self)
        return self.locals[name]

    def createErrorInfo(self, err, position):
        return ErrorInfo(err, position)

    def getDefault(self):
        return Default

    def translate(self, domain, msgid, mapping, default=None):
        global translators
        text = default or msgid
        for f in translators:
            text = f(msgid, language=self.language, request=self.request)
            try:
                text = f(msgid, language=self.language, request=self.request)
                if text and text!=msgid:
                    break
            except: 
                pass
        def repl(m, mapping=mapping):
            return ustr(mapping[m.group(m.lastindex).lower()])
        return VARIABLE.sub(repl, text)


class Iterator:
   
    def __init__(self, name, seq, engine):
        self.name = name
        self.seq = seq
        self.engine = engine
        self.nextIndex = 0

    def next(self):
        self.index = i = self.nextIndex
        try:
            item = self.seq[i]
        except IndexError:
            return 0
        self.nextIndex = i+1
        self.engine.setLocal(self.name, item)
        return 1

    def even(self):
        print "-even-"
        return not self.index % 2

    def odd(self):
        print "-odd-"
        return self.index % 2

    def number(self):
        return self.nextIndex

    def parity(self):
        if self.index % 2:
            return 'odd'
        return 'even'

    def first(self, name=None):
        if self.start: return 1
        return not self.same_part(name, self._last, self.item)

    def last(self, name=None):
        if self.end: return 1
        return not self.same_part(name, self.item, self._next)

    def length(self):
        return len(self.seq)
    

VARIABLE = re.compile(r'\$(?:(%s)|\{(%s)\})' % (NAME_RE, NAME_RE))

parsed_files = {}
parsed_strings = {}

def runTAL(writer, context=None, string=None, file=None, macro=None, language=None, request=None):

    if file:
        file = getMacroFile(file)

    if context is None:
        context = {}

    if string and not file:
        if string in parsed_strings:
            program,macros = parsed_strings[string]
        else:
            program,macros = None,None
    elif file and not string:
        if file in parsed_files:
            (program,macros,mtime) = parsed_files[file]
            mtime_file = os.stat(file)[stat.ST_MTIME]
            if mtime != mtime_file:
                program,macros = None,None
                mtime = mtime_file
        else:
            program,macros,mtime = None,None,None
   
    if not (program and macros):
        if file and file.endswith("xml"):
            talparser = TALParser(TALGenerator(AthanaTALEngine()))
        else:
            talparser = HTMLTALParser(TALGenerator(AthanaTALEngine()))
        if string:
            talparser.parseString(string)
            (program, macros) = talparser.getCode()
            parsed_strings[string] = (program,macros)
        else:
            talparser.parseFile(file)
            (program, macros) = talparser.getCode()
            parsed_files[file] = (program,macros,mtime)

    if macro and macro in macros:
        program = macros[macro]
    engine = AthanaTALEngine(macros, context, language=language, request=request)
    TALInterpreter(program, macros, engine, writer, wrap=0)()

def processTAL(context=None, string=None, file=None, macro=None, language=None, request=None):
    class STRWriter:
        def __init__(self):
            self.string = ""
        def write(self,text):
            if type(text) == type(u''):
                self.string += text.encode("utf-8")
            else:
                self.string += text
        def getvalue(self):
            return self.string
    wr = STRWriter()
    runTAL(wr, context, string=string, file=file, macro=macro, language=language, request=request)
    return wr.getvalue()


class MyWriter:
    def write(self,s):
        sys.stdout.write(s)

def test():
    p = TALParser(TALGenerator(AthanaTALEngine()))
    file = "test.xml"
    if sys.argv[1:]:
        file = sys.argv[1]
    p.parseFile(file)
    program, macros = p.getCode()

    class Node:
        def getText(self):
            return "TEST"

    engine = AthanaTALEngine(macros, {'node': Node()})
    TALInterpreter(program, macros, engine, MyWriter(), wrap=0)()


def ustr(v):
    """Convert any object to a plain string or unicode string,
    minimising the chance of raising a UnicodeError. This
    even works with uncooperative objects like Exceptions
    """
    if type(v) == type(""): #isinstance(v, basestring):
        return v
    else:
        fn = getattr(v,'__str__',None)
        if fn is not None:
            v = fn()
            if isinstance(v, basestring):
                return v
            else:
                raise ValueError('__str__ returned wrong type')
        return str(v)


# ================ MEDUSA ===============

# python modules
import os
import re
import select
import socket
import string
import sys
import time
import stat
import string
import mimetypes
import glob
from cgi import escape
from urllib import unquote, splitquery

# async modules
import asyncore
import socket

class async_chat (asyncore.dispatcher):
    """This is an abstract class.  You must derive from this class, and add
    the two methods collect_incoming_data() and found_terminator()"""

    # these are overridable defaults

    ac_in_buffer_size       = 4096
    ac_out_buffer_size      = 4096

    def __init__ (self, conn=None):
        self.ac_in_buffer = ''
        self.ac_out_buffer = ''
        self.producer_fifo = fifo()
        asyncore.dispatcher.__init__ (self, conn)

    def collect_incoming_data(self, data):
        raise NotImplementedError, "must be implemented in subclass"

    def found_terminator(self):
        raise NotImplementedError, "must be implemented in subclass"

    def set_terminator (self, term):
        "Set the input delimiter.  Can be a fixed string of any length, an integer, or None"
        self.terminator = term

    def get_terminator (self):
        return self.terminator

    # grab some more data from the socket,
    # throw it to the collector method,
    # check for the terminator,
    # if found, transition to the next state.

    def handle_read (self):

        try:
            data = self.recv (self.ac_in_buffer_size)
        except socket.error, why:
            self.handle_error()
            return

        self.ac_in_buffer = self.ac_in_buffer + data

        # Continue to search for self.terminator in self.ac_in_buffer,
        # while calling self.collect_incoming_data.  The while loop
        # is necessary because we might read several data+terminator
        # combos with a single recv(1024).

        while self.ac_in_buffer:
            lb = len(self.ac_in_buffer)
            terminator = self.get_terminator()
            if terminator is None or terminator == '':
                # no terminator, collect it all
                self.collect_incoming_data (self.ac_in_buffer)
                self.ac_in_buffer = ''
            elif isinstance(terminator, int):
                # numeric terminator
                n = terminator
                if lb < n:
                    self.collect_incoming_data (self.ac_in_buffer)
                    self.ac_in_buffer = ''
                    self.terminator = self.terminator - lb
                else:
                    self.collect_incoming_data (self.ac_in_buffer[:n])
                    self.ac_in_buffer = self.ac_in_buffer[n:]
                    self.terminator = 0
                    self.found_terminator()
            else:
                # 3 cases:
                # 1) end of buffer matches terminator exactly:
                #    collect data, transition
                # 2) end of buffer matches some prefix:
                #    collect data to the prefix
                # 3) end of buffer does not match any prefix:
                #    collect data
                terminator_len = len(terminator)
                index = self.ac_in_buffer.find(terminator)
                if index != -1:
                    # we found the terminator
                    if index > 0:
                        # don't bother reporting the empty string (source of subtle bugs)
                        self.collect_incoming_data (self.ac_in_buffer[:index])
                    self.ac_in_buffer = self.ac_in_buffer[index+terminator_len:]
                    # This does the Right Thing if the terminator is changed here.
                    self.found_terminator()
                else:
                    # check for a prefix of the terminator
                    index = find_prefix_at_end (self.ac_in_buffer, terminator)
                    if index:
                        if index != lb:
                            # we found a prefix, collect up to the prefix
                            self.collect_incoming_data (self.ac_in_buffer[:-index])
                            self.ac_in_buffer = self.ac_in_buffer[-index:]
                        break
                    else:
                        # no prefix, collect it all
                        self.collect_incoming_data (self.ac_in_buffer)
                        self.ac_in_buffer = ''

    def handle_write (self):
        self.initiate_send ()

    def handle_close (self):
        self.close()

    def push (self, data):
        self.producer_fifo.push (simple_producer (data))
        self.initiate_send()

    def push_with_producer (self, producer):
        self.producer_fifo.push (producer)
        self.initiate_send()

    def readable (self):
        "predicate for inclusion in the readable for select()"
        return (len(self.ac_in_buffer) <= self.ac_in_buffer_size)

    def writable (self):
        "predicate for inclusion in the writable for select()"
        # return len(self.ac_out_buffer) or len(self.producer_fifo) or (not self.connected)
        # this is about twice as fast, though not as clear.
        return not (
                (self.ac_out_buffer == '') and
                self.producer_fifo.is_empty() and
                self.connected
                )

    def close_when_done (self):
        "automatically close this channel once the outgoing queue is empty"
        self.producer_fifo.push (None)

    # refill the outgoing buffer by calling the more() method
    # of the first producer in the queue
    def refill_buffer (self):
        while 1:
            if len(self.producer_fifo):
                p = self.producer_fifo.first()
                # a 'None' in the producer fifo is a sentinel,
                # telling us to close the channel.
                if p is None:
                    if not self.ac_out_buffer:
                        self.producer_fifo.pop()
                        self.close()
                    return
                elif isinstance(p, str):
                    self.producer_fifo.pop()
                    self.ac_out_buffer = self.ac_out_buffer + p
                    return
                data = p.more()
                if data:
                    self.ac_out_buffer = self.ac_out_buffer + data
                    return
                else:
                    self.producer_fifo.pop()
            else:
                return

    def initiate_send (self):
        obs = self.ac_out_buffer_size
        # try to refill the buffer
        if (len (self.ac_out_buffer) < obs):
            self.refill_buffer()

        if self.ac_out_buffer and self.connected:
            # try to send the buffer
            try:
                num_sent = self.send (self.ac_out_buffer[:obs])
                if num_sent:
                    self.ac_out_buffer = self.ac_out_buffer[num_sent:]

            except socket.error, why:
                self.handle_error()
                return

    def discard_buffers (self):
        # Emergencies only!
        self.ac_in_buffer = ''
        self.ac_out_buffer = ''
        while self.producer_fifo:
            self.producer_fifo.pop()


class simple_producer:

    def __init__ (self, data, buffer_size=512):
        self.data = data
        self.buffer_size = buffer_size

    def more (self):
        if len (self.data) > self.buffer_size:
            result = self.data[:self.buffer_size]
            self.data = self.data[self.buffer_size:]
            return result
        else:
            result = self.data
            self.data = ''
            return result

class fifo:
    def __init__ (self, list=None):
        if not list:
            self.list = []
        else:
            self.list = list

    def __len__ (self):
        return len(self.list)

    def is_empty (self):
        return self.list == []

    def first (self):
        return self.list[0]

    def push (self, data):
        self.list.append (data)

    def pop (self):
        if self.list:
            return (1, self.list.pop(0))
        else:
            return (0, None)

# Given 'haystack', see if any prefix of 'needle' is at its end.  This
# assumes an exact match has already been checked.  Return the number of
# characters matched.
# for example:
# f_p_a_e ("qwerty\r", "\r\n") => 1
# f_p_a_e ("qwertydkjf", "\r\n") => 0
# f_p_a_e ("qwerty\r\n", "\r\n") => <undefined>

# this could maybe be made faster with a computed regex?
# [answer: no; circa Python-2.0, Jan 2001]
# new python:   28961/s
# old python:   18307/s
# re:        12820/s
# regex:     14035/s

def find_prefix_at_end (haystack, needle):
    l = len(needle) - 1
    while l and not haystack.endswith(needle[:l]):
        l -= 1
    return l

class counter:
    "general-purpose counter"

    def __init__ (self, initial_value=0):
        self.value = initial_value

    def increment (self, delta=1):
        result = self.value
        try:
            self.value = self.value + delta
        except OverflowError:
            self.value = long(self.value) + delta
        return result

    def decrement (self, delta=1):
        result = self.value
        try:
            self.value = self.value - delta
        except OverflowError:
            self.value = long(self.value) - delta
        return result

    def as_long (self):
        return long(self.value)

    def __nonzero__ (self):
        return self.value != 0

    def __repr__ (self):
        return '<counter value=%s at %x>' % (self.value, id(self))

    def __str__ (self):
        s = str(long(self.value))
        if s[-1:] == 'L':
            s = s[:-1]
        return s


# http_date
def concat (*args):
    return ''.join (args)

def join (seq, field=' '):
    return field.join (seq)

def group (s):
    return '(' + s + ')'

short_days = ['sun','mon','tue','wed','thu','fri','sat']
long_days = ['sunday','monday','tuesday','wednesday','thursday','friday','saturday']

short_day_reg = group (join (short_days, '|'))
long_day_reg = group (join (long_days, '|'))

daymap = {}
for i in range(7):
    daymap[short_days[i]] = i
    daymap[long_days[i]] = i

hms_reg = join (3 * [group('[0-9][0-9]')], ':')

months = ['jan','feb','mar','apr','may','jun','jul','aug','sep','oct','nov','dec']

monmap = {}
for i in range(12):
    monmap[months[i]] = i+1

months_reg = group (join (months, '|'))

# From draft-ietf-http-v11-spec-07.txt/3.3.1
#       Sun, 06 Nov 1994 08:49:37 GMT  ; RFC 822, updated by RFC 1123
#       Sunday, 06-Nov-94 08:49:37 GMT ; RFC 850, obsoleted by RFC 1036
#       Sun Nov  6 08:49:37 1994       ; ANSI C's asctime() format

# rfc822 format
rfc822_date = join (
        [concat (short_day_reg,','),    # day
         group('[0-9][0-9]?'),                  # date
         months_reg,                                    # month
         group('[0-9]+'),                               # year
         hms_reg,                                               # hour minute second
         'gmt'
         ],
        ' '
        )

rfc822_reg = re.compile (rfc822_date)

def unpack_rfc822 (m):
    g = m.group
    a = string.atoi
    return (
            a(g(4)),                # year
            monmap[g(3)],   # month
            a(g(2)),                # day
            a(g(5)),                # hour
            a(g(6)),                # minute
            a(g(7)),                # second
            0,
            0,
            0
            )

# rfc850 format
rfc850_date = join (
        [concat (long_day_reg,','),
         join (
                 [group ('[0-9][0-9]?'),
                  months_reg,
                  group ('[0-9]+')
                  ],
                 '-'
                 ),
         hms_reg,
         'gmt'
         ],
        ' '
        )

rfc850_reg = re.compile (rfc850_date)
# they actually unpack the same way
def unpack_rfc850 (m):
    g = m.group
    a = string.atoi
    return (
            a(g(4)),                # year
            monmap[g(3)],   # month
            a(g(2)),                # day
            a(g(5)),                # hour
            a(g(6)),                # minute
            a(g(7)),                # second
            0,
            0,
            0
            )

# parsdate.parsedate    - ~700/sec.
# parse_http_date       - ~1333/sec.

def build_http_date (when):
    return time.strftime ('%a, %d %b %Y %H:%M:%S GMT', time.gmtime(when))

time_offset = 0

def parse_http_date (d):
    global time_offset
    d = string.lower (d)
    tz = time.timezone
    m = rfc850_reg.match (d)
    if m and m.end() == len(d):
        retval = int (time.mktime (unpack_rfc850(m)) - tz)
    else:
        m = rfc822_reg.match (d)
        if m and m.end() == len(d):
            try:
                retval = int (time.mktime (unpack_rfc822(m)) - tz)
            except OverflowError:
                return 0
        else:
            return 0
    # Thanks to Craig Silverstein <csilvers@google.com> for pointing
    # out the DST discrepancy
    if time.daylight and time.localtime(retval)[-1] == 1: # DST correction
        retval = retval + (tz - time.altzone)
    return retval - time_offset

def check_date():
    global time_offset
    tmpfile = join_paths(GLOBAL_TEMP_DIR, "datetest"+str(random.random())+".tmp")
    open(tmpfile,"wb").close()
    time1 = os.stat(tmpfile)[stat.ST_MTIME]
    os.unlink(tmpfile)
    time2 = parse_http_date(build_http_date(time.time()))
    time_offset = time2-time1
    print time_offset

# producers

class simple_producer:
    "producer for a string"
    def __init__ (self, data, buffer_size=1024):
        self.data = data
        self.buffer_size = buffer_size

    def more (self):
        if len (self.data) > self.buffer_size:
            result = self.data[:self.buffer_size]
            self.data = self.data[self.buffer_size:]
            return result
        else:
            result = self.data
            self.data = ''
            return result

class file_producer:
    "producer wrapper for file[-like] objects"

    # match http_channel's outgoing buffer size
    out_buffer_size = 1<<16

    def __init__ (self, file):
        self.done = 0
        self.file = file

    def more (self):
        if self.done:
            return ''
        else:
            data = self.file.read (self.out_buffer_size)
            if not data:
                self.file.close()
                del self.file
                self.done = 1
                return ''
            else:
                return data

# A simple output producer.  This one does not [yet] have
# the safety feature builtin to the monitor channel:  runaway
# output will not be caught.

# don't try to print from within any of the methods
# of this object.

class output_producer:
    "Acts like an output file; suitable for capturing sys.stdout"
    def __init__ (self):
        self.data = ''

    def write (self, data):
        lines = string.splitfields (data, '\n')
        data = string.join (lines, '\r\n')
        self.data = self.data + data

    def writeline (self, line):
        self.data = self.data + line + '\r\n'

    def writelines (self, lines):
        self.data = self.data + string.joinfields (
                lines,
                '\r\n'
                ) + '\r\n'

    def flush (self):
        pass

    def softspace (self, *args):
        pass

    def more (self):
        if self.data:
            result = self.data[:512]
            self.data = self.data[512:]
            return result
        else:
            return ''

class composite_producer:
    "combine a fifo of producers into one"
    def __init__ (self, producers):
        self.producers = producers

    def more (self):
        while len(self.producers):
            p = self.producers[0]
            d = p.more()
            if d:
                return d
            else:
                self.producers.pop(0)
        else:
            return ''


class globbing_producer:
    """
    'glob' the output from a producer into a particular buffer size.
    helps reduce the number of calls to send().  [this appears to
    gain about 30% performance on requests to a single channel]
    """

    def __init__ (self, producer, buffer_size=1<<16):
        self.producer = producer
        self.buffer = ''
        self.buffer_size = buffer_size

    def more (self):
        while len(self.buffer) < self.buffer_size:
            data = self.producer.more()
            if data:
                self.buffer = self.buffer + data
            else:
                break
        r = self.buffer
        self.buffer = ''
        return r


class hooked_producer:
    """
    A producer that will call <function> when it empties,.
    with an argument of the number of bytes produced.  Useful
    for logging/instrumentation purposes.
    """

    def __init__ (self, producer, function):
        self.producer = producer
        self.function = function
        self.bytes = 0

    def more (self):
        if self.producer:
            result = self.producer.more()
            if not result:
                self.producer = None
                self.function (self.bytes)
            else:
                self.bytes = self.bytes + len(result)
            return result
        else:
            return ''

# HTTP 1.1 emphasizes that an advertised Content-Length header MUST be
# correct.  In the face of Strange Files, it is conceivable that
# reading a 'file' may produce an amount of data not matching that
# reported by os.stat() [text/binary mode issues, perhaps the file is
# being appended to, etc..]  This makes the chunked encoding a True
# Blessing, and it really ought to be used even with normal files.
# How beautifully it blends with the concept of the producer.

class chunked_producer:
    """A producer that implements the 'chunked' transfer coding for HTTP/1.1.
    Here is a sample usage:
            request['Transfer-Encoding'] = 'chunked'
            request.push (
                    producers.chunked_producer (your_producer)
                    )
            request.done()
    """

    def __init__ (self, producer, footers=None):
        self.producer = producer
        self.footers = footers

    def more (self):
        if self.producer:
            data = self.producer.more()
            if data:
                return '%x\r\n%s\r\n' % (len(data), data)
            else:
                self.producer = None
                if self.footers:
                    return string.join (
                            ['0'] + self.footers,
                            '\r\n'
                            ) + '\r\n\r\n'
                else:
                    return '0\r\n\r\n'
        else:
            return ''

class escaping_producer:

    "A producer that escapes a sequence of characters"
    " Common usage: escaping the CRLF.CRLF sequence in SMTP, NNTP, etc..."

    def __init__ (self, producer, esc_from='\r\n.', esc_to='\r\n..'):
        self.producer = producer
        self.esc_from = esc_from
        self.esc_to = esc_to
        self.buffer = ''
        self.find_prefix_at_end = find_prefix_at_end

    def more (self):
        esc_from = self.esc_from
        esc_to   = self.esc_to

        buffer = self.buffer + self.producer.more()

        if buffer:
            buffer = string.replace (buffer, esc_from, esc_to)
            i = self.find_prefix_at_end (buffer, esc_from)
            if i:
                # we found a prefix
                self.buffer = buffer[-i:]
                return buffer[:-i]
            else:
                # no prefix, return it all
                self.buffer = ''
                return buffer
        else:
            return buffer

class tail_logger:
    "Keep track of the last <size> log messages"
    def __init__ (self, logger, size=500):
        self.size = size
        self.logger = logger
        self.messages = []

    def log (self, message):
        self.messages.append (strip_eol (message))
        if len (self.messages) > self.size:
            del self.messages[0]
        self.logger.log (message)


def html_repr (object):
    so = escape (repr (object))
    if hasattr (object, 'hyper_respond'):
        return '<a href="/status/object/%d/">%s</a>' % (id (object), so)
    else:
        return so

def html_reprs (list, front='', back=''):
    reprs = map (
            lambda x,f=front,b=back: '%s%s%s' % (f,x,b),
            map (lambda x: escape (html_repr(x)), list)
            )
    reprs.sort()
    return reprs

# for example, tera, giga, mega, kilo
# p_d (n, (1024, 1024, 1024, 1024))
# smallest divider goes first - for example
# minutes, hours, days
# p_d (n, (60, 60, 24))

def progressive_divide (n, parts):
    result = []
    for part in parts:
        n, rem = divmod (n, part)
        result.append (rem)
    result.append (n)
    return result

# b,k,m,g,t
def split_by_units (n, units, dividers, format_string):
    divs = progressive_divide (n, dividers)
    result = []
    for i in range(len(units)):
        if divs[i]:
            result.append (format_string % (divs[i], units[i]))
    result.reverse()
    if not result:
        return [format_string % (0, units[0])]
    else:
        return result

def english_bytes (n):
    return split_by_units (
            n,
            ('','K','M','G','T'),
            (1024, 1024, 1024, 1024, 1024),
            '%d %sB'
            )

def english_time (n):
    return split_by_units (
            n,
            ('secs', 'mins', 'hours', 'days', 'weeks', 'years'),
            (         60,     60,      24,     7,       52),
            '%d %s'
            )

class file_logger:

    # pass this either a path or a file object.
    def __init__ (self, file, flush=1, mode='a'):
        if type(file) == type(''):
            if (file == '-'):
                self.file = sys.stdout
            else:
                self.file = open (file, mode)
        else:
            self.file = file
        self.do_flush = flush

    def __repr__ (self):
        return '<file logger: %s>' % self.file

    def write (self, data):
        self.file.write (data)
        self.maybe_flush()

    def writeline (self, line):
        self.file.writeline (line)
        self.maybe_flush()

    def writelines (self, lines):
        self.file.writelines (lines)
        self.maybe_flush()

    def maybe_flush (self):
        if self.do_flush:
            self.file.flush()

    def flush (self):
        self.file.flush()

    def softspace (self, *args):
        pass

    def log (self, message):
        if message[-1] not in ('\r', '\n'):
            self.write (message + '\n')
        else:
            self.write (message)

    def debug(self, message):
        self.log(message)

class unresolving_logger:
    "Just in case you don't want to resolve"
    def __init__ (self, logger):
        self.logger = logger

    def log (self, ip, message):
        self.logger.log ('%s:%s' % (ip, message))


def strip_eol (line):
    while line and line[-1] in '\r\n':
        line = line[:-1]
    return line

VERSION_STRING = string.split(RCS_ID)[2]
ATHANA_VERSION = "0.2.1"

# ===========================================================================
#                                                       Request Object
# ===========================================================================

class http_request:

    # default reply code
    reply_code = 200

    request_counter = counter()

    # Whether to automatically use chunked encoding when
    #
    #   HTTP version is 1.1
    #   Content-Length is not set
    #   Chunked encoding is not already in effect
    #
    # If your clients are having trouble, you might want to disable this.
    use_chunked = 1

    # by default, this request object ignores user data.
    collector = None

    def __init__ (self, *args):
        # unpack information about the request
        (self.channel, self.request,
         self.command, self.uri, self.version,
         self.header) = args

        self.outgoing = []
        self.reply_headers = {
                'Server'        : 'Athana/%s' % ATHANA_VERSION,
                'Date'          : build_http_date (time.time()),
                'Expires'       : build_http_date (time.time())
                }
        self.request_number = http_request.request_counter.increment()
        self._split_uri = None
        self._header_cache = {}

    # --------------------------------------------------
    # reply header management
    # --------------------------------------------------
    def __setitem__ (self, key, value):
        try:
            if key=='Set-Cookie':
                self.reply_headers[key] += [value]
            else:
                self.reply_headers[key] = [value]
        except:
            self.reply_headers[key] = [value]

    def __getitem__ (self, key):
        return self.reply_headers[key][0]

    def has_key (self, key):
        return self.reply_headers.has_key(key)

    def build_reply_header (self):
        h = []
        for k,vv in self.reply_headers.items():
            if type(vv) != type([]):
                h += ["%s: %s" % (k,vv)]
            else:
                for v in vv:
                    h += ["%s: %s" % (k,v)]
        return string.join([self.response(self.reply_code)] + h, '\r\n') + '\r\n\r\n'

    # --------------------------------------------------
    # split a uri
    # --------------------------------------------------

    # <path>;<params>?<query>#<fragment>
    path_regex = re.compile (
    #      path      params    query   fragment
            r'([^;?#]*)(;[^?#]*)?(\?[^#]*)?(#.*)?'
            )

    def split_uri (self):
        if self._split_uri is None:
            m = self.path_regex.match (self.uri)
            if m.end() != len(self.uri):
                raise ValueError, "Broken URI"
            else:
                self._split_uri = m.groups()
        return self._split_uri

    def get_header_with_regex (self, head_reg, group):
        for line in self.header:
            m = head_reg.match (line)
            if m.end() == len(line):
                return m.group (group)
        return ''

    def get_header (self, header):
        header = string.lower (header)
        hc = self._header_cache
        if not hc.has_key (header):
            h = header + ': '
            hl = len(h)
            for line in self.header:
                if string.lower (line[:hl]) == h:
                    r = line[hl:]
                    hc[header] = r
                    return r
            hc[header] = None
            return None
        else:
            return hc[header]

    # --------------------------------------------------
    # user data
    # --------------------------------------------------

    def collect_incoming_data (self, data):
        if self.collector:
            self.collector.collect_incoming_data (data)
        else:
            self.log_info(
                    'Dropping %d bytes of incoming request data' % len(data),
                    'warning'
                    )

    def found_terminator (self):
        if self.collector:
            self.collector.found_terminator()
        else:
            self.log_info (
                    'Unexpected end-of-record for incoming request',
                    'warning'
                    )

    def push (self, thing):
        if type(thing) == type(''):
            self.outgoing.append(simple_producer (thing))
        else:
            thing.more
            self.outgoing.append(thing)

    def response (self, code=200):
        message = self.responses[code]
        self.reply_code = code
        return 'HTTP/%s %d %s' % (self.version, code, message)

    def error (self, code, s=None):
        self.reply_code = code
        self.outgoing = []
        message = self.responses[code]
        if s is None:
            s = self.DEFAULT_ERROR_MESSAGE % {
                    'code': code,
                    'message': message,
                    }
        self['Content-Length'] = len(s)
        self['Content-Type'] = 'text/html'
        # make an error reply
        self.push (s)
        self.done()

    # can also be used for empty replies
    reply_now = error

    def done (self):
        "finalize this transaction - send output to the http channel"
       
        if hasattr(self,"tempfiles"):
            for f in self.tempfiles:
                os.unlink(f)

        # ----------------------------------------
        # persistent connection management
        # ----------------------------------------

        #  --- BUCKLE UP! ----

        connection = string.lower (get_header (CONNECTION, self.header))

        close_it = 0
        wrap_in_chunking = 0

        if self.version == '1.0':
            if connection == 'keep-alive':
                if not self.has_key ('Content-Length'):
                    close_it = 1
                else:
                    self['Connection'] = 'Keep-Alive'
            else:
                close_it = 1
        elif self.version == '1.1':
            if connection == 'close':
                close_it = 1
            elif not self.has_key ('Content-Length'):
                if self.has_key ('Transfer-Encoding'):
                    if not self['Transfer-Encoding'] == 'chunked':
                        close_it = 1
                elif self.use_chunked:
                    self['Transfer-Encoding'] = 'chunked'
                    wrap_in_chunking = 1
                else:
                    close_it = 1
        elif self.version is None:
            # Although we don't *really* support http/0.9 (because we'd have to
            # use \r\n as a terminator, and it would just yuck up a lot of stuff)
            # it's very common for developers to not want to type a version number
            # when using telnet to debug a server.
            close_it = 1

        outgoing_header = simple_producer (self.build_reply_header())

        if close_it:
            self['Connection'] = 'close'

        if wrap_in_chunking:
            outgoing_producer = chunked_producer (
                    composite_producer (list(self.outgoing))
                    )
            # prepend the header
            outgoing_producer = composite_producer(
                [outgoing_header, outgoing_producer]
                )
        else:
            # prepend the header
            self.outgoing.insert(0, outgoing_header)
            outgoing_producer = composite_producer (list(self.outgoing))

        # actually, this is already set to None by the handler:
        self.channel.current_request = None

        # apply a few final transformations to the output
        self.channel.push_with_producer (
                # globbing gives us large packets
                globbing_producer (
                            outgoing_producer
                        )
                )

        if close_it:
            self.channel.close_when_done()

    def log_date_string (self, when):
        t = time.localtime(when)
        return time.strftime ( '%d/%b/%Y:%H:%M:%S ', t)

    def log (self):
        self.channel.server.logger.log (
                self.channel.addr[0],
                '%d - - [%s] "%s"\n' % (
                        self.channel.addr[1],
                        self.log_date_string (time.time()),
                        self.request,
                        )
                )

    def write(self,text):
        if type(text) == type(''):
            self.push(text)
        elif type(text) == type(u''):
            self.push(text.encode("utf-8"))
        else:
            text.more
            self.push(text)

    def setStatus(self,status):
        self.reply_code = status

    def makeLink(self,page,params=None):
        query = ""
        if params is not None:
            first = 1
            for k,v in params.items():
                if first:
                    query += "?"
                else:
                    query += "&"
                query += urllib.quote(k)+"="+urllib.quote(v)
                first = 0
        return page+";"+self.sessionid+query

    def sendFile(self,path,content_type,force=0):

        try:
            file_length = os.stat(path)[stat.ST_SIZE]
        except OSError:
            self.error (404)
            return

        ims = get_header_match (IF_MODIFIED_SINCE, self.header)
        length_match = 1
        if ims:
            length = ims.group (4)
            if length:
                try:
                    length = string.atoi (length)
                    if length != file_length:
                        length_match = 0
                except:
                    pass
        ims_date = 0
        if ims:
            ims_date = parse_http_date (ims.group (1))

        try:
            mtime = os.stat (path)[stat.ST_MTIME]
        except:
            self.error (404)
            return
        if length_match and ims_date:
            if mtime <= ims_date and not force:
                print "File "+path+" was not modified since "+str(ims_date)+" (current filedate is "+str(mtime)+")-> 304"
                self.reply_code = 304
                return
        try:
            file = open (path, 'rb')
        except IOError:
            self.error (404)
            print "404"
            return

        self.reply_headers['Last-Modified'] = build_http_date (mtime)
        self.reply_headers['Content-Length'] = file_length
        self.reply_headers['Content-Type'] = content_type
        self.reply_headers['Connection'] = 'close';
        if self.command == 'GET':
            self.push(file_producer(file))
        return

    def setCookie(self, name, value, expire=None):
        if expire is None:
            s = name+'='+value;
        else:
            datestr = time.strftime("%a, %d-%b-%Y %H:%M:%S GMT", time.gmtime(expire))
            s = name+'='+value+'; expires='+datestr; #+'; path=PATH; domain=DOMAIN_NAME; secure';

        if 'Set-Cookie' not in self.reply_headers:
            self.reply_headers['Set-Cookie'] = [s]
        else:
            self.reply_headers['Set-Cookie'] += [s]

    def makeSelfLink(self,params):
        params2 = self.params.copy()
        for k,v in params.items():
            if v is not None:
                params2[k] = v
            else:
                try: del params2[k]
                except: pass
        ret = self.makeLink(self.fullpath, params2)
        return ret
        
    def writeTAL(self,page,context,macro=None):
        runTAL(self, context, file=page, macro=macro, request=self)
    
    def writeTALstr(self,string,context,macro=None):
        runTAL(self, context, string=string, macro=macro, request=self)

    def getTAL(self,page,context,macro=None):
        return processTAL(context,file=page, macro=macro, request=self)

    def getTALstr(self,string,context,macro=None):
        return processTAL(context,string=string, macro=macro, request=self)


    responses = {
            100: "Continue",
            101: "Switching Protocols",
            200: "OK",
            201: "Created",
            202: "Accepted",
            203: "Non-Authoritative Information",
            204: "No Content",
            205: "Reset Content",
            206: "Partial Content",
            300: "Multiple Choices",
            301: "Moved Permanently",
            302: "Moved Temporarily",
            303: "See Other",
            304: "Not Modified",
            305: "Use Proxy",
            400: "Bad Request",
            401: "Unauthorized",
            402: "Payment Required",
            403: "Forbidden",
            404: "Not Found",
            405: "Method Not Allowed",
            406: "Not Acceptable",
            407: "Proxy Authentication Required",
            408: "Request Time-out",
            409: "Conflict",
            410: "Gone",
            411: "Length Required",
            412: "Precondition Failed",
            413: "Request Entity Too Large",
            414: "Request-URI Too Large",
            415: "Unsupported Media Type",
            500: "Internal Server Error",
            501: "Not Implemented",
            502: "Bad Gateway",
            503: "Service Unavailable",
            504: "Gateway Time-out",
            505: "HTTP Version not supported"
            }

    # Default error message
    DEFAULT_ERROR_MESSAGE = string.join (
            ['<html><head>',
             '<title>Error response</title>',
             '</head>',
             '<body>',
             '<h1>Error response</h1>',
             '<p>Error code %(code)d.</p>',
             '<p>Message: %(message)s.</p>',
             '</body></html>',
             ''
             ],
            '\r\n'
            )

def getTAL(page,context,macro=None,language=None):
    return processTAL(context,file=page, macro=macro, language=language)

def getTALstr(string,context,macro=None,language=None):
    return processTAL(context,string=string, macro=macro, language=language)

# ===========================================================================
#                                                HTTP Channel Object
# ===========================================================================

class http_channel (async_chat):

    # use a larger default output buffer
    ac_out_buffer_size = 1<<16

    current_request = None
    channel_counter = counter()

    def __init__ (self, server, conn, addr):
        self.channel_number = http_channel.channel_counter.increment()
        self.request_counter = counter()
        async_chat.__init__ (self, conn)
        self.server = server
        self.addr = addr
        self.set_terminator ('\r\n\r\n')
        self.in_buffer = ''
        self.creation_time = int (time.time())
        self.check_maintenance()
        self.producer_lock = thread.allocate_lock()

    def initiate_send (self):
        self.producer_lock.acquire()
        try:
            async_chat.initiate_send(self)
        finally:
            self.producer_lock.release()

    def push (self, data):
        data.more
        self.producer_lock.acquire()
        try:
            self.producer_fifo.push (simple_producer (data))
        finally:
            self.producer_lock.release()
        self.initiate_send()

    def push_with_producer (self, producer):
        self.producer_lock.acquire()
        try:
            self.producer_fifo.push (producer)
        finally:
            self.producer_lock.release()
        self.initiate_send()
    
    def close_when_done (self):
        self.producer_lock.acquire()
        try:
            self.producer_fifo.push (None)
        finally:
            self.producer_lock.release()
        
        #results in select.error: (9, 'Bad file descriptor') if the socket map is poll'ed 
        #while this socket is being closed
        #we do it anyway, and catch the select.error in the main loop

        #XXX on Ubuntu's 2.6.10-5-386, the socket won't be closed until the select finishes (or
        #times out). We probably need to send a SIGINT signal or something. For now, we just
        #set a very small timeout (0.01) in the main loop, so that select() will be called often
        #enough.

        #it also results in a "NoneType has no attribute more" error if refill_buffer tries
        #to run data = p.more() on the None terminator (which we catch)
        try:
            self.initiate_send()
        except AttributeError:
            pass

    def __repr__ (self):
        ar = async_chat.__repr__(self)[1:-1]
        return '<%s channel#: %s requests:%s>' % (
                ar,
                self.channel_number,
                self.request_counter
                )

    # Channel Counter, Maintenance Interval...
    maintenance_interval = 500

    def check_maintenance (self):
        if not self.channel_number % self.maintenance_interval:
            self.maintenance()

    def maintenance (self):
        self.kill_zombies()

    # 30-minute zombie timeout.  status_handler also knows how to kill zombies.
    zombie_timeout = 30 * 60

    def kill_zombies (self):
        now = int (time.time())
        for channel in asyncore.socket_map.values():
            if channel.__class__ == self.__class__:
                if (now - channel.creation_time) > channel.zombie_timeout:
                    channel.close()

    # --------------------------------------------------
    # send/recv overrides, good place for instrumentation.
    # --------------------------------------------------

    # this information needs to get into the request object,
    # so that it may log correctly.
    def send (self, data):
        result = async_chat.send (self, data)
        self.server.bytes_out.increment (len(data))
        return result

    def recv (self, buffer_size):
        try:
            result = async_chat.recv (self, buffer_size)
            self.server.bytes_in.increment (len(result))
            return result
        except MemoryError:
            # --- Save a Trip to Your Service Provider ---
            # It's possible for a process to eat up all the memory of
            # the machine, and put it in an extremely wedged state,
            # where medusa keeps running and can't be shut down.  This
            # is where MemoryError tends to get thrown, though of
            # course it could get thrown elsewhere.
            sys.exit ("Out of Memory!")

    def handle_error (self):
        t, v = sys.exc_info()[:2]
        if t is SystemExit:
            raise t, v
        else:
            async_chat.handle_error (self)

    def log (self, *args):
        pass

    # --------------------------------------------------
    # async_chat methods
    # --------------------------------------------------

    def collect_incoming_data (self, data):
        if self.current_request:
            # we are receiving data (probably POST data) for a request
            self.current_request.collect_incoming_data (data)
        else:
            # we are receiving header (request) data
            self.in_buffer = self.in_buffer + data

    def found_terminator (self):
        if self.current_request:
            self.current_request.found_terminator()
        else:
            header = self.in_buffer
            self.in_buffer = ''
            lines = string.split (header, '\r\n')

            # --------------------------------------------------
            # crack the request header
            # --------------------------------------------------

            while lines and not lines[0]:
                # as per the suggestion of http-1.1 section 4.1, (and
                # Eric Parker <eparker@zyvex.com>), ignore a leading
                # blank lines (buggy browsers tack it onto the end of
                # POST requests)
                lines = lines[1:]

            if not lines:
                self.close_when_done()
                return

            request = lines[0]

            command, uri, version = crack_request (request)
            header = join_headers (lines[1:])

            # unquote path if necessary (thanks to Skip Montanaro for pointing
            # out that we must unquote in piecemeal fashion).
            rpath, rquery = splitquery(uri)
            if '%' in rpath:
                if rquery:
                    uri = unquote (rpath) + '?' + rquery
                else:
                    uri = unquote (rpath)

            r = http_request (self, request, command, uri, version, header)
            self.request_counter.increment()
            self.server.total_requests.increment()

            if command is None:
                self.log_info ('Bad HTTP request: %s' % repr(request), 'error')
                r.error (400)
                return

            # --------------------------------------------------
            # handler selection and dispatch
            # --------------------------------------------------
            for h in self.server.handlers:
                if h.match (r):
                    try:
                        self.current_request = r
                        # This isn't used anywhere.
                        # r.handler = h # CYCLE
                        h.handle_request (r)
                    except:
                        self.server.exceptions.increment()
                        (file, fun, line), t, v, tbinfo = asyncore.compact_traceback()
                        self.log_info(
                                        'Server Error: %s, %s: file: %s line: %s' % (t,v,file,line),
                                        'error')
                        try:
                            r.error (500)
                        except:
                            pass
                    return

            # no handlers, so complain
            r.error (404)

# ===========================================================================
#                                                HTTP Server Object
# ===========================================================================

class http_server (asyncore.dispatcher):

    SERVER_IDENT = 'HTTP Server (V%s)' % VERSION_STRING

    channel_class = http_channel

    def __init__ (self, ip, port, resolver=None, logger_object=None):
        self.ip = ip
        self.port = port
        asyncore.dispatcher.__init__ (self)
        self.create_socket (socket.AF_INET, socket.SOCK_STREAM)

        self.handlers = []

        if not logger_object:
            logger_object = file_logger (sys.stdout)

        self.set_reuse_addr()
        self.bind ((ip, port))

        # lower this to 5 if your OS complains
        self.listen (1024)

        host, port = self.socket.getsockname()
        if not ip:
            self.log_info('Computing default hostname', 'warning')
            ip = socket.gethostbyname (socket.gethostname())
        try:
            self.server_name = socket.gethostbyaddr (ip)[0]
        except socket.error:
            self.log_info('Cannot do reverse lookup', 'warning')
            self.server_name = ip       # use the IP address as the "hostname"

        self.server_port = port
        self.total_clients = counter()
        self.total_requests = counter()
        self.exceptions = counter()
        self.bytes_out = counter()
        self.bytes_in  = counter()

        if not logger_object:
            logger_object = file_logger (sys.stdout)

        self.logger = unresolving_logger (logger_object)

        self.log_info (
                'Athana (%s) started at %s'
                '\n\n'
                'The server is running! You can now direct your browser to:\n'
                '\thttp://%s:%d/'
                '\n' % (
                        ATHANA_VERSION,
                        time.ctime(time.time()),
                        self.server_name,
                        port,
                        )
                )

    def writable (self):
        return 0

    def handle_read (self):
        pass

    def readable (self):
        return self.accepting

    def handle_connect (self):
        pass

    def handle_accept (self):
        self.total_clients.increment()
        try:
            conn, addr = self.accept()
        except socket.error:
            # linux: on rare occasions we get a bogus socket back from
            # accept.  socketmodule.c:makesockaddr complains that the
            # address family is unknown.  We don't want the whole server
            # to shut down because of this.
            self.log_info ('warning: server accept() threw an exception', 'warning')
            return
        except TypeError:
            # unpack non-sequence.  this can happen when a read event
            # fires on a listening socket, but when we call accept()
            # we get EWOULDBLOCK, so dispatcher.accept() returns None.
            # Seen on FreeBSD3.
            self.log_info ('warning: server accept() threw EWOULDBLOCK', 'warning')
            return

        self.channel_class (self, conn, addr)

    def install_handler (self, handler, back=0):
        if back:
            self.handlers.append (handler)
        else:
            self.handlers.insert (0, handler)

    def remove_handler (self, handler):
        self.handlers.remove (handler)


CONNECTION = re.compile ('Connection: (.*)', re.IGNORECASE)

# merge multi-line headers
# [486dx2: ~500/sec]
def join_headers (headers):
    r = []
    for i in range(len(headers)):
        if headers[i][0] in ' \t':
            r[-1] = r[-1] + headers[i][1:]
        else:
            r.append (headers[i])
    return r

def get_header (head_reg, lines, group=1):
    for line in lines:
        m = head_reg.match (line)
        if m and m.end() == len(line):
            return m.group (group)
    return ''

def get_header_match (head_reg, lines):
    for line in lines:
        m = head_reg.match (line)
        if m and m.end() == len(line):
            return m
    return ''

REQUEST = re.compile ('([^ ]+) ([^ ]+)(( HTTP/([0-9.]+))$|$)')

def crack_request (r):
    m = REQUEST.match (r)
    if m and m.end() == len(r):
        if m.group(3):
            version = m.group(5)
        else:
            version = None
        return m.group(1), m.group(2), version
    else:
        return None, None, None


# This is the 'default' handler.  it implements the base set of
# features expected of a simple file-delivering HTTP server.  file
# services are provided through a 'filesystem' object, the very same
# one used by the FTP server.
#
# You can replace or modify this handler if you want a non-standard
# HTTP server.  You can also derive your own handler classes from
# it.
#
# support for handling POST requests is available in the derived
# class <default_with_post_handler>, defined below.
#

class default_handler:

    valid_commands = ['GET', 'HEAD']

    IDENT = 'Default HTTP Request Handler'

    # Pathnames that are tried when a URI resolves to a directory name
    directory_defaults = [
            'index.html',
            'default.html'
            ]

    default_file_producer = file_producer

    def __init__ (self, filesystem):
        self.filesystem = filesystem
        # count total hits
        self.hit_counter = counter()
        # count file deliveries
        self.file_counter = counter()
        # count cache hits
        self.cache_counter = counter()

    hit_counter = 0

    def __repr__ (self):
        return '<%s (%s hits) at %x>' % (
                self.IDENT,
                self.hit_counter,
                id (self)
                )

    # always match, since this is a default
    def match (self, request):
        return 1

    def can_handle(self, request):
        path, params, query, fragment = request.split_uri()
        if '%' in path:
            path = unquote (path)
        while path and path[0] == '/':
            path = path[1:]
        if self.filesystem.isdir (path):
            if path and path[-1] != '/':
                return 0
            found = 0
            if path and path[-1] != '/':
                path = path + '/'
            for default in self.directory_defaults:
                p = path + default
                if self.filesystem.isfile (p):
                    path = p
                    found = 1
                    break
            if not found:
                return 0
        elif not self.filesystem.isfile (path):
            return 0
        return 1

    # handle a file request, with caching.

    def handle_request (self, request):

        if request.command not in self.valid_commands:
            request.error (400) # bad request
            return

        self.hit_counter.increment()

        path, params, query, fragment = request.split_uri()

        if '%' in path:
            path = unquote (path)
        
        # strip off all leading slashes
        while path and path[0] == '/':
            path = path[1:]

        if self.filesystem.isdir (path):
            if path and path[-1] != '/':
                request['Location'] = 'http://%s/%s/' % (
                        request.channel.server.server_name,
                        path
                        )
                request.error (301)
                return

            # we could also generate a directory listing here,
            # may want to move this into another method for that
            # purpose
            found = 0
            if path and path[-1] != '/':
                path = path + '/'
            for default in self.directory_defaults:
                p = path + default
                if self.filesystem.isfile (p):
                    path = p
                    found = 1
                    break
            if not found:
                request.error (404) # Not Found
                return

        elif not self.filesystem.isfile (path):
            request.error (404) # Not Found
            return

        file_length = self.filesystem.stat (path)[stat.ST_SIZE]

        ims = get_header_match (IF_MODIFIED_SINCE, request.header)
        
        length_match = 1
        if ims:
            length = ims.group (4)
            if length:
                try:
                    length = string.atoi (length)
                    if length != file_length:
                        length_match = 0
                except:
                    pass

        ims_date = 0

        if ims:
            ims_date = parse_http_date (ims.group (1))

        try:
            mtime = self.filesystem.stat (path)[stat.ST_MTIME]
        except:
            request.error (404)
            return
        
        if length_match and ims_date:
            if mtime <= ims_date:
                request.reply_code = 304
                request.done()
                self.cache_counter.increment()
                print "File "+path+" was not modified since "+str(ims_date)+" (current filedate is "+str(mtime)+")"
                return
        try:
            file = self.filesystem.open (path, 'rb')
        except IOError:
            request.error (404)
            return

        request['Last-Modified'] = build_http_date (mtime)
        request['Content-Length'] = file_length
        self.set_content_type (path, request)

        if request.command == 'GET':
            request.push (self.default_file_producer (file))

        self.file_counter.increment()
        request.done()

    def set_content_type (self, path, request):
        ext = string.lower (get_extension (path))
        typ, encoding = mimetypes.guess_type(path)
        if typ is not None:
            request['Content-Type'] = typ
        else:
            # TODO: test a chunk off the front of the file for 8-bit
            # characters, and use application/octet-stream instead.
            request['Content-Type'] = 'text/plain'

    def status (self):
        return simple_producer (
                '<li>%s' % html_repr (self)
                + '<ul>'
                + '  <li><b>Total Hits:</b> %s'                 % self.hit_counter
                + '  <li><b>Files Delivered:</b> %s'    % self.file_counter
                + '  <li><b>Cache Hits:</b> %s'                 % self.cache_counter
                + '</ul>'
                )

# HTTP/1.0 doesn't say anything about the "; length=nnnn" addition
# to this header.  I suppose its purpose is to avoid the overhead
# of parsing dates...
IF_MODIFIED_SINCE = re.compile (
        'If-Modified-Since: ([^;]+)((; length=([0-9]+)$)|$)',
        re.IGNORECASE
        )

USER_AGENT = re.compile ('User-Agent: (.*)', re.IGNORECASE)

CONTENT_TYPE = re.compile (
        r'Content-Type: ([^;]+)((; boundary=([A-Za-z0-9\'\(\)+_,./:=?-]+)$)|$)',
        re.IGNORECASE
        )

get_header = get_header
get_header_match = get_header_match

def get_extension (path):
    dirsep = string.rfind (path, '/')
    dotsep = string.rfind (path, '.')
    if dotsep > dirsep:
        return path[dotsep+1:]
    else:
        return ''

class abstract_filesystem:
    def __init__ (self):
        pass

    def current_directory (self):
        "Return a string representing the current directory."
        pass

    def listdir (self, path, long=0):
        """Return a listing of the directory at 'path' The empty string
        indicates the current directory.  If 'long' is set, instead
        return a list of (name, stat_info) tuples
        """
        pass

    def open (self, path, mode):
        "Return an open file object"
        pass

    def stat (self, path):
        "Return the equivalent of os.stat() on the given path."
        pass

    def isdir (self, path):
        "Does the path represent a directory?"
        pass

    def isfile (self, path):
        "Does the path represent a plain file?"
        pass

    def cwd (self, path):
        "Change the working directory."
        pass

    def cdup (self):
        "Change to the parent of the current directory."
        pass


    def longify (self, path):
        """Return a 'long' representation of the filename
        [for the output of the LIST command]"""
        pass

# standard wrapper around a unix-like filesystem, with a 'false root'
# capability.

# security considerations: can symbolic links be used to 'escape' the
# root?  should we allow it?  if not, then we could scan the
# filesystem on startup, but that would not help if they were added
# later.  We will probably need to check for symlinks in the cwd method.

# what to do if wd is an invalid directory?

def safe_stat (path):
    try:
        return (path, os.stat (path))
    except:
        return None

class os_filesystem:
    path_module = os.path

    # set this to zero if you want to disable pathname globbing.
    # [we currently don't glob, anyway]
    do_globbing = 1

    def __init__ (self, root, wd='/'):
        self.root = root
        self.wd = wd

    def current_directory (self):
        return self.wd

    def isfile (self, path):
        p = self.normalize (self.path_module.join (self.wd, path))
        return self.path_module.isfile (self.translate(p))

    def isdir (self, path):
        p = self.normalize (self.path_module.join (self.wd, path))
        return self.path_module.isdir (self.translate(p))

    def cwd (self, path):
        p = self.normalize (self.path_module.join (self.wd, path))
        translated_path = self.translate(p)
        if not self.path_module.isdir (translated_path):
            return 0
        else:
            old_dir = os.getcwd()
            # temporarily change to that directory, in order
            # to see if we have permission to do so.
            try:
                can = 0
                try:
                    os.chdir (translated_path)
                    can = 1
                    self.wd = p
                except:
                    pass
            finally:
                if can:
                    os.chdir (old_dir)
            return can

    def cdup (self):
        return self.cwd ('..')

    def listdir (self, path, long=0):
        p = self.translate (path)
        # I think we should glob, but limit it to the current
        # directory only.
        ld = os.listdir (p)
        if not long:
            return list_producer (ld, None)
        else:
            old_dir = os.getcwd()
            try:
                os.chdir (p)
                # if os.stat fails we ignore that file.
                result = filter (None, map (safe_stat, ld))
            finally:
                os.chdir (old_dir)
            return list_producer (result, self.longify)

    # TODO: implement a cache w/timeout for stat()
    def stat (self, path):
        p = self.translate (path)
        return os.stat (p)

    def open (self, path, mode):
        p = self.translate (path)
        return open (p, mode)

    def unlink (self, path):
        p = self.translate (path)
        return os.unlink (p)

    def mkdir (self, path):
        p = self.translate (path)
        return os.mkdir (p)

    def rmdir (self, path):
        p = self.translate (path)
        return os.rmdir (p)

    # utility methods
    def normalize (self, path):
        # watch for the ever-sneaky '/+' path element
        path = re.sub('/+', '/', path)
        p = self.path_module.normpath (path)
        # remove 'dangling' cdup's.
        if len(p) > 2 and p[:3] == '/..':
            p = '/'
        return p

    def translate (self, path):
        # we need to join together three separate
        # path components, and do it safely.
        # <real_root>/<current_directory>/<path>
        # use the operating system's path separator.
        path = string.join (string.split (path, '/'), os.sep)
        p = self.normalize (self.path_module.join (self.wd, path))
        p = self.normalize (self.path_module.join (self.root, p[1:]))
        return p

    def longify (self, (path, stat_info)):
        return unix_longify (path, stat_info)

    def __repr__ (self):
        return '<unix-style fs root:%s wd:%s>' % (
                self.root,
                self.wd
                )

# this matches the output of NT's ftp server (when in
# MSDOS mode) exactly.

def msdos_longify (file, stat_info):
    if stat.S_ISDIR (stat_info[stat.ST_MODE]):
        dir = '<DIR>'
    else:
        dir = '     '
    date = msdos_date (stat_info[stat.ST_MTIME])
    return '%s       %s %8d %s' % (
            date,
            dir,
            stat_info[stat.ST_SIZE],
            file
            )

def msdos_date (t):
    try:
        info = time.gmtime (t)
    except:
        info = time.gmtime (0)
    # year, month, day, hour, minute, second, ...
    if info[3] > 11:
        merid = 'PM'
        info[3] = info[3] - 12
    else:
        merid = 'AM'
    return '%02d-%02d-%02d  %02d:%02d%s' % (
            info[1],
            info[2],
            info[0]%100,
            info[3],
            info[4],
            merid
            )

months = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun',
                  'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']

mode_table = {
        '0':'---',
        '1':'--x',
        '2':'-w-',
        '3':'-wx',
        '4':'r--',
        '5':'r-x',
        '6':'rw-',
        '7':'rwx'
        }

def unix_longify (file, stat_info):
    # for now, only pay attention to the lower bits
    mode = ('%o' % stat_info[stat.ST_MODE])[-3:]
    mode = string.join (map (lambda x: mode_table[x], mode), '')
    if stat.S_ISDIR (stat_info[stat.ST_MODE]):
        dirchar = 'd'
    else:
        dirchar = '-'
    date = ls_date (long(time.time()), stat_info[stat.ST_MTIME])
    return '%s%s %3d %-8d %-8d %8d %s %s' % (
            dirchar,
            mode,
            stat_info[stat.ST_NLINK],
            stat_info[stat.ST_UID],
            stat_info[stat.ST_GID],
            stat_info[stat.ST_SIZE],
            date,
            file
            )

# Emulate the unix 'ls' command's date field.
# it has two formats - if the date is more than 180
# days in the past, then it's like this:
# Oct 19  1995
# otherwise, it looks like this:
# Oct 19 17:33

def ls_date (now, t):
    try:
        info = time.gmtime (t)
    except:
        info = time.gmtime (0)
    # 15,600,000 == 86,400 * 180
    if (now - t) > 15600000:
        return '%s %2d  %d' % (
                months[info[1]-1],
                info[2],
                info[0]
                )
    else:
        return '%s %2d %02d:%02d' % (
                months[info[1]-1],
                info[2],
                info[3],
                info[4]
                )

class list_producer:
    def __init__ (self, list, func=None):
        self.list = list
        self.func = func

    # this should do a pushd/popd
    def more (self):
        if not self.list:
            return ''
        else:
            # do a few at a time
            bunch = self.list[:50]
            if self.func is not None:
                bunch = map (self.func, bunch)
            self.list = self.list[50:]
            return string.joinfields (bunch, '\r\n') + '\r\n'

class hooked_callback:
    def __init__ (self, hook, callback):
        self.hook, self.callback = hook, callback

    def __call__ (self, *args):
        apply (self.hook, args)
        apply (self.callback, args)

# An extensible, configurable, asynchronous FTP server.
#
# All socket I/O is non-blocking, however file I/O is currently
# blocking.  Eventually file I/O may be made non-blocking, too, if it
# seems necessary.  Currently the only CPU-intensive operation is
# getting and formatting a directory listing.  [this could be moved
# into another process/directory server, or another thread?]
#
# Only a subset of RFC 959 is implemented, but much of that RFC is
# vestigial anyway.  I've attempted to include the most commonly-used
# commands, using the feature set of wu-ftpd as a guide.


# TODO: implement a directory listing cache.  On very-high-load
# servers this could save a lot of disk abuse, and possibly the
# work of computing emulated unix ls output.

# Potential security problem with the FTP protocol?  I don't think
# there's any verification of the origin of a data connection.  Not
# really a problem for the server (since it doesn't send the port
# command, except when in PASV mode) But I think a data connection
# could be spoofed by a program with access to a sniffer - it could
# watch for a PORT command to go over a command channel, and then
# connect to that port before the server does.

# Unix user id's:
# In order to support assuming the id of a particular user,
# it seems there are two options:
# 1) fork, and seteuid in the child
# 2) carefully control the effective uid around filesystem accessing
#    methods, using try/finally. [this seems to work]

VERSION = string.split(RCS_ID)[2]

class ftp_channel (async_chat):

    # defaults for a reliable __repr__
    addr = ('unknown','0')

    # unset this in a derived class in order
    # to enable the commands in 'self.write_commands'
    read_only = 1
    write_commands = ['appe','dele','mkd','rmd','rnfr','rnto','stor','stou']

    restart_position = 0

    # comply with (possibly troublesome) RFC959 requirements
    # This is necessary to correctly run an active data connection
    # through a firewall that triggers on the source port (expected
    # to be 'L-1', or 20 in the normal case).
    bind_local_minus_one = 0

    def __init__ (self, server, conn, addr):
        self.server = server
        self.current_mode = 'a'
        self.addr = addr
        async_chat.__init__ (self, conn)
        self.set_terminator ('\r\n')

        # client data port.  Defaults to 'the same as the control connection'.
        self.client_addr = (addr[0], 21)

        self.client_dc = None
        self.in_buffer = ''
        self.closing = 0
        self.passive_acceptor = None
        self.passive_connection = None
        self.filesystem = None
        self.authorized = 0
        # send the greeting
        self.respond (
                '220 %s FTP server (Medusa Async V%s [experimental]) ready.' % (
                        self.server.hostname,
                        VERSION
                        )
                )

#       def __del__ (self):
#               print 'ftp_channel.__del__()'

    # --------------------------------------------------
    # async-library methods
    # --------------------------------------------------

    def handle_expt (self):
        # this is handled below.  not sure what I could
        # do here to make that code less kludgish.
        pass

    def collect_incoming_data (self, data):
        self.in_buffer = self.in_buffer + data
        if len(self.in_buffer) > 4096:
            # silently truncate really long lines
            # (possible denial-of-service attack)
            self.in_buffer = ''

    def found_terminator (self):

        line = self.in_buffer

        if not len(line):
            return

        sp = string.find (line, ' ')
        if sp != -1:
            line = [line[:sp], line[sp+1:]]
        else:
            line = [line]

        command = string.lower (line[0])
        # watch especially for 'urgent' abort commands.
        if string.find (command, 'abor') != -1:
            # strip off telnet sync chars and the like...
            while command and command[0] not in string.letters:
                command = command[1:]
        fun_name = 'cmd_%s' % command
        if command != 'pass':
            self.log ('<== %s' % repr(self.in_buffer)[1:-1])
        else:
            self.log ('<== %s' % line[0]+' <password>')
        self.in_buffer = ''
        if not hasattr (self, fun_name):
            self.command_not_understood (line[0])
            return
        fun = getattr (self, fun_name)
        if (not self.authorized) and (command not in ('user', 'pass', 'help', 'quit')):
            self.respond ('530 Please log in with USER and PASS')
        elif (not self.check_command_authorization (command)):
            self.command_not_authorized (command)
        else:
            try:
                result = apply (fun, (line,))
            except:
                self.server.total_exceptions.increment()
                (file, fun, line), t,v, tbinfo = asyncore.compact_traceback()
                if self.client_dc:
                    try:
                        self.client_dc.close()
                    except:
                        pass
                self.respond (
                        '451 Server Error: %s, %s: file: %s line: %s' % (
                                t,v,file,line,
                                )
                        )

    closed = 0
    def close (self):
        if not self.closed:
            self.closed = 1
            if self.passive_acceptor:
                self.passive_acceptor.close()
            if self.client_dc:
                self.client_dc.close()
            self.server.closed_sessions.increment()
            async_chat.close (self)

    # --------------------------------------------------
    # filesystem interface functions.
    # override these to provide access control or perform
    # other functions.
    # --------------------------------------------------

    def cwd (self, line):
        return self.filesystem.cwd (line[1])

    def cdup (self, line):
        return self.filesystem.cdup()

    def open (self, path, mode):
        return self.filesystem.open (path, mode)

    # returns a producer
    def listdir (self, path, long=0):
        return self.filesystem.listdir (path, long)

    def get_dir_list (self, line, long=0):
        # we need to scan the command line for arguments to '/bin/ls'...
        args = line[1:]
        path_args = []
        for arg in args:
            if arg[0] != '-':
                path_args.append (arg)
            else:
                # ignore arguments
                pass
        if len(path_args) < 1:
            dir = '.'
        else:
            dir = path_args[0]
        return self.listdir (dir, long)

    # --------------------------------------------------
    # authorization methods
    # --------------------------------------------------

    def check_command_authorization (self, command):
        if command in self.write_commands and self.read_only:
            return 0
        else:
            return 1

    # --------------------------------------------------
    # utility methods
    # --------------------------------------------------

    def log (self, message):
        self.server.logger.log (
                self.addr[0],
                '%d %s' % (
                        self.addr[1], message
                        )
                )

    def respond (self, resp):
        self.log ('==> %s' % resp)
        self.push (resp + '\r\n')

    def command_not_understood (self, command):
        self.respond ("500 '%s': command not understood." % command)

    def command_not_authorized (self, command):
        self.respond (
                "530 You are not authorized to perform the '%s' command" % (
                        command
                        )
                )

    def make_xmit_channel (self):
        # In PASV mode, the connection may or may _not_ have been made
        # yet.  [although in most cases it is... FTP Explorer being
        # the only exception I've yet seen].  This gets somewhat confusing
        # because things may happen in any order...
        pa = self.passive_acceptor
        if pa:
            if pa.ready:
                # a connection has already been made.
                conn, addr = self.passive_acceptor.ready
                cdc = xmit_channel (self, addr)
                cdc.set_socket (conn)
                cdc.connected = 1
                self.passive_acceptor.close()
                self.passive_acceptor = None
            else:
                # we're still waiting for a connect to the PASV port.
                cdc = xmit_channel (self)
        else:
            # not in PASV mode.
            ip, port = self.client_addr
            cdc = xmit_channel (self, self.client_addr)
            cdc.create_socket (socket.AF_INET, socket.SOCK_STREAM)
            if self.bind_local_minus_one:
                cdc.bind (('', self.server.port - 1))
            try:
                cdc.connect ((ip, port))
            except socket.error, why:
                self.respond ("425 Can't build data connection")
        self.client_dc = cdc

    # pretty much the same as xmit, but only right on the verge of
    # being worth a merge.
    def make_recv_channel (self, fd):
        pa = self.passive_acceptor
        if pa:
            if pa.ready:
                # a connection has already been made.
                conn, addr = pa.ready
                cdc = recv_channel (self, addr, fd)
                cdc.set_socket (conn)
                cdc.connected = 1
                self.passive_acceptor.close()
                self.passive_acceptor = None
            else:
                # we're still waiting for a connect to the PASV port.
                cdc = recv_channel (self, None, fd)
        else:
            # not in PASV mode.
            ip, port = self.client_addr
            cdc = recv_channel (self, self.client_addr, fd)
            cdc.create_socket (socket.AF_INET, socket.SOCK_STREAM)
            try:
                cdc.connect ((ip, port))
            except socket.error, why:
                self.respond ("425 Can't build data connection")
        self.client_dc = cdc

    type_map = {
            'a':'ASCII',
            'i':'Binary',
            'e':'EBCDIC',
            'l':'Binary'
            }

    type_mode_map = {
            'a':'t',
            'i':'b',
            'e':'b',
            'l':'b'
            }

    # --------------------------------------------------
    # command methods
    # --------------------------------------------------

    def cmd_type (self, line):
        'specify data transfer type'
        # ascii, ebcdic, image, local <byte size>
        t = string.lower (line[1])
        # no support for EBCDIC
        # if t not in ['a','e','i','l']:
        if t not in ['a','i','l']:
            self.command_not_understood (string.join (line))
        elif t == 'l' and (len(line) > 2 and line[2] != '8'):
            self.respond ('504 Byte size must be 8')
        else:
            self.current_mode = t
            self.respond ('200 Type set to %s.' % self.type_map[t])


    def cmd_quit (self, line):
        'terminate session'
        self.respond ('221 Goodbye.')
        self.close_when_done()

    def cmd_port (self, line):
        'specify data connection port'
        info = string.split (line[1], ',')
        ip = string.join (info[:4], '.')
        port = string.atoi(info[4])*256 + string.atoi(info[5])
        # how many data connections at a time?
        # I'm assuming one for now...
        # TODO: we should (optionally) verify that the
        # ip number belongs to the client.  [wu-ftpd does this?]
        self.client_addr = (ip, port)
        self.respond ('200 PORT command successful.')

    def new_passive_acceptor (self):
        # ensure that only one of these exists at a time.
        if self.passive_acceptor is not None:
            self.passive_acceptor.close()
            self.passive_acceptor = None
        self.passive_acceptor = passive_acceptor (self)
        return self.passive_acceptor

    def cmd_pasv (self, line):
        'prepare for server-to-server transfer'
        pc = self.new_passive_acceptor()
        port = pc.addr[1]
        ip_addr = pc.control_channel.getsockname()[0]
        self.respond (
                '227 Entering Passive Mode (%s,%d,%d)' % (
                        string.replace(ip_addr, '.', ','),
                        port/256,
                        port%256
                        )
                )
        self.client_dc = None

    def cmd_nlst (self, line):
        'give name list of files in directory'
        # ncftp adds the -FC argument for the user-visible 'nlist'
        # command.  We could try to emulate ls flags, but not just yet.
        if '-FC' in line:
            line.remove ('-FC')
        try:
            dir_list_producer = self.get_dir_list (line, 0)
        except os.error, why:
            self.respond ('550 Could not list directory: %s' % why)
            return
        self.respond (
                '150 Opening %s mode data connection for file list' % (
                        self.type_map[self.current_mode]
                        )
                )
        self.make_xmit_channel()
        self.client_dc.push_with_producer (dir_list_producer)
        self.client_dc.close_when_done()

    def cmd_list (self, line):
        'give a list of files in a directory'
        try:
            dir_list_producer = self.get_dir_list (line, 1)
        except os.error, why:
            self.respond ('550 Could not list directory: %s' % why)
            return
        self.respond (
                '150 Opening %s mode data connection for file list' % (
                        self.type_map[self.current_mode]
                        )
                )
        self.make_xmit_channel()
        self.client_dc.push_with_producer (dir_list_producer)
        self.client_dc.close_when_done()

    def cmd_cwd (self, line):
        'change working directory'
        if self.cwd (line):
            self.respond ('250 CWD command successful.')
        else:
            self.respond ('550 No such directory.')

    def cmd_cdup (self, line):
        'change to parent of current working directory'
        if self.cdup(line):
            self.respond ('250 CDUP command successful.')
        else:
            self.respond ('550 No such directory.')

    def cmd_pwd (self, line):
        'print the current working directory'
        self.respond (
                '257 "%s" is the current directory.' % (
                        self.filesystem.current_directory()
                        )
                )

    # modification time
    # example output:
    # 213 19960301204320
    def cmd_mdtm (self, line):
        'show last modification time of file'
        filename = line[1]
        if not self.filesystem.isfile (filename):
            self.respond ('550 "%s" is not a file' % filename)
        else:
            mtime = time.gmtime(self.filesystem.stat(filename)[stat.ST_MTIME])
            self.respond (
                    '213 %4d%02d%02d%02d%02d%02d' % (
                            mtime[0],
                            mtime[1],
                            mtime[2],
                            mtime[3],
                            mtime[4],
                            mtime[5]
                            )
                    )

    def cmd_noop (self, line):
        'do nothing'
        self.respond ('200 NOOP command successful.')

    def cmd_size (self, line):
        'return size of file'
        filename = line[1]
        if not self.filesystem.isfile (filename):
            self.respond ('550 "%s" is not a file' % filename)
        else:
            self.respond (
                    '213 %d' % (self.filesystem.stat(filename)[stat.ST_SIZE])
                    )

    def cmd_retr (self, line):
        'retrieve a file'
        if len(line) < 2:
            self.command_not_understood (string.join (line))
        else:
            file = line[1]
            if not self.filesystem.isfile (file):
                self.log_info ('checking %s' % file)
                self.respond ('550 No such file')
            else:
                try:
                    # FIXME: for some reason, 'rt' isn't working on win95
                    mode = 'r'+self.type_mode_map[self.current_mode]
                    fd = self.open (file, mode)
                except IOError, why:
                    self.respond ('553 could not open file for reading: %s' % (repr(why)))
                    return
                self.respond (
                        "150 Opening %s mode data connection for file '%s'" % (
                                self.type_map[self.current_mode],
                                file
                                )
                        )
                self.make_xmit_channel()

                if self.restart_position:
                    # try to position the file as requested, but
                    # give up silently on failure (the 'file object'
                    # may not support seek())
                    try:
                        fd.seek (self.restart_position)
                    except:
                        pass
                    self.restart_position = 0

                self.client_dc.push_with_producer (
                        file_producer (fd)
                        )
                self.client_dc.close_when_done()

    def cmd_stor (self, line, mode='wb'):
        'store a file'
        if len (line) < 2:
            self.command_not_understood (string.join (line))
        else:
            if self.restart_position:
                restart_position = 0
                self.respond ('553 restart on STOR not yet supported')
                return
            file = line[1]
            # todo: handle that type flag
            try:
                fd = self.open (file, mode)
            except IOError, why:
                self.respond ('553 could not open file for writing: %s' % (repr(why)))
                return
            self.respond (
                    '150 Opening %s connection for %s' % (
                            self.type_map[self.current_mode],
                            file
                            )
                    )
            self.make_recv_channel (fd)

    def cmd_abor (self, line):
        'abort operation'
        if self.client_dc:
            self.client_dc.close()
        self.respond ('226 ABOR command successful.')

    def cmd_appe (self, line):
        'append to a file'
        return self.cmd_stor (line, 'ab')

    def cmd_dele (self, line):
        if len (line) != 2:
            self.command_not_understood (string.join (line))
        else:
            file = line[1]
            if self.filesystem.isfile (file):
                try:
                    self.filesystem.unlink (file)
                    self.respond ('250 DELE command successful.')
                except:
                    self.respond ('550 error deleting file.')
            else:
                self.respond ('550 %s: No such file.' % file)

    def cmd_mkd (self, line):
        if len (line) != 2:
            self.command_not_understood (string.join (line))
        else:
            path = line[1]
            try:
                self.filesystem.mkdir (path)
                self.respond ('257 MKD command successful.')
            except:
                self.respond ('550 error creating directory.')

    def cmd_rmd (self, line):
        if len (line) != 2:
            self.command_not_understood (string.join (line))
        else:
            path = line[1]
            try:
                self.filesystem.rmdir (path)
                self.respond ('250 RMD command successful.')
            except:
                self.respond ('550 error removing directory.')

    def cmd_user (self, line):
        'specify user name'
        if len(line) > 1:
            self.user = line[1]
            self.respond ('331 Password required.')
        else:
            self.command_not_understood (string.join (line))

    def cmd_pass (self, line):
        'specify password'
        if len(line) < 2:
            pw = ''
        else:
            pw = line[1]
        result, message, fs = self.server.authorizer.authorize (self, self.user, pw)
        if result:
            self.respond ('230 %s' % message)
            self.filesystem = fs
            self.authorized = 1
            self.log_info('Successful login: Filesystem=%s' % repr(fs))
        else:
            self.respond ('530 %s' % message)

    def cmd_rest (self, line):
        'restart incomplete transfer'
        try:
            pos = string.atoi (line[1])
        except ValueError:
            self.command_not_understood (string.join (line))
        self.restart_position = pos
        self.respond (
                '350 Restarting at %d. Send STORE or RETRIEVE to initiate transfer.' % pos
                )

    def cmd_stru (self, line):
        'obsolete - set file transfer structure'
        if line[1] in 'fF':
            # f == 'file'
            self.respond ('200 STRU F Ok')
        else:
            self.respond ('504 Unimplemented STRU type')

    def cmd_mode (self, line):
        'obsolete - set file transfer mode'
        if line[1] in 'sS':
            # f == 'file'
            self.respond ('200 MODE S Ok')
        else:
            self.respond ('502 Unimplemented MODE type')

# The stat command has two personalities.  Normally it returns status
# information about the current connection.  But if given an argument,
# it is equivalent to the LIST command, with the data sent over the
# control connection.  Strange.  But wuftpd, ftpd, and nt's ftp server
# all support it.
#
##      def cmd_stat (self, line):
##              'return status of server'
##              pass

    def cmd_syst (self, line):
        'show operating system type of server system'
        # Replying to this command is of questionable utility, because
        # this server does not behave in a predictable way w.r.t. the
        # output of the LIST command.  We emulate Unix ls output, but
        # on win32 the pathname can contain drive information at the front
        # Currently, the combination of ensuring that os.sep == '/'
        # and removing the leading slash when necessary seems to work.
        # [cd'ing to another drive also works]
        #
        # This is how wuftpd responds, and is probably
        # the most expected.  The main purpose of this reply is so that
        # the client knows to expect Unix ls-style LIST output.
        self.respond ('215 UNIX Type: L8')
        # one disadvantage to this is that some client programs
        # assume they can pass args to /bin/ls.
        # a few typical responses:
        # 215 UNIX Type: L8 (wuftpd)
        # 215 Windows_NT version 3.51
        # 215 VMS MultiNet V3.3
        # 500 'SYST': command not understood. (SVR4)

    def cmd_help (self, line):
        'give help information'
        # find all the methods that match 'cmd_xxxx',
        # use their docstrings for the help response.
        attrs = dir(self.__class__)
        help_lines = []
        for attr in attrs:
            if attr[:4] == 'cmd_':
                x = getattr (self, attr)
                if type(x) == type(self.cmd_help):
                    if x.__doc__:
                        help_lines.append ('\t%s\t%s' % (attr[4:], x.__doc__))
        if help_lines:
            self.push ('214-The following commands are recognized\r\n')
            self.push_with_producer (lines_producer (help_lines))
            self.push ('214\r\n')
        else:
            self.push ('214-\r\n\tHelp Unavailable\r\n214\r\n')

class ftp_server (asyncore.dispatcher):
    # override this to spawn a different FTP channel class.
    ftp_channel_class = ftp_channel

    SERVER_IDENT = 'FTP Server (V%s)' % VERSION

    def __init__ (
            self,
            authorizer,
            hostname        =None,
            ip              ='',
            port            =21,
            logger_object=file_logger (sys.stdout)
            ):
        self.ip = ip
        self.port = port
        self.authorizer = authorizer

        if hostname is None:
            self.hostname = socket.gethostname()
        else:
            self.hostname = hostname

        # statistics
        self.total_sessions = counter()
        self.closed_sessions = counter()
        self.total_files_out = counter()
        self.total_files_in = counter()
        self.total_bytes_out = counter()
        self.total_bytes_in = counter()
        self.total_exceptions = counter()
        #
        asyncore.dispatcher.__init__ (self)
        self.create_socket (socket.AF_INET, socket.SOCK_STREAM)

        self.set_reuse_addr()
        self.bind ((self.ip, self.port))
        self.listen (5)

        if not logger_object:
            logger_object = sys.stdout

        self.logger = unresolving_logger (logger_object)

        self.log_info('FTP server started at %s\n\tAuthorizer:%s\n\tHostname: %s\n\tPort: %d' % (
                time.ctime(time.time()),
                repr (self.authorizer),
                self.hostname,
                self.port)
                )

    def writable (self):
        return 0

    def handle_read (self):
        pass

    def handle_connect (self):
        pass

    def handle_accept (self):
        conn, addr = self.accept()
        self.total_sessions.increment()
        self.log_info('Incoming connection from %s:%d' % (addr[0], addr[1]))
        self.ftp_channel_class (self, conn, addr)

    # return a producer describing the state of the server
    def status (self):

        def nice_bytes (n):
            return string.join (english_bytes (n))

        return lines_producer (
                ['<h2>%s</h2>'                          % self.SERVER_IDENT,
                 '<br>Listening on <b>Host:</b> %s' % self.hostname,
                 '<b>Port:</b> %d'                      % self.port,
                 '<br>Sessions',
                 '<b>Total:</b> %s'                     % self.total_sessions,
                 '<b>Current:</b> %d'           % (self.total_sessions.as_long() - self.closed_sessions.as_long()),
                 '<br>Files',
                 '<b>Sent:</b> %s'                      % self.total_files_out,
                 '<b>Received:</b> %s'          % self.total_files_in,
                 '<br>Bytes',
                 '<b>Sent:</b> %s'                      % nice_bytes (self.total_bytes_out.as_long()),
                 '<b>Received:</b> %s'          % nice_bytes (self.total_bytes_in.as_long()),
                 '<br>Exceptions: %s'           % self.total_exceptions,
                 ]
                )

# ======================================================================
#                                                Data Channel Classes
# ======================================================================

# This socket accepts a data connection, used when the server has been
# placed in passive mode.  Although the RFC implies that we ought to
# be able to use the same acceptor over and over again, this presents
# a problem: how do we shut it off, so that we are accepting
# connections only when we expect them?  [we can't]
#
# wuftpd, and probably all the other servers, solve this by allowing
# only one connection to hit this acceptor.  They then close it.  Any
# subsequent data-connection command will then try for the default
# port on the client side [which is of course never there].  So the
# 'always-send-PORT/PASV' behavior seems required.
#
# Another note: wuftpd will also be listening on the channel as soon
# as the PASV command is sent.  It does not wait for a data command
# first.

# --- we need to queue up a particular behavior:
#  1) xmit : queue up producer[s]
#  2) recv : the file object
#
# It would be nice if we could make both channels the same.  Hmmm..
#

class passive_acceptor (asyncore.dispatcher):
    ready = None

    def __init__ (self, control_channel):
        # connect_fun (conn, addr)
        asyncore.dispatcher.__init__ (self)
        self.control_channel = control_channel
        self.create_socket (socket.AF_INET, socket.SOCK_STREAM)
        # bind to an address on the interface that the
        # control connection is coming from.
        self.bind ((
                self.control_channel.getsockname()[0],
                0
                ))
        self.addr = self.getsockname()
        self.listen (1)

#       def __del__ (self):
#               print 'passive_acceptor.__del__()'

    def log (self, *ignore):
        pass

    def handle_accept (self):
        conn, addr = self.accept()
        dc = self.control_channel.client_dc
        if dc is not None:
            dc.set_socket (conn)
            dc.addr = addr
            dc.connected = 1
            self.control_channel.passive_acceptor = None
        else:
            self.ready = conn, addr
        self.close()


class xmit_channel (async_chat):

    # for an ethernet, you want this to be fairly large, in fact, it
    # _must_ be large for performance comparable to an ftpd.  [64k] we
    # ought to investigate automatically-sized buffers...

    ac_out_buffer_size = 16384
    bytes_out = 0

    def __init__ (self, channel, client_addr=None):
        self.channel = channel
        self.client_addr = client_addr
        async_chat.__init__ (self)

#       def __del__ (self):
#               print 'xmit_channel.__del__()'

    def log (self, *args):
        pass

    def readable (self):
        return not self.connected

    def writable (self):
        return 1

    def send (self, data):
        result = async_chat.send (self, data)
        self.bytes_out = self.bytes_out + result
        return result

    def handle_error (self):
        # usually this is to catch an unexpected disconnect.
        self.log_info ('unexpected disconnect on data xmit channel', 'error')
        try:
            self.close()
        except:
            pass

    # TODO: there's a better way to do this.  we need to be able to
    # put 'events' in the producer fifo.  to do this cleanly we need
    # to reposition the 'producer' fifo as an 'event' fifo.

    def close (self):
        c = self.channel
        s = c.server
        c.client_dc = None
        s.total_files_out.increment()
        s.total_bytes_out.increment (self.bytes_out)
        if not len(self.producer_fifo):
            c.respond ('226 Transfer complete')
        elif not c.closed:
            c.respond ('426 Connection closed; transfer aborted')
        del c
        del s
        del self.channel
        async_chat.close (self)

class recv_channel (asyncore.dispatcher):
    def __init__ (self, channel, client_addr, fd):
        self.channel = channel
        self.client_addr = client_addr
        self.fd = fd
        asyncore.dispatcher.__init__ (self)
        self.bytes_in = counter()

    def log (self, *ignore):
        pass

    def handle_connect (self):
        pass

    def writable (self):
        return 0

    def recv (*args):
        result = apply (asyncore.dispatcher.recv, args)
        self = args[0]
        self.bytes_in.increment(len(result))
        return result

    buffer_size = 8192

    def handle_read (self):
        block = self.recv (self.buffer_size)
        if block:
            try:
                self.fd.write (block)
            except IOError:
                self.log_info ('got exception writing block...', 'error')

    def handle_close (self):
        s = self.channel.server
        s.total_files_in.increment()
        s.total_bytes_in.increment(self.bytes_in.as_long())
        self.fd.close()
        self.channel.respond ('226 Transfer complete.')
        self.close()


import getopt
import re, sys
import asyncore
import os
import random
import imp
import time
import thread
import stat
import urllib
import traceback
import logging
import zipfile

HTTP_CONTINUE                     = 100
HTTP_SWITCHING_PROTOCOLS          = 101
HTTP_PROCESSING                   = 102
HTTP_OK                           = 200
HTTP_CREATED                      = 201
HTTP_ACCEPTED                     = 202
HTTP_NON_AUTHORITATIVE            = 203
HTTP_NO_CONTENT                   = 204
HTTP_RESET_CONTENT                = 205
HTTP_PARTIAL_CONTENT              = 206
HTTP_MULTI_STATUS                 = 207
HTTP_MULTIPLE_CHOICES             = 300
HTTP_MOVED_PERMANENTLY            = 301
HTTP_MOVED_TEMPORARILY            = 302
HTTP_SEE_OTHER                    = 303
HTTP_NOT_MODIFIED                 = 304
HTTP_USE_PROXY                    = 305
HTTP_TEMPORARY_REDIRECT           = 307
HTTP_BAD_REQUEST                  = 400
HTTP_UNAUTHORIZED                 = 401
HTTP_PAYMENT_REQUIRED             = 402
HTTP_FORBIDDEN                    = 403
HTTP_NOT_FOUND                    = 404
HTTP_METHOD_NOT_ALLOWED           = 405
HTTP_NOT_ACCEPTABLE               = 406
HTTP_PROXY_AUTHENTICATION_REQUIRED= 407
HTTP_REQUEST_TIME_OUT             = 408
HTTP_CONFLICT                     = 409
HTTP_GONE                         = 410
HTTP_LENGTH_REQUIRED              = 411
HTTP_PRECONDITION_FAILED          = 412
HTTP_REQUEST_ENTITY_TOO_LARGE     = 413
HTTP_REQUEST_URI_TOO_LARGE        = 414
HTTP_UNSUPPORTED_MEDIA_TYPE       = 415
HTTP_RANGE_NOT_SATISFIABLE        = 416
HTTP_EXPECTATION_FAILED           = 417
HTTP_UNPROCESSABLE_ENTITY         = 422
HTTP_LOCKED                       = 423
HTTP_FAILED_DEPENDENCY            = 424
HTTP_INTERNAL_SERVER_ERROR        = 500
HTTP_NOT_IMPLEMENTED              = 501
HTTP_BAD_GATEWAY                  = 502
HTTP_SERVICE_UNAVAILABLE          = 503
HTTP_GATEWAY_TIME_OUT             = 504
HTTP_VERSION_NOT_SUPPORTED        = 505
HTTP_VARIANT_ALSO_VARIES          = 506
HTTP_INSUFFICIENT_STORAGE         = 507
HTTP_NOT_EXTENDED                 = 510

GLOBAL_TEMP_DIR="/tmp/"
GLOBAL_ROOT_DIR="no-root-dir-set"
verbose = 0
multithreading_enabled = 0
number_of_threads = 32

def qualify_path(p):
    if p[-1] != '/':
        return p + "/"
    return p

def join_paths(p1,p2):
    if p1.endswith("/"):
        if p2.startswith("/"):
            return p1[:-1] + p2
        else:
            return p1 + p2
    else:
        if p2.startswith("/"):
            return p1 + p2
        else:
            return p1 + "/" + p2


translators = []
macroresolvers = []
ftphandlers = []
contexts = []

def getMacroFile(filename):
    global macrofile_callback
    for r in macroresolvers:
        try:
            f = r(filename)
            if f is not None and os.path.isfile(f):
                return f
        except: 
            pass
    if os.path.isfile(filename):
        return filename
    filename2 =  join_paths(GLOBAL_ROOT_DIR,filename)
    if os.path.isfile(filename2):
        return filename2
    raise IOError("No such file: "+filename2)
   

global_modules={}

def _make_inifiles(root, path):
    dirs = path.split("/")
    path = root
    for dir in dirs:
        path = join_paths(path, dir)
        inifile = join_paths(path, "__init__.py")
        # create missing __init__.py
        if not os.path.isfile(inifile):
            if lg:
                lg.log("creating file "+inifile)
            open(inifile, "wb").close()

def _load_module(filename):
    global global_modules
    b = BASENAME.match(filename)
    # filename e.g. /my/modules/test.py
    # b.group(1) = /my/modules/
    # b.group(2) = test.py
    if b is None:
        raise "Internal error with filename "+filename
    module = b.group(2)
    if module is None:
        raise "Internal error with filename "+filename
            
    while filename.startswith("./"):
        filename = filename[2:]

    if filename in global_modules:
        return global_modules[filename]

    dir = os.path.dirname(filename)
    path = dir.replace("/",".")

    _make_inifiles(GLOBAL_ROOT_DIR, dir)

    # strip tailing/leading dots
    while len(path) and path[0] == '.':
        path = path[1:]
    while len(path) and path[-1] != '.':
        path = path + "."
   
    module2 = (path + module)
    if lg:
        lg.log("Loading module "+module2)

    m = __import__(module2)
    try:
        i = module2.index(".")
        m = eval("m."+module2[i+1:])
        global_modules[filename] = m
    except:
        pass
    return m

system_modules = sys.modules.copy()
stdlib, x = os.path.split(os.__file__)
def _purge_all_modules():
    for m,mod in sys.modules.items():
        if m not in system_modules:
            if hasattr(mod, "__file__"):
                f = mod.__file__
                path, x = os.path.split(f)
                if not path.startswith(stdlib):
                    del sys.modules[m]

class WebContext:
    def __init__(self, name, root=None):
        self.name = name
        self.files = []
        self.startupfile = None
        if root:
            self.root = qualify_path(root)
        self.pattern_to_function = {}
        self.id_to_function = {}

    def addFile(self, filename):
        file = WebFile(self, filename)
        self.files += [file]
        return file

    def setRoot(self, root):
        self.root = qualify_path(root)
        while self.root.startswith("./"):
            self.root = self.root[2:]

    def setStartupFile(self, startupfile):
        self.startupfile = startupfile
        lg.log("  executing startupfile")
        self._load_module(self.startupfile)

    def getStartupFile(self):
        return self.startupfile

    def match(self, path):
        function = None
        for pattern,call in self.pattern_to_function.items():
            if pattern.match(path):
                function,desc = call
                if verbose:
                    lg.log("Request %s matches (%s)" % (req.path, desc))
        if function is None:
            for id,call in self.id_to_function.items():
                if path == id:
                    function,desc = call
                    if verbose:
                        lg.log("Request %s matches handler (%s)" % (req.path, desc))
        if not function:
            return None
        def call_and_close(f,req):
            status = f(req)
            if status is not None and type(1)==type(status) and status>10:
                req.reply_code = status
                if status == 404:
                    return req.error(status, "not found")
                elif(status >= 400 and status <= 500):
                    return req.error(status)
            return req.done()
        return lambda req: call_and_close(function,req)

class FileStore:
    def __init__(self, name, root=None):
        self.name = name
        self.handlers = []
        if type(root) == type(""):
            self.addRoot(root)
        elif type(root) == type([]):
            for dir in root:
                self.addRoot(dir)

    def match(self, path):
        return lambda req: self.findfile(req)

    def findfile(self, request):
        for handler in self.handlers:
            if handler.can_handle(request):
                return handler.handle_request(request)
        return request.error(404, "File "+request.path+" not found")

    def addRoot(self, dir):
        dir = qualify_path(dir)
        while dir.startswith("./"):
            dir = dir[2:]
        if zipfile.is_zipfile(GLOBAL_ROOT_DIR + dir[:-1]) and dir.lower().endswith("zip/"):
            self.handlers += [default_handler (zip_filesystem (GLOBAL_ROOT_DIR + dir[:-1]))]
        else:
            self.handlers += [default_handler (os_filesystem (GLOBAL_ROOT_DIR + dir))]

class WebFile:
    def __init__(self, context, filename):
        self.context = context
        if filename[0] == '/':
            filename = filename[1:]
        self.filename = filename
        self.m = _load_module(filename)
        self.handlers = []

    def addHandler(self, function):
        handler = WebHandler(self, function)
        self.handlers += [handler]
        return handler

    def addFTPHandler(self, ftpclass):
        global ftphandlers
        m = self.m
        try:
            c = eval("m."+ftpclass)
            if c is None:
                raise
            ftphandlers += [c]
        except:
            lgerr.log("Error in FTP Handler:" + str(sys.exc_info()[0]) + " " + str(sys.exc_info()[1]))
            traceback.print_tb(sys.exc_info()[2],None,lgerr)
            raise "No such function "+ftpclass+" in file "+self.filename

    def addMacroResolver(self, macroresolver):
        global macroresolvers
        m = self.m
        try:
            f = eval("m."+macroresolver)
            if f is None: 
                raise
            macroresolvers += [f]
        except:
            lgerr.log("Error in Macro Resolver:" + str(sys.exc_info()[0]) + " " + str(sys.exc_info()[1]))
            traceback.print_tb(sys.exc_info()[2],None,lgerr)
            raise "No such function "+macroresolver+" in file "+self.filename

    def addTranslator(self, handler):
        global translators
        m = self.m
        try:
            f = eval("m."+translator)
            if f is None: 
                raise
            translators += [f]
        except:
            lgerr.log("Error in Macro Resolver:" + str(sys.exc_info()[0]) + " " + str(sys.exc_info()[1]))
            traceback.print_tb(sys.exc_info()[2],None,lgerr)
            raise "No such function "+translator+" in file "+self.filename

    def getFileName(self):
        return self.context.root + self.filename

class WebHandler:
    def __init__(self, file, function):
        self.file = file
        self.function = function
        m = file.m
        try:
            self.f = eval("m."+function)
            if self.f is None: 
                raise
        except:
            lgerr.log("Error in Handler:" + str(sys.exc_info()[0]) + " " + str(sys.exc_info()[1]))
            traceback.print_tb(sys.exc_info()[2],None,lgerr)
            raise "No such function "+function+" in file "+self.file.filename

    def addPattern(self, pattern):
        p = WebPattern(self,pattern)
        desc = "pattern %s, file %s, function %s" % (pattern,self.file.filename,self.function)
        desc2 = "file %s, function %s" % (self.file.filename,self.function)
        self.file.context.pattern_to_function[p.getPattern()] = (self.f,desc)
        self.file.context.id_to_function["/"+self.function] = (self.f,desc2)
        return p

class WebPattern:
    def __init__(self, handler, pattern):
        self.handler = handler 
        self.pattern = pattern 
        if not pattern.endswith('$'):
            pattern = pattern + "$"
        self.compiled = re.compile(pattern)
    def getPattern(self):
        return self.compiled
    def getPatternString(self):
        return self.pattern

def read_ini_file(filename):
    global GLOBAL_TEMP_DIR,GLOBAL_ROOT_DIR,number_of_threads,multithreading_enabled,contexts
    lineno = 0
    fi = open(filename, "rb")
    file = None
    function = None
    context = None
    GLOBAL_ROOT_DIR = '/'
    for line in fi.readlines():
        lineno=lineno+1
        hashpos = line.find("#")
        if hashpos>=0:
            line = line[0:hashpos]
        line = line.strip()
        
        if line == "":
            continue #skip empty line

        equals = line.find(":")
        if equals<0:
            continue
        key = line[0:equals].strip()
        value = line[equals+1:].strip()
        if key == "tempdir":
            GLOBAL_TEMP_DIR = qualify_path(value)
        elif key == "threads":
            number_of_threads = int(value)
            multithreading_enabled = 1
        elif key == "base":
            GLOBAL_ROOT_DIR = qualify_path(value)
            sys.path += [GLOBAL_ROOT_DIR]
        elif key == "filestore":
            if len(value) and value[0] != '/':
                value = "/" + value 
            filestore = FileStore(value)
            contexts += [filestore]
            context = None
        elif key == "context":
            if len(value) and value[0] != '/':
                value = "/" + value 
            contextname = value
            context = WebContext(contextname)
            contexts += [context]
            filestore = None
        elif key == "startupfile":
            if context is not None:
                context.setStartupFile(value)
            else:
                raise "Error: startupfile must be below a context"
        elif key == "root":
            if value.startswith('/'):
                value = value[1:]
            if context:
                context.setRoot(value)
            if filestore:
                filestore.addRoot(value)
        elif key == "file":
            filename = value
            context.addFile(filename)
        elif key == "ftphandler":
            file.addFTPHandler(value)
        elif key == "handler":
            function = value
            file.addHandler(function)
        elif key == "macroresolver":
            file.addMacroResolver(value)
        elif key == "translator":
            file.addTranslator(value)
        elif key == "pattern":
            handler.addPattern(value)
        else:
            raise "Syntax error in line "+str(lineno)+" of file "+filename+":\n"+line
    fi.close()

def headers_to_map(mylist):
    headers={}
    for h in mylist:
        try:
            i = h.index(':')
        except:
            i = -1
        if i >= 0:
            key = h[0:i].lower()
            value = h[i+1:]
            if len(value)>0 and value[0] == ' ':
                value = value[1:]
            headers[key] = value
        else:
            if len(h.strip())>0:
                lg.log("invalid header: "+str(h))
    return headers
                    
class AthanaFile:
    def __init__(self,fieldname, parammap,filename,content_type):
        self.fieldname = fieldname
        self.parammap = parammap
        self.filename = filename
        self.content_type = content_type
        self.tempname = GLOBAL_TEMP_DIR+str(int(random.random()*999999))+os.path.splitext(filename)[1]
        self.filesize = 0
        self.fi = open(self.tempname, "wb")
    def adddata(self,data):
        self.filesize += len(data)
        self.fi.write(data)
    def close(self):
        self.fi.close()
        # only append file to parameters if it contains some data
        if self.filename or self.filesize:
            self.parammap[self.fieldname] = self
        del self.fieldname
        del self.parammap
        del self.fi
    def __str__(self):
        return "file %s (%s), %d bytes, content-type: %s" % (self.filename, self.tempname, self.filesize, self.content_type)

class AthanaField:
    def __init__(self,fieldname,parammap):
        self.fieldname = fieldname
        self.data = ""
        self.parammap = parammap
    def adddata(self,data):
        self.data += data
    def close(self):
        try:
            oldvalue = self.parammap[self.fieldname] + ";"
        except KeyError:
            oldvalue = ""
        self.parammap[self.fieldname] = oldvalue + self.data
        del self.data
        del self.parammap

class simple_input_collector:
    def __init__ (self, handler, request, length):
        self.request = request
        self.length = length
        self.handler = handler
        request.channel.set_terminator(length)
        self.data = ""

    def collect_incoming_data (self, data):
        self.data += data

    def found_terminator(self):
        self.request.channel.set_terminator('\r\n\r\n')
        self.request.collector = None
        d=self.data;del self.data
        r=self.request;del self.request
        parameters={}
        data = d.split('&')
        for e in data:
            if '=' in e:
                key,value = e.split('=')
                key = urllib.unquote_plus(key)
                try:
                    oldvalue = parameters[key]+";"
                except KeyError:
                    oldvalue = ""
                parameters[key] = oldvalue + urllib.unquote_plus(value)
            else:
                if len(e.strip())>0:
                    lg.log("Unknown parameter: "+e)
        self.handler.continue_request(r,parameters)

class upload_input_collector:
    def __init__ (self, handler, request, length, boundary):
        self.request = request
        self.length = length
        self.handler = handler
        self.boundary = boundary
        request.channel.set_terminator(length)
        self.data = ""
        self.pos = 0
        self.start_marker = "--"+boundary+"\r\n"
        self.end_marker = "--"+boundary+"--\r\n"
        self.prefix = "--"+boundary
        self.marker = "\r\n--"+boundary
        self.header_end_marker = "\r\n\r\n"
        self.current_file = None
        self.boundary = boundary
        self.file = None
        self.parameters = {}
        self.files = []

    def parse_semicolon_parameters(self,params):
        params = params.split("; ")
        parmap = {}
        for a in params:
            if '=' in a:
                key,value = a.split('=')
                if value.startswith('"') and value.endswith('"'):
                    value = value[1:-1]
                parmap[key] = value
        return parmap

    def startFile(self,headers):
        fieldname = None
        filename = None
        if self.file is not None:
            raise "Illegal state"
        if "content-disposition" in headers:
            cd = headers["content-disposition"]
            l = self.parse_semicolon_parameters(cd)
            if "name" in l:
                fieldname = l["name"]
            if "filename" in l:
                filename = l["filename"]
        if "content-type" in headers:
            content_type = headers["content-type"]
            self.file = AthanaFile(fieldname,self.parameters,filename,content_type)
            self.files += [self.file]
        else:
            self.file = AthanaField(fieldname,self.parameters)

    def split_headers(self,string):
        return string.split("\r\n")

    def collect_incoming_data (self, newdata):
        self.pos += len(newdata)
        self.data += newdata
       
        while len(self.data)>0:
            if self.data.startswith(self.end_marker):
                self.data = self.data[len(self.end_marker):]
                if self.file is not None:
                    self.file.close()
                    self.file = None
                return
            elif self.data.startswith(self.start_marker):
                try:
                    i = self.data.index(self.header_end_marker, len(self.start_marker))
                except:
                    i = -1
                if i>=0:
                    headerstr = self.data[len(self.start_marker):i+2]
                    headers = headers_to_map(self.split_headers(headerstr))
                    self.startFile(headers)
                    self.data = self.data[i+len(self.header_end_marker):]
                else:
                    return # wait for more data (inside headers)
            elif self.data.startswith(self.prefix):
                return
            else:
                try:
                    bindex = self.data.index(self.marker)
                    self.file.adddata(self.data[0:bindex])
                    self.file.close()
                    self.file = None
                    self.data = self.data[bindex+2:] # cut to position after \r\n
                except ValueError: #not found
                    if(len(self.data) <= len(self.marker)):
                        return #wait for more data before we make a decision or pass through data
                    else:
                        self.file.adddata(self.data[0:-len(self.marker)])
                        self.data = self.data[-len(self.marker):]

    def found_terminator(self):
        if len(self.data)>0:# and self.file is not None:
            if self.file is not None:
                self.file.close()
                self.file = None
            raise "Unfinished/malformed multipart request"
        if self.file is not None:
            self.file.close()
            self.file = None
            
        self.request.collector = None
        self.request.channel.set_terminator('\r\n\r\n')
        d=self.data;del self.data
        r=self.request;del self.request
        r.tempfiles = [f.tempname for f in self.files]
        self.handler.continue_request(r,self.parameters)

class Session(dict):
    def __init__(self, id):
        self.id = id
    def use(self):
        self.lastuse = time.time()

def exception_string():
    s = "Exception "+str(sys.exc_info()[0])
    info = sys.exc_info()[1]
    if info:
        s += " "+str(info)
    s += "\n"
    for l in traceback.extract_tb(sys.exc_info()[2]):
        s += "  File \"%s\", line %d, in %s\n" % (l[0],l[1],l[2])
    s += "    %s\n" % l[3]
    return s

BASENAME = re.compile("([^/]*/)*([^/.]*)(.py)?")
MULTIPART = re.compile ('multipart/form-data.*boundary=([^ ]*)', re.IGNORECASE)
SESSION_PATTERN = re.compile("^;[a-z0-9]{6}-[a-z0-9]{6}-[a-z0-9]{6}$")

use_cookies = 1

class AthanaHandler:
    def __init__(self):
        self.sessions = {}
        self.queue = []
        self.queuelock = thread.allocate_lock()

    def match(self, request):
        path, params, query, fragment = request.split_uri()
        #lg.log("===== request:"+path+"=====")
        return 1
    
    def handle_request (self, request):
        headers = headers_to_map(request.header)
        request.request_headers = headers

        size=headers.get("content-length",None)

        if size and size != '0':
            size=int(size)
            ctype=headers.get("content-type",None)
            b = MULTIPART.match(ctype)
            if b is not None:
                request.type = "MULTIPART"
                boundary = b.group(1)
                request.collector = upload_input_collector(self,request,size,boundary)
            else:
                request.type = "POST"
                request.collector = simple_input_collector(self,request,size)
        else:
            request.type = "GET"
            self.continue_request(request, {})

    def create_session_id(self):
        pid = abs((str(random.random())).__hash__())
        now = abs((str(time.time())).__hash__())
        rand = abs((str(random.random())).__hash__())
        x = "abcdefghijklmnopqrstuvwxyz0123456789"
        result = ""
        for a in range(0,6):
            result += x[pid%36]
            pid = pid / 36
        result += "-"
        for a in range(0,6):
            result += x[now%36]
            now = now / 36
        result += "-"
        for a in range(0,6):
            result += x[rand%36]
            rand = rand / 36
        return result

    def continue_request(self, request, parameters):

        path, params, query, fragment = request.split_uri()
        
        ip = request.request_headers.get("x-forwarded-for",None)
        if ip is None:
            try: ip = request.channel.addr[0]
            except: pass
        if ip:
            request.channel.addr = (ip,request.channel.addr[1])

        request.log()

        if query is not None:
            if query[0] == '?':
                query=query[1:]
            query = query.split('&')
            for e in query:
                key,value = e.split('=')
                key = urllib.unquote_plus(key)
                try:
                    oldvalue = parameters[key]+";"
                except KeyError:
                    oldvalue = ""
                parameters[key] = oldvalue + urllib.unquote_plus(value) #_plus?

        cookies = {}
        if "cookie" in request.request_headers:
            cookiestr = request.request_headers["cookie"]
            if cookiestr.rfind(";") == len(cookiestr)-1:
                cookiestr = cookiestr[:-1]
            items = cookiestr.split(';')
            for a in items:
                key,value = a.strip().split('=')
                cookies[key] = value

        request.Cookies = cookies

        sessionid = None
        if params is not None and SESSION_PATTERN.match(params):
            sessionid = params
            if sessionid[0] == ';':
                sessionid = sessionid[1:]
        elif use_cookies and "PSESSION" in cookies:
            sessionid = cookies["PSESSION"]

        if sessionid is not None:
            if sessionid in self.sessions:
                session = self.sessions[sessionid]
                session.use()
            else:
                session = Session(sessionid)
                self.sessions[sessionid] = session
        else:
            sessionid = self.create_session_id()
            session = Session(sessionid)
            self.sessions[sessionid] = session


        request['Connection'] = 'close';
        request['Content-Type'] = 'text/html; encoding=utf-8; charset=utf-8';

        maxlen = -1
        context = None
        global contexts
        for c in contexts:
            #lg.debug("Compare context "+c.name+" with request "+path)
            if path.startswith(c.name) and len(c.name)>maxlen:
                context = c
                maxlen = len(context.name)
        if context is None:
            request.error (404)
            return

        #print "Request ",'"'+path+'"',"maps to context",context.name
        fullpath = path
        path = path[len(context.name):]
        if len(path)==0 or path[0] != '/':
            path = "/" + path

        request.session = session
        request.sessionid = sessionid
        request.context = context
        request.path = path
        request.fullpath = fullpath
        request.paramstring = params
        request.query = query
        request.fragment = fragment
        request.params = parameters
        request.request = request
        request.ip = ip
        request.uri = request.uri.replace(context.name, "/")
        request._split_uri = None

        if use_cookies:
            request.setCookie('PSESSION', sessionid, time.time()+3600*2)

        request.channel.current_request = None

        function = context.match(path)
       
        if function is not None:
            if not multithreading_enabled:
                self.callhandler(function, request)
            else:
                self.queuelock.acquire()
                self.queue += [(function,request)]
                self.queuelock.release()
            return
        else:
            lg.log("Request %s matches no pattern (context: %s)" % (request.path,context.name))
            return request.error(404, "File %s not found" % request.path)
        
    def callhandler(self, function, req):
        request = req.request
        s = None
        try:
            status = function(req)
        except:
            lgerr.log("Error in page :" + str(sys.exc_info()[0]) + " " + str(sys.exc_info()[1]))
            traceback.print_tb(sys.exc_info()[2],None,lgerr)
            s = "<pre>"+exception_string()+"</pre>"
            return request.error(500,s)

def worker_thread(server):
    while 1:
        server.queuelock.acquire()
        if len(server.queue) == 0:
            server.queuelock.release()
            time.sleep(0.01)
        else:
            function,req = server.queue.pop()
            server.queuelock.release()
            try:
                server.callhandler(function,req)
            except:
                lgerr.log("Error while processing request:" + str(sys.exc_info()[0]) + " " + str(sys.exc_info()[1]))
                traceback.print_tb(sys.exc_info()[2],None,lgerr)

class fs:
    pass

class virtual_authorizer:
    def __init__ (self):
        pass
    def authorize (self, channel, username, password):
        channel.persona = -1, -1
        channel.read_only = 1
        #return 1, 'Ok.', fs()
        return 1, 'Ok.', os_filesystem("/home/kramm")

    def __repr__(self):
        return 'virtual'

class logging_logger:
    def __init__(self,name="athana"):
        self.logger = logging.getLogger(name)
    def log (self, message):
        self.logger.info(message.rstrip())
    def debug (self, message):
        self.logger.debug(message.rstrip())
    def write (self, message):
        self.logger.info(message.rstrip())
    def error (self, message):
        self.logger.error(message.rstrip())

lg = logging_logger()
lgerr = logging_logger("errors")

class zip_filesystem:
    def __init__(self, filename):
        self.filename = filename
        self.wd = '/'
        self.m = {}
        self.z = zipfile.ZipFile(filename)
        self.lock = thread.allocate_lock()
        for f in self.z.filelist:
            self.m['/' + f.filename] = f

    def current_directory(self):
        return self.wd

    def isfile(self, path):
        if len(path) and path[-1]=='/':
            return 0
        return (self.wd + path) in self.m

    def isdir (self, path):
        if not (len(path) and path[-1]=='/'):
            path += '/'
        return path in self.m

    def cwd (self, path):
        path = join_paths(self.wd, path)
        if not self.isdir (path):
            return 0
        else:
            self.wd = path
            return 1

    def cdup (self):
        try:
            i = self.wd[:-1].rindex('/')
            self.wd = self.wd[0:i+1]
        except ValueError:
            self.wd = '/'
        return 1

    def listdir (self, path, long=0):
        raise "Not implemented"

    # TODO: implement a cache w/timeout for stat()
    def stat (self, path):
        fullpath = join_paths(self.wd, path)
        if self.isfile(path):
            size = self.m[fullpath].file_size
            return (33188, 77396L, 10L, 1, 1000, 1000, size, 0,0,0)
        elif self.isdir(path):
            return (16895, 117481L, 10L, 20, 1000, 1000, 4096L, 0,0,0)
        else:
            raise "No such file or directory "+path

    def open (self, path, mode):
        class zFile:
            def __init__(self, content):
                self.content = content
                self.pos = 0
                self.len = len(content)
            def read(self,l=None):
                if l is None:
                    l = self.len - self.pos
                if self.len < self.pos + l:
                    l = self.len - self.pos
                s = self.content[self.pos : self.pos + l]
                self.pos += l
                return s
            def close(self):
                del self.content
                del self.len
                del self.pos
        self.lock.acquire()
        try:
            data = self.z.read(path)
        finally:
            self.lock.release()
        return zFile(data)

    def unlink (self, path):
        raise "Not implemented"
    def mkdir (self, path):
        raise "Not implemented"
    def rmdir (self, path):
        raise "Not implemented"

    def longify (self, (path, stat_info)):
        return unix_longify (path, stat_info)

    def __repr__ (self):
        return '<zipfile fs root:%s wd:%s>' % (self.filename, self.wd)


def setBase(base):
    global GLOBAL_ROOT_DIR
    GLOBAL_ROOT_DIR = qualify_path(base)
   
def setTempDir(tempdir):
    global GLOBAL_TEMP_DIR
    GLOBAL_TEMP_DIR = qualify_path(tempdir)
    
def addMacroResolver(m):
    global macroresolvers
    macroresolvers += [m]

def addTranslator(m):
    global translators
    translators += [m]

def addFTPHandler(m):
    global ftphandlers
    ftphandlers += [m]

def addContext(webpath, localpath):
    global contexts
    c = WebContext(webpath, localpath)
    contexts += [c]
    return c

def flush():
    global contexts,translators,ftphandlers,macroresolvers,global_modules
    contexts[:] = []
    translators[:] = []
    ftphandlers[:] = []
    macroresolvers[:] = []
    global_modules.clear()
    _purge_all_modules()

def addFileStore(webpath, localpaths):
    global contexts
    if len(webpath) and webpath[0] != '/':
        webpath = "/" + webpath 
    c = FileStore(webpath, localpaths)
    contexts += [c]
    return c

def setThreads(number):
    global number_of_threads
    global multithreading_enabled
    if number>1:
        multithreading_enabled=1
        number_of_threads=number
    else:
        multithreading_enabled=0
        number_of_threads=1
        
def run(port=8081):
    check_date()
    ph = AthanaHandler()
    hs = http_server ('', port, logger_object = lg)
    hs.install_handler (ph)

    if len(ftphandlers) > 0:
        ftp = ftp_server (virtual_authorizer(), port=8021, logger_object=lg)

    if multithreading_enabled: 
        threadlist = []
        for i in range(number_of_threads):
            threadlist += [thread.start_new_thread(worker_thread, (ph,))]

    while 1:
        try:
            asyncore.loop(timeout=0.01)
        except select.error:
            continue

"""
TODO: 
    * session clearup
    * temp directory in .cfg file
"""

def setTempDir(path):
    global GLOBAL_TEMP_DIR
    GLOBAL_TEMP_DIR = path

def mainfunction():
    global verbose,port,init_file,log_file,temp_path,multithreading_enabled,number_of_threads,GLOBAL_TEMP_DIR,contexts,lg,lgerr
    os.putenv('ATHANA_VERSION',ATHANA_VERSION)

    from optparse import OptionParser

    parser = OptionParser()

    parser.add_option("-v", "--verbose", dest="verbose", help="Be more verbose", action="store_true")
    parser.add_option("-q", "--quiet", dest="quiet", help="Be quiet", action="store_true")
    parser.add_option("-d", "--debug", dest="debug", help="Turn on debugging", action="store_true")
    parser.add_option("-p", "--port", dest="port", help="Set the port number", action="store",type="string")
    parser.add_option("-i", "--init-file", dest="init", help="Set the init file to use",action="store",type="string")
    parser.add_option("-l", "--log-file", dest="log", help="Set the logging file to use",action="store",type="string")
    parser.add_option("-t", "--temp-path", dest="temp", help="Set the temporary directory (default: /tmp/)",action="store",type="string")
    parser.add_option("-m", "--multithread", dest="multithreading_enabled", help="Enable multithreading",action="store_true")
    parser.add_option("-n", "--number-of-threads", dest="threads", help="Number of threads",action="store",type="int")
    parser.add_option("-T", "--talfile", dest="talfile", help="execute TAL File",action="store",type="string")

    (options, args) = parser.parse_args()

    verbose = 0
    init_file="web.cfg"
    log_file=None
    temp_path="/tmp/"
    port=8081
    
    if options.verbose != None : verbose = 2
    if options.quiet != None : verbose = 0
    if options.debug != None : verbose = 3
    if options.port != None : port = int(options.port)
    if options.init != None : init_file = options.init
    if options.log != None : log_file = options.log
    if options.temp != None : GLOBAL_TEMP_DIR = options.temp
    if options.multithreading_enabled : multithreading_enabled = 1
    if options.threads != None : number_of_threads = options.threads

    if options.talfile:
        print getTAL(options.talfile, {"mynone":None})
        sys.exit(0)

    if inifile:
        contexts += read_ini_file(inifile)
    
    if logfile is not None:
        fi = open(logfile, "wb")
        lg = file_logger (fi)
        lgerr = lg

    print "-"*72
    if multithreading_enabled:
        print "Starting Athana (%d threads)..." % number_of_threads
    else:
        print "Starting Athana..."
    print "Init-File:",init_file
    print "Log-File:",log_file
    print "Temp-Path:",GLOBAL_TEMP_DIR
    print "-"*72

    run(port)

if __name__ == '__main__':
    import athana
    athana.mainfunction()
