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

int argn;
char**argv;

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
	else	    next=0;

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
		t+=args_callback_option(&argv2[t][1],next);
	    }
	}
	else
	{
	    t+=args_callback_command(argv2[t],next);
	}
    }
}

#endif //__args_h__
