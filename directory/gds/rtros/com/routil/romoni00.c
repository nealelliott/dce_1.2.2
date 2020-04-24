/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: romoni00.c,v $
 * Revision 1.1.4.2  1996/03/11  13:20:03  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:32:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:17  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:07:50  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:52  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:41  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:54:31  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:24  keutel
 * 	creation
 * 	[1994/03/21  13:24:15  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: romoni00.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:03 $" } ;
#endif

/***************************************************************************\
*                                                                           *
*  * Copyright (c) 1991 by                                                  *
*  * SIEMENS NIXDORF INFORMATIONSSYSTEME AG                                 *
*  * All rights reserved                                                    *
*  *                                                                        *
*  * This software is furnished under licence and may be used only in       *
*  * accordance with the terms of that licence and with the inclusion of    *
*  * the above copyright notice. This software may not be provided or       *
*  * otherwise made available to, or used by, any other person. No title    *
*  * to or ownership of the software is hereby transferred.                 *
*                                                                           *
\***************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME 	: romoni00.c		           		      */
/*                                                                    */
/* AUTHOR       : Milind m. Aphale                                    */
/* DATE 	: 93-05-14        				      */
/*                                                                    */
/* COMPONENT	: RTROS PDU MONITOR               		      */
/*                                                                    */
/* PRD#/VERS.	: RTROS-V03.00					      */
/*                                                                    */
/* DESCRIPTION	: Testing facility to change PDUs.                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  | KZ | CR# FM# */
/* 03.00 K0 | 93-05-14|  original		       | ma |	      */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) romoni00.c 1.1 93/11/29   RTROS-V3.00";

/*********************************************************************/
/*                                                                   */
/*              I N C L U D E S                                      */
/*                                                                   */
/*********************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>       /* Include standard I/O  */
#include <stdlib.h>
#include <string.h>
#include <rogenrl.h>
#include <ros.h>
#include <routil0.h>
#include <rostor0.h>
#include <rodpat0.h>
#include <rospm00.h>
#include <romoni01.h>
#include <romoni02.h>
 
unsigned short int  PASCAL ro00m1_moniON PROTO (( void ));
void PASCAL ro00m0_moniFct PROTO ((
unsigned long  int       hEcb,          /* memory handle to ECB     */
					/* (for the a logging)      */
unsigned long  int       hAcb,          /* memory handle to ACB     */
				        /* (for the a logging)      */ 
signed   short int       sPmNo,         /* Protocol m/c no.         */ 
unsigned long  int       ulEvent,       /* Event                    */
unsigned long  int       *pP1,          /* ptr to event param P1    */          
unsigned long  int       *pP2,          /* ptr to event param P2    */          
unsigned long  int       *pP3,          /* ptr to event param P3    */          
unsigned long  int       *pP4));          /* ptr to event param P4    */          

int  PASCAL  ro00mc_readNumber PROTO ((
	   FILE *stream,
	   char *str,
	   int   defNum));

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro00ma_readString                                   */
/*                                                                    */
/* AUTHOR       : Mueller Johann  / Milind M. Aphale                  */
/* DATE 	: 93-05-14        				      */
/*                                                                    */
/* SYNTAX       :                                                     */

static     char * ro00ma_readString (
	FILE *stream,
	char *string,
	char *defstring)

/*                                                                    */
/* DESCRIPTION  : This function writes prompt string, reads input     */
/*                string and returns input string, or default string  */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*    stream    :  input stream                                       */
/*    string    :  prompt string                                      */
/*    defstring :  default string                                     */
/*                                                                    */
/* OUT-PARAMETERS             :  none                                 */
/*                                                                    */
/* RETURN VALUES              :  returns input or default string      */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ************************************************************* */
{/*entr*/
extern  int   errno;
static  char  bufarea[128];

int     inx;

/* ****************** Write promt string **************************** */
/* ******************                    **************************** */

fprintf (stdout                      ,
	 "%s[%s]: "                  ,
	 (string   ) ? string    : "",
	 (defstring) ? defstring : "");

/* ****************** Read input string ***************************** */
/* ******************                   ***************************** */
    
while (fgets (bufarea, 128, stream) == NULL && errno == EINTR);

/* ****************** Change newline to NULL character ************** */
/* ******************                                  ************** */

inx = 0;

do {
   switch (bufarea[inx])
	{
   case '\012': bufarea[inx] = '\0'; break;
   case '\0'  : inx          = 128;  break;
   default    : inx++;               break;
	}
   }
   while (inx < 128);

/* ****************** Return input string *************************** */
/* ******************                     *************************** */

return ((bufarea[0]) ? bufarea : defstring);
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro00mb_yesOrNo                                      */
/*                                                                    */
/* AUTHOR       : Mueller Johann / Milind M. Aphale                   */
/* DATE 	: 93-05-14        				      */
/*                                                                    */
/* SYNTAX       :                                                     */

 int  PASCAL ro00mb_yesOrNo (
	   FILE *stream,
	   char *string,
	   int   defvalue)

/*                                                                    */
/* DESCRIPTION  :  This function writes default value and reads answer*/
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*    stream    :  input stream                                       */
/*    string    :  prompt string                                      */
/*    defvalue  :  default value                                      */
/*                                                                    */
/* OUT-PARAMETERS             :  none                                 */
/*                                                                    */
/* RETURN VALUES              :  1 if received y  (yes)               */
/*                               0 if received n  (no)                */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ************************************************************* */
{/*entr*/
char noOrYes;

  for (;;) {
      noOrYes = *ro00ma_readString (stream, string, (defvalue) ? "y" : "n");

      switch (noOrYes) {
      case 'j':
      case 'y': return (1);
      case 'n': return (0);
      default : return(0); /* break; */
      }
  }
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro00mc_readNumber                                   */
/*                                                                    */
/* AUTHOR       : Mueller Johann / milind M. Aphale                   */
/* DATE 	: 93-05-14        				      */
/*                                                                    */
/* SYNTAX       :                                                     */

int  PASCAL  ro00mc_readNumber (
	   FILE *stream,
	   char *str,
	   int   defNum)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*    stream    :  input stream                                       */
/*    str       :  prompt string                                      */
/*    defNum    :  numeric default value                              */
/*                                                                    */
/* OUT-PARAMETERS             :  none                                 */
/*                                                                    */
/* RETURN VALUES              :  integer value of input or default    */
/*                               string                               */
/*                                                                    */
/* GLOBAL DATA   (read only)  :  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ************************************************************* */
{/*entr*/
int  nb;
char numPrint[32];

  sprintf (numPrint, "%d",  defNum);
  printf  ("\n");

  do {
     nb = atoi (ro00ma_readString (stream, str, numPrint));
     printf (r_s09y, nb);
     }
     while (! ro00mb_yesOrNo (stream, r_s09z, 1));

  printf ("\n");
  return (nb);
}/*end*/
    

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro00m1_moniON                                       */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale.                                   */
/* DATE 	: 93-05-14        				      */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned short int  PASCAL ro00m1_moniON ( void )

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS  : None.                                             */
/*                                                                    */
/*                                                                    */
/* OUT-PARAMETERS  :  none                                            */
/*                                                                    */
/* RETURN VALUES   : If environment variable  ROSPDU and              */
/*                   compiler option RC0091_PDUMONITOR are set        */
/*                   returns  1  else returns 0.                      */ 
/*                                                                    */
/*                                                                    */
/* GLOBAL DATA   (read only)  :  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ************************************************************* */
{

#ifdef  RC0091_PDUMONITOR

/* ****************** Check environment whether monitor should ****** */
/* ****************** be called or not.                        ****** */
unsigned short chk;

if  (! r_v090_envFlag)
     {
     buffer          = getenv (r_v091_envString);
     /* r_v090_envFlag  = ((buffer) && (! strcmp (buffer, r_v092_envSwitch)))
		     ? PDUMONITOR_ON : PDUMONITOR_OFF ; */
     chk = ((buffer) && (! strcmp (buffer, r_v092_envSwitch)));
     if(chk)
       return(1);
     return(0); 
     }

     /* return (r_v090_envFlag); */

#else

return(0);                        


#endif

}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro00m2_SpmPpmMoniFct                                   */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale.                                   */
/* DATE 	: 93-05-14        				      */
/*                                                                    */
/* SYNTAX       :                                                     */

unsigned short int  PASCAL ro00m2_SpmPpmMoniFct(
	unsigned long  int ulEvent, 
	unsigned long int *hStore)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS  : 					              */ 
/*                                                                    */
/*   ulEvent : event.                                                 */
/*   hStore  : handle to store.                                       */
/*                                                                    */
/* OUT-PARAMETERS  :  none                                            */
/*                                                                    */
/* RETURN VALUES   : 					              */	
/*                                                                    */
/* GLOBAL DATA   (read only)  :  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ************************************************************* */
{/*entr-ro00m2_SpmPpmMoniFct*/

#define F_LEN 50
#define CMD_LEN   (3 * F_LEN)

unsigned long int buflen;
static unsigned short int errorstate;
FILE *fp;
char filename[F_LEN],storefilename[F_LEN];
char executecommand[CMD_LEN];
char *cmd = "mv ";   

  buflen =( 1024 * 20 );
  
     /* Print the event no : */
  printf(" Event no = 0x%x\n", ulEvent);

  /* r_s09b = Saving ppdu part */
  errorstate = ro00mb_yesOrNo ( stdin, r_s09b, 0 );    
                                                                          
  /******************** Write received session user data **************/  
                                                                         
  while(errorstate)                                                       
  {                                                                       
    strcpy(filename,                                                      
           ro00ma_readString ( stdin , r_s09h , DefaultFile));            

    /* Before copying store to file, it is confirmed that
       file is opening for writing */
 
    if  ((fp = fopen (filename,"w")) == (FILE *) NULL) {
         printf (r_s09k);
         errorstate = 0;
    }
    fclose(fp);                                                           
    /* printf("filename = %s\n",filename); */

    if (errorstate) {
        if( ro0087_StoreToFile ( *(hStore), storefilename ) != NULL)
         {
           printf("Error in ro0087_StoreToFile \n");
           return ( -R_MONI_STORE_ERR );
         }
         /* printf("storefilename = %s\n",storefilename); */
         strcpy ( executecommand , cmd );
         strcat ( executecommand , storefilename );
         strcat (executecommand , " ");
         strcat (executecommand , filename);
         /*  printf("execute command = %s \n", executecommand ); */                                                                 
         system ( executecommand ); 
         errorstate = 0;
     }
     else                                                                   
         errorstate = ro00mb_yesOrNo (stdin , r_s09b , 0);
   }/*end-while*/
                                                                            
   /**************    Replace session user data   ****************/         
   ro0043_memset ((char FAR *) filename , (int)0, (unsigned long  int)F_LEN); 
   ro0043_memset ((char FAR *) storefilename , (int)0, 
                 (unsigned long  int)F_LEN); 
   ro0043_memset ((char FAR *) executecommand , (int)0, 
                 (unsigned long  int) CMD_LEN); 

/*
   ro082B_InitStruc( storefilename, F_LEN);
   ro082B_InitStruc( executecommand, CMD_LEN );
 by kc  on 23/6/93 */

   errorstate = ro00mb_yesOrNo (stdin, r_s09c , 0);      
   /* r_s09c = Replacing ppdu part */                                                                         
   while (errorstate)                                                       
   {                                                                        
     strcpy ( filename ,                                                    
            ro00ma_readString (stdin,r_s09h,DefaultSourceFile));
  
   /* Befort copying file to store , it is checked that file is 
      opening for reading */
     if  ((fp = fopen (filename,"r")) == (FILE *) NULL) {
         printf (r_s09k);
         errorstate = 0;
     }
     fclose(fp);

     if (errorstate) {
         *(hStore) = ro0082_FileToStore(filename,buflen);
         if( *(hStore) == NULL )
            { printf("Error in ro0082_FileToStore fn... \n");
              return ( -R_MONI_STORE_ERR);
            }
         errorstate = 0;
      }
      else                                                                  
          errorstate = ro00mb_yesOrNo (stdin, r_s09c, 0);
   }/*end-while*/

return (R_MONI_SUCS);                                                     

}/*end-ro00m2_SpmPpmMoniFct*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro00m0_moniFct                                      */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale.                                   */
/* DATE 	: 93-05-14        				      */
/*                                                                    */
/* SYNTAX       :                                                     */

void PASCAL ro00m0_moniFct (
unsigned long  int       hEcb,          /* memory handle to ECB     */
					/* (for the a logging)      */
unsigned long  int       hAcb,          /* memory handle to ACB     */
				        /* (for the a logging)      */ 
signed   short int       sPmNo,         /* Protocol m/c no.         */ 
unsigned long  int       ulEvent,       /* Event                    */
unsigned long  int       *pP1,          /* ptr to event param P1    */          
unsigned long  int       *pP2,          /* ptr to event param P2    */          
unsigned long  int       *pP3,          /* ptr to event param P3    */          
unsigned long  int       *pP4)          /* ptr to event param P4    */          

/* unsigned short int       mode;           mode = 0: sending PPDU   */
/*                                               = 1: receiving PPDU */  
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS  : 					              */ 
/*                                                                    */
/* OUT-PARAMETERS  :  none                                            */
/*                                                                    */
/* RETURN VALUES   :                                                  */
/*                                                                    */
/* GLOBAL DATA   (read only)  :  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ************************************************************* */
{

#ifdef  RC0091_PDUMONITOR
  switch ( sPmNo ) {

  case RC0214_Session      :
  case RC0215_Presentation :
       if ( ((ulEvent & 0x0f00) == RC0800_SpmEvent) && (*pP1) ) {
           /*-----------------------------------------------------------------*/
           /* Call the Moni function only if :                                */
           /* - Event passing through SPM-PPM boundary, and                   */
           /* - A store exists in P1 (user data of SPM).                      */
           /*-----------------------------------------------------------------*/
           printf("Automata Table No : 0x%x\n",sPmNo);
           if (ro00m2_SpmPpmMoniFct(ulEvent,pP1) != R_MONI_SUCS)
               printf("error in PDU MONITOR fn. in SPM \n");
       }
       break;

   default                  :
       break; 
  }
#endif 

}

/*EOF*/
