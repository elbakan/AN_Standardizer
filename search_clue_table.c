/****************************************************************/
/****************************************************************/
/*                                                              */
/*  search_clue_table searches the clue table for the string,	*/
/*  string, and returns a pointer to a clue_table struct if	*/
/*  it was found, or NULL if it wasn't found.			*/
/*                                                              */
/****************************************************************/
/****************************************************************/
/*                                                              */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clues.h"

struct clue_table *search_clue_table(string,state,zip)
char *string, *state, *zip;
{
    return(searchForClue(string,state,zip));
}
