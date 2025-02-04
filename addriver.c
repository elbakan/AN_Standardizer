#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include "patterns.h"
#include "stand.h"

/* addriver.c	software driver for the address standardizer.

		This program reads a data file and outputs
		each record with the standardized address.
*/


#define	SPACE50 	"                                                  "

/* in case the standard date & time macros aren't present */
#ifndef __DATE__
#define __DATE__ "?/?/?"
#endif !__DATE__
#ifndef __TIME__
#define __TIME__ "?:?"
#endif !__TIME__

/* in case BUFFERSIZE was defined differently */
#ifndef BUFFERSIZE
#define BUFFERSIZE 233
#endif !BUFFERSIZE

/* in case the standard boolean macros aren't present */
#ifndef TRUE
#define TRUE 1
#endif !TRUE
#ifndef FALSE
#define FALSE 0
#endif !FALSE

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


FILE	*fptrs,	*fptri,	*fptro;
extern	struct standardized_address *Addr_Stan2(char *address_string,
	int usf,int abbrev_opt,
	char *postal_state,char *zip_code);

int	main() {
register	long i = 0,j = 0;
long	k,	passstan,	addressat,	addresslength,	records;
long	fixedlength,	newlength,	written;
char	instring[BUFFERSIZE*2][500],
	outstring[BUFFERSIZE*2][500];
char	*fixedstring,	*teststring,	*a_filename;
FILE	*locations;

teststring = malloc(500);
fptrs = fopen("sum.add","w");
records = 0;
written = 0;
a_filename = malloc(200);

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
  fscanf(locations,"%d %d %d",
  	&addressat,	&addresslength,
  	&fixedlength);
  fclose(locations);
}

newlength =  fixedlength + 84;
fptro = fopen("stanoutput.txt","w");

k = 0;

/* This system uses buffered i/o.  Buffered input and output means
   that a number of records are read in before processing which
   takes place before the records are outputed */

for (i = 0;(i < BUFFERSIZE) && (!feof(fptri));i++) fgets(instring[i],400,fptri);
records += i;
if (!feof(fptri)) do {
  fprintf(stderr,"%d ",records);
  for (i = 0;i<BUFFERSIZE;i++) {
    fixedstring = strtok(instring[i],"\n");
    fixedstring[fixedlength] = 0;
    strncpy(teststring,&fixedstring[addressat],addresslength);
    *(teststring + addresslength) = 0;
    if (strncmp(teststring,SPACE50,addresslength)) {

  /*	passstan is the number of names in the name field (-1)
	e.g. 'Joe & Sue Smith' would return 1.
	a nonstandardized field would return -1	*/


		/* address standardization */

	  if ((AddrRes = (struct standardized_address *)Addr_Stan2(teststring,' ','u',"  ","     ")) != NULL) {
	    sprintf(outstring[i],"%s%6.6s%5.5s%20.20s%10.10s%4.4s%5.5s%3.3s%5.5s%6.6s%6.6s",fixedstring
		,AddrRes->hn
		,AddrRes->snpd
		,AddrRes->msn
		,AddrRes->snst
		,AddrRes->wsd
		,AddrRes->wsi
		,AddrRes->rrd
		,AddrRes->rri
 		,AddrRes->bxd
		,AddrRes->bxi);
	    outstring[i][newlength] = 0;
	    written++;
	  } else {	/*	maximum value in for loop must equal the sum
				of all %s operators above */
	    sprintf(outstring[i],"%s                                                                      ",fixedstring);
	    outstring[i][newlength] = 0;
	    written++;
	  }
    } else {
	sprintf(outstring[i],"%s                                                                      ",fixedstring);
	outstring[i][newlength] = 0;
	written++;
    }
  }
  for (j = 0;(j < i);j++) fprintf(fptro,"%s\n",outstring[j]);
  for (i = 0;(i < BUFFERSIZE) && (!feof(fptri));i++) fgets(instring[i],400,fptri);
  records += i;

} while (!feof(fptri));

i--;
records--;
for (j = 0;j < i;j++) {
  fixedstring = strtok(instring[j],"\n");
  fixedstring[fixedlength] = 0;
  strncpy(teststring,&fixedstring[addressat],addresslength);
  *(teststring + addresslength) = 0;
  if (strncmp(teststring,SPACE50,addresslength)) {
	if ((AddrRes = (struct standardized_address *)Addr_Stan2(teststring,' ','u',"  ","     ")) != NULL) {
	  sprintf(outstring[j],"%s%6.6s%5.5s%20.20s%10.10s%4.4s%5.5s%3.3s%5.5s%6.6s%6.6s",fixedstring
		,AddrRes->hn
		,AddrRes->snpd
		,AddrRes->msn
		,AddrRes->snst
		,AddrRes->wsd
		,AddrRes->wsi
		,AddrRes->rrd
		,AddrRes->rri
 		,AddrRes->bxd
		,AddrRes->bxi);
	  outstring[j][newlength] = 0;
	  written++;
	} else {	/*	maximum value in for loop must equal the sum
				of all %s operators above */
	sprintf(outstring[j],"%s                                                                      ",fixedstring);
	outstring[j][newlength] = 0;
	written++;
	}
  } else {
    sprintf(outstring[j],"%s                                                                      ",fixedstring);
    outstring[j][newlength] = 0;
    written++;
  }
}
for (j = 0;j < i;j++) fprintf(fptro,"%s\n",outstring[j]);

fprintf(fptrs,"\n Number of records read is            %6ld \n",records);
fprintf(fptrs," Number of records written is            %6ld \n",written);

fclose(fptri);	fclose(fptro);	fclose(fptrs);
return;
}
