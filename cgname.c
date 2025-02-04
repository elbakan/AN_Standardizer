#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include "standn.h"
#include "patterns.h"
#include "stand.h"

/* cgnname.c	software driver for the name standardizer,
		address standardizer.

		This program reads a form and outputs a
		web page with the standardized name
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
	int usf,int abbrev_opt,
	char *postal_state,char *zip_code);

void	main() {
register	long i = 0,j = 0,	iout,	kout;
long	k,	passstan,	nameat,	namelength,	addressat,	addresslength,	records;
long	fixedlength,	newlength,	written;
char	instring[BUFFERSIZE][400],
	outstring[BUFFERSIZE*3][500],	/* larger buffer necessary for	*/
					/* compound names		*/
	sparestring[BUFFERSIZE*3][500],
	standname[5][91];
char	*fixedstring,	*teststring;
FILE	*locations;

teststring = malloc(400);
fptrs = fopen("sumnp","w");
read_d(ptrn2);
records = 0;
written = 0;


newlength =  fixedlength + 164;
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
    
	/* name standardization */

    strncpy(namefld,&fixedstring[nameat],namelength);
    namefld[namelength+1] = 0;
    name_split();
    dict_match();

  /*	passstan is the number of names in the name field (-1)
	e.g. 'Joe & Sue Smith' would return 1.
	a nonstandardized field would return -1	*/

    if ((passstan = standn(ptrn2,standname)) >= 0) {

	for(k = 0;k <= passstan;k++) {

		/* address standardization */

	  if ((AddrRes = (struct standardized_address *)Addr_Stan2(teststring,' ','u',"  ","     ")) != NULL) {
	    sprintf(outstring[iout],"%s%6.6s%5.5s%20.20s%10.10s%4.4s%5.5s%3.3s%5.5s%4.4s%5.5s%90.90s",fixedstring
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
		,standname[k]);
	    outstring[iout++][newlength] = 0;
	    written++;
	  } else {	/*	maximum value in for loop must equal the sum
				of all %s operators above */
	    sprintf(outstring[iout],"%s                                                                   %90.90s%4.4s%07d",fixedstring,standname[k],a_filename,written++);
	    outstring[iout++][newlength] = 0;
	  }
	}
     } else {
	if ((AddrRes = (struct standardized_address *)Addr_Stan2(teststring,' ','u',"  ","     ")) != NULL) {
	  sprintf(sparestring[kout],"%s%6.6s%5.5s%20.20s%10.10s%4.4s%5.5s%3.3s%5.5s%4.4s%5.5s%90.90s",fixedstring
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
	  sparestring[kout++][newlength] = 0;
	} else {	/*	maximum value in for loop must equal the sum
				of all %s operators above */
	  sprintf(sparestring[kout],"%s                                                                   %90.90s",fixedstring,standname[k]);
	  sparestring[kout++][newlength] = 0;
        }
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
  strncpy(namefld,&fixedstring[nameat],namelength);
  namefld[namelength+1] = 0;
  name_split();
  dict_match();

  if ((passstan = standn(ptrn2,standname)) >= 0) {

    for(k = 0;k <= passstan;k++) {
	if ((AddrRes = (struct standardized_address *)Addr_Stan2(teststring,' ','u',"  ","     ")) != NULL) {
	  sprintf(outstring[iout],"%s%6.6s%5.5s%20.20s%10.10s%4.4s%5.5s%3.3s%5.5s%4.4s%5.5s%90.90s",fixedstring
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
		,standname[k]);
	  outstring[iout++][newlength] = 0;
	  written++;
	} else {	/*	maximum value in for loop must equal the sum
				of all %s operators above */
	sprintf(outstring[iout],"%s                                                                   %90.90s",fixedstring,standname[k]);
	outstring[iout++][newlength] = 0;
	written++;
	}
    }
  } else {
    if ((AddrRes = (struct standardized_address *)Addr_Stan2(teststring,' ','u',"  ","     ")) != NULL) {
	sprintf(sparestring[kout],"%s%6.6s%5.5s%20.20s%10.10s%4.4s%5.5s%3.3s%5.5s%4.4s%5.5s%90.90s",fixedstring
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
	  sparestring[kout++][newlength] = 0;
    } else {	/*	maximum value in for loop must equal the sum
				of all %s operators above */
	sprintf(sparestring[kout],"%s                                                                   %90.90s",fixedstring,standname[k]);
	sparestring[kout++][newlength] = 0;
    }
  }
}

for (j = 0;j < iout;j++) fprintf(fptro,"%s\n",outstring[j]);
for (j = 0;(j < kout);j++) fprintf(fother,"%s\n",sparestring[j]);

fprintf(fptrs,"\n Number of records read is            %6ld \n",records);
fprintf(fptrs," Number of records written is            %6ld \n",written);
fprintf(fptrs," Number of names standardized is      %6ld \n\n",n_stan);

new_locations(fixedlength,fptrs);
new_locations(fixedlength,stderr);

fclose(fptri);	fclose(fptro);	fclose(fptrs);	fclose(fother);
return;
}
