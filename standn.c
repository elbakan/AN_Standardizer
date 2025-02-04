/* np1.c by w winkler 921120  4p*/
/* frequency matcher: parameter-driven input*/
/* standn.c                        */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "standn.h"

char	pattern[32],	patmatch[2];

long standn(ptrn2,local_standname)
long ptrn2[][PTRNDEX2];
char local_standname[][91];
{
register	long	i,	j,	k = 0;
long		morenames,	cnt,	index = 0,	ll = 0,	kk = 0;
static		long	start[9] = {0,6,19,32,32,46,52,57,70};
char		temp[32],	partner[2],	deceased[2];
long		stanpass;

pattern[0] = SPACE;
pattern[1] = NULL_POINTER;
temp[0] = SPACE;
patmatch[0] = NULL_POINTER;
partner[0] = NULL_POINTER;
deceased[0] = NULL_POINTER;
for (i = 0;i < 90;i++) local_standname[0][i] = SPACE;
local_standname[0][90] = 0;
stanpass = 0;
morenames = 0;
cnt = 0;

for (i = 0;i <= numwords;i++) {
  strcat(pattern + cnt,wordtype[i]);
  if (partnerflag[i][0] > partner[0]) partner[0] = partnerflag[i][0];
  if ((commaflag[i] == 1) && (wordtype[i][0] != '1') && (wordtype[(i+1)][0] != '1')) {
    cnt++;
    strcat(pattern + cnt,"1");
  }
  if ((wordtype[i][0] == '5') && (skipflag[i][0] == '1'))
    deceased[0] = '1';
  else if ((wordtype[i][0] == '5') && (skipflag[i][0] != '1')) {
    local_standname[0][0] = SPACE;
    k = 1;
    for (j = 0;j < numwords;j++) {
	if((wordtype[j][0] == '9') ||	(wordtype[j][0] == '7') ||
					(wordtype[j][0] == '6')) {
	  strncpy(local_standname[0] + (k - 1),nameup + wdstrt[j],wdlen[j]);
	  k = k + wdlen[j] + 1;
	}
    }
    strncpy(local_standname[0] + 83,"100011",6);
    patmatch[0] = '1';
    n_bus++;
    return -1;
  }
  cnt++;
}

strcpy(temp,"\0");
strcat(temp,pattern);
strcpy(pattern,&temp[1]);

if ((index = s_table(pattern,1,n_patt,ptrn1)) > 0) {
  if (!strcmp(ptrn1[index],ptrn1[(index - 1)])) index--;
  patmatch[0] = '1';
  n_stan++;

  do {

    for (i = 0;i < 10;i++) {
	if ((ll = ptrn2[index][i]) > 0) {
	  ll--;	/* table index 1->n, wdstrt index 0->n-1 */
	  if (!i)	/* title */
            strncpy(local_standname[stanpass],nameup+wdstrt[ll],wdlen[ll]);
	  else if ((i == 1) || (i == 2)) {	/* first or middle */
	    strncpy((local_standname[stanpass]+start[i]),nameup+wdstrt[ll],wdlen[ll]);
	    strncpy((local_standname[stanpass]+start[i+6]),fullfirst[ll],strlen(fullfirst[ll]));
	  } else if (i == 3) {	/* prefix of last */
	    strncpy((local_standname[stanpass]+start[i]),nameup+wdstrt[ll],wdlen[ll]);
	    kk = ptrn2[index][(i + 1)] - 1;
	    strncpy((local_standname[stanpass]+start[i]+wdlen[ll]),nameup+wdstrt[kk],wdlen[kk]);
	  } else if (((i != 4) && (ptrn2[index][3])) ||
		((i == 4) && (!ptrn2[index][3]))) {	/* last with no prefix */
	    strncpy((local_standname[stanpass]+start[i]),nameup+wdstrt[ll],wdlen[ll]);
	  } else if (i > 4)	/* jr/sr if present */
	    strncpy((local_standname[stanpass]+start[i]),nameup+wdstrt[ll],wdlen[ll]);
	}
    }

    strncpy(&local_standname[stanpass][83],patmatch,1);
    if (jrsrflag[ptrn2[index][5]][0]>'1') {
	strncpy(&local_standname[stanpass][84],jrsrflag[ptrn2[index][5]],1);
    } else {
	strncpy(&local_standname[stanpass][84],"0",1);
    }
    strncpy(&local_standname[stanpass][85],partner,1);
    if ((deceased[0] == '1') && (ptrn2[index][9] == 1)) {
	strncpy(&local_standname[stanpass][86],"1",1);
    } else {
	strncpy(&local_standname[stanpass][86],"0",1);
    }

    /* Returns to beginning of loop if more than one name was present in the pattern */

    if (!(morenames = strcmp(ptrn1[index],ptrn1[(index+1)]))) {
	index++;		stanpass++;
	for (i = 0;i < 90;i++)	local_standname[stanpass][i] = SPACE;
	local_standname[stanpass][90] = 0;
    }
  } while (!morenames);
} else if (pattern != ptrn1[index]) {	/*	Appends flag to unstandardized name if
						pattern could not be found */
   
  strncpy(local_standname[0],nameup,strlen(nameup));
  strncpy((local_standname[0] + 83),"000011",6);
  return -1;
}

if (patmatch[0] == '1') {
  for (i = 0;i<=stanpass;i++) {
    local_standname[i][87] = toascii((i+48));
    local_standname[i][88] = toascii((stanpass+48));
  }
}

return(stanpass);

}

