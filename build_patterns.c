/****************************************************************/
/****************************************************************/
/*								*/
/*  build_patterns is sent the array of tokens structs (tst)	*/
/*  and the number of elements in tst (numtokens).  Returned	*/
/*  is 2 arrays of pattern structs;  pattsfound, containing	*/
/*  information about each pattern combination processed, and	*/
/*  return_patt, containing information on the winning pattern.	*/
/*								*/
/****************************************************************/
/****************************************************************/

#include "tokens.h"
#include "patterns.h"
#include "stand.h"

#if     !defined(TRUE) || ((TRUE) != 1)
#define TRUE    (1)
#endif
#if     !defined(FALSE) || ((FALSE) != 0)
#define FALSE   (0)
#endif

#define H_STAR_WEIGHT 1.0
#define T_STAR_WEIGHT 1.0
#define P_STAR_WEIGHT 1.0
#define R_STAR_WEIGHT 1.0
#define W_STAR_WEIGHT 0.9
#define B_STAR_WEIGHT 0.9
#define U_STAR_WEIGHT 0.9
#define H_PLUS_WEIGHT 0.4
#define B_PLUS_WEIGHT 0.6
#define P_PLUS_WEIGHT 0.6
#define R_PLUS_WEIGHT 0.4
#define W_PLUS_WEIGHT 0.4

short weight_scores = TRUE;
float unmatch_pts = 20.0;
int cur_clpos[MAX_INPUT_TOKENS+1];
int max_intoks[MAX_INPUT_TOKENS+1];
int enp, more_patts = TRUE;

const char *valid_output_token_types[NUMBER_OF_OUTPUT_TOKEN_TYPES] = {
"NB","BS","BN","DB","BD","TB","BT","BE","HN","HS","NA","SD","ST","PD",
"PT","EX","WD","WI","RN","RR","BX","XN","EI","H1","H2","P1","P2","1P",
"2P","B1","B2","BY","BI","NL"};

int build_patterns(tst,numtokens,pattsfound,return_patt,usf,state,zip)
struct token_struct *tst;
struct output_pattern *pattsfound, *return_patt;
int numtokens;
char usf, *state, *zip;
{
    int i,patts_matched,highest_score=(-10000),score=0,found9999;
    int return_numpatts,numpatts,processPattern(), calculateScore();
    short pass;
/*
    char nextpatt[MAX_PATT_SIZE];
*/
    char nextpatt[200];
    int nextPatternPermutation(); 
    char *tok_array[MAX_INPUT_TOKENS];
    void loadPositionArray(), buildTokenArray();
    void search_combo_patterns(), loadReturnPatternStruct();
    void checkForNL();

/*
 *  loadPositionArray will load the array max_intoks, which contains
 *  the number of input tokens for each clue in the pattern.
 *  EX:  123 main ave rt 4,  where "rt" has 2 tokens, HR and RR.
 */
    loadPositionArray(tst,numtokens);

    enp = numtokens-1;
    more_patts = TRUE;
/*
 *  nextPatternPermutation will return every possible pattern
 *  combination from tst.
 *  EX:  123 main st has 2 possible patterns:  NU AU TY and NU AU AU
 */

    while (nextPatternPermutation(nextpatt,tst,numtokens,&found9999)) {
/*
 *      If any clue word in the current pattern string has an id=9999
 *      (meaning this clue type is invalid for this usage), then continue
 */
        if (found9999) {
            continue;
        }
/*
 *      buildTokenArray loads tok_array, which is a simple array of
 *      input tokens for the current pattern
 */
        buildTokenArray(tok_array,nextpatt,numtokens);
/*
 *      process each pattern string
 */
        if ((numpatts=processPattern(tok_array,numtokens,pattsfound,usf,state,zip))==0){
            fprintf (stderr,"ERROR:  Nothing could be done with\n");
            fprintf (stderr,"the following pattern: %s\n",nextpatt);
            assert(0);
        }
/*
 *      check for a blowout
 */
        if (numpatts > MAX_OUTPUT_PATTERNS) {
            fprintf (stderr,"ERROR:  too many subpatterns were needed for\n");
            fprintf (stderr,"        the pattern: %s\n",nextpatt);
            fprintf (stderr,"        Max allowed: %d\n",MAX_OUTPUT_PATTERNS);
            assert(0);
        }
/*
 *      for each pattern string, call a function (search_combo_patterns)
 *      TWICE (hence giving it 2 chances to find combinations of 
 *      combination patterns) that will search for sub-pattern combinations,
 *      and a function (calculateScore) which calculates a score for the 
 *      current pattern string.
 */
        pass = 1; search_combo_patterns(pattsfound,numpatts,pass,usf,state,zip);
        pass = 2; search_combo_patterns(pattsfound,numpatts,pass,usf,state,zip);
/*
 *      Resolve NL tokens, if any
 */
        checkForNL(pattsfound,numpatts);
        score = calculateScore(pattsfound,numpatts);
/*
 *      Save the highest scoring pattern
 */
        if (score > highest_score) {
            highest_score = score;
            return_numpatts = numpatts;
            loadReturnPatternStruct(pattsfound,numpatts,return_patt);
        }

        if (print_all_patterns) {
            printf ("------------------------\n");
            printf ("%d patterns were found\n",numpatts);
            printf ("SCORE: %d\n",score);
            printf ("------------------------\n");
            for (i=0; i<numpatts; i++) {
                printf (" input_pattern: %s\n",(pattsfound+i)->input_patt);
                printf ("output_pattern: %s\n",(pattsfound+i)->output_patt);
                printf ("  pattern type: %s\n",(pattsfound+i)->pattern_type);
                printf ("      priority: %s\n",(pattsfound+i)->priority);
                printf ("           beg: %d\n",(pattsfound+i)->beg);
                printf ("           end: %d\n\n",(pattsfound+i)->end);
            }
        }
/*
 *      free token array
 */
        for (i=0; i<MAX_INPUT_TOKENS; i++) {
            free((char *)tok_array[i]);
        }

        if (score >= 90) break;
    }

/*
 *  return the total number of sub-patterns found in the winning
 *  pattern string
 */
    return(return_numpatts);
}



/****************************************************************/
/*								*/
/*  nextPatternPermutation will calculate the next possible	*/
/*  pattern string from tst, the array of token structs.	*/
/*								*/
/****************************************************************/
int nextPatternPermutation(pattstring,tst,numtok,found9999)
char *pattstring;
int numtok,*found9999;
struct token_struct *tst;
{
    int i,buildString();
    int lastPermutation(), back=1;

/*
 *  build the pattern string
 */
    if (! more_patts) return(FALSE);

    *found9999=buildString(pattstring,tst,numtok);
/*
 *  check for a blowout
 */
    if (strlen(pattstring) > MAX_PATT_SIZE) {
        fprintf (stderr,"ERROR:  pattern string is too long: %s\n",pattstring);
        fprintf (stderr,"        Max length is %d\n",MAX_PATT_SIZE);
        assert(0);
    }

    if (lastPermutation(numtok)) {
        more_patts = FALSE;
        return(TRUE);
    }
/*
 *  Calculate the next permutation.  The algorithm here is
 *  basically the same as nested do loops.
 */
    cur_clpos[enp]++;

    if (cur_clpos[enp] > max_intoks[enp]) {
        cur_clpos[enp]=1;
        if ((enp-back) < 0) {
            printf ("negative index: %d - %d\n",enp,back);
            assert(0);
        }
        cur_clpos[enp-back]++;
        while (cur_clpos[enp-back] > max_intoks[enp-back]) {
            cur_clpos[enp-back] = 1;
            back++;
            if ((enp-back) < 0) {
                more_patts = FALSE;
                break;
            }
            cur_clpos[enp-back]++;
        }
        back = 1;
    }

    return(TRUE);
}



/****************************************************************/
/*								*/
/*  loadPositionArray will loop through the array of token	*/
/*  structs, tst, and will load the integer token array,	*/
/*  max_intoks.							*/
/*								*/
/****************************************************************/
void loadPositionArray(tst,numtokens)
struct token_struct *tst;
int numtokens;
{
    int i;

    for (i=0; i<numtokens; i++, tst++) {
        if (strcmp(tst->clue_type_1,"  ")==0) max_intoks[i] = 0;
        else if (strcmp(tst->clue_type_2,"  ")==0) max_intoks[i] = 1;
        else if (strcmp(tst->clue_type_3,"  ")==0) max_intoks[i] = 2;
        else if (strcmp(tst->clue_type_4,"  ")==0) max_intoks[i] = 3;
        else if (strcmp(tst->clue_type_5,"  ")==0) max_intoks[i] = 4;
        else max_intoks[i] = 5;
    }
    for (i=0; i<MAX_INPUT_TOKENS; i++) cur_clpos[i] = 1;
}


/****************************************************************/
/*								*/
/*  This will return TRUE if all permutations of the input      */
/*  tokens have been processed.                                 */
/*								*/
/****************************************************************/
int lastPermutation(numtok)
int numtok;
{
    int i;

    for (i=0; i<numtok; i++) {
        if (cur_clpos[i] < max_intoks[i]) return(FALSE);
    }
    return(TRUE);
}




/****************************************************************/
/*								*/
/*  buildString is sent the will construct a character string	*/
/*  of input tokens from tst.					*/
/*  It will return TRUE if any tokens have id=9999              */
/*								*/
/****************************************************************/
int buildString(string,tst,numtok)
char *string;
struct token_struct *tst;
int numtok;
{
    int i,temp=FALSE;

    for (i=0; i<numtok; i++, tst++) {
        switch(cur_clpos[i]) {
            case 1: strncpy(string+(i*3),tst->clue_type_1,2); 
                    if (tst->clue_word_id1 == 9999) temp=TRUE; break;
            case 2: strncpy(string+(i*3),tst->clue_type_2,2); 
                    if (tst->clue_word_id2 == 9999) temp=TRUE; break;
            case 3: strncpy(string+(i*3),tst->clue_type_3,2); 
                    if (tst->clue_word_id3 == 9999) temp=TRUE; break;
            case 4: strncpy(string+(i*3),tst->clue_type_4,2); 
                    if (tst->clue_word_id4 == 9999) temp=TRUE; break;
            case 5: strncpy(string+(i*3),tst->clue_type_5,2); 
                    if (tst->clue_word_id5 == 9999) temp=TRUE; break;
        }
        *(string+(i*3)+2) = ' ';
    }
    strcpy(string+((numtok*3)-1),"\0");
    return (temp);
}


/****************************************************************/
/*								*/
/*  buildTokenArray will load the array of input tokens,	*/
/*  tok_array, from the pattern string patt.			*/
/*								*/
/****************************************************************/
void buildTokenArray(tok_array,patt,numtokens)
char **tok_array, *patt;
int numtokens;
{
    int i,pos;

    for (i=0, pos=0; i<MAX_INPUT_TOKENS; i++, pos+=3) {
       *(tok_array+i) = (char *) malloc(3);
       sprintf(*(tok_array+i),"%.2s\0",(i<numtokens?patt+pos:"  "));
    }
}



/****************************************************************/
/*								*/
/*  processPattern is the main function that processes each	*/
/*  pattern string, searching for combinations or whole pattern	*/
/*  strings in the pattern table.				*/
/*								*/
/****************************************************************/
int processPattern(tok_array,numtoks,pattsfound,usf,state,zip)
char **tok_array, usf, *state, *zip;
int numtoks;
struct output_pattern *pattsfound;
{
    static int priority_vals[]={80,40,0};
    int found=0,highpri=0,fr,to,minpri,i;
    int nextUnfoundPatternString();
    int out_toks_used[NUMBER_OF_OUTPUT_TOKEN_TYPES];
    struct pattern_table *ptrec, *findPatt();
    void loadOutputPatternStruct(), dirtyTokenArray();
    char ip[MAX_PATT_SIZE], op[MAX_PATT_SIZE];

/*
 *  initialize the "output tokens used" array
 */
    for (i=0; i<NUMBER_OF_OUTPUT_TOKEN_TYPES; i++)
        out_toks_used[i]=0;
/*
 *  First, search for the entire token array, calling findPatt
 *  up to 3 times; each time lowering the priority if nothing was
 *  found.
 */
    i=0;
    fr = 1; to = numtoks;
    while ((ptrec=
    findPatt(tok_array,ip,op,out_toks_used,&fr,&to,priority_vals[i],
    usf,state,zip)) == NULL) {
        if (priority_vals[i] == 0) break;
        fr = 1; to = numtoks;
        i++;
    }
/*
 *  set the highest priority,
 *  increment the number of patterns found counter, and
 *  load the output pattern struct
 */
    if (ptrec != NULL) {
        highpri = atoi(ptrec->priority);
        loadOutputPatternStruct(pattsfound,found,ip,op,ptrec,fr,to,tok_array);
        found++;
/*
        pattsfound++; found++;
*/
    }
/*
 *  Loop through the unfound token array, processing everything
 *  that wasnt found before.
 */
    while (nextUnfoundPatternString(tok_array,numtoks,&fr,&to)) {
        minpri = (highpri >= 60) ? 40 : 0;
/*
 *      Search for the next sub-pattern.  ONLY if findPatt returns
 *      true (ie. not NULL) you should check to re-set the highest
 *      priority found.
 */
        ptrec=findPatt(tok_array,ip,op,out_toks_used,&fr,&to,minpri,usf,state,zip);
        if (ptrec != NULL) {
            loadOutputPatternStruct(pattsfound,found,ip,op,ptrec,fr,to,tok_array);
            highpri = 
            (atoi(ptrec->priority)>highpri?atoi(ptrec->priority):highpri);
        }
        else {
/*
 *          Set to = fr because I only want to add single EI strings to
 *          the pattsfound structure
 */
            to = fr;
            loadOutputPatternStruct(pattsfound,found,ip,op,ptrec,fr,to,tok_array);
            dirtyTokenArray(tok_array,fr,to);
        }
        found++;
/*
        pattsfound++; found++;
*/
    }

    return(found);

}


/****************************************************************/
/*								*/
/*  findPatt is the main searching function that tries to find	*/
/*  any sub-pattern in the pattern table.			*/
/*								*/
/****************************************************************/
struct pattern_table *findPatt(tok_array,ip,op,out_toks_used,fr,to,minpri,
usf,state,zip)
char **tok_array;
char *ip, *op, usf, *state, *zip;
int *out_toks_used,*fr,*to,minpri;
{
    int i,gap,st,end,orig_gap,num_its;
    int outputTokAlreadyUsed();
    char string[MAX_PATT_SIZE];
    void buildStringFromTokenArray();
    struct pattern_table *temp, *search_pattern_table();
    void setOutputTokenAsUsed(), dirtyTokenArray();
/*
 *  The first pattern search (ie. when you are initially searching for
 *  the entire pattern) is different then subsequent searches.
 *
 *  Anytime a pattern is found, you must make sure that the following
 *  conditions are met:
 *      (1).  The priority is >= minpri
 *      (2).  The pattern type hasn't already been found.
 *
 *
 *  1ST CHECK:  Bias at the beginning of the pattern
 *  EXAMPLE:  Assume    123 main ave apt 2 a, then the first searches are:
 *                       1   2    3   4  5 6
 *                       1   2    3   4  5
 *                       1   2    3   4
 *                       1   2    3
 *                       1   2
 *                       1
 */
    for (st = *fr, end = *to; end - st >= 0; end--) {
        buildStringFromTokenArray(string,tok_array,st,end);
/*
        printf ("%d - %d   sending: %s, minpri: %d\n",st,end,string,minpri);
*/
        if (((temp=search_pattern_table(string,ip,op,usf,state,zip)) != NULL) &&
        (atoi(temp->priority) >= minpri) && (! outputTokAlreadyUsed(op,out_toks_used))) {
/*
 *          If its found, then set the dirty bit in out_toks_used, and
 *          return
 */
/*
            printf ("FOUND!!!\n");
*/
            setOutputTokenAsUsed(op,out_toks_used);
            dirtyTokenArray(tok_array,st,end);
            *fr = st; *to = end;
            return(temp);
        }
    }

/*
 *  2ND CHECK:  Bias at the end of the pattern
 *  EXAMPLE (from above):     2  3  4  5  6
 *                               3  4  5  6
 *                                  4  5  6
 *                                     5  6
 *                                        6
 */
    for (st = (*fr+1), end = *to; end - st >= 0; st++) {
        buildStringFromTokenArray(string,tok_array,st,end);
/*
        printf ("%d - %d   sending: %s, minpri: %d\n",st,end,string,minpri);
*/
        if (((temp=search_pattern_table(string,ip,op,usf,state,zip)) != NULL) &&
        (atoi(temp->priority) >= minpri) && (! outputTokAlreadyUsed(op,out_toks_used))) {
/*
 *          If its found, then set the dirty bit in out_toks_used, and
 *          return
 */
/*
            printf ("FOUND!!!\n");
*/
            setOutputTokenAsUsed(op,out_toks_used);
            dirtyTokenArray(tok_array,st,end);
            *fr = st; *to = end;
            return(temp);
        }
    }
/*
 *  FINAL CHECK:  eliminate the beginning and ending tokens, and
 *                search for the longest pattern in between.
 *
 *  EXAMPLE (from above):    2  3  4  5   search all "4"s
 *
 *                           2  3  4      search all "3"s
 *                              3  4  5
 *
 *                           2  3         search all "2"s
 *                              3  4
 *                                 4  5
 *
 *                           2            search all "1"s
 *                              3
 *                                 4
 *                                    5
 */
    end = (*to - 1);
    st = (*fr + 1);
    orig_gap = gap = end-st;
    while (gap >= 0) {
        num_its = (orig_gap - gap) + 1;
        for (i=0; i<num_its; i++) {
            buildStringFromTokenArray(string,tok_array,st+i,st+i+gap);
/*
            printf ("%d - %d   sending: %s, minpri: %d\n",st,end,string,minpri);
*/
            if (((temp=search_pattern_table(string,ip,op,usf,state,zip)) != NULL) &&
            (atoi(temp->priority) >= minpri) && (! outputTokAlreadyUsed(op,out_toks_used))) {
/*
 *              If its found, then set the dirty bit in out_toks_used, and
 *              return
 */
/*
                printf ("FOUND!!!\n");
*/
                setOutputTokenAsUsed(op,out_toks_used);
                dirtyTokenArray(tok_array,st+i,st+i+gap);
                *fr = st+i; *to = st+i+gap;
                return(temp);
            }
        }
        gap--;
    }
/*
 *  if nothing was found, then dirty the Token between the original
 *  starting and ending tokens and return NULL
 */
    return(NULL);

}


/****************************************************************/
/*								*/
/*  buildStringFromTokenArray will simply use the token array,	*/
/*  tok_array, to build a character string of input tokens	*/
/*  from "fr" to "to".						*/
/*								*/
/****************************************************************/
void buildStringFromTokenArray(string,tok_array,fr,to)
char **tok_array, *string;
int fr,to;
{
    int st,i;

    for (i=0, st=fr; st <= to; st++, i+=3) {
        sprintf(string+i,"%.2s\0",*(tok_array+(st-1)));
        *(string+i+2) = ' ';
    }
    *(string+(i-1)) = '\0';
}


/****************************************************************/
/*								*/
/*  nextUnfoundPatternString will use the token array,		*/
/*  tok_array, and determine the next consecutive set of	*/
/*  unused input tokens that haven't been used, and will	*/
/*  return their position in "from" and "to".			*/
/*								*/
/****************************************************************/
int nextUnfoundPatternString(tok_array,numtok,from,to)
char **tok_array;
int *from, *to, numtok;
{
    int i;

    *from = *to = 0;
    for (i=0; i<numtok; i++) {
        if (strncmp(*(tok_array+i),"  ",2)==0 ||
        strncmp(*(tok_array+i),"--",2)==0) {
            if (*from != 0)
                break;
            else
                continue;
        }
        if (*from == 0) *from = i+1;
        *to = i+1;
    }
    return ((*from == 0 || *to == 0) ? FALSE : TRUE);
}



/****************************************************************/
/*								*/
/*  dirtyTokenArray will retrieve the input tokens from "st"	*/
/*  to "end" in the token array, tok_array, and change their	*/
/*  values to "--", hence, signifying that they've already	*/
/*  been processed.						*/
/*								*/
/****************************************************************/
void dirtyTokenArray(tok_array,st,end)
char **tok_array;
int st,end;
{
    while (st <= end) {
        sprintf(*(tok_array+(st-1)),"%.2s\0","--");
        st++;
    }
}



/****************************************************************/
/*								*/
/*  setOutputTokenAsUsed receives an output token string that	*/
/*  was just found and will process each output token in the	*/
/*  string, and will "dirty" it's respective element in the	*/
/*  array, out_toks_used;  hence indicating that that output	*/
/*  token isn't allowed in anymore pattern searching.		*/
/*								*/
/****************************************************************/
void setOutputTokenAsUsed(string,out_toks_used)
char *string;
int *out_toks_used;
{
    int i,j,numpatts;

    numpatts = (trimln(string) + 1)/3;
    for (j=0; j<numpatts; j++) {
        for (i=0; i<NUMBER_OF_OUTPUT_TOKEN_TYPES; i++) {
            if (strncmp((string+(j*3)),*(valid_output_token_types+i),2) == 0) {
                *(out_toks_used+i) = 1;
                break;
            }
        }
    }
}



/****************************************************************/
/*								*/
/*  outputTokAlreadyUsed is an integer function that returns	*/
/*  TRUE if any output token in the string sent has already	*/
/*  been used, or found, in previous searches.			*/
/*								*/
/****************************************************************/
int outputTokAlreadyUsed(string,out_toks_used)
char *string;
int *out_toks_used;
{
    int i,j,numtokens;

    numtokens = (trimln(string) + 1)/3;
    for (j=0; j<numtokens; j++) {
        for (i=0; i<NUMBER_OF_OUTPUT_TOKEN_TYPES; i++) {
            if (strncmp((string+(j*3)),*(valid_output_token_types+i),2) == 0) {
                if (*(out_toks_used+i) == 1) return(TRUE);
                break;
            }
        }
    }
    return(FALSE);
}



/****************************************************************/
/*								*/
/*  loadOutputPatternStruct will load the array of pattern	*/
/*  structs, pattsfound, with its required information.		*/
/*								*/
/****************************************************************/
void loadOutputPatternStruct(pattsfound,found,ip,op,ptrec,fr,to,tok_array)
struct output_pattern *pattsfound;
int found, fr, to;
struct pattern_table *ptrec;
char **tok_array;
char *ip, *op;
{
    int i;
    char str[MAX_PATT_SIZE];
    void buildStringFromTokenArray();

/*
 *  If the pattern string from "fr" to "to" in the token array, tok_array,
 *  couldn't be found then the pattern is loaded as extra information
 */
/*
    printf ("ip: %s\n",ip);
    printf ("op: %s\n",op);
*/
    if (ptrec == NULL) {
        memset((pattsfound+found)->input_patt,' ',PATSIZE);
        memset((pattsfound+found)->output_patt,' ',PATSIZE);
        buildStringFromTokenArray(str,tok_array,fr,to);
        sprintf ((pattsfound+found)->input_patt,"%.*s\0",trimln(str),str);
        for (i=0; i<=(to-fr); i++) {
            sprintf(((pattsfound+found)->output_patt)+(3*i),"%.3s",
            (i==(to-fr) ? "EI\0" : "EI "));
        }
        sprintf((pattsfound+found)->pattern_type,"%.2s\0","EI");
        sprintf((pattsfound+found)->priority,"%.2s\0","30");
    }
/*
 *  else, the pattern WAS FOUND
 */
    else {
        memset((pattsfound+found)->input_patt,' ',PATSIZE);
        memset((pattsfound+found)->output_patt,' ',PATSIZE);
        sprintf((pattsfound+found)->input_patt,"%.*s\0",trimln(ip),ip);
        sprintf((pattsfound+found)->output_patt,"%.*s\0",trimln(op),op);
        sprintf((pattsfound+found)->pattern_type,"%.2s\0",ptrec->pattern_type);
        sprintf((pattsfound+found)->priority,"%.2s\0",ptrec->priority);
    }
    (pattsfound+found)->beg = fr;
    (pattsfound+found)->end = to;
}


/****************************************************************/
/*								*/
/*  calculateScore will determine a score for a pattern using	*/
/*  the following calculation:					*/
/*								*/
/*    score = avg. priority of all sub-patterns found		*/
/*            - (minus)						*/
/*            total number of unmatched tokens x PT		*/
/*								*/
/*            where PT = 0 for EI's embedded in patterns,	*/
/*            and PT = 20 for singular EI's.			*/
/*								*/
/****************************************************************/
int calculateScore(pattsfound, numpatts)
struct output_pattern *pattsfound;
int numpatts;
{
    int j,len,i,gap,avgpri,unmatchpatt=0,unmatchtok=0;
    int plusfound=FALSE, aufound=FALSE, longpatt=0;
    char *pos;
    char *pat_types[12]={"H*","T*","P*","R*","W*","B*","U*","H+","P+","R+","W+","B+"};
    float pat_weights[12]={H_STAR_WEIGHT,
                          T_STAR_WEIGHT,
                          P_STAR_WEIGHT,
                          R_STAR_WEIGHT,
                          W_STAR_WEIGHT,
                          B_STAR_WEIGHT,
                          U_STAR_WEIGHT,
                          H_PLUS_WEIGHT,
                          P_PLUS_WEIGHT,
                          R_PLUS_WEIGHT,
                          W_PLUS_WEIGHT,
                          B_PLUS_WEIGHT};
    float np=0.,weight_sum=0.,sweight=0.,totpri=0.;

    for (i=0; i<numpatts; i++) {
/*
 *      Dont average patterns where pattern type = "EI", but tally
 *      the number of unmatched patterns (AUs get tallied only once)
 */
        if (strncmp((pattsfound+i)->pattern_type,"EI",2) == 0) {
            if (strcmp((pattsfound+i)->input_patt,"AU") == 0) {
                if (!aufound) unmatchpatt++;
                aufound = TRUE;
            }
            else
                 unmatchpatt++;
            continue;
        }
/*
 *      Determine the weight for the pattern type
 */
        for (j=0; j<8; j++)
            if (strcmp((pattsfound+i)->pattern_type,pat_types[j])==0) break;

        sweight = pat_weights[j];
/*
 *      Dont average in the priority for the current sub-pattern if
 *      a "+" pattern type has already been averaged.
 */
        if ((pattsfound+i)->pattern_type[1] == '+') {
            if (plusfound) continue;
            plusfound=TRUE;
        }
      
        totpri += (atof((pattsfound+i)->priority) * (weight_scores ? sweight : 1.0) );
        weight_sum += sweight;
        np++;
/*
        printf ("input pattern: %s\n",(pattsfound+i)->input_patt);
        printf (" pattern type: %s\n",(pattsfound+i)->pattern_type);
        printf ("     priority: %s\n",(pattsfound+i)->priority);
        printf (" int priority: %f\n",atof((pattsfound+i)->priority));
        printf ("       weight: %f\n",sweight);
        printf ("       totpri: %f\n",totpri);
        printf ("   weight sum: %f\n",weight_sum);
*/
    }
    if (np == 0.) return(0);
    return (  ( (totpri/(weight_scores?weight_sum:np)) + 0.5)  - 
             ((float)unmatchpatt * unmatch_pts) );
}



/****************************************************************/
/*								*/
/*  loadReturnPatternStruct simply loads the pattern struct,	*/
/*  return_patt, using information from pattsfound, a		*/
/*  structure of the same type.					*/
/*								*/
/****************************************************************/
void loadReturnPatternStruct(pattsfound,numpatts,return_patt)
struct output_pattern *pattsfound, *return_patt;
int numpatts;
{
    int i;

    for (i=0; i<MAX_OUTPUT_PATTERNS; i++) {
/*
 *      Reset previously loaded info
 */
        memset((return_patt+i)->input_patt,' ',PATSIZE);
        memset((return_patt+i)->output_patt,' ',PATSIZE);
        memset((return_patt+i)->pattern_type,' ',3);
        memset((return_patt+i)->priority,' ',3);
        (return_patt+i)->beg = 0;
        (return_patt+i)->end = 0;
/*
 *      Load new info
 */
        if (i < numpatts) {
            strcpy((return_patt+i)->input_patt,(pattsfound+i)->input_patt);
            strcpy((return_patt+i)->output_patt,(pattsfound+i)->output_patt);
            strcpy((return_patt+i)->pattern_type,(pattsfound+i)->pattern_type);
            strcpy((return_patt+i)->priority,(pattsfound+i)->priority);
            (return_patt+i)->beg = (pattsfound+i)->beg;
            (return_patt+i)->end = (pattsfound+i)->end;
        }
    }
}



void checkForNL(pattsfound, numpatts)
struct output_pattern *pattsfound;
int numpatts;
{
    int i,j,k,numtoks;

    for (i=0; i<numpatts; i++) {
        if (strstr((pattsfound+i)->output_patt,"NL") == NULL) continue;

        numtoks = (trimln((pattsfound+i)->output_patt)+1)/3;
        for (j=0; j<numtoks; j++) {
            if (strncmp( ((pattsfound+i)->output_patt)+(j*3),"NL",2)==0) {
                for (k=j+1; k<numtoks; k++) {
                    if (strncmp( ((pattsfound+i)->output_patt)+(k*3),"NL",2)==0) continue;
                    if (strncmp( ((pattsfound+i)->output_patt)+(k*3),"NA",2)==0)
                        strncpy( ((pattsfound+i)->output_patt)+(j*3),"NA",2);
                    else if (strncmp( ((pattsfound+i)->output_patt)+(k*3),"BN",2)==0)
                        strncpy( ((pattsfound+i)->output_patt)+(j*3),"BN",2);
                    else
                        break;
                }
            }
        }
    }

}
