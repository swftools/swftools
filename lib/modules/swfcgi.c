/* swfcgi.c

   Parse CGI parameters
   
   Partly adopted from Steven Grimm's uncgi tool and library.

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This file is distributed under the GPL, see file COPYING for details 

*/


#define ishex(x) (((x) >= '0' && (x) <= '9') || ((x) >= 'a' && (x) <= 'f') || ((x) >= 'A' && (x) <= 'F'))

#define PREFIX "WWW_"

static int htoi(unsigned char * s)
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

static void url_unescape(unsigned char * s)
{ unsigned char  *dest = s;

  while (s[0])
  { if (s[0] == '+') dest[0] = ' ';
    else
    { if (s[0] == '%' && ishex(s[1]) && ishex(s[2]))
      { dest[0] = (unsigned char) htoi(s + 1);
        s += 2;
      }
      else dest[0] = s[0];
    }
    s++;dest++;
  }
  dest[0] = 0;
}

static void cgienv(unsigned char * var)
{ unsigned char *buf, *c, *s, *t, *oldval = NULL, *newval;
  int despace = 0, got_cr = 0;

  // fprintf(stderr,"%s\n",var);
  url_unescape(var);
  // fprintf(stderr,"%s\n",var);

  
  buf = (unsigned char*)malloc(strlen(var) + sizeof(PREFIX) + 2);
  if (!buf) return;

  strcpy(buf, PREFIX);
  if (var[0] == '_')
  { strcpy(&buf[sizeof(PREFIX)-1], &var[1]);
    despace = 1;
  }
  else strcpy(&buf[sizeof(PREFIX)-1], var);

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

  if ((oldval = getenv(buf)))
  { newval = (unsigned char*)malloc(strlen(oldval) + strlen(buf) + strlen(&c[1]) + 3);
    if (!newval) return;

    c[0] = '=';
    sprintf(newval, "%s#%s", buf, oldval);
    c[0] = 0;

    oldval -= strlen(buf) + 1; // skip past VAR= 
  }
  else 
  { c[0] = '=';
    newval = buf;
  }
  
  putenv(newval);
        
  if (oldval)
  { free(oldval);
    free(buf);
  }
}

static void scanquery(char * q)
{ char *next = q;
  if (!q) return;

  while (next)
  { next = strchr(q, '&');
    if (next) next[0] = 0;
    cgienv(q);
    if (next)
    { next[0] = '&';
      q = next+1;
    }
  } 
}

char * postread()
{ char * buf = NULL;
  int size = 0, sofar = 0, got;

  buf = getenv("CONTENT_TYPE");
  if ((!buf) || strcmp(buf, "application/x-www-form-urlencoded")) return NULL;

  buf = getenv("CONTENT_LENGTH");
  if (!buf) return NULL;
        
  size = atoi(buf);
  buf = (unsigned char*)malloc(size + 1);
  if (buf)
  { do
    { got = fread(buf + sofar, 1, size - sofar, stdin);
      sofar += got;
    } while (got && sofar < size);
    buf[sofar] = 0;
  }

  return buf;
}

void uncgi()
{ char *query, *dupquery, *method;

  query = getenv("QUERY_STRING");
  if ((query) && strlen(query))
  { dupquery = strdup(query);
    scanquery(dupquery);
    free(dupquery);
  }

  method = getenv("REQUEST_METHOD");
  if ((method) && ! strcmp(method, "POST"))
  { query = postread();
    if ((query)&&(query[0]!=0)) scanquery(query);
    free(query);
  }
  
}
      
#undef ishex
