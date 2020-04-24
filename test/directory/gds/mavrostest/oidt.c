/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: oidt.c,v $
 * Revision 1.1.10.2  1996/02/18  00:24:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:31  marty]
 *
 * Revision 1.1.10.1  1995/12/08  22:11:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:44:51  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:12:48  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  11:55:32  marrek]
 * 
 * Revision 1.1.6.2  1993/08/12  11:29:54  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:14:37  marrek]
 * 
 * Revision 1.1.4.3  1993/01/04  17:02:45  htf
 * 	Embedded copyright notice
 * 	[1993/01/04  16:15:42  htf]
 * 
 * Revision 1.1.4.2  1992/12/07  11:27:42  marrek
 * 	November 1992 code drop
 * 	Replace index by strchr
 * 	[1992/12/07  11:20:04  marrek]
 * 
 * Revision 1.1.2.3  1992/07/14  19:54:32  bmw
 * 	fix syntax errors
 * 	[1992/07/14  19:53:30  bmw]
 * 
 * Revision 1.1.2.2  1992/06/11  19:52:04  melman
 * 	 gds drop
 * 	[1992/06/11  19:18:12  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: oidt.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 00:24:09 $";
#endif

#include <string.h>
#include <dce/asn1.h>

main()
{	char buf[256], *x, *y;
	int i, er, m;
	asn1_oid oid;

	while (gets(buf)){
		er = 0;
		switch (buf[0]){
		case 'd':
			for (x = buf+1; *x == ' '; x++);
			y = strchr(x,' ');
			oid = asn1_oid_get(y,&er);
			*y++ = 0;
			if (oid.l){
				printf("Coding of \"%s\" is <",y);
				for (i=0;i<oid.l;i++) printf("%2x",oid.v[i]);
				printf(">\n");
				printf("declare(\"%s\",\"%s\") returns %d\n",
				x,y,er=asn1_oid_decl(x,oid));
			}else	printf("Error %d for %s\n",er,y);
			break;
		case 'p':
			m = 0; goto printing;
		case 'l':
			m = 1; goto printing;
		case 's':
			m = 2;
		printing:
			oid = asn1_oid_get(buf+1,&er);
			if (oid.l){
				printf("Coding of \"%s\" is <",buf+1);
				for (i=0;i<oid.l;i++) printf("%2x",oid.v[i]);
				printf(">\n");
				printf("Decoding yelds \"%s\"\n",
				asn1_oid_xput(buf,oid,m));
			}else	printf("Error %d for \"%s\"\n",er,y);
			break;
		case 0:
			break;
		default:
			printf("What? \"%s\"??\n",buf);
		}
		if (er)	printf("Error: %s\n",asn1_errmes(er));
	}
}
