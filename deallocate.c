/****************************************************************/
/*                                                              */
/*  Function to free all hash tables/arrays.                    */
/*                                                              */
/****************************************************************/
/*                                                              */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_LINKS 50
#include "clues.h"
#include "tokens.h"
#include "patterns.h"

extern struct clue_table *entries[CLUE_ARRAYSIZE];
extern struct master *master_clues_au[MASTER_ARRAYSIZE_AU];
extern struct master *master_clues_bt[MASTER_ARRAYSIZE_BU];
extern struct master *master_clues_dr[MASTER_ARRAYSIZE_DR];
extern struct master *master_clues_ei[MASTER_ARRAYSIZE_EI];
extern struct master *master_clues_ex[MASTER_ARRAYSIZE_EX];
extern struct master *master_clues_mp[MASTER_ARRAYSIZE_MP];
extern struct master *master_clues_fc[MASTER_ARRAYSIZE_FC];
extern struct master *master_clues_ot[MASTER_ARRAYSIZE_OT];
extern struct master *master_clues_bx[MASTER_ARRAYSIZE_BX];
extern struct master *master_clues_rr[MASTER_ARRAYSIZE_RR];
extern struct master *master_clues_wd[MASTER_ARRAYSIZE_WD];
extern struct master *master_clues_wi[MASTER_ARRAYSIZE_WI];
extern struct master *master_clues_am[MASTER_ARRAYSIZE_AM];
extern struct master *master_clues_sa[MASTER_ARRAYSIZE_SA];
extern struct master *master_clues_ur[MASTER_ARRAYSIZE_UR];
extern struct master *master_clues_ty[MASTER_ARRAYSIZE_TY];
extern struct pattern_table *patterns[PATTERN_ARRAYSIZE];


void deallocate()
{
    int i,tot_links;
    struct clue_table *nxtcl,*tmp_clarr[MAX_LINKS];
    struct pattern_table *nxtpa,*tmp_paarr[MAX_LINKS];

/*
 *  Free clue table
 */
    for (i=0; i<CLUE_ARRAYSIZE; i++) {
        if (entries[i] != NULL) {
            tot_links = 0;
            for (nxtcl=entries[i]; nxtcl != NULL; nxtcl=nxtcl->next) {
                tmp_clarr[tot_links++] = nxtcl;
            }
            while (tot_links > 0) {
                free(tmp_clarr[tot_links-1]);
                tot_links--;
            }
            entries[i] = NULL;
        }
    }

/*
 *  Free pattern table
 */
    for (i=0; i<PATTERN_ARRAYSIZE; i++) {
        if (patterns[i] != NULL) {
            tot_links = 0;
            for (nxtpa=patterns[i]; nxtpa != NULL; nxtpa=nxtpa->next) {
                tmp_paarr[tot_links++] = nxtpa;
            }
            while (tot_links > 0) {
                free(tmp_paarr[tot_links-1]);
                tot_links--;
            }
            patterns[i] = NULL;
        }
    }
 
/*
 *  Free master clues
 */
    for (i=0; i<MASTER_ARRAYSIZE_TY; i++) {
        if (master_clues_ty[i] != NULL) free(master_clues_ty[i]);
        master_clues_ty[i] = NULL;
    }
    for (i=0; i<MASTER_ARRAYSIZE_AU; i++) {
        if (master_clues_au[i] != NULL) free(master_clues_au[i]);
        master_clues_au[i] = NULL;
    }
    for (i=0; i<MASTER_ARRAYSIZE_BU; i++) {
        if (master_clues_bt[i] != NULL) free(master_clues_bt[i]);
        master_clues_bt[i] = NULL;
    }
    for (i=0; i<MASTER_ARRAYSIZE_DR; i++) {
        if (master_clues_dr[i] != NULL) free(master_clues_dr[i]);
        master_clues_dr[i] = NULL;
    }
    for (i=0; i<MASTER_ARRAYSIZE_EI; i++) {
        if (master_clues_ei[i] != NULL) free(master_clues_ei[i]);
        master_clues_ei[i] = NULL;
    }
    for (i=0; i<MASTER_ARRAYSIZE_EX; i++) {
        if (master_clues_ex[i] != NULL) free(master_clues_ex[i]);
        master_clues_ex[i] = NULL;
    }
    for (i=0; i<MASTER_ARRAYSIZE_MP; i++) {
        if (master_clues_mp[i] != NULL) free(master_clues_mp[i]);
        master_clues_mp[i] = NULL;
    }
    for (i=0; i<MASTER_ARRAYSIZE_FC; i++) {
        if (master_clues_fc[i] != NULL) free(master_clues_fc[i]);
        master_clues_fc[i] = NULL;
    }
    for (i=0; i<MASTER_ARRAYSIZE_OT; i++) {
        if (master_clues_ot[i] != NULL) free(master_clues_ot[i]);
        master_clues_ot[i] = NULL;
    }
    for (i=0; i<MASTER_ARRAYSIZE_BX; i++) {
        if (master_clues_bx[i] != NULL) free(master_clues_bx[i]);
        master_clues_bx[i] = NULL;
    }
    for (i=0; i<MASTER_ARRAYSIZE_RR; i++) {
        if (master_clues_rr[i] != NULL) free(master_clues_rr[i]);
        master_clues_rr[i] = NULL;
    }
    for (i=0; i<MASTER_ARRAYSIZE_WD; i++) {
        if (master_clues_wd[i] != NULL) free(master_clues_wd[i]);
        master_clues_wd[i] = NULL;
    }
    for (i=0; i<MASTER_ARRAYSIZE_WI; i++) {
        if (master_clues_wi[i] != NULL) free(master_clues_wi[i]);
        master_clues_wi[i] = NULL;
    }
    for (i=0; i<MASTER_ARRAYSIZE_AM; i++) {
        if (master_clues_am[i] != NULL) free(master_clues_am[i]);
        master_clues_am[i] = NULL;
    }
    for (i=0; i<MASTER_ARRAYSIZE_SA; i++) {
        if (master_clues_sa[i] != NULL) free(master_clues_sa[i]);
        master_clues_sa[i] = NULL;
    }
    for (i=0; i<MASTER_ARRAYSIZE_UR; i++) {
        if (master_clues_ur[i] != NULL) free(master_clues_ur[i]);
        master_clues_ur[i] = NULL;
    }
    for (i=0; i<MASTER_ARRAYSIZE_BU; i++) {
        if (master_clues_bt[i] != NULL) free(master_clues_bt[i]);
        master_clues_bt[i] = NULL;
    }

}
