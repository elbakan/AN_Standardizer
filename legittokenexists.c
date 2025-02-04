#include <stdio.h>
#include <stdlib.h>
#include <string.h>                        
#include <ctype.h>
#include "clues.h"

#define TRUE 1
#define FALSE 0

extern char *legal_tokens_blank[];
extern char *legal_tokens_B[];
extern char *legal_tokens_T[];
extern char *legal_tokens_S[];
extern char *legal_tokens_W[];
extern char *legal_tokens_N[];
extern char *legal_tokens_R[];
char **tokenStringArray;

int legitTokenExists(char usage, struct clue_table *clue, int *clue_array)
{
   int i;
   int count=0;
   int match=FALSE;

   for (i=0;i<5;i++)
      *(clue_array + i) = 0;


   if (strncmp(clue->clue_type_1,"  ",2) == 0) *(clue_array + 0) = 1;
   if (strncmp(clue->clue_type_2,"  ",2) == 0) *(clue_array + 1) = 1;
   if (strncmp(clue->clue_type_3,"  ",2) == 0) *(clue_array + 2) = 1;
   if (strncmp(clue->clue_type_4,"  ",2) == 0) *(clue_array + 3) = 1;
   if (strncmp(clue->clue_type_5,"  ",2) == 0) *(clue_array + 4) = 1;

   /* Check to see if all clue_type are blanks, if true, then abort */ 
   for (i=0;i<5;i++)
      count = count + *(clue_array + i);
   if (count == 5) {
      printf ("Error!  All clue_type_* are blanks.  Aborted. \n");
      exit(0);
   }

   if (usage == ' ') tokenStringArray = legal_tokens_blank; 
   if (usage == 'B') tokenStringArray = legal_tokens_B; 
   if (usage == 'T') tokenStringArray = legal_tokens_T; 
   if (usage == 'S') tokenStringArray = legal_tokens_S; 
   if (usage == 'W') tokenStringArray = legal_tokens_W; 
   if (usage == 'N') tokenStringArray = legal_tokens_N; 
   if (usage == 'R') tokenStringArray = legal_tokens_R; 

   if ((strncmp(clue->clue_type_1,"  ",2) != 0) &&
       (tokenFound(clue->clue_type_1, tokenStringArray))) {
         match = TRUE;
         *(clue_array + 0) = 1; 
   }
   if ((strncmp(clue->clue_type_2,"  ",2) != 0) &&
       (tokenFound(clue->clue_type_2, tokenStringArray))) {
         match = TRUE;
         *(clue_array + 1) = 1; 
   }
   if ((strncmp(clue->clue_type_3,"  ",2) != 0) && 
       (tokenFound(clue->clue_type_3, tokenStringArray))) { 
         match = TRUE;
         *(clue_array + 2) = 1; 
   }
   if ((strncmp(clue->clue_type_4,"  ",2) != 0) && 
       (tokenFound(clue->clue_type_4, tokenStringArray))) { 
         match = TRUE;
         *(clue_array + 3) = 1; 
   }
   if ((strncmp(clue->clue_type_5,"  ",2) != 0) &&
       (tokenFound(clue->clue_type_5, tokenStringArray))) { 
         match = TRUE;
         *(clue_array + 4) = 1; 
   }
   return match;
}
