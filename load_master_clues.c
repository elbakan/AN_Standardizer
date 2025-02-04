/****************************************************************/
/****************************************************************/
/*                                                              */
/* load_master_clues loads the master clue table.		*/
/*                                                              */
/****************************************************************/
/****************************************************************/
/*                                                              */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clues.h"

/*
 * Declare an array of master structures for each input token
 * type possible
 */

struct master *master_clues_ty[MASTER_ARRAYSIZE_TY];
struct master *master_clues_au[MASTER_ARRAYSIZE_AU];
struct master *master_clues_bt[MASTER_ARRAYSIZE_BU];
struct master *master_clues_dr[MASTER_ARRAYSIZE_DR];
struct master *master_clues_ei[MASTER_ARRAYSIZE_EI];
struct master *master_clues_ex[MASTER_ARRAYSIZE_EX];
struct master *master_clues_mp[MASTER_ARRAYSIZE_MP];
struct master *master_clues_fc[MASTER_ARRAYSIZE_FC];
struct master *master_clues_ot[MASTER_ARRAYSIZE_OT];
struct master *master_clues_bx[MASTER_ARRAYSIZE_BX];
struct master *master_clues_rr[MASTER_ARRAYSIZE_RR];
struct master *master_clues_wd[MASTER_ARRAYSIZE_WD];
struct master *master_clues_wi[MASTER_ARRAYSIZE_WI];
struct master *master_clues_am[MASTER_ARRAYSIZE_AM];
struct master *master_clues_sa[MASTER_ARRAYSIZE_SA];
struct master *master_clues_ur[MASTER_ARRAYSIZE_UR];


int load_master_clues(filename)
char *filename;
{

    FILE *fp;
    char rec[100];
    struct master *tst;
    int cnt=0, tid, retval;

/*
 *  Open the file
 */
    if (strchr(filename,'\n') != NULL) *strchr(filename,'\n') = 0;
    if ((fp=fopen(filename,"r"))==NULL) {
        printf("fopen failed for master clues file: %s\n",filename);
        return(0);
    }
/*
 *  Load the table
 */
    while (fgets(rec,sizeof(rec),fp)) {
        cnt++;
        if (strchr(rec,'\n') != NULL) *strchr(rec,'\n') = 0;
 
        if ((tst = (struct master *) malloc(sizeof(*tst))) == NULL)
            abort();

        if ((tst->clue_word_id = (char *) malloc(5)) == NULL)
            abort();
        sprintf(tst->clue_word_id,"%.4s\0",rec);

        if ((tst->full_name = (char *) malloc(25)) == NULL)
            abort();
        sprintf(tst->full_name,"%.24s\0",&rec[4]);

        if ((tst->std_abbrev = (char *) malloc(15)) == NULL)
            abort();
        sprintf(tst->std_abbrev,"%.14s\0",&rec[28]);

        if ((tst->short_abbrev = (char *) malloc(6)) == NULL)
            abort();
        sprintf(tst->short_abbrev,"%.5s\0",&rec[42]);

        if ((tst->usps_abbrev = (char *) malloc(5)) == NULL)
            abort();
        sprintf(tst->usps_abbrev,"%.4s\0",&rec[48]);

        if ((tst->valid_cfccs = (char *) malloc(9)) == NULL)
            abort();
        sprintf(tst->valid_cfccs,"%.8s\0",&rec[52]);

        if ((tst->clue_type = (char *) malloc(3)) == NULL)
            abort();
        sprintf(tst->clue_type,"%.2s\0",&rec[60]);

        tst->usage_flag = rec[62];
        tst->usps_flag = rec[63];
        tst->for_lang_flag = rec[64];

        if ((tst->translation = (char *) malloc(25)) == NULL)
            abort();
        sprintf(tst->translation,"%.24s\0",&rec[65]);

        tid = atoi(tst->clue_word_id);

        if (strcmp(tst->clue_type,"TY") == 0) {
            if (tid > MASTER_ARRAYSIZE_TY) {
                fprintf (stderr,"ERROR:  trying to load too many TY master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_TY);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_ty[tid] = tst;
        }

        else if (strcmp(tst->clue_type,"AU") == 0) {
            if (tid > MASTER_ARRAYSIZE_AU) {
                fprintf (stderr,"ERROR:  trying to load too many AU master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_AU);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_au[tid] = tst;
        }

        else if (strcmp(tst->clue_type,"BU") == 0) {
            if (tid > MASTER_ARRAYSIZE_BU) {
                fprintf (stderr,"ERROR:  trying to load too many BU master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_BU);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_bt[tid] = tst;
        }

        else if (strcmp(tst->clue_type,"DR") == 0) {
            if (tid > MASTER_ARRAYSIZE_DR) {
                fprintf (stderr,"ERROR:  trying to load too many DR master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_DR);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_dr[tid] = tst;
        }

        else if (strcmp(tst->clue_type,"EI") == 0) {
            if (tid > MASTER_ARRAYSIZE_EI) {
                fprintf (stderr,"ERROR:  trying to load too many EI master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_EI);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_ei[tid] = tst;
        }

        else if (strcmp(tst->clue_type,"EX") == 0) {
            if (tid > MASTER_ARRAYSIZE_EX) {
                fprintf (stderr,"ERROR:  trying to load too many EX master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_EX);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_ex[tid] = tst;
        }

        else if (strcmp(tst->clue_type,"MP") == 0) {
            if (tid > MASTER_ARRAYSIZE_MP) {
                fprintf (stderr,"ERROR:  trying to load too many MP master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_MP);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_mp[tid] = tst;
        }

        else if (strcmp(tst->clue_type,"FC") == 0) {
            if (tid > MASTER_ARRAYSIZE_FC) {
                fprintf (stderr,"ERROR:  trying to load too many FC master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_FC);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_fc[tid] = tst;
        }

        else if (strcmp(tst->clue_type,"OT") == 0) {
            if (tid > MASTER_ARRAYSIZE_OT) {
                fprintf (stderr,"ERROR:  trying to load too many OT master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_OT);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_ot[tid] = tst;
        }

        else if (strcmp(tst->clue_type,"BX") == 0) {
            if (tid > MASTER_ARRAYSIZE_BX) {
                fprintf (stderr,"ERROR:  trying to load too many BX master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_BX);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_bx[tid] = tst;
        }

        else if (strcmp(tst->clue_type,"RR") == 0) {
            if (tid > MASTER_ARRAYSIZE_RR) {
                fprintf (stderr,"ERROR:  trying to load too many RR master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_RR);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_rr[tid] = tst;
        }

        else if (strcmp(tst->clue_type,"WD") == 0) {
            if (tid > MASTER_ARRAYSIZE_WD) {
                fprintf (stderr,"ERROR:  trying to load too many WD master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_WD);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_wd[tid] = tst;
        }

        else if (strcmp(tst->clue_type,"WI") == 0) {
            if (tid > MASTER_ARRAYSIZE_WI) {
                fprintf (stderr,"ERROR:  trying to load too many WI master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_WI);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_wi[tid] = tst;
        }

        else if (strcmp(tst->clue_type,"AM") == 0) {
            if (tid > MASTER_ARRAYSIZE_AM) {
                fprintf (stderr,"ERROR:  trying to load too many AM master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_AM);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_am[tid] = tst;
        }

        else if (strcmp(tst->clue_type,"SA") == 0) {
            if (tid > MASTER_ARRAYSIZE_SA) {
                fprintf (stderr,"ERROR:  trying to load too many SA master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_SA);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_sa[tid] = tst;
        }

        else if (strcmp(tst->clue_type,"UR") == 0) {
            if (tid > MASTER_ARRAYSIZE_UR) {
                fprintf (stderr,"ERROR:  trying to load too many UR master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_UR);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_ur[tid] = tst;
        }

/*
 *      HR's are treated the same as TY's
 */
        else if (strcmp(tst->clue_type,"HR") == 0) {
            if (tid > MASTER_ARRAYSIZE_TY) {
                fprintf (stderr,"ERROR:  trying to load too many TY master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_TY);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_ty[tid] = tst;
        }
/*
 *      BP's are treated the same as BU's
 */
        else if (strcmp(tst->clue_type,"BP") == 0) {
            if (tid > MASTER_ARRAYSIZE_BU) {
                fprintf (stderr,"ERROR:  trying to load too many BU master clues.\n");
                fprintf (stderr,"        Highest ID that can be loaded: %d\n",
                MASTER_ARRAYSIZE_BU);
                fprintf (stderr,"                    Trying to load ID: %d\n",tid);
                return(0);
            }
            else
                master_clues_bt[tid] = tst;
        }

        else {
            fprintf (stderr,"ERROR: clue type %s doesn't exist.\n",tst->clue_type);
            return(0);
        }
    }

    fclose(fp);
    return(1);
}



/****************************************************************/
/*                                                              */
/*  searchForMasterEntry returns a pointer to a master		*/
/*  structure if the clue word id, id, is for token type, type. */
/*                                                              */
/****************************************************************/
struct master *searchForMasterEntry(id,type)
int id;
char *type;
{
    if (strcmp(type,"TY") == 0) return (master_clues_ty[id]);
    else if (strcmp(type,"WD") == 0) return (master_clues_wd[id]);
    else if (strcmp(type,"WI") == 0) return (master_clues_wi[id]);
    else if (strcmp(type,"OT") == 0) return (master_clues_ot[id]);
    else if (strcmp(type,"RR") == 0) return (master_clues_rr[id]);
    else if (strcmp(type,"FC") == 0) return (master_clues_fc[id]);
    else if (strcmp(type,"BX") == 0) return (master_clues_bx[id]);
    else if (strcmp(type,"AU") == 0) return (master_clues_au[id]);
    else if (strcmp(type,"BU") == 0) return (master_clues_bt[id]);
    else if (strcmp(type,"DR") == 0) return (master_clues_dr[id]);
    else if (strcmp(type,"EI") == 0) return (master_clues_ei[id]);
    else if (strcmp(type,"EX") == 0) return (master_clues_ex[id]);
    else if (strcmp(type,"MP") == 0) return (master_clues_mp[id]);
    else if (strcmp(type,"AM") == 0) return (master_clues_am[id]);
    else if (strcmp(type,"SA") == 0) return (master_clues_sa[id]);
    else if (strcmp(type,"UR") == 0) return (master_clues_ur[id]);
    else if (strcmp(type,"HR") == 0) return (master_clues_ty[id]);
    else if (strcmp(type,"BP") == 0) return (master_clues_bt[id]);
    else if (strcmp(type,"PT") == 0) return (master_clues_ty[id]);
    else {
        fprintf (stderr,"ERROR: clue type %s doesn't exist.\n",type);
        return(0);
    }
}
