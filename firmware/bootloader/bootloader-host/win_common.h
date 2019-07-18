#ifndef WIN_COMMON_H
#define WIN_COMMON_H

#ifdef _WIN32

#ifdef _MSC_VER
#define strncasecmp _strnicmp

int usleep(unsigned long long us);

#endif // _MSC_VER

// getopt for windows
// got this from
// https://gist.github.com/superwills/5815344

extern int opterr;		/* if error message should be printed */
extern int optind;		/* index into parent argv vector */
extern int optopt;		/* character checked for validity */
extern int optreset;  	/* reset getopt  */
extern char *optarg;	/* argument associated with option */

int getopt(int nargc, char * const nargv[], const char *ostr);


// works as linux basename
char *basename(const char* filepath);

#endif // _WIN32
#endif // WIN_COMMON_H
