#ifndef WIN_GETOPT_H
#define WIN_GETOPT_H

// got this from
// https://gist.github.com/superwills/5815344

extern int opterr;		/* if error message should be printed */
extern int optind;		/* index into parent argv vector */
extern int optopt;		/* character checked for validity */
extern int optreset;  	/* reset getopt  */
extern char *optarg;	/* argument associated with option */

int getopt(int nargc, char * const nargv[], const char *ostr);

#endif // WIN_GETOPT_H
