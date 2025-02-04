/****************************************************************/
/****************************************************************/
/*								*/
/*  build_input_tokens is sent an address string, and the	*/
/*  an array of token_struct's.  It will separate		*/
/*  the address string into words and will search for the	*/
/*  combinations of those words in the clues table.		*/
/*  Groups of tokens are calculated during this process.	*/
/*  It will load the tokens into an array of token_structs	*/
/*  and will return the number of elements in the array.	*/
/*								*/
/****************************************************************/
/****************************************************************/
/*								*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "clues.h"
#include "tokens.h"
#include "stand.h"

#if     !defined(TRUE) || ((TRUE) != 1)
#define TRUE    (1)
#endif
#if     !defined(FALSE) || ((FALSE) != 0)
#define FALSE   (0)
#endif

enum pad {spaces, slash, nothing};
const char special_characters[] = { '&', 
                                    '/',
                                    ',',
                                    '\'',
                                    '\0'};
int num_tokens_built;
short word_splits[MAX_WORDS],firstdash;

#define SZ_BLANK 23
#define SZ_B 9
#define SZ_T 13
#define SZ_S 15
#define SZ_W 6
#define SZ_N 5
#define SZ_R 6

const char *legal_tokens_blank[SZ_BLANK]={"NU","A1","AM","AU","BP","BU","BX",
"DR","EI","EX","FC","DA","HR","MP","NL","OT","PT","RR","SA","TY","WD","WI","\0"};
const char *legal_tokens_B[SZ_B]={"A1","AM","AU","BP","BU",
"DR","FC","OT","\0"};
const char *legal_tokens_T[SZ_T]={"A1","AM","AU",
"DR","EX","FC","HR","NL","OT","PT","SA","TY","\0"};
const char *legal_tokens_S[SZ_S]={"A1","AM","AU",
"DR","EX","FC","HR","NL","OT","PT","SA","TY",
"BP","BU","\0"};
const char *legal_tokens_W[SZ_W]={"AM","FC","OT","WI","WD","\0"};
const char *legal_tokens_N[SZ_N]={"NU","FC","DA","MP","\0"};
const char *legal_tokens_R[SZ_R]={"A1","AM","BX","FC","RR","\0"};


int build_input_tokens(addr_string,in_usage_flag,tokens_st,state,zip)
char *addr_string, in_usage_flag, *state, *zip;
struct token_struct *tokens_st;
{
    int numwords,endword,currword,tok_ind=0,sn,i,j,k,cword,cwordid,OTfound;
    int separateIntoWords(), validOTCombo(), validFraction(), dirNUcombo();
    int combo, loadTokenStruct(), legitA2exists(), legitA3exists();
    int status, clarr[5], legitTokenExists(),tst_loaded=FALSE;
    int unacceptableHRcombo();
    char *upCase(), *tokenifyWord();
    char new_addr_string[MAX_ADDR_RECLEN], ctype[3]; 
    char srchstr[MAX_ADDR_RECLEN], hold[MAX_ADDR_RECLEN], *pos;
    char *word_array[MAX_WORDS];
    struct clue_table *search_clue_table(), *clue;
    void initTokenStruct(), checkForANs(), switchTokStruct();
    void prepIncomingAddress(), combWords();

    sn = num_tokens_built = 0;
    for (i=0; i<MAX_WORDS; word_splits[i++] = 0) {}
/*
 *  Remove most special characters and collapse multiple spaces
 */
/*
    printf ("Address: %s\n",addr_string);
*/
    prepIncomingAddress(addr_string,new_addr_string,in_usage_flag);
/*
 *  Separate into an array of words
 */
    numwords = separateIntoWords(new_addr_string,word_array,in_usage_flag);
    if (numwords > MAX_WORDS) {
        fprintf (stderr,"ERROR:  address consists of %d words,\n",numwords);
        fprintf (stderr,"        which is more than the maximum number\n");
        fprintf (stderr,"        of words allowed (%ld)\n",MAX_WORDS);
        fprintf (stderr,"Address: %s\n",new_addr_string);
        assert(0);
    }
/*
    for (i=0; i<numwords; i++)
        printf ("word: %s\n",word_array[i]);
*/

/*
 *  This is the search pattern for a hypothetical address:
 *
 *  (1) (2)  (3)   (4)
 *  123  N  MAINE  AVE
 *
 *  a.  1,2,3,4
 *  b.    2,3,4
 *  c.      3,4
 *  d.        4
 *
 *  e.    1,2,3
 *  f.      2,3
 *  g.        3
 *
 *  h.      1,2
 *  i.        2
 *
 *  j.        1
 *
 *  NOTE:  This sequence assumes that a combination of words
 *         was never found in the clues table, AND
 *         that the maximum number of words in a clue isn't
 *         greater than 4.  If it was, say, 3 then the first
 *         search pattern above would be:   2,3,4
 *					      3,4
 *					        4
 *
 *
 *  VARIABLES
 *  ---------
 *
 *   endword:   The last word in a pattern search.
 *              In the example above, the endword for e-g is
 *              MAINE
 *  currword:   The first word in a pattern search.
 *              In the example above, currwords are
 *              a. 1    b. 2    c. 3    d. 4
 *
 */
    endword = numwords;
    while (endword>0) {
        currword = (endword<MAX_WORDSINCLUE)?1:(endword-MAX_WORDSINCLUE)+1;
        for (; currword <= endword;  currword++) {

            combWords(srchstr,&word_array[currword-1],currword,endword,spaces);
/*
            printf ("Search: %s, currword-%ld  endword-%ld\n",
            srchstr,currword,endword);
*/
            if ((clue=search_clue_table(upCase(srchstr,hold),state,zip)) 
            != NULL) {
/*
 *              Whenever a clue is found, make sure its token type is
 *              legitimate for the current usage flag
 */
                if (!legitTokenExists(in_usage_flag,clue,clarr)) {
                    clue = NULL;
                    continue;
                }
                if (unacceptableHRcombo(&word_array[currword-1],clue,
                currword,endword,numwords,state,zip)) continue;
                break;

            }

        }
        if (currword > endword) currword = endword;
        initTokenStruct(tokens_st);
        OTfound = FALSE;
/*
        printf ("    Break for currword-%ld  endword-%ld\n",
        currword,endword);
*/
/*
 *      Current word WAS NOT found
 */
        if (clue == NULL) {
            if (in_usage_flag == 'N' && legitA3exists(srchstr)) {
                combo=loadTokenStruct(srchstr,tokens_st,"A3",1,0,currword,
                endword);
                k=2;
            }
            else if (legitA2exists(srchstr)) {
                combo=loadTokenStruct(srchstr,tokens_st,"A2",1,0,currword,
                endword);
                k=2;
            }
            else
                k=1;

            combo=
            loadTokenStruct(srchstr,tokens_st,tokenifyWord(srchstr),k,0,
            currword,endword);
            if (combo) tokens_st--;
            tst_loaded = TRUE;
            endword--;
        }
/*
 *      Current word WAS found
 */
        else {
            if ((currword > 1) && 
            (validOTCombo(&word_array[currword-1],tokens_st-1,tst_loaded))) {
                OTfound = TRUE;
/*
 *              Combine the OT (previous word + current word)
 *              before placing it into the input_image
 */
                combWords(srchstr,&word_array[currword-2],currword-1,
                currword,nothing);
                combo=loadTokenStruct(srchstr,tokens_st,"AU",1,0,
                currword-1,endword);
                if (in_usage_flag != 'T' || in_usage_flag != 'S') 
                    combo=loadTokenStruct(srchstr,tokens_st,"OT",2,0,currword-1,
                    endword);

                tst_loaded = TRUE;
                endword -= 2;    /*  shift over an extra word	*/
				 /*  to account for the OT	*/ 
            } 
            else if ((in_usage_flag != 'T' || in_usage_flag != 'S' || 
            in_usage_flag != ' ') && dirNUcombo(srchstr,currword)) {
                combo=loadTokenStruct(srchstr,tokens_st,"A1",1,0,
                currword,endword);
                if (combo) tokens_st--;
                combo=loadTokenStruct(srchstr,tokens_st,"D1",2,0,
                currword,endword);
                if (combo) tokens_st--;
                tst_loaded = TRUE;
                endword--;
            }
            else if ((validFraction(&word_array[currword-1],currword,
            tst_loaded,tokens_st-1,clue->clue_type_1))) {
/*
 *              Combine the fraction with the previous word in the word
 *              array before placing it into the input_image
 */
                strcpy(srchstr,word_array[currword-2]);
                strcat(srchstr," ");
                strcat(srchstr,clue->translation);
                combo=loadTokenStruct(srchstr,tokens_st,"NU",1,0,
                currword-1,endword);
                if (combo) tokens_st--;
                tst_loaded = TRUE;
                endword -= 4;    /*  shift over 4 words   */

            }
            else {
/*
 *              Load the token struct with all tokens returned
 *              from the clue word abbreviation struct, EXCEPT
 *              if any clue word type is OT (we already handled
 *              those cases).
 *
 *              Also, if the current clue has only partially 
 *              acceptable tokens, the make the clue word id's for
 *              the invalid one be 9999.
 */

                for (cwordid=1, i=1; i<=5; i++) {
                    switch (i) {
                      case 1: 
                        cword = (clarr[i-1]==1?atoi(clue->clue_word_id1):9999);
                        sprintf(ctype,"%.2s\0",clue->clue_type_1);
                        break;
                      case 2: 
                        cword = (clarr[i-1]==1?atoi(clue->clue_word_id2):9999);
                        sprintf(ctype,"%.2s\0",clue->clue_type_2);
                        break;
                      case 3: 
                        cword = (clarr[i-1]==1?atoi(clue->clue_word_id3):9999);
                        sprintf(ctype,"%.2s\0",clue->clue_type_3);
                        break;
                      case 4: 
                        cword = (clarr[i-1]==1?atoi(clue->clue_word_id4):9999);
                        sprintf(ctype,"%.2s\0",clue->clue_type_4);
                        break;
                      case 5: 
                        cword = (clarr[i-1]==1?atoi(clue->clue_word_id5):9999);
                        sprintf(ctype,"%.2s\0",clue->clue_type_5);
                        break;
                    }
/*
 *                  Break when there's no more clues.
 *                  Continue if an OT has been found, because we've
 *                  already handled them, UNLESS the variable OTfound is TRUE
 *                  (meaning we've found a valid OT for this clue and it was
 *                  already combined, hence, we don't need the OT token).
 */
                    if (cword==0 && strncmp(ctype,"  ",2)==0)
                        break;
                    if (strncmp(ctype,"OT",2) == 0 && OTfound)
                        continue;

                    combo=loadTokenStruct(srchstr,tokens_st,ctype,cwordid,cword,
                    currword,endword);
                    if (combo) tokens_st--;
                    tst_loaded = TRUE;
                    cwordid++;

                }
                endword = currword -1;
            }
        }
/*
 *      Increment the counter for the number of tokens
 *      Increment the pointer to the array of token structs
 */
        num_tokens_built++;
/*
 *      check for a blowout
 */
        if (num_tokens_built > MAX_INPUT_TOKENS) {
            fprintf (stderr,"ERROR: trying to build more than %ld tokens.\n",
            MAX_INPUT_TOKENS);
            fprintf (stderr,"Address: %s\n",new_addr_string);
            assert(0);
        }
        tokens_st++;

    }   /*  end while  */

/*
 *  Free the word_array
 */
    for (i=0; i<numwords; i++) {
        free(word_array[i]);
    }
    tokens_st -= num_tokens_built;

/*
 *  Reverse the token array (the tokens were loaded in reverse order)
 */
    for (i=num_tokens_built/2, j=i-((num_tokens_built+1)%2); i<num_tokens_built; i++,j--) {
        switchTokStruct(tokens_st+i,tokens_st+j);
    }
    sn =num_tokens_built;
/*
 *  Check for AN's
 */

/*
    printf ("--------------------------------------------------\n");
    printf ("Num tokens: %ld\n",sn);
    for (i=0; i<=sn; i++) {
        printf ("image:  %s\n",(tokens_st+i)->input_image);
        printf ("types:  %s,%s,%s,%s,%s\n",
        (tokens_st+i)->clue_type_1,
        (tokens_st+i)->clue_type_2,
        (tokens_st+i)->clue_type_3,
        (tokens_st+i)->clue_type_4,
        (tokens_st+i)->clue_type_5);
        printf ("  ids:  %ld,%ld,%ld,%ld,%ld\n",
        (tokens_st+i)->clue_word_id1,
        (tokens_st+i)->clue_word_id2,
        (tokens_st+i)->clue_word_id3,
        (tokens_st+i)->clue_word_id4,
        (tokens_st+i)->clue_word_id5);
        printf ("  beg:  %ld\n",(tokens_st+i)->beg);
        printf ("  end:  %ld\n\n",(tokens_st+i)->end);
    }
    printf ("--------------------------------------------------\n");
*/

    checkForANs(tokens_st,&num_tokens_built);

/*
    printf ("Num tokens: %ld\n",num_tokens_built);
    for (i=0; i<=sn; i++) {
        printf ("image:  %s\n",(tokens_st+i)->input_image);
        printf ("types:  %s,%s,%s,%s,%s\n",
        (tokens_st+i)->clue_type_1,
        (tokens_st+i)->clue_type_2,
        (tokens_st+i)->clue_type_3,
        (tokens_st+i)->clue_type_4,
        (tokens_st+i)->clue_type_5);
        printf ("  ids:  %d,%d,%d,%d,%d\n",
        (tokens_st+i)->clue_word_id1,
        (tokens_st+i)->clue_word_id2,
        (tokens_st+i)->clue_word_id3,
        (tokens_st+i)->clue_word_id4,
        (tokens_st+i)->clue_word_id5);
        printf ("  beg:  %d\n",(tokens_st+i)->beg);
        printf ("  end:  %d\n\n",(tokens_st+i)->end);
    }
    printf ("--------------------------------------------------\n");
*/

    return(num_tokens_built);
}




/****************************************************************/
/*								*/
/*  separateIntoWords is sent the prepped address string and	*/
/*  an array of pointers to chars.  The array is filled with	*/
/*  words delineated by spaces, special characters, or changes	*/
/*  from alpha to digit, or vica versa.  The one exception	*/
/*  to the last statement is "1st" and "3rd".			*/
/*  For example, if "123 1st" is the incoming address, then	*/
/*  we are pretty sure that "1st" is the street name, hence	*/
/*  we never want to split "1" and "st", which could 		*/
/*  subsequently identify "st" as a type.			*/
/*								*/
/****************************************************************/

int separateIntoWords(str,word_array,usf)
char *str, **word_array, usf;
{
    char *newstring, prevch;
    int len,i,st,numwrds,alphanum,numalpha,isSpecialChar();
    extern int trimln();

/*
 *  For HN only addresses, if the first non-blank char found was a dash
 *  then make the 1st word in the array be "-".
 */
    st = 0; numwrds = 0;
    if (firstdash) {
        *word_array = (char *) malloc(2);
        sprintf(*word_array++,"%.2s\0","-");
        numwrds++;
    }

    len = trimln(str);
    for (i=0 ; i<len ; str++, i++) {
        if (i==0) {
            prevch = *str;
            continue;
        }
        alphanum = (isalpha(prevch) && isdigit(*str)) ? TRUE : FALSE;
        numalpha = (isalpha(*str) && isdigit(prevch)) ? TRUE : FALSE;
        if (
        (*str == ' ') ||
        (isSpecialChar(*str)) ||
        (isSpecialChar(prevch)) ||
        (alphanum) ||
        (numalpha && 
        (strncmp(str-1,"1st",3)!=0 && strncmp(str-1,"3rd",3)!=0) &&
        (strncmp(str-1,"1ST",3)!=0 && strncmp(str-1,"3RD",3)!=0))
        ) {
            if (!isSpecialChar(*str) || (i!=st)) {
               *word_array = (char *) malloc((i-st)+1);
               sprintf(*word_array++,"%.*s\0",i-st,str-(i-st));
               if (alphanum || numalpha) {
                   if (word_splits[numwrds] == 0)
                       word_splits[numwrds]=1;
                   else
                       word_splits[numwrds]=2;

                   word_splits[numwrds+1] = 1;
               }
               numwrds++;
            }
            st = (*str == ' ')?i+1:i;
        }
        prevch = *str;
    }
    *word_array = (char *) malloc((i-st)+1);
    sprintf(*word_array++,"%.*s\0",i-st,str-(i-st));
    numwrds++;

    return(numwrds);
}



/****************************************************************/
/*								*/
/*  prepIncomingAddress will receive the address string and	*/
/*  will remove all special characters, except one in the	*/
/*  "special_characters" array, and will squash any consecutive	*/
/*  blanks together.						*/
/*								*/
/****************************************************************/

void prepIncomingAddress(str,newstr,usf)
char *str, *newstr, usf;
{
    int pblank,cnt=0,len;
    int dashFound(),isSpecialChar();

    len = strlen(str);
/*
 *  check for a blowout!
 */
    if (len > MAX_ADDR_RECLEN) {
        fprintf (stderr,"ERROR:  address is longer than %d chars.\n",
        MAX_ADDR_RECLEN);
        fprintf (stderr,"Address: %s\n",str);
        fprintf (stderr," Length: %ld\n",strlen(str));
        assert(0);
    }
/*
 *  If were doing house number only (N) and the first char is a
 *  dash, then blank it, but set the "firstdash" variable to TRUE
 */
    firstdash = ( (usf=='N' && dashFound(str,len)) ? TRUE : FALSE);
    
    pblank = (*str == ' ' || (!isalnum(*str) && !isSpecialChar(*str)) ) 
    ? TRUE : FALSE;
    while (*str != '\0') {
/*
 *      skip any apostrophes
 */
        if (*str == '\'') {
            str++;
            continue;
        }
/*
 *      only accept '/' for potential fractions, or s/c
 *             prev. char must be 1 or 3, and
 *             next  char must be 2, 3, 4, or 8
 */
        if (*str == '/') {
            if ( (!isdigit(*(str-1)) && *(str-1)!='S' && *(str-1)!='s') ||
            (!isdigit(*(str+1)) && *(str+1)!='C' && *(str+1)!='c') ) *str = ' ';
        }
/*
        if (*str == '/') {
            if ( (*(str-1)!='1' && *(str-1)!= '3' && *(str-1)!='S' && *(str-1)!='s') ||
            (*(str+1) != '2' && *(str+1) != '3' && *(str+1) != 'C' && *(str+1) != 'c' &&
            *(str+1) != '4' && *(str+1) != '8') ) *str = ' ';
        }
*/

        if (!isalnum(*str) && !isSpecialChar(*str))
            *str = ' ';

        if (*str==' ' && pblank) {
            str++;
            continue;
        }
        if (usf == 'N' && *str == ' ') {   /*  for HN only, squash "123 e" together  */
           if ( (isdigit(*(str-1)) && isalpha(*(str+1))) ||
           (isdigit(*(str+1)) && isalpha(*(str-1))) ) {
               str++;
               continue;
           }
        }  
        *newstr = *str; 
        pblank = *newstr == ' ' ? TRUE : FALSE; 
        newstr++; str++; cnt++;
    }

    *newstr = *str;
}


/****************************************************************/
/*								*/
/*  isSpecialChar is will return true if the char sent is a	*/
/*  special character which can NOT be removed from the		*/
/*  address.							*/
/*								*/
/****************************************************************/

int isSpecialChar(c)
char c;
{
    int i=0;

    while (special_characters[i] != '\0') {
        if (special_characters[i] == c) return (TRUE);
        i++;
    }
    return (FALSE);
}


/****************************************************************/
/*								*/
/*  upCase will set all lower case characters in "str" to	*/
/*  upper case and will return a copy of str, leaving the	*/
/*  original unchanged.						*/
/*								*/
/****************************************************************/

char *upCase(str,out)
char *str, *out;
{
    int len=0;

    strcpy(out,str);
    while (*out != '\0') {
        if (islower(*out)) *out = toupper(*out);
        out++; len++;
    }
    return(out-len);
}

char *lowCase(str,out)
char *str, *out;
{
    int len=0;

    strcpy(out,str);
    while (*out != '\0') {
        if (isupper(*out)) *out = tolower(*out);
        out++; len++;
    }
    return(out-len);
}



/****************************************************************/
/*								*/
/*  combWords receives the word array, a starting point, and	*/
/*  an ending point and will combine the words in that range	*/
/*  into 1 string.						*/
/*								*/
/****************************************************************/

void combWords(str,array,st,end,combine)
char **array;
int st,end,combine;
char *str;
{
    int j,i,len=0;

/*
 *  Adjust st and end because arrays start with 0
 */
    st -= 1;  end -= 1;
    for (i=st, j=0; i<=end; i++, j++)
        len += strlen(*(array+j));
    
    for (i=st, j=0; i<=end; i++, j++) {
        if (i==st) strcpy(str,*(array+j));
        else strcat(str,*(array+j));
        if (i != end && (combine == spaces)) strcat(str," ");
        if (i != end && (combine == slash)) strcat(str,"/");
    }
}


/****************************************************************/
/*								*/
/*  tokenifyWord is sent a word and returns the			*/
/*  token (NU, AU, or A1) if it wasn't found in			*/
/*  the clues table						*/
/*								*/
/****************************************************************/

char *tokenifyWord(str)
char *str;
{
    char temp[3];
    int len,i,alphaex=FALSE, digitex=FALSE;

    len = strlen(str);
    if ((len == 1) && ( (isalpha(*str)) || *str == '-') )
        return("A1");

    for (i=0; i<len; i++, str++) {
        if (isdigit(*str))
            digitex = TRUE;
        else
            alphaex = TRUE;
    }

    return(alphaex ? "AU" : "NU");
}



/****************************************************************/
/*								*/
/*  legitA2exists is sent a string and will return TRUE if	*/
/*  the string contains only 2 alphas (A2), and FALSE otherwise */
/*								*/
/****************************************************************/

int legitA2exists(str)
char *str;
{
    int i;

    if (strlen(str) != 2) return(FALSE);

    for (i=0; i<2; i++) {
        if (!isalpha(*(str+i))) return(FALSE);
    }
    return(TRUE);
}



/****************************************************************/
/*								*/
/*  legitA3exists is sent a string and will return TRUE if	*/
/*  the string contains only 3 alphas (A3), and FALSE otherwise */
/*								*/
/****************************************************************/

int legitA3exists(str)
char *str;
{
    int i;

    if (strlen(str) != 3) return(FALSE);

    for (i=0; i<3; i++) {
        if (!isalpha(*(str+i))) return(FALSE);
    }
    return(TRUE);
}



/****************************************************************/
/*								*/
/*  initTokenStruct is sent the pointer to a token struct and	*/
/*  will initialize the 5 clue word id's to 0, and the 5 clue	*/
/*  types to blank, and the input image string to blanks.	*/
/*								*/
/****************************************************************/

void initTokenStruct(tst)
struct token_struct *tst;
{
    int i;

    memset(tst->input_image,' ',IMAGE_SIZE);

    tst->clue_word_id1 = 0;
    strcpy(tst->clue_type_1,"  ");

    tst->clue_word_id2 = 0;
    strcpy(tst->clue_type_2,"  ");

    tst->clue_word_id3 = 0;
    strcpy(tst->clue_type_3,"  ");

    tst->clue_word_id4 = 0;
    strcpy(tst->clue_type_4,"  ");

    tst->clue_word_id5 = 0;
    strcpy(tst->clue_type_5,"  ");

    tst->beg = 0;
    tst->end = 0;
}



/****************************************************************/
/*								*/
/*  validOTCombo is sent the word_array (positioned at the	*/
/*  current word), the previously determined token struct, and	*/
/*  the clues table struct for the current word.  It returns	*/
/*  true if:							*/
/*     1.  The current word is an ordinal type			*/
/*     2.  The previous word in the string is a matching	*/
/*         numeric (eg.  1 for st, 2 for nd, 3 for rd, and	*/
/*         0,4-9 for th).					*/
/*     3.  If the current word is st or rd, then the next	*/
/*         word in the string must be a TY			*/
/*								*/
/*  For example:     123 1 st ave;  if st is the current word	*/
/*  being processed then we know it's an ordinal type.		*/
/*								*/
/****************************************************************/

int validOTCombo(word_array,tst,tst_loaded)
struct token_struct *tst;
char **word_array;
int tst_loaded;
{
    int i,len;
    char *str, word[MAX_CLUE_RECLEN], hold[3], *upCase(), *lowCase();
    int ordinalType(), allDigits();
    void matchNumberWithOT();

/*
    upCase(*(word_array),word);
*/
    lowCase(*(word_array),word);
/*
 *  Make sure the current word is ST, ND, RD, or TH
 */
    if (! ordinalType(word)) {
        return (FALSE);
    }

/*
 *  Check to see if the previous word is all digits
 */
    str = *(word_array-1);
    if (! allDigits(str)) return(FALSE);

/*
 *  Examine the previous word to see what its OT compliment is.
 *  If the word is "TH" or "ND", then change the OT to match
 *  the number in the previous, if necessary.
 */
    matchNumberWithOT(str,hold);
    if (strncmp(word,"st",2) != 0 && strncmp(word,"rd",2) != 0) {
        lowCase(hold,word);
    }

    strcpy(*word_array,word);

/*
 *  If the OT is "ST" or "RD" then the word following the OT
 *  must be a type ("TY") token.
 */
    if (strcmp(*word_array,"st")==0 || strcmp(*word_array,"rd")==0) {
      if (tst_loaded) {
           if (strcmp(tst->clue_type_1,"TY") != 0 &&
           strcmp(tst->clue_type_2,"TY") != 0 &&
           strcmp(tst->clue_type_3,"TY") != 0 &&
           strcmp(tst->clue_type_4,"TY") != 0 &&
           strcmp(tst->clue_type_5,"TY") != 0) return(FALSE);
      }
      else
           return(FALSE);
    }

    return(TRUE);
}


/****************************************************************/
/*								*/
/*  validFraction is sent the word_array (positioned at the	*/
/*  current word), the previously determined token struct, and	*/
/*  the current word number, and will return TRUE if:		*/
/*								*/
/*     1.  The current word is a fraction,			*/
/*     2.  The previous word is a numeric, and			*/
/*     3.  The following word is NOT a TY or A1.		*/
/*     4.  The current word number is not 1, ie. 		*/
/*         "1/2 street" has nothing to combine 1/2 with.	*/
/*								*/
/*								*/
/*  EX:  123 1/2 main st.    will be combined to:		*/
/*        NU - 1231/2						*/
/*        AU - main						*/
/*        TY - st						*/
/*  The reason this is done is because for every NU ...         */
/*  pattern, there is a NU FC ... pattern.  This eliminates	*/
/*  the second set of patterns.					*/
/*								*/
/****************************************************************/

int validFraction(word_array,currword,tst_loaded,tst,ctype)
struct token_struct *tst;
int currword,tst_loaded;
char **word_array;
char *ctype;
{
    int i,len;
    char *str;

    if (currword <= 1) return(FALSE);

    if (strcmp(ctype,"FC") != 0) return (FALSE);

    str = *(word_array-1);
    len = strlen(str);
    for (i=0; i<len; i++, str++) {
        if (! isdigit(*str)) return (FALSE);
    }
/*
 *  If the endword = numwords then don't check the previously
 *  loaded tst struct, because nothing was loaded!!!!
 */
    if (tst_loaded) {
        if ((strcmp(tst->clue_type_1,"TY") == 0) ||
            (strcmp(tst->clue_type_1,"A1") == 0)) return(FALSE);
    }

    return(TRUE);
}

/****************************************************************/
/*								*/
/*  ordinalType is sent a string and will return true if it's	*/
/*  one of the 4 ordinal types (st, nd, rd, or th).		*/
/*								*/
/****************************************************************/

int ordinalType(str)
char *str;
{
    if (strcmp(str,"ST")==0 || strcmp(str,"ND")==0 ||
        strcmp(str,"RD")==0 || strcmp(str,"TH")==0 ||
        strcmp(str,"st")==0 || strcmp(str,"nd")==0 ||
        strcmp(str,"rd")==0 || strcmp(str,"th")==0) return(TRUE);
    else
        return(FALSE);
}


/****************************************************************/
/*								*/
/*  loadTokenStruct will build a token struct with the		*/
/*  information sent.						*/
/*  It will return TRUE if the following is true:		*/
/*								*/
/*    1.  the string sent is a comma.  Commas were given their	*/
/*        own words but we dont want them in the output token	*/
/*        structure.  For example,  123 main st, rt 4		*/
/*        In this case the comma was use to separate "st" and	*/
/*        "rt" but it serves no purpose from here on out.	*/
/*								*/
/****************************************************************/

int loadTokenStruct(srchstr,tst,ctype,cwordid,cword,curr,end)
char *srchstr;
struct token_struct *tst;
char *ctype;
int cwordid,cword,curr,end;
{
    char *hold;
/*
 *  EXCEPTION 1:  If the current string is a comma, then dont load
 *                anything, and return true.
 */

    if (strcmp(srchstr,",")==0) {
        num_tokens_built--;
        return(TRUE);
    }
        
    strcpy(tst->input_image,srchstr);

    switch (cwordid) {
        case 1: tst->clue_word_id1 = cword;
                strcpy(tst->clue_type_1,ctype);
                break;
        case 2: tst->clue_word_id2 = cword;
                strcpy(tst->clue_type_2,ctype);
                break;
        case 3: tst->clue_word_id3 = cword;
                strcpy(tst->clue_type_3,ctype);
                break;
        case 4: tst->clue_word_id4 = cword;
                strcpy(tst->clue_type_4,ctype);
                break;
        case 5: tst->clue_word_id5 = cword;
                strcpy(tst->clue_type_5,ctype);
                break;
    }
    tst->beg = curr;
    tst->end = end;

    return(FALSE);
}

/****************************************************************/
/*								*/
/*  charExists is sent a string and returns TRUE if any		*/
/*  character in str is an alpha, FALSE otherwise.		*/
/*								*/
/****************************************************************/

int charExists(str)
char *str;
{
    int i, len;

    for (i=0, len=trimln(str); i<len; i++)
        if (isalpha(*(str+i))) return(TRUE);

    return(FALSE);
}



/****************************************************************/
/*								*/
/*  embeddedBlanks is sent a string and returns a pointer to	*/
/*  the character directly after the first blank in str, or	*/
/*  NULL if str contains no blank characters.			*/
/*								*/
/****************************************************************/

char *embeddedBlanks(str)
char *str;
{
    char *pos;

    return ((pos=strchr(str,' ')) == NULL ? NULL : pos+1);
}


/****************************************************************/
/*								*/
/*  matchNumberWithOT is sent a number (char) and will match	*/
/*  that number with its valid Ordinal Type and return it,	*/
/*  UNLESS it's not a match and the OT is ST or RD (example:	*/
/*  "65st" wont be corrected).					*/
/*  EX:  62 is matched properly with "ND".			*/
/*								*/
/****************************************************************/

void matchNumberWithOT(num,ord)
char *num, *ord;
{
    char last_digit, sec_2_last_digit;

    last_digit = *(num + (strlen(num) -1));
    sec_2_last_digit = (trimln(num) > 1) ? *(num + (strlen(num) -2)) : ' ';
/*
 *  Valid combos:
 *                  ST    ND    RD    TH
 *                  --    --    --    --
 *                   1     2     3    4-20
 *                  21    22    23   24-30
 *                  31    32    33   34-40
 *                   .     .     .     .
 *                   .     .     .     .
 *                 101   102   103  104-120
 *                 121   122   123  124-130
 *                   .     .     .     .
 *                   .     .     .     .
 */
    if (last_digit == '1' && sec_2_last_digit != '1')
        strcpy(ord,"st");
    else if (last_digit == '2' && sec_2_last_digit != '1')
        strcpy(ord,"nd");
    else if (last_digit == '3' && sec_2_last_digit != '1')
        strcpy(ord,"rd");
    else
        strcpy(ord,"th");

}



/****************************************************************/
/*								*/
/*  allDigits is sent a string, str, and returns TRUE if every	*/
/*  character in str is a digit, FALSE otherwise.		*/
/*								*/
/****************************************************************/

int allDigits(str)
char *str;
{
    int i,len;

    len = strlen(str);
    for (i=0; i<len; i++) {
        if (! isdigit(*(str+i))) return (FALSE);
    }
    return(TRUE);
}


int roadOrHighway(clue)
struct clue_table *clue;
{
    int cword;

    cword = atoi(clue->clue_word_id1);
    if (
    (strncmp(clue->clue_type_1,"HR",2) == 0) &&
    ( (cword == 67) ||       /*  County   highway  */
      (cword == 69) ||       /*  County   road     */
      (cword == 99) ||       /*  Farm     road     */
      (cword == 257) ||      /*  State    highway  */
      (cword == 258) ||      /*  State    road     */
      (cword == 272) ||      /*  Township highway  */
      (cword == 273) ||      /*  Township road     */
      (cword >= 320) )       /*  Township road     */
    ) return(TRUE);

    return(FALSE);
}


/****************************************************************/
/*								*/
/*  If the current word is N, S, W, or E  AND  it was		*/
/*  previously split up from a numeric then make it only 	*/
/*  a 0A1...							*/
/*                                                              */
/*  EX:  123 main st e6						*/
/*  Here, we don't want "e" to be the suffix direction EAST.	*/
/*                                                              */
/*  EX:  123 main st e 6					*/
/*  Here, we want "e" to be the suffix direction EAST.		*/
/*								*/
/****************************************************************/
int dirNUcombo(str,curr)
char *str;
int curr;
{
    if ((strcmp(str,"N") == 0 || strcmp(str,"n") == 0 ||
         strcmp(str,"S") == 0 || strcmp(str,"s") == 0 ||
         strcmp(str,"E") == 0 || strcmp(str,"w") == 0 ||
         strcmp(str,"W") == 0 || strcmp(str,"e") == 0) &&
    (word_splits[curr-1] != 0)) return(TRUE);

    return(FALSE);
}



/****************************************************************/
/*								*/
/*   If the clue is ST xxx, (ie. ST RT, or ST RD,etc)		*/
/*   then examine the previous word.  If the previous word	*/
/*   is a type (eg. rd, ave, etc) or an SA (full state name	*/
/*   or a state abbreviation) or EX (old), then return FALSE	*/
/*   (keep this clue).  If it's not a type, then if any		*/
/*   character exists in the previous word, then return true. 	*/
/*   Also, if the clue is xxxx RD or xxxx ROAD then examine the	*/
/*   word AFTER the clue.  If it's not a NU, A1, or A2 then 	*/
/*   return true.  This will solve the following cases:		*/
/*								*/
/*       123 california st hwy 1 (keep st hwy)    		*/
/*       123 old st hwy 1 (keep st hwy) 	   		*/
/*       123 main st st rt 4 box 2340 (keep st rt)		*/
/*       123 main st rt 4 (dont keep st rt)			*/
/*       123 county rd (dont keep county rd)			*/
/*       123 county rd y33 (keep county rd)			*/
/*								*/
/****************************************************************/
int unacceptableHRcombo(word_array,clue,currword,endword,numwords,state,zip)
char **word_array, *state, *zip;
struct clue_table *clue;
int currword,endword,numwords;
{
    char *upCase(), *embeddedBlanks(), *pos, *prevword, *nextword;
    char hold[MAX_CLUE_RECLEN];
    int charExists(), allDigits(), roadOrHighway(), legitA2exists();
    struct clue_table *search_clue_table(), *prevclue;

/*
    if ((pos=embeddedBlanks(clue->name)) != NULL) {
*/
    if ((currword>1) && (pos=embeddedBlanks(clue->name)) != NULL) {
        if (strncmp(clue->name,"ST ",3) == 0) {
            prevword = *(word_array-1);
            prevclue = search_clue_table(upCase(prevword,hold),state,zip);
            if ((prevclue != NULL) &&
            ((strcmp(prevclue->clue_type_1,"TY") == 0) ||
            (strcmp(prevclue->clue_type_2,"TY") == 0) ||
            (strcmp(prevclue->clue_type_3,"TY") == 0) ||
            (strcmp(prevclue->clue_type_4,"TY") == 0) ||
            (strcmp(prevclue->clue_type_5,"TY") == 0) ||
            (strcmp(prevclue->clue_type_1,"EX") == 0) ||
            (strcmp(prevclue->clue_type_1,"SA") == 0) ||
            (strcmp(prevclue->clue_type_2,"SA") == 0) ||
            (strcmp(prevclue->clue_type_3,"SA") == 0) ||
            (strcmp(prevclue->clue_type_4,"SA") == 0) ||
            (strcmp(prevclue->clue_type_5,"SA") == 0))) return(FALSE);

            if (charExists(prevword)) return(TRUE);
        }
    }
    if (roadOrHighway(clue)) {
        if (endword == numwords) return(TRUE);
        nextword = *(word_array+((endword-currword) + 1));
        if (!allDigits(nextword) && trimln(nextword)>1 &&
        !legitA2exists(nextword)) return(TRUE);
    }

    return(FALSE);
}



void checkForANs(tst,numtoks)
struct token_struct *tst;
int *numtoks;
{
    int i,j,anfound=FALSE;
    int tokenInStruct();
    void copyTokStruct(), appendTokStruct();
    void nullTokenStruct();

/*
 *  Check for BX, HR, RR, BP, PT, or WD
 */

    for (i=0; i < *numtoks; i++) {
        if (tokenInStruct(tst+i,"BX") || tokenInStruct(tst+i,"HR") ||
        tokenInStruct(tst+i,"WD") || tokenInStruct(tst+i,"RR") ||
        tokenInStruct(tst+i,"PT") || tokenInStruct(tst+i,"BP")) {
/*
 *          Start at the word after the current word and make
 *          the subsequent words AN's if the following cases
 *          are true:
 *            1.  All subsequent words that are potential ANs must
 *                be adjacent.
 *            2.  The first word must be an NU or A1.
 *            3.  All other words must be an NU, A1, or AU and
 *                if we are processesing an HR, then they CANT be D1
 *                (this prevents  123 hwy 1e  from grouping 1e together)
 */

            for (j=(i+1); j < *numtoks; j++) {
                if (word_splits[(tst+j)->end - 1] == 0) break;

                if (j == (i+1)) {       /*  First word after  */
                    if (!tokenInStruct(tst+j,"NU") && !tokenInStruct(tst+j,"A1")) break;
/*
 *                  Next check solves:  123 hwy i80
 */
                    if (tokenInStruct(tst+j,"HR")) break;
                }
                else {
                    if ( (tokenInStruct(tst+j,"NU") || tokenInStruct(tst+j,"A1") ||
                    tokenInStruct(tst+j,"AU")) && 
                    (!tokenInStruct(tst+j,"D1") || !tokenInStruct(tst+i,"HR")) ) {
/*
 *                      Make the 1st word an AN
 */
                        strcpy((tst+(i+1))->clue_type_1,"AN");
                        (tst+(i+1))->clue_word_id1 = 0;
                        strcpy((tst+(i+1))->clue_type_2,"  ");
                        (tst+(i+1))->clue_word_id2 = 0;
                        strcpy((tst+(i+1))->clue_type_3,"  ");
                        (tst+(i+1))->clue_word_id3 = 0;
                        strcpy((tst+(i+1))->clue_type_4,"  ");
                        (tst+(i+1))->clue_word_id4 = 0;
                        strcpy((tst+(i+1))->clue_type_5,"  ");
                        (tst+(i+1))->clue_word_id5 = 0;
/*
 *                      Make every subsequent word an AN
 */
                        strcpy((tst+j)->clue_type_1,"AN");
                        (tst+j)->clue_word_id1 = 0;
                        strcpy((tst+j)->clue_type_2,"  ");
                        (tst+j)->clue_word_id2 = 0;
                        strcpy((tst+j)->clue_type_3,"  ");
                        (tst+j)->clue_word_id3 = 0;
                        strcpy((tst+j)->clue_type_4,"  ");
                        (tst+j)->clue_word_id4 = 0;
                        strcpy((tst+j)->clue_type_5,"  ");
                        (tst+j)->clue_word_id5 = 0;
                        anfound = TRUE;
                    }
                    else break;
                }
            }
            i = j - 1;
        }
    }
/*
 *  If any ANs were found then you must now collapse the token
 *  structure together.
 */
    if (anfound) {
        for (i=0; i < (*numtoks - 1); i++) {
            if (tokenInStruct(tst+i,"AN") && tokenInStruct(tst+i+1,"AN")) {

                appendTokStruct(tst+i,tst+i+1);
                nullTokenStruct(tst+i+1);

                for (j=i+1; j < (*numtoks-1); j++) {
                    copyTokStruct(tst+j,tst+j+1);
                    nullTokenStruct(tst+j+1);
                }

                --(*numtoks); --i;

            }
        }
    }


}


void appendTokStruct(tst1,tst2)
struct token_struct *tst1;
struct token_struct *tst2;
{
    strcat(tst1->input_image,tst2->input_image);
    strcpy(tst1->clue_type_1,tst2->clue_type_1);
    strcpy(tst1->clue_type_2,tst2->clue_type_2);
    strcpy(tst1->clue_type_3,tst2->clue_type_3);
    strcpy(tst1->clue_type_4,tst2->clue_type_4);
    strcpy(tst1->clue_type_5,tst2->clue_type_5);
    tst1->clue_word_id1 = tst2->clue_word_id1;
    tst1->clue_word_id2 = tst2->clue_word_id2;
    tst1->clue_word_id3 = tst2->clue_word_id3;
    tst1->clue_word_id4 = tst2->clue_word_id4;
    tst1->clue_word_id5 = tst2->clue_word_id5;
    tst1->beg = tst2->beg;
    tst1->end = tst2->end;
}



void copyTokStruct(tst1,tst2)
struct token_struct *tst1;
struct token_struct *tst2;
{
    strcpy(tst1->input_image,tst2->input_image);
    strcpy(tst1->clue_type_1,tst2->clue_type_1);
    strcpy(tst1->clue_type_2,tst2->clue_type_2);
    strcpy(tst1->clue_type_3,tst2->clue_type_3);
    strcpy(tst1->clue_type_4,tst2->clue_type_4);
    strcpy(tst1->clue_type_5,tst2->clue_type_5);
    tst1->clue_word_id1 = tst2->clue_word_id1;
    tst1->clue_word_id2 = tst2->clue_word_id2;
    tst1->clue_word_id3 = tst2->clue_word_id3;
    tst1->clue_word_id4 = tst2->clue_word_id4;
    tst1->clue_word_id5 = tst2->clue_word_id5;
    tst1->beg = tst2->beg;
    tst1->end = tst2->end;
}




void switchTokStruct(tst1,tst2)
struct token_struct *tst1;
struct token_struct *tst2;
{
    char himage[IMAGE_SIZE];
    char ht1[3], ht2[3], ht3[3], ht4[3], ht5[3];
    int hi1, hi2, hi3, hi4, hi5, hbeg, hend;

    strcpy(himage,tst1->input_image);
    strcpy(ht1,tst1->clue_type_1);
    strcpy(ht2,tst1->clue_type_2);
    strcpy(ht3,tst1->clue_type_3);
    strcpy(ht4,tst1->clue_type_4);
    strcpy(ht5,tst1->clue_type_5);
    hi1 = tst1->clue_word_id1;
    hi2 = tst1->clue_word_id2;
    hi3 = tst1->clue_word_id3;
    hi4 = tst1->clue_word_id4;
    hi5 = tst1->clue_word_id5;
    hbeg = tst1->beg;
    hend = tst1->end;

    strcpy(tst1->input_image,tst2->input_image);
    strcpy(tst1->clue_type_1,tst2->clue_type_1);
    strcpy(tst1->clue_type_2,tst2->clue_type_2);
    strcpy(tst1->clue_type_3,tst2->clue_type_3);
    strcpy(tst1->clue_type_4,tst2->clue_type_4);
    strcpy(tst1->clue_type_5,tst2->clue_type_5);
    tst1->clue_word_id1 = tst2->clue_word_id1;
    tst1->clue_word_id2 = tst2->clue_word_id2;
    tst1->clue_word_id3 = tst2->clue_word_id3;
    tst1->clue_word_id4 = tst2->clue_word_id4;
    tst1->clue_word_id5 = tst2->clue_word_id5;
    tst1->beg = tst2->beg;
    tst1->end = tst2->end;

    strcpy(tst2->input_image,himage);
    strcpy(tst2->clue_type_1,ht1);
    strcpy(tst2->clue_type_2,ht2);
    strcpy(tst2->clue_type_3,ht3);
    strcpy(tst2->clue_type_4,ht4);
    strcpy(tst2->clue_type_5,ht5);
    tst2->clue_word_id1 = hi1;
    tst2->clue_word_id2 = hi2;
    tst2->clue_word_id3 = hi3;
    tst2->clue_word_id4 = hi4;
    tst2->clue_word_id5 = hi5;
    tst2->beg = hbeg;
    tst2->end = hend;

}


int tokenInStruct(tst,token)
struct token_struct *tst;
char *token;
{
    if (strncmp(tst->clue_type_1,token,2) == 0 ||
    strncmp(tst->clue_type_2,token,2) == 0 ||
    strncmp(tst->clue_type_3,token,2) == 0 ||
    strncmp(tst->clue_type_4,token,2) == 0 ||
    strncmp(tst->clue_type_5,token,2) == 0) return(TRUE);

    return(FALSE);
}



void nullTokenStruct(tst)
struct token_struct *tst;
{
    int i;

    memset(tst->input_image,'\0',IMAGE_SIZE);

    tst->clue_word_id1 = 0;
    memset(tst->clue_type_1,'\0',3);

    tst->clue_word_id2 = 0;
    memset(tst->clue_type_2,'\0',3);

    tst->clue_word_id3 = 0;
    memset(tst->clue_type_3,'\0',3);

    tst->clue_word_id4 = 0;
    memset(tst->clue_type_4,'\0',3);

    tst->clue_word_id5 = 0;
    memset(tst->clue_type_5,'\0',3);

    tst->beg = 0;
    tst->end = 0;
}



int dashFound(str,len)
char *str;
int len;
{
    int i;
    for (i=0; i<len; i++) {
        if (*(str+i) == ' ') continue;
        if (*(str+i) == '-') 
            return(TRUE);
        else
            return(FALSE);
    }
    return(FALSE);
}
