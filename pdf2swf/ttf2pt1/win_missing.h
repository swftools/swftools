/*
 * Implementation of things missing in Windows
 */

#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif

#undef ntohs
#undef ntohl
#undef htonl

#ifdef WINDOWS_FUNCTIONS
/* byte order */

char *optarg;
int optind=1;

char getopt(int argc, char **argv, char *args) {
	int n,nlen=strlen(args),nLen=0;
	char nCmd;
	
	if (argv[optind] && *argv[optind]=='-') {
		nCmd=*((argv[optind]+1));

		for (n=0;n<nlen;n++) {
			if (args[n] == ':') continue;
			if (args[n] == nCmd) {
				if (args[n+1]==':') {
					char retVal;
					retVal=*(argv[optind]+1);
					optarg=argv[optind+1];
					if (!optarg) optarg="";
					optind+=2;
					return retVal;
				} else {
					char retVal;
					retVal=*(argv[optind]+1);
					optarg=NULL;
					optind+=1;
					return retVal;
				}
			}
		}	
	}
	return -1;
}

#else

unsigned short ntohs(unsigned short inv);
unsigned long ntohl(unsigned long inv);
unsigned long htonl(unsigned long inv);

extern char *optarg;
extern int optind;

char getopt(int argc, char **argv, char *args);
#endif
