#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "clues.h"
#include "tokens.h"
#include "patterns.h"
#include "stand.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

static struct token_struct tokens[MAX_INPUT_TOKENS];
struct output_pattern return_patt[MAX_OUTPUT_PATTERNS];
struct output_pattern pattsfound[MAX_OUTPUT_PATTERNS];
char *adstan_version = "7.0";
int print_all_patterns;
int print_final_tokens;



struct standardized_address *Addr_Stan2(address_string,usf,abbrev_opt,
postal_state,zip_code)
char *address_string, usf, abbrev_opt, *postal_state, *zip_code;
{
    int numtok,numpatts,i,j,len,cnt=0;
    int build_patterns();
    char rec[300], colstr[MAX_ADDR_RECLEN], usage_flag;
    char *blanks="                                                          ";
    void *build_stand_address(),deallocate();
    static int first_call = TRUE;
    extern void buildPositionArray();
    struct posstr pa[MAX_OUTPUT_PATTERNS];
    static struct standardized_address *sa;
    static char prev_st[3],prtables[3][80],ustables[3][80];
    FILE *ifp,*ifp_pr;

    usage_flag = toupper(usf);
/*
 *  Initialization procedures must be done on first pass
 */
    if (first_call) {
        sa = (struct standardized_address *)
        malloc(sizeof(struct standardized_address));
/*
 *      Load file names of US tables
 */
        if ((ifp=fopen("initfiles.dat","r")) == NULL) {
            printf("No initialization file provided, ");
            printf("attempting to default to:\n \"initfiles.dat\"...\n");
            if ((ifp=fopen("initfiles.dat","r")) == NULL) {
                printf("Default initialization file, initfiles.dat not found!\n");
                assert(0);
            }
        }
        while (fgets(rec,sizeof(rec),ifp)) {
            strcpy(&ustables[cnt++][0],rec);
        }
        fclose(ifp);
/*
 *      Load file names of PR tables, if they exist
 */
        if ((ifp_pr=fopen("initfiles_pr.dat","r")) != NULL) {
            cnt = 0;
            while (fgets(rec,sizeof(rec),ifp_pr)) {
                strcpy(&prtables[cnt++][0],rec);
            }
            fclose(ifp_pr);
        }
    }

/*
 *  Check for a new state code
 */
    if (first_call || strncmp(prev_st,postal_state,2) != 0) {
        if (!first_call) deallocate();

        if (strncmp(postal_state,"72",2) != 0) {
            if (! load_cluewords(&ustables[0][0])) {
                printf ("ERROR:  clue words couldn't be loaded!!!\n");
                assert(0);
            }
            if (! load_master_clues(&ustables[1][0])) {
                printf ("ERROR:  master clues couldn't be loaded!!!\n");
                assert(0);
            }
            if (! load_patterns(&ustables[2][0])) {
                printf ("ERROR:  patterns couldn't be loaded!!!\n");
                assert(0);
            }
            putchar('\n');
        }
        else {
            if (! load_cluewords(&prtables[0][0])) {
                printf ("ERROR:  PR clue words couldn't be loaded!!!\n");
                assert(0);
            }
            if (! load_master_clues(&prtables[1][0])) {
                printf ("ERROR:  PR master clues couldn't be loaded!!!\n");
                assert(0);
            }
            if (! load_patterns(&prtables[2][0])) {
                printf ("ERROR:  PR patterns couldn't be loaded!!!\n");
                assert(0);
            }
            putchar('\n');
        }
    }

/*
 *  build the tokens
 */
    numtok = build_input_tokens(address_string,usage_flag,tokens,
    postal_state,zip_code);

/*
 *  find the best pattern
 */
    numpatts = build_patterns(tokens,numtok,pattsfound,return_patt,
    usage_flag,postal_state,zip_code);

/*
 *  print out token fields, in column format (if so desired)
 */
    if (print_final_tokens) {

       buildPositionArray(pa,return_patt,numpatts);

       for (i=0,j=0; i<numpatts; i++) {
           len = trimln((return_patt+(pa[i].order-1))->input_patt);
           sprintf(colstr+j,"%.*s  |  ",
           len,(return_patt+(pa[i].order-1))->input_patt);
           j += (len+5);
       }
       *(colstr+(j-3)) = '\0';
       putchar('\n');
       printf (" INPUT TOKENS  -  %s\n",colstr);

       for (i=0,j=0; i<numpatts; i++) {
           len = trimln((return_patt+(pa[i].order-1))->output_patt);
           sprintf(colstr+j,"%.*s  |  ",
           len,(return_patt+(pa[i].order-1))->output_patt);
           j += (len+5);
       }
       *(colstr+(j-3)) = '\0';
       printf ("OUTPUT TOKENS  -  %s\n",colstr);

       for (i=0,j=0; i<numpatts; i++) {
           len = trimln((return_patt+(pa[i].order-1))->output_patt);
           sprintf(colstr+j,"%.2s%.*s  |  ",
           (return_patt+(pa[i].order-1))->pattern_type,len-2,blanks);
           j += (len+5);
       }
       *(colstr+(j-3)) = '\0';
       printf ("PATTERN TYPES  -  %s\n",colstr);

       for (i=0,j=0; i<numpatts; i++) {
           len = trimln((return_patt+(pa[i].order-1))->output_patt);
           sprintf(colstr+j,"%.2s%.*s  |  ",
           (return_patt+(pa[i].order-1))->priority,len-2,blanks);
           j += (len+5);
       }
       *(colstr+(j-3)) = '\0';
       printf ("     PRIORITY  -  %s\n",colstr);
       printf ("\n");
    }

/*
 *  load the standardized address structure
 */
    build_stand_address(tokens,numtok,return_patt,numpatts,abbrev_opt,
    usage_flag,sa,postal_state,zip_code);

    strcpy(prev_st,postal_state);
    first_call = FALSE;
/*
 *  Return a pointer to the standardized structure
 */
    return(sa);
}
