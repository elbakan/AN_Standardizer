/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                             *
 *  INT FUNCTION:  SQUASH(U,TYP)                               *
 *                                                             *
 *  Programmer:  Deborah Staples          Date:  07/13/92      *
 *                                                             *
 *  Description:                                               *
 *                                                             *
 *  SQUASH removes embedded blanks or collapses multiple       *
 *  embedded blanks from a character string.                   *
 *                                                             *
 *  Arguments:                                                 *
 *                                                             *
 *    Name   |  Length  |  Type  |  Use  |  Description        *
 *                                                             *
 *    U      |  1 word  |  char* |   B   |  Pointer to char    *
 *           |          |        |       |  array to squash    *
 *    TYP    |  1 word  |  int   |   I   |  Squash option      *
 *           |          |        |       |    1 = no embedded  *
 *           |          |        |       |        blanks       *
 *           |          |        |       |    2 = single       *
 *           |          |        |       |        blanks       *
 *                                                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <string.h>

int squish(char *u,int typ)
{
   int i,j,len;
   char t[512];

   if (typ != 1 && typ != 2)  {
       printf("\n%d %s",typ,"INVALID VALUE FOR TYP! USE 1 OR 2");
       return 0;
   }

   len = strlen(u);
   for (i=0;i<len;i++) *(t+i) = ' ';
   i = 0;  j = 0;

   while (*(u+i) != '\0')  {
       if (*(u+i) != ' ')  {
           *(t+j) = *(u+i);
           j++; i++;
       }
       else  {  
           while (*(u+i) == ' ')  i++;
           if (typ == 2)  i--;
           *(t+j) = *(u+i);
           j++; i++;          
       }
   }

   *(t+len) = '\0';
   strcpy(u,t);       
   return 1;
}
