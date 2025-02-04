/****************************************************************/
/****************************************************************/
/*								*/
/*  build_stand_address simply loads the final return 		*/
/*  structure, sa, with the information sent.  Note that	*/
/*  the abb flag is sent, which lets the user decide		*/
/*  which form of abbreviation (ex: ave, avenue, aven, etc)	*/
/*  is used to load the structure.				*/
/*								*/
/****************************************************************/
/****************************************************************/
/*								*/

#include <ctype.h>
#include "clues.h"
#include "tokens.h"
#include "patterns.h"
#include "stand.h"

#define FALSE 0
#define TRUE 1

enum pad {spaces, squash, dash, asterisk};
enum initialize {all, allButW, allButH};

void build_stand_address(tst,numtok,return_patt,numpatts,t_abb,usf,sa,state,zip)
struct token_struct *tst;
struct output_pattern *return_patt;
int numtok,numpatts;
char t_abb, usf, *state, *zip;
struct standardized_address *sa;
{
    char BNum[NUMBER_OUTPUT_FIELD_SIZE];
    char BNumPre[PREFIX_OUTPUT_FIELD_SIZE];
    char BNumSuf[SUFFIX_OUTPUT_FIELD_SIZE];
    char otok[3], itok[3], match_abb, abb, hold[MAX_LENGTH_OF_ANY_OUTPUT_FIELD];
    char oSD[DIRECTION_OUTPUT_FIELD_SIZE];
    extern char *upCase();
    char *fract, *fractionWithinNumber(); 
    struct token_struct *tst_beg;
    struct posstr pa[MAX_OUTPUT_PATTERNS];
    int i,j,cword,lenn,beg,end,pos,oSDcword=0,NAonly;
    int checkDirections(), checkPTtoNA(), checkPT();
    int getClueWordID(), loadStorageField();
    void checkForSpelledOutNumber(), initAddressStruct(); 
    void loadField(), checkHouseNumber(), checkBuildingNumber();
    void checkWithinStructure(), checkBox(), checkStreetName();
    void stripLeadingZeros();
    extern void buildPositionArray();

    abb = toupper(t_abb);
    if (abb == 'E' || abb == 'F' || abb == 'S')
        match_abb = 'F';
    else
        match_abb = 'U';
/*
 *  initialize sa
 */
    initAddressStruct(sa,all);
    memset(BNum,'\0',NUMBER_OUTPUT_FIELD_SIZE);
    memset(BNumPre,'\0',PREFIX_OUTPUT_FIELD_SIZE);
    memset(BNumSuf,'\0',SUFFIX_OUTPUT_FIELD_SIZE);
    memset(oSD,'\0',DIRECTION_OUTPUT_FIELD_SIZE);
    NAonly = TRUE;

    buildPositionArray(pa,return_patt,numpatts);

    tst_beg = tst;
/*
 *  loop through each sub-pattern found
 */
    for (i=0; i<numpatts; i++, tst=tst_beg) {
        pos = pa[i].order - 1;
        beg = (return_patt+pos)->beg;
        end = (return_patt+pos)->end;
        tst += (beg - 1);
/*
 *      For each sub-pattern, individually process each token
 */

/*
 *      I think this is the hard code for F Scott Fitzgerald Ave, so
 *      the F goes to street name, not house number suffix.
 */
        if (strncmp((return_patt+pos)->input_patt,"NU A1",5) == 0 &&
        strncmp((return_patt+pos)->output_patt,"HN HS",5) == 0 &&
        usf == 'T') {
            strncpy((return_patt+pos)->output_patt+3,"NA",2);
        }
        for (j=0; j <= (end-beg); j++, tst++) {
            sprintf(otok,"%.2s\0",(return_patt+pos)->output_patt+(3*j));
            sprintf(itok,"%.2s\0",(return_patt+pos)->input_patt+(3*j));

/*
            printf ("tst->input_image: %s\n",tst->input_image);
            printf ("itok: %s\n",itok);
            printf ("otok: %s\n",otok);
            printf ("cword: %d\n",getClueWordID(tst,itok));
*/
/*
 *          call getClueWordID to calculate and return the proper clue
 *          word id for the current token
 */
            cword = getClueWordID(tst,itok);
            if (strncmp(otok,"NA",2) != 0) NAonly = FALSE;
/*
 *          Strip leading zeros from all numeric fields except the
 *          house numbers (they will be dealt with later).
 */
            if (strncmp(itok,"NU",2) == 0 && strncmp(otok,"H1",2) !=0
            && strncmp(otok,"H2",2) !=0 && strncmp(otok,"HN",2) !=0)
                stripLeadingZeros(tst->input_image);

/*
 *          NOTE:  when loading certain fields, you must check to see if
 *          a fraction was earlier combined with the field.  If so, 
 *          remove it and load it in its appropriate field.
 */
            if (strncmp(otok,"H1",2) == 0 || strncmp(otok,"HN",2) == 0) {
                checkForSpelledOutNumber(tst,state,zip);
                if ((fract=fractionWithinNumber(tst->input_image)) != NULL) {
                    loadField(sa->hns,SUFFIX_OUTPUT_FIELD_SIZE,
                    fract,0,itok,abb,spaces);
                }
                loadField(sa->hn,NUMBER_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,spaces);
            }
            else if (strncmp(otok,"H2",2) == 0) {
                checkForSpelledOutNumber(tst,state,zip);
                if ((fract=fractionWithinNumber(tst->input_image)) != NULL)
                    loadField(sa->hns,SUFFIX_OUTPUT_FIELD_SIZE,
                    fract,0,itok,abb,spaces);
                loadField(sa->hn2,NUMBER_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,spaces);
            }
            else if (strncmp(otok,"P1",2) == 0) {
                loadField(sa->hnp,PREFIX_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,'S',spaces);
            }
            else if (strncmp(otok,"P2",2) == 0) {
                loadField(sa->hnp2,PREFIX_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,'S',spaces);
            }
            else if (strncmp(otok,"HS",2) == 0) {
                loadField(sa->hns,SUFFIX_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,spaces);
            }
            else if (strncmp(otok,"PD",2) == 0) {
                loadField(sa->snpd,DIRECTION_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,spaces);
                sa->snpd_c = cword;
            }
            else if (strncmp(otok,"PT",2) == 0) {
                if (checkPT(tst,cword,itok,abb,sa)) continue;
/*
 *              checkPTtoNA will change:
 *                     IMAGE:  ST         ST
 *              CLUE WORD ID: 263         79
 *               INPUT TOKEN:  TY  -to-   AU
 *              OUTPUT TOKEN:  PT         NA
 */
                if (checkPTtoNA(tst,&cword,(return_patt+pos)->input_patt+(3*j),
                (return_patt+pos)->output_patt+(3*j))) {
                    tst--; j--; continue;
                }
                loadField(sa->snpt,TYPE_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,spaces);
                sa->snpt_c = cword;
            }
            else if (strncmp(otok,"NA",2) == 0) {
                loadField(sa->osn,NAME_OUTPUT_FIELD_SIZE,
                tst->input_image,0,itok,abb,spaces);
/*
 *              If the abbreviation option is 'E' (use expanded matching names),
 *              then loadStorageField will copy the storage name to the matching
 *              name:  eg, we want "fifth" ave to have a matching name of "5th"
 */
                if (! loadStorageField(sa->ssn,NAME_OUTPUT_FIELD_SIZE,
                &cword,tst->input_image,abb,state,zip)) {
                    loadField(sa->ssn,NAME_OUTPUT_FIELD_SIZE,
                    tst->input_image,0,itok,abb,spaces);
                }
/*
 *              Make sure "st barnabas" has a matching name of "saint barnabas"
 *              for type 'E'
 */
                if (strncmp(itok,"TY",2)==0) {
                    lenn = trimln(tst->input_image);

                    if (strncmp(upCase(tst->input_image,hold),"ST",lenn) == 0) {
                        loadField(sa->msn,NAME_OUTPUT_FIELD_SIZE,"Saint",
                        (match_abb=='F'?0:cword),itok,
                        match_abb,spaces);
                    }
                    else if (strncmp(upCase(tst->input_image,hold),"DR",lenn) == 0) {
                        loadField(sa->msn,NAME_OUTPUT_FIELD_SIZE,"Dr",
                        (match_abb=='F'?0:cword),itok,
                        match_abb,spaces);
                    }
                    else {
                        loadField(sa->msn,NAME_OUTPUT_FIELD_SIZE,tst->input_image,
                        cword,itok,match_abb,spaces);
                    }
                }
                else {
/*
 *                  If the abbreviation option is 'E', then
 *                  ALL matching names now need the FULL names:
 *
 *                  springhill    -->   springhill
 *                  Spring hill   -->   Spr Hl
 *                  Before, these two names wouldn't match
 *
 *                  springhill    -->   springhill
 *                  Spring hill   -->   Spring Hill
 *                  Now they can.
 */
                    if (strncmp(itok,"NL",2)!=0) {
                        loadField(sa->msn,NAME_OUTPUT_FIELD_SIZE,
                        tst->input_image,cword,itok,match_abb,spaces);
                    }
                }
            }
            else if (strncmp(otok,"ST",2) == 0) {
                loadField(sa->snst,TYPE_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,spaces);
                sa->snst_c = cword;
            }
            else if (strncmp(otok,"SD",2) == 0) {
                strcpy(oSD,tst->input_image);
                oSDcword = cword;
                loadField(sa->snsd,DIRECTION_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,spaces);
                sa->snsd_c = cword;
            }
            else if (strncmp(otok,"EX",2) == 0) {
                loadField(sa->sne,EXTENSION_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,spaces);
                sa->sne_c = cword;
            }
            else if (strncmp(otok,"WD",2) == 0) {
                loadField(sa->wsd,TYPE_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,'U',spaces);
                sa->wsd_c = cword;
            }
            else if (strncmp(otok,"WI",2) == 0) {
                checkForSpelledOutNumber(tst,state,zip);
/*
                if ((fract=fractionWithinNumber(tst->input_image)) != NULL)
                    loadField(sa->ei,EXTRAINFO_OUTPUT_FIELD_SIZE,
                    fract,0,itok,abb,spaces);
*/

                loadField(sa->wsi,NUMBER_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,asterisk);
            }
            else if (strncmp(otok,"NB",2) == 0) {
                checkForSpelledOutNumber(tst,state,zip);
                loadField(BNum,NUMBER_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,squash);
            }
            else if (strncmp(otok,"1P",2) == 0) {
                loadField(BNumPre,PREFIX_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,'S',spaces);
            }
            else if (strncmp(otok,"BS",2) == 0) {
                loadField(BNumSuf,SUFFIX_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,spaces);
            }
            else if (strncmp(otok,"DB",2) == 0) {
                loadField(sa->ppd,DIRECTION_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,spaces);
                sa->ppd_c = cword;
            }
            else if (strncmp(otok,"TB",2) == 0) {
                loadField(sa->ppt,TYPE_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,spaces);
                sa->ppt_c = cword;
            }
            else if (strncmp(otok,"BN",2) == 0) {
                loadField(sa->opn,NAME_OUTPUT_FIELD_SIZE,
                tst->input_image,0,itok,abb,spaces);

                if (strncmp(itok,"TY",2)==0) {
                    loadField(sa->mpn,NAME_OUTPUT_FIELD_SIZE,
                    tst->input_image,0,itok,abb,spaces);
                }
                else {
                    if (strncmp(itok,"NL",2)!=0)
                        loadField(sa->mpn,NAME_OUTPUT_FIELD_SIZE,
                        tst->input_image,cword,itok,'S',spaces);
                }
            }
            else if (strncmp(otok,"BT",2) == 0) {
                loadField(sa->pst,TYPE_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,spaces);
                sa->pst_c = cword;
            }
            else if (strncmp(otok,"BD",2) == 0) {
                loadField(sa->psd,DIRECTION_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,spaces);
                sa->psd_c = cword;
            }
            else if (strncmp(otok,"BY",2) == 0) {
                loadField(sa->sd,TYPE_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,spaces);
            }
            else if (strncmp(otok,"BI",2) == 0) {
                loadField(sa->si,NUMBER_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,spaces);
            }
            else if (strncmp(otok,"RR",2) == 0) {
                loadField(sa->rrd,TYPE_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,spaces);
                sa->rrd_c = cword;
            }
            else if (strncmp(otok,"RN",2) == 0) {
                checkForSpelledOutNumber(tst,state,zip);
                if ((fract=fractionWithinNumber(tst->input_image)) != NULL)
                    loadField(sa->ei,EXTRAINFO_OUTPUT_FIELD_SIZE,
                    fract,0,itok,abb,spaces);
                loadField(sa->rri,NUMBER_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,dash);
            }
            else if (strncmp(otok,"BX",2) == 0) {
                loadField(sa->bxd,TYPE_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,'S',spaces);
                sa->bxd_c = cword;
            }
            else if (strncmp(otok,"XN",2) == 0) {
                checkForSpelledOutNumber(tst,state,zip);
                loadField(sa->bxi,NUMBER_OUTPUT_FIELD_SIZE,
                tst->input_image,cword,itok,abb,dash);
            }
            else if (strncmp(otok,"EI",2) == 0) {
                loadField(sa->ei,EXTRAINFO_OUTPUT_FIELD_SIZE,
                tst->input_image,0,itok,abb,spaces);
            }
            else if (strncmp(otok,"NL",2) == 0)
                continue;
            else {
                fprintf (stderr,"ERROR:  output token type does not exist: ");
                fprintf (stderr,"%s\ninput pattern: %s\noutput_pattern: %s\n",
                otok,(return_patt+pos)->input_patt,
                (return_patt+pos)->output_patt);
                assert(0);
            }
        }
    }

/*
 *  Before returning, a variety of checks will be made for special
 *  situations on certain fields
 */
    checkBox(sa,abb);
    checkHouseNumber(sa);
    checkBuildingNumber(sa,BNum,BNumPre,BNumSuf);
    if (usf != 'T' && usf != 'S') checkDirections(sa,oSD,oSDcword,abb);
    checkWithinStructure(sa,abb);
    checkStreetName(sa,NAonly,usf);

}


/****************************************************************/
/*								*/
/*  loadField loads the field, fld, in the structure, sa, with	*/
/*  the information sent.					*/
/*								*/
/****************************************************************/
void loadField(fld,maxlen,image,cword,intok,abbrev_opt,combine)
char *image, *fld, *intok;
char abbrev_opt;
int maxlen, cword, combine;
{
    char *addfield, hold[MAX_LENGTH_OF_ANY_OUTPUT_FIELD], padch[3];
    struct master *mp, *search_master_clues();
    short len = 0, len2 = 0;

/*
 *  If the clue word id for the token is 0 (ex: AU's, NU's, etc)
 *  then the master clues table cannot be searched
 */
    if (cword == 0)
        addfield = image;
/*
 *  If the clue type is AU and the clue word id non-zero then this
 *  could possibly be consecutive AUs that were combined.  In this
 *  case, you want to get the abbreviations from the master clue table
 *  of all words in the string...  eg:  123 mountain beach rd
 *                                      123 mt bch rd
 */
    else {
        if ((mp = search_master_clues(cword,intok)) == NULL) {
            fprintf (stderr,"ERROR:  master entry wasnt found for\n");
            fprintf (stderr,"clue word id %d, clue type %s\n",cword,intok);
            assert(0);
        }
/*
 *      If abbrev_opt is 'E', 'F', 'S', 'H', or 'U' then get the
 *      abbrev. from the master clue table.
 *      EXCEPTION:  if using abbrev opt 'F', then dont expand directions.
 *                  Example:  "E street" was incorrectly matching to
 *                            "East street"
 */
        switch (abbrev_opt) {
/*
           case 'F':  addfield = 
                      (strcmp(intok,"DR")==0 ? mp->usps_abbrev : mp->full_name);
                      break;
*/
           case 'F':  addfield = mp->full_name; break;
           case 'S':  addfield = mp->std_abbrev; break;
           case 'H':  addfield = mp->short_abbrev; break;
           case 'U':  addfield = mp->usps_abbrev; break;
           case 'E':  addfield = mp->usps_abbrev; break;
        }
    }
/*
 *  if combine == spaces then add a space between concatenations,
 *  else if combine == squash, then add nothing,
 *  else if combine == asterisk, then add an asterisk,
 *  else if combine == dash then add a dash ONLY WHEN concatenating
 *  integers, otherwise add nothing.
 */
    len = trimln(fld);
    sprintf(hold,"%.*s",len,fld);
    if (len == 0 || combine == squash) {
        len2 = 0; strcpy(padch,"  ");
    }
    else if (combine == spaces) {
        len2 = 1; strcpy(padch,"  ");
    }
    else if (combine == asterisk) {
        len2 = 1; strcpy(padch,"* ");
    }
    else {
        if (isdigit(*(fld+len-1)) && isdigit(*addfield)) {
            len2 = 1;
            strcpy(padch,"- ");
        }
        else {
            len2 = 0;
            strcpy(padch,"  ");
        }
    }
    sprintf(hold+len,"%.*s%.*s\0",len2,padch,trimln(addfield),addfield);
    sprintf(fld,"%.*s\0",maxlen-1,hold);
}




/****************************************************************/
/*								*/
/*  loadStorageField is called only when loading the Storage	*/
/*  name field.  It searches the clue table for the image and	*/
/*  checks the storage flag:  If it's '*', then the name	*/
/*  is loaded from the standard abbreviation field, and the	*/
/*  function returns TRUE, otherwise FALSE.			*/
/*								*/
/****************************************************************/
int loadStorageField(fld,maxlen,cword,image,abb,state,zip)
char *image, *fld, *state, *zip, abb;
int maxlen, *cword;
{
    struct clue_table *ct, *search_clue_table();
    char hold[MAX_CLUE_RECLEN], outstring[5];
    char fhold[MAX_LENGTH_OF_ANY_OUTPUT_FIELD];
    short len = 0;
    extern char *upCase();
    
/*
 *  If the clue word id for the token is 0, then a separate
 *  storage name can't exist, hence, return FALSE.
 */
    if (*cword == 0) return(FALSE);
    upCase(image,hold);

    if ((ct = search_clue_table(upCase(image,hold),state,zip)) == NULL) {
        fprintf (stderr,"ERROR:  clue word entry wasnt found for\n");
        fprintf (stderr,"input image: %s\n",image);
        assert(0);
    }

    if (ct->store_flag == '*') {

        len = trimln(fld);
        sprintf (fhold,"%.*s",len,fld);
        sprintf (fhold+len,"%.*s%.*s\0",
        (trimln(fld)==0?0:1)," ",trimln(ct->translation),ct->translation);
        sprintf(fld,"%.*s\0",maxlen-1,fhold);

        if (abb == 'E') {
            strcpy(image,ct->translation);
            *cword = 0;
        }

        return (TRUE);
    }

    return (FALSE);
}



/****************************************************************/
/*								*/
/*  getClueWordID is sent an input token, intok, and will 	*/
/*  determine its clue word id using the information loaded	*/
/*  in the token struct, tst.					*/
/*								*/
/****************************************************************/
int getClueWordID(tst,intok)
char *intok;
struct token_struct *tst;
{
    if (strncmp(tst->clue_type_1,intok,2) == 0)
        return (tst->clue_word_id1);
    else if (strncmp(tst->clue_type_2,intok,2) == 0)
        return (tst->clue_word_id2);
    else if (strncmp(tst->clue_type_3,intok,2) == 0)
        return (tst->clue_word_id3);
    else if (strncmp(tst->clue_type_4,intok,2) == 0)
        return (tst->clue_word_id4);
    else if (strncmp(tst->clue_type_5,intok,2) == 0)
        return (tst->clue_word_id5);
    else {
        fprintf (stderr,"ERROR:  The input token %s for the current\n",intok);
        fprintf (stderr,"input image %s could not be found in its\n",
        tst->input_image);
        fprintf (stderr,"respective token structure.\n");
        assert(0);
    }
}



/****************************************************************/
/*								*/
/*  initAddressStruct simply initializes the individual fields	*/
/*  in the address structure, sa.				*/
/*								*/
/****************************************************************/
void initAddressStruct(sa,flds)
struct standardized_address *sa;
int flds;
{
    if (flds != allButH) memset(sa->hn,'\0',NUMBER_OUTPUT_FIELD_SIZE);
    if (flds != allButH) memset(sa->hn2,'\0',NUMBER_OUTPUT_FIELD_SIZE);
    if (flds != allButH) memset(sa->hnp,'\0',PREFIX_OUTPUT_FIELD_SIZE);
    if (flds != allButH) memset(sa->hnp2,'\0',PREFIX_OUTPUT_FIELD_SIZE);
    if (flds != allButH) memset(sa->hns,'\0',SUFFIX_OUTPUT_FIELD_SIZE);
    memset(sa->snpd,'\0',DIRECTION_OUTPUT_FIELD_SIZE);
    memset(sa->snpt,'\0',TYPE_OUTPUT_FIELD_SIZE);
    memset(sa->osn,'\0',NAME_OUTPUT_FIELD_SIZE);
    memset(sa->msn,'\0',NAME_OUTPUT_FIELD_SIZE);
    memset(sa->ssn,'\0',NAME_OUTPUT_FIELD_SIZE);
    memset(sa->snst,'\0',TYPE_OUTPUT_FIELD_SIZE);
    memset(sa->snsd,'\0',DIRECTION_OUTPUT_FIELD_SIZE);
    memset(sa->sne,'\0',EXTENSION_OUTPUT_FIELD_SIZE);
    if (flds != allButW) memset(sa->wsd,'\0',TYPE_OUTPUT_FIELD_SIZE);
    if (flds != allButW) memset(sa->wsi,'\0',NUMBER_OUTPUT_FIELD_SIZE);
    memset(sa->ppd,'\0',DIRECTION_OUTPUT_FIELD_SIZE);
    memset(sa->ppt,'\0',TYPE_OUTPUT_FIELD_SIZE);
    memset(sa->opn,'\0',NAME_OUTPUT_FIELD_SIZE);
    memset(sa->mpn,'\0',NAME_OUTPUT_FIELD_SIZE);
    memset(sa->pst,'\0',TYPE_OUTPUT_FIELD_SIZE);
    memset(sa->psd,'\0',DIRECTION_OUTPUT_FIELD_SIZE);
    memset(sa->sd,'\0',TYPE_OUTPUT_FIELD_SIZE);
    memset(sa->si,'\0',NUMBER_OUTPUT_FIELD_SIZE);
    memset(sa->rrd,'\0',TYPE_OUTPUT_FIELD_SIZE);
    memset(sa->rri,'\0',NUMBER_OUTPUT_FIELD_SIZE);
    memset(sa->bxd,'\0',TYPE_OUTPUT_FIELD_SIZE);
    memset(sa->bxi,'\0',NUMBER_OUTPUT_FIELD_SIZE);
    memset(sa->ei,'\0',EXTRAINFO_OUTPUT_FIELD_SIZE);

    sa->snpd_c = sa->snpt_c = sa->snst_c = sa->snsd_c = 
    sa->ppd_c = sa->ppt_c = sa->pst_c = 
    sa->psd_c = sa->rrd_c = sa->bxd_c = sa->sne_c = 0;

    if (flds == all) sa->wsd_c = 0;
}


/****************************************************************/
/*								*/
/*  stripLeadingZeros will strip leading zeros from numbers     */
/*  except for ones in the Secondary House Number field.        */
/*								*/
/****************************************************************/
void stripLeadingZeros(image)
char *image;
{
    int len,i,j,nonZfound=FALSE;
    char temp[NUMBER_OUTPUT_FIELD_SIZE];

    if (*image != '0') return;

    len = strlen(image);
    for (i=0,j=0; i<len; i++) {
        if (*(image+i) != '0' || nonZfound) {
            nonZfound = TRUE;
            temp[j++] = *(image+i);
        }
    }
    temp[j] = '\0';
/*
 *  Only strip leading zeros if at least 1 non-zero digit was found
 */
    if (nonZfound) strcpy(image,temp);
}



/****************************************************************/
/*								*/
/*  fractionWithinNumber is sent a character string, string,	*/
/*  and will return the fraction embedded in the string if one	*/
/*  exists, and NULL otherwise.					*/
/*								*/
/****************************************************************/
char *fractionWithinNumber(string)
char *string;
{
    char *slpos, *blpos;

    slpos = strchr(string,'/');
    blpos = strchr(string,' ');
    if (slpos != NULL  &&  blpos != NULL) {
/*
 *      If you found a fraction, then return it after first
 *      eliminating it from the original number
 */
        *blpos = '\0';

        return(slpos-1);
    }
    return(NULL);
}



/****************************************************************/
/*								*/
/*  checkDirections is only valid if both a prefix and a suffix	*/
/*  direction were loaded and if the suffix direction is	*/
/*  is n, s, w, or e AND the street name isn't all numeric	*/
/*  (this check saves UTAH addresses) and no wsi currently	*/
/*  exists.							*/
/*  If so, the suffix direction is moved			*/
/*  over to the within structure identifier field.  This will	*/
/*  prevent the following address from have 2 directions:	*/
/*                                                              */
/*  123 n main ave s:						*/
/*                                                              */
/*   INPUT TOKENS  -  NU DR AU TY  |  DR 			*/
/*  OUTPUT TOKENS  -  HN PD NA ST  |  SD  			*/
/*  PATTERN TYPES  -  H*           |  H+			*/
/*								*/
/****************************************************************/
int checkDirections(sa,oSD,oSDcword,abb)
struct standardized_address *sa;
char *oSD, abb;
int oSDcword;
{
    extern int allDigits();

    if (*sa->snpd != '\0' && *sa->snsd != '\0' &&  !allDigits(sa->osn)) {
        if (strcmp(oSD,"N")==0 || strcmp(oSD,"n")==0 ||
        strcmp(oSD,"S")==0 || strcmp(oSD,"s")==0 ||
        strcmp(oSD,"E")==0 || strcmp(oSD,"e")==0 ||
        strcmp(oSD,"W")==0 || strcmp(oSD,"w")==0) {
            if (*sa->wsi != '\0') strcat(sa->wsi,"*");
            strcat(sa->wsi,oSD);
            memset(sa->snsd,'\0',DIRECTION_OUTPUT_FIELD_SIZE);
            sa->snsd_c = 0;
        }
    }
    return(TRUE);
}



/****************************************************************/
/*								*/
/*  If the prefix type is Bsrt (or some other deviation)	*/
/*  then the type is changed to rt and "business" is placed in	*/
/*  the extension field.					*/
/*								*/
/****************************************************************/
int checkPT(tst,cword,itok,abb,sa)
struct token_struct *tst;
int cword;
char *itok, abb;
struct standardized_address *sa;
{
    if (cword == 33 && strncmp(itok,"HR",2)==0 ) {

        loadField(sa->sne,EXTENSION_OUTPUT_FIELD_SIZE,
        " ",6,"EX",abb,0);
        sa->sne_c = 6;

        loadField(sa->snpt,TYPE_OUTPUT_FIELD_SIZE,
        " ",227,"HR",abb,0);
        sa->snpt_c = 227;

        return(TRUE);
    }

    return(FALSE);
}





/****************************************************************/
/*								*/
/*  checkPTtoNA is only called when load Prefix Types.		*/
/*  If the input image is ST, LE, or LA then it's placed in	*/
/*  the name field.  EX:  101 st barnabas rd			*/
/*                         HN PT NA       ST			*/
/*  Here, "st" obviously belongs in the name field.		*/
/*								*/
/****************************************************************/
int checkPTtoNA(tst,cword,itok,otok)
struct token_struct *tst;
int *cword;
char *itok, *otok;
{
    extern char *upCase();
    char hold[MAX_CLUE_RECLEN];
    int len;

    len = trimln(tst->input_image);
    if (strncmp(upCase(tst->input_image,hold),"ST",len) == 0) {
        strcpy(itok,"AU"); strcpy(otok,"NA"); *cword = 79; return(TRUE);
    }
    else if (strncmp(upCase(tst->input_image,hold),"LE",len) == 0) {
        strcpy(itok,"AU"); strcpy(otok,"NA"); *cword = 0; return(TRUE);
    }
    else if (strncmp(upCase(tst->input_image,hold),"LA",len) == 0) {
        strcpy(itok,"AU"); strcpy(otok,"NA"); *cword = 0; return(TRUE);
    }
    return(FALSE);
}



/************************************************************************/
/*									*/
/*  checkHouseNumber is checks the PrimaryHouseNumber field to		*/
/*  see if multiple numbers have been added;  if so, it splits		*/
/*  them up into PrimaryHouseNumber and SecondaryHouseNumber.		*/
/*  EX:  101 120 main st.  Here, "101 120" will have all been		*/
/*  placed in the PrimaryHouseNumber field.				*/
/*  Also, if H1 and H2 are present and P2 is blank,        	 	*/
/*  then add '-' to P2.  This will handle the QUEENS style		*/
/*  addresses.								*/
/*  Also, check for H1 = "0".                                   	*/
/*									*/
/************************************************************************/
void checkHouseNumber(sa)
struct standardized_address *sa;
{
    char *blpos;

    if ((blpos = strchr(sa->hn,' ')) != NULL) {
/*
 *      If you found a second house number , then return it after
 *      first eliminating it from the original number
 */
        *blpos = '\0';
        sprintf(sa->hn2,"%.*s\0",trimln(blpos+1),blpos+1);
    }

    if (*sa->hn != '\0' && *sa->hn2 != '\0' 
    && *sa->hnp2 == '\0')
        *sa->hnp2 = '-';
/*
 *  Check for a "0" in H1
    if (strcmp(sa->hn,"0")==0) {
        *sa->hn = *sa->hnp = *sa->hnp2 = '\0';
        if (*sa->hn2 != '\0') {
            strcpy(sa->hn,sa->hn2);
            *sa->hn2 = '\0';
        }
    }
    else if (*sa->hn == '0') {
        stripLeadingZeros(sa->hn);
    }
 */
    if (*sa->hn == '0' && (strcmp(sa->hn,"0")!=0) ) 
        stripLeadingZeros(sa->hn);

}


/****************************************************************/
/*								*/
/*  checkForSpelledOutNumber will check all NU tokens for	*/
/*  "spelled out" numbers (ie. ONE, TWO, etc) and will change	*/
/*  them to numerics, and spelled out ordinals (ie. FIRST,	*/
/*  SECOND, etc) and change them to numeric.			*/
/*								*/
/****************************************************************/
void checkForSpelledOutNumber(tst,state,zip)
struct token_struct *tst;
char *state, *zip;
{
    struct clue_table *search_clue_table(), *clue;
    extern char *upCase();
    char hold[MAX_CLUE_RECLEN];

    if (strncmp(tst->clue_type_2,"NU",2)==0 || strncmp(tst->clue_type_2,"OT",2)==0 ) {
        if ((clue=search_clue_table(upCase(tst->input_image,hold),state,zip)) == NULL) return;
        strncpy(tst->input_image,clue->translation,trimln(tst->input_image));
    }

}


/****************************************************************/
/*								*/
/*  checkWithinStructure will place a "#" in the wsd field if	*/
/*  wsd is blank and wsi is non-blank.  Also, the wsi field	*/
/*  is squashed if needed.					*/
/*  If no wsi exists and wsd != blank, then copy wsd to wsi.    */
/*								*/
/****************************************************************/
void checkWithinStructure(sa,abb)
struct standardized_address *sa;
char abb;
{
    extern int stan_iden();
    char outstr[NUMBER_OUTPUT_FIELD_SIZE];

    if (*(sa->wsi) != '\0') {
        if (*(sa->wsd) == '\0') {
            loadField(sa->wsd,TYPE_OUTPUT_FIELD_SIZE,"#",0,NULL,
            abb,spaces);
        }
        stan_iden(sa->wsi,outstr);
/*
        printf (" input string:  %s\n",sa->wsi);
        printf ("output string:  %s\n",outstr);
*/
        strcpy(sa->wsi,outstr);
    }
    else if (*(sa->wsd) != '\0') {
        if (*sa->wsd == '#')
            loadField(sa->ei,EXTRAINFO_OUTPUT_FIELD_SIZE,"#",0,"WD",'S',spaces);
        else
            strcpy(sa->wsi,sa->wsd);
        memset(sa->wsd,'\0',TYPE_OUTPUT_FIELD_SIZE);
    }

}



/****************************************************************/
/*								*/
/*  checkBuildingNumber will check to see if any of the		*/
/*  following fields have values:				*/
/*      1.  Building Number Prefix				*/
/*      2.  Building Number      				*/
/*      3.  Building Number Suffix 				*/
/*								*/
/*  If so, all 3 of them must be moved because these fields	*/
/*  are no longer valid.  If the WithinStructureIdentifier	*/
/*  field is blank, then put them there, otherwise put them	*/
/*  in the new StructureIdentifier field.			*/
/*								*/
/****************************************************************/
void checkBuildingNumber(sa,BNum,BNumPre,BNumSuf)
struct standardized_address *sa;
char *BNum, *BNumPre, *BNumSuf;
{
   char *tp, hold[30];

   if (*BNumPre == '\0' && *BNum == '\0' && *BNumSuf == '\0')
       return;

   sprintf (hold,"%.*s%.*s%.*s\0",trimln(BNumPre),
   BNumPre,trimln(BNum), BNum,trimln(BNumSuf), BNumSuf);

   strcpy( (*sa->wsi == '\0' ? sa->wsi : sa->si), hold);
}



void checkBox(sa,abb)
struct standardized_address *sa;
char abb;
{

/*
 *  If bxi has a value and bxd is blank then place "box" in bxd
 */
    if (*(sa->bxi) != '\0' && *(sa->bxd) == '\0') {
        loadField(sa->bxd,TYPE_OUTPUT_FIELD_SIZE,"Box",1,"BX",'S',spaces);
        sa->bxd_c = 1;
    }

}



/****************************************************************/
/*								*/
/*  If the usage flag is C (commercial only) and only the       */
/*  street name field was loaded, then move this all to the     */
/*  building name.  Also, check for numerics and ordinal        */
/*  numbers in the street name field and expand if necessary.   */
/*								*/
/****************************************************************/
void checkStreetName(sa,NAonly,usf)
struct standardized_address *sa;
int NAonly;
char usf;
{
    int i,p,ln,ln2,correctOTcontext();
    char ordtyp[3], hold_nm[NAME_OUTPUT_FIELD_SIZE+1];
    char hold_nm2[NAME_OUTPUT_FIELD_SIZE+1], *apos;
    extern int allDigits();
    extern void matchNumberWithOT();
    extern char *upCase();

/*
 *  Check for "E", "W", "N", or "S" in the matching name and 
 *  change to "EAST", "WEST", "NORTH", or "SOUTH"
 */
    upCase(sa->msn,hold_nm);
    upCase(sa->osn,hold_nm2);
    if (strcmp(hold_nm,hold_nm2) != 0) {
        if (strcmp(hold_nm,"N")==0) strcpy(sa->msn,"North");
        if (strcmp(hold_nm,"S")==0) strcpy(sa->msn,"South");
        if (strcmp(hold_nm,"E")==0) strcpy(sa->msn,"East");
        if (strcmp(hold_nm,"W")==0) strcpy(sa->msn,"West");
    }
/*
 *  Check for spelled out OT's in the storage and matching names
 */
    if (correctOTcontext(sa->osn,hold_nm)) {
        memset(sa->msn,'\0',NAME_OUTPUT_FIELD_SIZE);
        memset(sa->ssn,'\0',NAME_OUTPUT_FIELD_SIZE);
        strcpy(sa->msn,hold_nm);
        strcpy(sa->ssn,hold_nm);
    }
/*
 *  Check for all numeric in the street name.  If so, add its
 *  proper OT.  NOTE:  this is only valid if there is no
 *  prefix type (this is to prevent "123 st hwy 4" from being
 *  butchered), or if a UTAH address is present (PD and SD both
 *  exists), or if no extension is present (EX must be blank: 
 *  123 old 5).
 */
    if (*sa->snpt == '\0' && *sa->sne == '\0' &&
    (*sa->snpd == '\0' || *sa->snsd == '\0')) {
        if (*sa->osn != '\0' && allDigits(sa->osn)) {
            matchNumberWithOT(sa->osn,ordtyp);
            ln = trimln(sa->osn);
            strcpy(sa->osn + ln, ordtyp);
        }
        if (*sa->msn != '\0' && allDigits(sa->msn)) {
            matchNumberWithOT(sa->msn,ordtyp);
            ln = trimln(sa->msn);
            strcpy(sa->msn + ln, ordtyp);
        }
        if (*sa->ssn != '\0' && allDigits(sa->ssn)) {
            matchNumberWithOT(sa->ssn,ordtyp);
            ln = trimln(sa->ssn);
            strcpy(sa->ssn + ln, ordtyp);
        }
    }
/*
 *  Check for building names in the street name field
 */
    if (usf == 'C' && NAonly) {
        strcpy(sa->opn,sa->osn);
        strcpy(sa->mpn,sa->msn);
        memset(sa->osn,'\0',NAME_OUTPUT_FIELD_SIZE);
        memset(sa->msn,'\0',NAME_OUTPUT_FIELD_SIZE);
        memset(sa->ssn,'\0',NAME_OUTPUT_FIELD_SIZE);
    }

/*
 *  If usage flag is street name only (T) then append extra information
 *  field, if any, to the street name fields
 */
    if (usf == 'T' && *sa->ei != '\0') {
        ln = trimln(sa->ei);
        ln2 = trimln(sa->osn);
        sprintf(sa->osn + ln2," %.*s",ln,sa->ei);
        ln2 = trimln(sa->msn);
        sprintf(sa->msn + ln2," %.*s",ln,sa->ei);
        ln2 = trimln(sa->ssn);
        sprintf(sa->ssn + ln2," %.*s",ln,sa->ei);
        memset(sa->ei,'\0',EXTRAINFO_OUTPUT_FIELD_SIZE);
    }


}



/****************************************************************/
/*								*/
/*  correctOTcontext will check the string for "spelled out"	*/
/*  ordinal types, eg. "fifty second" and will change them	*/
/*  to their numeric ordinal types, eg. 52nd.			*/
/*								*/
/****************************************************************/
int correctOTcontext(str,out)
char *str, *out;
{
    extern char *upCase();
    char hold[MAX_CLUE_RECLEN];
    int lenOT1[] = {6,6,5,5,5,7,6,6};
    static char *spelledOT1[] = {"2TWENTY", "3THIRTY", "4FORTY", "5FIFTY",
                         "6SIXTY", "7SEVENTY", "8EIGHTY", "9NINETY"};
    static char *spelledOT2[] = {"1stFIRST", "2ndSECOND", "3rdTHIRD", 
                          "4thFOURTH", "5thFIFTH", "6thSIXTH", "7thSEVENTH", 
                          "8thEIGHTH", "9thNINTH"};
    int i,j;

    upCase(str,hold);
/*
 *  Change FIFTY SECOND to 52nd, 
 *         THIRTY FIRST to 31st, etc.
 */
    for (i=0; i<8; i++) {
        if ( strncmp( (*(spelledOT1+i)+1), hold, lenOT1[i]) == 0) {
            if (strlen(hold) <= lenOT1[i]) continue;
            for (j=0; j<9; j++) {
                if ( strncmp( (*(spelledOT2+j)+3), hold+lenOT1[i]+1,
                strlen(hold+lenOT1[i]+1) ) == 0) {
                    strncpy(out,*(spelledOT1 + i),1);
                    strncpy(out+1,*(spelledOT2 + j),3);
                    *(out+4) = '\0';
                    return(TRUE);
                }
            }
        }
    }

    return(FALSE);
} 
