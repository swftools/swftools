/* args.h
   Routines to simplify argument handling

   Part of the swftools package.

   Copyright (c) 2001 Matthias Kramm <kramm@quiss.org> 

   This file is distributed under the GPL, see file COPYING for details */

#ifndef __args_h__
#define __args_h__

extern int args_callback_option(char*,char*);
extern int args_callback_longoption(char*,char*);
extern int args_callback_command(char*,char*);
extern void args_callback_usage(char*name);

//int argn;
//char**argv;

void processargs(int argn2,char**argv2)
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

        if(argv2[t][0]=='-')
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
                else // - usually means "read stdout"
                {
                    t+=args_callback_option(&argv2[t][1],next);
                }
            }
        }
        else
        {
            t+=args_callback_command(argv2[t],next);
        }
    }
}

struct options_t
{
    char*shortoption;
    char*longoption;
};

int args_long2shortoption(struct options_t*options, char*name, char*val)
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

/* check whether the value t is in a given range.
  examples: 3 is in range 1-10: true
            7 is in range 2-4,6,8-10: false
	    9 is in range 1,2,3-12: true
*/
char is_in_range(int t, char*irange)
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

#endif //__args_h__
