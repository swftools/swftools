/* files.h

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

#ifndef __avm2_files_h__
#define __avm2_files_h__

typedef struct _include_dir {
    char*path;
    struct _include_dir*next;
} include_dir_t;

extern int current_line;
extern int current_column;
extern char* current_filename;
extern char* current_filename_short;
extern char* current_filename_long;
extern include_dir_t* current_include_dirs;

void add_include_dir(char*dir);

char*find_file(const char*filename, char report_errors);
void enter_file(const char*name, const char*filename, void*state);
FILE* enter_file2(const char*name, const char*filename, void*state);
void* leave_file();

char* concat_paths(const char*base, const char*add);
char* normalize_path(const char*path);
char* filename_to_lowercase(const char*name);

#endif
