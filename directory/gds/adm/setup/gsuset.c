/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: gsuset.c,v $
 * Revision 1.1.4.2  1996/02/18  19:42:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:56  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:21:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:51  root]
 * 
 * Revision 1.1.2.6  1994/09/19  08:39:15  marrek
 * 	Fixes for OT12176 (wrong incore tables.)
 * 	[1994/09/19  08:31:38  marrek]
 * 
 * Revision 1.1.2.5  1994/08/19  13:54:08  marrek
 * 	Improve testcode coverage OT11586.
 * 	[1994/08/19  11:45:50  marrek]
 * 
 * Revision 1.1.2.4  1994/08/11  13:05:33  marrek
 * 	Fix errors for interactive mode (ot11585).
 * 	[1994/08/10  13:08:13  marrek]
 * 
 * Revision 1.1.2.3  1994/07/19  14:48:51  marrek
 * 	Configuration messages for interactive mode.
 * 	[1994/07/19  11:58:52  marrek]
 * 
 * Revision 1.1.2.2  1994/07/07  15:41:49  marrek
 * 	July 1994 code drop.
 * 	[1994/07/07  15:40:14  marrek]
 * 
 * Revision 1.1.2.1  1994/06/21  14:39:27  marrek
 * 	Bug fix for June 94 code submission.
 * 	[1994/06/15  10:31:15  marrek]
 * 
 * 	Created in June 94 code submission.
 * 	[1994/06/10  16:03:48  marrek]
 * 
 * $EndLog$
 */

/**********************************************************************/
/*                                                                    */
/*  Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG         */
/*  All Rights Reserved                                               */
/*                                                                    */
/*  Project          : GDU - GDS Attribute Info Module                */
/*  File Name        : gdsset.c                                       */
/*  Description      : This file aids in tthe configuring and ini-    */
/*                     tialising of directory systems                 */
/*  Date             : Oktober 1993                                   */
/*  Author           : Eva Wilde, SNI BU BA KP13                      */
/*                                                                    */
/**********************************************************************/
 
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gsuset.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 19:42:12 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <nl_types.h>
#include <langinfo.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <gsu.h>
#include <d2dir.h>

#include <gds.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <locale.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>

int gsu_dirid;                   /* directory indicator */
int gsu_nocl;                    /* number of Client-processes */
int gsu_noser;                   /* number of act. Servers */
char gsu_config[GSU_CONFIG]="\0";  /* configuration: client/server */
char gsu_cladd[GSU_DSA]="\0";      /* PSAP-addresse of clientt */
char gsu_locdsa[GSU_DSA]="\0";     /* local DSA name */
char gsu_psaploc[GSU_PSAP]="\0";
char gsu_loc_def[GSU_LOC_DEF]="\0";/* local = default DSA   */
char gsu_contdsa[GSU_DSA]="\0";    /* contact DSA */
char gsu_psapcont[GSU_PSAP]="\0";          
char gsu_locals[GSU_LOCALS]="\0";  /* local DSA is initial DSA */
char gsu_defsch[GSU_DEFSCH]="\0";  /* default schema indicator */
char gsu_userpsw[GSU_USERPSW]="\0";/* user password for local DSA */
char gsu_e_secur[GSU_SECUR]="\0";  /* for Authentication mechanism   */
char gsu_princ[GSU_PRINC]="\0";    /* for Principal name   */
gsu_dsa *anfang1=NULL;             /* for additional remote DSA liste  */
gsu_dsa *anfang2=NULL;             /* for default DSA liste  */
unsigned16 initmode = 0;
unsigned16 oflg = 0;               /* for overwrite flag   */
unsigned16 listflg = 0;            /* for listing flag  */
unsigned16 gsu_authent_mech;       /* for Authentication mechanism   */
char *gsu_listfile = NULL;
FILE * stream=NULL;                /* for gsu_readline */
FILE * streamlist=NULL;            /* for listing   */
char * interact_file=NULL;         /* for parameter file  */
char gsu_line[GSU_LINE]="\0";
char gsu_answer_true[GSU_MAX_LANG]="\0";
char gsu_answer_false[GSU_MAX_LANG]="\0";

const char * gsu_client_sys = "C";
const char * gsu_server_sys = "S";
const char * gsu_secur_dce = "DCE";
const char * gsu_secur_simple = "simple";
const char * gsu_secur_simple_dce = "simple,DCE";
const char * gsu_secur_anonymous = "anonym";
const char * gsu_cladd_default_value = "TS=Server,NA='TCP/IP!internet=127.0.0.1+port=31010'";

/*** functions for interactive mode  ***/
static void gsu_interactive(void);
static void gsu_inter_server_sys(void);
static void gsu_inter_client_sys(void);
static void gsu_inter_readline_admin(char *);
static unsigned8 gsu_inter_read(char *, signed16);
static void gsu_inter_write(char *, char *, unsigned16, char);
static gsu_dsa * gsu_repeat(char *, FILE *, signed16);
static unsigned16 gsu_readline(char *, int *, char, char *);
static unsigned16 gsu_readline_dsas(char *, char *, char *);
static void gsu_readline_repeat(gsu_dsa *, char *);

/*** functions for file input mode  ***/
static void gsu_nointeract(char *);
static unsigned16 gsu_noint_read (char *string,char *var1,int *var2,char state,signed16 maxim,char *);
static unsigned16 gsu_noint_dsas(char *, char *, char *, char *);
static gsu_dsa * gsu_noint_repeat(char *,signed16, char *);

/*** functions for both  ***/
extern char *readline(char *);
extern void add_history(char *);
static void gsu_help(void);
static unsigned16 gsu_set_file (char *);
static unsigned16 gsu_syncheck(void);
static void gsu_dirid_exist(void);
static void gsu_dirid_delete(void);
static void gsu_deactivation(void);
static void gsu_setdir(void );
extern void gsu_step1(void);                                                    
extern void gsu_step2(void);                                                    
extern void gsu_step3(void);                                                    
extern void gsu_step4(void);                                                    
extern void gsu_step5(void);                                                    
extern void gsu_step6(void);                                                    
extern void gsu_step7(void);                                                    
extern void gsu_step8(void);                                                    
extern void gsu_step9(void);
extern void gsu_listing(char *, char *, char *);

int main (int argc, char *argv[])
{
signed16 i;
unsigned32 status;
char * main_file = NULL;
char * main_hilf = NULL;
char * nl_langinfo (nl_item item);
char main_int_string[GSU_LENINT]="\0";

/***   for SAMS - katalog    ***/
setlocale(LC_ALL, "");

dce_msg_define_msg_table(gds_msg_table, (unsigned32)(sizeof(gds_msg_table)/sizeof(gds_msg_table[0])),&status);
/*******************************/

/*** affiramitive response for yes/no queries   ***/
strncpy( gsu_answer_true, nl_langinfo(YESSTR) ,GSU_MAX_LANG);
strncpy( gsu_answer_false, nl_langinfo(NOSTR) ,GSU_MAX_LANG);
/*************************************************/

for (i=1; i < argc; i++)
  {
  if (argv[i][0] == '-') 
     {
     switch(argv[i][1])
        {
        case 'h': 
	         gsu_help();
                 break;
        case 'l':
		 if((gsu_listfile=(char *)malloc(strlen(argv[i])+1)) == NULL)
		     {
		     fprintf(stderr, "ERROR\n");
		     dce_fprintf(stderr,gc_t_memory_insufficient);
	             exit(GSU_EXIT_1);
		     }
	         strcpy(gsu_listfile, argv[i]+2);
	         listflg = 1;
                 if ((main_hilf=(char *)malloc(strlen("touch ")+strlen(gsu_listfile)+1))==NULL)
                    {
		    fprintf(stderr, "ERROR\n");
                    dce_fprintf(stderr,gc_t_memory_insufficient);
                    exit(GSU_EXIT_2);
                    }
                 strcpy(main_hilf, "touch ");
                 strcat(main_hilf, gsu_listfile);
                 if (system(main_hilf) < 0)
                    {
		    fprintf(stderr, "ERROR\n");
                    fprintf(stderr, "%s\n", main_hilf);
                    exit(GSU_EXIT_3);
                    }
                 if ((streamlist=fopen(gsu_listfile, "w")) == NULL)
                    {
		    fprintf(stderr, "ERROR\n");
                    dce_fprintf(stderr, gsu_fopen_error,gsu_listfile);
                    exit(GSU_EXIT_4);
                    }
                 gsu_listing("fopen(", gsu_listfile, ")");
                 break;
        case 'o': 
		 gsu_listing("overwrite flag was set",NULL,NULL);
	         oflg = 1;
                 break;
        default:  
	        main_hilf=(char *)dce_sprintf(gsu_unknown_switch,argv[i]);
	        gsu_listing(main_hilf,NULL,NULL);
		fprintf(stderr, "ERROR\n");
	        dce_fprintf(stderr,gsu_unknown_switch,argv[i]);
	        exit(GSU_EXIT_5);
        }
     }
 else
     {
     if (main_file == NULL)
	{
        main_file = argv[i];
	gsu_listing("main: inputfile = ", main_file, NULL);
	}
     else
	{
	main_hilf=(char *)dce_sprintf(gsu_input_file_poss);
	gsu_listing(main_hilf, NULL, NULL);
	fprintf(stderr, "ERROR\n");
	dce_fprintf(stderr, gsu_input_file_poss);
	exit(GSU_EXIT_6);
	}
     }
  }
if (main_file != NULL)
   {
   gsu_listing("gsu_nointeract()", NULL, NULL);
   gsu_nointeract(main_file);
   gsu_listing("gsu_syncheck()", NULL, NULL);
   printf("CONFIGURATION of GDS ...\n");
   if (gsu_syncheck() == 0)
       {
       gsu_listing("gsu_setdir()", NULL, NULL);
       gsu_setdir();
       }
   }
else
   {
   gsu_listing("gsu_interactive()", NULL, NULL);
   gsu_interactive();
   }

/*** change of gsu_secur in integer   ***/
if (!strcmp(gsu_e_secur, gsu_secur_dce))
   {
   gsu_authent_mech = D2_DCE_AUTH;
   }
else
   {
   if (!strcmp(gsu_e_secur, gsu_secur_simple))
      {
      gsu_authent_mech = D2_SIMPLE;
      }
   else
      {
      if (!strcmp(gsu_e_secur, gsu_secur_simple_dce))
	 {
	 gsu_authent_mech = D2_SIMPLE_DCE;
	 }
      else
	 {
         if (!strcmp(gsu_e_secur, gsu_secur_anonymous))
	    {
	    gsu_authent_mech = D2_ANONYMOUS;
	    }
	 else
	    {
	    if (!strcmp(gsu_config, gsu_server_sys))
	       {
	       fprintf(stderr, "ERROR\n");
	       dce_fprintf(stderr, gsu_secur_false);
	       main_hilf=(char *)dce_sprintf(gsu_secur_false);
	       gsu_listing(main_hilf, NULL, NULL);
	       exit(GSU_EXIT_7);
	       }
	    }
	 }
      }
   }
/*** end of change of gsu_secur in integer   ***/

printf("INITIALISATION and ADMINISTRATION of GDS ...\n");
switch(initmode){
		case 1: gsu_listing("gsu_step1()", NULL, NULL);
			gsu_step1();
			gsu_listing("gsu_step5()", NULL, NULL);
			gsu_step5();
			break;
	        case 2: gsu_listing("gsu_step1()", NULL, NULL);
			gsu_step1();
			gsu_listing("gsu_step2()", NULL, NULL);
			gsu_step2();
			gsu_listing("gsu_step6()", NULL, NULL);
			gsu_step6();
			gsu_listing("gsu_step5()", NULL, NULL);
			gsu_step5();
			break;
		case 3: gsu_listing("gsu_step1()", NULL, NULL);
			gsu_step1();
			gsu_listing("gsu_step2()", NULL, NULL);
			gsu_step2();
			gsu_listing("gsu_step6()", NULL, NULL);
			gsu_step6();
			gsu_listing("gsu_step5()", NULL, NULL);
			gsu_step5();
			gsu_listing("gsu_step7()", NULL, NULL);
			gsu_step7();
			gsu_listing("gsu_step9()", NULL, NULL);
			gsu_step9();
			gsu_listing("gsu_step8()", NULL, NULL);
			gsu_step8();
			break;
		case 4: gsu_listing("gsu_step1()", NULL, NULL);
			gsu_step1();
			gsu_listing("gsu_step2()", NULL, NULL);
			gsu_step2();
			gsu_listing("gsu_step6()", NULL, NULL);
			gsu_step6();
			gsu_listing("gsu_step3()", NULL, NULL);
			gsu_step3();
			gsu_listing("gsu_step5()", NULL, NULL);
			gsu_step5();
			gsu_listing("gsu_step7()", NULL, NULL);
			gsu_step7();
			gsu_listing("gsu_step9()", NULL, NULL);
			gsu_step9();
			gsu_listing("gsu_step8()", NULL, NULL);
			gsu_step8();
			break;
		case 5: gsu_listing("gsu_step1()", NULL, NULL);
			gsu_step1();
			gsu_listing("gsu_step2()", NULL, NULL);
			gsu_step2();
			gsu_listing("gsu_step6()", NULL, NULL);
			gsu_step6();
			gsu_listing("gsu_step4()", NULL, NULL);
			gsu_step4();
			gsu_listing("gsu_step3()", NULL, NULL);
			gsu_step3();
			gsu_listing("gsu_step5()", NULL, NULL);
			gsu_step5();
			gsu_listing("gsu_step7()", NULL, NULL);
			gsu_step7();
			gsu_listing("gsu_step9()", NULL, NULL);
			gsu_step9();
			gsu_listing("gsu_step8()", NULL, NULL);
			gsu_step8();
			break;
		default:
			sprintf(main_int_string, "%d", initmode);
			fprintf(stderr, "ERROR\n");
			dce_fprintf(stderr,gsu_unknown_switch, main_int_string);
			main_hilf=(char *)dce_sprintf(gsu_unknown_switch,main_int_string);
			gsu_listing(main_hilf,NULL,NULL);
			exit(GSU_EXIT_8);
		}
printf("GDSSETUP was successfully completed.\n");
gsu_listing("GDSSETUP was successfully completed.",NULL,NULL);
exit(0);
}

/***************************************************************************/
/*                                                                         */
/* Function Name : gsu_interactive                                         */
/* Description   : This function is used by interactive operating modes    */ 
/* Author        : Eva Wilde, SNI BU BA KP13                               */
/*                                                                         */
/***************************************************************************/
static void gsu_interactive(void)
{
signed16 i;
unsigned16 flgdat;
char inter_int_string[GSU_LENINT]="\0";
char * interact_str=NULL;
char * interact_value=NULL;

/***   don't use buffer for stdin   ***/
setbuf(stdin, 0);

/***   find out of file to store of configuration and initialization data ***/
if ((interact_value=getenv("INTERACT_FILE")) != NULL)
   {
   interact_file = interact_value;
   flgdat = gsu_set_file(interact_file);
   }
else
   {
   if ((interact_file=(char *)malloc(strlen(dcelocal_path)+strlen(GSU_DIR)+strlen(GSU_FILE)+1)) == NULL)
       {
       fprintf(stderr, "ERROR\n");
       dce_fprintf(stderr,gc_t_memory_insufficient);
       interact_str=(char *)dce_sprintf(gc_t_memory_insufficient);
       gsu_listing(interact_str,NULL,NULL);
       exit(GSU_EXIT_63);
       }
   sprintf(interact_file, "%s%s%s",dcelocal_path,GSU_DIR,GSU_FILE);
   flgdat = gsu_set_file(interact_file);
   }
gsu_listing("gsu_set_file(",interact_file, ")");

if (flgdat == 0)
   {
   gsu_listing("fopen(", interact_file, ", w)");
   if ((stream=fopen(interact_file, "w")) == NULL)
      {
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr,gsu_fopen_error,interact_file);
      interact_str=(char *)dce_sprintf(gsu_fopen_error, interact_file);
      gsu_listing(interact_str,NULL,NULL);
      exit(GSU_EXIT_9);
      }

               /***************************************/
               /*****     CONTIFURATION DATA      *****/
               /***************************************/
    interact_str=(char *)dce_sprintf(gsu_dirid_answer_sams);
    printf("%s", interact_str);
    if (gsu_inter_read(inter_int_string, GSU_LENINT)==0)
       {
       /***   set default value   ***/
       gsu_dirid = 1;
       }
    else
       {
       gsu_dirid=atoi(inter_int_string);
       }
    gsu_inter_write(interact_str, NULL, gsu_dirid, 'i');
    gsu_listing("gsu_inter_write(", interact_str, ",...)");
    sprintf(inter_int_string, "%d", gsu_dirid);
    gsu_listing("	gsu_dirid = ", inter_int_string, NULL);
    free(interact_str);

    interact_str=(char *)dce_sprintf(gsu_config_answer_sams);
    printf("%s",interact_str);
    if (gsu_inter_read(gsu_config,GSU_CONFIG)==0)
       {
       /***   set default value   ***/
       strcpy(gsu_config, gsu_client_sys);
       }
    gsu_inter_write(interact_str, gsu_config, 0, 'c');
    gsu_listing("gsu_inter_write(", interact_str, ",...)");
    gsu_listing("	gsu_config = ", gsu_config, NULL);
    free(interact_str);

    for (i=0; i<GSU_CONFIG; i++)
       gsu_config[i] = (char)toupper(gsu_config[i]);
    if (!strcmp(gsu_config, gsu_client_sys))
      {
      gsu_listing("gsu_inter_client_sys()", NULL, NULL);
      gsu_inter_client_sys();
      gsu_listing("fclose(stream)",  NULL, NULL);
      if (fclose(stream) != 0)
	 {
	 interact_str=(char *)dce_sprintf(gsu_fclose_error, interact_file);
	 gsu_listing(interact_str,  NULL, NULL);
	 fprintf(stderr, "ERROR\n");
	 dce_fprintf(stderr, gsu_fclose_error, interact_file);
	 free(interact_str);
	 exit(GSU_EXIT_10);
	 }
      }
    else
      {         /* gsu_config != "c"  */
      for (i=0; i<GSU_CONFIG; i++)
         gsu_config[i] = (char)toupper(gsu_config[i]);
      if (!strcmp(gsu_config,gsu_server_sys))
         {       /* gsu_config == "s" */
	 /************************************/
	 /*****   Client/Server System   *****/
	 /************************************/
	 gsu_listing("gsu_inter_server_sys()", NULL,NULL);
         gsu_inter_server_sys();
         if (fclose(stream) != 0)
	    {
	    interact_str=(char *)dce_sprintf(gsu_fclose_error,interact_file);
	    gsu_listing(interact_str, NULL, NULL);
	    free(interact_str);
	    fprintf(stderr, "ERROR\n");
	    dce_fprintf(stderr, gsu_fclose_error, interact_file);
	    exit(GSU_EXIT_11);
	    }
         }
      else
         {
	 interact_str=(char *)dce_sprintf(gsu_config_type_failed, "gsu_interactive");
	 gsu_listing(interact_str,  NULL, NULL);
	 free(interact_str);
	 fprintf(stderr, "ERROR\n");
         dce_fprintf(stderr, gsu_config_type_failed, "gsu_interactive");
         exit(GSU_EXIT_12);
         }
      }
   }
else
   {
   gsu_listing("gsu_inter_readline_admin(", interact_file, ")");
   gsu_inter_readline_admin(interact_file);
   }
  return; 
}

/**************************************************************************/
/*                                                                        */
/*  Function name : gsu_inter_write                                       */
/*  Description   : Write input in file                                   */
/*  Input Value   : char * prompt string                                  */
/*                  char * for store of input value (state='c')           */
/*                  unsigned16 for store of input value (state='i')       */
/*                  char 'i' for integer or 'c' for char                  */
/*  Date          : November 1993                                         */
/*  Author        : Eva Wilde, SNI BU BA NM12                             */
/*                                                                        */
/**************************************************************************/
static void gsu_inter_write(
	    char *gsu_wr_string,  /* IN - input string  */
	    char *gsu_wr_var1,    /* OUT - output variable  */
	    unsigned16 gsu_wr_var2,/* OUT - output variable  */
	    char gsu_wr_state)    /* IN - int/char flag */
{
char * inter_write_poi=NULL;

  if (gsu_wr_state == 'i')
    {
    while ((fprintf(stream, "%s\"%d\"\n",gsu_wr_string,gsu_wr_var2) ) < 0)
	  {
	  fprintf(stderr, "ERROR\n");
	  dce_fprintf(stderr, gsu_inter_write_error,interact_file);
	  inter_write_poi=(char *)dce_sprintf(gsu_inter_write_error,interact_file);
	  gsu_listing(inter_write_poi,NULL,NULL);
	  exit(GSU_EXIT_13);
	  };
    }
  else
     {
     if (gsu_wr_state == 'c')
	{
        while ((fprintf(stream,"%s\"%s\"\n",gsu_wr_string,gsu_wr_var1))<0)
	      {
	      fprintf(stderr, "ERROR\n");
	      dce_fprintf(stderr, gsu_inter_write_error,interact_file);
	      inter_write_poi=(char *)dce_sprintf(gsu_inter_write_error,interact_file);
	      gsu_listing(inter_write_poi,NULL,NULL);
	      exit(GSU_EXIT_14);
	      };
	}
     }
}

/************************************************************************/
/*                                                                      */
/*  Function name : gsu_readline_repeat                                 */
/*  Description   : Read repeat-DSAs (additional,default and remote)    */
/*  Input value   : gsu_dsa * begin of DSA list                         */
/*                  char * name of DSA                                  */
/*  Date          : November 1993                                       */
/*  Author        : Eva Wilde, SNI BU BA NM12                           */
/*                                                                      */
/************************************************************************/
static void gsu_readline_repeat(
	    gsu_dsa *gsu_repeat_anf,           /* OUT - DSA liste  */
	    char *readline_repeat_string)      /* IN - DSA name  */
{
gsu_dsa *begin=NULL;
char *repeat_help=NULL;
char *readline_repeat_help_string=NULL;
signed16 readline_repeat_counter=1;
signed16 i;

begin=gsu_repeat_anf;
while (gsu_repeat_anf != NULL)
   {
   add_history(gsu_repeat_anf->psapadd);
   add_history(gsu_repeat_anf->dsaname);
   gsu_repeat_anf=gsu_repeat_anf->next;
   };
gsu_repeat_anf=begin;
while ((gsu_repeat_anf!=NULL)&&(readline_repeat_counter<GSU_MAXANZDSA))
   {
   sprintf(gsu_line,"%s<%d>> ",readline_repeat_string, readline_repeat_counter);
   repeat_help = readline(gsu_line);
   strcpy(gsu_repeat_anf->dsaname, repeat_help);
   if (gsu_repeat_anf->dsaname[0] != '\0')
      {
      while ((fprintf(stream,"%s\"%s\"\n",gsu_line,gsu_repeat_anf->dsaname))<0)
          {
	  fprintf(stderr, "ERROR\n");
          dce_fprintf(stderr, gsu_inter_write_error, interact_file);
	  repeat_help=(char *)dce_sprintf(gsu_inter_write_error,interact_file);
	  gsu_listing(repeat_help, NULL, NULL);
          exit(GSU_EXIT_15);
          };
      readline_repeat_help_string=(char *)dce_sprintf(gsu_psap_answer_sams);
      if ((readline_repeat_help_string=realloc(readline_repeat_help_string,strlen(readline_repeat_help_string)+strlen(" ")+1)) == NULL)
         {
         readline_repeat_help_string=(char *)dce_sprintf(gc_t_memory_insufficient);
	 fprintf(stderr, "ERROR\n");
         dce_fprintf(stderr,gc_t_memory_insufficient);
         gsu_listing(readline_repeat_help_string, NULL, NULL);
         free(readline_repeat_help_string);
         exit(GSU_EXIT_16);
         }
      strcat(readline_repeat_help_string, " ");
      sprintf(gsu_line, "%s<%d>> ",readline_repeat_help_string,readline_repeat_counter);
      free(readline_repeat_help_string);
      repeat_help = readline(gsu_line);
      strcpy(gsu_repeat_anf->psapadd, repeat_help);
      while ((fprintf(stream,"%s\"%s\"\n",gsu_line,gsu_repeat_anf->psapadd))<0)
	  {
	  fprintf(stderr, "ERROR\n");
	  dce_fprintf(stderr, gsu_inter_write_error, interact_file);
	  readline_repeat_help_string=(char *)dce_sprintf(gsu_inter_write_error,interact_file);
	  gsu_listing(readline_repeat_help_string,NULL,NULL);
	  exit(GSU_EXIT_17);
	  };
      gsu_repeat_anf=gsu_repeat_anf->next;
      readline_repeat_counter++;
      }
   else
      {
      i=1;
      while (i < (readline_repeat_counter-1))
	 {
	 begin=begin->next;
	 i++;
 	 };
      gsu_repeat_anf=begin;
      gsu_repeat_anf->next= NULL;
      gsu_repeat_anf=gsu_repeat_anf->next;
      return;
      }
   };
i=1;
while (i < (readline_repeat_counter-1))
   {
   begin=begin->next;
   i++;
   };
gsu_repeat_anf=begin;
gsu_repeat_anf->next=gsu_repeat(readline_repeat_string,stream,readline_repeat_counter);
}

/***************************************************************************/
/*                                                                         */
/*  Function name : gsu_readline_dsas                                      */
/*  Description   : Read repeat-DSAs with PSAP from file,                  */
/*                  change it and write it in file again                   */
/*  Input Value   : char * to name of DSA                                  */
/*                  char * to name of PSAP                                 */
/*                  char * to stream                                       */
/*  Date          : November, 1993                                         */
/*  Author        : Eva Wilde, SNI BU BA NM12                              */
/*                                                                         */
/***************************************************************************/
static unsigned16 gsu_readline_dsas(
		 char *gsu_dsa_dsa,   /* OUT - DSA name */
		 char *gsu_dsa_psap,  /* OUT - PSAP   */
		 char *gsu_dsa_ch)    /* IN - pointer */ 
{
char *readline_dsas_poi=NULL;
char *readline_dsas_help=NULL;
char *readline_dsas_history=NULL;
char *readline_dsas_string=NULL;
unsigned16 psapflg=0;

if ((readline_dsas_history=(char *)malloc(GSU_INPUT+1)) == NULL)
    {
    fprintf(stderr, "ERROR\n");
    dce_fprintf(stderr,gc_t_memory_insufficient);
    readline_dsas_poi=(char *)dce_sprintf(gc_t_memory_insufficient);
    gsu_listing(readline_dsas_poi,NULL,NULL);
    exit(GSU_EXIT_18);
    }
if ((readline_dsas_poi=strstr(gsu_dsa_ch, gsu_line)) != NULL)
   {
   if (!strcmp(readline_dsas_poi+strlen(gsu_line), "\"\""))
      {
      psapflg=0;
      strcpy(readline_dsas_history,"\0");
      }
   else
      {
      psapflg=1;
      sscanf((readline_dsas_poi+strlen(gsu_line)), "\"%s", readline_dsas_history);
      strcpy(readline_dsas_history+strlen(readline_dsas_history)-1,"\0");
      }
   }
else
   {
   psapflg=0;
   strcpy(readline_dsas_history,"\0");
   }
add_history(readline_dsas_history);
readline_dsas_help=readline(gsu_line);
strcpy(gsu_dsa_dsa, readline_dsas_help);
free(readline_dsas_help);
if ((gsu_dsa_dsa[0] != '\n')&&(gsu_dsa_dsa[0] != '\0'))
   {
   while ((fprintf(stream,"%s\"%s\"\n",gsu_line,gsu_dsa_dsa)) < 0)
      {
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr, gsu_inter_write_error, interact_file);
      readline_dsas_poi=(char *)dce_sprintf(gsu_inter_write_error,interact_file);
      gsu_listing(readline_dsas_poi,NULL,NULL);
      exit(GSU_EXIT_19);
      };
   readline_dsas_string=(char *)dce_sprintf(gsu_psap_answer_sams);
   if ((readline_dsas_string=realloc(readline_dsas_string,strlen(readline_dsas_string)+strlen("> ")+1)) == NULL)
      {
      readline_dsas_string=(char *)dce_sprintf(gc_t_memory_insufficient);
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr,gc_t_memory_insufficient);
      gsu_listing(readline_dsas_string, NULL, NULL);
      free(readline_dsas_string);
      exit(GSU_EXIT_20);
      }
   strcat(readline_dsas_string, "> ");
   strcpy(gsu_line, readline_dsas_string);
   free(readline_dsas_string);
   if ((readline_dsas_poi!='\0')&&((readline_dsas_poi=strstr(readline_dsas_poi,gsu_line))!=NULL))
      {
      sscanf((readline_dsas_poi+strlen(gsu_line)), "\"%s", readline_dsas_history);
      strcpy(readline_dsas_history+strlen(readline_dsas_history)-1,"\0");
      }
   else
      {
      if (psapflg == 0)
          strcpy(readline_dsas_history, "\0");
      else
          {
	  fprintf(stderr, "ERROR\n");
          dce_fprintf(stderr,gsu_readline_dsas_found,gsu_line,interact_file);
	  readline_dsas_poi=(char *)dce_sprintf(gsu_readline_dsas_found,gsu_line,interact_file);
	  gsu_listing(readline_dsas_poi,NULL,NULL);
          exit(GSU_EXIT_21);
          }
      }
   add_history(readline_dsas_history);
   readline_dsas_help=readline(gsu_line);
   strcpy(gsu_dsa_psap, readline_dsas_help);
   free(readline_dsas_help);
   while ((fprintf(stream, "%s\"%s\"\n", gsu_line, gsu_dsa_psap)) < 0)
	{
	fprintf(stderr, "ERROR\n");
	dce_fprintf(stderr,gsu_inter_write_error, interact_file);
	readline_dsas_poi=(char *)dce_sprintf(gsu_inter_write_error,interact_file);
	gsu_listing(readline_dsas_poi,NULL,NULL);
	exit(GSU_EXIT_22);
	};
   return(1);
   }
else
   {
   return(0);
   }
}

/**********************************************************************/
/*                                                                    */
/*  Function name  : gsu_readline                                     */
/*  Description    : readline-Editor                                  */
/*  Input Value    : char * name of variable if state='c'             */
/*                   int * name of variable if state='i'              */
/*                   char 'i' for integer and 'c' for character       */
/*                   char * to stream                                 */
/*  Date           : November, 1993                                   */
/*  Author         : Eva Wilde, SNI BU BA NM12                        */
/*                                                                    */
/**********************************************************************/
static unsigned16 gsu_readline(char *gsu_rl_var1,  /*OUT - output variable*/
			       int *gsu_rl_var2,   /*OUT - output variable*/
			       char gsu_rl_state,  /*IN - input variable*/
		               char *gsu_rl_ch)    /*IN - input varable*/
{
 char *readline_poi=NULL;
 char *readline_help=NULL;
 char *readline_history=NULL;

 if ((readline_history=(char *)malloc(GSU_INPUT+1)) == NULL)
    {
    fprintf(stderr, "ERROR\n");
    dce_fprintf(stderr,gc_t_memory_insufficient);
    readline_poi=(char *)dce_sprintf(gc_t_memory_insufficient);
    gsu_listing(readline_poi,NULL,NULL);
    exit(GSU_EXIT_23);
    }
 if (gsu_rl_state == 'i')
     {
     if ((readline_poi=strstr(gsu_rl_ch, gsu_line)) != NULL)
         {
	 sscanf(readline_poi+strlen(gsu_line)+strlen("\""), "%s", readline_history);
	 readline_history[strlen(readline_history)-1] = '\0';
         }
     else
	 strcpy(readline_history, "\0");
      add_history(readline_history);
      *gsu_rl_var2 = atoi(readline_help=readline(gsu_line));
      if (*gsu_rl_var2 == 0)
	  {
	  return(0);
	  }
      else
	  {
          return(1);
	  }
     }
 else
    {
    if (gsu_rl_state == 'c')
       {
        if ((readline_poi=strstr(gsu_rl_ch, gsu_line)) != NULL)
	     {
             sscanf((readline_poi+strlen(gsu_line)), "\"%s", readline_history);
	     readline_history[strlen(readline_history)-1] = '\0';
	     }
        else
	     strcpy(readline_history, "\0");
        add_history(readline_history);
        readline_help = readline(gsu_line);
	strcpy(gsu_rl_var1, readline_help);
       if (gsu_rl_var1[0] == '\0')
	   {
	   return(0);
	   }
       else
	   {
	   return(1);
	   }
       }
   }
}

/***********************************************************************/
/*                                                                     */
/*  Function name :  gsu_nointeract                                    */
/*  Description   :  File input mode                                   */
/*  Input Value   :  char * to parameter file                          */
/*  Date          :  November, 1993                                    */
/*  Author        :  Eva Wilde, BU BA KP13                             */
/*                                                                     */
/***********************************************************************/
static void gsu_nointeract(char *gsu_nointer_datei)  /*IN - input file*/
{
unsigned8 dsaflg;
unsigned8 psapflg;
signed16 i;
unsigned16 nointer_anzbl=0;
unsigned16 nointer_anzbyte=0;
char nointer_int_string[GSU_LENINT]="\0";
char * nointer_str=NULL;
char * false_default_psap=NULL;
char * ch=NULL;
gsu_dsa *act=NULL;
FILE * stream=NULL;
struct stat *puff=NULL;

if ((stat(gsu_nointer_datei,puff)) == 0)   /* Dateistatus setzen */
   {
   if ((puff->st_size) = 0)              /* Groesse abfragen */
      {
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr,gsu_existing_error);
      nointer_str=(char *)dce_sprintf(gsu_existing_error);
      gsu_listing(nointer_str,NULL,NULL);
      exit(GSU_EXIT_127);
      }
   }
/***  store file without kommentare   ***/
if ((nointer_str=(char *)malloc(strlen("grep -v \"^#\"   >/tmp/gsu_nointer_file")+strlen(gsu_nointer_datei)+1)) == NULL)
   {
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr,gc_t_memory_insufficient);
   nointer_str=(char *)dce_sprintf(gc_t_memory_insufficient);
   gsu_listing(nointer_str,NULL,NULL);
   exit(GSU_EXIT_24);
   }
strcpy(nointer_str," grep -v \"^#\" " );
strcat(nointer_str, gsu_nointer_datei);
strcat(nointer_str, " >/tmp/gsu_nointer_file");
gsu_listing(nointer_str, NULL, NULL);
if (system(nointer_str) < 0)
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "%s\n", nointer_str);
   gsu_listing("system ERROR of ", nointer_str, NULL);
   exit(GSU_EXIT_25);
   }
free(nointer_str);

/*** open file to read and store if in memory "ch"   ***/
gsu_listing("fopen (/tmp/gsu_nointer_file, r)", NULL, NULL); 
if ((stream=fopen("/tmp/gsu_nointer_file", "r")) == NULL)
     {
     fprintf(stderr, "ERROR\n");
     fprintf(stderr, "fopen(/tmp/gsu_nointer_file)\n");
     gsu_listing("fopen(/tmp/gsu_nointer_file) ERROR",NULL,NULL);
     exit(GSU_EXIT_26);
     }
nointer_anzbl=0;
nointer_anzbyte=GSU_BLSIZE;
ch = NULL;
while (nointer_anzbyte == GSU_BLSIZE)
      {
      if (ch == NULL)
         {
          if ((ch = malloc (GSU_BLSIZE+1))==NULL)
             {
             nointer_str=(char *)dce_sprintf(gc_t_memory_insufficient);
	     fprintf(stderr, "ERROR\n");
	     dce_fprintf(stderr,gc_t_memory_insufficient);
	     gsu_listing(nointer_str, NULL, NULL);
	     free(nointer_str);
             exit(GSU_EXIT_27);
             }
         }
      else
         {
         if ((ch=realloc(ch, (1+nointer_anzbl)*GSU_BLSIZE+1)) == NULL)
             {
	     nointer_str=(char *)dce_sprintf(gc_t_memory_insufficient);
	     fprintf(stderr, "ERROR\n");
	     dce_fprintf(stderr,gc_t_memory_insufficient);
	     gsu_listing(nointer_str, NULL, NULL);
	     free(nointer_str);
             exit(GSU_EXIT_28);
             }
         } 
       nointer_anzbyte=fread(&ch[nointer_anzbl*GSU_BLSIZE],sizeof(char),GSU_BLSIZE, stream);
       nointer_anzbl++;
      };
gsu_listing("system(rm /tmp/gsu_nointer_file)", NULL, NULL);
if (system("rm /tmp/gsu_nointer_file") < 0)
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "rm /tmp/gsu_nointer_file\n");
   gsu_listing("system ERROR of rm /tmp/gsu_nointer_file", NULL, NULL);
   exit(GSU_EXIT_29);
   }

/******************************************************************/
/***   set all variables for configuration and administration   ***/ 
/******************************************************************/

	     /*********************************/
             /***     CONFIGURATION DATA    ***/
	     /*********************************/
nointer_str=(char *)dce_sprintf(gsu_dirid_answer_sams);
if (gsu_noint_read(nointer_str,NULL,&gsu_dirid,'i',0,ch) == 0 )
    {
    /*** set default value   ***/
    gsu_dirid = 1;
    }
gsu_listing("gsu_noint_read(", nointer_str, ",...)");
sprintf(nointer_int_string, "%d", gsu_dirid);
gsu_listing("	gsu_dirid = ", nointer_int_string, NULL);
free(nointer_str);

nointer_str=(char *)dce_sprintf(gsu_cladd_answer_sams);
if (gsu_noint_read(nointer_str,gsu_cladd,0,'c',GSU_PSAP,ch)==0)
   {
   /*** set default value  ***/
   strcpy(gsu_cladd, gsu_cladd_default_value);
   }
gsu_listing("gsu_noint_read(", nointer_str, ",...)");
gsu_listing("	gsu_cladd= ", gsu_cladd, NULL);
free(nointer_str);

nointer_str=(char *)dce_sprintf(gsu_config_answer_sams);
if (gsu_noint_read(nointer_str,gsu_config,0,'c',GSU_CONFIG,ch)==0)
   {
   /***   set default value   ***/
   strcpy(gsu_config,gsu_client_sys);
   }
gsu_listing("gsu_noint_read(", nointer_str, ",...)");
gsu_listing("	gsu_config = ", gsu_config, NULL);
free(nointer_str);

gsu_config[0] = (char)toupper(gsu_config[0]);
if (!strcmp(gsu_config, gsu_client_sys))
   {
   /**************************/
   /***    CLIENT SYSTEM   ***/
   /**************************/
   initmode = 1;
	     
	     /***   INITIALIZATION DATA  ***/
   nointer_str=(char *)dce_sprintf(gsu_locdsa_answer_sams);
   if((gsu_noint_dsas(nointer_str,gsu_locdsa,gsu_psaploc,ch))!=0)
      {
      free(nointer_str);
      nointer_str=(char *)dce_sprintf(gsu_no_local_poss);
      dce_fprintf(stderr, gsu_no_local_poss);
      gsu_listing(nointer_str, NULL, NULL);
      }
   free(nointer_str);
	     /***   END OF INITIALIZATION DATA  ***/

   nointer_str=(char *)dce_sprintf(gsu_userpsw_answer_sams);
   if((gsu_noint_read(nointer_str,gsu_userpsw,0,'c',GSU_USERPSW,ch))!=0)
      {
      free(nointer_str);
      nointer_str=(char *)dce_sprintf(gsu_password_ignor);
      dce_fprintf(stderr, gsu_password_ignor);
      gsu_listing(nointer_str, NULL, NULL);
      }
   free(nointer_str);

   nointer_str=(char *)dce_sprintf(gsu_nocl_answer_sams);
   if((gsu_noint_read(nointer_str,NULL, &gsu_nocl, 'i',0, ch))==0)
      {
      /***   set default value   ***/
      gsu_nocl = 16;
      }
   sprintf(nointer_int_string, "%d", gsu_nocl);
   gsu_listing("gsu_noint_read(", nointer_str, ",...)");
   gsu_listing("	gsu_nocl= ", nointer_int_string, NULL);
   free(nointer_str);
   
   nointer_str=(char *)dce_sprintf(gsu_noser_answer_sams);
   if((gsu_noint_read(nointer_str,NULL, &gsu_noser, 'i',0, ch))!=0)
      {
      free(nointer_str);
      nointer_str=(char *)dce_sprintf(gsu_no_server_ignor);
      dce_fprintf(stderr, gsu_no_server_ignor);
      gsu_listing(nointer_str, NULL, NULL);
      }
   free(nointer_str);

   nointer_str=(char *)dce_sprintf(gsu_contdsa_answer_sams);
   if (gsu_noint_dsas(nointer_str,gsu_contdsa,gsu_psapcont,ch)!=0)
      {
      free(nointer_str);
      nointer_str=(char *)dce_sprintf(gsu_cont_dsa_ignor);
      dce_fprintf(stderr, gsu_cont_dsa_ignor);
      gsu_listing(nointer_str, NULL, NULL);
      }
   free(nointer_str);

   nointer_str=(char *)dce_sprintf(gsu_locals_answer_sams);
   if(gsu_noint_read(nointer_str,gsu_locals,0,'c',GSU_LOCALS,ch)!=0)
      {
      free(nointer_str);
      nointer_str=(char *)dce_sprintf(gsu_local_schema_ignor);
      dce_fprintf(stderr, gsu_local_schema_ignor);
      gsu_listing(nointer_str, NULL, NULL);
      }
   free(nointer_str);

   nointer_str=(char *)dce_sprintf(gsu_defsch_answer_sams);
   if(gsu_noint_read(nointer_str,gsu_defsch,0,'c',GSU_DEFSCH,ch)!=0)
      {
      free(nointer_str);
      nointer_str=(char *)dce_sprintf(gsu_def_schema_ignor);
      dce_fprintf(stderr, gsu_def_schema_ignor);
      gsu_listing(nointer_str, NULL, NULL);
      }
   free(nointer_str);

   nointer_str=(char *)dce_sprintf(gsu_secur_answer_sams);
   if((gsu_noint_read(nointer_str, gsu_e_secur, 0, 'c',GSU_SECUR,ch))!=0)
      {
      free(nointer_str);
      nointer_str=(char *)dce_sprintf(gsu_secur_ignor);
      dce_fprintf(stderr, gsu_secur_ignor);
      gsu_listing(nointer_str, NULL, NULL);
      }
   free(nointer_str);

   nointer_str=(char *)dce_sprintf(gsu_princ_answer_sams);
   if((gsu_noint_read(nointer_str,gsu_princ,0,'c', GSU_PRINC, ch))!=0)
      {
      free(nointer_str);
      nointer_str=(char *)dce_sprintf(gsu_princ_ignor);
      dce_fprintf(stderr, gsu_princ_ignor);
      gsu_listing(nointer_str, NULL, NULL);
      }
   free(nointer_str);

	    /******************************************/
            /***   ADDITIONAL ADMINISTRATION DATA   ***/
	    /******************************************/
   nointer_str=(char *)dce_sprintf(gsu_def_dsa_answer_sams);
   anfang2 = gsu_noint_repeat(nointer_str,GSU_DSA, ch);
   gsu_listing("gsu_noint_read(", nointer_str, ",...)");
   act = anfang2;
   while (act != NULL)
      {
      gsu_listing("	dsaname = ", act->dsaname, NULL);
      gsu_listing("	psap    = ", act->psapadd, NULL);
      act = act->next;
      };
   free(nointer_str);
   }
else
   {
   for (i=0; i<GSU_CONFIG; i++)
      gsu_config[i] = (char)toupper(gsu_config[i]);
   if (!strcmp(gsu_config, gsu_server_sys))
   /*******************************/
   /***   Client/Server system ****/
   /*******************************/
      {
      nointer_str=(char *)dce_sprintf(gsu_nocl_answer_sams);
      if((gsu_noint_read(nointer_str,NULL, &gsu_nocl, 'i',0, ch))==0)
         {
         /***   set default value   ***/
         gsu_nocl=16;
         }
      gsu_listing("gsu_noint_read(", nointer_str, ",...)");
      sprintf(nointer_int_string, "%d", gsu_nocl);
      gsu_listing("	gsu_nocl = ", nointer_int_string, NULL);
      free(nointer_str);

      nointer_str=(char *)dce_sprintf(gsu_noser_answer_sams);
      if((gsu_noint_read(nointer_str,NULL, &gsu_noser, 'i',0, ch))==0)
         {
         /***   set default value   ***/
         gsu_noser=2;
         }
      gsu_listing("gsu_noint_read(", nointer_str, ",...)");
      sprintf(nointer_int_string, "%d", gsu_noser);
      gsu_listing("	gsu_noser = ", nointer_int_string, NULL);
      free(nointer_str);

      nointer_str=(char *)dce_sprintf(gsu_secur_answer_sams);
      if (gsu_noint_read(nointer_str,gsu_e_secur,0,'c',GSU_SECUR,ch)==0)
         {
         /***   set default value   ***/
         strcpy(gsu_e_secur, gsu_secur_dce);
         }
      gsu_listing("gsu_noint_read(", nointer_str, ",...)");
      gsu_listing("	gsu_secur = ", gsu_e_secur, NULL);
      free(nointer_str);
	    /***   END OF CONFIGURATZION DATA   ***/

            /***    INITIALIZATION DATA    ***/
      nointer_str=(char *)dce_sprintf(gsu_locdsa_answer_sams);
      if(gsu_noint_dsas(nointer_str,gsu_locdsa,gsu_psaploc, ch)==0)
         {
	 free(nointer_str);
         nointer_str=(char *)dce_sprintf(gsu_missing_local_dsa_name);
	 fprintf(stderr, "ERROR\n");
	 dce_fprintf(stderr, gsu_missing_local_dsa_name);
	 gsu_listing(nointer_str, NULL, NULL);
	 free(nointer_str);
         exit(GSU_EXIT_30);
         }
      gsu_listing("gsu_noint_dsas(", nointer_str, ",...)");
      gsu_listing("	gsu_locdsa = ", gsu_locdsa, NULL);
      free(nointer_str);

      nointer_str=(char *)dce_sprintf(gsu_userpsw_answer_sams);
      if ((gsu_noint_read(nointer_str,gsu_userpsw,0,'c',GSU_USERPSW,ch)==0)&&(!strcmp(gsu_e_secur,gsu_secur_simple)))
         {
	 free(nointer_str);
         nointer_str=(char *)dce_sprintf(gc_t_missing_user_password);
	 fprintf(stderr, "ERROR\n");
	 dce_fprintf(stderr, gc_t_missing_user_password);
	 gsu_listing(nointer_str, NULL, NULL);
	 free(nointer_str);
         exit(GSU_EXIT_31);
         }
      gsu_listing("gsu_noint_read(", nointer_str, ",...)");
      gsu_listing("	gsu_userpsw = ", gsu_userpsw, NULL);
      free(nointer_str);

      nointer_str=(char *)dce_sprintf(gsu_locals_answer_sams);
      if (gsu_noint_read(nointer_str,gsu_locals,0,'c',GSU_LOCALS,ch)==0)
	 {
	 /***   set default value   ***/
         strcpy(gsu_locals, gsu_answer_true);
	 }
      gsu_listing("gsu_noint_read(", nointer_str, ",...)");
      gsu_listing("	gsu_locals = ", gsu_locals, NULL);
      free(nointer_str);

      nointer_str=(char *)dce_sprintf(gsu_defsch_answer_sams);
      if((gsu_noint_read(nointer_str, gsu_defsch,0,'c',GSU_DEFSCH,ch))==0)
	 {
	 /***   set default value  ***/
	 strcpy(gsu_defsch,gsu_answer_true);
	 }
      gsu_listing("gsu_noint_read(", nointer_str, ",...)");
      gsu_listing("	gsu_defsch = ", gsu_defsch, NULL);
      free(nointer_str);

      nointer_str=(char *)dce_sprintf(gsu_contdsa_answer_sams);
      gsu_noint_dsas(nointer_str,gsu_contdsa,gsu_psapcont, ch);
      gsu_listing("gsu_noint_dsas(", nointer_str, ",...)");
      gsu_listing("	gsu_contdsa = ", gsu_contdsa, NULL);
      free(nointer_str);

      if (!strcmp(gsu_e_secur,gsu_secur_dce))
          {
          nointer_str=(char *)dce_sprintf(gsu_princ_answer_sams);
          if (gsu_noint_read(nointer_str,gsu_princ,0,'c',GSU_PRINC,ch)==0)
            {
	    free(nointer_str);
            nointer_str=(char *)dce_sprintf(gsu_missing_principal_name);
	    fprintf(stderr, "ERROR\n");
	    dce_fprintf(stderr, gsu_missing_principal_name);
	    gsu_listing(nointer_str, NULL, NULL);
	    free(nointer_str);
            exit(GSU_EXIT_32); 
            }
          gsu_listing("gsu_noint_read(", nointer_str, ",...)");
          gsu_listing("	gsu_princ = ", gsu_princ, NULL);
          free(nointer_str);
          }
	    /***    END OF INITIALIZATION DATA    ***/ 

     /*****   Discrimination of init-modes   *****/
     if ((gsu_contdsa[0] == '\n')||(gsu_contdsa[0] == '\0'))
         {
         if (!strcmp(gsu_locals, gsu_answer_true))
	    {
            if (!strcmp(gsu_defsch, gsu_answer_true))
		 {
	         initmode = 2;
		 act=anfang2;
		 while (act != NULL)
		    {
		    if (!strcmp(act->dsaname,gsu_locdsa))
		       {
		       dsaflg=1;
		       if (strcmp(act->psapadd, gsu_psaploc))
			  {
			  nointer_str=(char *)dce_sprintf(gsu_psap_must_same,act->dsaname);
			  fprintf(stderr, "ERROR\n");
			  dce_fprintf(stderr,gsu_psap_must_same,act->dsaname);
			  gsu_listing(nointer_str,NULL,NULL);
			  free(nointer_str);
			  exit(GSU_EXIT_33);
			  }
		       }
		    };
		 if ( dsaflg == 0)
		    {
		    if ((act=(gsu_dsa *)malloc(sizeof(gsu_dsa)+1)) == NULL)
		       {
		       nointer_str=(char *)dce_sprintf(gc_t_memory_insufficient);
		       fprintf(stderr, "ERROR\n");
		       dce_fprintf(stderr,gc_t_memory_insufficient);
		       gsu_listing(nointer_str, NULL, NULL);
		       free(nointer_str);
		       exit(GSU_EXIT_34);
		       }
		    act->next=anfang2;
		    strcpy(act->dsaname,gsu_locdsa);
		    strcpy(act->psapadd,gsu_psaploc);
		    anfang2 = act;
		    act = anfang2;
		    gsu_listing("neu default DSA's (with local DSA)",NULL,NULL);
		    while (act != NULL)
		       {
		       gsu_listing("	dsaname = ", act->dsaname, NULL);
		       gsu_listing("	psapadd = ", act->psapadd, NULL);
		       act = act->next;
		       };
		    }
		 }
            else
               {
               nointer_str=(char *)dce_sprintf(gsu_default_schema_failed,"gsu_nointeract");
	       fprintf(stderr, "ERROR\n");
	       dce_fprintf(stderr, gsu_default_schema_failed,"gsu_nointeract");
	       gsu_listing(nointer_str, NULL, NULL);
	       free(nointer_str);
               exit(GSU_EXIT_35);
               }
            }
	 else
	    {
	    nointer_str=(char *)dce_sprintf(gsu_local_schema_error);
	    fprintf(stderr, "ERROR\n");
	    dce_fprintf(stderr, gsu_local_schema_error);
	    gsu_listing(nointer_str, NULL, NULL);
	    free(nointer_str);
	    exit(GSU_EXIT_36);
	    }
         }
      else
         {
         if (!strcmp(gsu_locals, gsu_answer_true))
	    {
            if (!strcmp(gsu_defsch, gsu_answer_true))
	         initmode = 3;
	    else
	         {
                 nointer_str=(char *)dce_sprintf(gsu_default_schema_failed, "gsu_nointeract");
		 fprintf(stderr, "ERROR\n");
		 dce_fprintf(stderr,gsu_default_schema_failed, "gsu_nointeract");
		 gsu_listing(nointer_str,NULL,NULL);
		 free(nointer_str);
                 exit(GSU_EXIT_37);
	         }
	    }
         else
	    {
	    if (!strcmp(gsu_locals, gsu_answer_false))
	       {
	       if (!strcmp(gsu_defsch, gsu_answer_true))
	           initmode = 4;
	       else 
	          initmode = 5;
	       }
	    else
	       {
	       nointer_str=(char *)dce_sprintf(gsu_default_schema_failed,"gsu_nointeract");
	       fprintf(stderr, "ERROR\n");
	       dce_fprintf(stderr, gsu_default_schema_failed, "gsu_nointeract");
	       gsu_listing(nointer_str,NULL,NULL);
	       free(nointer_str);
	       exit(GSU_EXIT_38);
	       }
	    }
	 }
      /*****   End of Discrimination of init-modes   *****/

	    /******************************************/
            /***   ADDITIONAL ADMINISTRATION DATA   ***/
	    /******************************************/
      nointer_str=(char *)dce_sprintf(gsu_local_equal_default);
      if (gsu_noint_read(nointer_str,gsu_loc_def,0,'c',GSU_LOC_DEF,ch)==0)
         {
         gsu_config[0] = (char)toupper(gsu_config[0]);
         if (!strcmp(gsu_config, gsu_server_sys))
            {
            /*** set default value  ***/
            strcpy(gsu_loc_def, gsu_answer_true);
            }
         }
      gsu_listing("gsu_noint_read(", nointer_str, ",...)");
      gsu_listing("	gsu_loc_def = ", gsu_loc_def, NULL);
      free(nointer_str);
       
      nointer_str=(char *)dce_sprintf(gsu_def_dsa_answer_sams);
      if (!strcmp(gsu_loc_def, gsu_answer_true))
         {
         if ((anfang2=(gsu_dsa *)malloc(sizeof(gsu_dsa)+1)) == NULL)
            {
	    free(nointer_str);
            nointer_str=(char *)dce_sprintf(gc_t_memory_insufficient);
	    fprintf(stderr, "ERROR\n");
	    dce_fprintf(stderr,gc_t_memory_insufficient);
	    gsu_listing(nointer_str,NULL,NULL);
            exit(GSU_EXIT_39);
            }
         strcpy(anfang2->dsaname,gsu_locdsa);
         strcpy(anfang2->psapadd,gsu_psaploc);
         anfang2->next = gsu_noint_repeat(nointer_str,GSU_DSA, ch);
         }
      else
         {
         anfang2 = gsu_noint_repeat(nointer_str,GSU_DSA, ch);
         }
      gsu_listing("gsu_noint_read(", nointer_str, ",...)");
      act = anfang2;
      while (act != NULL)
         {
         if (!strcmp(gsu_locdsa, act->dsaname))
            {
            false_default_psap = act->dsaname;
            if (strcmp(gsu_psaploc, act->psapadd))
	       {
	       psapflg = 1;
	       }
            }
         gsu_listing("	dsaname = ", act->dsaname, NULL);
         gsu_listing("	psap    = ", act->psapadd, NULL);
         act = act->next;
         };
      if (psapflg == 1)
         {
	 free(nointer_str);
         nointer_str=(char *)dce_sprintf(gsu_psap_must_same,false_default_psap);
	 fprintf(stderr, "ERROR\n");
	 dce_fprintf(stderr, gsu_psap_must_same, false_default_psap);
	 gsu_listing(nointer_str, NULL, NULL);
	 free(nointer_str);
         exit(GSU_EXIT_40);
         }
      free(nointer_str);
      }
   else
      {
      free(nointer_str);
      nointer_str=(char *)dce_sprintf(gsu_config_type_failed, "gsu_nointeract");
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr, gsu_config_type_failed, "gsu_nointeract");
      gsu_listing(nointer_str, NULL, NULL);
      free(nointer_str);
      exit(GSU_EXIT_41);
      }
  }

	    /******************************************/
            /***   ADDITIONAL ADMINISTRATION DATA   ***/
	    /******************************************/
nointer_str=(char *)dce_sprintf(gsu_add_dsa_answer_sams);
anfang1 = gsu_noint_repeat(nointer_str,GSU_DSA, ch);
gsu_listing("gsu_noint_read(", nointer_str, ",...)");
act = anfang1;
while (act != NULL)
   {
   gsu_listing("	dsaname = ", act->dsaname, NULL);
   gsu_listing("	psap    = ", act->psapadd, NULL);
   act = act->next;
   };
free(nointer_str);

            /***   END OF ADDITIONAL ADMINISTRATION DATA   ***/

/***   close file   ***/
gsu_listing("fclose /tmp/gsu_nointer_file", NULL, NULL);
if (fclose(stream) != 0)
   {
   nointer_str=(char *)dce_sprintf(gsu_fclose_error, "/tmp/gsu_nointer_file");
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gsu_fclose_error, "/tmp/gsu_nointer_file");
   gsu_listing(nointer_str, NULL, NULL);
   exit(GSU_EXIT_42);
   }
}
 
/***********************************************************************/
/*                                                                     */
/*  Function name :  gsu_noint_read                                    */
/*  Description   :  Search "var<#>" in Puffer                         */
/*  Input Value   :  char * to prompt string                           */
/*                   char * to variable if state = 'c'                 */ 
/*                   int * to variable if state = 'i'                  */
/*                   char 'i' for integer or 'c' for character         */
/*                   unsigned16 for lenght of variable                 */
/*                   char * to stream                                  */ 
/*  Author        :  Eva Wilde, SNI BU BA KP13                         */
/*                                                                     */
/***********************************************************************/
static unsigned16 gsu_noint_read (char *string,  /*IN - input variable*/
			         char *var1,     /*OUT - output variable*/
			         int *var2,      /*OUT - output variable*/
			         char state,     /*IN - input variable*/
			         signed16 maxim, /*IN - input variable*/
			         char *noint_read_ch)/*IN - input variable*/
{
signed16 j;
char * noint_read_poi=NULL;

if (state == 'i')
   {
   if ((noint_read_poi=strstr(noint_read_ch, string)) != NULL)
      {
      sscanf((noint_read_poi+strlen(string)), "\"%d",(int *)var2);
      return(1);
      }
   return(0);
   }
else
   {
   if((noint_read_poi=strstr(noint_read_ch, string)) != NULL)
      {
      j=0;
      noint_read_poi=noint_read_poi+strlen(string)+strlen("\"");
      while((*noint_read_poi != '"')&&(j < maxim))
        {
        var1[j++]=*noint_read_poi;
        noint_read_poi++;
        }
      if (j > 0)
         return(1);
      else
         return(0);
      }
   else
      return(0);
   }
}

/****************************************************************************/
/*                                                                          */
/*   Function name  :  gsu_noint_dsas                                       */
/*   Description    :  read DSA-name with PSAP in noint_dsas_ch             */
/*   Input Value    :  char * to promt                                      */
/*                     char * to DSA name                                   */
/*                     char * to PSAP name                                  */
/*                     char * to stream                                     */
/*   Author         :  Eva Wilde, SNI BA NM123                              */
/*                                                                          */
/****************************************************************************/
static unsigned16 gsu_noint_dsas(char *string,     /*IN - input variable*/
			        char *gsu_dsas_dsa,/*OUT - output variable*/
			        char *gsu_dsas_psap,/*OUT - output variable*/ 
		                char *noint_dsas_ch)/*IN - input variable*/
{
char *noint_dsas_poi=NULL;
char *noint_dsas_string=NULL;
signed16 j;

if ((noint_dsas_poi=strstr(noint_dsas_ch, string)) != NULL)
   {
   j=0;
   noint_dsas_poi=noint_dsas_poi+strlen(string)+1;
   while((*noint_dsas_poi != '"')&&(j < GSU_DSA))
     {
     gsu_dsas_dsa[j++]=*noint_dsas_poi;
     noint_dsas_poi++;
     };
   if (j > 0)
      {
      noint_dsas_string=(char *)dce_sprintf(gsu_psap_answer_sams);
      if ((noint_dsas_string=realloc(noint_dsas_string,strlen(noint_dsas_string)+strlen("> ")+1)) == NULL)
         {
         noint_dsas_string=(char *)dce_sprintf(gc_t_memory_insufficient);
	 fprintf(stderr, "ERROR\n");
         dce_fprintf(stderr,gc_t_memory_insufficient);
         gsu_listing(noint_dsas_string, NULL, NULL);
         free(noint_dsas_string);
         exit(GSU_EXIT_43);
         }
      strcat(noint_dsas_string, "> ");
      if ((noint_dsas_poi=strstr(noint_dsas_poi,noint_dsas_string))!=NULL)
	 {
	 j=0;
	 noint_dsas_poi=noint_dsas_poi+strlen(noint_dsas_string)+strlen("\"");
	 free(noint_dsas_string);
	 while((*noint_dsas_poi != '"')&&(j < GSU_PSAP))
	    {
	    gsu_dsas_psap[j++]=*noint_dsas_poi;
	    noint_dsas_poi++;
	    };
	 }
      else 
	 {
	 free(noint_dsas_string);
	 fprintf(stderr, "ERROR\n");
	 dce_fprintf(stderr, gsu_missing_psap_name, gsu_dsas_dsa);
	 noint_dsas_poi=(char *)dce_sprintf(gsu_missing_psap_name,gsu_dsas_dsa);
	 gsu_listing(noint_dsas_poi,NULL,NULL);
	 exit(GSU_EXIT_44);
	 }
      return(1);
      }
   else
   return(0);
   }
else
   return(0);
}

/***************************************************************************/
/*                                                                         */
/*  File Name     : gsu_repeat                                             */
/*  Description   : Write default and additional remote DSA's in input     */
/*                  mode in file                                           */
/*  Input Value   : char * input's strings,                                */
/*                  FILE * opened File,                                    */
/*                  unsigned16 number of DSA                               */
/*  Return Value  : Pointer to List                                        */
/*  Author        : Eva Wilde, SNI BU BA KP13                              */
/*                                                                         */
/***************************************************************************/
static gsu_dsa * gsu_repeat(char * gsu_repeat_frage,/*IN - input variable*/
			    FILE * fd,              /*IN - input variable*/
			    signed16 zaehler)       /*IN - input variable*/
{
signed16 i;
unsigned16 flg = 1;
gsu_dsa * anfang = NULL;
gsu_dsa * act = NULL;
char *repeat_help=NULL;
char * repeat_string=NULL;

if ((act=(gsu_dsa *)malloc(sizeof(gsu_dsa)+1)) == NULL)
    {
    fprintf(stderr, "ERROR\n");
    dce_fprintf(stderr,gc_t_memory_insufficient);
    repeat_help=(char *)dce_sprintf(gc_t_memory_insufficient);
    gsu_listing(repeat_help,NULL,NULL);
    exit(GSU_EXIT_45);
    }
anfang = act;
i=zaehler;
while (flg == 1)
    {
    sprintf(gsu_line, "%s<%d>> ",gsu_repeat_frage, zaehler);
    add_history("\0");
    repeat_help = readline(gsu_line);
    strcpy(act->dsaname, repeat_help);
    if (act->dsaname[0] == '\0')
	{
	if (zaehler > i)
	  {
	  act=anfang;
	  zaehler=zaehler-i;
	  i=1;
	  while (i < zaehler)
	     {
	     act=act->next;
	     i++;
	     };
	  act->next=NULL;
	  flg = 0;
          return(anfang);
	  }
       else
	  {
	  anfang = NULL;
	  return(anfang);
	  }
       }
    repeat_string=(char *)dce_sprintf(gsu_psap_answer_sams);
    if ((repeat_string=realloc(repeat_string,strlen(repeat_string)+strlen(" ")+1)) == NULL)
         {
         repeat_string=(char *)dce_sprintf(gc_t_memory_insufficient);
	 fprintf(stderr, "ERROR\n");
         dce_fprintf(stderr,gc_t_memory_insufficient);
         gsu_listing(repeat_string, NULL, NULL);
         free(repeat_string);
         exit(GSU_EXIT_46);
         }
    strcat(repeat_string, " ");
    sprintf(gsu_line, "%s<%d>> ",repeat_string, zaehler);
    add_history("\0");
    repeat_help = readline(gsu_line);
    strcpy(act->psapadd, repeat_help);
    while ((fprintf(fd,"%s<%d>> \"%s\"\n",gsu_repeat_frage,zaehler,act->dsaname))<0)
       {
       fprintf(stderr, "ERROR\n");
       dce_fprintf(stderr, gsu_inter_write_error, interact_file);
       repeat_string=(char *)dce_sprintf(gsu_inter_write_error,interact_file);
       gsu_listing(repeat_string, NULL, NULL);
       exit(GSU_EXIT_47);
       };
    while ((fprintf(fd,"%s<%d>> \"%s\"\n",repeat_string,zaehler,act->psapadd))<0)
       {
       fprintf(stderr, "ERROR\n");
       dce_fprintf(stderr, gsu_inter_write_error, interact_file);
       repeat_string=(char *)dce_sprintf(gsu_inter_write_error,interact_file);
       gsu_listing(repeat_string, NULL, NULL);
       exit(GSU_EXIT_48);
       };
    free(repeat_string);
    if ((act->next=(gsu_dsa *)malloc(sizeof(gsu_dsa)+1)) == NULL)
       {
       fprintf(stderr, "ERROR\n");
       dce_fprintf(stderr,gc_t_memory_insufficient);
       repeat_string=(char *)dce_sprintf(gc_t_memory_insufficient);
       gsu_listing(repeat_string, NULL, NULL);
       exit(GSU_EXIT_49);
       }
    act = act->next;
    act->next=NULL;
    zaehler++;
    }
return(anfang);
}

/**************************************************************************/
/*                                                                        */
/*  Function name  :  gsu_noint_repeat                                    */
/*  Description    :  Read and store DSA in memory (for file mode)        */
/*  Input value    :  char * to promt,                                    */
/*                    unsigned16 max. length,                             */
/*                    char * to stream                                    */
/*  Return value   :  pointer to List                                     */
/*  Author         :  Eva Wilde, BU BA KP13                               */
/*                                                                        */
/**************************************************************************/
static gsu_dsa * gsu_noint_repeat(char * string1,    /*IN - input variable*/
				  signed16 maxim,    /*IN - input variable*/
				  char * noint_repeat_ch)    /*IN - input */
{
signed16 j;
signed16 zaehler = 1;
unsigned16 flg = 1;
char * poi=NULL;
char * poi1=NULL;
char * strdsa=NULL;
char * strpsap=NULL;
char * noint_repeat_string=NULL;
gsu_dsa * anfang = NULL;
gsu_dsa * act = NULL;

if ((act=(gsu_dsa *)malloc(sizeof(gsu_dsa)+1)) == NULL)
    {
    fprintf(stderr, "ERROR\n");
    dce_fprintf(stderr,gc_t_memory_insufficient);
    noint_repeat_string=(char *)dce_sprintf(gc_t_memory_insufficient);
    gsu_listing(noint_repeat_string,NULL,NULL);
    exit(GSU_EXIT_50);
    }
act->next=NULL;
anfang = act;
if ((strdsa=(char *)malloc(strlen(string1)+sizeof("<>>\" ")+sizeof(int)+1))==NULL)
    {
    fprintf(stderr, "ERROR\n");
    dce_fprintf(stderr,gc_t_memory_insufficient);
    noint_repeat_string=(char *)dce_sprintf(gc_t_memory_insufficient);
    gsu_listing(noint_repeat_string,NULL,NULL);
    exit(GSU_EXIT_51);
    }
noint_repeat_string=(char *)dce_sprintf(gsu_psap_answer_sams);
if ((noint_repeat_string=realloc(noint_repeat_string,strlen(noint_repeat_string)+strlen("> ")+1)) == NULL)
   {
   noint_repeat_string=(char *)dce_sprintf(gc_t_memory_insufficient);
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr,gc_t_memory_insufficient);
   gsu_listing(noint_repeat_string, NULL, NULL);
   free(noint_repeat_string);
   exit(GSU_EXIT_52);
   }
strcat(noint_repeat_string, "> ");
if ((strpsap=(char *)malloc(sizeof("<>\" ")+sizeof(int)+strlen(noint_repeat_string)+1))==NULL)
     {
     fprintf(stderr, "ERROR\n");
     dce_fprintf(stderr,gc_t_memory_insufficient);
     noint_repeat_string=(char *)dce_sprintf(gc_t_memory_insufficient);
     gsu_listing(noint_repeat_string,NULL,NULL);
     exit(GSU_EXIT_53);
     }
free(noint_repeat_string);
while (flg == 1)
    {
    sprintf(strdsa, "%s<%d>> \"", string1, zaehler);
    if (((poi1=strstr(noint_repeat_ch, strdsa))!=NULL)&&(poi1[strlen(strdsa)]!='"'))
       {
       j=0;
       poi1=poi1+strlen(strdsa);
       while((*poi1 != '"')&&(j < maxim))
          {
          act->dsaname[j++]=*poi1;
          poi1++;
          };
       act->dsaname[j]='\0';
       }
    else
       {
       if (zaehler > 1)
          {
          flg = 0;
          act=anfang;
          j=1;
          while (j < (zaehler-1))
             {
             act=act->next;
             j++;
             };
          act->next=NULL;
          return(anfang);
          }
       else
          {
          anfang = NULL;
          return(anfang);
          }
       }
    noint_repeat_string=(char *)dce_sprintf(gsu_psap_answer_sams);
    if ((noint_repeat_string=realloc(noint_repeat_string,strlen(noint_repeat_string)+strlen(" ")+1)) == NULL)
         {
         noint_repeat_string=(char *)dce_sprintf(gc_t_memory_insufficient);
	 fprintf(stderr, "ERROR\n");
         dce_fprintf(stderr,gc_t_memory_insufficient);
         gsu_listing(noint_repeat_string, NULL, NULL);
         free(noint_repeat_string);
         exit(GSU_EXIT_54);
         }
    strcat(noint_repeat_string, " ");
    sprintf(strpsap, "%s<%d>> \"", noint_repeat_string, zaehler);
    free(noint_repeat_string);
    if ((poi=strstr(poi1, strpsap)) != NULL)
	{
	j=0;
	poi=poi+strlen(strpsap);
	while((*poi != '"')&&(*poi != '\n')&&(j < maxim))
	   {
	   act->psapadd[j++]=*poi;
	   poi++;
	   };
        act->psapadd[j]='\0';
	}
    else
	{
	fprintf(stderr, "ERROR\n");
	dce_fprintf(stderr, gsu_missing_psap_name, act->dsaname);
	noint_repeat_string=(char *)dce_sprintf(gsu_missing_psap_name,act->dsaname);
	gsu_listing(noint_repeat_string,NULL,NULL);
	exit(GSU_EXIT_55);
	}
    if ((act->next=(gsu_dsa *)malloc(sizeof(gsu_dsa)+1)) == NULL)
        {
	fprintf(stderr, "ERROR\n");
        dce_fprintf(stderr,gc_t_memory_insufficient);
	noint_repeat_string=(char *)dce_sprintf(gc_t_memory_insufficient);
	gsu_listing(noint_repeat_string,NULL,NULL);
        exit(GSU_EXIT_56);
        }
    act = act->next;
    zaehler++;
    };
return(NULL);
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_syncheck                                   */
/*  Description   :  Does dir-id exist ?                            */
/*  Author        :  Eva Wilde, BU BA KP13                          */
/*                                                                  */
/********************************************************************/
static unsigned16 gsu_syncheck(void)
{
unsigned16 flgdir;
struct stat *puff=NULL;
char * help=NULL;

if((help=(char *)malloc(strlen("gdssysadm -fc -m3 -D |grep '^ ' > /tmp/gsu_syncheck_ausgabe") + sizeof(int)+3)) == NULL)
   {
   help=(char *)dce_sprintf(gc_t_memory_insufficient);
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr,gc_t_memory_insufficient);
   gsu_listing(help, NULL, NULL);
   free(help);
   exit(GSU_EXIT_57);
   }
sprintf(help, "gdssysadm -fc -m3 -D |grep '^%d ' > /tmp/gsu_syncheck_ausgabe",gsu_dirid);
gsu_listing(help, NULL, NULL);
if (system(help) < 0)
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "%s\n", help);
   gsu_listing("system ERROR of ", help, NULL);
   exit(GSU_EXIT_58);
   }
free(help);
if ((puff = malloc (sizeof(struct stat) +1)) == NULL)
   {
   help=(char *)dce_sprintf(gc_t_memory_insufficient);
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr,gc_t_memory_insufficient);
   gsu_listing(help, NULL, NULL);
   free(help);
   exit(GSU_EXIT_59);
   }
if ((stat("/tmp/gsu_syncheck_ausgabe", puff)) == 0)
   {
   if (puff->st_size > 0)
      {
      help=(char *)dce_sprintf(gsu_dir_id_exist, gsu_dirid);
      dce_fprintf(stderr, gsu_dir_id_exist, gsu_dirid);
      gsu_listing(help, NULL, NULL);
      free(help);
      gsu_listing("gsu_dirid_exist()", NULL, NULL);
      gsu_dirid_exist();
      flgdir=1;
      }
   else
     {
     flgdir=0;
     }
   }  
else
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "stat of /tmp/gsu_syncheck_ausgabe\n");
   gsu_listing("stat ERROR of /tmp/gsu_syncheck_ausgabe", NULL, NULL);
   free(help);
   exit(GSU_EXIT_60);
   }
gsu_listing("rm /tmp/gsu_syncheck_ausgabe", NULL, NULL);
if (system("rm /tmp/gsu_syncheck_ausgabe") < 0)
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "rm /tmp/gsu_syncheck_ausgabe\n");
   gsu_listing("system ERROR of rm /tmp/gsu_syncheck_ausgabe",NULL, NULL);
   exit(GSU_EXIT_61);
   }
if (flgdir == 1)
    {
    return(1);
    }
else
    return(0);
}

/*************************************************************************/
/*                                                                       */
/*  File Name   :  gsu_setdir                                            */
/*  Description :  Create Dir-id                                         */
/*  Author      :  Eva Wilde, BU BA KP13                                 */
/*                                                                       */
/*************************************************************************/
static void gsu_setdir(void)
{
char * cmd=NULL;
signed16 i;

/*********   DEAKTIVATION    *********/
gsu_listing("gsu_deactivation()", NULL, NULL);
gsu_deactivation();

/*****    CONFIGURATION    *****/
if ((cmd=(char *)malloc(sizeof("gdssysadm ")+6*sizeof(int)+strlen("-d -s -C -u -c -fc -m1 -A")+1))==NULL)
     {
     fprintf(stderr, "ERROR\n");
     dce_fprintf(stderr,gc_t_memory_insufficient);
     cmd=(char *)dce_sprintf(gc_t_memory_insufficient);
     gsu_listing(cmd,NULL,NULL);
     exit(GSU_EXIT_62);
     }
strcpy(cmd,"gdssysadm -fc -m1 -c");
if ((gsu_config[0] == 'C')||(gsu_config[0] == 'c'))
     strcat(cmd,"1");
else
   {
   if ((gsu_config[0] == 'S')||(gsu_config[0] == 's'))
      {
      strcat(cmd,"2");
      }
   else
      {
      free(cmd);
      cmd=(char *)dce_sprintf(gsu_config_type_failed, "gsu_setdir");
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr, gsu_config_type_failed, "gsu_setdir");
      gsu_listing(cmd, NULL, NULL);
      free(cmd);
      exit(GSU_EXIT_64);
      }
   }
strcat(cmd," -d");
if (gsu_dirid <= GSU_ID)
   sprintf((cmd+strlen(cmd)),"%d",gsu_dirid);
else
   {
   free(cmd);
   cmd=(char *)dce_fprintf(stderr, gsu_max_dir_id, GSU_ID);
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gsu_max_dir_id, GSU_ID);
   gsu_listing(cmd, NULL, NULL);
   exit(GSU_EXIT_65);
   }
if (gsu_noser <= GSU_SERVER)
    {
    for (i=0; i<GSU_CONFIG; i++)
       gsu_config[i] = (char)toupper(gsu_config[i]);
    if (!strcmp(gsu_config, gsu_server_sys))
       {
       strcat(cmd," -s");
       sprintf((cmd+strlen(cmd)),"%d",gsu_noser);
       }
    }
else
    {
    free(cmd);
    cmd=(char *)dce_fprintf(stderr, gsu_max_servers, GSU_SERVER);
    fprintf(stderr, "ERROR\n");
    dce_fprintf(stderr, gsu_max_servers, GSU_SERVER);
    gsu_listing(cmd, NULL, NULL);
    exit(GSU_EXIT_66);
    }
if (gsu_nocl <= GSU_CLIENT)
   {
   if (gsu_nocl == 0)
      gsu_nocl=16;
   strcat(cmd," -C");
   sprintf((cmd+strlen(cmd)),"%d",gsu_nocl);
   }
else
   {
   free(cmd);
   cmd=(char *)dce_fprintf(stderr, gsu_max_clients, GSU_CLIENT);
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gsu_max_clients, GSU_CLIENT);
   gsu_listing(cmd, NULL, NULL);
   exit(GSU_EXIT_67);
   }
if ((gsu_config[0] == 'S')||(gsu_config[0] == 's'))
   {
   strcat(cmd," -A");
   if (!strncmp(gsu_e_secur,gsu_secur_dce, strlen(gsu_secur_dce)))
      {
      sprintf((cmd+strlen(cmd)),"%d",GSU_DCE_AUTH);
      }
   else
      {
      if (!strncmp(gsu_e_secur,gsu_secur_simple, strlen(gsu_secur_simple)))
         {
         sprintf((cmd+strlen(cmd)),"%d",GSU_SIMPLE);
         }
      else
         {
         if (!strncmp(gsu_e_secur,gsu_secur_simple_dce, strlen(gsu_secur_simple_dce)))
            {
            sprintf((cmd+strlen(cmd)),"%s",GSU_SIMPLE_DCE);
            }
	 else
	    {
            sprintf((cmd+strlen(cmd)),"%d",GSU_ANONYMOUS);
	    }
         }
      }
   }
gsu_listing(cmd, NULL, NULL);
if (system (cmd) < 0)
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "%s\n",cmd);
   gsu_listing(cmd, " ERROR", NULL);
   exit(GSU_EXIT_68);
   }
free(cmd);

/********   AKTIVATION      *********/
gsu_listing("gdssysadm -fA -p", NULL, NULL);
if (system ("gdssysadm -fA -p") < 0)
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "gdssysadm -fA -p\n");
   gsu_listing("system ERROR of gdssysadm -fA -p", NULL, NULL);
   exit(GSU_EXIT_69);
   }
}

/*************************************************************************/
/*                                                                       */
/*  File Name   :  gsu_inter_read                                        */
/*  Description :  read var[maxim]                                       */
/*  Input Value :  char * to variable name                               */
/*                 unsigned16 max. length of variable                    */
/*  Author      :  Eva Wilde, BU BA KP13                                 */
/*  Date        :  Oktober 1993                                          */
/*                                                                       */
/*************************************************************************/
static unsigned8 gsu_inter_read(char *var,   /*OUT - output variable*/
			   signed16 maxim)   /*IN - input variable*/
{
signed16 j=0;

while (((var[j]=(char)getchar()) != '\n')&&(j<(maxim-1)))
        j++;
if (j > 0)
   {
   var[j] = '\0';
   return(1);
   }
else
   {
   var[j] = '\0';
   return(0);
   }
}
 
/*************************************************************************/
/*                                                                       */
/*  File Name   :  gsu_help                                              */
/*  Description :  Help for gdssetup                                     */
/*  Author      :  Eva Wilde, BU BA KP13                                 */
/*  Date        :  Oktober 1993                                          */
/*                                                                       */
/*************************************************************************/
static void gsu_help(void)
{
dce_printf(gsu_help_text);
}

/*************************************************************************/
/*                                                                       */
/*  File Name   :  gsu_set_file                                          */
/*  Description :  Search name of file. In this file will be store all   */
/*                 input parameters and all executed operations.         */
/*  Input Value :  char * to output file                                 */
/*  Author      :  Eva Wilde, BU BA KP13                                 */
/*                                                                       */
/*************************************************************************/
static unsigned16 gsu_set_file(char * set_file_datei) /*IN - input value*/
{
struct stat * puff=NULL;
char * set_file_cmd=NULL;
unsigned16 flgdat;

if ((puff = malloc (sizeof(struct stat) +1)) == NULL)
      {
      set_file_cmd=(char *)dce_sprintf(gc_t_memory_insufficient);
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr,gc_t_memory_insufficient);
      gsu_listing(set_file_cmd, NULL, NULL);
      free(set_file_cmd);
      exit(GSU_EXIT_70);
      }
gsu_listing("stat(", set_file_datei, ")");
if ((stat(set_file_datei,puff)) == 0)   /*  set of filestate */
   {                                    /* if does it exist */
   if ((puff->st_size) >0)              /* is size > 0 */
      {
      if ((access(set_file_datei,W_OK)) == 0)
         flgdat = 1;
      else
         {
         set_file_cmd=(char *)dce_sprintf(gsu_access_right_failed,set_file_datei);
	 fprintf(stderr, "ERROR\n");
	 dce_fprintf(stderr,gsu_access_right_failed,set_file_datei);
	 gsu_listing(set_file_cmd, NULL, NULL);
	 free(set_file_cmd);
         exit(GSU_EXIT_71);
         }
       }
    else                                /* empty file */
       {
       if ((access(set_file_datei,W_OK)) == 0)
          flgdat = 0;
       else
          {
 	  set_file_cmd=(char *)dce_sprintf(gsu_access_right_failed,set_file_datei);
	  fprintf(stderr, "ERROR\n");
	  dce_fprintf(stderr,gsu_access_right_failed,set_file_datei);
	  gsu_listing(set_file_cmd, NULL, NULL);
	  free(set_file_cmd);
 	  exit(GSU_EXIT_72);
  	  }
       }
   return(flgdat);
   }
else                             /* file does not exist */
  {
  if ((set_file_cmd=malloc(sizeof("touch ")+strlen(set_file_datei)+1)) == NULL)
     {
     set_file_cmd=(char *)dce_sprintf(gc_t_memory_insufficient);
     fprintf(stderr, "ERROR\n");
     dce_fprintf(stderr,gc_t_memory_insufficient);
     gsu_listing(set_file_cmd, NULL, NULL);
     free(set_file_cmd);
     exit(GSU_EXIT_73);
     }
  strcpy(set_file_cmd,"touch ");
  strcat(set_file_cmd,set_file_datei);
  gsu_listing(set_file_cmd, NULL, NULL);
  if (system (set_file_cmd) < 0)
     {
     fprintf(stderr, "ERROR\n");
     fprintf(stderr, "%s\n", set_file_cmd);
     gsu_listing("system ERROR of ",set_file_cmd,NULL);
     exit(GSU_EXIT_74);
     }
  free(set_file_cmd);
  flgdat = 0;
  return(flgdat);
  }
}

/*************************************************************************/
/*                                                                       */
/*  File Name   :  gsu_inter_server_sys                                  */
/*  Description :  Read all input parameters in input mode for CL/SV     */
/*  Author      :  Eva Wilde, BU BA KP13                                 */
/*  Date        :  januar 1994                                           */
/*                                                                       */
/*************************************************************************/
static void gsu_inter_server_sys(void)
{
char inter_server_string[GSU_LENINT]="\0";
char *server_sys_str=NULL;
gsu_dsa * act=NULL;

	    /**********************************/
            /*****   CONFIGURATION DATA   *****/
	    /**********************************/
server_sys_str=(char *)dce_sprintf(gsu_nocl_answer_sams);
printf("%s",server_sys_str);
if (gsu_inter_read(inter_server_string,GSU_LENINT)==0)
   {
   /***   set default value   ***/
   gsu_nocl = 16;
   }
else
   {
   gsu_nocl=atoi(inter_server_string);
   }
gsu_listing("gsu_inter_read(", server_sys_str, ",...)");
gsu_inter_write(server_sys_str, NULL, gsu_nocl, 'i');
gsu_listing("gsu_inter_write(", server_sys_str, ",...)");
sprintf(inter_server_string, "%d", gsu_nocl);
gsu_listing("	gsu_nocl = ", inter_server_string, NULL);
free(server_sys_str);

server_sys_str=(char *)dce_sprintf(gsu_noser_answer_sams);
printf("%s",server_sys_str);
if (gsu_inter_read(inter_server_string,GSU_LENINT)==0)
   {
   /***   set default value   ***/
   gsu_noser=2;
   }
else
   {
   gsu_noser=atoi(inter_server_string);
   }
gsu_listing("gsu_inter_read(", server_sys_str, ",...)");
gsu_inter_write(server_sys_str, NULL, gsu_noser, 'i');
gsu_listing("gsu_inter_write(", server_sys_str, ",...)");
sprintf(inter_server_string, "%d", gsu_noser);
gsu_listing("	gsu_noser = ", inter_server_string, NULL);
free(server_sys_str);

server_sys_str=(char *)dce_sprintf(gsu_secur_answer_sams);
printf("%s",server_sys_str);
if (gsu_inter_read(gsu_e_secur,GSU_SECUR)==0)
   {
   /***   set default alue   ***/
   strcpy(gsu_e_secur, gsu_secur_dce);
   }
gsu_listing("gsu_inter_read(", server_sys_str, ",...)");
gsu_inter_write(server_sys_str, gsu_e_secur, 0, 'c');
gsu_listing("gsu_inter_write(", server_sys_str, ",...)");
gsu_listing("	gsu_secur = ", gsu_e_secur, NULL);
free(server_sys_str);
            /*****   END OF CONFIGURATION DATA   *****/

            /*****       CONFIGURATION       *****/
printf("CONFIGURATION of GDS ...\n");
gsu_listing("gsu_syncheck()", NULL, NULL);
if (gsu_syncheck() == 0)
    {
    gsu_listing("gsu_setdir()", NULL, NULL);
    gsu_setdir();
    }
            /*****    END OF CONFIGURATION     *****/

	    /************************************/
	    /*****    INITIALIZATION DATA   *****/
	    /************************************/
server_sys_str=(char *)dce_sprintf(gsu_cladd_answer_sams);
printf("%s",server_sys_str);
if (gsu_inter_read(gsu_cladd,GSU_PSAP)==0)
   {
   /***   set default value   ***/
   strcpy(gsu_cladd,gsu_cladd_default_value);
   }
gsu_listing("gsu_inter_read(", server_sys_str, ",...)");
gsu_inter_write(server_sys_str, gsu_cladd, 0, 'c');
gsu_listing("gsu_inter_write(", server_sys_str, ",...)");
gsu_listing("	gsu_cladd = ", gsu_cladd, NULL);
free(server_sys_str);

server_sys_str=(char *)dce_sprintf(gsu_locdsa_answer_sams);
printf("%s",server_sys_str);
if(gsu_inter_read(gsu_locdsa,GSU_DSA)==0)
   {
   free(server_sys_str);
   server_sys_str=(char *)dce_sprintf(gsu_missing_local_dsa_name);
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gsu_missing_local_dsa_name);
   gsu_listing(server_sys_str,NULL,NULL);
   free(server_sys_str);
   exit(GSU_EXIT_75);
   }
gsu_listing("gsu_inter_read(", server_sys_str, ",...)");
gsu_inter_write(server_sys_str, gsu_locdsa, 0, 'c');
gsu_listing("gsu_inter_write(", server_sys_str, ",...)");
gsu_listing("	gsu_locdsa = ", gsu_locdsa, NULL);
free(server_sys_str);

server_sys_str=(char *)dce_sprintf(gsu_psap_answer_sams);
if ((server_sys_str=realloc(server_sys_str,strlen(server_sys_str)+strlen("> ")+1)) == NULL)
   {
   server_sys_str=(char *)dce_sprintf(gc_t_memory_insufficient);
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr,gc_t_memory_insufficient);
   gsu_listing(server_sys_str, NULL, NULL);
   free(server_sys_str);
   exit(GSU_EXIT_76);
   }
strcat(server_sys_str, "> ");
printf("%s",server_sys_str);
if (gsu_inter_read(gsu_psaploc,GSU_PSAP)==0)
   {
   free(server_sys_str);
   server_sys_str=(char *)dce_sprintf(gsu_missing_psap_name, gsu_locdsa);
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gsu_missing_psap_name, gsu_locdsa);
   gsu_listing(server_sys_str,NULL,NULL);
   exit(GSU_EXIT_77);
   }
gsu_listing("gsu_inter_read(", server_sys_str, ",...)");
gsu_inter_write(server_sys_str, gsu_psaploc, 0, 'c');
gsu_listing("gsu_inter_write(", server_sys_str, ",...)");
gsu_listing("	gsu_psaploc = ", gsu_psaploc, NULL);
free(server_sys_str);

server_sys_str=(char *)dce_sprintf(gsu_contdsa_answer_sams);
printf("%s",server_sys_str);
gsu_inter_read(gsu_contdsa,GSU_DSA);
gsu_listing("gsu_inter_read(", server_sys_str, ",...)");
gsu_inter_write(server_sys_str, gsu_contdsa, 0, 'c');
gsu_listing("gsu_inter_write(", server_sys_str, ",...)");
gsu_listing("	gsu_contdsa = ", gsu_contdsa, NULL);
free(server_sys_str);

if (gsu_contdsa[0] != '\0')
   {
   server_sys_str=(char *)dce_sprintf(gsu_psap_answer_sams);
   if ((server_sys_str=realloc(server_sys_str,strlen(server_sys_str)+strlen("> ")+1)) == NULL)
      {
      server_sys_str=(char *)dce_sprintf(gc_t_memory_insufficient);
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr,gc_t_memory_insufficient);
      gsu_listing(server_sys_str, NULL, NULL);
      free(server_sys_str);
      exit(GSU_EXIT_78);
      }
   strcat(server_sys_str, "> ");
   printf("%s",server_sys_str);
   if (gsu_inter_read(gsu_psapcont,GSU_PSAP)==0)
      {
      free(server_sys_str);
      server_sys_str=(char *)dce_sprintf(gsu_missing_psap_name,gsu_contdsa);
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr, gsu_missing_psap_name, gsu_contdsa);
      gsu_listing(server_sys_str,NULL,NULL);
      free(server_sys_str);
      exit(GSU_EXIT_79);
      }
   gsu_listing("gsu_inter_read(", server_sys_str, ",...)");
   gsu_inter_write(server_sys_str, gsu_psapcont, 0, 'c');
   gsu_listing("gsu_inter_write(", server_sys_str, ",...)");
   gsu_listing("	gsu_psapcont = ", gsu_psapcont, NULL);
   free(server_sys_str);

   server_sys_str=(char *)dce_sprintf(gsu_defsch_answer_sams);
   printf("%s",server_sys_str);
   if (gsu_inter_read(gsu_defsch,GSU_DEFSCH)==0)
      {
      /***   set default value   ***/
      strcpy(gsu_defsch,gsu_answer_true);
      }
   gsu_listing("gsu_inter_read(", server_sys_str, ",...)");
   gsu_inter_write(server_sys_str, gsu_defsch, 0, 'c');
   gsu_listing("gsu_inter_write(", server_sys_str, ",...)");
   gsu_listing("	gsu_defsch = ", gsu_defsch, NULL);
   free(server_sys_str);

   server_sys_str=(char *)dce_sprintf(gsu_locals_answer_sams);
   printf("%s",server_sys_str);  
   if (gsu_inter_read(gsu_locals,GSU_LOCALS)==0)
      {
      /***   set default value   ***/
      strcpy(gsu_locals, gsu_answer_true);
      }
   gsu_listing("gsu_inter_read(", server_sys_str, ",...)");
   gsu_inter_write(server_sys_str, gsu_locals,0,'c');
   gsu_listing("gsu_inter_write(", server_sys_str, ",...)");
   gsu_listing("	gsu_locals = ", gsu_locals, NULL);
   free(server_sys_str);

    /*****   DSA administration   *****/
   server_sys_str=(char *)dce_sprintf(gsu_add_dsa_answer_sams);
   anfang1=gsu_repeat(server_sys_str, stream, 1);
   act = anfang1;
   gsu_listing("gsu_repeat(", server_sys_str, ",...)");
   while (act != NULL)
      {
      gsu_listing("	dsaname = ", act->dsaname, NULL);
      gsu_listing("	psap    = ", act->psapadd, NULL);
      act = act->next;
      };
   free(server_sys_str);

   server_sys_str=(char *)dce_sprintf(gsu_local_equal_default);
   printf("%s",server_sys_str);
   if (gsu_inter_read(gsu_loc_def,GSU_LOC_DEF)==0)
      {
      /***   set default value   ***/
      strcpy(gsu_loc_def, gsu_answer_true);
      }
   gsu_listing("gsu_inter_read(", server_sys_str, ",...)");
   gsu_inter_write(server_sys_str, gsu_loc_def, 0, 'c');
   gsu_listing("gsu_inter_write(", server_sys_str, ",...)");
   gsu_listing("	gsu_loc_def = ", gsu_loc_def, NULL);
   free(server_sys_str);

   server_sys_str=(char *)dce_sprintf(gsu_def_dsa_answer_sams);
   if (!strcmp(gsu_loc_def, gsu_answer_true))
      {
      if ((anfang2=(gsu_dsa *)malloc(sizeof(gsu_dsa)+1)) == NULL)
         {
	 free(server_sys_str);
         server_sys_str=(char *)dce_sprintf(gc_t_memory_insufficient);
	 fprintf(stderr, "ERROR\n");
	 dce_fprintf(stderr,gc_t_memory_insufficient);
	 gsu_listing(server_sys_str,NULL,NULL);
	 free(server_sys_str);
         exit(GSU_EXIT_80);
         }
      strcpy(anfang2->dsaname,gsu_locdsa);
      strcpy(anfang2->psapadd,gsu_psaploc);
      anfang2->next=gsu_repeat(server_sys_str, stream, 2);
      }
   else
      {
      anfang2=gsu_repeat(server_sys_str, stream, 1);
      }
   act = anfang2;
   gsu_listing("gsu_repeat(", server_sys_str, ",...)");
   while (act != NULL)
      {
      gsu_listing("	dsaname = ", act->dsaname, NULL);
      gsu_listing("	psap    = ", act->psapadd, NULL);
      act = act->next;
      };
   free(server_sys_str);
    /*****   end of DSA administration   *****/
   }
else
    {
    initmode=2;
    server_sys_str=(char *)dce_sprintf(gsu_locals_answer_sams);
    printf("%s",server_sys_str);  
    if (gsu_inter_read(gsu_locals,GSU_LOCALS)==0)
       {
       /***   set default value   ***/
       strcpy(gsu_locals, gsu_answer_true);
       }
    gsu_listing("gsu_inter_read(", server_sys_str, ",...)");
    if (strcmp(gsu_locals, gsu_answer_true))
       {
       free(server_sys_str);
       server_sys_str=(char *)dce_sprintf(gsu_local_schema_error);
       fprintf(stderr, "ERROR\n");
       dce_fprintf(stderr, gsu_local_schema_error);
       gsu_listing(server_sys_str,NULL,NULL);
       free(server_sys_str);
       exit(GSU_EXIT_81);
       }
    gsu_inter_write(server_sys_str, gsu_locals,0,'c');
    gsu_listing("gsu_inter_write(", server_sys_str, ",...)");
    gsu_listing("	gsu_locals = ", gsu_locals, NULL);
    free(server_sys_str);

    server_sys_str=(char *)dce_sprintf(gsu_defsch_answer_sams);
    printf("%s",server_sys_str);
    if (gsu_inter_read(gsu_defsch,GSU_DEFSCH)==0)
       {
       /***   set default value   ***/
       strcpy(gsu_defsch, gsu_answer_true);
       }
    gsu_listing("gsu_inter_read(", server_sys_str, ",...)");
    if (strcmp(gsu_defsch, gsu_answer_true))
       {
       free(server_sys_str);
       server_sys_str=(char *)dce_sprintf(gsu_default_schema_failed,"gsu_inter_server_sys");
       fprintf(stderr, "ERROR\n");
       dce_fprintf(stderr, gsu_default_schema_failed,"gsu_inter_server_sys");
       gsu_listing(server_sys_str,NULL,NULL);
       free(server_sys_str);
       exit(GSU_EXIT_82);
       }
    gsu_inter_write(server_sys_str, gsu_defsch, 0, 'c');
    gsu_listing("gsu_inter_write(", server_sys_str, ",...)");
    gsu_listing("	gsu_defsch = ", gsu_defsch, NULL);
    free(server_sys_str);

         /*****   DSA Administration   *****/
    server_sys_str=(char *)dce_sprintf(gsu_add_dsa_answer_sams);
    anfang1=gsu_repeat(server_sys_str, stream, 1);
    act = anfang1;
    gsu_listing("gsu_repeat(", server_sys_str, ",...)");
    while (act != NULL)
       {
       gsu_listing("	dsaname = ", act->dsaname, NULL);
       gsu_listing("	psap    = ", act->psapadd, NULL);
       act = act->next;
       };
    free(server_sys_str);

   server_sys_str=(char *)dce_sprintf(gsu_local_equal_default);
   printf("%s",server_sys_str);
   if (gsu_inter_read(gsu_loc_def,GSU_LOC_DEF)==0)
      {
      /***   set default value   ***/
      strcpy(gsu_loc_def, gsu_answer_true);
      }
   gsu_listing("gsu_inter_read(", server_sys_str, ",...)");
   gsu_inter_write(server_sys_str, gsu_loc_def, 0, 'c');
   gsu_listing("gsu_inter_write(", server_sys_str, ",...)");
   gsu_listing("	gsu_loc_def = ", gsu_loc_def, NULL);
   free(server_sys_str);

   server_sys_str=(char *)dce_sprintf(gsu_def_dsa_answer_sams);
   if (!strcmp(gsu_loc_def, gsu_answer_true))
      {
      if ((anfang2=(gsu_dsa *)malloc(sizeof(gsu_dsa)+1)) == NULL)
         {
	 free(server_sys_str);
         server_sys_str=(char *)dce_sprintf(gc_t_memory_insufficient);
	 fprintf(stderr, "ERROR\n");
	 dce_fprintf(stderr,gc_t_memory_insufficient);
	 gsu_listing(server_sys_str,NULL,NULL);
	 free(server_sys_str);
         exit(GSU_EXIT_128);
         }
      strcpy(anfang2->dsaname,gsu_locdsa);
      strcpy(anfang2->psapadd,gsu_psaploc);
      anfang2->next=gsu_repeat(server_sys_str, stream, 2);
      }
    else
       {
       anfang2=gsu_repeat(server_sys_str, stream, 1);
       }
    act = anfang2;
    gsu_listing("gsu_repeat(", server_sys_str, ",...)");
    while (act != NULL)
       {
       gsu_listing("	dsaname = ", act->dsaname, NULL);
       gsu_listing("	psap    = ", act->psapadd, NULL);
       act = act->next;
       };
    free(server_sys_str);
      /*****   end of DSA Administration   *****/
    }

if (isatty(0))
   {
   gsu_listing("stty -echo ", NULL, NULL);
   if (system ("stty -echo") < 0)
     {
     fprintf(stderr, "ERROR\n");
     fprintf(stderr, "stty -echo\n");
     gsu_listing("system ERROR of stty -echo",NULL,NULL);
     exit(GSU_EXIT_84);
     }
   }

server_sys_str=(char *)dce_sprintf(gsu_userpsw_answer_sams);
printf("%s",server_sys_str);
if ((gsu_inter_read(gsu_userpsw,GSU_USERPSW)==0)&&(!strcmp(gsu_e_secur,gsu_secur_simple)))
   {
   free(server_sys_str);
   server_sys_str=(char *)dce_sprintf(gc_t_missing_user_password);
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gc_t_missing_user_password);
   gsu_listing(server_sys_str,NULL,NULL);
   free(server_sys_str);
   exit(GSU_EXIT_85);
   }
gsu_listing("gsu_inter_read(", server_sys_str, ",...)");

if (isatty(0))
   {
   gsu_listing("stty echo ", NULL, NULL);
   if (system ("stty echo") < 0)
     {
     fprintf(stderr, "ERROR\n");
     fprintf(stderr, "stty echo\n");
     gsu_listing("system ERROR of stty echo",NULL,NULL);
     exit(GSU_EXIT_86);
     }
   printf("\n");
   }

gsu_inter_write(server_sys_str, gsu_userpsw, 0, 'c');
gsu_listing("gsu_inter_write(", server_sys_str, ",...)");
gsu_listing("	gsu_userpsw = ", gsu_userpsw, NULL);
free(server_sys_str);

if (!strcmp(gsu_e_secur,gsu_secur_dce))
  {
  server_sys_str=(char *)dce_sprintf(gsu_princ_answer_sams);
  printf("%s",server_sys_str);
  if (gsu_inter_read(gsu_princ,GSU_PRINC)==0)
     {
     free(server_sys_str);
     server_sys_str=(char *)dce_sprintf(gsu_missing_principal_name);
     fprintf(stderr, "ERROR\n");
     dce_fprintf(stderr, gsu_missing_principal_name);
     gsu_listing(server_sys_str,NULL,NULL);
     free(server_sys_str);
     exit(GSU_EXIT_87);
     }
  gsu_listing("gsu_inter_read(", server_sys_str, ",...)");
  gsu_inter_write(server_sys_str, gsu_princ, 0, 'c');
  gsu_listing("gsu_inter_write(", server_sys_str, ",...)");
  gsu_listing("	gsu_princ = ", gsu_princ, NULL);
  free(server_sys_str);
  }
  /*****   End of CACHE Administration   *****/

  /*****   Discrimination of initialisation modes   *****/
if (gsu_contdsa[0] != '\0')
   {
    /*****    Dom -> Def oder NoDef    *****/
    if (!strcmp(gsu_locals, gsu_answer_true))
       {
       if (!strcmp(gsu_defsch, gsu_answer_true))
          initmode = 3;
       else
          {
          server_sys_str=(char *)dce_sprintf(gsu_default_schema_failed,"gsu_inter_server_sys");
	  fprintf(stderr, "ERROR\n");
	  dce_fprintf(stderr,gsu_default_schema_failed,"gsu_inter_server_sys");
	  gsu_listing(server_sys_str,NULL,NULL);
	  free(server_sys_str);
          exit(GSU_EXIT_88);
          }
       }
    else
       {
       if (!strcmp(gsu_locals, gsu_answer_false))
          {
          if (!strcmp(gsu_defsch, gsu_answer_true))
              initmode = 4;
          else
             {
             if (!strcmp(gsu_defsch, gsu_answer_false))
	        initmode = 5;
   	     else
	        {
                server_sys_str=(char *)dce_sprintf(gsu_default_schema_failed,"gsu_inter_server_sys");
		fprintf(stderr, "ERROR\n");
		dce_fprintf(stderr,gsu_default_schema_failed,"gsu_inter_server_sys");
		gsu_listing(server_sys_str,NULL,NULL);
		free(server_sys_str);
                exit(GSU_EXIT_89);
	        }
	     }
          }
         else
          {
          server_sys_str=(char *)dce_sprintf(gsu_local_schema_error);
	  fprintf(stderr, "ERROR\n");
	  dce_fprintf(stderr, gsu_local_schema_error);
	  gsu_listing(server_sys_str,NULL,NULL);
	  free(server_sys_str);
          exit(GSU_EXIT_90);
          }
       }
   }
}

/*************************************************************************/
/*                                                                       */
/*  File Name   :  gsu_inter_client_sys                                  */
/*  Description :  Read all input parameters in input mode for CL system */
/*  Author      :  Eva Wilde, BU BA KP13                                 */
/*  Date        :  januar 1994                                           */
/*                                                                       */
/*************************************************************************/
static void gsu_inter_client_sys(void)
{
char * client_sys_str=NULL;
char inter_client_sys_string[GSU_LENINT]="\0";
gsu_dsa *act=NULL;

          /*****    INITIALIZATION DATA    *****/
client_sys_str=(char *)dce_sprintf(gsu_cladd_answer_sams);
printf("%s",client_sys_str);
if (gsu_inter_read(gsu_cladd,GSU_PSAP)==0)
   {
   /***   set default value   ***/
   strcpy(gsu_cladd,gsu_cladd_default_value);
   }
gsu_listing("gsu_inter_read(", client_sys_str, ",...)");
gsu_inter_write(client_sys_str, gsu_cladd, 0, 'c');
gsu_listing("gsu_inter_write(", client_sys_str, ",...)");
gsu_listing("	gsu_cladd = ", gsu_cladd, NULL);
free(client_sys_str);
          /*****    END OF INITIALIZATION DATA    *****/

          /*****   CONFIGURATION DATA     *****/
client_sys_str=(char *)dce_sprintf(gsu_nocl_answer_sams);
printf("%s",client_sys_str);
if (gsu_inter_read(inter_client_sys_string, GSU_LENINT)==0)
   {
   /***   set default value   ***/
   gsu_nocl = 16;
   }
else
   {
   gsu_nocl=atoi(inter_client_sys_string);
   }
gsu_listing("gsu_inter_read(", client_sys_str, ",...)");
gsu_inter_write(client_sys_str, NULL, gsu_nocl, 'i');
gsu_listing("gsu_inter_write(", client_sys_str, ",...)");
sprintf(inter_client_sys_string, "%d", gsu_nocl);
gsu_listing("	gsu_nocl= ", inter_client_sys_string, NULL);
free(client_sys_str);
          /*****  END OF CONFIGURATION DATA     *****/

/*****       CONFIGURATION       *****/
fprintf(stderr, "CONFIGURATION of GDS ...\n");
gsu_listing("gsu_syncheck()", NULL, NULL);
if (gsu_syncheck() == 0)
    {
    gsu_listing("gsu_setdir()", NULL, NULL);
    gsu_setdir();
    }
/*****  END OF CONFIGURATION     *****/

          /*****     ADDITIONAL ADMINISTRATION DATA    *****/
client_sys_str=(char *)dce_sprintf(gsu_add_dsa_answer_sams);
anfang1=gsu_repeat(client_sys_str, stream, 1);
act = anfang1;
gsu_listing("gsu_repeat(", client_sys_str, ",...)");
while (act != NULL)
  {
  gsu_listing("	dsaname = ", act->dsaname, NULL);
  gsu_listing("	psap    = ", act->psapadd, NULL);
  act = act->next;
  };
free(client_sys_str);

client_sys_str=(char *)dce_sprintf(gsu_def_dsa_answer_sams);
anfang2=gsu_repeat(client_sys_str, stream, 1);
act = anfang2;
gsu_listing("gsu_repeat(", client_sys_str, ",...)");
while (act != NULL)
  {
  gsu_listing("	dsaname = ", act->dsaname, NULL);
  gsu_listing("	psap    = ", act->psapadd, NULL);
  act = act->next;
  };
free(client_sys_str);
          /*****  END OF ADDITIONAL ADMINISTRATION DATA    *****/

initmode = 1;
}

/*************************************************************************/
/*                                                                       */
/*  File Name   :  gsu_inter_readline_admin                              */
/*  Description :  If you call gdssetup second and ...-times, you can    */
/*                 edit your old inputs parameter (it was store in       */
/*                 dcelocal_path/GSU_DIR/GSU_FILE)                       */
/*  Input Value :  char * to parameter file                              */
/*  Author      :  Eva Wilde, BU BA KP13                                 */
/*  Date        :  januar 1994                                           */
/*                                                                       */
/*************************************************************************/
static void gsu_inter_readline_admin(char * readline_admin_file) /*IN-OUT*/
{
signed16 i;
unsigned16 inter_admin_anzbl;
unsigned16 inter_admin_anzbyte;
unsigned16 inter_admin_hilf;
char readline_admin_integer_var[GSU_LENINT]="\0";
char * ch=NULL;
char * readline_admin_help_str=NULL;
gsu_dsa * act=NULL;

/***   fopen file to read   ***/
gsu_listing("fopen ", readline_admin_file, " for read");
if ((stream=fopen(readline_admin_file, "r")) == NULL)
   {
   readline_admin_help_str=(char *)dce_sprintf(gsu_fopen_error,readline_admin_file);
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr,gsu_fopen_error,readline_admin_file);
   gsu_listing(readline_admin_help_str,NULL,NULL);
   free(readline_admin_help_str);
   exit(GSU_EXIT_91);
   }
inter_admin_anzbl=0;
inter_admin_anzbyte=GSU_BLSIZE;
ch = NULL;

/***  read all date from file in store   ***/
while (inter_admin_anzbyte == GSU_BLSIZE)
   {
   if (ch == NULL)
      {
      if ((ch = (char *)malloc (GSU_BLSIZE+1))==NULL)
         {
         readline_admin_help_str=(char *)dce_sprintf(gc_t_memory_insufficient);
	 fprintf(stderr, "ERROR\n");
	 dce_fprintf(stderr,gc_t_memory_insufficient);
	 gsu_listing(readline_admin_help_str,NULL,NULL);
	 free(readline_admin_help_str);
         exit(GSU_EXIT_92);
         }
      }
   else
      {
      if ((ch=(char *)realloc(ch,(1+inter_admin_anzbl)*GSU_BLSIZE+1))==NULL)
         {
         readline_admin_help_str=(char *)dce_sprintf(gc_t_memory_insufficient);
	 fprintf(stderr, "ERROR\n");
	 dce_fprintf(stderr,gc_t_memory_insufficient);
	 gsu_listing(readline_admin_help_str,NULL,NULL);
	 free(readline_admin_help_str);
         exit(GSU_EXIT_93);
         }
      }
   inter_admin_anzbyte=fread(&ch[inter_admin_anzbl*GSU_BLSIZE],sizeof(char),
    		        GSU_BLSIZE, stream);
   inter_admin_anzbl++;
   };
ch[inter_admin_anzbl*GSU_BLSIZE] = '\0';

/***   close file    ***/
gsu_listing("fclose ", readline_admin_file, NULL);
if (fclose(stream) != 0)
   {
   readline_admin_help_str=(char *)dce_sprintf(gsu_fclose_error,readline_admin_file);
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr,gsu_fclose_error,readline_admin_file);
   gsu_listing(readline_admin_help_str,NULL,NULL);
   free(readline_admin_help_str);
   exit(GSU_EXIT_94);
   }

/***   open file to write   ***/
gsu_listing("fopen ", readline_admin_file, " for write");
if ((stream=fopen(readline_admin_file, "w")) == NULL)
   {
   readline_admin_help_str=(char *)dce_sprintf(gsu_fopen_error,readline_admin_file);
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr,gsu_fopen_error,readline_admin_file);
   gsu_listing(readline_admin_help_str,NULL,NULL);
   free(readline_admin_help_str);
   exit(GSU_EXIT_95);
   }

   /*************  Calls of readline()  ******************/
            /*****   CONFIGURATION DATA  *****/
readline_admin_help_str=(char *)dce_sprintf(gsu_dirid_answer_sams);
strcpy(gsu_line, readline_admin_help_str);
free(readline_admin_help_str);
if (gsu_readline((char *)NULL, &gsu_dirid, 'i', ch) == 0)
   {
   /***   set default value   ***/
   gsu_dirid = 1;
   }
gsu_listing("gsu_readline(", gsu_line, ",...)");
gsu_inter_write(gsu_line, NULL, gsu_dirid, 'i');
gsu_listing("gsu_inter_write(", gsu_line, ",...)");
sprintf(readline_admin_integer_var, "%d", gsu_dirid);
gsu_listing("	gsu_dirid = ", readline_admin_integer_var, NULL);

readline_admin_help_str=(char *)dce_sprintf(gsu_config_answer_sams);
strcpy(gsu_line, readline_admin_help_str);
free(readline_admin_help_str);
if (gsu_readline(gsu_config, NULL, 'c', ch) == 0)
   {
   /***   set default value   ***/
   strcpy(gsu_config, gsu_client_sys);
   }
gsu_listing("gsu_readline(", gsu_line, ",...)");
gsu_inter_write(gsu_line, gsu_config, 0, 'c');
gsu_listing("gsu_inter_write(", gsu_line, ",...)");
gsu_listing("	gsu_config = ", gsu_config, NULL);
for (i=0; i<GSU_CONFIG; i++)
   gsu_config[i] = (char)toupper(gsu_config[i]);

if (!strcmp(gsu_config, gsu_client_sys))
    {
    /*************************/
    /***   CLIENT SYSTEM   ***/
    /*************************/
    initmode=1;

    readline_admin_help_str=(char *)dce_sprintf(gsu_nocl_answer_sams);
    strcpy(gsu_line, readline_admin_help_str);
    free(readline_admin_help_str);
    if (gsu_readline((char *)NULL, &gsu_nocl, 'i', ch) == 0)
        {
	/***   set default value   ***/
	gsu_nocl = 16;
        }
    gsu_listing("gsu_readline(", gsu_line, ",...)");
    gsu_inter_write(gsu_line, NULL, gsu_nocl, 'i');
    gsu_listing("gsu_inter_write(", gsu_line, ",...)");
    sprintf(readline_admin_integer_var, "%d", gsu_nocl);
    gsu_listing("	gsu_nocl= ", readline_admin_integer_var, NULL);
    	   /***   END OF CONFIGURATION DATA   ***/

    		/***   CONFIGURATION   ***/ 
    fprintf(stderr, "CONFIGURATION of GDS ...\n");
    gsu_listing("gsu_syncheck()", NULL, NULL);
    if (gsu_syncheck() == 0)
	{
	gsu_listing("gsu_setdir()", NULL, NULL);
        gsu_setdir();
	}
    		/***  End of CONFIGURATION   ***/ 

	    /***   INITIALIZATION DATA   ***/
    readline_admin_help_str=(char *)dce_sprintf(gsu_cladd_answer_sams);
    strcpy(gsu_line, readline_admin_help_str);
    free(readline_admin_help_str);
    if (gsu_readline(gsu_cladd, NULL, 'c', ch) == 0)
        {
	/***   set default value   ***/
	strcpy(gsu_cladd,gsu_cladd_default_value);
        }
    gsu_listing("gsu_readline(", gsu_line, ",...)");
    gsu_inter_write(gsu_line, gsu_cladd, 0, 'c');
    gsu_listing("gsu_inter_write(", gsu_line, ",...)");
    gsu_listing("	gsu_cladd = ", gsu_cladd, NULL);

    readline_admin_help_str=(char *)dce_sprintf(gsu_add_dsa_answer_sams);
    anfang1 = gsu_noint_repeat(readline_admin_help_str,GSU_DSA, ch);
    if (anfang1 != NULL)
        gsu_readline_repeat(anfang1, readline_admin_help_str);
    else
        anfang1=gsu_repeat(readline_admin_help_str, stream, 1);
    act = anfang1;
    gsu_listing(readline_admin_help_str, NULL, NULL);
    while (act != NULL)
       {
       gsu_listing("	dsaname = ", act->dsaname, NULL);
       gsu_listing("	psap    = ", act->psapadd, NULL);
       act = act->next;
       };
    free(readline_admin_help_str);

    readline_admin_help_str=(char *)dce_sprintf(gsu_def_dsa_answer_sams);
    anfang2 = gsu_noint_repeat(readline_admin_help_str,GSU_DSA, ch);
    if (anfang2 != NULL)
        gsu_readline_repeat(anfang2, readline_admin_help_str);
    else
        anfang2=gsu_repeat(readline_admin_help_str, stream, 1);
    act = anfang2;
    gsu_listing(readline_admin_help_str, NULL, NULL);
    while (act != NULL)
       {
       gsu_listing("	dsaname = ", act->dsaname, NULL);
       gsu_listing("	psap    = ", act->psapadd, NULL);
       act = act->next;
       };
    free(readline_admin_help_str);
	   /***   END OF INITIALISATION DATA   ***/

    /***   close file   ***/
    gsu_listing("fclose ", readline_admin_file, NULL); 
    if (fclose(stream) != 0)
       {
       readline_admin_help_str=(char *)dce_sprintf(gsu_fclose_error,readline_admin_file);
       fprintf(stderr, "ERROR\n");
       dce_fprintf(stderr,gsu_fclose_error,readline_admin_file);
       gsu_listing(readline_admin_help_str,NULL,NULL);
       free(readline_admin_help_str);
       exit(GSU_EXIT_96);
       }
           /********************************/
           /***   END OF CLIENT SYSTEM   ***/
           /********************************/
    }
else
   {             /*   gsu_ config != c  */
   for (i=0; i<GSU_CONFIG; i++)
      gsu_config[i] = (char)toupper(gsu_config[i]);
   if (!strcmp(gsu_config, gsu_server_sys))
      {
      /********************************/
      /***   CLIENT/SERVER SYSTEM   ***/
      /********************************/
      readline_admin_help_str=(char *)dce_sprintf(gsu_nocl_answer_sams);
      strcpy(gsu_line, readline_admin_help_str);
      free(readline_admin_help_str);
      if (gsu_readline((char *)NULL, &gsu_nocl, 'i', ch) == 0)
	  {
	  /***   set default value   ***/
          gsu_nocl=16;
	  }
      gsu_listing("gsu_readline(", gsu_line, ",...)");
      gsu_inter_write(gsu_line, NULL, gsu_nocl, 'i');
      gsu_listing("gsu_inter_write(", gsu_line, ",...)");
      sprintf(readline_admin_integer_var, "%d", gsu_nocl);
      gsu_listing("	gsu_nocl = ", readline_admin_integer_var, NULL);

      readline_admin_help_str=(char *)dce_sprintf(gsu_noser_answer_sams);
      strcpy(gsu_line, readline_admin_help_str);
      free(readline_admin_help_str);
      if ((gsu_readline((char *)NULL, &gsu_noser, 'i', ch)) == 0)
	  {
	  /***   set default value   ***/
          gsu_noser=2;
	  }
      gsu_listing("gsu_readline(", gsu_line, ",...)");
      gsu_inter_write(gsu_line, NULL, gsu_noser, 'i');
      gsu_listing("gsu_inter_write(", gsu_line, ",...)");
      sprintf(readline_admin_integer_var, "%d", gsu_noser);
      gsu_listing("	gsu_noser = ", readline_admin_integer_var, NULL);

      readline_admin_help_str=(char *)dce_sprintf(gsu_secur_answer_sams);
      strcpy(gsu_line, readline_admin_help_str);
      free(readline_admin_help_str);
      if (gsu_readline(gsu_e_secur, NULL, 'c', ch) == 0)
	 {
	 /***   set default value   ***/
	 strcpy(gsu_e_secur, gsu_secur_dce);
	 }
      gsu_listing("gsu_readline(", gsu_line, ",...)");
      gsu_inter_write(gsu_line, gsu_e_secur, 0, 'c');
      gsu_listing("gsu_inter_write(", gsu_line, ",...)");
      gsu_listing("	gsu_secur = ", gsu_e_secur, NULL);
	   /***   END OF CONFIGURATION DATA   ***/

		  /***   CONFIGURATION   ***/
      fprintf(stderr, "CONFIGURATION of GDS ...\n");
      gsu_listing("gsu_syncheck()", NULL, NULL);
      if (gsu_syncheck() == 0)
	  {
	  gsu_listing("gsu_setdir()", NULL, NULL);
          gsu_setdir();
	  }
	      /***  END OF CONFIGURATION   ***/

	   /***   INITIALIZATION DATA   ***/
      readline_admin_help_str=(char *)dce_sprintf(gsu_cladd_answer_sams);
      strcpy(gsu_line, readline_admin_help_str);
      free(readline_admin_help_str);
      if (gsu_readline(gsu_cladd, NULL, 'c', ch) == 0)
          {
	  /***   set default value   ***/
	  strcpy(gsu_cladd, gsu_cladd_default_value);
          }
      gsu_listing("gsu_readline(", gsu_line, ",...)");
      gsu_inter_write(gsu_line, gsu_cladd, 0, 'c');
      gsu_listing("gsu_inter_write(", gsu_line, ",...)");
      gsu_listing("	gsu_cladd = ", gsu_cladd, NULL);

      readline_admin_help_str=(char *)dce_sprintf(gsu_locdsa_answer_sams);
      strcpy(gsu_line, readline_admin_help_str);
      free(readline_admin_help_str);
      if (gsu_readline_dsas(gsu_locdsa, gsu_psaploc, ch) == 0)
         {
         readline_admin_help_str=(char *)dce_sprintf(gsu_missing_local_dsa_name);
	 fprintf(stderr, "ERROR\n");
	 dce_fprintf(stderr,gsu_missing_local_dsa_name);
	 gsu_listing(readline_admin_help_str,NULL,NULL);
	 free(readline_admin_help_str);
         exit(GSU_EXIT_97);
         }
      gsu_listing("gsu_readline(", gsu_line, ",...)");
      gsu_inter_write(gsu_line, gsu_locdsa, 0, 'c');
      gsu_listing("gsu_inter_write(", gsu_line, ",...)");
      gsu_listing("	gsu_locdsa = ", gsu_locdsa, NULL);
      gsu_listing("	gsu_psaploc = ", gsu_psaploc, NULL);

      readline_admin_help_str=(char *)dce_sprintf(gsu_contdsa_answer_sams);
      strcpy(gsu_line, readline_admin_help_str);
      free(readline_admin_help_str);
      inter_admin_hilf=gsu_readline_dsas(gsu_contdsa, gsu_psapcont, ch);
      gsu_listing("gsu_readline_dsas(", gsu_line, ",...)");
      gsu_listing("	gsu_contdsa = ", gsu_contdsa, NULL);
      gsu_listing("	gsu_psapcont = ", gsu_psapcont, NULL);

      readline_admin_help_str=(char *)dce_sprintf(gsu_defsch_answer_sams);
      strcpy(gsu_line, readline_admin_help_str);
      free(readline_admin_help_str);
      if (gsu_readline(gsu_defsch, NULL, 'c', ch) == 0)
	 {
	 /***   set default value   ***/
	 strcpy(gsu_defsch, gsu_answer_true);
	 }
      gsu_listing("gsu_readline(", gsu_line, ",...)");
      gsu_inter_write(gsu_line, gsu_defsch, 0, 'c');
      gsu_listing("gsu_inter_write(", gsu_line, ",...)");
      gsu_listing("	gsu_defsch = ", gsu_defsch, NULL);

      readline_admin_help_str=(char *)dce_sprintf(gsu_local_equal_default);
      strcpy(gsu_line, readline_admin_help_str);
      free(readline_admin_help_str);
      if (gsu_readline(gsu_loc_def, 0, 'c', ch) == 0)
	 {
	 /***   set default value   ***/
	 strcpy(gsu_loc_def, gsu_answer_true);
	 }
      gsu_listing("gsu_readline(", gsu_line, ",...)");
      gsu_inter_write(gsu_line, gsu_loc_def, 0, 'c');
      gsu_listing("gsu_inter_write(", gsu_line, ",...)");
      gsu_listing("	gsu_loc_def = ", gsu_loc_def, NULL);

      readline_admin_help_str=(char *)dce_sprintf(gsu_def_dsa_answer_sams);
      anfang2 = gsu_noint_repeat(readline_admin_help_str,GSU_DSA, ch);
      if (anfang2 != NULL)
         {
	 if (!strcmp(gsu_loc_def, gsu_answer_true))
	    {
	    if ((act=(gsu_dsa *)malloc(sizeof(gsu_dsa)+1))==NULL)
	       {
	       readline_admin_help_str=(char *)dce_sprintf(gc_t_memory_insufficient);
	       fprintf(stderr, "ERROR\n");
	       dce_fprintf(stderr,gc_t_memory_insufficient);
	       gsu_listing(readline_admin_help_str,NULL,NULL);
	       free(readline_admin_help_str);
	       exit(GSU_EXIT_98);
	       }
	    strcpy(act->dsaname,gsu_locdsa);
	    strcpy(act->psapadd,gsu_psaploc);
	    act->next = anfang2;
	    anfang2 = act;
	    }
         gsu_readline_repeat(anfang2, readline_admin_help_str);
	 }
      else
	 {
	 if (!strcmp(gsu_loc_def, gsu_answer_true))
	    {
	    if ((anfang2=(gsu_dsa *)malloc(sizeof(gsu_dsa)+1))==NULL)
	       {
	       readline_admin_help_str=(char *)dce_sprintf(gc_t_memory_insufficient);
	       fprintf(stderr, "ERROR\n");
	       dce_fprintf(stderr,gc_t_memory_insufficient);
	       gsu_listing(readline_admin_help_str,NULL,NULL);
	       free(readline_admin_help_str);
	       exit(GSU_EXIT_99);
	       }
	    strcpy(anfang2->dsaname,gsu_locdsa);
	    strcpy(anfang2->psapadd,gsu_psaploc);
            anfang2->next=gsu_repeat(readline_admin_help_str, stream, 2);
	    }
         else
	    {
            anfang2=gsu_repeat(readline_admin_help_str, stream, 1);
	    }
	 }
      act = anfang2;
      gsu_listing(readline_admin_help_str, NULL, NULL);
      while (act != NULL)
	 {
	 gsu_listing("	dsaname = ", act->dsaname, NULL);
	 gsu_listing("	psap = ", act->psapadd, NULL);
	 act = act->next;
	 };
      free(readline_admin_help_str);

      if (isatty(0))
	 {
         gsu_listing("stty -echo", NULL, NULL);
         if (system ("stty -echo") < 0)
            {
	    fprintf(stderr, "ERROR\n");
            fprintf(stderr,"stty -echo\n");
	    gsu_listing("system ERROR of stty -echo",NULL,NULL);
            exit(GSU_EXIT_100);
            }
	 }
      readline_admin_help_str=(char *)dce_sprintf(gsu_userpsw_answer_sams);
      printf("%s", readline_admin_help_str);
      if ((gsu_inter_read(gsu_userpsw,GSU_USERPSW) == 0)&&(!strcmp(gsu_e_secur, gsu_secur_simple)))
         {
	 free(readline_admin_help_str);
         readline_admin_help_str=(char *)dce_sprintf(gc_t_missing_user_password);
	 fprintf(stderr, "ERROR\n"),
	 dce_fprintf(stderr, gc_t_missing_user_password);
	 gsu_listing(readline_admin_help_str, NULL, NULL);
	 free(readline_admin_help_str);
         exit(GSU_EXIT_101);
         }
      if (isatty(0))
	 {
         gsu_listing("gsu_inter_read(", readline_admin_help_str, ",...)");
         gsu_listing("stty echo ", NULL, NULL);
         if (system ("stty echo") < 0)
           {
	   fprintf(stderr, "ERROR\n"),
           fprintf(stderr,"stty echo\n");
	   gsu_listing("system ERROR of stty echo",NULL,NULL);
           exit(GSU_EXIT_102);
           }
         printf("\n");
	 }
      gsu_inter_write(readline_admin_help_str, gsu_userpsw, 0, 'c');
      gsu_listing("gsu_inter_write(", readline_admin_help_str, ",...)");
      gsu_listing("	gsu_userpsw = ", gsu_userpsw, NULL);
      free(readline_admin_help_str);

      if (!strcmp(gsu_e_secur, gsu_secur_dce))
         {
         readline_admin_help_str=(char *)dce_sprintf(gsu_princ_answer_sams);
         strcpy(gsu_line, readline_admin_help_str);
         free(readline_admin_help_str);
         if (gsu_readline(gsu_princ, NULL, 'c', ch) == 0)
            {
            readline_admin_help_str=(char *)dce_sprintf(gsu_missing_principal_name);
	    fprintf(stderr, "ERROR\n");
	    dce_fprintf(stderr,gsu_missing_principal_name);
	    gsu_listing(readline_admin_help_str,NULL,NULL);
	    free(readline_admin_help_str);
            exit(GSU_EXIT_103);
            }
         gsu_listing("gsu_readline(", gsu_line, ",...)");
         gsu_inter_write(gsu_line, gsu_princ, 0, 'c');
         gsu_listing("gsu_inter_write(", readline_admin_help_str, ",...)");
         gsu_listing("	gsu_princ = ", gsu_princ, NULL);
         }
	   /***   END OF INITIALIZATION DATA   ***/

           /***  ADDITIONAL ADMINISTRATION DATA   ***/
      readline_admin_help_str=(char *)dce_sprintf(gsu_add_dsa_answer_sams);
      anfang1 = gsu_noint_repeat(readline_admin_help_str,GSU_DSA, ch);
      if (anfang1 != NULL)
          gsu_readline_repeat(anfang1, readline_admin_help_str);
      else
          anfang1=gsu_repeat(readline_admin_help_str, stream, 1);
      act = anfang1;
      gsu_listing(readline_admin_help_str, NULL, NULL);
      while (act != NULL)
	 {
	 gsu_listing("	dsaname = ", act->dsaname, NULL);
	 gsu_listing("	psap = ", act->psapadd, NULL);
	 act = act->next;
	 };
      free(readline_admin_help_str);

      readline_admin_help_str=(char *)dce_sprintf(gsu_locals_answer_sams);
      strcpy(gsu_line, readline_admin_help_str);
      free(readline_admin_help_str);
      if (gsu_readline(gsu_locals, NULL, 'c', ch) == 0)
	 {
	 /***   set default value   ***/
         strcpy(gsu_locals, gsu_answer_true);
	 }
      gsu_listing("gsu_readline(", gsu_line, ",...)");
      gsu_inter_write(gsu_line, gsu_locals, 0, 'c');
      gsu_listing("gsu_inter_write(", gsu_line, ",...)");
      gsu_listing("	gsu_locals = ", gsu_locals, NULL);
           /***  END OF ADDITIONAL ADMINISTRATION DATA   ***/

	/*****   Discrimination of init-modes   *****/
       if (inter_admin_hilf == 0)
          {
          if (!strcmp(gsu_locals, gsu_answer_true))
            {
            if (!strcmp(gsu_defsch, gsu_answer_true))
	       initmode = 2;
            else
	       {
	       readline_admin_help_str=(char *)dce_sprintf(gsu_default_schema_failed,"gsu_inter_readline_admin");
	       fprintf(stderr, "ERROR\n");
	       dce_fprintf(stderr,gsu_default_schema_failed,"gsu_inter_readline_admin");
	       gsu_listing(readline_admin_help_str,NULL,NULL);
	       free(readline_admin_help_str);
	       exit(GSU_EXIT_104);
	       }
            }
          else
            {
            readline_admin_help_str=(char *)dce_sprintf(gsu_local_schema_error,"gsu_inter_readline_admin");
	    fprintf(stderr, "ERROR\n");
	    dce_fprintf(stderr,gsu_local_schema_error,"gsu_inter_readline_admin");
	    gsu_listing(readline_admin_help_str,NULL,NULL);
	    free(readline_admin_help_str);
            exit(GSU_EXIT_105);
            }
         }
       else
          /***   if Contact-DSA != NULL   ***/
          {
          if (!strcmp(gsu_locals, gsu_answer_true))
            {
            if (!strcmp(gsu_defsch, gsu_answer_true))
	       initmode = 3;
            else
	       {
	       readline_admin_help_str=(char *)dce_sprintf(gsu_default_schema_failed,"gsu_inter_readline_admin");
	       fprintf(stderr, "ERROR\n");
	       dce_fprintf(stderr,gsu_default_schema_failed,"gsu_inter_readline_admin");
	       gsu_listing(readline_admin_help_str,NULL,NULL);
	       free(readline_admin_help_str);
	       exit(GSU_EXIT_106);
	       }
            }
          else
            {
            if (!strcmp(gsu_locals,gsu_answer_false))
               {
               if (!strcmp(gsu_defsch, gsu_answer_true))
	          initmode = 4;
	       else
	          {
	          if (!strcmp(gsu_defsch, gsu_answer_false))
		      initmode = 5;
	          else
		      {
		      readline_admin_help_str=(char *)dce_sprintf(gsu_default_schema_failed,"gsu_inter_readline_admin");
		      fprintf(stderr, "ERROR\n");
		      dce_fprintf(stderr,gsu_default_schema_failed,"gsu_inter_readline_admin");
		      gsu_listing(readline_admin_help_str,NULL,NULL);
		      free(readline_admin_help_str);
		      exit(GSU_EXIT_107);
		      }
	          }
               }
             else
	       {
	       readline_admin_help_str=(char *)dce_sprintf(gsu_local_schema_error,"gsu_inter_readline_admin");
	       fprintf(stderr, "ERROR\n");
	       dce_fprintf(stderr,gsu_local_schema_error,"gsu_inter_readline_admin");
	       gsu_listing(readline_admin_help_str,NULL,NULL);
	       free(readline_admin_help_str);
	       exit(GSU_EXIT_108);
	       }
             }
           }
         /*****   End of Discrimination of init-modes   *****/
               /***************************************/
               /***   END OF CLIENT/SERVER SYSTEM   ***/
               /***************************************/
      }
   else
      {
      readline_admin_help_str=(char *)dce_sprintf(gsu_config_type_failed,"gsu_inter_readline_admin");
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr,gsu_config_type_failed,"gsu_inter_readline_admin");
      gsu_listing(readline_admin_help_str,NULL,NULL);
      free(readline_admin_help_str);
      exit(GSU_EXIT_109);
      }
   }   /*   end of gsu_config != c  */
}

/*************************************************************************/
/*                                                                       */
/*  File Name   :  gsu_listing                                           */
/*  Description :  Write listing file                                    */
/*  Input Value :  char * to input string for listing file               */
/*                 char * to input string for listing file               */
/*                 char * to input string for listing file               */
/*  Author      :  Eva Wilde, BU BA KP13                                 */
/*  Date        :  januar 1994                                           */
/*                                                                       */
/*************************************************************************/
void gsu_listing(char *listing_string1,  /*IN - input variable */
		 char *listing_string2,  /*IN - input variable */
		 char *listing_string3)  /*IN - input variable */
{
char * list_poi=NULL;

if (listflg == 1)
   {
   while(fprintf(streamlist,"%s",listing_string1) < 0)
      {
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr,gsu_inter_write_error, gsu_listfile);
      list_poi=(char *)dce_sprintf(gsu_inter_write_error,gsu_listfile);
      gsu_listing(list_poi,NULL,NULL);
      exit(GSU_EXIT_110);
      };
   fflush(streamlist);
   if (listing_string2 != NULL)
      {
      while (fprintf(streamlist,"%s",listing_string2) < 0)
         {
	 fprintf(stderr, "ERROR\n");
         dce_fprintf(stderr,gsu_inter_write_error, gsu_listfile);
         list_poi=(char *)dce_sprintf(gsu_inter_write_error,gsu_listfile);
         gsu_listing(list_poi,NULL,NULL);
         exit(GSU_EXIT_111);
         };
      fflush(streamlist);
      if (listing_string3 != NULL)
	 {
         while (fprintf(streamlist,"%s",listing_string3) < 0)
            {
	    fprintf(stderr, "ERROR\n");
            dce_fprintf(stderr,gsu_inter_write_error, gsu_listfile);
            list_poi=(char *)dce_sprintf(gsu_inter_write_error,gsu_listfile);
            gsu_listing(list_poi,NULL,NULL);
            exit(GSU_EXIT_112);
            };
         fflush(streamlist);
	 }
      }
   while (fprintf(streamlist,"\n") < 0)
      {
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr,gsu_inter_write_error, gsu_listfile);
      list_poi=(char *)dce_sprintf(gsu_inter_write_error,gsu_listfile);
      gsu_listing(list_poi,NULL,NULL);
      exit(GSU_EXIT_113);
      };
   fflush(streamlist);
   }
}

/*************************************************************************/
/*                                                                       */
/*  File Name   :  gsu_dirid_exist                                       */
/*  Description :  Has dir-id the same config-type ?                     */
/*                 If no : with overwrite - dir-id will be delete and    */
/*                                          new configurate              */
/*                         without overwrite - exit                      */
/*                 else new configurate                                  */
/*  Author      :  Eva Wilde, BU BA KP13                                 */
/*  Date        :  januar 1994                                           */
/*                                                                       */
/*************************************************************************/
static void gsu_dirid_exist(void)
{
unsigned16 config_type_flg = 0;
char * dirid_exist_poi;
struct stat *puff=NULL;

gsu_listing("grep Clt/Srv-System /tmp/gsu_syncheck_ausgabe > /tmp/gsu_syncheck_client_server", NULL, NULL);
if (system("grep Clt/Srv-System /tmp/gsu_syncheck_ausgabe > /tmp/gsu_syncheck_client_server") < 0)
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "grep Clt/Srv-System /tmp/gsu_syncheck_ausgabe > /tmp/gsu_syncheck_client_server\n");
   gsu_listing("system ERROR of grep Clt/Srv-System /tmp/gsu_syncheck_ausgabe > /tmp/gsu_syncheck_client_server",NULL,NULL);
   exit(GSU_EXIT_114);
   }
if ((puff = malloc (sizeof(struct stat) +1)) == NULL)
   {
   dirid_exist_poi=(char *)dce_sprintf(gc_t_memory_insufficient);
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr,gc_t_memory_insufficient);
   gsu_listing(dirid_exist_poi,NULL,NULL);
   free(dirid_exist_poi);
   exit(GSU_EXIT_115);
   }
gsu_listing("stat /tmp/gsu_syncheck_client_server", NULL, NULL);
if ((stat("/tmp/gsu_syncheck_client_server", puff)) == 0)
   {
   if (puff->st_size > 0)
      config_type_flg = 1;    /*   Client/Server system   */
   else
      config_type_flg = 2;    /*   Client system   */
   }
else                                                                            
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr,"/tmp/gsu_syncheck_client_server\n");
   gsu_listing("stat ERROR of /tmp/gsu_syncheck_client_server",NULL,NULL);
   exit(GSU_EXIT_116);
   }
free(puff);

if (!strcmp(gsu_config, gsu_client_sys))
   {
   /***   gsu_config = client system   ***/
   if ((config_type_flg == 1)&&(oflg == 0))
      {
      dirid_exist_poi=(char *)dce_sprintf(gsu_dirid_server_no_overwrite, gsu_dirid);
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr, gsu_dirid_server_no_overwrite, gsu_dirid);
      gsu_listing(dirid_exist_poi, NULL, NULL);
      free(dirid_exist_poi);
      exit(GSU_EXIT_117);
      }
   if ((config_type_flg == 1)&&(oflg == 1))
      {
      gsu_listing("gsu_dirid_delete()", NULL, NULL);
      gsu_dirid_delete();
      gsu_listing("gsu_setdir()", NULL, NULL);
      gsu_setdir();
      }
   }
else
   {
   /***   gsu_config = client/server system   ***/
   if ((config_type_flg == 2)&&(oflg == 0))
      {
      dirid_exist_poi=(char *)dce_sprintf(gsu_dirid_client_no_overwrite, gsu_dirid);
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr, gsu_dirid_client_no_overwrite, gsu_dirid);
      gsu_listing(dirid_exist_poi, NULL, NULL);
      free(dirid_exist_poi);
      exit(GSU_EXIT_118);
      }
   if ((config_type_flg == 2)&&(oflg == 1))
      {
      gsu_listing("gsu_dirid_delete()", NULL, NULL);
      gsu_dirid_delete();
      gsu_listing("gsu_setdir()", NULL, NULL);
      gsu_setdir();
      }
   }
}

/*************************************************************************/
/*                                                                       */
/*  File Name   :  gsu_dirid_delete                                      */
/*  Description :  Delete dir-id                                         */
/*  Author      :  Eva Wilde, BU BA KP13                                 */
/*  Date        :  januar 1994                                           */
/*                                                                       */
/*************************************************************************/
static void gsu_dirid_delete(void)
{
char *cmd=NULL;
char delete_string[GSU_LENINT]="\0";

/***   DEAKTIVIEREN   ***/
gsu_listing("gsu_deactivation()", NULL, NULL);
gsu_deactivation();

/***   delete gsu_dirid   ***/
if ((cmd=(char *)malloc(strlen("gdssysadm -fc -d -m2 ")+sizeof(int)+1))== NULL)
   {
   cmd=(char *)dce_sprintf(gc_t_memory_insufficient);
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr,gc_t_memory_insufficient);
   gsu_listing(cmd, NULL, NULL);
   free(cmd);
   exit(GSU_EXIT_119);
   }
strcpy(cmd, "gdssysadm -fc -d");
sprintf(delete_string, "%d", gsu_dirid);
strcat (cmd, delete_string);
strcat (cmd, " -m2");
gsu_listing(cmd, NULL, NULL);
if (system(cmd) < 0)
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr,"%s\n", cmd);
   gsu_listing("system ERROR of ", cmd, NULL);
   exit(GSU_EXIT_120);
   }
free(cmd);
}

/*************************************************************************/
/*                                                                       */
/*  File Name   :  gsu_deactivation                                      */
/*  Description :  DIR.X deactivate if active                            */
/*  Author      :  Eva Wilde, BU BA KP13                                 */
/*  Date        :  januar 1994                                           */
/*                                                                       */
/*************************************************************************/
static void gsu_deactivation(void)
{
unsigned8 activflg = 0;
char * deactivation_poi;
struct stat *puff=NULL;

gsu_listing("gdsdirinfo > /tmp/gsu_deactiv_info",NULL,NULL);
if (system("gdsdirinfo > /tmp/gsu_deactiv_info") < 0)
    {
    fprintf(stderr, "ERROR\n");
    fprintf(stderr,"gdsdirinfo > /tmp/gsu_deactiv_info\n");
    gsu_listing("system ERROR of gdsdirinfo > /tmp/gsu_deactiv_info",NULL,NULL);
    exit(GSU_EXIT_121);
    }
/*** if /tmp/gsu_deactiv_info is not empty, than DIR is activ ***/
if ((puff = malloc (sizeof(struct stat) +1)) == NULL)
    {
     deactivation_poi=(char *)dce_sprintf(gc_t_memory_insufficient);
     fprintf(stderr, "ERROR\n");
     dce_fprintf(stderr,gc_t_memory_insufficient);
     gsu_listing(deactivation_poi, NULL, NULL);
     free(deactivation_poi);
     exit(GSU_EXIT_122);
    }
if ((stat("/tmp/gsu_deactiv_info", puff)) == 0)
    {
    if (puff->st_size > 0)
        activflg = 1;          /* DIR is active */
    else
        activflg = 2;          /* DIR isn't active */
    }
else
    {
    fprintf(stderr, "ERROR\n");
    fprintf(stderr,"/tmp/gsu_deactiv_info\n");
    gsu_listing("stat ERROR of /tmp/gsu_deactiv_info", NULL, NULL);
    exit(GSU_EXIT_123);
    }
gsu_listing("rm /tmp/gsu_deactiv_info", NULL, NULL);
if (system("rm /tmp/gsu_deactiv_info") < 0)
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "rm /tmp/gsu_deactiv_info\n");
   gsu_listing("system ERROR of rm /tmp/gsu_deactiv_info", NULL, NULL);
   exit(GSU_EXIT_124);
   }
free(puff);

/***   if DIR is active     ***/
/***   gdssysadm -fd  ***/
if (activflg == 1)
   {
   gsu_listing("gdssysadm -fd -p", NULL, NULL);
   if (system ("gdssysadm -fd -p") < 0)
       {
       fprintf(stderr, "ERROR\n");
       fprintf(stderr, "gdssysadm -fd -p\n");
       gsu_listing("system ERROR of gdssysadm -fd -p", NULL, NULL);
       exit(GSU_EXIT_125);
       }
   }
}
