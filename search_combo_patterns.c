/****************************************************************/
/****************************************************************/
/*								*/
/*  search_combo_patterns is sent the array of pattern		*/
/*  structs (pattsfound), the number of sub-patterns found	*/
/*  in pattsfound (numpatts) and a pass value.			*/
/*  If pass == 1 (ie. the first time called), then any search	*/
/*  pattern is allowed, else any pattern string containing a	*/
/*  "+" is dis-allowed.						*/
/*								*/
/*  EXAMPLE:   given  123 87 MAIN AVE SOUTH:  the pattern types */
/*             found from searching are:  NU H* DR.		*/
/*             On the first pass we hope to find NU H*, hence,	*/
/*             the pattern types will then be:  H+ H* DR.	*/
/*             Subsequently, on the 2nd pass we hope to find:	*/
/*             H* DR.						*/
/*								*/
/*								*/
/****************************************************************/
/****************************************************************/

#include "tokens.h"
#include "patterns.h"
#include "stand.h"


#define TRUE 1
#define FALSE 0

struct posstr posarr[MAX_OUTPUT_PATTERNS];

void search_combo_patterns(pattsfound,numpatts,pass,usf,state,zip)
struct output_pattern *pattsfound;
int numpatts;
short pass;
char usf, *state, *zip;
{
    int j,i,starfound,pos,fr,to;
/*
    char string[200]; 
    char ip[200], op[200];
*/
    char string[MAX_PATT_SIZE]; 
    char ip[MAX_PATT_SIZE], op[MAX_PATT_SIZE];
    void buildStringFromPatterns();
    struct pattern_table *temp, *search_pattern_table();
    void buildPositionArray(), modifyStructure();

    for (fr=1, to=numpatts; (to-fr)>0; to--) {
/*
 *      A problem could potentially exist with order of elements in the array of
 *      structures:   the first element could contain tokens 2 - 4 and the
 *      second element could contain token 1.  We want these switched, hence,
 *      building a "position array" will resolve this.
 */
        buildPositionArray(posarr,pattsfound,numpatts);
/*
 *      First, make sure that at least 1 '*' token pattern exists
 */
        for (starfound=FALSE, j=fr; j <= to; j++) {
            pos = posarr[j-1].order - 1;
            if ((pattsfound+pos)->pattern_type[1] == '*') starfound = TRUE;
        }
        if (! starfound) return;
/*
 *      A problem could potentially exist with order of elements in the array of
 *      structures:   the first element could contain tokens 2 - 4 and the
 *      second element could contain token 1.  We want these switched, hence,
 *      building a "position array" will resolve this.
 */
        buildStringFromPatterns(string,pattsfound,fr,to);
/*
 *      FOR ALL PASSES:    Only search for the string if at least one "*" 
 *                         pattern exists in the current string.
 *      FOR THE 1st PASS:  patterns containing "+" pattern types ARE allowed
 *      FOR THE 2nd PASS:  patterns containing "+" pattern types ARE NOT allowed
 */

        if (strchr(string,'*') != NULL &&
        (pass == 1 ? TRUE : (strchr(string,'+') == NULL ? TRUE : FALSE)) &&
        (temp=search_pattern_table(string,ip,op,usf,state,zip)) != NULL) {
            modifyStructure(temp,pattsfound,ip,op,fr,to);
            break;
        }
        else if ((to-fr) <= 1) {
            fr++; to = numpatts+1;
        }
    }

}



/****************************************************************/
/*								*/
/*  buildStringFromPatterns will construct a character string   */
/*  from elements "fr" to "to" in the array of structs,		*/
/*  pattsfound.							*/
/*								*/
/****************************************************************/
void buildStringFromPatterns(string,pattsfound,fr,to)
struct output_pattern *pattsfound;
int fr,to;
char *string;
{
    int pos,i,len;

    for (i=0; fr <= to; fr++) {
/*
 *      "pos", calculated from the array posarr, is used to properly
 *      increment in the array of structs, pattsfound.
 */
        pos = posarr[fr-1].order - 1;
/*
 *      Determine length of sub-string
 */
        if ( (pattsfound+pos)->pattern_type[1] != '*' &&
        (pattsfound+pos)->pattern_type[1] != '+')
            len = ( ((pattsfound+pos)->end) - ((pattsfound+pos)->beg) +1) *3;
        else
            len = 3;
/*
 *      Add sub-string to string
 */
        sprintf(string+i,"%.*s ",len,
        ((pattsfound+pos)->pattern_type[1]=='*' || 
         (pattsfound+pos)->pattern_type[1]=='+') 
        ?
        ((pattsfound+pos)->pattern_type) 
        :
        ((pattsfound+pos)->input_patt));

        i += len;
    }
    *(string+(i-1)) = '\0';
}


/****************************************************************/
/*								*/
/*  modifyStructure will modify the proper element in the	*/
/*  pattsfound structure if a valid combination pattern was	*/
/*  found.							*/
/*								*/
/****************************************************************/
void modifyStructure(patt,pattsfound,ip,op,fr,to)
struct pattern_table *patt;
struct output_pattern *pattsfound;
char *ip, *op;
int fr,to;
{
   int pos=0,j;

/*
   printf ("ip: %s\n",ip);
   printf ("op: %s\n",op);
   printf ("fr: %d\n",fr);
   printf ("to: %d\n",to);
*/

   while ((to-fr)>=0) {
       j = posarr[fr-1].order - 1;
       if ( (pattsfound+j)->pattern_type[1] != '*') {
           sprintf((pattsfound+j)->pattern_type,"%.2s\0",patt->pattern_type);
           sprintf((pattsfound+j)->output_patt,"%.2s\0",op+pos);
           sprintf((pattsfound+j)->priority,"%.2s\0",patt->priority);
       }
       fr++; pos+=3;
   }
}



/****************************************************************/
/*								*/
/*  buildPositionArray loads the posarr structure, which is 	*/
/*  needed to retrieve the elements in the pattsfound array	*/
/*  of structure AS THEY ARE ORDERED IN THE ADDRESS STRING,	*/
/*  not as the were loaded.					*/
/*								*/
/****************************************************************/
void buildPositionArray(posarr,pattsfound,numpatts)
struct posstr *posarr;
struct output_pattern *pattsfound;
int numpatts;
{
    int hold,i,j,k=0,switchmade=TRUE;

/*
 *  Initialize all the beginning token and order values to 0 for
 *  all elements.
 */
    for (i=0; i<MAX_OUTPUT_PATTERNS; i++) {
        (posarr+i)->beg_tok = ((i<numpatts) ? ((pattsfound+i)->beg) : 0);
        (posarr+i)->order = ((i<numpatts) ? i+1 : 0);
    }

/*
 *  load the structure
 */
    while ( k < (numpatts -1) && switchmade) {
        switchmade = FALSE; k++;
        for (j=0; j < numpatts-k; j++) {
            if ((posarr+j)->beg_tok > (posarr+j+1)->beg_tok) {
                switchmade = TRUE;
                hold = (posarr+j)->beg_tok;
                (posarr+j)->beg_tok = (posarr+j+1)->beg_tok;
                (posarr+j+1)->beg_tok = hold;
                hold = (posarr+j)->order;
                (posarr+j)->order = (posarr+j+1)->order;
                (posarr+j+1)->order = hold;
            }
        }
    }

}
