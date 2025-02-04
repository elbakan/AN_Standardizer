#include <string.h>

/* search_table		preforms a binary search on a given table:

			It looks for the string "data" of length "length"
			in table "c_table" between subscripts "low"
			and "high".  It returns the subscript if it is
			found, otherwise it returns 0.

			BUGS: problems could arise if low is 0.

*/

long	search_table(data,low,high,length,c_table)
char	data[41];       /*search, data length*/
long	low,high,length;
char	*c_table[];
{
long mid=0;

while (low <= high) {
  mid = (low + high) / 2;
  if (!strncmp(data,c_table[mid],length)) return(mid);
	else if (strncmp(data,c_table[mid],length) < 0) high = mid - 1;
	else low = mid + 1;
}

return 0;
}

/* s_table		preforms a binary search on a given table:

			It looks for the string "data" of arbitrary length
			in table "c_table" between subscripts "low"
			and "high".  It returns the subscript if it is
			found, otherwise it returns 0.

			BUGS: problems could arise if low is 0.

*/


long	s_table(data,low,high,c_table)  /* search, no data length */
char	data[41];
long	low,high;
char	*c_table[];

{
long mid=0;

while (low <= high) {
  mid = (low + high) / 2;
  if (!strcmp(data,c_table[mid])) return(mid);
	else if (strcmp(data,c_table[mid]) < 0) high = mid - 1;
	else low = mid + 1;
}

return 0;
}


