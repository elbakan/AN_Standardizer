#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdio.h>
#include	"standn.h"

long	check_blank(in_question)
long	in_question;
{
char	str1[30];

str1[0] = 0;
strncat(str1,&nameup[(wdstrt[numwords])],in_question);
return(search_table(str1,1,n_blnk,in_question,blnk));
} 
