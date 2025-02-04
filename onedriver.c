#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include "standn.h"
#include "patterns.h"
#include "stand.h"

/* gdriver.c	software driver for the name standardizer,
		address standardizer.

		This program reads a data file and outputs
		each record with the standardized address,
		name.
*/


/* in case the standard date & time macros aren't present */
#ifndef __DATE__
#define __DATE__ "?/?/?"
#endif
#ifndef __TIME__
#define __TIME__ "?:?"
#endif

/* in case BUFFERSIZE was defined differently */
#ifndef BUFFERSIZE
#define BUFFERSIZE 100
#endif

/* in case the standard boolean macros aren't present */
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* Modify if new file requires a different starting location of the
   address field */

struct	standardized_address	*AddrRes;

int	ReturnValueStatus; 

long	NumberOfAddresses,	/* count of invocations of AddrStan	*/
	NumberParsed,		/* successfully parsed			*/
	NumberOfTokens,		/* total number of tokens found		*/
	NumberOfCluesFnd,	/* total number of clues found		*/
	NumberOfCluesAssgd,	/* total number of clues assigned	*/
	NumberOfFunctions,	/* function processing from clues	*/
	NumberOfPatsFnd,	/* total number of patterns found	*/
	NumberOfPatGpsFnd,	/* number of pattern groups found	*/
	NumberOfExceptions,	/* additional processing needed		*/
	NumberOfFailures;	/* address standardizer failed		*/


FILE	*fptrs;
extern	char	pattern[32],	patmatch[2];
extern	void	new_locations(long start,FILE *sumfile);
extern	struct standardized_address *Addr_Stan2(char *address_string,
	int usf,int abbrev_opt,
	char *postal_state,char *zip_code);

int	main() {
int	 i = 0,j = 0,	iout,	kout,	l;
int	k,	passstan,	nameat,	namelength,	addressat,	addresslength,	records;
int	fixedlength,	newlength,	written;
char	instring[400],
	outstring[500],	/* larger buffer necessary for	*/
					/* compound names		*/
	sparestring[500],
	standname[5][91];
char	fixedstring[400],	teststring[400],	*printstring,	*print_other;
FILE	*locations;

fptrs = fopen("sumnp","w");
read_d(ptrn2);
records = 0;
written = 0;
printstring = malloc(100);
print_other = malloc(100);

if ((locations = fopen("parmdr.txt","r")) == NULL) {
  fprintf(stderr,"Information.\tCannot open parameter file 'parmdr.txt'\n");
  fprintf(fptrs,"Information.\tCannot open parameter file 'parmdr.txt'\n");
  exit(1);
} else {
  fscanf(locations,"%s",a_filename);
  if ((fptri = fopen(a_filename,"r")) == NULL) {
    fprintf(stderr,"Cannot open main input file %s \n",a_filename);
    fprintf(fptrs,"Cannot open main input file %s \n",a_filename);
    fclose(fptrs);
    exit(1);
  }
  fscanf(locations,"%d %d %d %d %d",
  	&nameat,	&namelength,
  	&addressat,	&addresslength,
  	&fixedlength);
  fclose(locations);
}

newlength =  fixedlength + 127;
fptro = fopen("stanoutput.txt","w");
fother = fopen("otheroutput.txt","w");

k = 0;

fgets(instring,400,fptri);
l = strlen(instring) - 1;
instring[l] = 0;

records++;
sprintf(printstring,"%%%d.%ds%%5.5s%%20.20s%%10.10s%%4.4s%%5.5s%%3.3s%%5.5s%%4.4s%%5.5s%%4.4s%%46.46s\n",fixedlength,fixedlength);
sprintf(print_other,"%%%d.%ds                                                                   %%46.46s\n",fixedlength,fixedlength);

do {

    strcpy(fixedstring,instring);
//    fixedstring[fixedlength-1] = 0;
    strncpy(teststring,&fixedstring[addressat],addresslength);
//    *(teststring + addresslength) = 0;
	/* name standardization */

    strncpy(namefld,&fixedstring[nameat],namelength);
//    namefld[namelength+1] = 0;
    name_split();
    dict_match();

  /*	passstan is the number of names in the name field (-1)
	e.g. 'Joe & Sue Smith' would return 1.
	a nonstandardized field would return -1	*/

    if ((passstan = standn(ptrn2,standname)) >= 0) {

		/* address standardization */
	  if ((AddrRes = (struct standardized_address *)Addr_Stan2(teststring,' ','u',"  ","     ")) != NULL) {
	    fprintf(fptro,printstring,fixedstring
		,AddrRes->hn
		,AddrRes->snpd
		,AddrRes->msn
		,AddrRes->snst
		,AddrRes->wsd
		,AddrRes->wsi
		,AddrRes->rrd
		,AddrRes->rri
 		,AddrRes->bxd
		,AddrRes->bxi
		,standname[0]);
	    written++;
	  } else {	/*	maximum value in for loop must equal the sum
				of all %s operators above */
	    fprintf(fptro,print_other,fixedstring,standname[0]);
            written++;
	  }
     } else {
	if ((AddrRes = (struct standardized_address *)Addr_Stan2(teststring,' ','u',"  ","     ")) != NULL) {
	  fprintf(fother,printstring,fixedstring
		,AddrRes->hn
		,AddrRes->snpd
		,AddrRes->msn
		,AddrRes->snst
		,AddrRes->wsd
		,AddrRes->wsi
		,AddrRes->rrd
		,AddrRes->rri
 		,AddrRes->bxd
		,AddrRes->bxi
		,standname[0]);
          written++;

	} else {	/*	maximum value in for loop must equal the sum
				of all %s operators above */
	  fprintf(fother,print_other,fixedstring,standname[0]);
          written++;
        }
     }

  fgets(instring,400,fptri);
//  instring[fixedlength] = 0;
  records++;
  l = strlen(instring) - 1;
  instring[l] = 0;

} while (!feof(fptri));


fprintf(fptrs,"\n Number of records read is            %6ld \n",records);
fprintf(fptrs," Number of records written is            %6ld \n",written);

new_locations(fixedlength,fptrs);
new_locations(fixedlength,stderr);

fclose(fptri);	fclose(fptro);	fclose(fptrs);	fclose(fother);
return 0;
}
