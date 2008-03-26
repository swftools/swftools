/* swfcgi.c

   Parse CGI parameters
   
   Partly adopted from Steven Grimm's uncgi tool and library.

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
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

#include <stdlib.h>
#include <stdio.h>
#include "../rfxswf.h"

#define ishex(x) (((x) >= '0' && (x) <= '9') || ((x) >= 'a' && (x) <= 'f') || ((x) >= 'A' && (x) <= 'F'))

#define PREFIX "WWW_"

static int swf_htoi(unsigned char * s)
{ int     value;
  char    c;

  c = s[0];
  if (isupper(c)) c = tolower(c);
  value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

  c = s[1];
  if (isupper(c)) c = tolower(c);
  value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

  return (value);
}

static void swf_url_unescape(unsigned char * s)
{ unsigned char  *dest = s;

  while (s[0])
  { if (s[0] == '+') dest[0] = ' ';
    else
    { if (s[0] == '%' && ishex(s[1]) && ishex(s[2]))
      { dest[0] = (unsigned char) swf_htoi(s + 1);
        s += 2;
      }
      else dest[0] = s[0];
    }
    s++;dest++;
  }
  dest[0] = 0;
}

static void swf_cgienv(unsigned char * var)
{ unsigned char *buf, *c, *s, *t, *oldval = NULL, *newval;
  int despace = 0, got_cr = 0;

  // fprintf(stderr,"%s\n",var);
  swf_url_unescape(var);
  // fprintf(stderr,"%s\n",var);

  
  buf = (unsigned char*)rfx_alloc(strlen((const char*)var) + sizeof(PREFIX) + 2);
  if (!buf) return;

  strcpy((char*)buf, (const char*)PREFIX);
  if (var[0] == '_')
  { strcpy((char*)&buf[sizeof(PREFIX)-1], (const char*)&var[1]);
    despace = 1;
  }
  else strcpy((char*)&buf[sizeof(PREFIX)-1], (const char*)var);

  for (c = buf; c[0] ; c++)
  { if (c[0] == '.') c[0] = '_';
    if (c[0] == '=') break;
  }
  if (!c[0]) c[1] = 0;
  c[0] = 0;

  if (despace && c[1])
  { for (s = c+1; s[0] && isspace(s[0]); s++);
    t = c + 1;
    while (s[0])
    { if (s[0] == '\r')
      { got_cr = 1;
        s++;
        continue;
      }
      if (got_cr)
      { if (s[0] != '\n')
        *t++ = '\n';
        got_cr = 0;
      }
      *t++ = *s++;
    }
    while (t > c && isspace(*--t));
    t[1] = 0;
  }

  if ((oldval = (unsigned char*)getenv((const char*)buf)))
  { newval = (unsigned char*)rfx_alloc(strlen((const char*)oldval) + strlen((const char *)buf) + strlen((const char*)&c[1]) + 3);
    if (!newval) return;

    c[0] = '=';
    sprintf((char*)newval, "%s#%s", buf, oldval);
    c[0] = 0;

    oldval -= strlen((const char*)buf) + 1; // skip past VAR= 
  }
  else 
  { c[0] = '=';
    newval = buf;
  }
  
  putenv((char *)newval);
        
  if (oldval)
  { rfx_free(oldval);
    rfx_free(buf);
  }
}

static void swf_scanquery(char * q)
{ char *next = q;
  if (!q) return;

  while (next)
  { next = strchr(q, '&');
    if (next) next[0] = 0;
    swf_cgienv((unsigned char*)q);
    if (next)
    { next[0] = '&';
      q = next+1;
    }
  } 
}

char * swf_postread()
{ char * buf = NULL;
  int size = 0, sofar = 0, got;

  buf = getenv("CONTENT_TYPE");
  if ((!buf) || strcmp(buf, "application/x-www-form-urlencoded")) return NULL;

  buf = getenv("CONTENT_LENGTH");
  if (!buf) return NULL;
        
  size = atoi(buf);
  buf = (char*)rfx_alloc(size + 1);
  if (buf)
  { do
    { got = fread(buf + sofar, 1, size - sofar, stdin);
      sofar += got;
    } while (got && sofar < size);
    buf[sofar] = 0;
  }

  return buf;
}

void swf_uncgi()
{ char *query, *dupquery, *method;

  query = getenv("QUERY_STRING");
  if ((query) && strlen(query))
  { dupquery = strdup(query);
    swf_scanquery(dupquery);
    rfx_free(dupquery);
  }

  method = getenv("REQUEST_METHOD");
  if ((method) && ! strcmp(method, "POST"))
  { query = swf_postread();
    if ((query)&&(query[0]!=0)) swf_scanquery(query);
    rfx_free(query);
  }
  
}
      
#undef ishex
#undef PREFIX
