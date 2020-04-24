# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: ops_parsr.ksh,v $
# Revision 1.1.11.2  1996/03/11  02:39:06  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:41  marty]
#
# Revision 1.1.11.1  1995/12/11  21:56:29  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/06/29  17:38 UTC  bissen
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/1  1994/03/18  19:29 UTC  bissen
# 
# 	HP revision /main/bissen_systest/1  1994/03/18  19:05 UTC  bissen
# 	change deny to fail (HP didn't change the fail's to deny -bissen)
# 	[1995/12/11  20:56:19  root]
# 
# Revision 1.1.7.2  1993/09/30  20:47:20  qbarry
# 	Replaced FAIL with DENY.  See DCECDSACL6.FUNC 1.1.5.2.
# 	[1993/09/29  19:45:14  qbarry]
# 
# Revision 1.1.7.1  1993/09/09  20:16:23  cmckeen
# 	 HP's TET'ized version
# 	[1993/09/09  20:15:23  cmckeen]
# 
# Revision 1.1.5.2  1993/08/20  13:30:02  bissen
# 	Initial RP branch
# 	[1993/08/20  13:29:15  bissen]
# 
# Revision 1.1.2.4  1993/03/04  21:43:42  eperkins
# 	Corrected to properly handle line continuation on multiple lines
# 	[1993/03/04  21:43:05  eperkins]
# 
# Revision 1.1.2.3  1993/02/05  15:19:26  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:39:18  cjd]
# 
# Revision 1.1.3.2  1993/02/04  21:48:12  cjd
# 	Embedded copyright notice
# 
# Revision 1.1.2.2  1992/11/13  19:40:51  eperkins
# 	Initial version, DCE 1.0.2 system test cds.acl.6
# 	[1992/11/13  19:23:17  eperkins]
# 
# $EndLog$
awk 'BEGIN {
	progs="cdscp,acl_edit,rgy_edit,rpccp,klist,dce_login_noexec"
	prog_n=split ( progs, prog, "," )
	userline=1
}

NR == userline  { if ( USER == "" ) {
					print "User or principal argument is required"
					exit
				}
				else if ( $1 ~ /^# / ) {
					userline += 1
					next 
				}
				else if ( $1 == USER ) {
					if ( ARG == "-u" )
						print USER
					uflag=USER
					next
				}
				else {
					userline += 1 
					next
				}	
}
$1 != "" { for (i = 1; i<=prog_n; ++i)
		if ( $1 == prog[i] && uflag || orig_l[n] ~ /\\/) {
			if ( orig_l[n] ~ /\\/ )
				contf = 1
			gd_line=NR
			l=0
			n=split ( $0, orig_l)
			while ( $NF ~ /\\/ || contf == 1 ){
				if ( contf =1 && $NF !~ /\\/ )
					contf = 0
				for ( m =1; m < NF; ++m )
					  cmda[++l] = sprintf ( "%s ", $m )
				getline
				continue
			}
			if( $NF ~ /pass|fail/ && NF != 1 && $NF !~ /\\/ ) {
				q=split ( $0, cmd, " ")
			}
			if ( ARG == "-c" )
			  if (l > 0 ){ 
				for ( j=1; j <= l; ++j )
					printf ("%s", cmda[j])
				printf("\n")
			  }
			  else {
				if ( q > 0) {
             				for ( j = 1; j< q; ++j )  
						printf ("%s ", cmd[j] )
					printf ("\n")
					q=0
				}
			  }
			 else if ( ARG == "-e" && $NF ~ /pass|fail/ && \
				gd_line == NR )
				print $NF 
	
		}
		else if ( i == prog_n && gd_line != NR ) {
			uflag = ""
		}
}' ARG=$1 USER=$2 -
	
