/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: attrdec.c,v $
 * Revision 1.1.10.2  1996/02/18  00:24:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:29  marty]
 *
 * Revision 1.1.10.1  1995/12/08  22:11:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:44:48  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:12:42  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  11:55:06  marrek]
 * 
 * Revision 1.1.6.2  1993/08/12  11:27:32  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:14:13  marrek]
 * 
 * Revision 1.1.4.2  1993/01/04  17:02:31  htf
 * 	Embedded copyright notice
 * 	[1993/01/04  16:15:29  htf]
 * 
 * Revision 1.1.2.2  1992/06/11  19:51:35  melman
 * 	 gds drop
 * 	[1992/06/11  19:17:52  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: attrdec.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 00:24:05 $";
#endif

#include <dce/asn1.h>
#include <test.h>

extern asn1	article_0_dec();
extern void	article_0_err();
extern void	article_0_free();
extern asn1	article_0_cod();
extern int	article_0_len();
extern asn1	article_0_lwd();
extern asn1	article_0_lwc();
extern asn1_string article_0_in();
extern asn1_string article_0_out();
extern int	article_0_olen();
extern void	article_0_cpy();

extern asn1	article_1_dec();
extern void	article_1_err();
extern void	article_1_free();
extern asn1	article_1_cod();
extern int	article_1_len();
extern asn1	article_1_lwd();
extern asn1	article_1_lwc();
extern asn1_string article_1_in();
extern asn1_string article_1_out();
extern int	article_1_olen();
extern void	article_1_cpy();

extern asn1	article_2_dec();
extern void	article_2_err();
extern void	article_2_free();
extern asn1	article_2_cod();
extern int	article_2_len();
extern asn1	article_2_lwd();
extern asn1	article_2_lwc();
extern asn1_string article_2_in();
extern asn1_string article_2_out();
extern int	article_2_olen();
extern void	article_2_cpy();

extern asn1	article_3_dec();
extern void	article_3_err();
extern void	article_3_free();
extern asn1	article_3_cod();
extern int	article_3_len();
extern asn1	article_3_lwd();
extern asn1	article_3_lwc();
extern asn1_string article_3_in();
extern asn1_string article_3_out();
extern int	article_3_olen();
extern void	article_3_cpy();

extern asn1	asn1_stupid_dec();
extern void	asn1_stupid_err();
extern void	asn1_stupid_free();
extern asn1	asn1_stupid_cod();
extern int	asn1_stupid_len();
extern asn1	asn1_stupid_lwd();
extern asn1	asn1_stupid_lwc();
extern asn1_string asn1_stupid_in();
extern asn1_string asn1_stupid_out();
extern int	asn1_stupid_olen();
extern void	asn1_stupid_cpy();

struct asn1_proctable art_tab[] = {
{article_0_dec, article_0_err, article_0_free, article_0_cod,
article_0_len, article_0_lwd, article_0_lwc, article_0_in,
article_0_out, article_0_olen, article_0_cpy},
{article_1_dec, article_1_err, article_1_free, article_1_cod,
article_1_len, article_1_lwd, article_1_lwc, article_1_in,
article_1_out, article_1_olen, article_1_cpy},
{article_2_dec, article_2_err, article_2_free, article_2_cod,
article_2_len, article_2_lwd, article_2_lwc, article_2_in,
article_2_out, article_2_olen, article_2_cpy},
{article_3_dec, article_3_err, article_3_free, article_3_cod,
article_3_len, article_3_lwd, article_3_lwc, article_3_in,
article_3_out, article_3_olen, article_3_cpy},
{asn1_stupid_dec, asn1_stupid_err, asn1_stupid_free, asn1_stupid_cod,
asn1_stupid_len, asn1_stupid_lwd, asn1_stupid_lwc, asn1_stupid_in,
asn1_stupid_out, asn1_stupid_olen, asn1_stupid_cpy}};

#define RESET(x) {if(x < 0 || x > 3) x = 4;}

asn1_decfunc variable_article_dec(x)
int x;
{
	RESET(x);
	return(art_tab[x].dec);
}

asn1_errfunc variable_article_err(x)
int x;
{
	RESET(x);
	return(art_tab[x].err);
}

asn1_freefunc variable_article_free(x)
int x;
{
	RESET(x);
	return(art_tab[x].free);
}

asn1_codfunc variable_article_cod(x)
int x;
{
	RESET(x);
	return(art_tab[x].cod);
}

asn1_lenfunc variable_article_len(x)
int x;
{
	RESET(x);
	return(art_tab[x].len);
}

asn1_lwdfunc variable_article_lwd(x)
int x;
{
	RESET(x);
	return(art_tab[x].lwd);
}

asn1_lwcfunc variable_article_lwc(x)
int x;
{
	RESET(x);
	return(art_tab[x].lwc);
}

asn1_infunc variable_article_in(x)
int x;
{
	RESET(x);
	return(art_tab[x].in);
}

asn1_outfunc variable_article_out(x)
int x;
{
	RESET(x);
	return(art_tab[x].out);
}

asn1_olenfunc variable_article_olen(x)
int x;
{
	RESET(x);
	return(art_tab[x].olen);
}

asn1_cpyfunc variable_article_cpy(x)
int x;
{
	RESET(x);
	return(art_tab[x].cpy);
}


asn1	asn1_stupid_dec()
{
	return(0);
}

void	asn1_stupid_err(){
	return;
}

void	asn1_stupid_free(){
	return;
}

/*ARGUSED*/
asn1	asn1_stupid_cod(asn1z,m,n,x)
asn1 asn1z;
int m,n;
void * x;
{
	return(asn1_null_cod(asn1z,-1,0));
}

int	asn1_stupid_len(){
	return(2);
}

asn1	asn1_stupid_lwd(){
	return(0);
}

/*ARGUSED*/
asn1	asn1_stupid_lwc(asn1z,x)
asn1 asn1z;
void * x;
{
	return(asn1z);
}

asn1_string asn1_stupid_in(){
	return(0);
}

/*ARGUSED*/
asn1_string asn1_stupid_out(asn1z,x)
asn1_string asn1z;
void * x;
{
	*asn1z++= ASN1_OPENING_BRACKET;
	*asn1z++= ASN1_CLOSING_BRACKET;
	return(asn1z);
}

int	asn1_stupid_olen(){
	return(2);
}

void	asn1_stupid_cpy(){
	return;
}

