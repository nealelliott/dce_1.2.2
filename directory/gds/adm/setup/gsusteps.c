/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: gsusteps.c,v $
 * Revision 1.1.4.2  1996/02/18  19:42:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:01  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:21:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:53  root]
 * 
 * Revision 1.1.2.6  1994/10/05  15:38:08  keutel
 * 	OT 12507
 * 	[1994/10/05  15:01:32  keutel]
 * 
 * Revision 1.1.2.5  1994/09/19  08:39:18  marrek
 * 	Fixes for OT12176 (wrong incore tables.)
 * 	[1994/09/19  08:32:00  marrek]
 * 
 * Revision 1.1.2.4  1994/09/01  15:16:08  keutel
 * 	OT 11988: core dump on SVR4
 * 	[1994/09/01  13:32:19  keutel]
 * 
 * Revision 1.1.2.3  1994/08/19  13:54:11  marrek
 * 	Improve testcode coverage OT11586.
 * 	[1994/08/19  11:46:13  marrek]
 * 
 * Revision 1.1.2.2  1994/07/07  15:41:52  marrek
 * 	July 1994 code drop.
 * 	[1994/07/07  15:39:49  marrek]
 * 
 * Revision 1.1.2.1  1994/06/21  14:39:29  marrek
 * 	Bug fix for June 94 code submission.
 * 	[1994/06/15  10:31:35  marrek]
 * 
 * 	Created in June 94 code submission.
 * 	[1994/06/10  16:04:06  marrek]
 * 
 * $EndLog$
 */

/**********************************************************************/
/*                                                                    */
/*  Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG         */
/*  All Rights Reserved                                               */
/*                                                                    */
/*  Project          : GDU - GDS Attribute Info Module                */
/*  File Name        : steps.c                                        */
/*  Description      : This file aids in the configuring and ini-     */
/*                     tialising of directory systems                 */
/*  Date             : Oktober 1993                                   */
/*  Author           : Eva Wilde, SNI BU BA KP13                      */
/*                                                                    */
/**********************************************************************/
 
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gsusteps.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 19:42:15 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <string.h>
#include <gds.h>
#include <gsu.h>
#include <dce/d2dir.h>
#include <d27util.h>
#include <stdlib.h>
#include <gdscp.h>
#include <gctconv.h>
#include <sys/types.h>

extern int gsu_dirid;                       /* directory indicator */
extern int gsu_nocl;                        /* number of Client-processes */
extern int gsu_noser;                       /* number of act. Servers */
extern char gsu_config[GSU_CONFIG];         /* configuration: client/server */
extern char gsu_cladd[GSU_DSA];             /* PSAP-addresse of client */
extern char gsu_locdsa[GSU_DSA];            /* local DSA name */
extern char gsu_psaploc[GSU_PSAP];          /* PSAP from local DSA  */
extern char gsu_contdsa[GSU_DSA];           /* contact DSA */
extern char gsu_psapcont[GSU_PSAP];         /* PSAP from contact DSA  */
extern char gsu_locals[GSU_LOCALS];         /* local DSA is initial DSA */
extern char gsu_defsch[GSU_DEFSCH];         /* default schema indicator */
extern char gsu_userpsw[GSU_USERPSW];       /* user password for local DSA */
extern char gsu_e_secur[GSU_SECUR];         /* for Authentication mechanism */
extern char gsu_princ[GSU_PRINC];           /* for Principal name  */
extern const char * gsu_client_sys;         /* constant for client system */
extern gsu_dsa *anfang1;                    /* for additional remote DSA liste*/
extern gsu_dsa *anfang2;                    /* for default DSA liste  */
extern unsigned16 oflg;			    /* for ovewrite flag  */
extern unsigned16 listflg;		    /* for listing */
extern unsigned16 initmode;		    /* for determinate of init.types */
extern unsigned16 gsu_authent_mech;         /* for Authentication mechanism */
extern FILE * streamlist;
unsigned8 first_level_flg=0;		    /* for first level operations */
char gsu_locdsa_country[GSU_COUNTRY];	    /* for first level country */
char ** gsu_in_vect;			    /* for input vector in gdscp  */
char * gsu_dce_printf_string;               /* for listing-strings   */
gc_t_vector * gsu_out_vect;		    /* for output in gdscp  */
char *distname[D2_NP_MAX];                  /* part of distinguished name */
gc_t_vector *respart[D2_NP_MAX];      /* Attribute of part distinguished name */

/***   strings for gsu_in_vect   ***/
const char * gsu_attribute = "-attribute";
const char * gsu_subtree = "-subtree";
const char * gsu_attr_dirid = "-dirid";
const char * gsu_attr_cache = "-cache";
const char * gsu_changeattr = "-changeattr";
const char * gsu_removeattr = "-removeattr";
const char * gsu_addattr = "-addattr";
const char * gsu_attr_dsa = "-dsa";
const char * gsu_attr_psap = "-psap";
const char * gsu_psap_name = "PSA={";
const char * gsu_psap_name_only = "PSA";
const char * gsu_pswd_name = "UP=";
const char * gsu_preferchain = "-preferchain";
const char * gsu_localscope = "-localscope";
const char * gsu_first_level = "/";
const char * gsu_onelevel = "-onelevel";
const char * gsu_duacache = "-duacache";
const char * gsu_duafirst = "-duafirst";
const char * gsu_usedsa = "-usedsa";
const char * gsu_cacheclass = "-cacheclass";
const char * gsu_dontusecopy = "-dontusecopy";
const char * gsu_baseobject = "-baseobject";
const char * gsu_default = "-default";
const char * gsu_allattr = "-allattr";
const char * gsu_preferadminfkt = "-preferadmfunctions";
const char * gsu_ocl = "OCL=DSA;APE";
const char * gsu_ocl_names[]={"OCL=C","OCL=ORG","OCL=OU","OCL=APP","OCL=DSA;APE"};
const char * gsu_schema = "/CN=schema";
const char * gsu_mk = "MK";
const char * gsu_am = "AM=";          /* for Security  */
const char * gsu_secur_step_dce = "DCE";
const char * gsu_pn = "PN=";          /* for Principal name */
const char * gsu_mk_name = "MK={";
const char * gsu_resident = "RESIDENT";
const char * gsu_true = "TRUE";
const char * gsu_false = "FALSE";
const char * gsu_sea = "SEA";
const char * gsu_sea_name = "SEA={";
const char * gsu_at = "43.12.2.1107.1.3.4.6";
const char * gsu_oct = "43.12.2.1107.1.3.4.5";
const char * gsu_srt = "43.12.2.1107.1.3.4.4";
const char * gsu_tst = "43.12.2.1107.1.3.4.2";
const char * gsu_client_id = "43.12.2.1107.1.3.4.10=CLIENT";
const char * gsu_def_dsa_id = "43.12.2.1107.1.3.4.8=DEFAULT-DSA";
const char * gsu_loc_dsa_id = "43.12.2.1107.1.3.4.9=LOCAL-DSA";

extern void gsu_listing(char *, char *, char *);
static void gsu_listing_malloc(char *listing_string, signed16 listing_counter);
extern void gsu_step1(void);                                                    
extern void gsu_step2(void);                                                    
extern void gsu_step3(void);                                                    
extern void gsu_step4(void);                                                    
extern void gsu_step5(void);                                                    
extern void gsu_step6(void);                                                    
extern void gsu_step7(void);                                                    
extern void gsu_step8(void);                                                    
extern void gsu_step9(void);
static void gsu_bind(signed16, char **);
static void gsu_create (char **, signed16, gc_t_vector **);
static void gsu_alloc(signed16);
static void gsu_add_defdsa_cache(char *, char *);
static unsigned16 gsu_read_partname(unsigned8);
static void gsu_create_sh_contdsa(unsigned16, unsigned16, char *);
static void gsu_create_sh_dsa(char *, signed16);
static void gsu_create_mk_dsaname(char *, unsigned16, unsigned16);
static void gsu_extend_remdsa(char *);
static unsigned16 gsu_num_of_frst_obj(gc_t_vector *);
static void gsu_create_myfrst(gc_t_vector *);
static void gsu_exit(void);
static void gsu_service_control_cache(void);
static void gsu_default_control_service(void);
static unsigned8 gsu_attribute_value_delete(char *, char *);
static void gsu_preferadminfkt_fkt(void);
static void gsu_dontusecopy_false_fkt(void);
static void gsu_localscope_true_fkt(void);
static void gsu_dsa_name_file(void);
static void gsu_search_psap_dsa(gsu_dsa *search_psap_dsa_act);

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_step1                                      */
/*  Description   :  Client-address -> DUA-Cache                    */
/*  Date          :  November 1993                                  */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
void gsu_step1(void)
{
char * step1_help=NULL;
char step1_dirid_string[GSU_LENINT];

if (gsu_cladd[0] == '\0')
   return;

/*   bind to cache   */
gsu_alloc(3);
gsu_in_vect[0] = (char *)gsu_attr_dirid;
sprintf(step1_dirid_string, "%d",1);
gsu_in_vect[1] = step1_dirid_string;
gsu_in_vect[2] = (char *)gsu_attr_cache;
gsu_listing_malloc("gc_t_bind", 3);
gsu_bind(3, gsu_in_vect);
free(gsu_in_vect);

/*   modify the service control settings   */
gsu_service_control_cache();

/*   create client in cache   */
gsu_alloc(3);
if ((step1_help=(char *)malloc(strlen(gsu_psap_name)+strlen(gsu_cladd) +strlen("}")+1)) == NULL)
   {
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gc_t_memory_insufficient);
   step1_help=(char *)dce_sprintf(gc_t_memory_insufficient);
   gsu_listing(step1_help, NULL, NULL);
   gsu_exit();
   }
strcpy(step1_help, gsu_psap_name);
strcat(step1_help, gsu_cladd);
strcat(step1_help, "}");
gsu_in_vect[0] = (char *)gsu_client_id;
gsu_in_vect[1] = (char *)gsu_attribute;
gsu_in_vect[2] = (char *)step1_help;
gsu_create(gsu_in_vect, 3, &gsu_out_vect);
free(gsu_in_vect);
free(step1_help);
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_step2                                      */
/*  Description   :  Local-DSA -> DUA-Cache                         */
/*  Date          :  November 1993                                  */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
void gsu_step2(void)
{
char * step2_help=NULL;
char * step2_help1=NULL;
char * step2_help2=NULL;
char step2_dirid_string[GSU_LENINT];
unsigned8 step2_counter;

/*  create of DSA-file   */
gsu_dsa_name_file();

/*   bind to cache   */
gsu_alloc(3);
gsu_in_vect[0] = (char *)gsu_attr_dirid;
sprintf(step2_dirid_string, "%d", gsu_dirid);
gsu_in_vect[1] = step2_dirid_string;
gsu_in_vect[2] = (char *)gsu_attr_cache;
gsu_listing_malloc("gc_t_bind", 3);
gsu_bind(3, gsu_in_vect); 
free(gsu_in_vect);

/*   modify the service control settings   */
gsu_service_control_cache();

/*  create SEA attributte   */
if ((step2_help=(char *)malloc(strlen(gsu_sea_name) + strlen(gsu_locdsa)+strlen("}")+1)) == NULL)
   {
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gc_t_memory_insufficient);
   step2_help=(char *)dce_sprintf(gc_t_memory_insufficient);
   gsu_listing(step2_help, NULL, NULL);
   gsu_exit();
   }
strcpy(step2_help, gsu_sea_name);
strcat(step2_help, gsu_locdsa);
strcat(step2_help, "}");
if ((step2_help1=(char *)malloc(strlen(gsu_am)+sizeof(int)+1)) == NULL)
   {
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gc_t_memory_insufficient);
   step2_help1=(char *)dce_sprintf(gc_t_memory_insufficient);
   gsu_listing(step2_help1, NULL, NULL);
   gsu_exit();
   }
sprintf(step2_help1, "%s%d", gsu_am, gsu_authent_mech);
if (!strcmp(gsu_e_secur,gsu_secur_step_dce))
   {
   if ((step2_help2=(char *)malloc(strlen(gsu_pn)+strlen(gsu_princ)+1))==NULL)
      {
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr, gc_t_memory_insufficient);
      step2_help2=(char *)dce_sprintf(gc_t_memory_insufficient);
      gsu_listing(step2_help2, NULL, NULL);
      gsu_exit();
      }
   strcpy(step2_help2, gsu_pn);
   strcat(step2_help2, gsu_princ);
   gsu_alloc(5);
   step2_counter=5;
   gsu_in_vect[3] = (char *)step2_help1;
   gsu_in_vect[4] = (char *)step2_help2;
   }
else
   {
   gsu_alloc(3);
   step2_counter=3;
   }

gsu_in_vect[0] = (char *)gsu_loc_dsa_id;
gsu_in_vect[1] = (char *)gsu_attribute;
gsu_in_vect[2] = (char *)step2_help;
gsu_create(gsu_in_vect, step2_counter, &gsu_out_vect);
free(gsu_in_vect);
free(step2_help);
if (step2_help1 != NULL)
   {
   free(step2_help1);
   }
if (step2_help2 != NULL)
   {
   free(step2_help2);
   }

/*   create local DSA in cache   */
gsu_alloc(5);
if ((step2_help=(char *)malloc(strlen(gsu_psap_name)+strlen(gsu_psaploc)+strlen("}")+1))==NULL)
   {
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gc_t_memory_insufficient);
   step2_help=(char *)dce_sprintf(gc_t_memory_insufficient);
   gsu_listing(step2_help, NULL, NULL);
   gsu_exit();
   }
if ((step2_help1=(char *)malloc(strlen(gsu_pswd_name)+strlen(gsu_userpsw)+1))==NULL)
   {
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gc_t_memory_insufficient);
   step2_help1=(char *)dce_sprintf(gc_t_memory_insufficient),
   gsu_listing(step2_help1, NULL, NULL);
   gsu_exit();
   }
strcpy(step2_help, gsu_psap_name);
strcat(step2_help, gsu_psaploc);
strcat(step2_help, "}");
if (gsu_userpsw[0] != '\0')
   {
   strcpy(step2_help1, gsu_pswd_name);
   strcat(step2_help1, gsu_userpsw);
   gsu_in_vect[4] = step2_help1;
   step2_counter = 5;
   }
else
   {
   step2_counter = 4;
   }
gsu_in_vect[0] = gsu_locdsa;
gsu_in_vect[1] = (char *)gsu_attribute;
gsu_in_vect[2] = (char *)gsu_ocl;
gsu_in_vect[3] = step2_help;
gsu_create(gsu_in_vect, step2_counter, &gsu_out_vect);
free(gsu_in_vect);
free(step2_help);
free(step2_help1);

/*   modify the service control settings   */
gsu_default_control_service();
gsu_preferadminfkt_fkt();
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_step3                                      */
/*  Description   :  Schema-MK -> same as that of Init-DSA schema   */
/*  Date          :  November 1993                                  */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
void gsu_step3(void)
{
int i;
char * step3_schema_cont=NULL;    /*  to save schema from contact DSA  */
char * step3_mk_locdsa=NULL;      /*  to save MK from local DSA  */
char step3_dirid_string[GSU_LENINT]="\0";

/*   bind to contact DSA   */
gsu_alloc(4);
gsu_in_vect[0] = (char *)gsu_attr_dirid;
sprintf(step3_dirid_string, "%d", gsu_dirid);
gsu_in_vect[1] = step3_dirid_string;
gsu_in_vect[2] = (char *)gsu_attr_psap;
gsu_in_vect[3] = gsu_psapcont;
gsu_listing_malloc("gc_t_bind", 4);
gsu_bind(4, gsu_in_vect);
free(gsu_in_vect);

/* read schema from contact DSA and save it in step3_schema_cont   */
gsu_alloc(3);
gsu_in_vect[0] = (char *)gsu_schema;
gsu_in_vect[1] = (char *)gsu_attribute;
gsu_in_vect[2] = (char *)gsu_mk;
gsu_listing_malloc("gc_t_read", 3);
if (gc_t_read(gsu_in_vect, 3, TRUE, TRUE, &gsu_out_vect) != GC_SUCCESS)
    {
    fprintf(stderr, "ERROR\n gc_t_read ");
    for(i=0; i<3; i++)
       {
       fprintf(stderr, "%s ",gsu_in_vect[i]);
       }
    fprintf(stderr, "\n");
    fprintf(stderr, "    %s\n",  gsu_out_vect[0].value);
    gsu_listing(gsu_out_vect[0].value, NULL, NULL);
    gc_t_free_result(gsu_out_vect);
    free(gsu_in_vect);
    gsu_exit();
    }
if ((step3_schema_cont=(char *)malloc(strlen(gsu_out_vect[1].value)+1))==NULL)
   {
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gc_t_memory_insufficient);
   step3_schema_cont=(char *)dce_sprintf(gc_t_memory_insufficient);
   gsu_listing(step3_schema_cont,NULL,NULL);
   gsu_exit();
   }
strcpy(step3_schema_cont,gsu_out_vect[1].value);
gc_t_free_result(gsu_out_vect);
free(gsu_in_vect);

/*   bind to local DSA   */
gsu_alloc(4);
gsu_in_vect[0] = (char *)gsu_attr_dirid;
sprintf(step3_dirid_string, "%d", gsu_dirid);
gsu_in_vect[1] = step3_dirid_string;
gsu_in_vect[2] = (char *)gsu_attr_dsa;
gsu_in_vect[3] = gsu_locdsa;
gsu_listing_malloc("gc_t_bind", 4);
gsu_bind(4, gsu_in_vect);
free(gsu_in_vect);

/*   modify the service control settings   */
gsu_default_control_service();
gsu_preferadminfkt_fkt();

/*  change schema from local DSA with schema from contact DSA   */
gsu_alloc(4);
if ((step3_mk_locdsa=(char *)malloc(strlen(gsu_mk_name)+strlen(gsu_locdsa)+strlen("}") + 1)) == NULL)
   {
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gc_t_memory_insufficient);
   step3_mk_locdsa=(char *)dce_sprintf(gc_t_memory_insufficient);
   gsu_listing(step3_mk_locdsa,NULL,NULL);
   gsu_exit();
   }
strcpy(step3_mk_locdsa, gsu_mk_name);
strcat(step3_mk_locdsa, gsu_locdsa);
strcat(step3_mk_locdsa, "}");
gsu_in_vect[0] = (char *)gsu_schema;
gsu_in_vect[1] = (char *)gsu_changeattr;
gsu_in_vect[2] = step3_mk_locdsa;
gsu_in_vect[3] = step3_schema_cont;
gsu_listing_malloc("gc_t_modify", 4);
if (gc_t_modify(gsu_in_vect, 4, TRUE, &gsu_out_vect) != GC_SUCCESS)
    {
    fprintf(stderr, "ERROR\n gc_t_modify ");
    for(i=0; i<4; i++)
       {
       fprintf(stderr, "%s ", gsu_in_vect[i]);
       }
    fprintf(stderr, "\n");
    fprintf(stderr, "    %s\n", gsu_out_vect[0].value);
    gsu_listing(gsu_out_vect[0].value, NULL, NULL);
    gc_t_free_result(gsu_out_vect);
    free(gsu_in_vect);
    gsu_exit();
    }
free(gsu_in_vect);
free(step3_mk_locdsa);
free(step3_schema_cont);
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_step4                                      */
/*  Description   :  Schema (Init-DSA) -> Local-DSA                 */
/*  Date          :  November 1993                                  */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
void gsu_step4(void)
{
signed16 i;
char step4_dirid_string[GSU_LENINT]="\0";
gc_t_vector * step4_zw=NULL;

/*   bind to contact DSA   */
gsu_alloc(4);
gsu_in_vect[0] = (char *)gsu_attr_dirid;
sprintf(step4_dirid_string, "%d", gsu_dirid);
gsu_in_vect[1] = step4_dirid_string;
gsu_in_vect[2] = (char *)gsu_attr_dsa;
gsu_in_vect[3] = gsu_contdsa;
gsu_listing_malloc("gc_t_bind", 4);
gsu_bind(4, gsu_in_vect);
free(gsu_in_vect);

/* read AT, OCT, SRT and TST from contact DSA and save it in step4_zw */
gsu_alloc(6);
gsu_in_vect[0] = (char *)gsu_schema;
gsu_in_vect[1] = (char *)gsu_attribute;
gsu_in_vect[2] = (char *)gsu_at;
gsu_in_vect[3] = (char *)gsu_oct;
gsu_in_vect[4] = (char *)gsu_srt;
gsu_in_vect[5] = (char *)gsu_tst;
gsu_listing_malloc("gc_t_read", 6);
if (gc_t_read(gsu_in_vect, 6, TRUE, TRUE, &step4_zw) != GC_SUCCESS)
   {
   fprintf(stderr, "ERROR\n gc_t_read ");
   for(i=0; i<6;i++)
      {
      fprintf(stderr,"%s ", gsu_in_vect[i]);
      }
   fprintf(stderr, "\n");
   fprintf(stderr, "    %s\n",  step4_zw[0].value);
   gsu_listing(step4_zw[0].value, NULL, NULL);
   gc_t_free_result(step4_zw);
   free(gsu_in_vect);
   gsu_exit();
   }
free(gsu_in_vect);

/*   bind to local dsa   */
gsu_alloc(4);
gsu_in_vect[0] = (char *)gsu_attr_dirid;
sprintf(step4_dirid_string,"%d", gsu_dirid);
gsu_in_vect[1] = step4_dirid_string;
gsu_in_vect[2] = (char *)gsu_attr_dsa;
gsu_in_vect[3] = gsu_locdsa;
gsu_listing_malloc("gc_t_bind", 4);
gsu_bind(4, gsu_in_vect);
free(gsu_in_vect);

/*   modify service controls   */
gsu_default_control_service();

/*   remove AT, OCT, SRT and TST in local DSA  */
gsu_alloc(6);
gsu_in_vect[0] = (char *)gsu_schema;
gsu_in_vect[1] = (char *)gsu_removeattr;
gsu_in_vect[2] = (char *)gsu_at;
gsu_in_vect[3] = (char *)gsu_oct;
gsu_in_vect[4] = (char *)gsu_srt;
gsu_in_vect[5] = (char *)gsu_tst;
gsu_listing_malloc("gc_t_modify", 6);
if (gc_t_modify(gsu_in_vect, 6, TRUE, &gsu_out_vect) != GC_SUCCESS)
   {
   fprintf(stderr, "ERROR\n gc_t_modify ");
   for(i=0; i<6; i++)
      {
      fprintf(stderr, "%s ", gsu_in_vect[i]);
      }
   fprintf(stderr, "\n");
   fprintf(stderr, "    %s\n",  gsu_out_vect[0].value);
   gsu_listing(gsu_out_vect[0].value, NULL, NULL);
   gc_t_free_result(gsu_out_vect);
   free(gsu_in_vect);
   gsu_exit();
   }
free(gsu_in_vect);

/*   add attributes from contact DSA in local DSA   */
gsu_alloc(3);
gsu_in_vect[0] = (char *)gsu_schema;
gsu_in_vect[1] = (char *)gsu_addattr;
for (i=1; i<5; i++)
   {
   gsu_in_vect[2] = step4_zw[i].value;
   gsu_listing_malloc("gc_t_modify", 3);
   if (gc_t_modify(gsu_in_vect, 3, TRUE, &gsu_out_vect) != GC_SUCCESS)
      {
      fprintf(stderr, "ERROR\n gc_t_modify ");
      for(i=0; i<3; i++)
	 {
	 fprintf(stderr, "%s ", gsu_in_vect[i]);
	 }
      fprintf(stderr, "\n");
      fprintf(stderr, "    %s\n",  gsu_out_vect[0].value);
      gsu_listing(gsu_out_vect[0].value, NULL, NULL);
      gc_t_free_result(gsu_out_vect);
      free(gsu_in_vect);
      gsu_exit();
      }
   }
gc_t_free_result(step4_zw);
free(gsu_in_vect);
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_step5                                      */
/*  Description   :  Default and additional DSA's -> Cache          */
/*  Date          :  November 1993                                  */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
void gsu_step5(void)
{
char * step5_help=NULL;
char step5_dirid_string[GSU_LENINT]="\0";
gsu_dsa *step5_act=NULL;

/*   bind to cache   */
gsu_alloc(3);
gsu_in_vect[0] = (char *)gsu_attr_dirid;
sprintf(step5_dirid_string, "%d", gsu_dirid);
gsu_in_vect[1] = step5_dirid_string;
gsu_in_vect[2] = (char *)gsu_attr_cache;
gsu_listing_malloc("gc_t_bind", 3);
gsu_bind(3, gsu_in_vect);
free(gsu_in_vect);

/*   modify the service control settings   */
gsu_service_control_cache();

/*   create all default DSA's in Cache   */
step5_act=anfang2;      /*  default DSA   */
while (step5_act != NULL)
   {
   gsu_listing("gsu_add_defdsa_cache(",step5_act->dsaname,")");
   gsu_add_defdsa_cache(step5_act->dsaname, step5_act->psapadd);
   step5_act=step5_act->next;
   };

/*   create all additional DSA's with those PSAP address in cache   */
step5_act=anfang1;       /*   additional remote DSA   */
while (step5_act != NULL)
   {
   if (step5_act->psapadd[0] == '\0')
      {
      gsu_listing("gsu_search_psap_dsa(",step5_act->dsaname,")");
      gsu_search_psap_dsa(step5_act);
      }
   gsu_alloc(4);
   if ((step5_help=(char *)malloc(strlen(gsu_psap_name)+strlen(step5_act->psapadd)+strlen("}")+1))==NULL)
      {
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr, gc_t_memory_insufficient);
      step5_help=(char *)dce_sprintf(gc_t_memory_insufficient);
      gsu_listing(step5_help,NULL,NULL);
      gsu_exit();
      }
   strcpy(step5_help, gsu_psap_name);
   strcat(step5_help, step5_act->psapadd);
   strcat(step5_help, "}");
   gsu_in_vect[0] = step5_act->dsaname;
   gsu_in_vect[1] = (char *)gsu_attribute;
   gsu_in_vect[2] = step5_help;
   gsu_in_vect[3] = (char *)gsu_ocl;
   gsu_create(gsu_in_vect, 4, &gsu_out_vect);
   free(gsu_in_vect);
   free(step5_help);
   step5_act=step5_act->next;
   };
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_step6                                      */
/*  Description   :  Local-DSA-Obj -> Local-DSA                     */
/*  Date          :  November 1993                                  */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
void gsu_step6(void)
{
int ret_val;
signed16 step6_counter;
char step6_dirid_string[GSU_LENINT]="\0";
char *step6_hilf_psap=NULL;
char *step6_hilf_authent=NULL;
char *step6_hilf_princ=NULL;

dce_printf(gsu_sleep_sec, 60);
gsu_listing("sleep for 60 s.", NULL, NULL);
sleep(60);

if (initmode != 2)
   {
   /*   modify the service control settings   */
   gsu_service_control_cache();

   /*   bind to cache   */
   gsu_alloc(3);
   gsu_in_vect[0] = (char *)gsu_attr_dirid;
   sprintf(step6_dirid_string, "%d",gsu_dirid);
   gsu_in_vect[1] = step6_dirid_string;
   gsu_in_vect[2] = (char *)gsu_attr_cache;
   gsu_listing_malloc("gc_t_bind", 3);
   gsu_bind(3, gsu_in_vect);
   free(gsu_in_vect);

   /*   create contact DSA in cache   */
   gsu_alloc(5);
   if ((step6_hilf_psap=(char *)malloc(strlen(gsu_psap_name)+strlen(gsu_psapcont)+strlen("}")+1))==NULL)
      {
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr, gc_t_memory_insufficient);
      step6_hilf_psap=(char *)dce_sprintf(gc_t_memory_insufficient);
      gsu_listing(step6_hilf_psap,NULL,NULL);
      gsu_exit();
      }
   if ((step6_hilf_authent=(char *)malloc(strlen(gsu_am)+sizeof(int)+1))==NULL)
      {
      fprintf(stderr, "ERROR\n"),
      dce_fprintf(stderr, gc_t_memory_insufficient);
      step6_hilf_authent=(char *)dce_sprintf(gc_t_memory_insufficient);
      gsu_listing(step6_hilf_authent,NULL,NULL);
      gsu_exit();
      }
   strcpy(step6_hilf_psap, gsu_psap_name);
   strcat(step6_hilf_psap, gsu_psapcont);
   strcat(step6_hilf_psap, "}");
   sprintf(step6_hilf_authent, "%s%d", gsu_am, gsu_authent_mech);
   gsu_in_vect[0] = gsu_contdsa;
   gsu_in_vect[1] = (char *)gsu_attribute;
   gsu_in_vect[2] = (char *)gsu_ocl;
   gsu_in_vect[3] = step6_hilf_psap;
   gsu_in_vect[4] = step6_hilf_authent;
   gsu_create(gsu_in_vect, 5, &gsu_out_vect);
   free(gsu_in_vect);
   free(step6_hilf_psap);

   dce_printf(gsu_sleep_sec, 60);
   gsu_listing("sleep for 60 s.", NULL, NULL);
   sleep(60);

   /*   bind to contact DSA   */
   gsu_alloc(4);
   gsu_in_vect[0] = (char *)gsu_attr_dirid;
   sprintf(step6_dirid_string, "%d",gsu_dirid);
   gsu_in_vect[1] = step6_dirid_string;
   gsu_in_vect[2] = (char *)gsu_attr_dsa;
   gsu_in_vect[3] = gsu_contdsa;
   gsu_listing_malloc("gc_t_bind", 4);
   gsu_bind(4, gsu_in_vect);
   free(gsu_in_vect);

   /*   modify the service control settings   */
   gsu_default_control_service();
   gsu_preferadminfkt_fkt();
   gsu_dontusecopy_false_fkt();
   gsu_localscope_true_fkt();

   /*   read local DSA name from contact DSA   */
   /*   if it doesn'n exist, than part what is exist add as shadow
        and part what isn't exist add as MK   */
   gsu_alloc(1);
   gsu_in_vect[0] = gsu_locdsa;
   gsu_listing_malloc("gc_t_read", 1);
   if ((ret_val=gc_t_read(gsu_in_vect,1,TRUE,TRUE,&gsu_out_vect)) != GC_SUCCESS)
       /***  local DSA name doesn't exist   ***/
       {
       if (ret_val == GC_T_DS_E_NO_SUCH_OBJECT)
          {
          free(gsu_in_vect);
	  gsu_listing("     return value = ", gsu_out_vect[1].value, NULL);
	  gsu_listing("gsu_read_partname()", NULL,NULL);
	  step6_counter=gsu_read_partname(1);
          sprintf(step6_dirid_string, "%d",step6_counter);
	  gsu_listing("     return value= ", step6_dirid_string, NULL);
	  gsu_listing("gsu_create_sh_contdsa",NULL,NULL);
          gsu_create_sh_contdsa(1,step6_counter,gsu_locdsa);
          if (step6_counter > 0)
	     {
	     gsu_listing("gsu_create_sh_dsa",NULL,NULL);
	     gsu_create_sh_dsa(gsu_locdsa,step6_counter);
	     }
	  else
	     {
             /*   bind to gsu_locdsa */
             gsu_alloc(4);
             gsu_in_vect[0] = (char *)gsu_attr_dirid;
             sprintf(step6_dirid_string, "%d", gsu_dirid);
             gsu_in_vect[1] = step6_dirid_string;
             gsu_in_vect[2] = (char *)gsu_attr_dsa;
             gsu_in_vect[3] = gsu_locdsa;
             gsu_listing_malloc("gc_t_bind", 4);
             gsu_bind(4, gsu_in_vect);
             free(gsu_in_vect);
	     }
	  gsu_listing("gsu_create_mk_dsaname",NULL,NULL);
          gsu_create_mk_dsaname(gsu_locdsa,step6_counter,1);
          }
       else
          {
          fprintf(stderr, "ERROR\n");
	  fprintf(stderr, "gc_t_read %s\n",gsu_in_vect[0]);
          fprintf(stderr, "    %s\n",  gsu_out_vect[0].value);
          gsu_listing(gsu_out_vect[0].value, NULL, NULL);
          gc_t_free_result(gsu_out_vect);
          free(gsu_in_vect);
          gsu_exit();
          }
       }
   else
       {
       /***  local DSA name already exists in contact DSA  ***/   
       free(gsu_in_vect);
       dce_fprintf(stderr,gsu_local_dsa_contain_contact_dsa,gsu_out_vect[0].value);
       step6_hilf_princ=(char *)dce_sprintf(gsu_local_dsa_contain_contact_dsa,gsu_out_vect[0].value);
       gsu_listing(step6_hilf_princ,NULL,NULL);
       free(step6_hilf_princ);
       gc_t_free_result(gsu_out_vect);
       }
   }
else
   {
   /**********   initmode = 2    ***********/
   /*   modify the service control settings   */
   gsu_default_control_service();
   gsu_preferadminfkt_fkt();

   /*   bind to local DSA   */
   gsu_alloc(4);
   gsu_in_vect[0] = (char *)gsu_attr_dirid;
   sprintf(step6_dirid_string, "%d", gsu_dirid);
   gsu_in_vect[1] = step6_dirid_string;
   gsu_in_vect[2] = (char *)gsu_attr_dsa;
   gsu_in_vect[3] = gsu_locdsa;
   gsu_listing_malloc("gc_t_bind", 4);
   gsu_bind(4, gsu_in_vect);
   free(gsu_in_vect);

   gsu_listing("gsu_create_mk_dsaname",NULL,NULL);
   gsu_create_mk_dsaname(gsu_locdsa, 0, 1);
   }
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_step7                                      */
/*  Description   :  additional Remote DSA (Shadows) -> Local-DSA   */
/*  Date          :  November 1993                                  */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
void gsu_step7(void)
{
signed16 i;
unsigned16 step7_counter;
unsigned16 step7_mkflg;
int ret_val;
char step7_dirid_string[GSU_LENINT]="\0";
char * step7_help=NULL;
gsu_dsa *act=NULL;

/*   create all remote DSA's in contact DSA   */
act = anfang1;   /*  additional remote DSA list   */
while(act != NULL)
   {
   if (strcmp(act->dsaname, gsu_contdsa))
      {
      /*   modify the service control settings   */
      gsu_default_control_service();
      gsu_preferadminfkt_fkt();

      /*   bind to CACHE   */
      gsu_alloc(3);
      gsu_in_vect[0] = (char *)gsu_attr_dirid;
      sprintf(step7_dirid_string, "%d",gsu_dirid);
      gsu_in_vect[1] = step7_dirid_string;
      gsu_in_vect[2] = (char *)gsu_attr_cache;
      gsu_listing_malloc("gc_t_bind", 3);
      gsu_bind(3, gsu_in_vect);
      free(gsu_in_vect);
      
      /*   modify the service control settings   */
      gsu_service_control_cache();
   
      /*   create additional remote DSA in CACHE   */
      gsu_alloc(3);
      if ((step7_help=(char *)malloc(strlen(gsu_psap_name)+strlen(act->psapadd) +strlen("}")+1)) == NULL)
         {
	 fprintf(stderr, "ERROR\n");
         dce_fprintf(stderr, gc_t_memory_insufficient);
	 step7_help=(char *)dce_sprintf(gc_t_memory_insufficient);
	 gsu_listing(step7_help,NULL,NULL);
         gsu_exit();
         }
      strcpy(step7_help, gsu_psap_name);
      strcat(step7_help, act->psapadd);
      strcat(step7_help, "}");
      gsu_in_vect[0] = act->dsaname;
      gsu_in_vect[1] = (char *)gsu_attribute;
      gsu_in_vect[2] = (char *)step7_help;
      gsu_create(gsu_in_vect, 3, &gsu_out_vect);
      free(gsu_in_vect);
      free(step7_help);
      }

   /*   modify the service control settings   */
   gsu_default_control_service();
   gsu_preferadminfkt_fkt();

   if (!strcmp(gsu_config, gsu_client_sys))
      {
      act=act->next;
      continue;
      }

   /*   bind to contact DSA  */
   gsu_alloc(4);
   gsu_in_vect[0] = (char *)gsu_attr_dirid;
   sprintf(step7_dirid_string, "%d", gsu_dirid);
   gsu_in_vect[1] = step7_dirid_string;
   gsu_in_vect[2] = (char *)gsu_attr_dsa;
   gsu_in_vect[3] = gsu_contdsa;
   gsu_listing_malloc("gc_t_bind", 4);
   gsu_bind(4, gsu_in_vect);
   free(gsu_in_vect);

   /*   read from contact DSA remote DSA   */
   gsu_alloc(1);
   gsu_in_vect[0] = act->dsaname;
   gsu_listing_malloc("gc_t_read", 1);
   if ((ret_val=gc_t_read(gsu_in_vect,1,TRUE,TRUE,&gsu_out_vect)) != GC_SUCCESS)
      {
      if (ret_val == GC_T_DS_E_NO_SUCH_OBJECT)
         {
         /***   DSA name doesn't exist   ***/
         free(gsu_in_vect);
	 gsu_listing("     return value = ", gsu_out_vect[1].value, NULL);
	 if (!strcmp(gsu_out_vect[1].value,act->dsaname))
	    {
	    gsu_listing("gsu_read_partname()",NULL,NULL);
	    step7_counter=gsu_read_partname(1);
            sprintf(step7_dirid_string, "%d", step7_counter);
	    gsu_listing("     return value = ",step7_dirid_string,NULL);
	    gsu_listing("gsu_create_sh_dsa",NULL,NULL);
	    gsu_create_sh_dsa(gsu_locdsa, step7_counter);
	    }
	 else
	    {
	    dce_fprintf(stderr,gsu_remote_dsa_no_in_contact,act->dsaname);
	    }
         }
      else
         {
         fprintf(stderr, "ERROR\n");
	 fprintf(stderr,"gc_t_read %s\n", gsu_in_vect[0]);
         fprintf(stderr, "    %s\n",  gsu_out_vect[0].value);
         gsu_listing(gsu_out_vect[0].value, NULL, NULL);
         gc_t_free_result(gsu_out_vect);
         free(gsu_in_vect);
	 gsu_exit();
         }
      }
   else
      /***   additional remote DSA name already exists   ***/
      {
      free(gsu_in_vect);
      if (act->psapadd == NULL)
	 {
	 for(i=0; gsu_out_vect[i].type != GC_T_EOL; i++)
	    {
            if (!strncmp(gsu_out_vect[i].value,gsu_psap_name,strlen(gsu_psap_name)))
	       {
               step7_mkflg=1;
	       strcpy(act->psapadd,gsu_out_vect[i].value+strlen(gsu_psap_name));
	       }
	    }
	 if (step7_mkflg != 1)
	    {
	    fprintf(stderr, "ERROR\n");
	    dce_fprintf(stderr, gsu_missing_psap_name, act->dsaname);
	    gsu_dce_printf_string=(char *)dce_sprintf(gsu_missing_psap_name,act->dsaname);
	    gsu_listing(gsu_dce_printf_string, NULL, NULL);
	    gsu_exit();
	    }
	 }

      gsu_listing("gsu_read_partname()",NULL,NULL);
      step7_counter=gsu_read_partname(0);
      sprintf(step7_dirid_string, "%d", step7_counter);
      gsu_listing("     return value = ",step7_dirid_string,NULL);
      gsu_listing("gsu_create_sh_dsa()",NULL,NULL);
      gsu_create_sh_dsa(gsu_locdsa, step7_counter);
      }
   act=act->next;
   };
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_step8                                      */
/*  Description   :  Local DSA (Shadow) -> other DSA's              */
/*  Date          :  November 1993                                  */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
void gsu_step8(void)
{
signed16 i;
int ret_val;
unsigned16 step8_mkflg;
unsigned16 step8_counter;
unsigned16 step8_counter1;
unsigned16 vect_counter;
gsu_dsa *step8_act=NULL;
gc_t_vector * step8_out_vect=NULL;
char step8_dirid_string[GSU_LENINT]="\0";

if (anfang1->dsaname == NULL)
   return;

/*   bind to local DSA   */
gsu_alloc(4);
gsu_in_vect[0] = (char *)gsu_attr_dirid;
sprintf(step8_dirid_string, "%d", gsu_dirid);
gsu_in_vect[1] = step8_dirid_string;
gsu_in_vect[2] = (char *)gsu_attr_dsa;
gsu_in_vect[3] = gsu_locdsa;
gsu_listing_malloc("gc_t_bind", 4);
gsu_bind(4, gsu_in_vect);
free(gsu_in_vect);

/*   modify the service control settings   */
gsu_default_control_service();
gsu_preferadminfkt_fkt();

/*   read local DSA   */
gsu_alloc(1);
gsu_in_vect[0] = gsu_locdsa;
gsu_listing_malloc("gc_t_read", 1);
if ((gc_t_read(gsu_in_vect, 1, TRUE, TRUE, &step8_out_vect)) != GC_SUCCESS)
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "gc_t_read %s\n", gsu_in_vect[i]);
   fprintf(stderr, "    %s\n", step8_out_vect[0].value);
   gsu_listing(step8_out_vect[0].value, NULL, NULL);
   gc_t_free_result(step8_out_vect);
   free(gsu_in_vect);
   gsu_exit();
   }
free(gsu_in_vect);

/*   get number of attributes from local DSA   */
step8_mkflg=0;
for(i=0; step8_out_vect[i].type != GC_T_EOL; i++)
   {
   if (strncmp(step8_out_vect[i].value, gsu_mk,(size_t)strlen(gsu_mk)) == 0)
      step8_mkflg=1;
   }
step8_counter = i;
if (step8_mkflg == 0)
   {
   dce_fprintf(stderr,gsu_has_not_mk, step8_out_vect[0].value);
   }

/*  create attributes from local DSA in all remote DSA's   */
step8_act=anfang1;    /*  additional remote DSA list  */
while (step8_act != NULL)
   {
   gsu_listing("----- step8_act = ", step8_act->dsaname, NULL);
   if ((strcmp(step8_act->dsaname,gsu_contdsa))&&(strcmp(step8_act->dsaname,gsu_locdsa)))
      {
      if (step8_act->psapadd[0]=='\0')
	 {
         /*   search PSAP from DSA in contact DSA   */
         gsu_listing("gsu_search_psap_dsa",NULL,NULL);
         gsu_search_psap_dsa(step8_act);

         /*   modify the service control settings   */
         gsu_default_control_service();
         gsu_preferadminfkt_fkt();
	 }
   
      /*  bind to additional remote DSA   */
      gsu_alloc(4);
      gsu_in_vect[0] = (char *)gsu_attr_dirid;
      sprintf(step8_dirid_string, "%d", gsu_dirid);
      gsu_in_vect[1] = step8_dirid_string;
      gsu_in_vect[2] = (char *)gsu_attr_psap;
      gsu_in_vect[3] = step8_act->psapadd;
      gsu_listing_malloc("gc_t_bind", 4);
      gsu_bind(4, gsu_in_vect);
      free(gsu_in_vect);

      /*  modify service controls */
      gsu_dontusecopy_false_fkt();
      gsu_localscope_true_fkt();
   
      /*read local DSA name from additional remote DSA   */
      /*if it doesn'n exist: add existing parts as shadows and rest as master*/
      gsu_alloc(1);
      gsu_in_vect[0] = gsu_locdsa;
      gsu_listing("gc_t_read ",gsu_in_vect[0],NULL);
      if ((ret_val=gc_t_read(gsu_in_vect,1,TRUE,TRUE,&gsu_out_vect)) != GC_SUCCESS)
          /***  local DSA name doesn't exist   ***/
          {
          if (ret_val == GC_T_DS_E_NO_SUCH_OBJECT)
             {
             free(gsu_in_vect);
	     gsu_listing("     return value = ",gsu_out_vect[1].value,NULL);
	     gsu_listing("gsu_read_partname()", NULL, NULL);
             step8_counter1=gsu_read_partname(1);
             sprintf(step8_dirid_string, "%d", step8_counter1);
	     gsu_listing("     return value = ", step8_dirid_string, NULL);
	     gsu_listing("gsu_create_sh_contdsa()",NULL,NULL);
             gsu_create_sh_contdsa(0,step8_counter1,gsu_locdsa);
             }
          else
             {
             fprintf(stderr, "ERROR\n");
	     fprintf(stderr, "gc_t_read %s\n", gsu_in_vect[0]);
             fprintf(stderr, "    %s\n",  gsu_out_vect[0].value);
             gsu_listing(gsu_out_vect[0].value, NULL, NULL);
             gc_t_free_result(gsu_out_vect);
             free(gsu_in_vect);
             gsu_exit();
             }

          /*   create all attributes from local DSA in remote DSA   */
          gsu_alloc(step8_counter+2);
          gsu_in_vect[0] = step8_out_vect[0].value;
          gsu_in_vect[1] = (char *)gsu_attribute;
          i=1;
          vect_counter = 2;
          while (step8_out_vect[i].type != GC_T_EOL)
             {
             if (gsu_attribute_value_delete(step8_out_vect[0].value,step8_out_vect[i].value)==0)
                {
                gsu_in_vect[vect_counter] = step8_out_vect[i].value;
                vect_counter++;
                }
             i++;
             };
          gsu_create(gsu_in_vect, vect_counter, &gsu_out_vect);
          free(gsu_in_vect);
          }
      else
          {
          /***  remote DSA name already exists in contact DSA  ***/   
	  gsu_listing("     return value = ",gsu_out_vect[0].value,NULL);
          gc_t_free_result(gsu_out_vect);
          free(gsu_in_vect);
          }
      }
   step8_act=step8_act->next;
   };
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_step9                                      */
/*  Description   :  First Level DSA actions                        */
/*  Date          :  November 1993                                  */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
void gsu_step9(void)
{
signed16 i;
signed16 j;
signed16 step9_counter=0;
gc_t_vector *step9_frst_level=NULL;
gc_t_vector *step9_my_frst_level=NULL;
char step9_dirid_string[GSU_LENINT]="\0";
char *step9_other_mk=NULL;

if (first_level_flg == 0)
   return;

/*   modify the service control settings   */
gsu_default_control_service();
gsu_preferadminfkt_fkt();

/*   bind to contact DSA   */
gsu_alloc(4);
gsu_in_vect[0] = (char *)gsu_attr_dirid;
sprintf(step9_dirid_string, "%d", gsu_dirid);
gsu_in_vect[1] = step9_dirid_string;
gsu_in_vect[2] = (char *)gsu_attr_dsa;
gsu_in_vect[3] = gsu_contdsa;
gsu_listing_malloc("gc_t_bind", 4);
gsu_bind(4, gsu_in_vect);
free(gsu_in_vect);

/*   modify the service control settings   */
gsu_dontusecopy_false_fkt();
gsu_localscope_true_fkt();

/* search first level objects in contact DSA and save it in step9_frst_level */
gsu_alloc(3);
gsu_in_vect[0] = (char *)gsu_first_level;
gsu_in_vect[1] = (char *)gsu_onelevel;
gsu_in_vect[2] = (char *)gsu_allattr;
gsu_listing_malloc("gc_t_search", 3);
if (gc_t_search(gsu_in_vect, 3, TRUE, TRUE, &step9_frst_level) != GC_SUCCESS)
   {
   fprintf(stderr, "ERROR\n gc_t_search ");
   for(i=0; i<3; i++)
      {
      fprintf(stderr, "%s ", gsu_in_vect[i]);
      }
   fprintf(stderr, "\n");
   fprintf(stderr, "    %s\n", step9_frst_level[0].value);
   gsu_listing(step9_frst_level[0].value, NULL, NULL);
   gc_t_free_result(step9_frst_level);
   free(gsu_in_vect);
   gsu_exit();
   }
free(gsu_in_vect);

gsu_listing("    return value :", NULL, NULL);
for (i=0; step9_frst_level[i].type != GC_T_EOL; i++)
   {
   if (step9_frst_level[i].type == GC_T_DN)
      gsu_listing("     ",step9_frst_level[i].value,NULL);
   }

/*   modify the service control settings   */
gsu_default_control_service();
gsu_preferadminfkt_fkt();

/*   bind to local DSA   */
gsu_alloc(4);
gsu_in_vect[0] = (char *)gsu_attr_dirid;
sprintf(step9_dirid_string, "%d", gsu_dirid);
gsu_in_vect[1] = step9_dirid_string;
gsu_in_vect[2] = (char *)gsu_attr_dsa;
gsu_in_vect[3] = gsu_locdsa;
gsu_listing_malloc("gc_t_bind", 4);
gsu_bind(4, gsu_in_vect);
free(gsu_in_vect);

/*   modify the service control settings   */
gsu_dontusecopy_false_fkt();
gsu_localscope_true_fkt();

/*   read first level object(country) from local DSA  */
/*   and save it in step9_my_frst_level   */
gsu_alloc(1);
gsu_in_vect[0] = gsu_locdsa_country;
gsu_listing_malloc("gc_t_read", 1);
if (gc_t_read(gsu_in_vect, 1, TRUE, TRUE, &step9_my_frst_level) != GC_SUCCESS)
   {
   fprintf(stderr, "ERROR\n gc_t_read ");
   for(i=0; i<1; i++)
      {
      fprintf(stderr, "%s ", gsu_in_vect[i]);
      }
   fprintf(stderr, "\n");
   fprintf(stderr, "    %s\n", step9_my_frst_level[0].value);
   gsu_listing(step9_my_frst_level[0].value, NULL, NULL);
   gc_t_free_result(step9_my_frst_level);
   free(gsu_in_vect);
   gsu_exit();
   }
free(gsu_in_vect);

gsu_listing("    return value :", NULL, NULL);
for (i=0; step9_my_frst_level[i].type != GC_T_EOL; i++)
   {
   if (step9_my_frst_level[i].type == GC_T_DN)
      gsu_listing("     ",step9_my_frst_level[i].value,NULL);
   }

/* copy other first level objects in my local-DSA and extend remote-dsa-liste */
i=1;
while (step9_frst_level[i].type != GC_T_EOL)
   {
   if ((step9_frst_level[i].type == GC_T_DN)&&(strcmp(gsu_locdsa_country,step9_frst_level[i].value))&&(strcmp(step9_frst_level[i].value,"/CN=Schema")))
      {
      gsu_listing("gsu_num_of_frst_obj()",NULL,NULL);
      step9_counter = gsu_num_of_frst_obj(&step9_frst_level[i]);
      sprintf(step9_dirid_string, "%d", step9_counter);
      gsu_listing("     return value = ", step9_dirid_string, NULL);
      gsu_alloc(step9_counter+1);
      gsu_in_vect[0] = step9_frst_level[i].value;
      step9_counter=i;
      i++; 
      if (step9_counter > 1)   
	 {
         gsu_in_vect[1] = (char *)gsu_attribute;
         j = 2;
         while ((step9_frst_level[i].type != GC_T_DN) && (step9_frst_level[i].type != GC_T_EOL))
            {
            if (gsu_attribute_value_delete(step9_frst_level[step9_counter].value,step9_frst_level[i].value)==0)
	       {
               gsu_in_vect[j] = step9_frst_level[i].value;
               j++;
	       }
            i++;
            };
	 }
      else
         {
	 j=1;
         }
      gsu_create(gsu_in_vect, j, &gsu_out_vect);
      free(gsu_in_vect);
      }
   else
      i++;
   };
/***   end of copy other first level objects in local DSA  ***/

/***   copy my first level objects in other first level objects   ***/
i=0;
while (step9_frst_level[i].type != GC_T_EOL)
   {
   if ((step9_frst_level[i].type == GC_T_DN)&&(strcmp(step9_frst_level[i].value,"/CN=Schema"))&&(strcmp(step9_frst_level[i].value,gsu_locdsa_country)))
      {
      /* search of MK from other first level objects */
      j=i+1;
      while((step9_frst_level[j].type!=GC_T_EOL)&&(step9_frst_level[j].type != GC_T_DN))
	 {
         if((strncmp(step9_frst_level[j].value,gsu_mk,(size_t)strlen(gsu_mk))) == 0)
	    {
            if ((step9_other_mk=(char *)malloc(strlen(step9_frst_level[j].value)+1))==NULL)
               {
	       fprintf(stderr, "ERROR\n");
               dce_fprintf(stderr, gc_t_memory_insufficient);
	       step9_other_mk=(char *)dce_sprintf(gc_t_memory_insufficient);
	       gsu_listing(step9_other_mk,NULL,NULL);
               gsu_exit();
               }
	    strcpy(step9_other_mk,(step9_frst_level[j].value)+strlen(gsu_mk_name));
	    step9_other_mk[strlen(step9_other_mk)-1]='\0';
	    }
	 j++;
	 };

      /* bind to other first level objects  */ 
      gsu_alloc(4);
      gsu_in_vect[0] = (char *)gsu_attr_dirid;
      sprintf(step9_dirid_string, "%d",gsu_dirid);
      gsu_in_vect[1] = step9_dirid_string;
      gsu_in_vect[2] = (char *)gsu_attr_dsa;
      gsu_in_vect[3] = step9_other_mk;
      gsu_listing_malloc("gc_t_bind", 4);
      gsu_bind(4, gsu_in_vect);
      free(gsu_in_vect);

      /* create all my first level objects in other first level objects */
      gsu_listing("gsu_create_myfrst()",NULL,NULL);
      gsu_create_myfrst(step9_my_frst_level);
      }
   i++;
   };
gc_t_free_result(step9_frst_level);
gc_t_free_result(step9_my_frst_level);
/***   end of copy my first level objects in other first level objects ***/
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_bind                                       */
/*  Description   :  bind to gsu_bind_dsaname                       */
/*  Input Value   :  unsigned16 number of bind_in_vect's            */
/*                   char ** to bind_in_vect                        */
/*  Date          :  November 1993                                  */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static void gsu_bind(
	    signed16 bind_counter,   /* IN - input variable */
	    char **bind_in_vect)     /* OUT - input variable */
{
signed16 i;

if (gc_t_bind(bind_in_vect, bind_counter, TRUE, &gsu_out_vect) != GC_SUCCESS)
    {
    fprintf(stderr, "ERROR\n gc_t_bind ");
    for(i=0; i<bind_counter; i++)
       {
       fprintf(stderr, "%s ", bind_in_vect[i]);
       }
    fprintf(stderr, "\n");
    fprintf(stderr, "    %s\n", gsu_out_vect[0].value);
    gsu_listing(gsu_out_vect[0].value, NULL, NULL);
    gc_t_free_result(gsu_out_vect);
    free(bind_in_vect);
    gsu_exit();
    }
}

/***********************************************************************/
/*                                                                     */
/*  Function name  : gsu_alloc                                         */
/*  Description    : for malloc of gsu_in_vect                         */
/*  Input Value    : unsigned16 number of mallocs-in-vector's          */
/*  Date           : November 1993                                     */
/*  Author         : Eva Wilde, SNI BA NM123                           */
/*                                                                     */
/***********************************************************************/
static void gsu_alloc(
	    signed16 alloc_counter) /*IN - input variable */
{
char * alloc_poi=NULL;

if (alloc_counter > 0)
   {
   if ((gsu_in_vect = (char **)malloc(alloc_counter*sizeof(char *))) == NULL)
       {
       fprintf(stderr, "ERROR\n");
       dce_fprintf(stderr, gc_t_memory_insufficient);
       alloc_poi=(char *)dce_sprintf(gc_t_memory_insufficient);
       gsu_listing(alloc_poi,NULL,NULL);
       gsu_exit();
       }
   }
}

/***********************************************************************/
/*                                                                     */
/*  Function name  : gsu_create                                        */
/*  Description    : gc_t_create call                                  */
/*  Input Value    : char ** input vector for gc_t_create fkt's        */
/*                   unsigned16 counter for gc_t_create fkt's          */
/*                   gc_t_vector ** to output for gc_t_create fkt's    */
/*  Date           : November 1993                                     */
/*  Author         : Eva Wilde, SNI BA NM123                           */
/*                                                                     */
/***********************************************************************/
static void gsu_create(
	    char **create_in_vect,        /* IN - input variable */
	    signed16 create_vect_count,   /* IN - input variable */
	    gc_t_vector **create_out_vect)/* IN - input variable */
{
signed16 i;
int return_value;
char *create_poi=NULL;

gsu_listing_malloc("gc_t_create ", create_vect_count);
if ((return_value=gc_t_create(create_in_vect, create_vect_count, TRUE, create_out_vect)) != GC_SUCCESS)
    {
    if (return_value == GC_T_DS_E_ENTRY_EXISTS)
       {
       if (oflg == 1)
	  {
	  dce_fprintf(stderr,gsu_object_exists_overwrite, create_in_vect[0]);
	  create_poi=(char *)dce_sprintf(gsu_object_exists_overwrite, create_in_vect[0]);
	  gsu_listing(create_poi,NULL,NULL);
	  free(create_poi);
	  gsu_listing("Create new attribute :",NULL,NULL);
	  for (i=2; i<create_vect_count; i++)
	     gsu_listing(create_in_vect[i], NULL,NULL);
	  gsu_in_vect[1] = (char *)gsu_changeattr;
          gsu_listing_malloc("gc_t_modify ", create_vect_count);
	  if (gc_t_modify(create_in_vect,create_vect_count,TRUE,&gsu_out_vect) != GC_SUCCESS)
	     {
	     fprintf(stderr, "ERROR\n gc_t_modify ");
	     for(i=0; i<create_vect_count; i++)
		{
		fprintf(stderr, "%s ",create_in_vect[i]);
		}
	     fprintf(stderr, "\n");
	     fprintf(stderr, "    %s\n",gsu_out_vect[0].value);
             gsu_listing(gsu_out_vect[0].value, NULL, NULL);
	     gc_t_free_result(gsu_out_vect);
	     free(create_in_vect);
	     gsu_exit();
	     }
	  }
       else
	  {
	  dce_fprintf(stderr,gsu_object_exists_no_overwrite,create_in_vect[0]);
	  create_poi=(char *)dce_sprintf(gsu_object_exists_no_overwrite,create_in_vect[0]);
	  gsu_listing(create_poi, NULL, NULL);
	  free(create_poi);
	  }
       }
    else
       {
       fprintf(stderr, "ERROR\n gc_t_modify");
       for(i=0; i<create_vect_count; i++)
	  {
	  fprintf(stderr, "%s ",create_in_vect[i]);
	  }
       fprintf(stderr, "\n");
       fprintf(stderr, "%s\n", create_out_vect[0]->value);
       gsu_listing(create_out_vect[0]->value, NULL, NULL);
       gc_t_free_result(*create_out_vect);
       gsu_exit();
       }
    }
}

/***********************************************************************/
/*                                                                     */
/*  Function name  : gsu_add_defdsa_cache                              */
/*  Description    : Other Default-DSA -> DUA-Cache                    */
/*  Input Value    : char * to DSA name                                */
/*                   char * to PSAP                                    */
/*  Date           : November 1993                                     */
/*  Author         : Eva Wilde, SNI BA NM123                           */
/*                                                                     */
/***********************************************************************/
static void gsu_add_defdsa_cache(
	    char * gsu_def_dsaname,  /* IN - Input variable */
	    char * gsu_def_psapadd)  /* IN - Input variable */
{
int i;
int ret_val;
char * defdsa_hilf1=NULL;
char * defdsa_hilf2=NULL;

/*   read sea-also attributes from Cache   */
gsu_alloc(4);
gsu_in_vect[0] = (char *)gsu_def_dsa_id;
gsu_in_vect[1] = (char *)gsu_attribute;
gsu_in_vect[2] = (char *)gsu_sea;
gsu_listing_malloc("gc_t_read", 3);
if ((ret_val = gc_t_read(gsu_in_vect,3,TRUE,TRUE,&gsu_out_vect)) != GC_SUCCESS)
    /***   DEFAULT-DSA doesn't exist   ***/
    {
    if (ret_val == GC_T_DS_E_NO_SUCH_OBJECT)
        /***   create neu SEA-ALSO attribute   ***/
       {
       gc_t_free_result(gsu_out_vect);
       if ((defdsa_hilf1=(char *)malloc(strlen(gsu_sea_name)+strlen("=}")+strlen(gsu_def_dsaname)+1))==NULL)
          {
	  fprintf(stderr, "ERROR\n");
          dce_fprintf(stderr, gc_t_memory_insufficient);
	  defdsa_hilf1=(char *)dce_sprintf(gc_t_memory_insufficient);
	  gsu_listing(defdsa_hilf1,NULL,NULL);
	  gsu_exit();
          }
       strcpy(defdsa_hilf1, gsu_sea_name);
       strcat(defdsa_hilf1, gsu_def_dsaname);
       strcat(defdsa_hilf1, "}");
       gsu_in_vect[2] = defdsa_hilf1;
       gsu_create(gsu_in_vect, 3, &gsu_out_vect);
       free(gsu_in_vect);
       free(defdsa_hilf1);

        /*   create PSAP address from gsu_def_dsaname   */
       gsu_alloc(4);
       gsu_in_vect[0] = (char *)gsu_def_dsaname;
       gsu_in_vect[1] = (char *)gsu_attribute;
       if ((defdsa_hilf1=(char *)malloc(strlen(gsu_psap_name) + strlen("}")+strlen(gsu_def_psapadd)+1))==NULL)
          {
	  fprintf(stderr, "ERROR\n");
          dce_fprintf(stderr, gc_t_memory_insufficient);
	  defdsa_hilf1=(char *)dce_sprintf(gc_t_memory_insufficient);
	  gsu_listing(defdsa_hilf1,NULL,NULL);
	  gsu_exit();
          }
       strcpy(defdsa_hilf1 , gsu_psap_name);
       strcat(defdsa_hilf1 , gsu_def_psapadd);
       strcat(defdsa_hilf1 , "}");
       gsu_in_vect[2] = defdsa_hilf1;
       gsu_in_vect[3] = (char *)gsu_ocl;
       gsu_create(gsu_in_vect, 4, &gsu_out_vect);
       free(gsu_in_vect);
       free(defdsa_hilf1);
       }
    else
       {
       fprintf(stderr, "ERROR\n gc_t_read ");
       for(i=0; i<3; i++)
	  {
	  fprintf(stderr, "%s ", gsu_in_vect[i]);
	  }
       fprintf(stderr, "\n");
       fprintf(stderr, "   %s\n",  gsu_out_vect[0].value);
       gsu_listing(gsu_out_vect[0].value, NULL, NULL);
       gc_t_free_result(gsu_out_vect);
       free(gsu_in_vect);
       gsu_exit();
       }
    }
else
    {
    free(gsu_in_vect);
    /***   SEA-ALSO attribute exists   ***/   
    if (strstr(gsu_out_vect[1].value,gsu_def_dsaname) == NULL)
       /***   create the aktuell def-DSA as SEA-ALSO attribute   ***/
       {
       if ((defdsa_hilf2=(char *)malloc(strlen(gsu_out_vect[1].value)+strlen("{};")+strlen(gsu_def_dsaname)+3)) == NULL)
          {
	  fprintf(stderr, "ERROR\n");
          dce_fprintf(stderr, gc_t_memory_insufficient);
	  defdsa_hilf2=(char *)dce_sprintf(gc_t_memory_insufficient);
	  gsu_listing(defdsa_hilf2,NULL,NULL);
	  gsu_exit();
          }
       strcpy(defdsa_hilf2, gsu_out_vect[1].value);
       strcat(defdsa_hilf2, ";{");
       strcat(defdsa_hilf2, gsu_def_dsaname);
       strcat(defdsa_hilf2, "}");
       if ((defdsa_hilf1=(char *)malloc(strlen(gsu_out_vect[0].value)+1))==NULL)
          {
	  fprintf(stderr, "ERROR\n");
          dce_fprintf(stderr, gc_t_memory_insufficient);
	  defdsa_hilf1=(char *)dce_sprintf(gc_t_memory_insufficient);
	  gsu_listing(defdsa_hilf1,NULL,NULL);
	  gsu_exit();
          }
       strcpy(defdsa_hilf1, gsu_out_vect[0].value);
       gc_t_free_result(gsu_out_vect);
       gsu_alloc(3);
       gsu_in_vect[0] = defdsa_hilf1;
       gsu_in_vect[1] = (char *)gsu_attribute;
       gsu_in_vect[2] = defdsa_hilf2;
       gsu_create(gsu_in_vect, 3, &gsu_out_vect);
       free(gsu_in_vect);
       free(defdsa_hilf1);
       free(defdsa_hilf2);

       /*   create PSAP address from gsu_def_dsaname   */
       gsu_alloc(4);
       gsu_in_vect[0] = (char *)gsu_def_dsaname;
       gsu_in_vect[1] = (char *)gsu_attribute;
       if ((defdsa_hilf1=(char *)malloc(strlen(gsu_psap_name)+strlen("}")+strlen(gsu_def_psapadd)+1))==NULL)
          {
	  fprintf(stderr, "ERROR\n");
          dce_fprintf(stderr, gc_t_memory_insufficient);
	  defdsa_hilf1=(char *)dce_sprintf(gc_t_memory_insufficient);
	  gsu_listing(defdsa_hilf1,NULL,NULL);
	  gsu_exit();
          }
       strcpy(defdsa_hilf1 , gsu_psap_name);
       strcat(defdsa_hilf1 , gsu_def_psapadd);
       strcat(defdsa_hilf1 , "}");
       gsu_in_vect[2] = defdsa_hilf1;
       gsu_in_vect[3] = (char *)gsu_ocl;
       gsu_create(gsu_in_vect, 4, &gsu_out_vect);
       free(gsu_in_vect);
       free(defdsa_hilf1);
       }
     else
       {
       /***  default dsa exists in cache as SEA-ALSO attribute   ***/
       dce_fprintf(stderr,gsu_object_in_cache, gsu_def_dsaname);
       gc_t_free_result(gsu_out_vect);
       return;
       }
    }
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_read_partname                              */
/*  Description   :  save of resolved part names                    */
/*  Input Value   :  unsigned8 number of DSA-name-parts             */
/*  Date          :  Dezember 1993                                  */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static unsigned16 gsu_read_partname(unsigned8 partname_counter)
{
signed16 i=0;
signed16 j=0;
signed16 z=0;
char *partname_poi_dn=NULL;   /*  for pointer to the distinguished name  */

/***   read resolved-name-part-object   ***/
if (((gsu_out_vect[partname_counter].type) == GC_T_RESOLVED_DN)||((gsu_out_vect[partname_counter].type) == GC_T_DN))
   {
   if ((partname_poi_dn=(char *)malloc(strlen(gsu_out_vect[partname_counter].value)+1))==NULL)
      {
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr, gc_t_memory_insufficient);
      partname_poi_dn=(char *)dce_sprintf(gc_t_memory_insufficient);
      gsu_listing(partname_poi_dn,NULL,NULL);
      gsu_exit();
      }
   strcpy(partname_poi_dn,gsu_out_vect[partname_counter].value);
   gc_t_free_result(gsu_out_vect);
   gsu_alloc(3);
   gsu_in_vect[1] = (char *)gsu_baseobject;
   gsu_in_vect[2] = (char *)gsu_allattr;
   while (partname_poi_dn[i] != '\0')
      {
      if (partname_poi_dn[i] == '/')
          {
          do {
	     i++;
             }while((partname_poi_dn[i] != '/')&&(partname_poi_dn[i] != '\0'));
	  if (i == 1)
	     {
	     first_level_flg = 1;
	     z=0;
	     do {
		gsu_locdsa_country[z]=gsu_locdsa[z];
		z++;
		} while ((gsu_locdsa[z] != '/')&&(z < GSU_COUNTRY));
	     gsu_locdsa_country[z] = '\0';
	     return(0);
	     }
	  else
	     {
             if ((distname[j]=(char *)malloc(i+2)) == NULL)
	        {
		fprintf(stderr, "ERROR\n");
                dce_fprintf(stderr, gc_t_memory_insufficient);
		distname[j]=(char *)dce_sprintf(gc_t_memory_insufficient);
		gsu_listing(distname[j],NULL,NULL);
	        gsu_exit();
	        }
             strncpy(distname[j], partname_poi_dn, i);
             distname[j][i]='\0';
	     }
          gsu_in_vect[0] = distname[j];
          gsu_listing_malloc("gc_t_search", 3);
          if (gc_t_search(gsu_in_vect,3,TRUE,TRUE,&respart[j]) != GC_SUCCESS)
	    {
	    fprintf(stderr, "ERROR\n gc_t_search ");
	    for(i=0; i<3; i++)
	       {
	       fprintf(stderr, "%s ",gsu_in_vect[i]);
	       }
	    fprintf(stderr, "\n");
	    fprintf(stderr, "   %s\n",  respart[j][0].value);
            gsu_listing(respart[j][0].value, NULL, NULL);
	    gc_t_free_result(respart[j]);
	    free(gsu_in_vect);
	    gsu_exit();
	    }
          j++;
          }
      };
   free(gsu_in_vect);
   free(partname_poi_dn);
   }
else
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "gc_t_read %s\n", gsu_in_vect[0]);
   fprintf(stderr, "   %s\n", gsu_out_vect[0].value);
   gsu_listing(gsu_out_vect[0].value, NULL, NULL);
   gc_t_free_result(gsu_out_vect);
   gsu_exit();
   }
return(j);
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_create_sh_contdsa                          */
/*  Description   :  Create shadows-objects in DSA                  */
/*  Input Value   :  unsigned16 flag for password                   */
/*                   unsigned16 number of objects                   */
/*                   char * DSA name                                */
/*  Date          :  Dezember 1993                                  */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static void gsu_create_sh_contdsa(
	    unsigned16 flgpsw,		/* IN - input variable */
	    unsigned16 anz,  		/* IN - input variable */
	    char *create_sh_dsa)	/* IN - input variable */
{
signed16 i;
unsigned16 create_sh_counter=2;
unsigned16 create_sh_ocl_counter;
char *create_sh_contdsa_poi=NULL;
char *poi=NULL;
char *create_sh_mk=NULL;
char *create_sh_name=NULL;
char *create_sh_pswd=NULL;
char *create_sh_psap=NULL;
char *create_sh_secur=NULL;
char *create_sh_princ=NULL;

/*   modify the service control settings   */
gsu_default_control_service();
gsu_dontusecopy_false_fkt();
gsu_localscope_true_fkt();

/*   create shadows in DSA */
if ((poi=(char *)malloc(strlen(create_sh_dsa)+1))==NULL)
   {
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gc_t_memory_insufficient);
   poi=(char *)dce_sprintf(gc_t_memory_insufficient);
   gsu_listing(poi,NULL,NULL);
   gsu_exit();
   }
strcpy(poi,create_sh_dsa);
if (anz == 0)
   {
   i = 0;
   if ((create_sh_contdsa_poi=(char *)malloc(strlen(gsu_locdsa)+1))==NULL)
      {
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr, gc_t_memory_insufficient);
      create_sh_contdsa_poi=(char *)dce_sprintf(gc_t_memory_insufficient);
      gsu_listing(create_sh_contdsa_poi,NULL,NULL);
      gsu_exit();
      }
   strcpy(create_sh_contdsa_poi,gsu_locdsa);
   }
else
   {
   i=(int)strlen(distname[anz-1]);
   if ((create_sh_contdsa_poi=(char *)malloc(strlen(distname[anz-1])+1))==NULL)
      {
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr, gc_t_memory_insufficient);
      create_sh_contdsa_poi=(char *)dce_sprintf(gc_t_memory_insufficient);
      gsu_listing(create_sh_contdsa_poi,NULL,NULL);
      gsu_exit();
      }
   strcpy(create_sh_contdsa_poi,distname[anz-1]);
   }
create_sh_ocl_counter = anz;

gsu_alloc(8);
gsu_in_vect[1] = (char *)gsu_attribute;
if ((create_sh_mk=(char *)malloc(strlen(gsu_mk_name)+strlen(create_sh_dsa)+strlen("}")+1)) == NULL)
   {
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gc_t_memory_insufficient);
   create_sh_mk=(char *)dce_sprintf(gc_t_memory_insufficient);
   gsu_listing(create_sh_mk,NULL,NULL);
   gsu_exit();
   }
strcpy(create_sh_mk, gsu_mk_name);
strcat(create_sh_mk, create_sh_dsa);
strcat(create_sh_mk, "}");
gsu_in_vect[create_sh_counter++] = create_sh_mk;
if (strstr(poi, create_sh_contdsa_poi) != NULL)
   {
   while (poi[i] != '\0')
      {
      if (poi[i] == '/')
        {
         do {
            i++;
            } while ((poi[i] != '\0')&&(poi[i] != '\n')&&(poi[i] != '/'));
	 if ((create_sh_name=(char *)malloc(i+2))==NULL)
	    {
	    fprintf(stderr, "ERROR\n");
            dce_fprintf(stderr, gc_t_memory_insufficient);
	    create_sh_name=(char *)dce_sprintf(gc_t_memory_insufficient);
	    gsu_listing(create_sh_name,NULL,NULL);
	    gsu_exit();
	    }
         strncpy(create_sh_name, poi, i);
         create_sh_name[i]='\0';
         gsu_in_vect[0] = create_sh_name;
         if (((poi[i] == '\0')||(poi[i] == '\n'))&&(flgpsw ==1))
            {
   	    if ((create_sh_pswd=(char *)malloc(strlen(gsu_pswd_name)+strlen(gsu_userpsw)+1))==NULL)
	       {
	       fprintf(stderr, "ERROR\n"),
               dce_fprintf(stderr, gc_t_memory_insufficient);
	       create_sh_pswd=(char *)dce_sprintf(gc_t_memory_insufficient);
	       gsu_listing(create_sh_pswd,NULL,NULL);
	       gsu_exit();
	       }
	    if ((create_sh_psap=(char *)malloc(strlen(gsu_psap_name)+strlen(gsu_psaploc)+strlen("}")+1))==NULL)
	       {
	       fprintf(stderr, "ERROR\n");
               dce_fprintf(stderr, gc_t_memory_insufficient);
	       create_sh_psap=(char *)dce_sprintf(gc_t_memory_insufficient);
	       gsu_listing(create_sh_psap,NULL,NULL);
	       gsu_exit();
	       }
	    if ((create_sh_secur=(char *)malloc(strlen(gsu_am)+sizeof(int)+1))==NULL)
	       {
	       fprintf(stderr, "ERROR\n");
               dce_fprintf(stderr, gc_t_memory_insufficient);
	       create_sh_secur=(char *)dce_sprintf(gc_t_memory_insufficient);
	       gsu_listing(create_sh_secur,NULL,NULL);
	       gsu_exit();
	       }
	    if ((create_sh_princ=(char *)malloc(strlen(gsu_pn)+strlen(gsu_princ)+1))==NULL)
	       {
	       fprintf(stderr, "ERROR\n");
               dce_fprintf(stderr, gc_t_memory_insufficient);
	       create_sh_princ=(char *)dce_sprintf(gc_t_memory_insufficient);
	       gsu_listing(create_sh_princ,NULL,NULL);
	       gsu_exit();
	       }
            strcpy(create_sh_psap, gsu_psap_name);
            strcat(create_sh_psap, gsu_psaploc);
            strcat(create_sh_psap, "}");
	    gsu_in_vect[create_sh_counter++] = create_sh_psap;
	    sprintf(create_sh_secur, "%s%d",gsu_am,gsu_authent_mech);
	    gsu_in_vect[create_sh_counter++] = create_sh_secur;
            if (!strcmp(gsu_e_secur, gsu_secur_step_dce))
	       {
               strcpy(create_sh_princ, gsu_pn);
               strcat(create_sh_princ, gsu_princ);
	       gsu_in_vect[create_sh_counter++] = create_sh_princ;
	       }
	    if ((flgpsw == 1)&&(gsu_userpsw[0] != '\0'))
	       {
               strcpy(create_sh_pswd, gsu_pswd_name);
               strcat(create_sh_pswd, gsu_userpsw);
	       gsu_in_vect[create_sh_counter++] = create_sh_pswd;
	       }
	    }
	  else
	     {
	     if (((poi[i] == '\0')||(poi[i] == '\n'))&&(flgpsw ==0))
	     /***   REMOTE DSA's   ***/
		{
		if (((poi[i] == '\0')||(poi[i] == '\n'))&&(flgpsw ==1))
		   {
	           free(create_sh_name);
	           free(create_sh_pswd);
	           free(create_sh_psap);
	           free(create_sh_secur);
	           free(create_sh_princ);
		   }
		continue;
		}
	     }
	  gsu_in_vect[create_sh_counter++]=(char *)gsu_ocl_names[create_sh_ocl_counter++];
          gsu_create(gsu_in_vect,create_sh_counter,&gsu_out_vect);
	  free(create_sh_name);
	  create_sh_counter=3;
	  if (((poi[i] == '\0')||(poi[i] == '\n'))&&(flgpsw ==1))
	     {
	     free(create_sh_pswd);
	     free(create_sh_psap);
	     free(create_sh_secur);
	     free(create_sh_princ);
	     }
        }
      else
        {
	i++;
        }
      };
   }
else
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "gsu_create_sh_contdsa: %s not in %s\n",poi, create_sh_contdsa_poi);
   gsu_listing(poi, " not in ", create_sh_contdsa_poi);
   gsu_exit();
   }
free(gsu_in_vect);
free(create_sh_mk);
free(poi);
}

/********************************************************************/ 
/*                                                                  */
/*  Function name :  gsu_create_sh_dsa                              */
/*  Description   :  Create shadows-objects in dsa_dsaname          */
/*  Input Value   :  char * to DSA name                             */
/*                   unsigned16 number of dsa-name-parts            */
/*  Date          :  Dezember 1993                                  */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static void gsu_create_sh_dsa(
	    char *dsa_dsaname,        /*  IN - input variable */
	    signed16 anz)             /*  IN - input variable */
{
signed16 i;
signed16 j;
signed16 z;
unsigned16 dsa_dsaname_counter;
char dsaname_dirid_string[GSU_LENINT]="\0";

/*   bind to dsa_dsaname   */
gsu_alloc(4);
gsu_in_vect[0] = (char *)gsu_attr_dirid;
sprintf(dsaname_dirid_string, "%d", gsu_dirid);
gsu_in_vect[1] = dsaname_dirid_string;
gsu_in_vect[2] = (char *)gsu_attr_dsa;
gsu_in_vect[3] = dsa_dsaname;
gsu_listing_malloc("gc_t_bind", 4);
gsu_bind(4, gsu_in_vect);
free(gsu_in_vect);

/*   modify the service control settings   */
gsu_default_control_service();
gsu_preferadminfkt_fkt();
gsu_dontusecopy_false_fkt();
gsu_localscope_true_fkt();

/*   create shadows in distname[i]   */
i=0;
while (i < anz)
   {
   dsa_dsaname_counter=0;
   while(respart[i][dsa_dsaname_counter].type != GC_T_EOL)
      {
      dsa_dsaname_counter++;
      };
   gsu_alloc(dsa_dsaname_counter+1);
   gsu_in_vect[0] = respart[i][0].value;
   gsu_in_vect[1] = (char *)gsu_attribute;
   z=1;
   j=2;
   while(respart[i][z].type != GC_T_EOL)
      {
      if((strncmp(respart[i][z].value,"ACL=",(size_t)strlen("ACL=")))!= 0)
         {
         if (gsu_attribute_value_delete(respart[i][0].value,respart[i][z].value)==0)
	    {
            gsu_in_vect[j++] = respart[i][z].value;
	    }
       	 }
      if((strncmp(respart[i][z].value,gsu_mk,(size_t)strlen(gsu_mk)))== 0)
	 {
	 gsu_listing("gsu_extend_remdsa(",respart[i][z].value, ")");
         gsu_extend_remdsa(respart[i][z].value);
	 }
      z++;
      };
   gsu_create(gsu_in_vect, j, &gsu_out_vect);
   free(gsu_in_vect);
   gc_t_free_result(respart[i]);
   i++;
   };
return;
}

/********************************************************************/          
/*                                                                  */          
/*  Function name :  gsu_create_mk_dsaname                          */
/*  Description   :  Create MK-objects in gsu_create_mk_dsa         */
/*  Input Value   :  char * to DSA name                             */
/*                   unsigned16 number of dsa-name-parts            */
/*                   unsigned16 flag for password                   */
/*  Date          :  Dezember 1993                                  */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static void gsu_create_mk_dsaname(
	    char *gsu_create_mk_dsa,  /* IN - input variable */
	    unsigned16 anz,           /* IN - input variable */
	    unsigned16 pswflg)        /* IN - input variable */
{
signed16 i;
unsigned16 create_mk_counter;
unsigned16 create_mk_anz;
char *poi=NULL;
char *create_mk_pswd=NULL;
char *create_mk_psap=NULL;
char *create_mk_name=NULL;
char *create_mk_secur=NULL;
char *create_mk_princ=NULL;

/*   setting service control   */
gsu_default_control_service();
gsu_preferadminfkt_fkt();

if ((poi=(char *)malloc(strlen(gsu_create_mk_dsa)+1))==NULL)
   {
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gc_t_memory_insufficient);
   poi=(char *)dce_sprintf(gc_t_memory_insufficient);
   gsu_listing(poi,NULL,NULL);
   gsu_exit();
   }
strcpy(poi,gsu_create_mk_dsa);
if (anz != 0)
   {
   i=strlen(distname[anz-1]);
   }
else
   {
   i=0;
   }
create_mk_counter = anz;

while (poi[i] != '\0')
   {
   if (poi[i] == '/')
     {
      do {
         i++;
         } while ((poi[i] != '/')&&(poi[i] != '\0'));
      if ((create_mk_name=(char *)malloc(i+1))==NULL)
         {
	 fprintf(stderr, "ERROR\n");
         dce_fprintf(stderr, gc_t_memory_insufficient);
	 create_mk_name=(char *)dce_sprintf(gc_t_memory_insufficient);
	 gsu_listing(create_mk_name,NULL,NULL);
         gsu_exit();
         }
       strncpy(create_mk_name, poi, i);
       create_mk_name[i]='\0';
       if (poi[i] == '\0')
          {
          if ((create_mk_pswd=(char *)malloc(strlen(gsu_pswd_name)+strlen(gsu_userpsw)+1))==NULL)
             {
	     fprintf(stderr, "ERROR\n");
             dce_fprintf(stderr, gc_t_memory_insufficient);
	     create_mk_pswd=(char *)dce_sprintf(gc_t_memory_insufficient);
	     gsu_listing(create_mk_pswd,NULL,NULL);
             gsu_exit();
             }
          if ((create_mk_psap=(char *)malloc(strlen(gsu_psap_name)+strlen("}")+strlen(gsu_psaploc)+1))==NULL)
             {
	     fprintf(stderr, "ERROR\n");
             dce_fprintf(stderr, gc_t_memory_insufficient);
	     create_mk_psap=(char *)dce_sprintf(gc_t_memory_insufficient);
	     gsu_listing(create_mk_psap,NULL,NULL);
             gsu_exit();
             }
	  if ((create_mk_secur=(char *)malloc(strlen(gsu_am)+sizeof(int)+1))==NULL)
	     {
	     fprintf(stderr, "ERROR\n");
             dce_fprintf(stderr, gc_t_memory_insufficient);
	     create_mk_secur=(char *)dce_sprintf(gc_t_memory_insufficient);
	     gsu_listing(create_mk_secur,NULL,NULL);
             gsu_exit();
	     }
	  if ((create_mk_princ=(char *)malloc(strlen(gsu_pn)+strlen(gsu_princ)+1))==NULL)
	     {
	     fprintf(stderr, "ERROR\n");
             dce_fprintf(stderr, gc_t_memory_insufficient);
	     create_mk_princ=(char *)dce_sprintf(gc_t_memory_insufficient);
	     gsu_listing(create_mk_princ,NULL,NULL);
             gsu_exit();
	     }
          gsu_alloc(7);
	  create_mk_anz = 4;
          if ((pswflg == 1)&&(gsu_userpsw[0] != '\0'))
	     {
             strcpy(create_mk_pswd, gsu_pswd_name);
             strcat(create_mk_pswd, gsu_userpsw);
             gsu_in_vect[create_mk_anz++] = create_mk_pswd;
	     }
          strcpy(create_mk_psap, gsu_psap_name);
          strcat(create_mk_psap, gsu_psaploc);
          strcat(create_mk_psap, "}");
          gsu_in_vect[3] = create_mk_psap;
	  sprintf(create_mk_secur, "%s%d",gsu_am,gsu_authent_mech);
          gsu_in_vect[create_mk_anz++] = create_mk_secur;
	  if (!strcmp(gsu_e_secur,gsu_secur_step_dce))
	     {
             strcpy(create_mk_princ, gsu_pn);
             strcat(create_mk_princ, gsu_princ);
             gsu_in_vect[create_mk_anz++] = create_mk_princ;
	     }
	  }
       else
          {
          gsu_alloc(3);
	  create_mk_anz = 3; 
          }
     gsu_in_vect[0] = create_mk_name;
     gsu_in_vect[1] = (char *)gsu_attribute;
     gsu_in_vect[2] = (char *)gsu_ocl_names[create_mk_counter++];
     gsu_create(gsu_in_vect, create_mk_anz, &gsu_out_vect);
     free(gsu_in_vect);
     free(create_mk_name);
     if (poi[i] == '\0')
	{
        free(create_mk_psap);
        free(create_mk_pswd);
        free(create_mk_secur);
        free(create_mk_princ);
	}
     }
   };
free(poi);
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_extend_remdsa                              */
/*  Description   :  Extend remote-DSA-list                         */
/*  Input Value   :  char * to MK-attribute-list                    */
/*  Date          :  Dezember 1993                                  */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static void gsu_extend_remdsa(
	    char *extend_name)  /* IN - input variable */
{
size_t i;
unsigned16 flgrem;
char *extend_master=NULL;
gsu_dsa *act=NULL;
gsu_dsa *last=NULL;

if (!strcmp(extend_name, gsu_locdsa))
   {
   return;
   }

/* extend remote DSA liste   */
if ((strncmp(extend_name, gsu_mk_name, strlen(gsu_mk_name)))==0)
 {
 if ((extend_master=(char *)malloc(strlen(extend_name)+1))==NULL)
    {
    fprintf(stderr, "ERROR\n");
    dce_fprintf(stderr, gc_t_memory_insufficient);
    extend_master=(char *)dce_sprintf(gc_t_memory_insufficient);
    gsu_listing(extend_master,NULL,NULL);
    gsu_exit();
    }
 strcpy(extend_master,extend_name+strlen(gsu_mk_name)); 
 for (i=0; i<strlen(extend_master); i++)
    {
    if (extend_master[i] == '}')
       extend_master[i] = '\0';
    }
 act=anfang1;     /*  additional remote DSA liste  */
 flgrem=0;
 while (act != NULL)
    {
    if ((strcmp(act->dsaname,extend_master))==0)
       flgrem=1;
    last=act;
    act=act->next;
    };
 if ((flgrem == 0)&&(anfang1 != NULL))
    {
    if ((act=(gsu_dsa *)malloc(sizeof(gsu_dsa))) == NULL)
      {
      fprintf(stderr, "ERROR\n");
      dce_fprintf(stderr, gc_t_memory_insufficient);
      extend_master=(char *)dce_sprintf(gc_t_memory_insufficient);
      gsu_listing(extend_master,NULL,NULL);
      gsu_exit();
      }
    last->next=act;
    last=last->next;
    strcpy(last->dsaname,extend_master);
    last->psapadd[0]='\0';
    free(extend_master);
    last->next=NULL;
    }
 else
    {
    if ((flgrem == 0)&&(anfang1 == NULL))
       {
       if ((act=(gsu_dsa *)malloc(sizeof(gsu_dsa))) == NULL)
         {
	 fprintf(stderr, "ERROR\n");
         dce_fprintf(stderr, gc_t_memory_insufficient);
	 extend_master=(char *)dce_sprintf(gc_t_memory_insufficient);
	 gsu_listing(extend_master,NULL,NULL);
         gsu_exit();
         }
       strcpy(act->dsaname,extend_master);
       act->psapadd[0]='\0';
       act->next=NULL;
       anfang1=act;
       }
    free(extend_master);
    }
 }
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_num_of_frst_obj                            */
/*  Description   :  number of attributtes in the list              */
/*  Input Value   :  gc_t_vector * to first level objects liste     */
/*  Date          :  Januar 1994                                    */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static unsigned16 gsu_num_of_frst_obj(
		  gc_t_vector *gsu_frst_obj_liste) /*IN - input variable*/
{ 
signed16 i;
unsigned16 frst_mk_flg;

i=1;
frst_mk_flg=0;
while ((gsu_frst_obj_liste[i].type != GC_T_EOL) &&
       (gsu_frst_obj_liste[i].type != GC_T_DN))
   {
   if (!strncmp(gsu_frst_obj_liste[i].value,gsu_mk,strlen(gsu_mk)))
      {
      gsu_listing("gsu_extend_remdsa(",gsu_frst_obj_liste[i].value, ")");
      gsu_extend_remdsa(gsu_frst_obj_liste[i].value);
      frst_mk_flg=1;
      }
   i++;
   };
if (frst_mk_flg == 0)
   {
   dce_fprintf(stderr,gsu_has_not_mk, gsu_frst_obj_liste[0].value);
   }
return(i);
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_create_myfrst                              */
/*  Description   :  Create all attributes from myfrst_vector       */
/*  Input Value   :  gc_t_vector * to first level objects liste     */
/*  Date          :  Januar 1994                                    */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static void gsu_create_myfrst(
	    gc_t_vector * create_myfrst_vector) /* IN - pointer to list */
{
signed16 i;
signed16 j;
signed16 z;
unsigned16 counter;
signed16 create_myfrst_counter;
char create_myfrst_int_string[GSU_LENINT]="\0";

j=0;
while (create_myfrst_vector[j].type != GC_T_EOL)
 {
 if (create_myfrst_vector[j].type == GC_T_DN)
    {
    create_myfrst_counter = 0;
    gsu_listing("gsu_num_of_frst_obj(",create_myfrst_vector[j].value,")");
    create_myfrst_counter = gsu_num_of_frst_obj(&create_myfrst_vector[j]);
    sprintf(create_myfrst_int_string, "%d", create_myfrst_counter);
    gsu_listing("     return value = ",create_myfrst_int_string,NULL);
    if (create_myfrst_counter > 0)
       {
       gsu_alloc(create_myfrst_counter+1);
       gsu_in_vect[0] = create_myfrst_vector[j].value;
       counter=j;
       gsu_in_vect[1] = (char *)gsu_attribute;
       j++;
       i=2;
       for (z=2; z<(create_myfrst_counter+1); z++)
           {
           if (gsu_attribute_value_delete(create_myfrst_vector[counter].value,create_myfrst_vector[j].value)==0)
	      {
              gsu_in_vect[i++] = create_myfrst_vector[j].value;
	      }
	   j++;
           }
       gsu_create(gsu_in_vect, i, &gsu_out_vect);
       free(gsu_in_vect);
       }
    }
 else
    {
    j++;
    }
 };
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_exit                                       */
/*  Description   :  end of programm                                */
/*  Date          :  Januar 1994                                    */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static void gsu_exit(void)
{
fprintf(stderr, "ERROR\n");
gsu_listing("gc_t_unbind", NULL, NULL);
if (gc_t_unbind(&gsu_out_vect) != GC_SUCCESS)
   {
   fprintf(stderr, "unbind ERROR\n");
   fprintf(stderr, "   %s\n", gsu_out_vect[0].value);
   gsu_listing(gsu_out_vect[0].value, NULL, NULL);
   }
gsu_listing("K. O.", NULL, NULL);
exit(GSU_EXIT_126);
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_dsa_name_file                              */
/*  Date          :  Januar 1994                                    */
/*  Description   :  make of local DSA file                         */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static void gsu_dsa_name_file(void)
{
signed16 i=0;
signed16 j=0;
FILE *fdcm;
char *dsaname_file=NULL;
char *local_dsa_name=NULL;
char *local_dsa_poi=NULL;

/******************
/C=	85.4.6<D2_PRTBL_REP>
/O=     <D2_RDN_SEP>85.4.10<D2_T61_REP>
/OU=	<D2_RDN_SEP>85.4.11<D2_T61_REP>
/CN=	<D2_RDN_SEP>85.4.3<D2_T61_REP>
/CN=	<D2_RDN_SEP>85.4.3<D2_T61_REP>
*******************/

if ((local_dsa_name=(char *)malloc(strlen(gsu_locdsa)+1+4*sizeof(D2_RDN_SEP)+4*sizeof(D2_T61_REP)+sizeof(D2_PRTBL_REP)+5*strlen("85.4.11")))==NULL)
   {
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gc_t_memory_insufficient);
   local_dsa_poi=(char *)dce_sprintf(gc_t_memory_insufficient);
   gsu_listing(local_dsa_poi,NULL,NULL);
   gsu_exit();
   }
if ((local_dsa_poi=(char *)malloc(strlen(gsu_locdsa)+1))==NULL)
   {
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gc_t_memory_insufficient);
   local_dsa_poi=(char *)dce_sprintf(gc_t_memory_insufficient);
   gsu_listing(local_dsa_poi,NULL,NULL);
   gsu_exit();
   }
strcpy(local_dsa_poi,gsu_locdsa);
if (!strncmp(local_dsa_poi, "/C=", strlen("/C=")))
   {
   strcpy(local_dsa_name, "85.4.6");
   sprintf(local_dsa_name, "%s%c", local_dsa_name, D2_PRTBL_REP);
   i=(int)(strlen(local_dsa_name));
   j=(int)strlen("/C=");
   while ((local_dsa_poi[j] != '/')&&(local_dsa_poi[j] != '\0'))
      {
      local_dsa_name[i] = local_dsa_poi[j];
      i++;
      j++;
      };
   local_dsa_name[i] = '\0';
   }
else
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "gsu_dsa_name_file: error /C=\n");
   gsu_listing("gsu_dsa_name_file: error /C=", NULL, NULL);
   gsu_exit();
   }

if (!strncmp(local_dsa_poi+j, "/O=", strlen("/O=")))
   {
   sprintf(local_dsa_name, "%s%c", local_dsa_name, D2_RDN_SEP);
   strcat(local_dsa_name, "85.4.10");
   sprintf(local_dsa_name, "%s%c", local_dsa_name, D2_T61_REP);
   i=(int)strlen(local_dsa_name);
   j=j+strlen("/O=");
   while ((local_dsa_poi[j] != '/')&&(local_dsa_poi[j] != '\0'))
      {
      local_dsa_name[i] = local_dsa_poi[j];
      i++;
      j++;
      };
   local_dsa_name[i] = '\0';
   }
else
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "gsu_dsa_name_file: error /O=\n");
   gsu_listing("gsu_dsa_name_file: error /O=", NULL, NULL);
   gsu_exit();
   }

if (!strncmp(local_dsa_poi+j, "/OU=", strlen("/OU=")))
   {
   sprintf(local_dsa_name, "%s%c",local_dsa_name, D2_RDN_SEP);
   strcat(local_dsa_name, "85.4.11");
   sprintf(local_dsa_name, "%s%c",local_dsa_name, D2_T61_REP);
   i=(int)strlen(local_dsa_name);
   j=j+strlen("/OU=");
   while ((local_dsa_poi[j] != '/')&&(local_dsa_poi[j] != '\0'))
      {
      local_dsa_name[i] = local_dsa_poi[j];
      i++;
      j++;
      };
   local_dsa_name[i] = '\0';
   }
else
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "gsu_dsa_name_file: error /OU=\n");
   gsu_listing("gsu_dsa_name_file: error /OU=", NULL, NULL);
   gsu_exit();
   }

if (!strncmp(local_dsa_poi+j, "/CN=", strlen("/CN=")))
   {
   sprintf(local_dsa_name, "%s%c",local_dsa_name, D2_RDN_SEP);
   strcat(local_dsa_name, "85.4.3");
   sprintf(local_dsa_name, "%s%c",local_dsa_name, D2_T61_REP);
   i=(int)strlen(local_dsa_name);
   j=j+strlen("/CN=");
   while ((local_dsa_poi[j] != '/')&&(local_dsa_poi[j] != '\0'))
      {
      local_dsa_name[i] = local_dsa_poi[j];
      i++;
      j++;
      };
   local_dsa_name[i] = '\0';
   }
else
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "gsu_dsa_name_file: error /CN=\n");
   gsu_listing("gsu_dsa_name_file: error /CN=", NULL, NULL);
   gsu_exit();
   }

if (!strncmp(local_dsa_poi+j, "/CN=", strlen("/CN=")))
   {
   sprintf(local_dsa_name, "%s%c",local_dsa_name, D2_RDN_SEP);
   strcat(local_dsa_name, "85.4.3");
   sprintf(local_dsa_name, "%s%c",local_dsa_name, D2_T61_REP);
   i=(int)strlen(local_dsa_name);
   j=j+strlen("/CN=");
   while ((local_dsa_poi[j] != '/')&&(local_dsa_poi[j] != '\0'))
      {
      local_dsa_name[i] = local_dsa_poi[j];
      i++;
      j++;
      };
   local_dsa_name[i] = '\0';
   }
else
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "gsu_dsa_name_file: error /CN=\n");
   gsu_listing("gsu_dsa_name_file: error /CN=", NULL, NULL);
   gsu_exit();
   }

if ((dsaname_file = (char *)malloc(strlen(dcelocal_path)+strlen(D27_SERVER_DIR)+strlen(D27_DSA_DIR)+strlen(D27_SUBDSA_DIR)+strlen(D27_DSANAME)+5))==NULL)
    {
    fprintf(stderr, "ERROR\n");
    dce_fprintf(stderr, gc_t_memory_insufficient);
    dsaname_file=(char *)dce_sprintf(gc_t_memory_insufficient);
    gsu_listing(dsaname_file,NULL,NULL);
    gsu_exit();
    }
sprintf(dsaname_file, "%s%s%s%s%d%s", dcelocal_path, D27_SERVER_DIR, D27_DSA_DIR, D27_SUBDSA_DIR, gsu_dirid, D27_DSANAME);
if ((fdcm=fopen(dsaname_file, "w")) == NULL)
   {
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gsu_fopen_error,dsaname_file);
   gsu_dce_printf_string=(char *)dce_sprintf(gsu_fopen_error,dsaname_file);
   gsu_listing(gsu_dce_printf_string, NULL, NULL);
   gsu_exit();
   }
fprintf(fdcm, "%s", local_dsa_name);
if (!strcmp(gsu_e_secur, gsu_secur_step_dce))
   fprintf(fdcm, "\n%s", gsu_princ);
if (fclose(fdcm) != 0)
   {
   fprintf(stderr, "ERROR\n");
   dce_fprintf(stderr, gsu_fclose_error,"gsu_dsa_name_file");
   gsu_dce_printf_string=(char *)dce_sprintf(gsu_fclose_error,"gsu_dsa_name_file");
   gsu_listing(gsu_dce_printf_string, NULL, NULL);
   gsu_exit();
   }
free(local_dsa_poi);
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_service_control_cache                      */
/*  Date          :  Januar 1994                                    */
/*  Description   :  modify the service control                     */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static void gsu_service_control_cache(void)
{
int i;

/*   modify the service control settings   */
/*   preferadminfunctions - prefer administration functions   */
/*   duacache - use DUA cache   */
/*   duafirst - use DUA cache first   */
/*   usedsa - use DSA   */
/*   cacheclass  - to indicate the storyge clas of cache   */
gsu_alloc(12);
gsu_in_vect[0] = (char *)gsu_preferadminfkt;
gsu_in_vect[1] = (char *)gsu_true;
gsu_in_vect[2] = (char *)gsu_duacache;
gsu_in_vect[3] = (char *)gsu_true;
gsu_in_vect[4] = (char *)gsu_duafirst;
gsu_in_vect[5] = (char *)gsu_true;
gsu_in_vect[6] = (char *)gsu_usedsa;
gsu_in_vect[7] = (char *)gsu_false;
gsu_in_vect[8] = (char *)gsu_cacheclass;
gsu_in_vect[9] = (char *)gsu_resident;
gsu_in_vect[10] = (char *)gsu_dontusecopy;
gsu_in_vect[11] = (char *)gsu_false;
gsu_listing_malloc("gc_t_moddsc", 12);
if (gc_t_moddsc(gsu_in_vect, 12, &gsu_out_vect) != GC_SUCCESS)
   {
   fprintf(stderr, "ERROR\n gc_t_moddsc ");
   for(i=0; i<12; i++)
      {
      fprintf(stderr, "%s ", gsu_in_vect[i]);
      }
   fprintf(stderr, "\n");
   fprintf(stderr, "   %s\n", gsu_out_vect[0].value);
   gsu_listing(gsu_out_vect[0].value, NULL, NULL);
   gc_t_free_result(gsu_out_vect);
   free(gsu_in_vect);
   gsu_exit();
   }
free(gsu_in_vect);
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_default_control_service                    */
/*  Date          :  April  1994                                    */
/*  Description   :  Set the default control service                */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static void gsu_default_control_service(void)
{
gsu_alloc(1);
gsu_in_vect[0] = (char *)gsu_default;
gsu_listing_malloc("gc_t_moddsc", 1);
if (gc_t_moddsc(gsu_in_vect, 1, &gsu_out_vect) != GC_SUCCESS)
   {
   fprintf(stderr, "ERROR\n");
   fprintf(stderr, "gc_t_moddsc %s\n", gsu_in_vect[0]);
   fprintf(stderr, "   %s\n", gsu_out_vect[0].value);
   gsu_listing(gsu_out_vect[0].value, NULL, NULL);
   gc_t_free_result(gsu_out_vect);
   free(gsu_in_vect);
   gsu_exit();
   }
free(gsu_in_vect);
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_preferadminfkt_fkt                         */
/*  Date          :  April  1994                                    */
/*  Description   :  set preferadmin to true                        */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static void gsu_preferadminfkt_fkt(void)
{
int i;

gsu_alloc(2);
gsu_in_vect[0] = (char *)gsu_preferadminfkt;
gsu_in_vect[1] = (char *)gsu_true;
gsu_listing_malloc("gc_t_moddsc", 2);
if (gc_t_moddsc(gsu_in_vect, 2, &gsu_out_vect) != GC_SUCCESS)
   {
   fprintf(stderr, "ERROR\n gc_t_moddsc ");
   for(i=0; i<2; i++)
      {
      fprintf(stderr, "%s ", gsu_in_vect[i]);
      }
   fprintf(stderr, "\n");
   fprintf(stderr, "    %s\n", gsu_out_vect[0].value);
   gsu_listing(gsu_out_vect[0].value, NULL, NULL);
   gc_t_free_result(gsu_out_vect);
   free(gsu_in_vect);
   gsu_exit();
   }
free(gsu_in_vect);
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_listing_malloc                             */
/*  Description   :  if listing flag is setting, then write all     */
/*                   in listing-file                                */
/*  Input Value   :  char * to listing string                       */
/*                   int number of gsu_in_vect                      */
/*  Date          :  Januar 1994                                    */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static void gsu_listing_malloc(
	    char *listing_string,   /* IN - input variable */
	    signed16 listing_counter)    /* IN - input variable */
{
signed16 i;
unsigned16 lengh;
char * listing_malloc_poi=NULL;

if (listflg == 1)
   {
   for (i=0; i<listing_counter; i++)
      {
      if (i == 0)
         {
         if ((listing_malloc_poi=(char *)malloc(strlen(listing_string)+strlen(" ")+strlen(gsu_in_vect[0])+1))==NULL)
	    {
	    fprintf(stderr, "ERROR\n");
            dce_fprintf(stderr, gc_t_memory_insufficient);
	    listing_malloc_poi=(char *)dce_sprintf(gc_t_memory_insufficient);
	    gsu_listing(listing_malloc_poi,NULL,NULL);
            gsu_exit();
	    }
         strcpy(listing_malloc_poi, listing_string);
         strcat(listing_malloc_poi, " ");
         strcat(listing_malloc_poi, gsu_in_vect[0]);
         }
      else
         {
	 lengh = strlen(listing_malloc_poi);
	 if ((listing_malloc_poi=realloc(listing_malloc_poi,(lengh+strlen(gsu_in_vect[i])+strlen(" ")+1)))==NULL)
	    {
	    fprintf(stderr, "ERROR\n");
            dce_fprintf(stderr, gc_t_memory_insufficient);
            gsu_dce_printf_string=(char *)dce_sprintf(gc_t_memory_insufficient);
            gsu_listing(gsu_dce_printf_string, NULL, NULL);
            gsu_exit();
	    }
         strcat(listing_malloc_poi, " ");
	 strcat(listing_malloc_poi, gsu_in_vect[i]);                            
	 }
      }
      gsu_listing(listing_malloc_poi, NULL, NULL);
      free(listing_malloc_poi);
   }
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_dontusecopy_false_fkt                            */
/*  Description   :  Set control service                            */
/*  Date          :  April  1994                                    */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static void gsu_dontusecopy_false_fkt(void)
{
int i;

gsu_alloc(2);
gsu_in_vect[0] = (char *)gsu_dontusecopy;
gsu_in_vect[1] = (char *)gsu_false;
gsu_listing_malloc("gsu_dontusecopy_false_fkt", 2);
if (gc_t_moddsc(gsu_in_vect, 2, &gsu_out_vect) != GC_SUCCESS)
   {
   fprintf(stderr, "ERROR\n gc_t_moddsc ");
   for(i=0; i<2; i++)
      {
      fprintf(stderr, "%s ", gsu_in_vect[i]);
      }
   fprintf(stderr, "\n");
   fprintf(stderr, "    %s\n", gsu_out_vect[0].value);
   gsu_listing(gsu_out_vect[0].value, NULL, NULL);
   gc_t_free_result(gsu_out_vect);
   free(gsu_in_vect);
   gsu_exit();
   }
free(gsu_in_vect);
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_localscope_true_fkt                             */
/*  Description   :  Set control service                            */
/*  Date          :  April  1994                                    */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static void gsu_localscope_true_fkt(void)
{
int i;

gsu_alloc(2);
gsu_in_vect[0] = (char *)gsu_localscope;
gsu_in_vect[1] = (char *)gsu_true;
gsu_listing_malloc("gsu_localscope_true_fkt", 2);
if (gc_t_moddsc(gsu_in_vect, 2, &gsu_out_vect) != GC_SUCCESS)
   {
   fprintf(stderr, "ERROR\n gc_t_moddsc ");
   for(i=0; i<2; i++)
      {
      fprintf(stderr, "%s ", gsu_in_vect[i]);
      }
   fprintf(stderr, "\n");
   fprintf(stderr, "    %s\n", gsu_out_vect[0].value);
   gsu_listing(gsu_out_vect[0].value, NULL, NULL);
   gc_t_free_result(gsu_out_vect);
   free(gsu_in_vect);
   gsu_exit();
   }
free(gsu_in_vect);
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_attribute_value_delete                     */
/*  Description   :  Delete of name attribute                       */
/*  Input Value   :  char * to gsu_in_vector                        */
/*                   char * to string it should be deleted          */
/*  Date          :  April  1994                                    */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static unsigned8 gsu_attribute_value_delete(
		       char *attr_value_name, /* IN/OUT - input/output*/
		       char *attr_value_part) /* IN - input variable */
{
if (!strncmp(attr_value_part,attr_value_name+strlen(attr_value_name)-strlen(attr_value_part), strlen(attr_value_part)))
   {
   return(1);
   }
else
   return(0);
}

/********************************************************************/
/*                                                                  */
/*  Function name :  gsu_search_psap_dsa                            */
/*  Description   :  Search PSAP from DSA in Contact DSA            */
/*  Input Value   :  gsu_dsa * to DSA name                          */
/*  Date          :  April  1994                                    */
/*  Author        :  Eva Wilde, SNI BA NM123                        */
/*                                                                  */
/********************************************************************/
static void gsu_search_psap_dsa(
		  gsu_dsa *search_psap_dsa_act)  /*IN - input variable*/
{
int i;
int ret_val;
char search_dirid_string[GSU_LENINT]="\0";
gc_t_vector * search_psap_out_vect=NULL;

/* bind to contact DSA */
gsu_alloc(4);
gsu_in_vect[0] = (char *)gsu_attr_dirid;
sprintf(search_dirid_string, "%d", gsu_dirid);
gsu_in_vect[1] = search_dirid_string;
gsu_in_vect[2] = (char *)gsu_attr_dsa;
gsu_in_vect[3] = gsu_contdsa;
gsu_listing_malloc("gc_t_bind", 4);
gsu_bind(4, gsu_in_vect);
free(gsu_in_vect);

/*   modify the service control settings   */
gsu_default_control_service();
gsu_preferadminfkt_fkt();

/* search remote DSA with PSAP addresse */
gsu_alloc(4);
gsu_in_vect[0] = search_psap_dsa_act->dsaname;
gsu_in_vect[1] = (char *)gsu_subtree;
gsu_in_vect[2] = (char *)gsu_attribute;
gsu_in_vect[3] = (char *)gsu_psap_name_only;
gsu_listing_malloc("gc_t_search", 4);
if ((ret_val = gc_t_search(gsu_in_vect,4,TRUE,TRUE,&search_psap_out_vect)) != GC_SUCCESS)
   {
   fprintf(stderr, "ERROR\n gc_t_search ");
   for(i=0; i<4; i++)
      {
      fprintf(stderr, "%s ", gsu_in_vect[i]);
      }
   fprintf(stderr, "\n");
   fprintf(stderr, "    %s\n",search_psap_out_vect[0].value);
   gsu_listing(search_psap_out_vect[0].value, NULL, NULL);
   gsu_exit();
   }
else
   {
   free(gsu_in_vect);
   gsu_listing("   return value = ",search_psap_out_vect[1].value,NULL);
   }
strcpy(search_psap_dsa_act->psapadd,search_psap_out_vect[1].value+strlen(gsu_psap_name));
search_psap_dsa_act->psapadd[strlen(search_psap_dsa_act->psapadd)-strlen("}")] = '\0';
}
