#ifndef TOKEN__
#define TOKEN__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define IMAGE_SIZE (long)50                  /*  maximum size of an input image    */
#define MAX_INPUT_TOKENS (long)39            /*  maximum number of input tokens    */
#define MAX_WORDS (long)30                   /*  maximum number of words in an
                                           address                           */
#define MAX_WORDSINCLUE (long)5              /*  maximum number of words in a
                                           single clue (EX: "ave of the"
                                           has 3                             */

struct token_struct {
   char input_image[IMAGE_SIZE];       /*  input image                       */
   unsigned long clue_word_id1;         /*  clue word id 1                    */
   char clue_type_1[3];                /*  clue type 1                       */
   unsigned long clue_word_id2;         /*  clue word id 2                    */
   char clue_type_2[3];                /*  clue type 2                       */
   unsigned long clue_word_id3;         /*  clue word id 3                    */
   char clue_type_3[3];                /*  clue type 3                       */
   unsigned long clue_word_id4;         /*  clue word id 4                    */
   char clue_type_4[3];                /*  clue type 4                       */
   unsigned long clue_word_id5;         /*  clue word id 5                    */
   char clue_type_5[3];                /*  clue type 5                       */
   short beg;                          /*  beginning word position           */
   short end;                          /*  ending word position              */
};

#endif
