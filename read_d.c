#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "standn.h"

extern	FILE	*fptrs;

void	read_d(ptrn2)
long	ptrn2[][PTRNDEX2];
{
register	long	i = 1,	j = 0;
char		hold[90];
FILE		*fptrt;

if ((fptrt = fopen("conj.dat","r")) == NULL) {
  fprintf(stderr,"Cannot open file of conjunctions \n");
  fclose(fptrs);
  exit(1);
}

while (!feof(fptrt) && (i < CONJDEX)) {
  if ((conj[i] = (char *)malloc(11)) == NULL) {
    fprintf(stderr," Malloc for conj[] when i = %d failed\n",i);
    fclose(fptrs);
    exit(1);
  }
  fscanf(fptrt,"%10s",conj[i]);
  i++;
}
i -= 2;

fprintf(fptrs," Conjunction table size is\t%6d \n",i);
n_conj = i;
i = 1; 
fclose(fptrt);


if ((fptrt = fopen("jrsr.dat","r")) == NULL) {
  fprintf(stderr,"Cannot open file of jr/srs \n");
  fclose(fptrs);
  exit(1);
}
while (!feof(fptrt) && (i < JRSRDEX)) {
  if ((jrsrcd[i] = (char *)malloc(13)) == NULL) {
    fprintf(stderr,"Malloc for jrsrcd[] when i = %d failed\n",i);
    fclose(fptrs);
    exit(1);
  }
  if ((jrsr2[i] = (char *)malloc(2)) == NULL) {
    fprintf(stderr,"Malloc for jrsr2[] when i = %d failed\n",i);
    fclose(fptrs);
    exit(1);
  }
  fscanf(fptrt,"%12s%s",jrsrcd[i],jrsr2[i]);
  i++;
}
i -= 2;
fprintf(fptrs," Jr/sr table size is\t\t%6d \n",i);
n_jrsr = i;
i = 1; 
fclose(fptrt);


if ((fptrt = fopen("nick.dat","r")) == NULL) {
  fprintf(stderr,"Cannot open file of nicknames \n");
  fclose(fptrs);
  exit(1);
}
while (!feof(fptrt) && (i < NICKDEX)) {
  if ((new_name[i] = (char *)malloc(17)) == NULL) {
    fprintf(stderr,"Malloc for new_name[] when i = %d failed\n",i);
    fclose(fptrs);
    exit(1);
  }
  if ((orig_name[i] = (char *)malloc(17)) == NULL) {
    fprintf(stderr,"Malloc for orig_name[] when i = %d failed\n",i);
    fclose(fptrs);
    exit(1);
  }
  fscanf(fptrt,"%16s%16s%s",orig_name[i],new_name[i],hold);
  i++;
}
i -= 2;
fprintf(fptrs," Nickname table size is\t\t%6d \n",i);
n_nick = i;
i = 1; 
fclose(fptrt);


if ((fptrt = fopen("patt.dat","r")) == NULL) {
  fprintf(stderr,"Cannot open file of patterns \n");
  fclose(fptrs);
  exit(1);
}
while (!feof(fptrt) && (i < PTRNDEX1)) {
  if ((ptrn1[i] = (char *)malloc(21)) == NULL) {
    fprintf(stderr,"Malloc for ptrn1[] when i = %d failed\n",i);
    fclose(fptrs);
    exit(1);
  }
  fscanf(fptrt,"%20s",ptrn1[i]);
  for(j = 0;j < PTRNDEX2;j++) fscanf(fptrt,"%2d",&ptrn2[i][j]);
  fgets(hold,MAX_INPUT_SIZE,fptrt);   /*finish line*/
  i++;
}
i -= 2;
fprintf(fptrs," Pattern table size is\t\t%6d \n",i);
n_patt = i;
i = 1; 
fclose(fptrt);


if ((fptrt = fopen("pref.dat","r")) == NULL) {
  fprintf(stderr,"Cannot open file of prefixes \n");
  fclose(fptrs);
  exit(1);
}
while (!feof(fptrt) && (i < PREDEX)) {
  if ((prefix[i] = (char *)malloc(12)) == NULL) {
    fprintf(stderr,"Malloc for prefix[] when i = %d failed\n",i);
    fclose(fptrs);
    exit(1);
  }
  fgets(prefix[i],MAX_INPUT_SIZE,fptrt);
  strtok(prefix[i],"\n");
  i++;
}
i -= 2;
fprintf(fptrs," Prefix table size is\t\t%6d \n",i);
n_pref = i;
i = 1; 
fclose(fptrt);


/* Skip lists are words associated with businesses */

if ((fptrt = fopen("skpl.dat","r")) == NULL) {
  fprintf(stderr,"Cannot open file of skip words \n");
  fclose(fptrs);
  exit(1);
}
while (!feof(fptrt) && (i < SKPDEX)) {
  if ((skplst[i] = (char *)malloc(17)) == NULL) {
    fprintf(stderr,"Malloc for skplst[] when i = %d failed\n",i);
    fclose(fptrs);
    exit(1);
  }
  if ((skplst2[i] = (char *)malloc(2)) == NULL) {
    fprintf(stderr,"Malloc for skplst2[] when i = %d failed\n",i);
    fclose(fptrs);
    exit(1);
  }
  fscanf(fptrt,"%17s%s",skplst[i],skplst2[i]);
  i++;
}
i -= 2;
fprintf(fptrs," Skip list table size is\t%6d \n",i);
n_skpl = i;
i = 1; 
fclose(fptrt);


if ((fptrt = fopen("suff.dat","r")) == NULL) {
  fprintf(stderr,"Cannot open file of suffixes \n");
  fclose(fptrs);
  exit(1);
}
while (!feof(fptrt) && (i < SUFDEX)) {
  if ((suffix[i] = (char *)malloc(7)) == NULL) {
    fprintf(stderr,"Malloc for suffix[] when i = %d failed\n",i);
    fclose(fptrs);
    exit(1);
  }
  fscanf(fptrt,"%6s",suffix[i]);
  i++;
}
i -= 2;
fprintf(fptrs," Suffix table size is\t\t%6d \n",i);
n_suff = i;
i = 1; 
fclose(fptrt);


if ((fptrt = fopen("titl.dat","r")) == NULL) {
  fprintf(stderr,"Cannot open file of titles \n");
  fclose(fptrs);
  exit(1);
}
while (!feof(fptrt) && (i < TITLDEX)) {
  if ((title[i] = (char *)malloc(15)) == NULL) {
    fprintf(stderr,"Malloc for title[] when i = %d failed\n",i);
    fclose(fptrs);
    exit(1);
  }
  fscanf(fptrt,"%s",title[i]);
  i++;
}
i -= 2;
fprintf(fptrs," Title table size is\t\t%6d \n",i); 
n_titl = i;
i = 1;
fclose(fptrt);


if ((fptrt = fopen("two.dat","r")) == NULL) {
  fprintf(stderr,"Cannot open file of two suffixes \n");
  fclose(fptrs);
  exit(1);
}
while (!feof(fptrt) && (i < TWODEX)) {
  if ((two_suff[i] = (char *)malloc(4)) == NULL) {
    fprintf(stderr,"Malloc for two_suff[] when i = %d failed\n",i);
    fclose(fptrs);
    exit(1);
  }
  fgets(two_suff[i],5,fptrt);
  two_suff[i][3] = '\0';
  i++;
}
i -= 2;
fprintf(fptrs," Two suffix table size is\t%6d \n",i); 
n_two = i;
i = 1;
fclose(fptrt);


if ((fptrt = fopen("thre.dat","r")) == NULL) {
  fprintf(stderr,"Cannot open file of three suffixes \n");
  fclose(fptrs);
  exit(1);
}
while (!feof(fptrt) && (i < THREDEX)) {
  if ((three_suff[i] = (char *)malloc(6)) == NULL) {
    fprintf(stderr,"Malloc for three_suff[] when i = %d failed\n",i);
    fclose(fptrs);
    exit(1);
  }
  fgets(three_suff[i],7,fptrt);
  three_suff[i][5] = '\0';
  i++;
}
i -= 2;
fprintf(fptrs," Three suffix table size is\t%6d \n",i); 
n_three = i; 
i = 1;
fclose(fptrt);

if ( (fptrt=fopen("blank.dat","r"))==NULL ) {
  fprintf(stderr,"\ncannot open file of blanks \n");
  fclose(fptrs);
  exit(1);
}
while (!feof(fptrt) && (i<BLNKDEX)) {
  if ( (blnk[i]=(char *)malloc(31)) ==NULL ) {
    fprintf(stderr," Malloc for blnk[] when i = %d failed\n",i);
    fclose(fptrs);
    exit(1);
  }
  fgets(blnk[i],30,fptrt);
  strtok(blnk[i],"\n");
  i++;
}
i -= 2;
fprintf(fptrs," Blank table size is            %6d \n",i); 
n_blnk=i; 
fclose(fptrt);

return;
}


