/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: test_mvr.c,v $
 * Revision 1.1.10.2  1996/02/18  00:24:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:33  marty]
 *
 * Revision 1.1.10.1  1995/12/08  22:12:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:44:59  root]
 * 
 * Revision 1.1.8.2  1994/02/25  19:32:28  keutel
 * 	OT 10018
 * 	[1994/02/25  15:30:48  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:45:26  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  11:55:47  marrek]
 * 
 * Revision 1.1.6.2  1993/08/12  11:31:03  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:14:52  marrek]
 * 
 * Revision 1.1.4.4  1993/01/14  17:20:31  marrek
 * 	replace bcopy() by memcpy() and bcmp() by memcmp().
 * 	replace #ifdef _AIX by #ifdef SNI_SVR4
 * 	[1993/01/14  13:31:59  marrek]
 * 
 * Revision 1.1.4.3  1993/01/04  17:02:50  htf
 * 	Embedded copyright notice
 * 	[1993/01/04  16:15:47  htf]
 * 
 * Revision 1.1.4.2  1992/12/07  11:28:17  marrek
 * 	November 1992 code drop
 * 	Replace index by strchr
 * 	[1992/12/07  11:22:44  marrek]
 * 
 * Revision 1.1.2.3  1992/07/14  19:54:36  bmw
 * 	fix syntax errors
 * 	[1992/07/14  19:53:49  bmw]
 * 
 * Revision 1.1.2.2  1992/06/11  19:52:18  melman
 * 	 gds drop
 * 	[1992/06/11  19:18:23  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: test_mvr.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 00:24:11 $";
#endif

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <dce/asn1.h>
#define MAIN
#include <test.h>
#include <sys/time.h>

unsigned int max_decoding_time = 30;

main(argc,argv)
char ** argv;
{	int i, n, ret = 0, lookbad, time_correction = -1;
	char buff[256];
	entry *x, *y;
	FILE *FF;

	if (scanf ("%d",&n) == 0){
		perror("Cant read the number of entries");
		exit(-1);
	}
	i = (n>32)?n:32;
	x = (entry *) malloc((unsigned)i*sizeof(entry));
	y = (entry *) malloc((unsigned)i*sizeof(entry));
	for (i = 0; i <n; i++) if (scan_entry(&x[i])<0){
		(void) fprintf(stderr,"Error in entry number %d\n",i+1);
		exit(-1);
	}
	fclose(stdin);

	for(i=1; i<argc;){
		if (strcmp(argv[i],"-debug") == 0){
			max_decoding_time = 0;
			i++;
			continue;
		}
		if (strcmp(argv[i],"-ber") == 0){
			if (ber_test(x,y,n))
				ret = 1;
			i++;
			continue;
		}
		if (strcmp(argv[i],"-hard") == 0){
			int nb_tests;

			if (++i >= argc) goto usage;
			nb_tests = atoi(argv[i]);
			if (hard_test(x,y,n,nb_tests))
				ret = 1;
			i++;
			continue;
		}
		if (strcmp(argv[i],"-dump") == 0){
			if (++i >= argc) goto usage;
			if (strcmp(argv[i],"-")){
				if ((FF = fopen(argv[i],"w"))== 0){
					perror(argv[i]); goto usage;}
				ber_dump(FF,x,n);
				(void) fclose(FF);
			}else	ber_dump(stdout,x,n);
			i++;
			continue;
		}
		if (strcmp(argv[i],"-diff") == 0){
			if (++i >= argc) goto usage;
			(void) sprintf(buff,"diff - %s",argv[i]);
			(void) printf(
			"Comparaison of encoding with \"%s\":\n",argv[i]);
			(void) fflush(stdout); (void) fflush(stderr);
			if ((FF = popen(buff,"w")) == 0){
				perror(buff); goto usage;}
			ber_dump(FF,x,n);
			if (pclose(FF) != 0)
				ret = 1;
			else	(void) printf("No differences!\n");
			i++;
			continue;
		}
		if (strcmp(argv[i],"-dtxt") == 0){
			if (++i >= argc) goto usage;
			(void) sprintf(buff,"diff - %s",argv[i]);
			(void) printf(
			"Comparaison of print out with \"%s\":\n",argv[i]);
			(void) fflush(stdout); (void) fflush(stderr);
			if ((FF = popen(buff,"w")) == 0){
				perror(buff); goto usage;}
			text_out(FF,x,n);
			if (pclose(FF) != 0)
				ret = 1;
			else	(void) printf("No differences!\n");
			i++;
			continue;
		}
		if (strcmp(argv[i],"-prt") == 0){
			if (++i >= argc) goto usage;
			if (strcmp(argv[i],"-")){
				if ((FF = fopen(argv[i],"w"))==0){
					perror(argv[i]); goto usage;}
				(void) text_out(FF,x,n);
				(void) fclose(FF);
			}else	(void) text_out(stdout,x,n);
			i++;
			continue;
		}
		if (strcmp(argv[i],"-load") == 0
		||  strcmp(argv[i],"-lbad") == 0){
			lookbad = (strcmp(argv[i],"-lbad"))?0:1;
			if (++i >= argc) goto usage;
			if ((FF = fopen(argv[i],"r"))==0){
				perror(argv[i]); goto usage;}
			if (compare_file(FF,argv[i],x,y,n,lookbad) == 0)
				ret = 1;
			i++;
			continue;
		}
		if (strcmp(argv[i],"-ltxt") == 0
		||  strcmp(argv[i],"-lbtx") == 0){
			lookbad = (strcmp(argv[i],"-lbtx"))?0:1;
			if (++i >= argc) goto usage;
			if ((FF = fopen(argv[i],"r"))==0){
				perror(argv[i]); goto usage;}
			if (compare_text_file(FF,argv[i],x,y,n,lookbad) == 0)
				ret = 1;
			i++;
			continue;
		}
		if (strcmp(argv[i],"-delta") == 0){
			if (++i >= argc) goto usage;
			time_correction = atoi(argv[i]);
			i++;
			continue;
		}
		if (strcmp(argv[i],"-utc") == 0){
			if (++i >= argc) goto usage;
			if ((FF = fopen(argv[i],"r"))==0){
				perror(argv[i]); goto usage;}
			if (time_test(FF,time_correction) != 0)
				ret = 1;
			i++;
			continue;
		}
		if (strcmp(argv[i],"-copy") == 0){
			if (copy_test(x,y,n)!=0)
				ret = 1;
			i++;
			continue;
		}
		if (strcmp(argv[i],"-x509") == 0){
			if (++i >= argc) goto usage;
			if (unique_test(x,y,n,argv[i])!=0)
				ret = 1;
			i++;
			continue;
		}
		goto usage;
	}
	exit(ret);
usage:
	(void) fprintf(stderr, "Usage: %s ", argv[0]);
	(void) fprintf(stderr, "[-ber] [-x509 {file|-}] [-hard n]\\\n");
	(void) fprintf(stderr,
	"\t[-dump {file|-}] [-diff file] [-load file] [-lbad file]\\\n");
	(void) fprintf(stderr,
	"\t[-dtxt file] [-ltxt file] [-lbtx file] [-prt {file|-}]\\\n");
	(void) fprintf(stderr,
	"\t[-ftdump file][-ftcod file] [-ftgood file] [-ftbad file]\\\n");
	(void) fprintf(stderr,
	"\t[-utc file] [-copy] [-hash h] [-cmp file] [-cmpb file] \n");
	exit(-1);
}

/********************************\
* 				 *
* Test of the copying routines.	 *
* 				 *
\********************************/
copy_test(x,y,n)
entry *x, *y;
int n;
{	int nn,i,ret=0;

	(void) printf("Testing the copying routines:\n");
	printf("Memory allocation before copying = %d.\n",
	asn1_alloc_snap_shot((char ***)0));
	(void) alarm(max_decoding_time);
	entry_cpy(y,&nn,x,n);
	(void) alarm((unsigned)0);
	printf("Memory allocation after copying =  %d.\n",
	asn1_alloc_snap_shot((char ***)0));
	if (nn != n){
		printf("The number of entry differs (%d for %d).\n",nn,n);
		ret = 1;
	}else for (i=0;i<nn;i++){
		if (compare_entry(&x[i],&y[i]) == 0){
			(void) printf("Difference in entry[%d].\n",i);
			ret = 1;
		}
	}
	entry_free(y,nn);
	printf("Memory allocation after freeing =  %d.\n",
	asn1_alloc_snap_shot((char ***)0));
	return(ret);
}

/******************************\
* 			       *
* Production of a test output  *
* 			       *
\******************************/
text_out(FF,x,n)
FILE * FF;
entry *x;
int n;
{	char  * text_buf, * end_of_text;
	unsigned int l_text_buf;

	l_text_buf = entry_olen(x,n);
	text_buf = malloc(l_text_buf);
	end_of_text = entry_out(text_buf,x,n);
	(void)fwrite(text_buf,end_of_text - text_buf, 1, FF);
	putc('\n',FF);
}

/**************************************\
* 				       *
* Testing the text decoding routines.  *
* 				       *
\**************************************/
compare_text_file(FF,fn,x,y,n,lookbad)
FILE * FF;
entry *x, *y;
char * fn;
int n, lookbad;
{	int line = 0, numitem = 0, ret = 1;
	char * z, * zl;

	while ((line = asn1_load_text(&z,&zl,FF,stderr,fn,line)) >= 0){
		(void) printf("Compare with item %d of \"%s\"\n",
		++numitem, fn);
		(void) fflush(stdout); (void) fflush(stderr);
		if (compare_table((asn1)z,(asn1)zl,x,y,n,2) == lookbad)
			ret = 0;
		free(z);
	}
	(void) printf("End of file \"%s\" after %d items.\n",fn,numitem);
	return(ret);
}

/*******************************\
* 			        *
* Production of a BER encoding  *
* 			        *
\*******************************/
ber_dump(FF,x,n)
FILE * FF;
entry *x;
int n;
{	int l,i;
	asn1 z, zm;

	printf("Testing the BER coding.\n");
	alarm(max_decoding_time);
	l = entry_len(0,x,n);
	alarm((unsigned)0);
	printf("The BER coding will use %d bytes.\n",l);
	z = (asn1) malloc((unsigned)l);
	for (i=0; i<l; i++) z[i] = 0;
	alarm(max_decoding_time);
	zm = entry_cod(z,-1,0,x,n);
	alarm((unsigned)0);
	(void)asn1_dump(FF,z,zm);
	free((char *)z);
}

/************************************************\
* 						 *
* Testing the BER coding in various conditions.	 *
* 						 *
\************************************************/
ber_test(x,y,n)
entry *x,*y;
int n;
{
	asn1 z, zm;
	int lz, ret = 0;

	lz = entry_len(0,x,n);
	z = (asn1) malloc((unsigned)lz);

	(void) printf("Testing the standard encoding:\n");
	(void) alarm(max_decoding_time);
	zm = entry_cod(z,-1,0,x,n);
	(void) alarm((unsigned)0);
	dump_binary(z,zm);
	(void) fflush(stdout);
	(void) printf("Comparison of input and output:\n");
	if (compare_asn1(z,zm,x,y,n)) ret = 1;
	free((char *)z);
	return(ret);
}
/*******************************************************\
* 						        *
* Testing the resistance of the BER decoding routines.  *
* 						        *
\*******************************************************/
hard_test(x,y,n, nb_test)
entry *x,*y;
int n, nb_test;
{
	asn1 z, zm;
	int lz, rand_loc, i;

	lz = entry_len(0,x,n);
	z = (asn1) malloc((unsigned)lz);

	(void) printf("Testing the resistance of standard decoding.\n");
	printf("Memory allocation before resistance test = %d.\n",
	asn1_alloc_snap_shot((char ***)0));
	(void) alarm(max_decoding_time);
	zm = entry_cod(z,-1,0,x,n);
	(void) alarm((unsigned)0);
	lz = zm - z;
	rand_loc = (lz>>1) + nb_test;
	if (rand_loc >= lz)
		rand_loc = lz - 1;

	for (i = rand_loc; i<lz; i++){
		z[i] = rand();
	}

	for (i = 0; i < nb_test && rand_loc > 0; ){
		int j;

		for (j=0; j <64 && i < nb_test && rand_loc >0; 
		i++, j++, rand_loc--){
			int c, nn;

			asn1_wrongbyte = 0;
			asn1_diagnostic = 0;
			z[rand_loc] = rand();
			(void) alarm(max_decoding_time);
			c = (entry_dec(z,zm,y,&nn))?'=':'?';
			(void) alarm((unsigned)0);
			putc(c,stdout);
			fflush(stdout);
			entry_free(y,nn);
		}
		putc('\n',stdout);
	}
	free((char *)z);
	(void) printf("The decoding routines resisted.\n");
	printf("Memory allocation after freeing =  %d.\n",
	asn1_alloc_snap_shot((char ***)0));
	return(0);
}

/***************************************************\
* 						    *
* Test of the unique coding mode required by X.509  *
* 						    *
\***************************************************/
unique_test(x,y,n,f)
entry *x, *y;
int n;
char * f;
{	asn1 z, zm;
	int l,nn,ret;

	printf("Testing the unique coding.\n");
	printf("Memory allocation before unique coding test = %d.\n",
	asn1_alloc_snap_shot((char ***)0));
	l = entry_len(0,x,n);
	z = (asn1) malloc((unsigned) l);
	(void) alarm(max_decoding_time);
	zm = entry_cod(z,-1,ASN1_X509,x,n);
	(void) alarm((unsigned)0);
	dump_binary(z,zm);
	if (f && *f != '-'){
		char * x;
		FILE * F;

		printf("Comparaison of the unique encoding with \"%s\":\n",f);
		x = malloc ((unsigned) strlen(f) + 10);
		fflush(stdout);
		sprintf(x,"diff - %s",f);
		if (F = popen(x,"w")){
			asn1_dump(F,z,zm);
			if ((ret = pclose(F)) == 0){
				printf("No difference!\n");
			}
		}else	perror(f);
		free(x);
	}else	asn1_dump(stdout,z,zm);
	if (entry_dec(z,zm,y,&nn) == 0){
		printf("Error in decoding: %s.\n",
		asn1_errmes(asn1_diagnostic));
		ret = -1;
	}else	printf("Successfull decoding!\n");

	entry_free(y,nn);
	free((char *)z);
	printf("Memory allocation after freeing =  %d.\n",
	asn1_alloc_snap_shot((char ***)0));
	return(ret);
}

/*****************************************\
* 					  *
* Testing the standard decoding routines  *
* 					  *
\*****************************************/
compare_file(FF,fn,x,y,n,lookbad)
FILE * FF;
entry *x, *y;
char * fn;
int n, lookbad;
{	int line = 0, numitem = 0, ret = 1;
	asn1 z, zm;

	while ((line = asn1_load(&z,&zm,FF,stderr,fn,line)) >= 0){
		(void) printf("Compare with item %d of \"%s\"\n",
		++numitem, fn);
		(void) fflush(stdout); (void) fflush(stderr);
		if (compare_table(z,zm,x,y,n,0) == lookbad)
			ret = 0;
		free((char *)z);
	}
	(void) printf("End of file \"%s\" after %d items.\n",fn,numitem);
	return(ret);
}

/***********************************************************************\
* 								        *
* Testing the consistency of ASN-1 coding and decoding, with variants.  *
* 								        *
\***********************************************************************/
compare_asn1(z,zm,x,y,n)
asn1 z, zm;
table x, y;
int n;
{	int ret = 0, lz = zm - z;
	asn1 zae, undeflen(), definelen();


	(void) fflush(stdout);
	(void) alarm(max_decoding_time);
	if(compare_table(z,zm,x,y,n,0)==0)
		ret = 1;
	(void) alarm((unsigned)0);
	(void) printf("Comparison with undefined lengths:\n");
	zae = (asn1) malloc((unsigned)2*lz);
	(void) alarm(max_decoding_time);
	if(compare_table(zae,undeflen(zae,z,zm,&lz),x,y,n,0)==0)
		ret = 1;
	(void) alarm((unsigned)0);
	(void) printf("Comparison with defined lengths:\n");
	(void) alarm(max_decoding_time);
	if (compare_table(zae,definelen(zae,z,zm,&lz),x,y,n,0) == 0)
		ret = 1;
	(void) alarm((unsigned)0);
	free((char *)zae);
	return(ret);
}

/*******************************\
* 			        *
* Application of a simple test  *
* 			        *
\*******************************/
compare_table(z,zm,x,y,n,mode)
int n, mode;
asn1 z, zm;
table x, y;
{	int i, nd, ret = 1;
	asn1 zmd;

	printf("Memory allocation before decoding (mode = %d) = %d.\n",
	mode, asn1_alloc_snap_shot((char ***)0));
	switch (mode){
	case 0: zmd = entry_dec(z,zm,y,&nd); break;
	case 2: zmd = (asn1) entry_in((char *)z,(char *)zm,y,&nd);
		break;
	default:
		(void) printf("What?\n");
		return(0);
	}
	printf("Memory allocation after decoding  (mode = %d) = %d.\n",
	mode, asn1_alloc_snap_shot((char ***)0));
	if ((zmd == 0) || (mode != 1 && zmd != zm)){
		if (zmd){
			(void) printf
			("Decoded length %d instead of %d!\n",zmd -z, zm - z);
		}else{	(void) printf("Decoding error (%x, %x): %s.\n",
			asn1_wrongbyte, asn1_wrongbyte - z,
			asn1_errmes(asn1_diagnostic));
		}
		dump_binary(z,zm);
		ret = 0;
		goto finish;
	}
	if (nd != n){
		(void) printf("Decoded %d items instead of %d!\n",nd,n);
		if (nd > n) nd = n;
		(void) printf("Only %d items to compare.\n",nd);
		ret = 0;
	}
	for (i=0;i<nd;i++){
		if (compare_entry(&x[i],&y[i]) == 0){
			(void) printf("Difference in entry[%d].\n",i);
			ret = 0;
		}
	}
finish:
	(void) printf("End of comparison.\n");
	(void) fflush(stdout);
	entry_free(y,nd);
	printf("Memory allocation after free      (mode = %d) = %d.\n",
	mode, asn1_alloc_snap_shot((char ***)0));
	(void) fflush(stdout);
	(void) fflush(stderr);
	return(ret);
}

/***************************************************************************\
* 									    *
* In order to fully test various possible decodings, we reencode the whole  *
* structure, so that only "undefine length forms" are used for structured   *
* fields								    *
* 									    *
\***************************************************************************/
asn1 undeflen(z1,z2,zm,l0)
asn1 z1, z2, zm;
int * l0;
{	int s, l, ll;
	asn1 z2m;

	s = copy_head(z1,z2,&l);
	*l0 += l; z1 += l; z2 += l;
	if (*z2 == 0x80){
		z2++; (*l0)++;
		ll = -1;
	}else{
		z2 = asn1_lendec(z2m = z2,zm,&ll);
		*l0 += z2 - z2m;
	}
	if (s) {
		*z1++ = 0x80;
		if (ll == -1){
			while (*z2 != 0x00){
				l = 0;
				z1 = undeflen(z1,z2,zm,&l);
				z2 += l; *l0 += l;
			}
			*l0 += 2; z2 += 2;
		}else{	z2m = z2 + ll;
			while (z2 < z2m){
				l = 0;
				z1 = undeflen(z1,z2,zm,&l);
				z2 += l;
			}
			*l0 += ll;
		}
		*z1++ = 0; *z1++ = 0;
	}else{	z1 = asn1_lencod(z1,ll);
		memcpy((char *)z1,(char *)z2,ll);
		*l0 += ll;
		z1 += ll;
	}
	return(z1);
}

/***************************************************************************\
* 									    *
* In order to fully test various possible decodings, we reencode the whole  *
* structure, so that only "defined length forms" are used for structured    *
* fields								    *
* 									    *
\***************************************************************************/
asn1 definelen(z1,z2,z2m,l0)
asn1 z1, z2, z2m;
int  *l0;
{
	int s, l, ll;
	asn1 limit, z2_0, z3, z4;

	s = copy_head(z1,z2_0 = z2,&l);
	z1 += l; z2 += l;
	z2 = asn1_length(z2,z2m,&limit);
	if (s && z2 != limit){
		if (z2 && limit) z2m = limit;
		z3 = z4 = (asn1)malloc((unsigned)(z2m - z2));
		while (z2 < z2m && *z2){
			l = 0;
			z4 = definelen(z4,z2,z2m,&l);
			z2 += l;
		}
		limit = asn1_close(z2,z2m,limit);
		z1 = asn1_lencod(z1,ll = z4-z3);
		if (ll) memcpy((char *)z1,(char *)z3,ll);
		z1 += ll;
		free((char *)z3);
	}else{
		z1 = asn1_lencod(z1,ll = limit - z2);
		if (ll){
			memcpy((char *)z1,(char *)z2,ll);
			z1 += ll;
		}
	}
	*l0 = limit - z2_0;
	return(z1);
}

int copy_head(z1,z2,l)
asn1 z1, z2;
int *l;
{
	*z1 = * z2; *l = 0;
	if (((*z2)&31) == 31) do{
		(*l)++;
		z1[*l] = z2[*l];
	}while (z2[*l]&128);
	(*l)++;
	return(*z1&32);
}

/*******\
*       *
* scan  *
*       *
\*******/
scan_entry(x)
entry * x;
{
	if (scanf("%d",&x->i) != 1){
		(void) fprintf(stderr,"Cant read entry type\n");
		return(-1);
	}
	switch(x->i){
	case 0: return(scan_seq2(&x->o.s2));
	case 1: return(scan_set4(&x->o.s4));
	case 2: return(scan_bits(&x->o.b));
	case 3: return(scan_flags(&x->o.f));
	case 4: 
		return(scan_int(&x->o.f));
	case 5:
		return(scan_real(&x->o.r));
	case 6:
		return(scan_choice1(&x->o.va));
	default:
		(void) fprintf(stderr,"Incorrect type of entry component\n");
		return(-1);
	}
}

scan_set4(x)
set4 * x;
{	int ret;

	if ((ret=scan_seq3(&x->a)) < 0){
		(void) fprintf(stderr,"Error in SEQ3 component of SET4\n");
	}else if ((ret=scan_set1(&x->b)) < 0){
		(void) fprintf(stderr,"Error in SET1 component of SET4\n");
	}
	return(ret);
}

scan_seq3(x)
seq3 * x;
{	int ret;
	if ((ret=scan_field(&x->oid)) < 0){
		(void) fprintf(stderr,"Error in OID component of SEQ3\n");
	}else if ((ret=scan_any(&x->any)) < 0){
		(void) fprintf(stderr,"Error in ANY component of SEQ3\n");
	}
	return(ret);
}

scan_seq2(x)
seq2 * x;
{	int ret;
	if ((ret=scan_seq1(&x->a)) < 0){
		(void) fprintf(stderr,"Error in component A of SEQ2\n");
	}else if ((ret=scan_set1(&x->b)) < 0){
		(void) fprintf(stderr,"Error in component B of SEQ2\n");
	}else if ((ret=scan_set1(&x->c)) < 0){
		(void) fprintf(stderr,"Error in component C of SEQ2\n");
	}
	return(ret);
}

scan_set1(x)
set1 * x;
{	int ret=0, i;
	if (scanf("%d",&x->i1) <= 0){
		(void) fprintf(stderr,"Error in component I1 of SET1\n");
		return(-1);
	}
	if (scanf("%d",&x->nc) <= 0){
		(void) fprintf(stderr,"Error in component NC of SET1\n");
		return(-1);
	}
	if (x->nc)
	{	x->c1 = (choice1 *) malloc((unsigned)x->nc * sizeof(choice1));
		for (i=0;i<x->nc;i++){
			if (scan_choice1(&x->c1[i]) < 0){
				(void) fprintf(stderr,
				"Error in component C1[%d] of SET1\n",i);
				return(-1);
			}
		}
	}
	if ((ret=scan_choice1(&x->c2)) < 0){
		(void) fprintf(stderr,"Error in component C2 of SET1\n");
	}else if ((ret=scan_choice1(&x->c3)) < 0){
		(void) fprintf(stderr,"Error in component C3 of SET1\n");
	}
	return(ret);
}

scan_choice1(x)
choice1 * x;
{	if (scanf("%d",&x->x) <= 0){
		(void) fprintf(stderr,"Error in component X of CHOICE1\n");
		return(-1);
	}
	switch(x->x){
	case 0:
	case 1:
		if (scanf("%d",&x->o.i) <= 0){
			(void) fprintf(stderr,"Error in component O.I of CHOICE1\n");
			return(-1);
		}else	return(0);
	case 2:
		if (scan_string(&x->o.s) < 0){
			(void) fprintf(stderr,"Error in O.S component of CHOICE1\n");
			return(-1);
		}else	return(0);
	case 3:
		if (scan_any(&x->o.any) < 0){
			(void) fprintf(stderr,
			"Error in O.ANY component of CHOICE1\n");
			return(-1);
		}else	return(0);
	default:
		(void) fprintf(stderr,
		"Incorrect choice index for CHOICE1: %d\n", x->x);
		return(-1);
	}
}

scan_seq1(x)
seq1 * x;
{

	if (scan_boolean(&x->b) < 0){
		(void) fprintf(stderr,"Error in component B of SEQ1\n");
		return(-1);
	}
	if (scanf("%d",&x->i) <= 0){
		(void) fprintf(stderr,"Error in component I of SEQ1\n");
		return(-1);
	}
	if (scan_field(&x->o) < 0){
		(void) fprintf(stderr,"Error in component O of SEQ1\n");
		return(-1);
	}
	return(0);
}

scan_any (y)
field * y;
{	field x;
	int ret;

	if ((ret = scan_field(&x)) >= 0){
		if (x.l <= 0) ret = -1;
		*y = x;
	}
	return(ret);
}

scan_bits(x)
field * x;
{
	if (scan_field(x) < 0){
		fprintf(stderr,"Error in bits string\n");
		return(-1);
	}
	if (x->v){
		x->l = ((x->l - 2)*8) + (x->v[0]&7);
		x->v++;
	}else	x->l = 0;
	return(0);
}

#define UNHEXA(x)	((x<='9' && x >= 0)?(x - '0'):(x - 'a' + 10))

#define ENDOFILE	(void) fprintf(stderr,"Unexpected end of file\n");\
			return(-1);
scan_field(x)
field * x;
{	int i,j;
	char xa[256], *xb;

	if (scan_blank() < 0) {ENDOFILE}
	if (scanf("'%[0-9a-f]'",xa)<1){
		(void) fprintf(stderr,"Error while reading field value");
		return(-1);
	}

	if ((x->l = ((int)strlen(xa) + 1) / 2) > 0){
		x->v = (asn1)malloc((unsigned)x->l);
		xb = xa; i=0;
		while (j = *xb++) {
			x->v[i] = (UNHEXA(j))<<4;
			if (j = *xb){
				x->v[i] |= (UNHEXA(j));
				xb++;
			}
			i++;
		}
	}else	x->v = 0;
	return(0);
}

scan_boolean(x)
char *x;
{	char xa[256];

	if (scan_blank() < 0) {ENDOFILE}
	if (scanf("%s",xa) < 1){
		perror("Error while reading a boolean string\n");
		return(-1);
	}
	if (strcmp(xa,"TRUE")==0)
		*x = 1;
	else if (strcmp(xa,"FALSE")==0)
		*x = 0;
	else{
		(void) fprintf(stderr,"Incorrect boolean value\n");
		return(-1);
	}
	return(0);
}

scan_string(x)
char ** x;
{	char xa[256];

	if (scan_blank() < 0) {ENDOFILE}
	if (scanf("\"%[^\"]\"",xa) < 1){
		perror("Error while reading a string\n");
		return(-1);
	}
	if (strcmp(xa,"null string")){
		*x = malloc((unsigned)strlen(xa)+1);
		(void)strcpy(*x,xa);
	}else	*x = 0;
	return(0);
}

scan_int(x)
int * x;
{
	if (scanf("%d",x) <= 0){
		(void) fprintf(stderr,"Error in integer component\n");
		return(-1);
	}
	return(0);
}

/****************************************************************************\
* 									     *
* scan_flags:								     *
* 									     *
* In order to test the generation of the bit string constants, we will pick  *
* up two bits, and use the constants F0, F1, F2, F3. The constant have a     *
* system dependant generation.						     *
* 									     *
\****************************************************************************/
scan_flags(f)
int * f;
{	int bit[2], i;

	if (scanf("%d %d",&bit[0],&bit[1]) <= 0){
		(void) fprintf(stderr,"Error in flag component\n");
		return(-1);
	}
	*f = 0;
	for (i=0; i<2; i++) switch(bit[i]){
	case 0:	*f |= F0; break;
	case 1:	*f |= F1; break;
	case 2:	*f |= F2; break;
	case 3:	*f |= F3; break;
	default:
		(void) fprintf(stderr,"flag F%d is not defined\n",bit[i]);
		return(-1);
		break;
	}
	return(0);
}
/*********************************************************************\
* 								      *
* scan_real:							      *
* 								      *
* The representation of REALs can vary enormously between different   *
* machines. We use a ``mantissa + exponent'' format here, where the   *
* exponent is a power of two; thus, we have some portable test!	      *
* 								      *
\*********************************************************************/
scan_real(x)
double * x;
{	double mantissa;
	int power2_exponent;

	if (scanf("%lf %d",&mantissa, &power2_exponent) <= 0){
		(void) fprintf(stderr,"Error in real component\n");
		return(-1);
	}
	*x = ldexp(mantissa,power2_exponent);
	return(0);
}

scan_blank()
{	int c;

	do c = getc(stdin);
	while (c == ' ' || c == '\n' || c == '\t' || c == '\r');
	if (c == EOF) return (-1);
	(void) ungetc(c,stdin);
	return(0);
}

/**********\
* 	   *
* compare  *
* 	   *
\**********/
compare_entry(x,y)
entry *x, *y;
{
	if (x->i != y->i){
		(void) printf("Entry types differs: %d vs %d\n",x->i,y->i);
		return(0);
	}
	switch(x->i){
	case 0:
		if(compare_seq2(&x->o.s2,&y->o.s2) == 0){
			(void) printf("Components SEQ2 of ENTRY differ\n");
			return(0);
		}
		break;
	case 1:
		if(compare_set4(&x->o.s4,&y->o.s4) == 0){
			(void) printf("Components SET4 of ENTRY differ\n");
			return(0);
		}
		break;
	case 2:
		if(compare_bits(&x->o.b,&y->o.b) == 0){
			(void) printf("Components B of ENTRY differ\n");
			return(0);
		}
		break;
	case 3:
	case 4:
		if (x->o.f != y->o.f){
			(void) printf(
			"Components F of ENTRY type %d differ: %d vs %d\n",
			x->i, x->o.f, y->o.f);
			return(0);
		}
		break;			
	case 5:
		if (compare_real(x->o.r, y->o.r) == 0){
			(void) printf(
			"Components R of ENTRY type %d differ\n",
			x->i);
			return(0);
		}
		break;
	case 6:
		if (compare_choice1(&x->o.va, &y->o.va) == 0){
			(void) printf(
			"Components VA of ENTRY type %d differ\n",
			x->i);
			return(0);
		}
		break;
	default:
		(void) printf("Incorrect entry type: %d.\n",x->i);
		return(0);
	}
	return(1);
}

compare_set4(x,y)
set4 * x, * y;
{	int ret = 1;
	if(compare_seq3(&x->a,&y->a) == 0){
		(void) printf("Components A of SET4 differ\n");
		ret = 0;
	}
	if(compare_set1(&x->b,&y->b) == 0){
		(void) printf("Components B of SET4 differ\n");
		ret = 0;
	}
	return(ret);
}

compare_seq3 (x,y)
seq3 * x, * y;
{	int ret=1;

	if(compare_field(&x->oid,&y->oid) == 0){
		(void) printf("Components OID of SEQ3 differ\n");
		ret = 0;
	}
	if(compare_any(x->any,y->any) == 0){
		(void) printf("Components ANY of SEQ4 differ\n");
		ret = 0;
	}
	return(ret);
}

compare_seq2(x,y)
seq2 * x, * y;
{	int a, b, c;

	if ((a = compare_seq1(&x->a,&y->a)) == 0)
		(void) printf("Components A of SEQ2 differ\n");
	if ((b = compare_set1(&x->b,&y->b)) == 0)
		(void) printf("Components B of SEQ2 differ\n");
	if ((c = compare_set1(&x->c,&y->c)) == 0)
		(void) printf("Components C of SEQ2 differ\n");
	return(a && b && c);
}

compare_set1(x,y)
set1 * x, * y;
{	int ret = 1, i;

	if (x->i1 != y->i1){
		(void) printf(
		"Components I1 of SET1 differ: %d vs %d\n",x->i1,y->i1);
		ret = 0;
	}
	if (x->nc != y->nc){
		(void) printf(
		"Components NC of SET1 differ: %d vs %d\n",x->nc,y->nc);
		ret = 0;
	}else for (i=0;i<x->nc;i++){
		if (compare_choice1(&x->c1[i],&y->c1[i]) == 0){
			(void) printf("Components C1[%d] differ\n",i);
			ret = 0;
		}
	}
	if (compare_choice1(&x->c2,&y->c2) == 0){
		(void) printf("Components C2 differ\n",i);
		ret = 0;
	}
	if (compare_choice1(&x->c3,&y->c3) == 0){
		(void) printf("Components C3 differ\n",i);
		ret = 0;
	}
	return(ret);
}

compare_choice1(x,y)
choice1 *x, *y;
{	int ret = 1;

	if (x->x != y->x){
		(void) printf(
		"Components X of CHOICE1 differ: %d vs %d\n",x->x,y->x);
		ret = 0;
	}else switch(x->x){
	case 0:
	case 1:
		if (x->o.i != y->o.i){
			(void) printf(
			"Components O.I of CHOICE1 differ: %d vs %d\n",
			x->o.i,y->o.i);
			ret = 0;
		}
		break;
	case 2:
		if (compare_string(x->o.s,y->o.s) == 0){
			(void) printf("Components O.S of CHOICE1 differ\n");
			ret = 0;
		}
		break;
	case 3:
		if (compare_any(x->o.any,y->o.any) == 0){
			(void) printf("Components ANY of CHOICE1 differ\n");
			ret = 0;
		}
		break;
	default:
		(void) printf("Incorrect index X in CHOICE1: %d\n",x->x);
		ret = 0;
		break;
	}
	return(ret);
}

compare_seq1(x,y)
seq1 *x, *y;
{	int ret = 1;

	if ((x->b == 0) != (y->b == 0)){
		(void) printf("Components B of SEQ1 differ: %d vs %d\n",
		x->b, y->b);
		ret = 0;
	}
	if (x->i != y->i){
		(void) printf("Components I of SEQ1 differ: %d vs %d\n",
		x->i, y->i);
		ret = 0;
	}
	if (compare_field(&x->o, &y->o) == 0){
		(void) printf("Components O of SEQ1 differ\n");
		ret = 0;
	}
	return(ret);
}

compare_field(x,y)
field *x, *y;
{	int i;

	if (x->l != y->l){
		(void) printf("Field lengths differ: %d vs %d\n",x->l,y->l);
		return(0);
	}else if (x->l != 0 && y->v == 0){
		(void) printf("Field value is <nil>, length is %d\n",y->l);
		return(0);
	}else if (x->l != 0 && memcmp((char *)x->v,(char *)y->v,x->l)){
		(void) printf("Field values differ:\n");
		(void) putc('\'',stdout);
		for(i=0;i<x->l;i++) (void) printf("%2x",x->v[i]);
		(void) printf("'\nvs\n'");
		for(i=0;i<y->l;i++) (void) printf("%2x",y->v[i]);
		(void) printf("'\n");
		return(0);
	}
	return(1);
}

compare_bits(x,y)
field *x, *y;
{	int i,l,m;

	if (x->l != y->l){
		(void) printf("Field lengths differ: %d vs %d\n",x->l,y->l);
		return(0);
	}else if (x->l != 0){
		i = x->l&7;
		l = (x->l)>>3;
		if (i){	i = 7-i;
			for (m=0;i>=0;i--)
				m |= (1<<i);
		}else	m = 0;
		if( (l && memcmp((char *)x->v,(char *)y->v,l))
		||  (m && ((x->v[l]|m) != (y->v[l]|m)))){
			(void) printf("Bit field values differ:\n");
			(void) putc('\'',stdout);
			for(i=0;i<x->l;i++) (void) printf("%2x",x->v[i]);
			(void) printf("'\nvs\n'");
			for(i=0;i<y->l;i++) (void) printf("%2x",y->v[i]);
			(void) printf("'\n");
			return(0);
		}
	}
	return(1);
}

/*ARGSUSED*/
compare_any(x,y)
field x, y;
{
	return(1);
}

compare_string(x,y)
char *x, *y;
{
	if (x == 0) x = "null string";
	if (y == 0) y = "null string";
	if (strcmp(x,y)){
		(void) printf("String differs:\n\"%s\" vs\n\"%s\"\n",x,y);
		return(0);
	}
	return(1);
}

compare_real(x,y)
double x, y;
{
	if (x == y ||
	(x != 0 && fabs (y/x - 1) < 1.0E-8))
		return(1);
	(void) printf("Floating point number differs: %g vs %g\n",x,y);
	return(0);
}

dump_binary(zz,zm)
asn1 zz, zm;
{	int i,im;
	(void) printf("Dump binary (%x - %x):\n", zz, zm);
	while (zz < zm){
		if ((im=zm -zz) > 32) im = 32;
		for (i=0;i < im; i++)
			(void) printf("%2x",*zz++);
		(void) printf("\n");
	}
}

/*********************************************\
* 					      *
* Test of the UTC time manipulation library.  *
* 					      *
\*********************************************/
time_test(F,delta_local)
FILE * F;
int delta_local;
{
	char * x, buf[256], dt2[256];
	int l, err=0, n_test=0, utc;
	time_t corrected;
	struct timeb t1;

	struct timeval tval;
	struct timezone tzon;

	gettimeofday(&tval, &tzon);
	t1.time = tval.tv_sec;
	t1.timezone = tzon.tz_minuteswest;

	printf("Start of UTC time tests at %s",ctime(&t1.time));
	if (delta_local == -1){
		delta_local = t1.timezone;
	}
	printf("Local time zone is %d. Correction will be %d.\n",
	t1.timezone, delta_local = -60 - delta_local);
	delta_local *= 60;
#ifdef BERKELEY
	printf("Daylight saving time deltas will be removed.\n");
#endif
	while (fgets(buf,256,F)){
#ifdef BERKELEY
		struct tm * local;
#endif
		if (buf[0] == 0 || buf[0] == '#') continue;
		if ((x = strchr(buf,'/')) == 0){
			fprintf(stderr,
			"Wrong time test: %s",buf);
			continue;
		}
		*x++ = 0;
		if ((l = (utc = strchr(buf,'.') == 0)?
		asn1_time(buf,strlen(buf),&t1):
		asn1_time_gen(buf,strlen(buf),&t1)) != 0){
			printf("asn1_time(%s) returns %d!\n",
			buf, l);
			err++;
		}
		corrected = t1.time - delta_local;
#ifdef BERKELEY
		local = localtime(&t1.time);
		if (local->tm_isdst)
			corrected -= 3600;
#endif
		if (strcmp(x,ctime(&corrected)) != 0){
			printf("asn1_time(%s) returns %s instead of %s!\n",
			buf, ctime(&corrected), x);
			err++;
		}
		l = (utc)?asn1_utc_time(dt2,&t1):asn1_gen_time(dt2,&t1);
		if (strcmp(buf,dt2) != 0){
			printf("asn1_gen_time(%s) returns %s!\n",
			buf, dt2);
			err++;
		}
		n_test++;
	}
	(void) fclose(F);
	printf("End of UTC time tests. %d error%s in %d tests.\n",
	err, (err > 1)?"s":"", n_test);
	return(err);
}
