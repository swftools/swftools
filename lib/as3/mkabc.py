#!/usr/bin/python

import re

fi = open("abc.c", "rb")
fo = open("abc_ops.c", "wb")

R = re.compile('{(0x..),\s*"([^"]*)"\s*,\s*"([^"]*)"\s*}\s*,\s*')

for line in fi.readlines():
    line = line.strip()
    m = R.match(line)
    if m:
        op,name,params = m.group(1),m.group(2),m.group(3)
        params = params.strip()
        paramstr = ""
        seen = {}
        names = []

        for c in params:
            paramstr += ", "
            if c == "2":
                type,pname="char*","name"
            elif c == "s":
                type,pname="char*","s"
            elif c in "nubs":
                type,pname="int","v"
            elif c == "m":
                type,pname="abc_method_body_t*","m"
            elif c == "i":
                type,pname="abc_method_t*","m"
            elif c == "c":
                type,pname="abc_class_t*","m"
            elif c == "j":
                type,pname="abc_label_t*","j"
            elif c == "S":
                type,pname="void*","labels"
            else:
                raise "Unknown type "+c
            paramstr += type
            paramstr += " "
            if pname in seen:
                seen[pname]+=1
                pname += str(seen[pname])
            else:
                seen[pname]=1
            paramstr += pname
            names += [pname]

        fo.write("abc_code_t* abc_%s(abc_code_t*prev%s)\n" % (name, paramstr))
        fo.write("{\n")
        fo.write("    abc_code_t*self = add_opcode(prev, %s);\n" % op)
        i = 0
        for pname,c in zip(names,params):
            if(c == "2"):
                fo.write("    self->params[%d] = %s;\n" % (i,pname));
            elif(c in "nu"):
                fo.write("    self->params[%d] = (void*)(ptroff_t)%s;\n" % (i,pname))
            elif(c == "b"):
                fo.write("    self->params[%d] = (void*)(ptroff_t)%s;\n" % (i,pname))
            elif(c == "s"):
                fo.write("    self->params[%d] = strdup(%s);\n" % (i,pname))
            elif(c == "m"):
                fo.write("    self->params[%d] = %s;\n" % (i,pname))
            elif(c == "c"):
                fo.write("    self->params[%d] = %s;\n" % (i,pname))
            elif(c == "i"):
                fo.write("    self->params[%d] = %s;\n" % (i,pname))
            elif(c == "j"):
                fo.write("    /* FIXME: write label %s */\n" % pname)
            elif(c == "S"):
                fo.write("    /* FIXME: write labels %s */\n" % pname)
            else:
                raise "Unknown type "+c
            i = i+1
        fo.write("    return self;\n")
        fo.write("}\n")

        fo.write("#define "+name+"(")
        fo.write(",".join(["method"]+names))
        fo.write(") {method->code = abc_"+name+"(")
        fo.write(",".join(["method->code"]+names))
        fo.write(");}\n")


fo.close()
fi.close()
#{0x75, "convert_d", ""},

