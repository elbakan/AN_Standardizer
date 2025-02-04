#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "standn.h"

/* name_split parses the charactor string into individual componetents */

extern	long	check_blank(long in_question);

void	name_split()
{
register	long	i,	j;
char		*p,	nameup1[60];
long		endchar;

p = namefld;
while (*p != '\0') {
  *p = (toupper(*p));
  p++;
}
p = namefld;
while (*p==' ') p++;	/* left justify */
strcpy(nameup1,"\0");
strcat(nameup1,p);
endchar = strlen(nameup1) - 1;

while (nameup1[endchar] == ' ')	endchar--;

for(i=0;i<=endchar;i++) {
  if (nameup1[i] != '.')  nameup[i] = nameup1[i];
  else nameup[i] = ' ';
}

nameup[(endchar + 1)] = '\0';
numwords = wdstrt[0] = wdend[0] = 0;

for (i=0;i<=endchar;i++) {
  if (isnotdelim(nameup[i]))
    wdend[numwords] = i;
  else if (isnotdelim(nameup[i+1])) {
    numwords++;
    wdstrt[numwords] = i + 1;
} }

if ((j=(wdend[numwords] - wdstrt[numwords]))> 1) {
  if (check_blank(j+1)) numwords--;
} else if (nameup[(wdstrt[numwords])]=='&') numwords--;

return;
}
