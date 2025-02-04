/****************************************************************/
/*                                                              */
/*  decodeAddrField searches the master clue table for          */
/*  clue word id "id" of clue type "clue_type" and returns      */
/*  a pointer to the master clue structure, or NULL if not      */
/*  found.  Valid clue type's are:                              */
/*                                                              */
/*     TY - street types                                        */
/*     DR - directions                                          */
/*     WD - within structure descriptors                        */
/*     BU - property descriptions                               */
/*     RR - rural routes                                        */
/*     BX - boxes                                               */
/*     EX - street name extensions                              */
/*                                                              */
/*  The following may be helpful:                               */
/*                                                              */
/*  TO DECODE       USE CLUE TYPE                               */
/*  ---------       -------------                               */
/*    snpd_c             DR                                     */
/*    snpt_c             TY                                     */
/*    snst_c             TY                                     */
/*    snsd_c             DR                                     */
/*    wsd_c              WD                                     */
/*    ppd_c              DR                                     */
/*    ppt_c              BU                                     */
/*    pst_c              BU                                     */
/*    psd_c              DR                                     */
/*    rrd_c              RR                                     */
/*    bxd_c              BX                                     */
/*    sne_c              EX                                     */
/*                                                              */
/****************************************************************/
/*                                                              */

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

struct master *decodeAddrField(id,clue_type)
int id;
char *clue_type;
{
    static int first_call = TRUE;
    FILE *fp;
    char rec[200];

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
    return (searchForMasterEntry(id,clue_type));
}
