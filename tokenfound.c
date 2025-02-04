#include <stdio.h>
#include <stdlib.h>
#include <string.h>                        
#include <ctype.h>

#define TRUE 1
#define FALSE 0

int tokenFound(char *token, char *tokenStringArray[])
{
   int i=0;
   int match=FALSE;
 
   while ((strcmp(*(tokenStringArray + i),"\0") != 0) && (!match)) {
      if (strcmp(token, *(tokenStringArray + i)) == 0) match = TRUE; 
      i++;
   }
   return match;
}  
