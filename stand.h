#ifndef STAND__
#define STAND__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define MAX_LENGTH_OF_ANY_OUTPUT_FIELD 100  /*  max length of any output fld */
#define NAME_OUTPUT_FIELD_SIZE 80           /*  name fields                  */
#define NUMBER_OUTPUT_FIELD_SIZE 15         /*  number fields                */
#define DIRECTION_OUTPUT_FIELD_SIZE 15      /*  direction fields             */
#define TYPE_OUTPUT_FIELD_SIZE 25           /*  type fields                  */
#define PREFIX_OUTPUT_FIELD_SIZE 10         /*  prefix fields                */
#define SUFFIX_OUTPUT_FIELD_SIZE 10         /*  suffix fields                */
#define EXTENSION_OUTPUT_FIELD_SIZE 10      /*  extension fields             */
#define EXTRAINFO_OUTPUT_FIELD_SIZE 50      /*  extra information field      */


struct standardized_address {
   char hnp[PREFIX_OUTPUT_FIELD_SIZE];      /*  house number prefix          */
   char hn[NUMBER_OUTPUT_FIELD_SIZE];       /*  house number                 */
   char hnp2[PREFIX_OUTPUT_FIELD_SIZE];     /*  secondary house number
                                                prefix/separator             */
   char hn2[NUMBER_OUTPUT_FIELD_SIZE];      /*  secondary house number       */
   char hns[SUFFIX_OUTPUT_FIELD_SIZE];      /*  house number suffix          */
   char snpd[DIRECTION_OUTPUT_FIELD_SIZE];  /*  street name prefix -
                                                directional                  */
   char snpt[TYPE_OUTPUT_FIELD_SIZE];       /*  street name prefix -
                                                type                         */
   char osn[NAME_OUTPUT_FIELD_SIZE];        /*  original street name         */
   char msn[NAME_OUTPUT_FIELD_SIZE];        /*  matching street name         */
   char ssn[NAME_OUTPUT_FIELD_SIZE];        /*  storage street name          */
   char snst[TYPE_OUTPUT_FIELD_SIZE];       /*  street name suffix -
                                                type                         */
   char snsd[DIRECTION_OUTPUT_FIELD_SIZE];  /*  street name suffix -
                                                directional                  */
   char sne[EXTENSION_OUTPUT_FIELD_SIZE];   /*  street name extension
                                                indicator                    */
   char wsd[TYPE_OUTPUT_FIELD_SIZE];        /*  within structure descriptor  */
   char wsi[NUMBER_OUTPUT_FIELD_SIZE];      /*  within structure identifier  */
   char ppd[DIRECTION_OUTPUT_FIELD_SIZE];   /*  property description prefix -
                                                directional                  */
   char ppt[TYPE_OUTPUT_FIELD_SIZE];        /*  property description prefix -
                                                type                         */
   char opn[NAME_OUTPUT_FIELD_SIZE];        /*  original property description
                                                name                         */
   char mpn[NAME_OUTPUT_FIELD_SIZE];        /*  matching property description
                                                name                         */
   char pst[TYPE_OUTPUT_FIELD_SIZE];        /*  property description suffix -
                                                type                         */
   char psd[DIRECTION_OUTPUT_FIELD_SIZE];   /*  property description suffix -
                                                directional                  */
   char sd[TYPE_OUTPUT_FIELD_SIZE];         /*  structure descriptor         */
   char si[NUMBER_OUTPUT_FIELD_SIZE];       /*  structure identifier         */
   char rrd[TYPE_OUTPUT_FIELD_SIZE];        /*  rural route descriptor       */
   char rri[NUMBER_OUTPUT_FIELD_SIZE];      /*  rural route identifier       */
   char bxd[TYPE_OUTPUT_FIELD_SIZE];        /*  box descriptor               */
   char bxi[NUMBER_OUTPUT_FIELD_SIZE];      /*  box identifier               */
   char ei[EXTRAINFO_OUTPUT_FIELD_SIZE];    /*  extra information            */

    int snpd_c;                             /*  street name prefix -
                                                directional code             */
    int snpt_c;                             /*  street name prefix -
                                                type code                    */
    int snst_c;                             /*  street name suffix -
                                                type code                    */
    int snsd_c;                             /*  street name suffix -
                                                directional code             */
    int wsd_c;                              /*  within structure descriptor
                                                code                         */
    int ppd_c;                              /*  property description prefix -
                                                directional code             */
    int ppt_c;                              /*  property description prefix -
                                                type code                    */
    int pst_c;                              /*  property description suffix -
                                                type code                    */
    int psd_c;                              /*  property description suffix -
                                                direction code               */
    int rrd_c;                              /*  rural route descriptor code  */
    int bxd_c;                              /*  box descriptor code          */
    int sne_c;                              /*  street name extension
                                                indicator code               */
};

struct posstr {
    int beg_tok;
    int order;
};


#endif
