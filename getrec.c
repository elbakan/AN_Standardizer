#include <stdio.h>
int getrec(rec, len, file_ptr)

/*  Reads a record and pads it to length indicated by len.  Sets
everything to blank following the first encountered newline or
null.  Puts a null at end, but no newline.

len = number of characters in record, not including endline or null.
Example: if 80 characters in records stored in rec[0] thru rec[79],
send len = 80.  */

char *rec;
int len;
FILE *file_ptr;

{
   int j, k, next;
   next = fgets(rec, len+2, file_ptr);
   for (j=0; j<len; j++){
      if (rec[j] == 10 || rec[j] == 0) {
         for (k=j; k<len; k++) rec[k] = ' ';
         continue;
      }   
   }
   rec[len] = 0;   /* NULL */
   return next;
}
