#define	MAX_INPUT_SIZE		400
#define	NULL_POINTER		'\0'
#define	SPACE			' '
#define	NULL50	"                                                            "
#define	WORDS			30	/* maximum number of words	*/
#define	CONJDEX			25	/* conjunctions			*/
#define	JRSRDEX			25	/* jr/srs			*/
#define	NICKDEX			1600	/* nicknames			*/
#define	PREDEX			30	/* prefixes			*/
#define	TITLDEX			25	/* titles			*/
#define	SUFDEX			25	/* suffixes			*/
#define	SKPDEX			1500	/* skip list words		*/
#define	PTRNDEX1		1200	/* patterns			*/
#define	PTRNDEX2		10
#define	TWODEX			16
#define	THREDEX			16
#define	CONJSIZE		10
#define	PTRN1SIZE		20
#define	BLNKDEX			16	/* blank list			*/
#define	TITLE			0
#define	FIRST			1
#define	MIDDLE			2
#define	PREFLAST		3
#define	LAST			4

#define isnotdelim(c)		(isalpha(c) || isdigit(c) || (c=='&') || (c=='-') || (c=='%') || (c=='+'))

char *conj[CONJDEX],*jrsrcd[JRSRDEX],*new_name[NICKDEX],
     *orig_name[NICKDEX],*prefix[PREDEX],*ptrn1[PTRNDEX1],
     *skplst[SKPDEX],*suffix[SUFDEX],*title[TITLDEX],
     *two_suff[TWODEX],*three_suff[THREDEX],*blnk[BLNKDEX],
     namefld[60],nameup[60],fullfirst[WORDS][17],
     jrsrflag[WORDS][2],wordtype[WORDS][2],partnerflag[WORDS][2],
     skipflag[WORDS][2],a_filename[100],*jrsr2[JRSRDEX],*skplst2[SKPDEX];

/* modified data structure standname

struct nametype {
  char title[91];
  char first[91];
  char middle[91];
  char prefix[91];
  char last[91];
};
*/

long  ptrn2[PTRNDEX1][PTRNDEX2],n_titl,n_conj,n_nick,n_jrsr,n_patt,
     n_suff,n_pref,n_skpl,n_two,n_three,n_blnk,
     numwords,wdlen[WORDS],
     wdstrt[WORDS],wdend[WORDS],commaflag[WORDS];
/*   start[9]={1,7,20,33,33,47,53,58,71},
     fin[9]={6,19,32,46,46,52,57,70,83},
     stanpass;  */
 long n_stan,n_bus;

  /*  CHARACTER*16 FULLFIRST(N)     ! Full first name/not nickname
                                    ! COMMON status
      CHARACTER*1  JRSRFLAG(N)      ! 0 = not present
                                    ! 1 = Jr/2nd
                                    ! 2 = 3rd
                                    ! 3 = 4th
                                    ! 4 = senior
      CHARACTER*1  LNAMEFLAG        ! 0 = last name not found yet
                                    ! 1 = last name has been located
      CHARACTER*1  PARTNERFLAG(N)   ! Possible partnership flag
                                    ! 1 = possible partnership
                                    ! 2 = possible corporation
      CHARACTER*1  PREFIXFLAG(N)    ! 1 = on prefix list
                                    ! 2 = on prefix list,may be 1st name
      CHARACTER*1  SKIPFLAG(N)      ! 0 = not a skip word
                                    ! 1 = deceased
                                    ! 2 = church type organization
                                    ! 3 =
                                    ! 4 =
      CHARACTER*1  WORDTYPE(N)      ! Type of word
                                    ! 1 = conjunction
                                    ! 2 = Jr/Sr
                                    ! 3 = title
                                    ! 4 = suffix
                                    ! 5 = skip
                                    ! 6 = initial
                                    ! 7 = last name
                                    ! 8 = prefix
                                    ! 9 = first name    */


void read_d(long ptrn2[][PTRNDEX2]);
void name_split();
void dict_match();
long standn(long ptrn2[][PTRNDEX2],char local_standname[][91]);
extern long search_table(char data[],long low,long high,long length,char *c_table[]);
extern long s_table(char data[],long low,long high,char *c_table[]);

FILE	*fptri,*fptro,*fother;

