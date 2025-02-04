#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include "patterns.h"
#include "stand.h"

/* driveras.c	software driver for the address standardizer.

		This program reads a data file and outputs
		each record with the standardized address
*/


/* in case the standard date & time macros aren't present */
#ifndef __DATE__ 
#define __DATE__ "?/?/?"
#endif !__DATE__
#ifndef __TIME__ 
#define __TIME__ "?:?"
#endif !__TIME__

/* in case BUFFERSIZE was defined differently */
#ifndef BUFFERSIZE 
#define BUFFERSIZE 100
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


FILE	*fptrs;
extern	char	pattern[32],	patmatch[2];
extern	void	new_locations(long start,FILE *sumfile);
extern	struct standardized_address *Addr_Stan2(char *address_string,
	char usf,char abbrev_opt,
	char *postal_state,char *zip_code);

void	main() {
register	long i = 0,j = 0,	iout,	kout;
long	k,	passstan,	nameat,	namelength,	addressat,	addresslength,	records;
long	fixedlength,	newlength,	written;
char	instring[BUFFERSIZE][400],	a_filename[100],
	outstring[BUFFERSIZE*2][500],	/* larger buffer necessary for	*/
					/* compound names		*/
	sparestring[BUFFERSIZE][500];
char	*fixedstring,	*teststring;
FILE	*locations;
FILE    *fptri,	*fptro,	*fother;

teststring = malloc(400);
fptrs = fopen("sumnp","w");
records = 0;
written = 0;

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

newlength =  fixedlength + 127 - 46;
fptro = fopen("stanoutput.txt","w");
fother = fopen("otheroutput.txt","w");

k = 0;

/* This system uses buffered i/o.  Buffered input and output means
   that a number of records are read in before processing which
   takes place before the records are outputed */

for (i = 0;(i < BUFFERSIZE) && (!feof(fptri));i++) fgets(instring[i],400,fptri);
records += i;
if (!feof(fptri)) do {
  for (i = 0,iout = 0,kout = 0;i<BUFFERSIZE;i++) {
    fixedstring = strtok(instring[i],"\n");
    fixedstring[fixedlength] = 0;
    strncpy(teststring,&fixedstring[addressat],addresslength);
    *(teststring + addresslength) = 0;
    if ((AddrRes = (struct standardized_address *)Addr_Stan2(teststring,' ','u',"  ","     ")) != NULL) {
	    sprintf(outstring[iout],"%s%6.6s%5.5s%20.20s%10.10s%4.4s%5.5s%3.3s%5.5s%4.4s%5.5s%4.4s%07d",fixedstring
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
		,a_filename,written++);
	    outstring[iout++][newlength] = 0;
	  } else {	/*	maximum value in for loop must equal the sum
				of all %s operators above */
	    sprintf(outstring[iout],"%s                                                                   %4.4s%07d",fixedstring,a_filename,written++);
	    outstring[iout++][newlength] = 0;
	  }
  }

  for (j = 0;(j < iout);j++) fprintf(fptro,"%s\n",outstring[j]);
  for (j = 0;(j < kout);j++) fprintf(fother,"%s\n",sparestring[j]);
  for (i = 0;(i < BUFFERSIZE) && (!feof(fptri));i++) fgets(instring[i],400,fptri);
  records += i;

} while (!feof(fptri));

iout = 0;	kout = 0;
i--;
records--;
for (j = 0;j < i;j++) {
  fixedstring = strtok(instring[j],"\n");
  fixedstring[fixedlength] = 0;
  strncpy(teststring,&fixedstring[addressat],addresslength);
  *(teststring + addresslength) = 0;
  if ((AddrRes = (struct standardized_address *)Addr_Stan2(teststring,' ','u',"  ","     ")) != NULL) {
	  sprintf(outstring[iout],"%s%6.6s%5.5s%20.20s%10.10s%4.4s%5.5s%3.3s%5.5s%4.4s%5.5s%4.4s%07d",fixedstring
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
		,a_filename,written++);
	  outstring[iout++][newlength] = 0;
	} else {	/*	maximum value in for loop must equal the sum
				of all %s operators above */
	sprintf(outstring[iout],"%s                                                                   %4.4s%07d",fixedstring,a_filename,written++);
	outstring[iout++][newlength] = 0;
  } else {
    if ((AddrRes = (struct standardized_address *)Addr_Stan2(teststring,' ','u',"  ","     ")) != NULL) {
	sprintf(sparestring[kout],"%s%6.6s%5.5s%20.20s%10.10s%4.4s%5.5s%3.3s%5.5s%4.4s%5.5s",fixedstring
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
	  sparestring[kout++][newlength] = 0;
    } else {	/*	maximum value in for loop must equal the sum
				of all %s operators above */
	sprintf(sparestring[kout],"%s                                                                   ",fixedstring);
	sparestring[kout++][newlength] = 0;
    }
  }
}

for (j = 0;j < iout;j++) fprintf(fptro,"%s\n",outstring[j]);
for (j = 0;(j < kout);j++) fprintf(fother,"%s\n",sparestring[j]);

new_locations2(fixedlength,fptrs);
new_locations2(fixedlength,stderr);

fclose(fptri);	fclose(fptro);	fclose(fother);
return;
}
