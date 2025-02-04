/****************************************************************/
/****************************************************************/
/*                                                              */
/*  search_master_clues searches the master clue table for	*/
/*  clue word is, clue_id, of clue type, clue_type.		*/
/*                                                              */
/****************************************************************/
/****************************************************************/
/*                                                              */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clues.h"

struct master *search_master_clues(clue_id,clue_type)
char *clue_type;
int clue_id;
{
    return (searchForMasterEntry(clue_id,clue_type));
}
