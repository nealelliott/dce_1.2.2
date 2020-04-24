#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: gdsinfo.sh,v $
# Revision 1.1.10.2  1996/03/09  23:13:04  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:16  marty]
#
# Revision 1.1.10.1  1995/12/08  15:25:54  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:54:06  root]
# 
# Revision 1.1.8.1  1994/02/22  18:05:21  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:49:34  marrek]
# 
# Revision 1.1.6.3  1993/08/11  14:23:31  marrek
# 	July 1993 code drop.
# 	[1993/08/02  13:11:20  marrek]
# 
# Revision 1.1.6.2  1993/07/02  15:07:54  marrek
# 	Changes for port to HP800.
# 	[1993/07/02  14:37:22  marrek]
# 
# Revision 1.1.4.4  1992/12/31  18:39:23  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:12:05  bbelch]
# 
# Revision 1.1.4.3  1992/11/27  17:01:19  marrek
# 	November 1992 code drop
# 	[1992/11/26  10:09:12  marrek]
# 
# Revision 1.1.4.2  1992/10/05  16:44:09  marrek
# 	Insert #!/bin/sh .
# 	[1992/10/05  16:41:23  marrek]
# 
# Revision 1.1.2.3  1992/07/01  16:52:15  melman
# 	GDS drop of 6/26/92
# 	[1992/07/01  16:20:50  melman]
# 
# Revision 1.1.2.2  1992/06/01  20:12:05  melman
# 	New GDS merged drop
# 	[1992/05/28  21:04:49  melman]
# 
# $EndLog$
#
# $RCSfile: gdsinfo.sh,v $ $Revision: 1.1.10.2 $ $Date: 1996/03/09 23:13:04 $
#
#****************************************************************************
#                                                                           *
#         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
#                                ALL RIGHTS RESERVED                        *
#                                                                           *
#****************************************************************************

# Shell-procedure for display directory system state informations
# Input parameter:
#                 $1 = Path name <dcelocal>/var/adm/directory/gds
#		  $2 = Path name <dcelocal>
#                 $3 = Display mode (S = screen format, C = command format)
# Exit values:	  0  = Operation successfully performed
#

# ----------------------------------------------------------------------------

get_dirparam() {
# Short description: This function gets the value which is assigned to the given
#		     parameter (name) from the directory system parameter file.
# Input parameter: $1 = Parameter name
#                  $2 = Path name <dcelocal>
# Return values:   0  = Function successfully performed
#		   1  = Can't get parameter value
	awk '$1 == "'$1'" { for (i = 2; i < NF && index ($(i), "#") != 1; i++)
				printf "%s ", $(i)
			    exit
			  }' $2/$CL_PATH/conf/dirparam || return 1
	return 0
}

# ----------------------------------------------------------------------------

# main () {
CL_PATH=var/adm/directory/gds
SPRPROG="gdslanguage"
SPRFILE="`get_dirparam NLSFILENAME $2`"
SETNR=2

case $TERM in
hpterm | hp ) # Added for __hpux (HP's terminals)
	ESC="&a" 
	P0="`echo $ESC`10c05Y";P1="`echo $ESC`25c08Y";P2="`echo $ESC`25c09Y";
	P3="`echo $ESC`25c10Y";P4="`echo $ESC`25c11Y";P5="`echo $ESC`25c12Y";
	P6="`echo $ESC`25c13Y";P7="`echo $ESC`25c14Y";P8="`echo $ESC`20c18Y";
	P9="`echo $ESC`20c12Y";
	;;
* ) # other systems
	ESC="["   
	P0="`echo $ESC`05;10H";P1="`echo $ESC`08;25H";P2="`echo $ESC`09;25H";
	P3="`echo $ESC`10;25H";P4="`echo $ESC`11;25H";P5="`echo $ESC`12;25H";
	P6="`echo $ESC`13;25H";P7="`echo $ESC`14;25H";P8="`echo $ESC`18;20H";
	P9="`echo $ESC`12;20H";
	;;
esac

. $1/.profile
gdsdirinfo | \
awk 'BEGIN { if ("'$3'" == "S") {
	        P[0]="'$P0'";P[1]="'$P1'";P[2]="'$P2'";
                P[3]="'$P3'";P[4]="'$P4'";P[5]="'$P5'";
                P[6]="'$P6'";P[7]="'$P7'";P[8]="'$P8'";
                P[9]="'$P9'"
	     }
	   }
     $1 == "DUA-Cache" { CACHE++ } $1 == "Monitor"  { IPC++   }
     $1 == "C-Stub"    { CSTUB++ } $1 == "S-Stub"   { SSTUB++ }
     $1 == "DSA"       { DSA++   } 
     END { if (CACHE > 0 || IPC > 0 || CSTUB > 0 || SSTUB > 0 || DSA > 0) {
		print P[i++] MESSAGE1 ;
		if (CACHE > 0) print P[i++] CACHE " " MESSAGE2
		if (CSTUB > 0) print P[i++] CSTUB " " MESSAGE3
		if (SSTUB > 0) print P[i++] SSTUB " " MESSAGE4
		if (DSA > 0)   print P[i++] DSA   " " MESSAGE5
		if (IPC > 0)   print P[i++] IPC   " " MESSAGE6
            } else {
		print P[9] MESSAGE7 ;
            }
            if ("'$D2_LOG'" == "on") {
		print P[8] MESSAGE8 ;
            } else {
		print P[8] MESSAGE9 ;
	    }
     }'   MESSAGE1="`$SPRPROG $SPRFILE $SETNR 1`" \
	  MESSAGE2="`$SPRPROG $SPRFILE $SETNR 2`" \
	  MESSAGE3="`$SPRPROG $SPRFILE $SETNR 3`" \
	  MESSAGE4="`$SPRPROG $SPRFILE $SETNR 4`" \
	  MESSAGE5="`$SPRPROG $SPRFILE $SETNR 5`" \
	  MESSAGE6="`$SPRPROG $SPRFILE $SETNR 6`" \
	  MESSAGE7="`$SPRPROG $SPRFILE $SETNR 7`" \
	  MESSAGE8="`$SPRPROG $SPRFILE $SETNR 8`" \
	  MESSAGE9="`$SPRPROG $SPRFILE $SETNR 9`" -
exit 0
# }
