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

#endif //__args_h__
