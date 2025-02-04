/****************************************************************/
/****************************************************************/
/*                                                              */
/*  load_cluewords loads the clue word table into a hash table	*/
/*                                                              */
/****************************************************************/
/****************************************************************/
/*                                                              */

#include "clues.h"

struct clue_table *entries[CLUE_ARRAYSIZE];

int load_cluewords(filename)
char *filename;
{
    struct clue_table *find;
    FILE *fp;
    char srec[CLUE_RECLEN];
    char image[25], abbr[15], lang;
    char usf, store_flag;
    char cw1[5], cw2[5], cw3[5], cw4[5], cw5[5];
    char ct1[3], ct2[3], ct3[3], ct4[3], ct5[3];
    char state[3], county[4], place[5], zip[6], mincs[4];
    int cnt=0,i;
    struct clue_table *ent;

/*
 *  Open the clue words table
 */
    if (strchr(filename,'\n') != NULL) *strchr(filename,'\n') = 0;
    if ((fp=fopen(filename,"r"))==NULL) {
        printf("fopen failed for clue table file: %s\n",filename);
        return(0);
    }
/*
 *  Initialize entries to NULL
 */
    for (i=0; i<CLUE_ARRAYSIZE; i++) entries[i] = NULL;
/*
 *  read each record and load it into the hash table
 */
    while (fgets(srec,sizeof(srec),fp)) {
        cnt++;
        if (strchr(srec,'\n') != NULL) *strchr(srec,'\n') = 0;
/*
 *      Check for overflow
 */
        if (cnt > CLUE_ARRAYSIZE) {
            printf ("ERROR:  trying to load too many clues.\n");
            printf ("        Max number that can be loaded: %d\n",
            CLUE_ARRAYSIZE);
            return(0);
        }
        sprintf(image,"%.24s\0",srec);
        sprintf(abbr,"%.14s\0",&srec[24]);
        usf = srec[38];
        lang = srec[39];
        sprintf(cw1,"%.4s\0",&srec[40]);
        sprintf(ct1,"%.2s\0",&srec[44]);
        sprintf(cw2,"%.4s\0",&srec[46]);
        sprintf(ct2,"%.2s\0",&srec[50]);
        sprintf(cw3,"%.4s\0",&srec[52]);
        sprintf(ct3,"%.2s\0",&srec[56]);
        sprintf(cw4,"%.4s\0",&srec[58]);
        sprintf(ct4,"%.2s\0",&srec[62]);
        sprintf(cw5,"%.4s\0",&srec[64]);
        sprintf(ct5,"%.2s\0",&srec[68]);
        sprintf(state,"%.2s\0",&srec[70]);
        sprintf(county,"%.3s\0",&srec[72]);
        sprintf(place,"%.4s\0",&srec[75]);
        sprintf(zip,"%.5s\0",&srec[79]);
        sprintf(mincs,"%.3s\0",&srec[85]);
        store_flag = srec[87];

        ent = addClue(image,abbr,usf,lang,cw1,ct1,cw2,ct2,cw3,ct3,
        cw4,ct4,cw5,ct5,state,county,place,zip,mincs,store_flag);
    }
/*
    for (i=0; i<CLUE_ARRAYSIZE; i++) {
        if (entries[i] != NULL) {
            fprintf (fpout,"%d: %d\n",i,entries[i]);
            for (find=entries[i];find->next != NULL;find=find->next) {
                fprintf (fpout,"      %d\n",find->next);
            }
        }
    }
*/
    fclose(fp);
    return(1);

}


/****************************************************************/
/*                                                              */
/*  trimln returns the length of the string, str, not		*/
/*  including any trailing blanks.				*/
/*                                                              */
/****************************************************************/
int trimln(str)
char *str;
{
    int start;

    for (start=(strlen(str)-1);start>=0;start--) {
        if (strncmp(&str[start]," ",1) != 0) return(start+1);
    }
    return(0);
}


/****************************************************************/
/*                                                              */
/*  clueHash calculates the integer hash value for the string,	*/
/*  str, by summing up the ascii values for each character	*/
/*  in the string and returning the remainder of that value	*/
/*  divided by the maximum hash size macro, CLUE_HASHSIZE.	*/
/*                                                              */
/****************************************************************/
unsigned int clueHash(str)
char *str;
{
    unsigned int val, len, i;

    len = trimln(str);
    for (val=0, i=0; (*str != '\0' && i<len); i++, str++) {
        val = *str + (31*val);
    }
    return (val%CLUE_HASHSIZE);
}


/****************************************************************/
/*                                                              */
/*  stringdup simply returns a pointer to a copy of the string,	*/
/*  string.							*/
/*                                                              */
/****************************************************************/
char *stringdup(string)
char *string;
{
   char *temp;
   int len;

   if ((len = trimln(string)) == 0) len = strlen(string);
   temp = (char *)malloc(len+1);
   if (temp != NULL) {
       strncpy(temp,string,len);
       *(temp+len) = '\0';
   }
   return temp;
}


/****************************************************************/
/*                                                              */
/*  searchForClue searches in the hash table for the string,	*/
/*  str, and returns a pointer to a clue_table structure if	*/
/*  it's found, or NULL if it's not found.			*/
/*                                                              */
/****************************************************************/
struct clue_table *searchForClue(str,state,zip)
char *str, *state, *zip;
{
    struct clue_table *temp;
    int ln1, ln2;
/*
 *  Walk along the linked list
 */
    for (temp=entries[clueHash(str)]; temp != NULL; temp=temp->next) {
        ln1 = trimln(str);
        ln2 = trimln(temp->name);
        if ((strncmp(str,temp->name,ln1) == 0) && (ln1 == ln2)) {
            if (strncmp(temp->state,"  ",2) != 0 && strncmp(temp->state,state,2) != 0)
               continue;
            if (strncmp(temp->zip,"  ",2) != 0 && strncmp(temp->zip,zip,2) != 0)
               continue;
            return (temp);
        }
    }
/*
 *  If not found, then return NULL
 */
    return (NULL);
}

/****************************************************************/
/*                                                              */
/*  addClue simply adds an entry to the hash table.		*/
/*                                                              */
/****************************************************************/
struct clue_table *addClue(image,abbr,usf,lang,cw1,ct1,cw2,ct2,cw3,
ct3,cw4,ct4,cw5,ct5,state,county,place,zip,mincs,store_flag)

char *image,*abbr,usf,lang,*cw1,*ct1,*cw2,*ct2,*cw3,*ct3,*cw4;
char *ct4,*cw5,*ct5,*state,*county,*place,*zip,*mincs,store_flag;
{
    struct clue_table *temp, *find;
    unsigned int val;
    char *stringdup();

/*
 *  name NOT FOUND
 */
    if ((temp=searchForClue(image,state,zip)) == NULL) {
        temp = (struct clue_table *) malloc(sizeof(*temp));
        if (temp==NULL ||
           (temp->name=stringdup(image))==NULL ||
           (temp->translation=stringdup(abbr))==NULL ||
           (temp->usage_flag=usf)=='\0' ||
           (temp->language=lang)=='\0' ||
           (temp->clue_word_id1=stringdup(cw1))==NULL ||
           (temp->clue_type_1=stringdup(ct1))==NULL ||
           (temp->clue_word_id2=stringdup(cw2))==NULL ||
           (temp->clue_type_2=stringdup(ct2))==NULL ||
           (temp->clue_word_id3=stringdup(cw3))==NULL ||
           (temp->clue_type_3=stringdup(ct3))==NULL ||
           (temp->clue_word_id4=stringdup(cw4))==NULL ||
           (temp->clue_type_4=stringdup(ct4))==NULL ||
           (temp->clue_word_id5=stringdup(cw5))==NULL ||
           (temp->clue_type_5=stringdup(ct5))==NULL ||
           (temp->state=stringdup(state))==NULL ||
           (temp->county=stringdup(county))==NULL ||
           (temp->place=stringdup(place))==NULL ||
           (temp->zip=stringdup(zip))==NULL ||
           (temp->store_flag=store_flag)=='\0' ||
           (temp->min_compar_score=stringdup(mincs))==NULL) return(NULL);
        temp->next = NULL;        /*  SET THE NEW ENTRY */
        val = clueHash(image);

        if (entries[val] == NULL) {
           entries[val] = temp;
        }
        else {
           for (find=entries[val];find->next != NULL;find=find->next) {}
           find->next = temp;
        }
    }
/*
 *  name FOUND
 */
    else {
        printf ("%s already exists in the clue hash table\n",image);
        assert(0);
    }
/*
    free(temp);
*/
    return (temp);
}
