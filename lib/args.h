/* args.h
   Routines to simplify argument handling

   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 
 
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

#ifndef __args_h__
#define __args_h__

#ifndef NO_ARGPARSER

extern int args_callback_option(char*,char*);
extern int args_callback_longoption(char*,char*);
extern int args_callback_command(char*,char*);
extern void args_callback_usage(char*name);

//int argn;
//char**argv;

static void processargs(int argn2,char**argv2)
{
    int t;
    if(argn2==1)
    {
        args_callback_usage(argv2[0]);
        exit(1);
    }
    for(t=1;t<argn2;t++)
    {
        char*next;
        if(t<argn2-1) next=argv2[t+1];
        else        next=0;

        if(argv2[t][0]=='-' && argv2[t][1])
        {
            if(argv2[t][1]=='-')
            {
                if(!strcmp(&argv2[t][2],"help")) 
                {
                    args_callback_usage(argv2[0]);
                    exit(1);
                }
                t+=args_callback_longoption(&argv2[t][2],next);
            }
            else
            {
                if(strchr("?h",argv2[t][1]))
                {
                    args_callback_usage(argv2[0]);
                    exit(1);
                }
                if(argv2[t][1]) // this might be something like e.g. -xvf
                {
                    char buf[2];
                    int s=1;
                    int ret;
                    buf[1]=0;
                    do{
                        if(argv2[t][s+1]) {
                          buf[0] = argv2[t][s];
                          ret = args_callback_option(buf,&argv2[t][s+1]);
                        }
                        else {
                          t+= args_callback_option(&argv2[t][s], next);
                          break;
                        }
                        s++;
                    } while(!ret);
                }
            }
        }
        else
        {
	    int num = args_callback_command(argv2[t],next);
	    if(num>2) {
		fprintf(stderr, "internal error in command line parsing\n");
		exit(1);
	    }
            t+=num;
        }
    }
}

struct options_t
{
    const char*shortoption;
    const char*longoption;
};

static int args_long2shortoption(struct options_t*options, char*name, char*val)
{
    char*equal = strchr(name,'=');
    if (equal) {
        *equal = 0;
        equal++;
    }
    while(options->shortoption) {
        if(!strcmp(options->longoption, name)) {
                char*tmp = (char*)malloc(strlen(options->shortoption)
                        +(equal?strlen(equal)+2:2));
                strcpy(tmp, options->shortoption);
                if(equal) {
                    //strcpy(&tmp[strlen(tmp)], equal);
                    int ret = args_callback_option(tmp, equal);
                    if(!ret) {
                        fprintf(stderr, "Warning: Option --%s takes no parameter.\n", name);
                    }
                    return 0;
                }
                return args_callback_option(tmp,val);
        }
        options++;
    }
    fprintf(stderr, "Unknown option: --%s\n", name);
    exit(1);
}

#endif

/* check whether the value t is in a given range.
  examples: 3 is in range 1-10: true
            7 is in range 2-4,6,8-10: false
	    9 is in range 1,2,3-12: true
*/
static char is_in_range(int t, char*irange)
{
    char*pos = irange;
    char*digits;
    int num;
    char range = 0;
    int last=0;
    char tmp;

    if(!irange)  // no range resembles (-OO,OO)
	return 1;

    while(*pos)
    {
	while(*pos == ' ' || *pos == '\r' || *pos == '\n' || *pos == '\t')
	    pos++;

	digits = pos;
	while(*digits>='0' && *digits<='9')
	    digits++;
	if(digits == pos) {
	    fprintf(stderr, "Error: \"%s\" is not a valid format (digit expected)\n",irange);
	    exit(1);
        }
	
	tmp=*digits;*digits=0;
	num = atoi(pos);
	*digits=tmp;
	pos = digits;

	while(*pos == ' ' || *pos == '\r' || *pos == '\n' || *pos == '\t')
	    pos++;

	if(range && last<=t && num>=t)
	    return 1;
	if(range) {
	    range = 0;
	    if(*pos)
	     pos ++;
	    continue;
	}

	if(*pos=='-')
	{
	    if(range) {
		fprintf(stderr, "Error: \"%s\" is not a valid format (too many '-'s)\n",irange);
		exit(1);
	    }
	    last = num;
	    range = 1;
	    if(*pos)
	     pos ++;
	    continue;
	} 
	else 
	{
	    /* if it isn't a '-', we assume it is a seperator like
	       ',', ';', ':', whatever. */
	    if(t == num)
		return 1;
	    if(*pos)
	     pos ++;
	    continue;
	}
    }
    if(range && last<=t)
	return 1;
    return 0;
}

static char* filename2template(char*filename, int*startindex)
{
    int l = strlen(filename);
    char*newname = (char*)malloc(l+5);
    /* first look whether the file is already numbered */
    while(1) {
        l--;
        if(l<0 || strchr("0123456789", filename[l]))
            break;
    };
    if(l>=0) {
        int lastdigit=l;
        int firstdigit=l;
        while(firstdigit && strchr("0123456789", filename[firstdigit-1]))
            firstdigit--;
        *startindex = atoi(filename+firstdigit);
        memcpy(newname, filename, firstdigit);
        sprintf(newname+firstdigit, "%%%dd", lastdigit+1-firstdigit);
        strcat(newname+firstdigit, filename+lastdigit+1);
        return newname;
    }
    /* if it isn't, try to paste a %d between filename and extension */
    char*dot = strrchr(filename, '.');
    if(dot) {
        int pos = dot-filename;
        memcpy(newname, filename, pos);
        newname[pos++] = '.';
        newname[pos++] = '%';
        newname[pos++] = 'd';
        strcpy(newname+pos, dot);
        *startindex = 1;
        return newname;
    }
    /* if that didn't work either, just append the number at the end */
    strcpy(newname, filename);
    strcat(newname, ".%d");
    *startindex = 1;
    return newname;
}

#endif //__args_h__
