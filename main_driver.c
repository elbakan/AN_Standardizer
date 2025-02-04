#include "patterns.h"


main(ac,av)
int ac;
char *av[];
{
    char colstr[130], string[125], abbrev_opt, usage_flag;
    int i,j,len;
    void Addr_Stan2_Init();
    void print_stand_fields();
    struct standardized_address *sa, *Addr_Stan2();
    char *dashes="------------------------------------------------------------------------------------------------------";


/*
 *  Call the standardizer initialization function
 */
    Addr_Stan2_Init();

/*
 *  Set the usage flag to ' ' (residential or commercial), and the abbreviation
 *  option to 'f' (full names returned)
 */
    usage_flag = ' ';
    abbrev_opt = 'u';

/*
 *  Ask the user for an address
 */
    for (;;) {
       printf ("\n\n\nEnter the address (<cr> to exit): ");
       if (gets(string) == NULL || string[0] == NULL) break;
       if (strchr(string,'!') != NULL) *(strchr(string,'!')) = '\0';
       len = trimln(string)+18;
       printf ("\n");
       printf ("%.72s\n",dashes);
       printf ("            ADDRESS ENTERED:  %s\n",string);
       printf ("%.72s\n",dashes);

       string[trimln(string)] = '\0';
/*
 *     Standardize the address
 */
       sa = 
       Addr_Stan2(string,usage_flag,abbrev_opt);


/*
 *     print out individual fields, in row format (if so desired)
 */
       print_stand_fields(sa);
    }
}
