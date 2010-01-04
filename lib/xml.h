/* xml.h
   Lightweight and fast xml parser.

   Part of the swftools package.
   
   Copyright (c) 2010 Matthias Kramm <kramm@quiss.org> 
 
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

#ifndef __xml_h__
#define __xml_h__

#include "bitio.h"

typedef struct _xmlattribute {
    const char*name;
    const char*value;
    struct _xmlattribute*next;
} xmlattribute_t;

typedef struct _xmlconsumer {
    void (*start_tag)(struct _xmlconsumer*, char*name, xmlattribute_t*attributes);
    void (*data)(struct _xmlconsumer*, char*data, int len);
    void (*end_tag)(struct _xmlconsumer*, char*name);
    void*internal;
} xmlconsumer_t;

int xml_parse(reader_t*reader, xmlconsumer_t*out);

#endif
