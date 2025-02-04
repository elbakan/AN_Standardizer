/*
**++
**
**  MODULE DESCRIPTION:
**
**      {@tbs@}
**
**  AUTHORS:
**
**      CHRISTINE CAIN
**
**  CREATION DATE:  01-03-94
**      
**
**
**  MODIFICATION HISTORY:
**
**      
**--
*/
#define TRUE 1
#define FALSE 0
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int stan_iden( char *str, char *outstr) {

    int	    numlen;
    int	    digit, digit1, digit2, prev_digit;
    int	    floor, level;
    int	    len, len2star, lenFirst,lenFl;
    int	    lenStr, lenStar, Len2star, lenBefstar;
    char    *star, *star3, *star2;
    char    temp[11];
    int	    type_iden=0;
    int	    count = 0, i = 0;
    int	    chr1Cnt = 0,chr2Cnt = 0,dig1Cnt = 0,dig2Cnt = 0;
    int	    befStar, aftStar,midlen;
    int     and, end;

    and = FALSE;
    strncpy(outstr, "         ", 9);

/****
      Determine length of input string and the 
      first position of an asterisk 
****/


    star = strchr (str, (int)'*');    /*..  23nd*Fl*5 --> *Fl*5  ..*/

    if( star == NULL) {
	strcpy(outstr, str);          /*..  Single character sent ..*/
	return;
    }


       floor = strncmp("Fl",str, 2);
       level = strncmp("Lvl", str, 3);
       if(floor == 0 || level == 0) 
       {
	 star2 = strchr (star+1, (int)'*');
	 if(star2 == NULL)
	 {
	    star2 = strchr (star, (int)'*');
	    len2star = strlen(star2);
	    strncpy(temp,star2+1,len2star-1);
	    *(temp+(len2star-1)) = '*';
	    if(floor == 0) strcpy(temp+len2star,"Fl");
	    if(level == 0) strcpy(temp+len2star,"Lvl");
	    strcpy(str, temp);
	 }
	 else 
	 {
	   star2 = strchr (star+1, (int)'*');
	   if(star2 != NULL ) 
	   {
	     len2star = strlen(star2);
	     lenStar = strlen(star);
 	     lenStr = strlen(str);
	     midlen = lenStar - len2star;
	     lenFirst = lenStr - lenStar + 1;
	     strncpy(temp,star+1,midlen);
	     strncpy(temp+midlen,str,lenFirst);
	     strcpy(temp+midlen+lenFirst,star2+1);
	     strcpy(str,temp);
	     
	   }
	 }
       }           /* End floor or level */

/***
**** Flips the input:   3*Fl4th ----->> to 4th*Fl*3
**** which the following code can handle
****/

       floor = strncmp("Fl",star+1, 2);
       level = strncmp("Lvl", star+1, 3);
       
       star2 = strchr (star+1, (int)'*');
       if(strncmp("nd", star-2, 2) != 0 &&
          strncmp("ND", star-2, 2) != 0 &&
          strncmp("rd", star-2, 2) != 0 &&  /* rd, th, st or nd */
          strncmp("RD", star-2, 2) != 0 &&  /* as in --> 22nd   */
          strncmp("th", star-2, 2) != 0 &&
          strncmp("TH", star-2, 2) != 0 &&
          strncmp("st", star-2, 2) != 0 &&
          strncmp("ST", star-2, 2) != 0)  
	{
	 if(floor == 0)
	 {
	    if(digit = isdigit(*(star+4)) != 0) 
	    {
		lenStar = strlen(star);
		lenStr = strlen(str);
		lenFirst = lenStr - lenStar;
		lenFl = lenStr - lenFirst - 4;
		strncpy(temp,star+4,lenFl);
		*(temp+lenFl) = '*';
		strncpy(temp+lenFl+1,star+1,2);
		end = lenFl+3;
		*(temp+end) = '*';
		strncpy(temp+end+1,str,lenFirst); 
		strcpy(str, temp);
	    }
	 }                /* end floor */
	 else if(level == 0)
	 {
	    if(digit = isdigit(*(star+5)) != 0) 
	    {
		lenStar = strlen(star);
		lenStr = strlen(str);
		lenFirst = lenStr - lenStar;
		lenFl = lenStr - lenFirst - 5;
		strncpy(temp,star+5,lenFl);
		*(temp+lenFl) = '*';
		strncpy(temp+lenFl+1,star+1,2);
		end = lenFl+3;
		*(temp+end) = '*';
		strncpy(temp+end+1,str,lenFirst); 
		strcpy(str, temp);
	    }
	 }
	}    /* end check for th, rd etc */



	

    star = strchr (str, (int)'*');    /*..  23nd*Fl*5 --> *Fl*5  ..*/

    lenStr = strlen(str);            
    lenStar = strlen(star);           /*.. Eg: Length of *Fl*5  ..*/
    lenFirst = lenStr - lenStar;

/*****
	If "Side" is contained in input string handle separately
*****/

  if(strncmp("nd", star-2, 2) != 0 &&
     strncmp("ND", star-2, 2) != 0 &&
     strncmp("rd", star-2, 2) != 0 &&  /* rd, th, st or nd */
     strncmp("RD", star-2, 2) != 0 &&  /* as in --> 22nd   */
     strncmp("th", star-2, 2) != 0 &&
     strncmp("TH", star-2, 2) != 0 &&
     strncmp("st", star-2, 2) != 0 &&
     strncmp("ST", star-2, 2) != 0) 
  {
	
    if(((digit = isdigit(*(star+1))) != 0) &&  
        (strncmp("Side",str, 4) == 0    ||
        strncmp("Bsmt",str, 4) == 0    ||
        strncmp("Inte",str, 4) == 0    ||
        strncmp("Rear",str, 4) == 0    ||
        strncmp("Botm",str, 4) == 0    ||
        strncmp("Midl",str, 4) == 0    ||
        strncmp("Lowr",str, 4) == 0    ||
        strncmp("Uppr",str, 4) == 0    ||
        strncmp("Frnt",str, 4) == 0    ||
        strncmp("Down",str, 4) == 0    ||
        strncmp("Left",str, 4) == 0    ||
        strncmp("Rght",str, 4) == 0    ||
        strncmp("Lbby",str, 4) == 0    ||
        strncmp("Ofc",str, 3) == 0    ||
        strncmp("Top",str, 3) == 0    ||
        strncmp("PH",str, 2) == 0    ||
        strncmp("Up",str, 2) == 0 ))

    {
	strcpy(outstr,star+1);
	strncpy(outstr+(lenStar-1),str,lenFirst); 
/*	strcpy(outstr,str); */
/*	*star = '-'; */
	
/*	strcpy(outstr+lenFirst,star+1); */
	*(outstr+lenStr-1) = '\0';
	return;	
    }
    else if(((digit = isdigit(*str)) != 0) &&  
	    (strncmp("Side",star+1,4) == 0 || 
	    strncmp("Bsmt",star+1,4) == 0 || 
	    strncmp("Inte",star+1,4) == 0 || 
	    strncmp("Rear",star+1,4) == 0 || 
	    strncmp("Botm",star+1,4) == 0 || 
	    strncmp("Midl",star+1,4) == 0 || 
	    strncmp("Lowr",star+1,4) == 0 || 
	    strncmp("Uppr",star+1,4) == 0 || 
	    strncmp("Frnt",star+1,4) == 0 || 
	    strncmp("Down",star+1,4) == 0 || 
	    strncmp("Left",star+1,4) == 0 || 
	    strncmp("Rght",star+1,4) == 0 || 
	    strncmp("Lbby",star+1,4) == 0 || 
	    strncmp("Ofc",star+1,3) == 0 || 
	    strncmp("Top",star+1,3) == 0 || 
	    strncmp("PH",star+1,2) == 0 || 
	    strncmp("Up",star+1,2) == 0 )) 

    {
	strcpy(outstr,str);
/*	*star = '-'; */
	strcpy(outstr+lenFirst,star+1);
	*(outstr+lenStr-1) = '\0';
	return;	
    }
  }                              /* End check for st, nd etc. */

/******
	Search for second asterisk
*******/
    
    star2 = strchr (star+1, (int)'*'); 

    if(star2 != NULL) 
    {
       floor = strncmp("Fl",star2+1, 2);
       level = strncmp("Lvl", star2+1, 2);

       if(floor == 0 || level == 0) 
       {
 	   *(star2+3) = '*';
	   lenBefstar = lenStr - lenStar;
	   strncpy(star2+4, str, lenBefstar);
	   strncpy(str, star+1,lenStr);
	   *(str+lenStr) = '\0';

           star = strchr (str, (int)'*');     /*** REinitialize ***/
           lenStr = strlen(str);            
           lenStar = strlen(star);            
           star2 = strchr (star+1, (int)'*');
       } 
    }

/*****	
    Look for the following types : LOWR*BOTM  ---->  BOTM-LOWR 
				   UP*BSMT    ---->  BSMT-UP 
				   UP*TOP     ---->  TOP-UP 

    Skip any which contain "RD" etc. or digits. 
*** ***/

    if( lenStr >=5 && star2 == NULL && lenStar >=3 &&
	(strncmp("nd", star-2, 2) != 0 &&
	 strncmp("ND", star-2, 2) != 0 &&
	 strncmp("rd", star-2, 2) != 0 &&  /* rd, th, st or nd */
	 strncmp("RD", star-2, 2) != 0 &&  /* as in --> 22nd   */
	 strncmp("th", star-2, 2) != 0 &&
	 strncmp("TH", star-2, 2) != 0 &&
	 strncmp("st", star-2, 2) != 0 &&
	 strncmp("ST", star-2, 2) != 0) && 
	((digit = isdigit(*(star+1))) == 0) &&  
	((digit = isdigit(*str)) == 0)) {

	lenBefstar = lenStr-lenStar;

/*******
      Put in alphabetic order by flipping the before portion of the
      asterisk and the portion after the asterisk.
*******/

	if( *str > *(star+1))             
 
	{
		strncpy(outstr, star+1, lenStr);   
		strncpy(outstr+lenStar-1, "-", 1);
		strncpy(outstr+lenStar, str, lenBefstar);
		*(outstr+lenStr) = '\0';
		return;
	}
	else                       /* No need to flip just put a '-' in  */
	{
		*star = '-';
		strcpy(outstr, str);
		*(outstr+lenStr) = '\0';
		return;
	}
    }
		

/*****
    Check for Input: 2*G or 2*4   Output: 2G or 2-4
*****/

    befStar = lenStr - lenStar;
    aftStar = lenStr - befStar - 1;
    star2 = strchr (star+1, (int)'*');   
    if(star2 != NULL) aftStar = aftStar - strlen(star2);

    for( i = 0; i < befStar; i++)
    {
	if((digit = isdigit(*(str+i))) == 0) 
	   chr1Cnt++;                      /* Count number of characters */
	else
	   dig1Cnt++;                      /* Count number of digits */
    }

    for( i = 1; i <= aftStar; i++)
    {
	if((digit = isdigit(*(star+i))) == 0)
	   chr2Cnt++;                     /* Count of characters after '*' */
	else
	   dig2Cnt++;
    }

	if(chr1Cnt == befStar && (lenStr < 3 || *(star+1) == '&'))
	{
	   if(chr2Cnt == aftStar && *(star+1) != '&' ) 
	   {
		*star = '-';             /* Both characters, put a dash */
	        strcpy(outstr, str);
	   }
	   else if(dig2Cnt == aftStar || *(star+1) == '&')
	   {
		strncpy(outstr,str,befStar);    /* Both different,put togther*/
		strcpy(outstr+befStar, star+1);
	   }
	}
	else if(dig1Cnt == befStar && (lenStr < 3 || *(star+1) == '&'))
	{
	   if(dig2Cnt == aftStar && *(star+1) != '&') 
	   {
		*star = '-';
	        strcpy(outstr, str);
	   }
	   else if(chr2Cnt == aftStar || *(star+1) == '&')
	   {
		strncpy(outstr,str,befStar);
		strcpy(outstr+befStar, star+1);
	   }
	}

/*****
    Check for Input: 3G*1 --->   Output: 3G1    or
	      Input: 3G*b --->   Output: 3G-b
******/

	else
	{
	   if(strncmp("nd", star-2, 2) == 0 ||
	      strncmp("ND", star-2, 2) == 0 ||
	      strncmp("rd", star-2, 2) == 0 ||  /* rd, th, st or nd */
	      strncmp("RD", star-2, 2) == 0 ||  /* as in --> 22nd   */
	      strncmp("th", star-2, 2) == 0 ||
	      strncmp("TH", star-2, 2) == 0 ||
	      strncmp("st", star-2, 2) == 0 ||
	      strncmp("ST", star-2, 2) == 0) 
		goto skip;
	   else
	   {	
	
	      digit1 = isdigit(*(star+1));
	      digit2 = isdigit(*(star-1));
	      if((digit1 != 0 && digit2 == 0) ||
	         (digit1 == 0 && digit2 != 0)) 
	      {
		strncpy(outstr,str,befStar);
		strcpy(outstr+befStar, star+1);
	      }

	      else
	      {
		*star = '-';
	        strcpy(outstr, str);
              }	   
	      goto more;   
	   }
	   
	}



more:
	if( *(star+1) == '&') and = TRUE;
        star = strchr (star+1, (int)'*');   
	if(star != NULL) 
	{
   	   len = strlen(outstr) - strlen(star);
	   if((digit = isdigit(*(star+1))) == 0) 
	   {
		if(dig2Cnt == aftStar || and == TRUE) 
		{
			strcpy(star, star+1);
			strcpy(outstr+len,star);
		}	
		
	        else if(chr2Cnt == aftStar)
		{
			*star = '-';
			strcpy(outstr+len,star);
		}						
	   }
	   else 
	   {
		if(dig2Cnt == aftStar) 
		{
			*star = '-';	
			strcpy(outstr+len,star);
		}
			
	        else if(chr2Cnt == aftStar)
		{
		        strcpy(star, star+1);
			strcpy(outstr+len,star);
		}

	   }
	   goto more;
	 }
	 else
	   return;

skip:    

/*****
    Check for a digit in the first character 
*****/

    if((digit = isdigit(*str)) != 0) {

/******
    Check for Fl or Lvl contained in the string after the first star.
******/

       floor = strncmp("Fl",star+1, 2);
       level = strncmp("Lvl", star+1, 2);

       if(floor == 0 || level == 0) {

	    if(strncmp("nd", star-2, 2) == 0 ||
	       strncmp("ND", star-2, 2) == 0 ||
	       strncmp("rd", star-2, 2) == 0 ||  /* rd, th, st or nd */
	       strncmp("RD", star-2, 2) == 0 ||  /* as in --> 22nd   */
	       strncmp("th", star-2, 2) == 0 ||
	       strncmp("TH", star-2, 2) == 0 ||
	       strncmp("st", star-2, 2) == 0 ||
	       strncmp("ST", star-2, 2) == 0) {


/******
    Copy the digits before the st, nd, th, rd to the output string.
    Numlen is the number of digits before the ND, ST, TH, RD.
******/

		    numlen = lenStr - lenStar - 2;

		    for(i = numlen-1 ; i >= 0; i--) {
			*(outstr+i) = *(str+i);      
		    } 

					    
/*****
    Check for a second asterisk.  Eg: 22nd*Fl*G
******/
    		    star = strchr(star+1, (int) '*');
/*		    star3 = strchr(star+1, (int) '*'); */

/***** 
    Only one asterisk.
    Input: 2nd*Fl ---> Output: 2Fl
******/
		    if(star == NULL) {
			if(floor == 0) 
			{
			    strncpy(outstr+numlen, "Fl",2);
			    *(outstr+numlen+2) = '\0';
			}
			if(level == 0) 
			{
			    strncpy(outstr+numlen, "Lvl",3);
			    *(outstr+numlen+3) = '\0';
			}
			goto check;
		    }

		    Len2star = strlen(star+1); 

/*****
    Determine if first two characters after the asterisk are digits or letters.
******/

		    digit = isdigit(*(star+1));
		    if( *(star+2) != '*')
		       digit2 = isdigit(*(star+2));
		    else 
/******
    Shift over on character if a asterisk is in the star+2 position
    Example:   star = *2*3
******/

	 	    {
		       digit2 = isdigit(*(star+3));
		       if((digit2 != 0 && digit == 0) ||
			   (digit2 == 0 && digit != 0)) 
		       {
		            for ( i = 2; i <= Len2star; i++) 
				     *(star+i) = *(star+i+1);
		            *(star+Len2star) = '\0';
			    Len2star--;
		       }
		    }
		   

	

/***** 
    First or first and second characters are letters.  
    Input: 2nd*Fl*EG ---> Output: 2EG
******/

		    if(( digit == 0 && digit2 == 0 &&
                        Len2star < 3)  || (digit == 0 && Len2star == 1) ||
			(digit == 0 && star2 != NULL && Len2star < 3)) 

			strcpy(outstr+numlen,star+1);

/******
    First or first and second characters are digits.
    Input: 2nd*Fl*34 ---> Output: 2-34
*******/

		    else if(( digit != 0 && digit2 != 0 &&
			     Len2star < 3) || (digit != 0 && Len2star == 1) ||
			     (digit != 0 && star2 != NULL && Len2star < 3))  {

			*(outstr+numlen) = '-';           
			strcpy(outstr+numlen+1, star+1);
		    }

		    else	    {

/*****  
    First and second characters after asterisk are mixed.
    Input: 2nd*Fl*2E   ----> Output: 2Fl-2E   or
    Input: 2nd*Fl*Rear ----> Output: 2Fl-Rear
*****/

		      if(floor == 0) 
		      {
			strncpy(outstr+numlen, "Fl-", 3);
		        strcpy(outstr+numlen+3, star+1);
		      }
		      if(level == 0)
		      {
			strncpy(outstr+numlen, "Lvl-", 4);
		        strcpy(outstr+numlen+4, star+1);
		      }
		
		    }
	      }
	      goto check;
	  }                          /* End Fl or Lvl */
	  else              {

/******	
    First character is a digit, but no Fl or Lvl in the field.
    Input: 2nd*Rear ---> Output: 2Fl-Rear
*******/

	   if(lenStar > 3){
	    i = 0;
	    while( digit != 0) {
		*(outstr+i) = *(str+i);
		i++;
		digit = isdigit(*(str+i));
	    }

	      strncpy(outstr+i, "Fl-",3);
	      strcpy(outstr+i+3, star+1);

	    goto check; 
	    
	   }
	  }			    /* End no Fl or Lvl */

      }                             /* End 1st char digit */		
		




/******
    Search for more asterisks and append onto the string
******/

check:
	    
/*****
    Before searching for another asterisk, check that previous star 
    did not contain an ampersand
*****/

	    star = strchr(outstr, (int) '*');
	    if(star != NULL) 
	    {
		len = strlen(outstr) - strlen(star);
		*(star) = '-';
		len++;
				/*.. Increment len to include '-' ..*/
		i = 0;
		while ((*(star+1+i) != '*') && (*(star+1+i) != '\0'))
		{ 
		    *(outstr+len+i) = *(star+1+i);
		    i++;
		}
	    


	    star = strchr(star+1, (int) '*');
	    while (star != NULL) 
	    {
		len = strlen(outstr) - strlen(star);
		prev_digit = isdigit(*(outstr+len-1));
		digit     = isdigit(*(star+1));

/******
    Two consecutive letters or two consecutive digits.
******/

		if(prev_digit == 0 && digit == 0 ||
		    prev_digit != 0 && digit != 0) 
		{
			*(outstr+len) = '-';
			i = 1;
			while (*(star+i) != '*' && *(star+i) != '\0')
			{ 
			    *(outstr+len+i) = *(star+i);
			    *(outstr+len+1+i) = '\0';
			    i++;
			}
		}

		else
		{	
			i = 0;
			while ((*(star+1+i) != '*') && (*(star+1+i) != '\0'))
			{ 
			    *(outstr+len+i) = *(star+1+i);
			    *(outstr+len+1+i) = '\0';
			    i++;
			}
		}
		star = strchr(star+1, (int) '*');
					
	
	    }           /*.. End while ..*/
	}


}

