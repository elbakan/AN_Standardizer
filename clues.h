#ifndef CLUES__
#define CLUES__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define CLUE_ARRAYSIZE 2000         /*  max array size of clue table        */
#define CLUE_HASHSIZE 2000          /*  hash table size of clue table
                                        NOTE:  must be >= CLUE_ARRAYSIZE    */
#define CLUE_RECLEN 94              /*  record length of clue table record  */
#define MAX_ADDR_RECLEN 200         /*  maximum length of any address       */
#define MAX_CLUE_RECLEN 100         /*  maximum length of clue that is
                                        searched in the clue table          */


#define MASTER_ARRAYSIZE_TY 700     /*  master array size for types         */
#define MASTER_ARRAYSIZE_AU 500     /*  master array size for alphas        */
#define MASTER_ARRAYSIZE_BU 600     /*  master array size for properties    */
#define MASTER_ARRAYSIZE_DR 8       /*  master array size for directions    */
#define MASTER_ARRAYSIZE_EI 10      /*  master array size for extra info    */
#define MASTER_ARRAYSIZE_EX 10      /*  master array size for extensions    */
#define MASTER_ARRAYSIZE_MP 10      /*  master array size for mile posts    */
#define MASTER_ARRAYSIZE_FC 10      /*  master array size for fractions     */
#define MASTER_ARRAYSIZE_OT 4       /*  master array size for ordinal types */
#define MASTER_ARRAYSIZE_BX 50      /*  master array size for boxes         */
#define MASTER_ARRAYSIZE_RR 50      /*  master array size for rural routes  */
#define MASTER_ARRAYSIZE_WD 100     /*  master array size for within descr. */
#define MASTER_ARRAYSIZE_WI 100     /*  master array size for within ident. */
#define MASTER_ARRAYSIZE_AM 10      /*  master array size for ampersands    */
#define MASTER_ARRAYSIZE_SA 80      /*  master array size for state abbrevs */
#define MASTER_ARRAYSIZE_UR 80      /*  master array size for PR urbanizati */
                                    /*  urbanizations                       */

struct clue_table {
   char *name;                      /*  input name                          */
   char *translation;               /*  output name (translation)           */
   char usage_flag;                 /*  usage flag ('B'=com/res, ' '=both   */
   char language;                   /*  language                            */
   char *clue_word_id1;             /*  clue word id 1                      */
   char *clue_type_1;               /*  clue type 1                         */
   char *clue_word_id2;             /*  clue word id 2                      */
   char *clue_type_2;               /*  clue type 2                         */
   char *clue_word_id3;             /*  clue word id 3                      */
   char *clue_type_3;               /*  clue type 3                         */
   char *clue_word_id4;             /*  clue word id 4                      */
   char *clue_type_4;               /*  clue type 4                         */
   char *clue_word_id5;             /*  clue word id 5                      */
   char *clue_type_5;               /*  clue type 5                         */
   char *state;                     /*  state code                          */
   char *county;                    /*  county code                         */
   char *place;                     /*  place code                          */
   char *zip;                       /*  zip code                            */
   char *min_compar_score;          /*  minimum comparitor score            */
   char store_flag;                 /*  storage flag ('*' or ' ')           */
   struct clue_table *next;         /*  pointer to the next element in the
                                        linked list                         */
};
struct master {
   char *clue_word_id;              /*  clue word id                        */
   char *full_name;                 /*  full name                           */
   char *std_abbrev;                /*  standard abbreviation               */
   char *short_abbrev;              /*  short abbreviation                  */
   char *usps_abbrev;               /*  USPS abbreviation                   */
   char *valid_cfccs;               /*  CFCCs allowed (A,B,C,D,E,F,G,H)     */
   char *clue_type;                 /*  clue type                           */
   char usage_flag;                 /*  usage flag                          */
   char usps_flag;                  /*  usps flag                           */
   char for_lang_flag;              /*  foreign language flag               */
   char *translation;               /*  translation                         */
};


/*
 *  Declare external arrays
 */

/*
extern struct clue_table *entries[CLUE_ARRAYSIZE];
extern struct master *master_clues_ty[MASTER_ARRAYSIZE_TY];
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
*/

/*
 *  Declare external functions
 */

extern int trimln();
extern unsigned int clueHash();
extern char *stringdup();
extern struct clue_table *searchForClue();
extern struct clue_table *addClue();
extern struct master *searchForMasterEntry();


#endif
