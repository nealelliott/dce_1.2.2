/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d27obid.c,v $
 * Revision 1.1.10.2  1996/02/18  18:23:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:46  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:16:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:26  root]
 * 
 * Revision 1.1.8.5  1994/07/19  14:49:03  marrek
 * 	Warnings removed.
 * 	[1994/07/19  12:25:53  marrek]
 * 
 * Revision 1.1.8.4  1994/06/21  14:48:32  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:47:13  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  16:03:36  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:13:09  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:48:17  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:34:00  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:55:48  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:54:15  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:32:34  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  13:13:08  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:20:07  marrek
 * 	Insert <pthread.h>
 * 	Replace MAX_BUF_LEN by D2_DNL_MAX
 * 	[1993/02/02  09:46:55  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  22:05:39  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:49:00  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  18:54:32  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  15:44:46  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:11:45  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:34:50  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d27obid.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:23:47 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

#include <gds.h>

#include <dce/d2dir.h>
#include <dce/asn1.h>
#include <d27obid.h>
#include <dce/d27util.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

#define D20_SHFNMAX  80
#define NO_OID_COMP     (D2_DNL_MAX / 2 + 1)


/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

typedef struct {
		char    *name;
		d2t_oid *oid;
		} INTNDTVAL;

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

static struct {
	  int       nb_att ;        /* number of att in the table   */
	  att_typ   *att_list;
	} att_table = { 0, NULL};

static unsigned32 suboid[NO_OID_COMP];

/**********************************************************************/
/*                                                                    */
/*       D E C L A R A T I O N S       Functions                      */
/*                                                                    */
/**********************************************************************/

static signed32 d2a063_put_oid(dua_oid *oid, char *s);
static signed32 d2a064_get_oid(char *str, size_t len, dua_oid *oid);
static signed32 get_oid_from_ints(const unsigned32, boolean, d2t_oid*,
	asn1_oid *);
static signed32 put_oid_as_ints(boolean mem_flag, asn1_oid *oid, d2t_oid *s);
static signed32 d27_oid_digits(byte *z, int l, char *buf);
static signed32 d27_one_digit_more(signed32 *t, signed32 n, signed32 x,
	signed32 m1, signed32 m2);

/*exon ****************************************************************/
/*                                                                    */
/* TYP          : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d2a063_put_oid                                      */
/*                                                                    */
/* DESCRIPTION  : converts an object identifier structure into the    */
/*                printable representation.                           */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NOERROR  conversion successfull                            */
/*      D2_ERROR    unable to convert.                                */
/*                                                                    */
/*exoff ***************************************************************/

static signed32 d2a063_put_oid(
    dua_oid * oid,      /* IN    -  object identifier structure       */
    char    * s)        /* OUT   -  printable object identifier       */
{
  d2t_oid int_oid;
  char    str[2*D2_LOBJ_CLASS] ;
  unsigned32 *ptr_int;
  signed16 i;

  if (!oid->is_oid_ints)
  {
    put_oid_as_ints (TRUE, &oid->contents.oid_asn1, &int_oid);
  }
  else
  {
    int_oid = oid->contents.oid_ints;
  }
  ptr_int = int_oid.d2_subids;
  sprintf(str , "%ld", (*ptr_int) );
  for (i=int_oid.d2_nb_el-1, ptr_int++; i>0; i--,ptr_int++)
  {
    sprintf(str,"%s.%ld",str,*ptr_int);
    if ((signed32)strlen(str) > D2_LOBJ_CLASS)
    {
      return (D2_ERROR);
    }
  }
  strcpy(s, str);
  return (D2_NOERROR);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYP          : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d2a064_get_oid                                      */
/*                                                                    */
/* DESCRIPTION  : converts a printable object identifier into the     */
/*                object identifier structure                         */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NOERROR  conversion successfull                            */
/*      D2_ERROR    unable to convert.                                */
/*                                                                    */
/*exoff ***************************************************************/

static signed32 d2a064_get_oid(
    char *str,          /* IN    -  printable object identifier       */
    size_t len,         /* IN    -  length of printable object id     */
    dua_oid * oid)      /* OUT   -  object identifier structure       */
{
  signed16 i;
  int k;
  d2t_oid  *int_oid;
  unsigned32 *ptr_oid;
  char   s[D2_DNL_MAX];    /* never expects str to be longer than this */
			    /* str could max. be D2_DNL_MAX.		*/

  strncpy(s,str,len);
  s[len] = '\0';
  int_oid = &(oid->contents.oid_ints);
  int_oid->d2_nb_el = 0;
  int_oid->d2_subids = suboid;

  i = 0;
  ptr_oid = int_oid->d2_subids;
  while ((k=sscanf(s, "%ld.%s", ptr_oid+i, s)) >= 1 )
  {
    i +=1;
    if (k == 1)
    {
      break;
    }
  }

  if (i == 0)
  {
    return(D2_ERROR);
  }

  int_oid->d2_nb_el = i;
  oid->is_oid_ints  = TRUE;
  return (D2_NOERROR);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYP          : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d27_301_oid_str                                     */
/*                                                                    */
/* DESCRIPTION  : tries to match the contents of "oid" with the       */
/*                longest possible D2_obj_id representation associated*/
/*                to the items defined in the ATTRIBUTE table.        */
/* RETURNVALUE     :                                                  */
/*      0:      compression successfull                               */
/*      -1:     unable to compress                                    */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d27_301_oid_str(
    D2_obj_id *oid,     /* OUT   -  binary object identifier          */
    char *s)            /* IN    -  printable object identifier       */
{
  signed16 i, i_max;
  int max = 0, new_max;
  dua_oid   oid_dua;

  for (i = 0; i < att_table.nb_att; i++)
  {
    if (att_table.att_list[i].oid.l <= oid->d2_typ_len &&
	memcmp(oid->d2_type, att_table.att_list[i].oid.v,
	  att_table.att_list[i].oid.l) == 0)
    {
      new_max = att_table.att_list[i].oid.l;
      if (new_max > max)
      {
	i_max = i;
	max = new_max;
      }
      if (max == oid->d2_typ_len)
      {
	break;
      }
    }
  }

  oid_dua.is_oid_ints                 = TRUE;
  oid_dua.contents.oid_ints.d2_nb_el  = 0;
  oid_dua.contents.oid_ints.d2_subids = suboid;
  if (max == 0)
  {
    if (put_oid_as_ints(FALSE,(asn1_oid *)oid,&oid_dua.contents.oid_ints)
	!= D2_ERROR)
    {
      if (d2a063_put_oid(&oid_dua, s) != D2_ERROR)
      {
	return(0) ;
      }
    }
    return (-1);
  }
  else
  {
    strcpy (s, att_table.att_list[i_max].name);
    if (max == oid->d2_typ_len)
    {
      return (0);
    }
    s += strlen(att_table.att_list[i_max].name);
    *s++ = '.';
    d27_oid_digits(oid->d2_type + max, oid->d2_typ_len - max, s);
  }
  return (0);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYP          : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d27_302_str_oid                                     */
/*                                                                    */
/* DESCRIPTION  : scans the ATTRIBUTE table to check if "s" is not    */
/*                already present as a defined attribute; if it is the*/
/*                case, the corresponding oid is returned.            */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      0:      expansion successfull                                 */
/*      -1:     unable to expanse.                                    */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d27_302_str_oid(
    char *s,            /* IN    -  printable object identifier       */
    D2_obj_id *oid)     /* OUT   -  binary object identifier          */

{
  signed16 i;
  dua_oid oid_dua;

  for (i=0; i<att_table.nb_att; i++)
  {
    if (( strlen(att_table.att_list[i].name) == strlen(s)) &&
	 (strcmp (att_table.att_list[i].name, s) == 0))
    {
      memcpy(oid->d2_type,att_table.att_list[i].oid.v,oid->d2_typ_len =
	     att_table.att_list[i].oid.l);
      return (0);
    }
  }
  if (d2a064_get_oid(s, strlen(s)+1, &oid_dua) != D2_ERROR)
  {
    if (get_oid_from_ints(svc_c_sev_warning,FALSE,&oid_dua.contents.oid_ints,
	(asn1_oid *)oid) != D2_ERROR)
    {
      return(0) ;
    }
  }
  return (-1);
}

/*exon*****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d27_303_cmp_obj_id                                  */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 05.12.90                                            */
/*                                                                    */
/* DESCRIPTION  : Compares two Object-identifiers                     */
/*                                                                    */
/* RETURNVALUE     :     -1     First object identiefier is smaller   */
/*                        1     Second object identifier is smaller   */
/*                        0     object identifier are equal           */
/*                                                                    */
/*exoff****************************************************************/

signed32 d27_303_cmp_obj_id(
    D2_obj_id *type1,   /* IN    -  first object identifier to compare*/
    D2_obj_id *type2)   /* IN    -  second object id. to compare      */

{                           /*@ d27_303_cmp_obj_id                    */

  signed32 len;         /* minimum length of Object-identifier strings*/
  signed32 ret;         /* return value                               */

/**********************************************************************/
/*                                                                    */
/*                    C O D E                                         */
/*                                                                    */
/**********************************************************************/

			/* calculate minimum of type lengths          */
  len = type1->d2_typ_len < type2->d2_typ_len ?
			    type1->d2_typ_len : type2->d2_typ_len;

			/* type lengths are equal                     */
  if (type1->d2_typ_len == type2->d2_typ_len)
  {                     /* compare object identifier strings          */
    return(memcmp(type1->d2_type,type2->d2_type,len));
  }

  if ((ret = memcmp(type1->d2_type,type2->d2_type,len)) == 0)
  {
			/* one object identifier is initial substring */
			/* of the other                               */
    if (type1->d2_typ_len < type2->d2_typ_len)
    {
      return(-1);
    }
    else
    {
      return(1);
    }
  }
  else
  {
    return(ret);
  }

}                           /*@ d27_303_cmp_obj_id                    */

/*****************************************************************************/
/*                                                                           */
/*  FUNCTION:  d27_307_init_att ()                                           */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function reads the DUA ATTRIBUTE table from file and             */
/*      initializes the att_table variable                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      D2_NOERROR  = The function has been performed successfully.          */
/*      D2_ERROR    = An error    occurred.                                  */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: Praveen Gupta                                  DATE: 03.11.90    */
/*****************************************************************************/

signed32 d27_307_init_att(
    char *p_env)        /* IN    -  name of directory containing the  */
			/*          asn_attr file                     */
{
  static boolean  table_init = FALSE;

  FILE    *fp;
  char    shfname[D20_SHFNMAX] ;
  att_typ *table ;
  char    *names;
  char    *n_ptr ;
  char    *d_ptr;
  dua_oid oid;

  register signed16 i ;

  if                            /* table not yet initialized          */
    (table_init == FALSE)
  {                             /* open the asn1_attr file            */
    sprintf (shfname, "%s%s", p_env, D27_ASN1_ATTR) ;
    if ((fp = D27_FOPEN(svc_c_sev_fatal,shfname, "r")) == NULL)
    {
      return(D2_ERROR) ;
    }
    if(fscanf(fp,"%d",&att_table.nb_att) != 1)
    {
      D27_FPCHECK(svc_c_sev_fatal,fp);
      dce_svc_printf(GDS_S_NUM_ASN1_ATTR_ERR_MSG);
      D27_FCLOSE(svc_c_sev_fatal,fp);
      return(D2_ERROR) ;
    }

    if((table = att_table.att_list = (att_typ *)D27_MALLOC(
       svc_c_sev_fatal,att_table.nb_att * sizeof(att_typ))) == NULL )
    {
      D27_FCLOSE(svc_c_sev_fatal,fp);
      return(D2_ERROR) ;
    }

    if((names = (char *)D27_MALLOC(svc_c_sev_fatal,
       att_table.nb_att * (D2_NP_ID_LEN + 1))) == NULL )
    {
      free(att_table.att_list) ;
      D27_FCLOSE(svc_c_sev_fatal,fp);
      return(D2_ERROR) ;
    }

		/* read file */
    for(i = 0, n_ptr = names; i < att_table.nb_att; i++, table++)
    {
      if(fscanf(fp,"%s %d",n_ptr,&table->syntax) != 2)
      {
	D27_FPCHECK(svc_c_sev_fatal,fp);
	dce_svc_printf(GDS_S_ILL_ASN1_ATTR_ENTRY_ERR_MSG);
	free(names) ;
	free(att_table.att_list) ;
	D27_FCLOSE(svc_c_sev_fatal,fp) ;
	return(D2_ERROR) ;
      }
      table->name = n_ptr ;
      d_ptr = n_ptr;
      n_ptr += strlen(n_ptr) + 1 ;
      if (d2a064_get_oid(d_ptr, strlen(d_ptr)+1, &oid) == D2_ERROR)
      {
	free(names) ;
	free(att_table.att_list) ;
	dce_svc_printf(GDS_S_OBID_ASN1_ATTR_ERR_MSG,d_ptr);
	D27_FCLOSE(svc_c_sev_fatal,fp) ;
	return(D2_ERROR) ;
      }

      if (get_oid_from_ints(svc_c_sev_fatal,TRUE,
	  &(oid.contents.oid_ints),&(table->oid)) == D2_ERROR)
      {
	D27_FCLOSE(svc_c_sev_fatal,fp) ;
	return(D2_ERROR) ;
      }

    }
    D27_FCLOSE(svc_c_sev_fatal,fp) ;
    table_init = TRUE;
  }

return(D2_NOERROR);

}

/*exon ****************************************************************/
/*                                                                    */
/* TYP          : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d27_one_digit_more                                  */
/*                                                                    */
/* DESCRIPTION  : adds the new item "x" within the int array "t"; the */
/*                array "t" contains integer representation in the    */
/*                "m2" decimal base, but in reverse order.            */
/*                example:  (from base 10 to base 128)                */
/*                input-> t:      85                                  */
/*                        n:      1                                   */
/*                        x:      7                                   */
/*                        m1:     10                                  */
/*                        m2:     128                                 */
/*                output-> t:     89      6                           */
/*                returned  :     2                                   */
/*                                                                    */
/*                (6 * 128 + 89 = 857)                                */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      >= 0        number of elements in the array t                 */
/*                                                                    */
/*exoff ***************************************************************/

static signed32 d27_one_digit_more(
    signed32 *t,        /* INOUT -  array of digits of base m1 / m2   */
    signed32 n,         /* IN    -  the number of elements within "t" */
    signed32 x,         /* IN    -  the new item to be introduced     */
    signed32 m1,        /* IN    -  the base of "x"                   */
    signed32 m2)        /* IN    -  the base of the result "t"        */

{                       /* d27_one_digit_more                         */
  register signed16 i;

  if (n == 0)
  {
    n = 1;
    t[0] = x;
  }
  else
  {
    for (i = 0; i < n ; i++)
    {
      t[i] *= m1;
    }
    t[0] += x;
  }

  i = 0;
  while (i < n)
  {
    if (t[i] >= m2)
    {
      if (i == (n-1))
      {
	      t[n++] = t[i]/m2;
      }
      else
      {
	t[i+1] += t[i]/m2;
      }
      t[i] = t[i]%m2;
    }
    i++;
  }

  return(n);

}                       /* d27_one_digit_more                         */

/*exon ****************************************************************/
/*                                                                    */
/* TYP          : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : digits_oid                                          */
/*                                                                    */
/* DESCRIPTION  : This function converts an object identifier from    */
/*                a digital string to "D2_obj_id" representation.     */
/*                It assumes that there is only one digital string.   */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      byte *      byte after converted object identifier            */
/*                                                                    */
/*exoff ***************************************************************/

static byte *digits_oid(
    byte *oid,          /* OUT   -  converted object identifier       */
    char *digits)       /* IN    -  printable object identifier       */
{
  signed32 n;
  signed32 t[32];

  n = 0;

  for (; *digits; digits++)
  {
    n = d27_one_digit_more (t,n,*digits - '0',10,128);
  }

  while (n)
  {
    *(oid++) = 0x80 | t [--n];
  }
  *(oid - 1) &= 0x7f;
  return (oid);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYP          : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d27_oid_digits                                      */
/*                                                                    */
/* DESCRIPTION  : This function converts an object identifier from    */
/*                "D2_obj_id" representation to a digital string.     */
/*                "l" bytes from the input parameter "z" will be      */
/*                interpreted.                                        */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      byte *      byte after converted object identifier            */
/*                                                                    */
/*exoff ***************************************************************/

static signed32 d27_oid_digits (
    byte *z,            /* IN   -  object identifier to convert       */
    int  l,             /* IN   -  length of object identifier        */
    char *buf)          /* OUT  -  string of digits                   */
{
  signed32 i = 0, n;
  signed32 t[32];

  while (i < l)
  {
    n = 0;
    while(i < l)
    {
      n = d27_one_digit_more(t,n,z[i]&0x7f,128,10);
      if ((z[i++]&0x80) == 0)
      {
	break;
      }
    }
    while (n)
    {
      *buf++ = '0' + t[--n];
    }
    *buf++ = '.';
  }
  *buf = 0;

  return(0);
}

static signed32 get_oid_from_ints(
			/* IN    -  severity level for servicability  */
    const unsigned32 attributes,
    boolean mem_flag,   /* IN    -  indicates whether memory must be  */
			/*          allocated                         */
    d2t_oid *s,         /* IN    -  object identifier as structure    */
    asn1_oid *oid)      /* OUT   -  object identifier in ASN.1        */
{
  byte     *oid_val;
  signed16 i;
  char     sub_oid[D2_DNL_MAX];

  if (mem_flag == TRUE)
  {
    if ((oid->v = (byte *)D27_MALLOC(attributes,D2_DNL_MAX)) == NULL)
    {
      return(D2_ERROR) ;
    }
  }

  oid_val         = oid->v;
  for (i = 0; i < s->d2_nb_el; i++)
  {
    sprintf(sub_oid,"%d\0",s->d2_subids[i]);
    oid_val = digits_oid(oid_val,sub_oid);
  }

  oid->l = oid_val - oid->v;
  if (mem_flag == TRUE)
  {
    oid->v = (byte *)D27_REALLOC(attributes,oid->v,oid->l);
  }
  return (D2_NOERROR);
}

static signed32 put_oid_as_ints(
    boolean  mem_flag,  /* IN    -  indicates whether memory must be  */
			/*          allocated                         */
    asn1_oid *oid,      /* IN    -  object identifier in ASN.1        */
    d2t_oid  *s)        /* OUT   -  object identifier as structure    */
{
  signed32   i = 0, n;
  signed32   t[32];
  unsigned32 *new_int;
  byte       *z;

  z = oid->v;

  s->d2_nb_el = 0;
  if (mem_flag == TRUE)
  {
     s->d2_subids = (unsigned32 *)D27_MALLOC(svc_c_sev_warning,
       32 * sizeof(unsigned32));
  }
  while (i < oid->l)
  {
    n = 0;
    while(i < oid->l)
    {
      n = d27_one_digit_more(t,n,z[i]&0x7f,128,10);
      if ((z[i++] & 0x80) == 0)
      {
	break;
      }
    }
    new_int = s->d2_subids + (s->d2_nb_el ++);
    *new_int = (unsigned32)t[--n];
    while (n)
    {
      *new_int = 10L * *new_int + (unsigned32)t[--n];
    }
  }
  if (mem_flag == TRUE)
  {
    s->d2_subids = (unsigned32 *)D27_REALLOC(svc_c_sev_warning,s->d2_subids,
      s->d2_nb_el * sizeof(unsigned32));
  }

  return (0);
}

signed32 recover_att_syntax(
    asn1_oid *oid)      /* object identifier in asn1 structure        */
{
  signed16 i;
  char s[D2_DNL_MAX];

  for (i = 0; i < att_table.nb_att; i++)
  {
    if (att_table.att_list[i].oid.l <= oid->l &&
	memcmp(oid->v,att_table.att_list[i].oid.v,
	att_table.att_list[i].oid.l) == 0)
    {
      return(att_table.att_list[i].syntax);
    }
  }
  
  if (d27_301_oid_str((D2_obj_id *)oid , s) == D2_NOERROR)
    {
    DCE_SVC_LOG((GDS_S_ASN1_OID_NOT_IN_TABLE_MSG,s));
    }
  return (D2_ERROR);
}
