#include <stdio.h>
int putrec(rec, len, file_ptr)

/*  Reads a record and pads it to length indicated by len.  Sets
everything to blank following the first encountered newline or
null.  Puts a newline and null at end.  rec should be dimensioned to
len + 2. 

len = number of characters in record, not including endline or null.
Example: if 80 characters in records stored in rec[0] thru rec[79],
send len = 80.  */

char *rec;
int len;
FILE *file_ptr;

{
   int j, k;
   for (j=0; j<len; j++){
      if (rec[j] == 10 || rec[j] == 0) {
         for (k=j; k<len; k++) rec[k] = ' ';
         continue;
      }   
   }
   rec[len] = '\n';   /* NEWLINE */
   rec[len+1] = 0;   /* NULL */
   fputs(rec, file_ptr);
   return 0;
}
