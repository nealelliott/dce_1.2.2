#ifdef HPUX1000P
#!/usr/bin/sh
#else
#!/bin/sh
#endif
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: gdsconf.sh,v $
# Revision 1.1.12.2  1996/03/09  23:12:56  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:07  marty]
#
# Revision 1.1.12.1  1995/12/08  15:25:27  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1994/09/22  17:23 UTC  dtruong
# 	Merge GDS from Hpdce02_02
# 	[1995/12/08  14:54:00  root]
# 
# Revision 1.1.10.7  1994/09/12  14:24:51  keutel
# 	OT 12097: protect gds database from removal by non-root-user
# 	[1994/09/12  11:57:57  keutel]
# 
# Revision 1.1.10.6  1994/08/16  08:16:02  marrek
# 	Fix for delta update (ot 11617).
# 	[1994/08/12  11:59:51  marrek]
# 
# Revision 1.1.10.5  1994/07/06  15:06:43  marrek
# 	July 1994 code submission.
# 	[1994/07/04  13:12:20  marrek]
# 
# Revision 1.1.10.4  1994/06/21  14:44:42  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:28:51  marrek]
# 
# Revision 1.1.10.3  1994/05/10  15:50:43  marrek
# 	April 1994 code submission.
# 	[1994/04/27  12:16:02  marrek]
# 
# Revision 1.1.10.2  1994/03/23  15:08:45  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:18:02  keutel]
# 
# Revision 1.1.10.1  1994/02/22  18:05:12  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:48:36  marrek]
# 
# Revision 1.1.8.1  1993/10/29  15:10:57  keutel
# 	CR 9261
# 	[1993/10/29  14:08:31  keutel]
# 
# Revision 1.1.6.3  1993/08/11  14:16:42  marrek
# 	July 1993 code drop.
# 	[1993/08/02  13:10:11  marrek]
# 
# Revision 1.1.6.2  1993/07/02  15:07:43  marrek
# 	Changes for port to HP800.
# 	[1993/07/02  14:37:33  marrek]
# 
# Revision 1.1.4.6  1993/03/03  13:54:53  treff
# 	DCT @ OSF
# 	Remove extraneous ' from comment, upon which
# 	ksh  barfed.
# 	[1993/03/03  13:54:16  treff]
# 
# Revision 1.1.4.5  1993/02/18  15:53:31  marrek
# 	fix for OT 7269
# 	[1993/02/18  15:52:49  marrek]
# 
# Revision 1.1.4.4  1992/12/31  18:39:07  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:11:39  bbelch]
# 
# Revision 1.1.4.3  1992/12/04  18:03:26  marrek
# 	November 1992 code drop
# 	[1992/12/04  15:39:03  marrek]
# 
# Revision 1.1.4.2  1992/10/05  16:43:31  marrek
# 	Insert #!/bin/sh .
# 	[1992/10/05  16:40:45  marrek]
# 
# Revision 1.1.2.3  1992/07/01  16:52:12  melman
# 	GDS drop of 6/26/92
# 	[1992/07/01  16:20:46  melman]
# 
# Revision 1.1.2.2  1992/06/01  20:10:00  melman
# 	New GDS merged drop
# 	[1992/05/28  21:04:18  melman]
# 
# $EndLog$
#
# $RCSfile: gdsconf.sh,v $ $Revision: 1.1.12.2 $ $Date: 1996/03/09 23:12:56 $
#
#****************************************************************************
#                                                                           *
#         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
#                                ALL RIGHTS RESERVED                        *
#                                                                           *
#****************************************************************************

# @SOURCEWARNING@

# Shell-script used to configure a directory system
# Input parameter:
#            $1 = Path <dcelocal>/var/adm/directory/gds
#	     $2 = Configuration mode
#		     1  = Creation of configuration data
#		     2  = Deletion of configuration data
#		     3  = Display of configuration data (first part)
#		     31 = Display of configuration data (second part)
#                    32 = Display of configuration data (command format)
#                    4  = Changing of configuration data
#            $3 = Directory identifier
#		  (if configuration mode is 1 or 2)
#            $4 = Configuration type (Client-, Client/Server-System)
#		  (not used, if configuration mode is 2 or 3)
#            $5 = No. of server-processes
#                 (not used, if configuration mode is 2 or 3)
#	     $6 = Update mode (yes, no)
#                 (not used, if configuration mode is 2 or 3)
#	     $7 = Data base type (C-isam, informix)
#		  (not used, if configuration mode is 2 or 3)
#            $8 = Max. no. of directory system clients
#		  (not used, if configuration mode is 2 or 3)
#            $9 = Security mechanism
#            $10= Path <dcelocal>/etc
#            $11= Path <dcelocal>/var/directory/gds
#            $12= Path <dcelocal>
#
# Exit values:
#	     0  = Operation successfully performed
#	     2  = Execution of operation impossible
#	     3  = The directory system is active
#	     4  = No Configuration information available
#	     6  = Configuration information not found
#	     7  = Configuration information already exists
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
CLTSYS=Client-System
CLTSRVSYS=Clt/Srv-System

# transfer the values of the positional parameters to the variables PAR1...12
for i in 1 2 3 4 5 6 7 8 9 10 11 12
do
	eval PAR$i=$1; shift
done

case $PAR2 in
    1)  # CREATION - write configuration informations to configuration file
	if [ -s $PAR10/gdsconfig ]
	then
		eval `awk ' $2 == "'$PAR3'" { FOUND++ }
			    END { if (FOUND > 0) print "FOUND=1"
				  else print "FOUND=0" }' $PAR10/gdsconfig`
		if [ $FOUND -ne 0 ]
		then
			exit 7
		fi
	fi
	# generate DUA-cache directory
	if [ \( ! -s $PAR1/cache/CACHEDIR$PAR3 \) ]
	then
		mkdir $PAR1/cache/CACHEDIR$PAR3
		chmod 700 $PAR1/cache/CACHEDIR$PAR3
		mkdir $PAR1/cache/CACHEDIR$PAR3/update
		chmod 700 $PAR1/cache/CACHEDIR$PAR3/update
	fi

	# create CACHEJOB file, LOCKJOBS file
	touch $PAR1/cache/CACHEDIR$PAR3/update/LOCKJOBS
	chmod 666 $PAR1/cache/CACHEDIR$PAR3/update/LOCKJOBS
	echo "2 0 0 0 0 0 0 0" > $PAR1/cache/CACHEDIR$PAR3/update/CACHEJOB
	chmod 666 $PAR1/cache/CACHEDIR$PAR3/update/CACHEJOB

	if [ $PAR5 -eq 0 ]
	then
		echo "DIR-ID: $PAR3 CONF-TYPE: $PAR4 SERVER-NO: - UPDATE: $PAR6 DB-TYPE: $PAR7 CLIENTS: $PAR8 AUTH-MECH: $PAR9" \
							>> $PAR10/gdsconfig
	else
		# generate data base informations
		if [ $PAR7 != "-" -a ! -d $PAR11/dsa/dir$PAR3 ]
		then
			mkdir $PAR11/adm/dsa/dir$PAR3
			chmod 777 $PAR11/adm/dsa/dir$PAR3
			mkdir $PAR11/adm/dsa/dir$PAR3/update
			chmod 777 $PAR11/adm/dsa/dir$PAR3/update
			mkdir $PAR11/dsa/dir$PAR3 $PAR11/dsa/dir$PAR3/update
			chmod 700 $PAR11/dsa/dir$PAR3
			mkdir $PAR11/dsa/dir$PAR3/chdb
			cp $PAR11/dsa/dversion $PAR11/dsa/dir$PAR3
			cp $PAR11/dsa/conf $PAR11/dsa/dir$PAR3
			touch $PAR11/dsa/dir$PAR3/dsatdt
			chmod 644 $PAR11/dsa/dir$PAR3/dsatdt
			case $PAR7 in
                        C-isam) for A in \
				      $PAR11/dsa/attr0.dat \
				      $PAR11/dsa/attr0.idx \
				      $PAR11/dsa/attr1.dat \
				      $PAR11/dsa/attr1.idx \
				      $PAR11/dsa/attr2.dat \
				      $PAR11/dsa/attr2.idx \
				      $PAR11/dsa/objects0.dat \
				      $PAR11/dsa/objects0.idx
				 do
				   cp $A $PAR11/dsa/dir$PAR3
				 done

				 $PAR12/bin/gdsgendb \
					 $PAR11/dsa/dir$PAR3 \
					 $PAR11/adm/dsa/dir$PAR3 ;;

			informix) cp $PAR11/dsa/boot_scheme $PAR11/dsa/dir$PAR3 ;;
			*)        echo ">>: unknown data base system" 1>&2
			          exit 29
			esac
			ls $PAR11/dsa/dir$PAR3 | xargs -i \
			  chmod 600 $PAR11/dsa/dir$PAR3/{}
			chmod 700 $PAR11/dsa/dir$PAR3/update
			chmod 700 $PAR11/dsa/dir$PAR3/chdb
		fi

		# create SHDJOB file, LOCKJOBS file
		touch $PAR11/dsa/dir$PAR3/update/LOCKJOBS
		touch $PAR11/dsa/dir$PAR3/update/SHDJOBS
		touch $PAR11/dsa/dir$PAR3/update/LOGDAEMON
		chmod 666 $PAR11/dsa/dir$PAR3/update/LOCKJOBS
		chmod 666 $PAR11/dsa/dir$PAR3/update/SHDJOBS
		chmod 666 $PAR11/dsa/dir$PAR3/update/LOGDAEMON

		echo "DIR-ID: $PAR3 CONF-TYPE: $PAR4 SERVER-NO: $PAR5 UPDATE: $PAR6 DB-TYPE: $PAR7 CLIENTS: $PAR8 AUTH-MECH: $PAR9" \
							>> $PAR10/gdsconfig
	fi
	;;
    2)	# DELETION - search and delete configuration information
	eval `awk ' $2 == "'$PAR3'" { FOUND++ }
		    END { if (FOUND > 0) print "FOUND=1"
			  else print "FOUND=0" }' $PAR10/gdsconfig`
	[ $FOUND -ne 1 ] && exit 6

	gdsdirinfo 1>&2 && exit 3

	awk ' $2 != "'$PAR3'" { print $0 }' $PAR10/gdsconfig > $PAR10/gdsconfig.$$
	mv $PAR10/gdsconfig.$$ $PAR10/gdsconfig
	# delete DUA-cache informations
	[ -d $PAR1/cache/CACHEDIR$PAR3 ] && { rm -rf $PAR1/cache/CACHEDIR$PAR3 || exit $?; }

	# delete data base informations
	[ -d $PAR11/dsa/dir$PAR3 ] && { rm -rf $PAR11/dsa/dir$PAR3 || exit $?; }
	[ -d $PAR11/adm/dsa/dir$PAR3 ] && { rm -rf $PAR11/adm/dsa/dir$PAR3 || exit $?; }
	[ -s $PAR1/.crontab ] && {
		awk ' $(NF-1) == "'$PAR3'" { next }
		      { print $0 }' $PAR1/.crontab > $PAR1/.crontab.$$
	        mv $PAR1/.crontab.$$ $PAR1/.crontab; }
	;;
    3|31|32) # DISPLAYING - search and display configuration information
	SPRPROG="gdslanguage"
	SPRFILE="`get_dirparam NLSFILENAME $PAR12`"
	SETNR=1

	case $TERM in
	hpterm | hp ) # Added for __hpux (HP's terminals)
		ESC="&a"   # Use HP's terminal (__hpux)
		PA="`echo $ESC`04c10Y";
		P0="`echo $ESC`09c12Y";P1="`echo $ESC`09c13Y";P2="`echo $ESC`09c14Y";
		P3="`echo $ESC`09c15Y";P4="`echo $ESC`09c16Y";P5="`echo $ESC`09c17Y";
		P6="`echo $ESC`09c18Y";P7="`echo $ESC`09c19Y";P8="`echo $ESC`09c20Y";
		P9="`echo $ESC`09c21Y";
		;;
	* ) # other systems
		ESC="["   
		PA="`echo $ESC`10;04H";
		P0="`echo $ESC`12;09H";P1="`echo $ESC`13;09H";P2="`echo $ESC`14;09H";
		P3="`echo $ESC`15;09H";P4="`echo $ESC`16;09H";P5="`echo $ESC`17;09H";
		P6="`echo $ESC`18;09H";P7="`echo $ESC`19;09H";P8="`echo $ESC`20;09H";
		P9="`echo $ESC`21;09H";
		;;
	esac

	if [ -s $PAR10/gdsconfig ]
	then
	    if [ "$PAR2" = "31" ]
	    then
		MINL=11; MAXL=20
		MESSAGE="`$SPRPROG $SPRFILE $SETNR 1`"
		FORMAT="`$SPRPROG $SPRFILE $SETNR 2`"
	    elif [ "$PAR2" = "32" ]
	    then
		MINL=1; MAXL=20
		MESSAGE="`$SPRPROG $SPRFILE $SETNR 3`"
		FORMAT="`$SPRPROG $SPRFILE $SETNR 4`"
	    else
		MINL=1; MAXL=10
		MESSAGE="`$SPRPROG $SPRFILE $SETNR 1`"
		FORMAT="`$SPRPROG $SPRFILE $SETNR 2`"
	    fi
            awk ' BEGIN { if ("'$PAR2'" != "32") {
                                P1="'$PA'";P[I++]="'$P0'";
                                P[I++]="'$P1'"; P[I++]="'$P2'";
                                P[I++]="'$P3'"; P[I++]="'$P4'";
                                P[I++]="'$P5'"; P[I++]="'$P6'";
                                P[I++]="'$P7'"; P[I++]="'$P8'";
                                P[I]="'$P9'"; I=0
			  }
			}
		NR == 1 { print P1 MESSAGE }
		NR >= '$MINL' && NR <= '$MAXL' {
		    if ($8 == "*") UPDM=MESS_J ; else UPDM=MESS_N
			if ($14 == "-1") { AUTHM="-" }
			if ($14 == "2") { AUTHM=M_SIMP }
			if ($14 == "5") { AUTHM=M_DCE }
			if ($14 == "6") { AUTHM=M_STR }
			if ($14 == "25") { AUTHM=M_SIMP_DCE }
			if ($14 == "26") { AUTHM=M_SIMP_STR }
		    CINFO = sprintf (FORMAT, P[I++], $2, $4, $6, $12, UPDM, AUTHM) ;
		    print CINFO ;
		}
	      ' MESSAGE="$MESSAGE" FORMAT="$FORMAT" \
		MESS_J="`$SPRPROG $SPRFILE $SETNR 5`" \
		MESS_N="`$SPRPROG $SPRFILE $SETNR 6`" \
		M_SIMP="`$SPRPROG $SPRFILE $SETNR 7`" \
		M_DCE="`$SPRPROG $SPRFILE $SETNR 8`" \
		M_STR="`$SPRPROG $SPRFILE $SETNR 9`" \
		M_SIMP_DCE="`$SPRPROG $SPRFILE $SETNR 10`" \
		M_SIMP_STR="`$SPRPROG $SPRFILE $SETNR 11`" $PAR10/gdsconfig
	     exit $?
	else
	     exit 4
	fi ;;
    4)  # CHANGING - change configuration information
	if [ -s $PAR10/gdsconfig ]
	then
		eval `awk ' $2 == "'$PAR3'" { FOUND++ }
			    END { if (FOUND > 0) print "FOUND=1"
				  else print "FOUND=0" }' $PAR10/gdsconfig`
		# dir-id does not exist, can not be changed
		[ $FOUND -eq 0 ] && exit 6
	else
		# system is not configured
		exit 4
	fi

	gdsdirinfo 1>&2 && exit 3

	if [ $PAR5 -eq 0 ]
	then
		PAR5="-"
	else
		# generate data base informations
		if [ $PAR7 != "-" -a ! -d $PAR11/dsa/dir$PAR3 ]
		then
			mkdir $PAR11/adm/dsa/dir$PAR3
			chmod 777 $PAR11/adm/dsa/dir$PAR3
			mkdir $PAR11/adm/dsa/dir$PAR3/update
			chmod 777 $PAR11/adm/dsa/dir$PAR3/update
			mkdir $PAR11/dsa/dir$PAR3 $PAR11/dsa/dir$PAR3/update
			chmod 700 $PAR11/dsa/dir$PAR3
			mkdir $PAR11/dsa/dir$PAR3/chdb
			cp $PAR11/dsa/dversion $PAR11/dsa/dir$PAR3
			cp $PAR11/dsa/conf $PAR11/dsa/dir$PAR3
			touch $PAR11/dsa/dir$PAR3/dsatdt
			chmod 644 $PAR11/dsa/dir$PAR3/dsatdt
			case $PAR7 in
                        C-isam) for A in \
				      $PAR11/dsa/attr0.dat \
				      $PAR11/dsa/attr0.idx \
				      $PAR11/dsa/attr1.dat \
				      $PAR11/dsa/attr1.idx \
				      $PAR11/dsa/attr2.dat \
				      $PAR11/dsa/attr2.idx \
				      $PAR11/dsa/objects0.dat \
				      $PAR11/dsa/objects0.idx
				 do
				   cp $A $PAR11/dsa/dir$PAR3
				 done

				 $PAR12/bin/gdsgendb \
					 $PAR11/dsa/dir$PAR3 \
					 $PAR11/adm/dsa/dir$PAR3 ;;

			informix) cp $PAR11/dsa/boot_scheme $PAR11/dsa/dir$PAR3 ;;
			*)        echo ">>: unknown data base system" 1>&2
			          exit 29
			esac
			ls $PAR11/dsa/dir$PAR3 | xargs -i \
			  chmod 600 $PAR11/dsa/dir$PAR3/{}
			chmod 700 $PAR11/dsa/dir$PAR3/update
			chmod 700 $PAR11/dsa/dir$PAR3/chdb
		fi

		# create LOCKJOBS file
		[ ! -s $PAR11/dsa/dir$PAR3/update/LOCKJOBS ] && {
	    		touch $PAR11/dsa/dir$PAR3/update/LOCKJOBS
	    		chmod 666 $PAR11/dsa/dir$PAR3/update/LOCKJOBS
		}
		# create SHDJOBS file
		[ ! -s $PAR11/dsa/dir$PAR3/update/SHDJOBS ] && {
	    		touch $PAR11/dsa/dir$PAR3/update/SHDJOBS
	    		chmod 666 $PAR11/dsa/dir$PAR3/update/SHDJOBS
		}
		# create LOGDAEMON file
		[ ! -s $PAR11/dsa/dir$PAR3/update/LOGDAEMON ] && {
	    		touch $PAR11/dsa/dir$PAR3/update/LOGDAEMON
	    		chmod 666 $PAR11/dsa/dir$PAR3/update/LOGDAEMON
		}
	fi

	# replace old values in d2config
	awk ' { if ( $2 == "'$PAR3'" )
			print NEWVALUE
		else
		        print $0 }' \
	NEWVALUE="DIR-ID: $PAR3 CONF-TYPE: $PAR4 SERVER-NO: $PAR5 UPDATE: $PAR6 DB-TYPE: $PAR7 CLIENTS: $PAR8 AUTH-MECH: $PAR9" \
	$PAR10/gdsconfig > $PAR10/gdsconfig.new
	mv $PAR10/gdsconfig.new $PAR10/gdsconfig

	# generate DUA-cache directory
	if [ \( ! -s $PAR1/cache/CACHEDIR$PAR3 \) ] 
	then
		mkdir $PAR1/cache/CACHEDIR$PAR3
		chmod 700 $PAR1/cache/CACHEDIR$PAR3
		mkdir $PAR1/cache/CACHEDIR$PAR3/update
		chmod 700 $PAR1/cache/CACHEDIR$PAR3/update

		# create CACHEJOB file, LOCKJOBS file
		touch $PAR1/cache/CACHEDIR$PAR3/update/LOCKJOBS
		chmod 666 $PAR1/cache/CACHEDIR$PAR3/update/LOCKJOBS
		echo "2 0 0 0 0 0 0 0" > $PAR1/cache/CACHEDIR$PAR3/update/CACHEJOB
		chmod 666 $PAR1/cache/CACHEDIR$PAR3/update/CACHEJOB
	fi
esac
[ -s $PAR10/gdsconfig ] && {
	# examine no. of IPC-server- resp. IPC-client entries
	eval `awk 'BEGIN { CACHE=0; CSTUB=0; SSTUB=0; DSA=0; CLT=0 }
	{
	    if ($4 != "Clt/Srv-System/L") {
		# reserve server entry for DUA-cache
		CACHE = 1 ;
		# reserve server entries for remote DAP access
		CSTUB += $12 ;
	    }
	    if ($4 != "Client-System") {
		# reserve server entries for DSAs
		# (configured and dynamically activated)
		DSA += int (2 + ($6 * 1.5)) ;
		if ($4 != "Clt/Srv-System/L") {
		    # reserve server entries for outgoing remote DSP access
		    SSTUB += int ($12 * 0.5) ;
		    # reserve client entries for outgoing remote DSP access
		    CLT += int ($12 * 0.5) ;
		}
	    }
	    # reserve client entries for local/remote DAP access and
	    # incoming DAP/DSP access
	    CLT += $12 ;
	}
	END { printf "NOSRV=%d; NOCLT=%d; NODSA=%d", CACHE + CSTUB + SSTUB + \
	      			       DSA , CLT, DSA }' $PAR10/gdsconfig`
	echo ">>: Requested no. of IPC-server entries = $NOSRV" >&2
	echo ">>: Requested no. of IPC-client entries = $NOCLT" >&2
	awk ' BEGIN { MAXSRV = 256; MAXCLT = 256; NOUSR = 0; NOHEAP = 0;
		      NOSRV = '$NOSRV'; NOCLT = '$NOCLT'; NODSA = '$NODSA';
		      if (NOSRV > MAXSRV) NOSRV = MAXSRV ;
		      if (NOCLT > MAXCLT) NOCLT = MAXCLT ;
		      if (NODSA > 0) { NOUSR = NOCLT ; NOHEAP = NOCLT }}
	{ for (i = 1; i <= NF; i++) {
		if (substr ($(i), 1, 2) == "-s") {
			printf " -s%d", NOSRV ;
			continue ;
		}
		if (substr ($(i), 1, 2) == "-c") {
			printf " -c%d", NOCLT ;
			continue ;
		}
		if (substr ($(i), 1, 2) == "-u") {
			printf " -u%d", NOUSR ;
			continue ;
		}
		if (substr ($(i), 1, 2) == "-h") {
			printf " -h%d", NOHEAP ;
			continue ;
		}
		printf " %s", $(i) ;
	   }
	   printf "\n" ;
	}' $PAR1/conf/ipcconf > $PAR1/conf/ipcconf.$$
	mv $PAR1/conf/ipcconf.$$ $PAR1/conf/ipcconf
	chmod 700 $PAR1/conf/ipcconf
}
exit 0
# }
