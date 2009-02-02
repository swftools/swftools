#!/usr/bin/python
#
# runtests.py
#
# Run compiler unit tests
#
# Copyright (c) 2008 Matthias Kramm <kramm@quiss.org>
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

import sys
import os
import time
import subprocess
import marshal
import select
from optparse import OptionParser

def check(s):
    row = None
    ok = 0
    for line in s.split("\n"):
        if line.startswith("[") and line.endswith("]"):
            continue
        if not line.strip():
            continue
        if not line.startswith("ok"):
            return 0
        if line.startswith("ok "):
            if "/" not in line:
                return 0
            i = line.index('/')
            nr,len = int(line[3:i]),int(line[i+1:])
            if nr<1 or nr>len:
                return 0
            if not row:
                row = [0]*len
            if row[nr-1]:
                return 0
            row[nr-1] = 1
        elif line == "ok":
            ok = 1
    if ok:
        return not row
    if row:
        return 0 not in row
    return 0

def runcmd(cmd,args,wait):
    #fo = os.tmpfile()
    fi,fo = os.pipe()
    fo = os.fdopen(fo, "wb")
    p = subprocess.Popen([cmd] + args, executable=cmd, stdout=fo, stderr=fo)
    ret = -1
    output = ""
    for i in range(wait*10):
        if fi in select.select([fi],[],[], 0.01)[0]:
            output += os.read(fi, 8192)
            if "[exit]" in output:
                break
        ret = p.poll()
        if ret is not None:
            break
        time.sleep(0.1)
    else:
        os.kill(p.pid, 9)
        os.system("killall -9 %s >/dev/null 2>/dev/null" % cmd)
    fo.close()
   
    if fi in select.select([fi],[],[], 0.01)[0]:
        output += os.read(fi, 8192)

    os.close(fi)
    return ret,output

class Cache:
    def __init__(self, filename):
        try:
            self.filename2status = marshal.load(open(filename, "rb"))
        except IOError:
            self.filename2status = {}

    def parse_args(self):
        parser = OptionParser()
        parser.add_option("-d", "--diff", dest="diff", help="Only run tests that failed the last time",action="store_true")
        (options, args) = parser.parse_args()
        self.__dict__.update(options.__dict__)

        self.checknum=-1
        if len(args):
            self.checknum = int(args[0])

    @staticmethod
    def load(filename):
        return Cache(filename)

    def save(self, filename):
        fi = open(filename, "wb")
        marshal.dump(self.filename2status, fi)
        fi.close()

    def highlight(self, nr, filename):
        return self.checknum==nr

    def skip_file(self, nr, filename):
        if self.checknum>=0 and nr!=self.checknum:
            return 1
        if self.diff and self.filename2status[filename]=="ok":
            return 1
        return 0

    def file_status(self, filename, status):
        self.filename2status[filename] = status

class TestBase:
    def __init__(self, nr, file, run):
        self.nr = nr
        self.dorun = run
        self.file = file
        self.flash_output = None
        self.flash_error = None
        self.compile_output = None
        self.compile_error = None

    def compile(self):
        try: os.unlink("abc.swf");
        except: pass
        ret,output = runcmd("./parser",[self.file],wait=60)
        self.compile_error = 0
        self.compile_output = output
        if ret:
            self.compile_output += "\nExit status %d" % (-ret)
            self.compile_error = 1
            return 0
        if not os.path.isfile("abc.swf"):
            self.compile_error = 1
            return 0
        return 1

    def run(self):
        ret,output = runcmd("flashplayer",["abc.swf"],wait=1)
        os.system("killall flashplayer")
        self.flash_output = output
        
        if not check(self.flash_output):
            self.flash_error = 1
            return 0
        return 1

    def doprint(self):
        print self.r(str(self.nr),3)," ",
        if self.compile_error:
            if self.dorun:
                print "err"," - ",
            else:
                print "err","   ",
        else:
            print "ok ",
            if self.dorun:
                if not self.flash_error:
                    print "ok ",
                else:
                    print "err",
            else:
                print "   ",
        print " ",
        print self.file

    def doprintlong(self):
        print self.nr, self.file
        print "================================"
        print "compile:", (self.compile_error and "error" or "ok")
        print self.compile_output
        if not self.dorun:
            return
        print "================================"
        print "run:", (self.flash_error and "error" or "ok")
        print self.flash_output
        print "================================"

    def r(self,s,l):
        if(len(s)>=l):
            return s
        return (" "*(l-len(s))) + s
    def l(self,s,l):
        if(len(s)>=l):
            return s
        return s + (" "*(l-len(s)))

class Test(TestBase):
    def __init__(self, cache, nr, file):
        TestBase.__init__(self, nr, file, run=1)
        if self.compile() and self.run():
            cache.file_status(file, "ok")
        else:
            cache.file_status(file, "error")

class ErrTest(TestBase):
    def __init__(self, cache, nr, file):
        TestBase.__init__(self, nr, file, run=0)
        if self.compile():
            cache.file_status(file, "error")
            self.compile_error = True
        else:
            cache.file_status(file, "ok")
            self.compile_error = False

class Suite:
    def __init__(self, cache, dir):
        self.dir = dir
        self.cache = cache
        self.errtest = "err" in dir
    def run(self, nr):
        print "-"*40,"tests \""+self.dir+"\"","-"*40
        for file in sorted(os.listdir(self.dir)):
            if not file.endswith(".as"):
                continue
            nr = nr + 1
            file = os.path.join(self.dir, file)

            if cache.skip_file(nr, file):
                continue

            if self.errtest:
                test = ErrTest(cache, nr, file)
            else:
                test = Test(cache, nr, file)

            if not cache.highlight(nr, file):
                test.doprint()
            else:
                test.doprintlong()
        return nr

cache = Cache.load(".tests.cache")
cache.parse_args()

nr = 0
nr = Suite(cache, "err").run(nr)
nr = Suite(cache, "ok").run(nr)

cache.save(".tests.cache")
