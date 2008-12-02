/* registry.c

   Routines for compiling Flash2 AVM2 ABC Actionscript

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2008 Matthias Kramm <kramm@quiss.org>
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include "pool.h"
#include "registry.h"

static namespace_t static_empty_ns = {
    ACCESS_PACKAGE, ""
};
static namespace_t static_flash_display_ns = {
    ACCESS_PACKAGE, "flash.display"
};
static multiname_t static_object_class = {
    QNAME, &static_empty_ns, 0, "Object"
};
static multiname_t static_anytype_class = {
    QNAME, &static_empty_ns, 0, 0
};
static multiname_t static_string_class = {
    QNAME, &static_empty_ns, 0, "String"
};
static multiname_t static_boolean_class = {
    QNAME, &static_empty_ns, 0, "Boolean"
};
static multiname_t static_number_class = {
    QNAME, &static_empty_ns, 0, "Number"
};
static multiname_t static_int_class = {
    QNAME, &static_empty_ns, 0, "int"
};
static multiname_t static_uint_class = {
    QNAME, &static_empty_ns, 0, "uint"
};
static multiname_t static_movieclip_class = {
    QNAME, &static_flash_display_ns, 0, "MovieClip"
};

multiname_t* registry_getobjectclass() {return &static_object_class;}
multiname_t* registry_getanytype() {return &static_anytype_class;}
multiname_t* registry_getstringclass() {return &static_string_class;}
multiname_t* registry_getintclass() {return &static_int_class;}
multiname_t* registry_getuintclass() {return &static_uint_class;}
multiname_t* registry_getbooleanclass() {return &static_boolean_class;}
multiname_t* registry_getnumberclass() {return &static_number_class;}
multiname_t* registry_getMovieClip() {return &static_movieclip_class;}


multiname_t* registry_findclass(const char*package, const char*name)
{
    multiname_t*m=0;
    if(!package) {
        m = multiname_new(0, name);
    } else {
        namespace_t*ns = 0;

        /* things in the "flash" package are usually public */
        if(!strncmp(package, "flash", 5))
            ns =namespace_new_package(package);
        else
            ns = namespace_new_packageinternal(package);

        m = multiname_new(ns,name);
        namespace_destroy(ns);
    }
    return m;
}
multiname_t* registry_getsuperclass(multiname_t*m)
{
    if(!m->name)
        return 0;
    if(!strcmp(m->name, "Object"))
        return 0;

    else if(!strcmp(m->name, "MovieClip"))
        return multiname_fromstring("[package]flash.display::Sprite");
    else if(!strcmp(m->name, "Sprite"))
        return multiname_fromstring("[package]flash.display::DisplayObjectContainer");
    else if(!strcmp(m->name, "DisplayObjectContainer"))
        return multiname_fromstring("[package]flash.display::InteractiveObject");
    else if(!strcmp(m->name, "InteractiveObject"))
        return multiname_fromstring("[package]flash.display::DisplayObject");
    else if(!strcmp(m->name, "DisplayObject"))
        return multiname_fromstring("[package]flash.events::EventDispatcher");
    else
        return &static_object_class;
}

