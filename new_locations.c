#include	<stdio.h>

void new_locations(start_point,to_file)
long	start_point;
FILE	*to_file;
{
fprintf(to_file,"\t\t\t\tStart\tLength\n");
fprintf(to_file,"House Number\t\t\t%ld\t6\n",start_point + 1);
fprintf(to_file,"Street Name Prefix\t\t%ld\t5\n",start_point + 7);
fprintf(to_file,"Street Name\t\t\t%ld\t20\n",start_point + 12);
fprintf(to_file,"Street Name Suffix\t\t%ld\t10\n",start_point + 32);
fprintf(to_file,"Within Structure Designator\t%ld\t4\n",start_point + 42);
fprintf(to_file,"Within Structure Indicator\t%ld\t5\n",start_point + 46);
fprintf(to_file,"Rural Route Designator\t\t%ld\t3\n",start_point + 51);
fprintf(to_file,"Rural Route Indicator\t\t%ld\t5\n",start_point + 54);
fprintf(to_file,"Box Designator\t\t\t%ld\t4\n",start_point + 59);
fprintf(to_file,"Box Indicator\t\t\t%ld\t5\n",start_point + 63);
fprintf(to_file,"Title\t\t\t\t%ld\t6\n",start_point + 68);
fprintf(to_file,"First Name\t\t\t%ld\t13\n",start_point + 74);
fprintf(to_file,"Middle Name\t\t\t%ld\t13\n",start_point + 87);
fprintf(to_file,"Last Name\t\t\t%ld\t14\n\n",start_point + 100);
return;
}
