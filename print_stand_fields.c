#include "stand.h"
#include "clues.h"


void print_stand_fields(sa)
struct standardized_address *sa;
{
  struct master *ent;

/*
 *  print out individual fields, in row format
 */

  if (strncmp(sa->hnp,"\0",1) != 0)
    printf ("        Primary Prefix:  %s\n",sa->hnp);
  if (strncmp(sa->hn,"\0",1) != 0)
    printf ("  Primary House Number:  %s\n",sa->hn);
  if (strncmp(sa->hnp2,"\0",1) != 0)
    printf ("      Secondary Prefix:  %s\n",sa->hnp2);
  if (strncmp(sa->hn2,"\0",1) != 0)
    printf ("Secondary House Number:  %s\n",sa->hn2);
  if (strncmp(sa->hns,"\0",1) != 0)
    printf ("   House Number Suffix:  %s\n",sa->hns);
  if (strncmp(sa->snpd,"\0",1) != 0)
    printf ("      Prefix Direction:  %s\n",sa->snpd);
  if (strncmp(sa->snpt,"\0",1) != 0)
    printf ("           Prefix Type:  %s\n",sa->snpt);
  if (strncmp(sa->osn,"\0",1) != 0)
    printf ("  Original Street Name:  %s\n",sa->osn);
  if (strncmp(sa->msn,"\0",1) != 0)
    printf ("  Matching Street Name:  %s\n",sa->msn);
  if (strncmp(sa->ssn,"\0",1) != 0)
    printf ("   Storage Street Name:  %s\n",sa->ssn);
  if (strncmp(sa->snst,"\0",1) != 0)
    printf ("           Suffix Type:  %s\n",sa->snst);
  if (strncmp(sa->snsd,"\0",1) != 0)
    printf ("      Suffix Direction:  %s\n",sa->snsd);
  if (strncmp(sa->sne,"\0",1) != 0)
    printf ("    Extended Extension:  %s\n",sa->sne);

  if (strncmp(sa->wsd,"\0",1) != 0) {
    printf (" Within Structure Desc:  %s\n",sa->wsd);
  }
  if (strncmp(sa->wsi,"\0",1) != 0) {
    printf ("   Within Structure Id:  %s\n",sa->wsi);
  }


  if (strncmp(sa->ppd,"\0",1) != 0)
    printf ("   Property Prefix Dir:  %s\n",sa->ppd);
  if (strncmp(sa->ppt,"\0",1) != 0)
    printf ("  Property Prefix Type:  %s\n",sa->ppt);
  if (strncmp(sa->opn,"\0",1) != 0)
    printf ("Original Property Name:  %s\n",sa->opn);
  if (strncmp(sa->mpn,"\0",1) != 0)
    printf ("Matching Property Name:  %s\n",sa->mpn);
  if (strncmp(sa->pst,"\0",1) != 0)
    printf ("  Property Suffix Type:  %s\n",sa->pst);
  if (strncmp(sa->psd,"\0",1) != 0)
    printf ("   Property Suffix Dir:  %s\n",sa->psd);
  if (strncmp(sa->sd,"\0",1) != 0)
    printf ("  Structure Descriptor:  %s\n",sa->sd);
  if (strncmp(sa->si,"\0",1) != 0)
    printf ("  Structure Identifier:  %s\n",sa->si);


  if (strncmp(sa->rrd,"\0",1) != 0)
    printf ("Rural Route Descriptor:  %s\n",sa->rrd);
  if (strncmp(sa->rri,"\0",1) != 0)
    printf ("Rural Route Identifier:  %s\n",sa->rri);
  if (strncmp(sa->bxd,"\0",1) != 0)
    printf ("        Box Descriptor:  %s\n",sa->bxd);
  if (strncmp(sa->bxi,"\0",1) != 0)
    printf ("        Box Identifier:  %s\n",sa->bxi);
  if (strncmp(sa->ei,"\0",1) != 0)
    printf ("     EXTRA INFORMATION:  %s\n",sa->ei);

}

