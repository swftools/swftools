/* parser.h

   Copyright (c) 2003 Matthias Kramm <kramm@quiss.org>

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

#ifndef __parser_h__
#define __parser_h__

enum type_t {
    TWIP=0, 
    NUMBER=1,
    COMMAND=2,
    STRING=3,
    ASSIGNMENT=4,
    IDENTIFIER=5,
    RAWDATA=6,
    END=7
};

extern char*type_names[];

struct token_t {
    enum type_t type;
    int text_pos;
    char* text;
    int line;
    int column;
};

extern struct token_t* generateTokens(char*filename);
extern void freeTokens(struct token_t*);

#endif
