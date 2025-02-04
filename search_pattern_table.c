/****************************************************************/
/****************************************************************/
/*                                                              */
/*  search_pattern_table searches the pattern table for the	*/
/*  pattern string, "string".					*/
/*                                                              */
/*  NOTE:  if consecutive AU input tokens are contained in the	*/
/*         pattern string, then they are squashed together	*/
/*         before searching.					*/
/*                                                              */
/****************************************************************/
/****************************************************************/
/*                                                              */

#include "patterns.h"
#include "tokens.h"
#define TRUE 1
#define FALSE 0

struct pattern_table *search_pattern_table(instring,ip,op,usf,state,zip)
char *instring, *ip, *op, usf, *state, *zip;
{
    int AUarr[MAX_INPUT_TOKENS], NLarr[MAX_INPUT_TOKENS];
    char srch[MAX_PATT_SIZE], hout[MAX_PATT_SIZE];
    char string[MAX_PATT_SIZE], t_op[MAX_PATT_SIZE];
    int i,j,tot_numintoks,numintoks,numouttoks,cnt=0,cnt2;
    int multiAUs=FALSE,foundNL=FALSE;
    struct pattern_table *temp, *retpat;

    for (i=0; i<MAX_INPUT_TOKENS; i++) {
        AUarr[i] = NLarr[i] = 0;
    }

    tot_numintoks = (strlen(instring)+1)/3;
/*
 *  Save copy of input string before you do anything
 */
    memset(ip,' ',MAX_PATT_SIZE);
    strcpy(ip,instring);
/*
 *  Search for NL (null) tokens
 */
    for (j=0; j<tot_numintoks; j++) {
        if (strncmp(instring+(j*3),"NL",2)==0) {
            NLarr[j] = 1;
            foundNL = TRUE;
        }
        else {
            sprintf (&string[cnt*3],"%.2s ",instring+(j*3));
            cnt++;
        }
    }
/*
 *  If all NLs then return
 */
    if (cnt == 0) return(NULL);

    numintoks = (strlen(string)+1)/3;
    string[((cnt-1)*3)+2] = '\0';
    sprintf (srch,"%.2s ",string);
    cnt = 0;
/*
 *  Collapse consecutive AUs
 */
    for (j=1; numintoks>1, j<numintoks; j++) {
        if ((strncmp(string+(j*3),"AU",2) == 0) &&
        (strncmp(string+(j*3),string+((j-1)*3),2) == 0)) {
            AUarr[cnt] += 1;
            multiAUs = TRUE;
            continue;
        }
        cnt++;
        sprintf (&srch[cnt*3],"%.2s ",string+(j*3));
    }
    srch[(cnt*3)+2] = '\0';
/*
 *  pattern string FOUND
 */
    if ((temp = searchForPattern(srch,state,zip,usf)) != NULL) {
        memset(op,' ',MAX_PATT_SIZE);
        memset(t_op,' ',MAX_PATT_SIZE);
        if (multiAUs) {
            numouttoks = cnt+1;
            cnt2 = 0;
            for (i=0; i<numouttoks; i++) {
                for (j=0; j<=AUarr[i]; j++) {
                    sprintf(hout+(cnt2*3),"%.2s ",temp->output_token_string+(i*3));
                    cnt2++;
                }
            }
            *(hout+((cnt2*3)-1)) = '\0';
            strcpy(t_op,hout);
        }
        else {
            strcpy(t_op,temp->output_token_string);
        }
/*
 *      Reload NLs if any were sent
 */
        cnt = 0;
        if (foundNL) {
            for (i=0; i<tot_numintoks; i++) {
                if (NLarr[i] == 1) {
                    sprintf (op+(i*3),"NL ");
                }
                else {
                    sprintf (op+(i*3),"%.2s ",&t_op[cnt*3]);
                    cnt++;
                }
            }
            op[i*3] = '\0';
        }
        else {
            strcpy(op,t_op);
        }

        *(op+strlen(ip)) = '\0';
        return(temp);

    }
/*
 *  pattern string NOT FOUND
 */
    else {
        return(NULL);
    }
}
