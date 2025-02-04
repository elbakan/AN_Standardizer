#ifndef PATTERNS__
#define PATTERNS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX(a,b)  ((a)<(b) ? (b) : (a))     /*  MAX macro                    */
#define MIN(a,b)  ((a)<(b) ? (a) : (b))     /*  MIN macro                    */
#define PATTERN_ARRAYSIZE 1100              /*  maximum  array size for the
                                                pattern table                */
#define PATTERN_RECLEN 82                   /*  record length for the
                                                pattern records              */
#define PATSIZE 40                          /*  in/out token string size     */
#define PATTERN_HASHSIZE 1100               /*  pattern table hash size      */
#define MAX_OUTPUT_PATTERNS 30              /*  maximum number of output
                                                sub-patterns                 */
#define NUMBER_OF_PATTERN_TYPES 20          /*  number of pattern types      */
#define NUMBER_OF_OUTPUT_TOKEN_TYPES 34     /*  number of output token types */
#define MAX_PATT_SIZE 100                   /*  maximum length of any pattern*/


struct pattern_table {
   char input_token_string[PATSIZE];        /*  input token string           */
   char output_token_string[PATSIZE];       /*  output token string          */
   char pattern_type[3];                    /*  pattern type                 */
   char priority[3];                        /*  priority                     */
   char state[3];                           /*  state code                   */
   char county[4];                          /*  county code                  */
   char place[5];                           /*  place code                   */
   char zip_code[6];                        /*  zip code                     */
   char language;                           /*  language                     */
   char usf;                                /*  usage flag                   */
   char exclude;                            /*  exclude flag                 */
   struct pattern_table *next;              /*  pointer to the next element
                                                in the linked list           */
};

struct output_pattern {
   char input_patt[PATSIZE];                /*  input pattern string         */
   char output_patt[PATSIZE];               /*  output pattern string        */
   char pattern_type[3];                    /*  pattern type                 */
   char priority[3];                        /*  priority                     */
   short beg;                               /*  beginning word position      */
   short end;                               /*  ending word position         */
};


/*
 *  Declare all external functions
 */

extern int trimln();
extern unsigned int patternHash();
extern char *stringdup();
extern struct pattern_table *searchForPattern();
extern struct pattern_table *addPattern();
extern int print_all_patterns;
extern int print_final_tokens;


#endif
