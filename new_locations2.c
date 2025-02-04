#include	<stdio.h>

void new_locations2(start_point,to_file)
long	start_point;
FILE	*to_file;
{
fprintf(to_file,"\t\t\t\tStart\tLength\n");
fprintf(to_file,"House Number\t\t\t%d\t6\n",start_point + 1);
fprintf(to_file,"Street Name Prefix\t\t%d\t5\n",start_point + 7);
fprintf(to_file,"Street Name\t\t\t%d\t20\n",start_point + 12);
fprintf(to_file,"Street Name Suffix\t\t%d\t10\n",start_point + 32);
fprintf(to_file,"Within Structure Designator\t%d\t4\n",start_point + 42);
fprintf(to_file,"Within Structure Indicator\t%d\t5\n",start_point + 46);
fprintf(to_file,"Rural Route Designator\t\t%d\t3\n",start_point + 51);
fprintf(to_file,"Rural Route Indicator\t\t%d\t5\n",start_point + 54);
fprintf(to_file,"Box Designator\t\t\t%d\t4\n",start_point + 59);
fprintf(to_file,"Box Indicator\t\t\t%d\t5\n",start_point + 63);
return;
}
