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
    #fo = open(tempfile, "wb")
    fo= os.tmpfile()
    p = subprocess.Popen([cmd] + args, executable=cmd, stdout=fo, stderr=fo)
    ret = -1
    for i in range(wait*10):
        ret = p.poll()
        if ret is not None:
            break
        time.sleep(0.1)
    else:
        os.kill(p.pid, 9)
        os.system("killall -9 "+cmd)
  
    fo.seek(0)
    output = fo.read()
    fo.close()
    return ret,output

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
            self.compile_error = 1
        if not os.path.isfile("abc.swf"):
            self.compile_error = 1

    def run(self):
        ret,output = runcmd("flashplayer",["abc.swf"],wait=1)
        os.system("killall flashplayer")
        self.flash_output = output
        
        if not check(self.flash_output):
            self.flash_error = 1

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
        print "compile:", (test.compile_error and "error" or "ok")
        print test.compile_output
        if not self.dorun:
            return
        print "================================"
        print "run:", (test.flash_error and "error" or "ok")
        print test.flash_output
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
    def __init__(self, nr, file):
        TestBase.__init__(self, nr, file, run=1)
        self.compile()
        if not self.compile_error:
            self.run()

class ErrTest(TestBase):
    def __init__(self, nr, file):
        TestBase.__init__(self, nr, file, run=0)
        self.compile()
        self.compile_error = not self.compile_error

class Suite:
    def __init__(self, dir):
        self.dir = dir
        self.errtest = "err" in dir
    def run(self, nr):
        print "-"*40,"tests \""+self.dir+"\"","-"*40
        for file in sorted(os.listdir(self.dir)):
            if not file.endswith(".as"):
                continue
            nr = nr + 1
            file = os.path.join(self.dir, file)
            if checknum>=0 and nr!=checknum:
                continue
            if self.errtest:
                test = ErrTest(nr,file)
            else:
                test = Test(nr,file)
            if checknum!=nr:
                test.doprint()
            else:
                test.doprintlong()
        return nr

checknum=-1
if len(sys.argv)>1:
    checknum = int(sys.argv[1])

nr = 0
nr = Suite("err").run(nr)
nr = Suite("ok").run(nr)

