#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: cp_test.sh,v $
# Revision 1.2.12.2  1996/03/11  01:44:36  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:15:31  marty]
#
# Revision 1.2.12.1  1995/12/08  22:09:20  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:44:00  root]
# 
# Revision 1.2.7.3  1994/06/23  18:31:06  mccann
# 	remove subtree and inchname stuff
# 	[1994/06/23  12:52:22  mccann]
# 
# 	remove subtree and inchname stuff
# 
# Revision 1.2.7.2  1994/06/02  17:20:07  proulx
# 	Fix completion status reporting and clearinghouse name choices
# 	[1994/06/02  15:15:54  proulx]
# 
# Revision 1.2.5.3  1993/08/26  20:38:08  pwang
# 	Replaced arp witj getip [OT#8185]
# 	[1993/08/26  20:37:50  pwang]
# 
# Revision 1.2.5.2  1993/06/24  20:42:15  hinman
# 	[hinman@sni] - Fix bug found in giveback testing: USERNAME was not being set
# 	[1993/06/23  11:23:46  hinman]
# 
# 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
# 	[1993/06/17  14:40:27  hinman]
# 
# Revision 1.2.2.5  1993/02/24  13:21:03  jd
# 	Various script fixups.
# 	[1993/02/23  23:14:53  jd]
# 
# Revision 1.2.2.4  1993/02/04  01:40:32  jd
# 	Gave the ps command the correct options.
# 	[1993/02/04  01:39:59  jd]
# 
# Revision 1.2.2.3  1993/01/12  18:28:20  bbelch
# 	Embedding copyright notice
# 	[1993/01/07  01:12:26  bbelch]
# 
# Revision 1.2.2.2  1992/12/04  22:08:59  m_sawyer
# 	Merge changes from DEC incorporating modifications to logfile handling,
# 	LCD post-processing, clearinghouse filtering and much more.
# 	[1992/12/04  22:07:26  m_sawyer]
# 
# Revision 1.2  1992/01/19  22:13:56  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#
#  OSF DCE Version 1.0 
#
myname=`basename $0`
###############################################################################
#
# cp_test.sh - Script to run the dns control program tests.
#
# revision history:
# -----------------
# 16-nov-90 mbs original version
# 20-nov-90 mbs added some progress information
# 27-nov-90 mbs added header line from 'ps -aux' for starting statistics
# 28-nov-90 mbs added header line from 'ps -aux' for ending statistics
# 29-nov-90 mbs added -noshow and -noch switches
#               added -dir, -ch1, -ch2, -ns switches
# 30-nov-90 mbs tweek sed command
#		added -pid switch
# 07-dec-90 mbs fixed typo
# 18-feb-91 mbs added -nocds switch
# 20-feb-91 mbs change to -cds and -dns switches
# 22-feb-91 mbs added -noch command line arg
#               now use cdscp if *not* -dns
# 01-mar-91 mbs fix problem with sed and clearinghouse diddling
# 24-may-91 mbs added .clearinghouse3 stuff for create clearinghouse
#               command testing
# 29-may-91 mbs added ${grep_string}
# 13-jun-91 mbs remove dns-specific stuff since they never use this script
#               add some more 'sed' stuff for script massaging
# 18-jun-91 mbs /usr/bin/cdscp -=> /opt/dcelocal/bin/cdscp
# 19-jun-91 mbs remove sed stuff for .'s
# 20-jun-91 mbs ... bugfix
# 04-oct-91 mbs add -nodisable so we can disable testing the disable commands
#               add nslookup/sed stuff to determine our inet address
#               add -inet switch to enter inet address on the command line
# 07-oct-91 mbs add -pipe option
# 08-oct-91 mbs update cms library
# 14-oct-91 mbs eliminate usage of 'namespace' and replace it with 'cellname'
# 25-oct-91 mbs bugfix: delete test script after we're done
# 19-nov-91 mbs make -pipe the default (changed to -nopipe)
# 04-dec-91 mbs remove path from 'ipcs' invocation
# 08-jan-92 mbs bugfix: fix clearinghouse filtering
# 04-feb-92 mbs add -noskulk switch
# 05-feb-92 mbs improve usage of ${pid}
#               make cleanup more robust with 'trap'
# 11-feb-92 mbs add mfox's improvements
# 19-feb-92 mbs use 'tee' for output to logfile
# 21-feb-92 mbs exit with "status"
# 25-feb-92 mbs bugfix: shell syntax error
# 26-feb-92 mbs use different default name for 3rd clearinghouse to
#               avoid conflicts
#               default our cellname to the contents of
#               /opt/dcelocal/dce_cf.db if the file exists
# 04-mar-92 mbs display overall status before we exit
# 05-mar-92 mbs bugfix: exit/return status from 'sub-shell'
#               tighten up the switch structure
# 07-may-92 mbs add special filtering for "/:" alias
#               add -keeplines to permit keeping test scripts
#               add -use_alias switch to cause scripts to use "/.:" instead
#               ... of the full-blown cellname (/.../mumble.dec.com)
# 25-jun-92 mbs removed use of 'tee' for logfile since some of the
#               output seems to be getting dropped on the floor
#               add determination of decnet nsp
# 14-oct-92 mbs add hooks for subtree commands
###############################################################################

###############################################################################
#Portability Stuff

#Set up HOSTNAME.
HOSTNAME=''
type hostname 1>/dev/null 2>&1
if [ "$?" -eq 0 ]; then
	HOSTNAME=`hostname` 
else
	type uname 1>/dev/null 2>&1
	if [ "$?" -eq 0 ]; then
		HOSTNAME=`uname -n`
	fi
fi
if [ "$HOSTNAME" = "" ]; then
	echo "*** Setup Error:"
	echo "Could not set up \$HOSTNAME!!!\nStopping now.\n"
	exit 1
fi

#Set up PS.
PS=ps

#Set up PSFLAGS.
PSFLAGS=''
for psflags in -el -ef -aux; do
	$PS $psflags 1>/dev/null 2>&1
	if [ $? -eq 0 ]; then
		PSFLAGS=$psflags
		break
	fi
done
if [ "$PSFLAGS" = "" ]; then
	echo "*** Setup Error:"
	echo "Could not find correct flags to use with '$PS'\nStopping now.\n"
	exit 1
fi

internal_whoami()
{
type whoami 1>/dev/null 2>&1
if [ "$?" -eq 0 ]; then
	whoami
else
	type id 1>/dev/null 2>&1
	if [ "$?" -eq 0 ]; then
		(IFS="()";set - `id`;$ECHO $2)
	fi
fi
}

#Set up USERNAME.
USERNAME=`internal_whoami`
if [ "$USERNAME" = "" ]; then
      echo "*** Setup Error:\n"
      echo "Could not identify username\n.Stopping now.\n"
      exit 1
fi

#End of portability stuff
###############################################################################

#
# ensure we have /etc in our path
#
PATH=/etc:$PATH

#
# Check invocation
#

while true
  do
    case "$#" in

      0)  echo ""
          echo "usage: ${myname} [-switches] testname"
          echo ""
          echo "    valid switches are:"
          echo ""
          echo "        -cell name      specify cellname"
          echo "        -remcell name   specify remote cellname"
          echo "        -ch1  name      specify clearinghouse (primary)"
          echo "        -ch2  name      specify clearinghouse (secondary)"
          echo "        -ch3  name      specify create clearinghouse name"
          echo "        -dir  dirname   specify toplevel test directory"
          echo "        -inet address   specify internet address"
          echo "                        eg. -inet 16.20.16.32"
          echo "        -dnet address   specify decnet address"
          echo "                        eg. -dnet 4.919"
          echo "        -cdscpdir dir   specify the directory of "
          echo "                        cdscp executable"
          echo ""
          echo "        -noshow         strip show commands from scripts"
          echo "        -noskulk        strip skulk commands from scripts"
          echo "        -nodeldir       strip deldir commands from scripts"
          echo "        -noch           strip clearinghouse stuff (all)"
          echo "        -noch1          strip clearinghouse stuff (primary)"
          echo "        -noch2          strip clearinghouse stuff (secondary)"
          echo "        -noch3          strip clearinghouse stuff (create)"
          echo "        -nopipe         don't pipe commands to 'cdscp'"
          echo ""
          echo "        -disable        don't strip disable commands from scripts"
          echo "        -pid            use 'pid' \$\$ to ensure unique filenames"
          echo "        -keeplines      don't delete the testscript when done"
	  echo "        -restart        restart dce before this test"
          echo "        -use_alias      use '/.:' for the local root"
          echo "                        (i.e. executes set -x shell command)"
	  echo "        -v              verbose"
          echo ""
          exit 1
	  ;;

      *)  case $1 in

	    -ch1)	clearinghouse1_arg=$2;	shift;shift;;
	    -ch2)	clearinghouse2_arg=$2;	shift;shift;;
	    -ch3)	clearinghouse3_arg=$2;	shift;shift;;
	    -cell)	cellname_arg=$2;	shift;shift;;
	    -remcell)	remcellname_arg=$2;	shift;shift;;
	    -dir)	toptestdir_arg=$2;	shift;shift;;
	    -cdscpdir)  cdscpdir_arg=$2;        shift;shift;;
	    -inet)	inetaddr_arg=$2;	shift;shift;;

	    -pid)	pid=$$
			prefix="${pid}>"
			shift
			;;

	    -disable)	disable_arg=1;	shift;;
	    -noshow)	noshow_arg=1;	shift;;
	    -noskulk)	noskulk_arg=1;	shift;;
	    -nodeldir)	nodeldir_arg=1; shift;;
	    -noch)	noch_arg=1;	shift;;
	    -noch1)	noch1_arg=1;    shift;;	
	    -noch2)	noch2_arg=1;	shift;;
	    -noch3)	noch3_arg=1;	shift;;
	    -nopipe)	nopipe_arg=1;	shift;;
	    -restart)	restart_arg=1;	shift;;
            -keeplines) keeplines_arg=1; shift;;
            -use_alias) use_alias_arg=1; shift;;
	    -v)		set -x;		shift;;

	    *)		cptest=$1
			testfile=cp_${cptest}.tests
			if [ ! -f ${testfile} ]; then
			    echo "I can't find ${testfile}"
			    exit 2
			fi
			break
			;;

	  esac
	  ;;

    esac
  done

#
# Command line switches
#

noch=${noch_arg=0}
noch1=${noch1_arg=0}
noch2=${noch2_arg=0}
noch3=${noch3_arg=0}
nodeldir=${nodeldir_arg=0}
noshow=${noshow_arg=0}
noskulk=${noskulk_arg=0}
restartdce=${restart_arg=0}

nopipe=${nopipe_arg=0}
keeplines=${keeplines_arg=0}
use_alias=${use_alias_arg=0}
disable=${disable_arg=0}
if [ ${disable} -ne 0 ];
  then nodisable=0
  else nodisable=1
fi

#
# Some definitions
#

workdir=`pwd`
logfile=${cptest}.log
test_script=/usr/tmp/cp_${cptest}${pid}.sh
test_lines=${workdir}/${cptest}_lines${pid}
exit_status=0

#
# Initialize our logfile
#

echo "${prefix}Initializing logfile (${logfile}) ..."
echo "# Invocation: $0 $*" > ${logfile}
echo "# Date:       `date`" >> ${logfile}
echo "" >> ${logfile}

#
# Prepare to cleanup after ourselves
#
trap "rm -f ${test_script} ${test_lines}; exit 1" 1 2 3 9 15

#
# Determine the cellname
#
# Note: we default this to the abbreviation for the local cell
#

if [ -f /opt/dcelocal/dce_cf.db ];
    then cellname=${cellname_arg=`awk '/cellname/ {print $2}' /opt/dcelocal/dce_cf.db`}
    else cellname=${cellname_arg="/.:"}
fi
echo "${prefix}cellname       : ${cellname}" | tee -a ${logfile}

#
# Set up cell names to be substituted into test scripts. Note that if
# remcellname was not specified on the command line then the generate
# script will contain syntax errors.
#
remcellname="${remcellname_arg=/.../no_rem_cell_name}"
nocellname="/.../c=us/o=ibm/o=nosuchcell"
bindcellname="/.../no.such.cell.name"

echo "${prefix}remcellname    : ${remcellname}"  | tee -a ${logfile}
echo "${prefix}invalidcellname: ${nocellname}"   | tee -a ${logfile}
echo "${prefix}bindcellname   : ${bindcellname}" | tee -a ${logfile}

#
# Determine the location of the cdscp executable we should use to test
#
cdscpdir=${cdscpdir_arg="/opt/dcelocal/bin"}
echo "${prefix}cdscp directory: ${cdscpdir}"     | tee -a ${logfile}

#
# Determine the toplevel test directory (ie. the testspace)
#
toptestdir=${toptestdir_arg="`whoami`${pid}"}
echo "${prefix}toptestdir     : ${toptestdir}" | tee -a ${logfile}

testspace=" ${cellname}/${toptestdir}"
echo "${prefix}testspace      :${testspace}" | tee -a ${logfile}

#
# Determine our internet address
#
# Note: this is gross but it does the trick.  If perchance this should
#       cause problems on some systems, the workaround is to simply
#       comment this mess out and use the -inet x.y.x switch
#
ourhostname="`hostname`"
echo "${prefix}hostname       : ${ourhostname}" | tee -a ${logfile}
ouraddress=`arp $ourhostname | sed 's/.*(\(.*\)).*/\1/'`
#ouraddress=`nslookup ${ourhostname} | \
#        sed -e "1,/${ourhostname}/d" -e "s/Address:  //g"`
#
inetaddr=${inetaddr_arg="${ouraddress}"}
echo "${prefix}internet_addr  : ${inetaddr}" | tee -a ${logfile}

#
# Setup the clearinghouse references
#
if [ ${noch1} -eq 0 ]
then
    clearinghouse1=${clearinghouse1_arg=`hostname | cut -f1 -d.`_ch}
    echo "${prefix}clearinghouse1 : ${clearinghouse1}"      | tee -a ${logfile}
else
    clearinghouse1=NONE
fi

if [ ${noch2} -eq 0 ]
then
    clearinghouse2=${clearinghouse2_arg=`hostname | cut -f1 -d.`2_ch}
    echo "${prefix}clearinghouse2 : ${clearinghouse2}"      | tee -a ${logfile}
else
    clearinghouse2=none
fi

if [ ${noch3} -eq 0 ]
then
    clearinghouse3=${clearinghouse3_arg=`hostname | cut -f1 -d.`3_ch}
    echo "${prefix}clearinghouse3 : ${clearinghouse3}"      | tee -a ${logfile}
else
    clearinghouse3=none
fi

# 
# make sure the clearinhouse checks return an accurate answer.
#
if [ ${noch} -ne 1 ]
then
    if [  ${noch1} -ne 1 ]
    then
	${cdscpdir}/cdscp set dir /.: to skulk

cdscp <<cdscp # | grep ${clearinghouse1} || cdscp create clearinghouse ${cellname}/${clearinghouse1}
set cdscp confidence  high
list clearinghouse /.:/*
cdscp
    fi
    
    if [ ${noch2} -ne 1 ]
    then
	${cdscpdir}/cdscp set dir /.: to skulk
cdscp <<cdscp # | grep ${clearinghouse2} || cdscp create clearinghouse ${cellname}/${clearinghouse2}
set cdscp confidence  high
list clearinghouse /.:/*
cdscp
    fi
    
    if [ ${noch3} -ne 1 ]
    then
	${cdscpdir}/cdscp set dir /.: to skulk
cdscp <<cdscp # | grep ${clearinghousr3} || cdscp create clearinghouse ${cellname}/${clearinghouse3}
set cdscp confidence  high
list clearinghouse /.:/*
cdscp
    fi
fi

#
# slap on the cellname (and massage the test file for other stuff)
#
echo ""
echo "${prefix}adding cellname (${testspace}) to test_lines ..." | tee -a ${logfile}
sed -e "/^#/d" \
    -e "s+>Expect+#Expect+g" \
    -e "s+ \/\:+ special_fs_alias+g" \
    -e "s+ \/+ ${testspace}/+g" \
    -e "s+${testspace}/clearinghouse1+${cellname}/${clearinghouse1}+g" \
    -e "s+${testspace}/clearinghouse2+${cellname}/${clearinghouse2}+g" \
    -e "s+${testspace}/clearinghouse3+${cellname}/${clearinghouse3}+g" \
    -e "s+our_internet_address+${inetaddr}+g" \
    -e "s+our_cellname+${cellname}+g" \
    -e "s+other_cellname+${remcellname}+g" \
    -e "s+nosuch_cellname+${nocellname}+g" \
    -e "s+bind_cellname+${bindcellname}+g" \
    -e "s+\/$++g" \
    -e "s+ special_fs_alias+ \/\:+g" \
    -e "s+\/ + +g" \
    -e "s+  + +g" \
    ${testfile} > ${test_lines}

#
# command line switches
#

noch=${noch_arg=0}
noch1=${noch1_arg=0}
noch2=${noch2_arg=0}
noch3=${noch3_arg=0}
nodeldir=${nodeldir_arg=0}
noshow=${noshow_arg=0}
noskulk=${noskulk_arg=0}

nopipe=${nopipe_arg=0}
disable=${disable_arg=0}
if [ ${disable} -ne 0 ];
  then nodisable=0
  else nodisable=1
fi

#
# special filtering
#

if [ ${noch} -ne 0 ]; then
    echo "${prefix}disabling clearinghouse references (all)" | tee -a ${logfile}
    sed -e "/clearinghouse/d" \
        -e "/${clearinghouse1}/d" \
        -e "/${clearinghouse2}/d" \
        -e "/${clearinghouse3}/d" \
        ${test_lines} > ${test_lines}.tmp
    mv ${test_lines}.tmp ${test_lines}
fi

if [ ${noch1} -ne 0 ]; then
    echo "${prefix}disabling '${clearinghouse1}' clearinghouse references (primary)" | tee -a ${logfile}
    sed -e "/${clearinghouse1}/d" \
        -e "/clearinghouse1/d" \
        ${test_lines} > ${test_lines}.tmp
    mv ${test_lines}.tmp ${test_lines}
fi

if [ ${noch2} -ne 0 ]; then
    echo "${prefix}disabling '${clearinghouse2}' clearinghouse references (secondary)" | tee -a ${logfile}
    sed -e "/${clearinghouse2}/d" \
        -e "/clearinghouse2/d" \
        ${test_lines} > ${test_lines}.tmp
    mv ${test_lines}.tmp ${test_lines}
fi

if [ ${noch3} -ne 0 ]; then
    echo "${prefix}disabling '${clearinghouse3}' clearinghouse references (create)" | tee -a ${logfile}
    sed -e "/${clearinghouse3}/d" \
        -e "/clearinghouse3/d" \
        ${test_lines} > ${test_lines}.tmp
    mv ${test_lines}.tmp ${test_lines}
fi

if [ ${nodeldir} -ne 0 ]; then
    echo "${prefix}disabling 'delete directory' commands" | tee -a ${logfile}
    sed -e "/^delete directory/d" ${test_lines} > ${test_lines}.tmp
    mv ${test_lines}.tmp ${test_lines}
fi

if [ ${noshow} -ne 0 ]; then
    echo "${prefix}disabling 'show' commands" | tee -a ${logfile}
    sed -e "/^show/d" ${test_lines} > ${test_lines}.tmp
    mv ${test_lines}.tmp ${test_lines}
fi

if [ ${noskulk} -ne 0 ]; then
    echo "${prefix}disabling 'skulk' commands" | tee -a ${logfile}
    sed -e "/skulk/d" ${test_lines} > ${test_lines}.tmp
    mv ${test_lines}.tmp ${test_lines}
fi

if [ ${nodisable} -ne 0 ]; then
    echo "${prefix}disabling 'disable mumble' commands" | tee -a ${logfile}
    sed -e "/^disable/d" ${test_lines} > ${test_lines}.tmp
    mv ${test_lines}.tmp ${test_lines}
fi

#
# create a test_script to actually do the work
#

echo "${prefix}building script file (${test_script}) ..." | tee -a ${logfile}
echo "#!/bin/sh"                                        >  ${test_script}
echo "exit_status=0"                                    >> ${test_script}
echo "trap 'rm -f ${test_script}; exit 1' 1 2 3 9 15"   >> ${test_script}
echo "while read text"                                  >> ${test_script}
echo "do"                                               >> ${test_script}

if [ ${nopipe} -eq 1 ];
  then
    echo "echo \"cdscp> \$text\""                       >> ${test_script}
    echo "${cdscpdir}/cdscp \$text"                     >> ${test_script}
    echo "if [ \$? -ne 0 ];then echo \$text failed; exit_status=1;fi"  >> ${test_script}
    echo "done"                                         >> ${test_script}
  else
    echo "echo \"\$text\""                              >> ${test_script}
    echo "done | ${cdscpdir}/cdscp"               >> ${test_script}
    echo "if [ \$? -ne 0 ];then echo \$text failed; exit_status=1;fi"  >> ${test_script}
fi
echo "exit \$exit_status"                               >>${test_script}


##
## Pre-processing for some fussy scripts
##
	echo "${prefix}Preprocessing before tests ..." | tee -a ${logfile}
if [ ${restartdce} -eq 1 ];
	then
	echo " "
	echo "${prefix}Restarting DCE before tests ..." | tee -a ${logfile}
	echo " "
	#
	# Shutdown and restart DCE
	#
	dcesetup stop
	dcesetup start
fi

	echo "${prefix}More preprocessing before tests ..." | tee -a ${logfile}

if [ ${cptest} = unauthorised ] || [ ${cptest} = intercell ];
  then
  #	
  # Get rid of authentication keys
  #
  echo "${prefix}Destroying the keys for tests"   | tee -a ${logfile}
  kdestroy;
fi

#
# Capture some statistics
#
echo "${prefix}Capturing starting statistics ..."       | tee -a ${logfile}
echo "#"                                                >> ${logfile}
echo "# `date` - Starting statistics"                   >> ${logfile}
echo "#"                                                >> ${logfile}
echo ""                                                 >> ${logfile}
df                                                      >> ${logfile}
echo ""                                                 >> ${logfile}
#/etc/pstat -s                                           >> ${logfile}
echo ""                                                 >> ${logfile}
ipcs                                                    >> ${logfile}
echo ""                                                 >> ${logfile}
$PS $PSFLAGS | head -1                                 >> ${logfile}
$PS $PSFLAGS | grep 'cds'                              >> ${logfile}

#
# Execute the script file while catching both stdout and stderr
#
echo "${prefix}Executing script file (${test_script}) ..." | tee -a ${logfile}
echo ""                                                 >> ${logfile}
echo "#"                                                >> ${logfile}
echo "# `date` - Begin testing"                         >> ${logfile}
echo "#"                                                >> ${logfile}
echo ""                                                 >> ${logfile}
chmod 755 ${test_script}
(${test_script} < ${test_lines} 2>&1)                   >> ${logfile}
exit_status=$?
echo ""                                                 >> ${logfile}

##
## Post processing for some fussy scripts
##



#
# Capture some more statistics
#
echo "${prefix}Capturing ending statistics ..."         | tee -a ${logfile}
echo ""                                                 >> ${logfile}
echo "#"                                                >> ${logfile}
echo "# `date` - Ending statistics"                     >> ${logfile}
echo "#"                                                >> ${logfile}
echo ""                                                 >> ${logfile}
df                                                      >> ${logfile}
echo ""                                                 >> ${logfile}
#/etc/pstat -s                                           >> ${logfile}
echo ""                                                 >> ${logfile}
ipcs                                                    >> ${logfile}
echo ""                                                 >> ${logfile}
$PS $PSFLAGS | head -1                                  >> ${logfile}
$PS $PSFLAGS | grep 'cds'                               >> ${logfile}

#
# Cleanup
#

rm -f ${test_script} ${test_lines}
if [ ${keeplines} -eq 0 ];
  then rm -f ${test_lines}
fi

echo "${prefix}Done testing '${cptest}'" | tee -a ${logfile}

#
# display our overall status and exit with status
#
# NOTE: The information provided by the exit status may be misleading and
# does not represent the true completion status of the test.  Examination
# of the log is required to verify satisfactory results.
#
echo ""                                                 | tee -a ${logfile}
echo "#"                                                >> ${logfile}
echo "# `date` - Exiting test"                          >> ${logfile}
echo "#"                                                >> ${logfile}
echo ""                                                 >> ${logfile}
if [ ${exit_status} -ne 0 ];
  then echo "*** Exit status has been set"              | tee -a ${logfile}
  else echo "*** No errors were detected"               | tee -a ${logfile}
fi

exit ${exit_status}

