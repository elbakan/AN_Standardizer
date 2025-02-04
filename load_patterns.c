/****************************************************************/
/****************************************************************/
/*                                                              */
/*  load_pattern  loads the pattern table into a hash table     */
/*                                                              */
/****************************************************************/
/****************************************************************/
/*                                                              */

#include "patterns.h"

struct pattern_table *patterns[PATTERN_ARRAYSIZE];

int load_patterns(filename)
char *filename;
{
    FILE *fp;
    char srec[PATTERN_RECLEN];
    char intok[36], outtok[36], pattyp[3], pri[3], st[3];
    char cou[4], plc[5], zip[6], lang, usf, excl;
    int cnt=0,i;
    struct pattern_table *pat;

    for (i=0; i<PATTERN_ARRAYSIZE; i++)
        patterns[i] = NULL;

/*
 *  Open the file
 */
    if (strchr(filename,'\n') != NULL) *strchr(filename,'\n') = 0;
    if ((fp=fopen(filename,"r"))==NULL) {
        printf("fopen failed for pattern file: %s\n",filename);
        return(0);
    }
/*
 *  Load the table, processing 3 records at a time:
 *      record 1:  input pattern
 *        "    2:  output pattern, pattern type, priority, etc.
 *        "    3:  blank line
 */
    while (fgets(srec,sizeof(srec),fp)) {
        if (strchr(srec,'\n') != NULL) *strchr(srec,'\n') = 0;
        sprintf(intok,"%.35s\0",&srec[1]);
/*
 *      Retrive the translation record
 */
        if (! fgets(srec,sizeof(srec),fp)) {
            fprintf(stderr,"No translation record for %s\n",intok);
            assert(0);
        }
/*
 *      load the variables
 */
        sprintf(outtok,"%.35s\0",&srec[1]);
        sprintf(pattyp,"%.2s\0",&srec[37]);
        sprintf(pri,"%.2s\0",&srec[40]);
        sprintf(st,"%.2s\0",&srec[42]);
        sprintf(cou,"%.3s\0",&srec[44]);
        sprintf(plc,"%.4s\0",&srec[47]);
        sprintf(zip,"%.5s\0",&srec[51]);
/*
        strncpy(pattyp,&srec[37],2);
        strncpy(pri,&srec[40],2);
        strncpy(st,&srec[42],2);
        strncpy(cou,&srec[44],3);
        strncpy(plc,&srec[47],4);
        strncpy(zip,&srec[51],5);
*/
        lang = srec[56];
        usf = srec[57];
        excl = srec[58];
        
        pat=addPattern(intok,outtok,pattyp,pri,st,cou,plc,zip,lang,usf,excl);
        if (! fgets(srec,sizeof(srec),fp)) break;

        cnt++;
        if (cnt > PATTERN_ARRAYSIZE) {
            printf ("ERROR:  trying to load too many patterns.\n");
            printf ("        Max number that can be loaded: %d\n",
            PATTERN_ARRAYSIZE);
            return(0);
        }
    }
    fclose(fp);
    return(1);

}



/****************************************************************/
/*                                                              */
/*  patternHash determines the integer hash value for the	*/
/*  pattern string, str.					*/
/*                                                              */
/****************************************************************/
unsigned int patternHash(str)
char *str;
{
    unsigned int val, len, i;

    len = trimln(str);
    for (val=0, i=0; (*str != '\0' && i<len); i++, str++) {
        val = *str + (31*val);
    }
    return (val%PATTERN_HASHSIZE);
}



/****************************************************************/
/*                                                              */
/*  searchForPattern searches for the string, str, in the	*/
/*  pattern table.						*/
/*                                                              */
/****************************************************************/
struct pattern_table *searchForPattern(str,state,zip,usf)
char *str, *state, *zip, usf;
{
    struct pattern_table *temp, *hold;
    int ln1, ln2, i;
/*
 *  Walk along the linked list
 */
/*
    printf ("     searching for pattern: %s, usf: %c\n",str,usf);
*/
    for (hold=NULL,temp=patterns[patternHash(str)]; temp!=NULL; temp=temp->next) {

        ln1 = trimln(str);
        ln2 = trimln(temp->input_token_string);
        if ((strncmp(str,temp->input_token_string,ln1) == 0) && 
        (ln1 == ln2)) {
            if (strncmp(temp->state,"  ",2) != 0 && strncmp(temp->state,state,2) != 0)
               continue;
            if (strncmp(temp->zip_code,"  ",2) != 0 && strncmp(temp->zip_code,zip,2) != 0)
               continue;
/*
 *          Save the pattern where usage flag is blank, unless the exclude flag is set.
 *          If the incoming usage flag is blank, then you can match to anything.
 *          UPDATE:  if the input usage flag is 'S', then you can match to 'T', 'TX',
 *                   or 'B', but NOT 'BX'.
 */
/*
            printf ("   legit found: %s, usf: %c, pri: %s\n",temp->input_token_string,
            temp->usf,temp->priority);
*/
            if (usf == ' ') {
                if (temp->exclude != 'X') {
                    if (temp->usf == ' ') return(temp);
                    hold = temp;
                }
            }
            else if (usf == 'S') {
                if (temp->usf == 'T' || (temp->usf == 'B' && temp->exclude != 'X')) 
                return(temp);
            }
            else if (usf == temp->usf) {
                return (temp);
            }
        }
    }
/*
 *  If you couldnt find matching usage flags, then return NULL
 */
    return (hold);
}



/****************************************************************/
/*                                                              */
/*  addPattern adds an entry in the pattern hash table.		*/
/*                                                              */
/****************************************************************/
struct pattern_table *addPattern(intok,outtok,pattyp,pri,st,cou,
plc,zip,lang,usf,excl)
char *intok,*outtok,*pattyp,*pri,*st,*cou,*plc,*zip,lang,usf,excl;
{
    struct pattern_table *temp, *find;
    unsigned int val;

/*
 *  Load the structure
 */
    temp = (struct pattern_table *) malloc(sizeof(*temp));
    if (temp==NULL) return(NULL);

    memset(temp->input_token_string,' ',PATSIZE);
    memset(temp->output_token_string,' ',PATSIZE);
    strcpy(temp->input_token_string,intok);
    strcpy(temp->output_token_string,outtok);
/*
    strncpy(temp->input_token_string,intok,trimln(intok));
    strncpy(temp->output_token_string,outtok,trimln(outtok));
*/
    strncpy(temp->pattern_type,pattyp,2);
    strncpy(temp->priority,pri,2);
    strncpy(temp->state,st,2);
    strncpy(temp->county,cou,3);
    strncpy(temp->place,plc,4);
    strncpy(temp->zip_code,zip,5);
    temp->language = lang;
    temp->usf = usf;
    temp->exclude = excl;
    temp->next = NULL;        /*  SET THE NEXT ENTRY */
    val = patternHash(intok);

    if (patterns[val] == NULL)
        patterns[val] = temp;
    else {
        for (find=patterns[val]; find->next!=NULL; find=find->next){}
        find->next = temp;
    }

    return (temp);
}
