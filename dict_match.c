#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "standn.h"

/* dict_match
*/

void	dict_match()
{
register	long	i = 0;
long		index,	lenw,	lnameflag;
char		word[60];
long		prefixflag[WORDS];

lnameflag = 0;

for (i = 0;i<WORDS;i++) {
  fullfirst[i][0] = '\0';
  prefixflag[i] = 0; 
  jrsrflag[i][0] = partnerflag[i][0] = skipflag[i][0] = wordtype[i][0] = '0'; 
  jrsrflag[i][1] = partnerflag[i][1] = skipflag[i][1] = wordtype[i][1] = '\0'; 
}

i = numwords;

while (i >=  0) {
  strcpy(word,"\0");
    lenw = wdlen[i] = (wdend[i] - wdstrt[i] + 1);
    strncat(word,&nameup[wdstrt[i]],lenw);
    if ((index = s_table(word,1,n_conj,conj)) > 0) {
      partnerflag[i][0] = '1';
      wordtype[i][0] = '1';
      }
     else if ((index = s_table(word,1,n_jrsr,jrsrcd)) > 0) {
      jrsrflag[i][0] = (*jrsr2[index]);
      wordtype[i][0] = '2';
      }
     else if ((index = s_table(word,1,n_titl,title)) > 0) {
      wordtype[i][0] = '3';
      }
     else if ((index = s_table(word,1,n_suff,suffix)) > 0) {
      wordtype[i][0] = '4';
      }
     else if ((lenw > 1) &&
       ((index = s_table(word,1,n_skpl,skplst)) > 0)) {
      skipflag[i][0] = (*skplst2[index]);
      if (skipflag[i][0] !=  '1')  partnerflag[i][0] = '2';
      wordtype[i][0] = '5';
      }
     else if ((lenw == 1) && (isalpha(word[0]))) {
      wordtype[i][0] = '6';
     }
     else if (!lnameflag) {
      lnameflag++;
      wordtype[i][0] = '7';
     }
     else if ((lenw>1) &&
        ((index = search_table(word,1,n_pref,lenw,prefix))>0)) {
      wordtype[i][0] = '8';
      if (prefix[index][11] == '1') {
        prefixflag[i]=2;
        fullfirst[i][0] = '\0';
        if ((index = s_table(word,1,n_nick,orig_name))>0) {
          fullfirst[i][0] = '\0';
          strcat(fullfirst[i],new_name[index]);
          }
        else strcat(fullfirst[i],word);
        }
       else prefixflag[i]++;
     }
     else if ((wordtype[i][0] == '0') && (lnameflag == 1)) {
      wordtype[i][0] = '9';
      fullfirst[i][0] = '\0';
      if ((index = s_table(word,1,n_nick,orig_name))>0) {
        strcat(fullfirst[i],new_name[index]);
        }
       else strcat(fullfirst[i],word);
      }
    i--;
   } /* end main while(i>0) */ 

    if ((numwords>3) && (wordtype[numwords][0] == '6') &&
        (wordtype[(numwords-1)][0] == '6') && (wordtype[(numwords-2)][0] == '6')) {
      strcpy(word,"\0");
      strncat(word,&nameup[wdstrt[(numwords-2)]],5);
      if ((index = search_table(word,1,n_three,5,three_suff))>0) {
        numwords -=  2;
        wordtype[numwords][0] = '4';
        nameup[(wdstrt[numwords]+1)] = nameup[(wdstrt[numwords]+2)];
        nameup[(wdstrt[numwords]+2)] = nameup[(wdstrt[numwords]+4)];
        nameup[(wdstrt[numwords]+3)] = '\0';
        wdend[numwords] = wdstrt[numwords]+2;
        wdlen[numwords] +=  2;
       }
     }
     else if ((numwords>2) && (wordtype[numwords][0] == '6') &&
              (wordtype[(numwords-1)][0] == '6')){
      strcpy(word,"\0");
      strncat(word,&nameup[wdstrt[(numwords - 1)]],3);
      if ((index = search_table(word,1,n_two,3,two_suff)) > 0) {
        numwords--;
        wordtype[numwords][0] = '4';
        nameup[(wdstrt[numwords] + 1)] = nameup[(wdstrt[numwords] + 2)];
        nameup[(wdstrt[numwords] + 2)] = '\0';
        wdend[numwords] = wdstrt[numwords]+1;
        wdlen[numwords]++;
       }
     }
  return;
}
