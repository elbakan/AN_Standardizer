/****************************************************************/
/*                                                              */
/*  stan_type_decode searches the master clue table for         */
/*  the type id and returns the translation in the requested    */
/*  form.                                                       */
/*                                                              */
/*  Valid abbreviations:                                        */
/*      'F' - Full                                              */
/*      'S' - Standard                                          */
/*      'H' - Short                                             */
/*      'U' - USPS                                              */
/*                                                              */
/****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#include "clues.h"

int stan_type_decode(int id, char *name, char abb)
{
    static int first_call = TRUE;
    FILE *fp;
    char t_abb, rec[200];
    struct master *searchForMasterEntry(), *typeres;

    t_abb = toupper(abb);
/*
 *  Check to see if the master clue table was loaded
 */
    if (first_call && searchForMasterEntry(1,"DR") == NULL) {
        if ((fp=fopen("initfiles","r")) == NULL) {
            printf("No initialization file provided, ");
            printf("attempting to default to:\n \"initfiles.dat\"...\n");
            if ((fp=fopen("initfiles.dat","r")) == NULL) {
                printf("Default initialization file, initfiles.dat not found!\n");
                assert(0);
            }
        }
        else
            printf("Initialization file successfully opened.\n");

        fgets(rec,sizeof(rec),fp);
        fgets(rec,sizeof(rec),fp);
        if (! load_master_clues(rec)) {
            printf ("ERROR:  master clues couldn't be loaded!!!\n");
            assert(0);
        }

    }
    first_call = FALSE;
/*
 *  search for the master clue and return
 */
    if ( !(typeres = searchForMasterEntry(id,"TY")) )
        return(FALSE);
    else {
        switch (t_abb) {
            case 'F': strcpy(name,typeres->full_name); break;
            case 'S': strcpy(name,typeres->std_abbrev); break;
            case 'H': strcpy(name,typeres->short_abbrev); break;
            case 'U': strcpy(name,typeres->usps_abbrev); break;
            default:  printf ("ERROR:  Invalid abbreviation: %c\n",abb);
                      return(FALSE);
        }
    }
    return(TRUE);
}
