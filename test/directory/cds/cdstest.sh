#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: cdstest.sh,v $
# Revision 1.1.12.2  1996/03/11  01:43:44  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:14:53  marty]
#
# Revision 1.1.12.1  1995/12/08  22:08:33  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:43:29  root]
# 
# Revision 1.1.9.1  1994/06/23  18:31:00  mccann
# 	remove subtree and inchname stuff
# 	[1994/06/23  12:52:11  mccann]
# 
# 	remove subtree and inchname stuff
# 
# Revision 1.1.7.2  1993/06/24  20:41:46  hinman
# 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
# 	[1993/06/17  14:40:06  hinman]
# 
# Revision 1.1.5.3  1993/02/24  13:20:58  jd
# 	New versions
# 	[1993/02/23  23:00:21  jd]
# 
# Revision 1.1.5.2  1993/01/12  18:26:08  bbelch
# 	Embedding copyright notice
# 	[1993/01/07  01:10:36  bbelch]
# 
# Revision 1.1.2.2  1992/06/09  14:28:26  mfox
# 	New shell script for testing CDS via cdstest.
# 	[1992/06/09  14:26:15  mfox]
# 
# $EndLog$
myname=`basename $0`
###############################################################################
#
# cdstest.sh - Script to run the cdstest program tests.
#
# revision history:
# -----------------
# 30-jan-92 mbs original version from danny cobb
# 06-feb-92 mbs modified for more comprehensive testing and
#               incorporation into the CDS test plan
# 11-feb-92 mbs more of the same
# 25-feb-92 mbs add replica testing
# 26-feb-92 mbs clean-up replica testing
#               use different default name for 3rd clearinghouse to
#               avoid conflicts
# 28-feb-92 mbs add softlink testing
#               add group testing
#               add miscellaneous command testing
# 02-mar-92 mbs fix overall exit status handling
#               add group 'member' tests
# 03-mar-92 mbs fixup 'type' for group 'member' tests
# 04-mar-92 mbs expand/convert group member names
#               add logfile processing
# 05-mar-92 mbs make tests optional by catagory
# 18-mar-92 mbs added attribute testing
# 20-mar-92 mbs more attribute testing
# 09-apr-92 mbs bugfix: fixed attribute testing (awk bug)
# 15-apr-92 mbs add -nodelete switch
# 30-apr-92 mbs augment object attribute testing
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
for psflags in -ef -aux; do
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

if [ $# -eq 0 ];
    then parsing="false"
    else parsing="true"
fi

while ${parsing}
  do
    case $1 in

        -ch1)           clearinghouse1_arg=$2; shift; shift;;
	-ch2)	        clearinghouse2_arg=$2; shift; shift;;
	-ch3)	        clearinghouse3_arg=$2; shift; shift;;
	-cell)	        cellname_arg=$2;       shift; shift;;
	-class)	        classname_arg=$2;      shift; shift;;
        -count)         iterations_arg=$2;     shift; shift;;
	-dir)           toptestdir_arg=$2;     shift; shift;;
        -inet)          inetaddr_arg=$2;       shift; shift;;
	-version)	classversion_arg=$2;   shift; shift;;

        -pid)	        pid=$$;	prefix="${pid}>";shift;;

	-noch)	        noch_arg=1;     shift;;
	-noch1)	        noch1_arg=1;    shift;;
	-noch2)	        noch2_arg=1;    shift;;
	-noch3)	        noch3_arg=1;    shift;;

        -debug)         debug_arg=1;    shift;;
        -nodelete)      nodelete_arg=1; shift;;

        all)            do_all=1;       shift;;                        
        directory)      do_directory=1; shift;;
        object)         do_object=1;    shift;;
        attribute)      do_attribute=1; shift;;
        group)          do_group=1;     shift;;
        member)         do_member=1;    shift;;
        replica)        do_replica=1;   shift;;
        softlink)       do_softlink=1;  shift;;
        expand)         do_expand=1;    shift;;
        convert)        do_convert=1;   shift;;
        timestamp)      do_timestamp=1; shift;;
        message)        do_message=1;   shift;;
        oplength)       do_opaque=1;    shift;;

        *)  echo ""
            echo "usage: ${myname} [-switches] iterations [tests ...]"
            echo ""
            echo "    valid tests are:"
            echo ""
            echo "        directory       perform directory tests"
            echo "        object          perform object tests"
            echo "        attribute       perform attribute tests"
            echo "        group           perform group tests"
            echo "        member          perform group member tests"
            echo "        replica         perform directory replica tests"
            echo "        softlink        perform softlink tests"
            echo "        expand          fullname expansion tests"
            echo "        convert         fullname string to opaque tests"
            echo "        timestamp       string to timestamp tests"
            echo "        message         convert error code to message string"
            echo "        oplength        Get length of opaque simple name"          
            echo ""
            echo "    valid switches are:"
            echo ""
            echo "        -cell name      specify cellname"
            echo "        -ch1  name      specify clearinghouse (primary)"
            echo "        -ch2  name      specify clearinghouse (secondary)"
            echo "        -ch3  name      specify create clearinghouse name"
            echo "        -dir  dirname   specify toplevel test directory"
            echo "        -inet address   specify internet address"
            echo "                        eg. -inet 16.20.16.32"
            echo ""
            echo "        -class name     specify classname for create object"
            echo "        -version vers   specify classversion for create object"
            echo ""
            echo "        -count          # of directories, objects, groups,"
            echo "                        members, softlinks to create"
            echo "                        (default value is 10)"
            echo ""
            echo "        -noch           strip clearinghouse stuff (all)"
            echo "        -noch1          strip clearinghouse stuff (primary)"
            echo "        -noch2          strip clearinghouse stuff (secondary)"
            echo "        -noch3          strip clearinghouse stuff (create)"
            echo ""
            echo "        -pid            use 'pid' \$\$ to ensure unique filenames"
            echo "        -debug          display but don't execute cdstest commands"
            echo "        -nodelete       don't delete what we create"
            echo ""
            exit 1
            ;;
    esac

    if [ $# -eq 0 ];
        then parsing="false"
    fi

  done

#
# Determine what actions have been requested and what others are
# therefore necessary.
#

# if nothing has been specified, assume all tests are to be done

if [ ${do_directory=0} -eq 0 -a ${do_object=0}   -eq 0 -a \
     ${do_attribute=0} -eq 0                           -a \
     ${do_group=0}     -eq 0 -a ${do_member=0}   -eq 0 -a \
     ${do_replica=0}   -eq 0 -a ${do_softlink=0} -eq 0 -a \
     ${do_expand=0}    -eq 0 -a ${do_convert=0}  -eq 0 -a \
     ${do_timestamp=0} -eq 0 -a ${do_message=0}  -eq 0 -a ${do_opaque=0}  -eq 0 ]
  then do_all=1
fi

# if all tests have been requested then set each test active

if [ ${do_all=0} -eq 1 ]; then
  do_directory=1
  do_object=1
  do_attribute=1
  do_group=1
  do_member=1
  do_replica=1
  do_softlink=1
  do_softlink_dir=1
  do_softlink_obj=1
  do_expand=1
  do_convert=1
  do_timestamp=1
  do_message=1
  do_opaque=1
fi

# make sure that certain test prerequisites are met

if [ ${do_all=0} -eq 0 ]; then

  # replica tests need directories

  if [ ${do_replica=0} -eq 1 ]; then
    do_directory=1
  fi

  # group member tests need groups

  if [ ${do_member=0} -eq 1 ]; then
    do_group=1
  fi

  # softlink tests need directories and/or objects

  if [ ${do_softlink=0} -eq 1 ]; then
    if [ ${do_directory} -eq 0 -a ${do_object} -eq 0 ]; then
      do_directory=1
    fi
    do_softlink_dir=${do_directory}
    do_softlink_obj=${do_object}
  fi

  # attribute tests need directories and/or objects

  if [ ${do_attribute=0} -eq 1 ]; then
    if [ ${do_directory} -eq 0 -a ${do_object} -eq 0 ]; then
      do_object=1
    fi
  fi
fi

# if no 'creation' type tests are specified then don't create
# a top level directory

if [ ${do_directory=0} -eq 1 -o ${do_object=0}   -eq 1 -o ${do_group=0} -eq 1 -o ${do_member=0} -eq 1 ]
  then do_create_top=1
  else do_create_top=0
fi

#
# Some definitions
#

workdir=`pwd`
cdstest_tmp=/tmp/${myname}$$.tmp
logfile=${workdir}/${myname}${pid}.log
exit_status=0
iterations=${iterations_arg=10}

#
# Initialize our logfile
#

echo "${prefix}Initializing logfile (${logfile}) ..."
echo "# Invocation: $0 $*" > ${logfile}
echo "# Date:       `date`" >> ${logfile}
echo "" >> ${logfile}

#
# Prepare to cleanup ourselves
#

trap "rm -f ${cdstest_tmp}; exit 1" 1 2 3 9 15

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
# Determine the toplevel test directory (ie. the testspace)
#

toptestdir=${toptestdir_arg="$USERNAME${pid}"}
echo "${prefix}toptestdir     : ${toptestdir}" | tee -a ${logfile}

testspace="${cellname}/${toptestdir}"
localtestspace="/.:/${toptestdir}"
echo "${prefix}testspace      : ${testspace}" | tee -a ${logfile}

#
# Determine our internet address
#
# Note: this is gross but it does the trick.  If perchance this should
#       cause problems on some systems, the workaround is to simply
#       comment this mess out and use the -inet x.y.x switch
#
ourhostname="$HOSTNAME"
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

clearinghouse1=${cellname}/${clearinghouse1_arg=`$HOSTNAME | cut -f1 -d.`_ch}
echo "${prefix}clearinghouse1 : ${clearinghouse1}" | tee -a ${logfile}

clearinghouse2=${cellname}/${clearinghouse2_arg="barf_ch"}
echo "${prefix}clearinghouse2 : ${clearinghouse2}" | tee -a ${logfile}

clearinghouse3=${cellname}/${clearinghouse3_arg=`$HOSTNAME | cut -f1 -d.`2_ch}
echo "${prefix}clearinghouse3 : ${clearinghouse3}" | tee -a ${logfile}

#
# Setup the create object classname and version
#

classname=${classname_arg="$USERNAME"}
echo "${prefix}classname      : ${classname}" | tee -a ${logfile}

classversion=${classversion_arg="1.0"}
echo "${prefix}classversion   : ${classversion}" | tee -a ${logfile}

#
# Setup the number of iterations
#

echo "${prefix}iterations     : ${iterations}" | tee -a ${logfile}

#
# Command line switches
#

debug=${debug_arg=0}
noch=${noch_arg=0}
noch1=${noch1_arg=0}
noch2=${noch2_arg=0}
noch3=${noch3_arg=0}
nodelete=${nodelete_arg=0}

#
# Setup the 'test' executable
#

if [ ${debug} -eq 0 ];
  then CDSTEST="cdstest"
  else CDSTEST="cat"
fi
echo "${prefix}CDSTEST        : ${CDSTEST}" | tee -a ${logfile}

#
##############################################################################
#
# Create the target directory for this test.
#
##############################################################################
#


if [ ${do_create_top} -eq 1 ];
  then
    (echo 'sd'; echo "${cellname}")                           >  ${cdstest_tmp}
    (echo 'cd'; echo "${testspace}";echo "${clearinghouse1}") >> ${cdstest_tmp}
    (echo 'sd'; echo "${testspace}")                          >> ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
           (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi
    rm -f ${cdstest_tmp}
fi

#
##############################################################################
#
# DIRECTORY TESTS
#
##############################################################################
#

if [ ${do_directory} -ne 0 ]; then

    # Create some directories.

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"cd\\n${testspace}/dir%08d\\n${clearinghouse1}\\n\", i); \
	exit }" > ${cdstest_tmp}

    # Skulks are good.

    (echo 'sd'; echo "${cellname}")  >> ${cdstest_tmp}
    (echo 'sd'; echo "${testspace}") >> ${cdstest_tmp}

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"sd\\n${testspace}/dir%08d\\n\", i); \
	exit }" >> ${cdstest_tmp}

    # Enumerate directories (children)

    (echo 'ec'; echo "${testspace}"; echo '*') >> ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
# | tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi
    rm -f ${cdstest_tmp}
fi

#
##############################################################################
#
# OBJECT TESTS
#
##############################################################################
#

if [ ${do_object} -ne 0 ]; then

    # Create some objects.

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"co\\n${testspace}/obj%08d\\n${classname}\\n${classversion}\\n\", i); \
	exit }" > ${cdstest_tmp}

    # Skulks are good.

    (echo 'sd'; echo "${cellname}")  >> ${cdstest_tmp}
    (echo 'sd'; echo "${testspace}") >> ${cdstest_tmp}

    # Enumerate objects.

    (echo 'eo'; echo "${testspace}"; echo '*'; echo "${classname}") >> ${cdstest_tmp}

    if [ ${do_directory} -ne 0 ]; then

        # Create some objects in those directories.

        awk "BEGIN { \
	  for (i=1; i<=${iterations}; i++) \
	    printf(\"co\\n${testspace}/dir%08d/obj%08d\\n${classname}\\n${classversion}\\n\", i, i); \
	  exit }" >> ${cdstest_tmp}

        # Skulks are good.

        (echo 'sd'; echo "${cellname}")  >> ${cdstest_tmp}
        (echo 'sd'; echo "${testspace}") >> ${cdstest_tmp}

        awk "BEGIN { \
	  for (i=1; i<=${iterations}; i++) \
	    printf(\"sd\\n${testspace}/dir%08d\\n\", i); \
	  exit }" >> ${cdstest_tmp}

        # Enumerate objects in directories.

        awk "BEGIN { \
          for (i=1; i<=${iterations}; i++) \
            printf(\"eo\\n${testspace}/dir%08d\\n*\\n${classname}\\n\", i); \
          exit }" >> ${cdstest_tmp}
    fi

    ${CDSTEST} <${cdstest_tmp} >> ${logfile} 
# | tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi
    rm -f ${cdstest_tmp}
fi

#
##############################################################################
#
# GROUP TESTS
#
##############################################################################
#

if [ ${do_group} -ne 0 ]; then

    # Create some groups.

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"cg\\n${testspace}/grp%08d\\n\", i); \
	exit }" > ${cdstest_tmp}

    # Skulks are good.

    (echo 'sd'; echo "${cellname}")  >> ${cdstest_tmp}
    (echo 'sd'; echo "${testspace}") >> ${cdstest_tmp}

    # Enumerate groups as groups.

    (echo 'eg'; echo "${testspace}"; echo '*') >> ${cdstest_tmp}

    # Enumerate groups as objects.

    (echo 'eo'; echo "${testspace}"; echo '*'; echo 'CDS_Group') >> ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp}  >> ${logfile}
# | tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi
    rm -f ${cdstest_tmp}
fi

#
##############################################################################
#
# GROUP MEMBER TESTS
#
##############################################################################
#

if [ do_${group_member} -ne 0 ]; then

    # Create some members.

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"am\\n${testspace}/grp00000001\\n${testspace}/mbr%08d\\n0\\n\", i); \
	exit }" > ${cdstest_tmp}

    awk "BEGIN { \
	for (i=2; i<=${iterations}; i++) \
	  printf(\"am\\n${testspace}/grp%08d\\n${testspace}/mbr%08d\\n0\\n\", i, i); \
	exit }" >> ${cdstest_tmp}

    # Skulks are good.

    (echo 'sd'; echo "${cellname}")  >> ${cdstest_tmp}
    (echo 'sd'; echo "${testspace}") >> ${cdstest_tmp}

    # Read members.

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"rm\\n${testspace}/grp%08d\\n\", i); \
	exit }" >> ${cdstest_tmp}

    # Test members.

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"tm\\n${testspace}/grp00000001\\n${testspace}/mbr%08d\\n\", i); \
	exit }" >> ${cdstest_tmp}

    awk "BEGIN { \
	for (i=2; i<=${iterations}; i++) \
	  printf(\"tm\\n${testspace}/grp%08d\\n${testspace}/mbr%08d\\n\", i, i); \
	exit }" >> ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi
    rm -f ${cdstest_tmp}
fi

#
##############################################################################
#
# REPLICA TESTS
#
##############################################################################
#

if [ ${do_replica} -ne 0 ]; then

    # Create some replicas of directories

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"ar\\n${testspace}/dir%08d\\n${clearinghouse2}\\n3\\n\", i); \
	exit }" > ${cdstest_tmp}

    # Skulks are good.

    (echo 'sd'; echo "${cellname}")  >> ${cdstest_tmp}
    (echo 'sd'; echo "${testspace}") >> ${cdstest_tmp}

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"sd\\n${testspace}/dir%08d\\n\", i); \
	exit }" >> ${cdstest_tmp}

    #
    # NOTE: Unfortunately, that's about all that we can do with replicas.
    #

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi
    rm -f ${cdstest_tmp}
fi

#
##############################################################################
#
# SOFTLINK TESTS
#
##############################################################################
#

if [ ${do_softlink} -ne 0 ]; then

    # Create some softlinks to directories and objects.

    if [ ${do_softlink_dir} -ne 0 ]; then
        awk "BEGIN { \
	  for (i=1; i<=${iterations}; i++) \
	    printf(\"cs\\n${testspace}/dir_lnk%08d\\n${testspace}/dir%08d\\n\\n\", i, i); \
	  exit }" > ${cdstest_tmp}
    fi

    if [ ${do_softlink_obj} -ne 0 ]; then
        awk "BEGIN { \
	  for (i=1; i<=${iterations}; i++) \
	    printf(\"cs\\n${testspace}/obj_lnk%08d\\n${testspace}/dir%08d/obj%08d\\n\\n\", i, i, i); \
	  exit }" >> ${cdstest_tmp}
    fi

    # Skulks are good.

    (echo 'sd'; echo "${cellname}")  >> ${cdstest_tmp}
    (echo 'sd'; echo "${testspace}") >> ${cdstest_tmp}

    if [ ${do_softlink_dir} -ne 0 ]; then
        awk "BEGIN { \
	  for (i=1; i<=${iterations}; i++) \
	    printf(\"sd\\n${testspace}/dir%08d\\n\", i); \
	  exit }" >> ${cdstest_tmp}
    fi

    # Enumerate directory softlinks.

    if [ ${do_softlink_dir} -ne 0 ]; then
        (echo 'es'; echo "${testspace}"; echo 'dir*') >> ${cdstest_tmp}
    fi

    # Enumerate object softlinks.

    if [ ${do_softlink_obj} -ne 0 ]; then
        (echo 'es'; echo "${testspace}"; echo 'obj*') >> ${cdstest_tmp}
    fi

    # Enumerate softlinks.

    (echo 'es'; echo "${testspace}"; echo '*')    >> ${cdstest_tmp}

    # Resolve directory softlinks.

    if [ ${do_softlink_dir} -ne 0 ]; then
        awk "BEGIN { \
	  for (i=1; i<=${iterations}; i++) \
	    printf(\"vn\\n${testspace}/dir_lnk%08d\\n\", i); \
	  exit }" >> ${cdstest_tmp}
    fi

    # Resolve object softlinks.

    if [ ${do_softlink_obj} -ne 0 ]; then
        awk "BEGIN { \
	  for (i=1; i<=${iterations}; i++) \
	    printf(\"vn\\n${testspace}/obj_lnk%08d\\n\", i); \
	  exit }" >> ${cdstest_tmp}
    fi

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi
    rm -f ${cdstest_tmp}
fi

#
##############################################################################
#
# ATTRIBUTE TESTS
#
##############################################################################
#

if [ ${do_attribute} -ne 0 ]; then

    # enumerate the attributes of some known objects

    (echo 'ea'; echo "${cellname}"; echo "1")       >> ${cdstest_tmp}
    (echo 'ea'; echo "${cellname}"; echo "7")       >> ${cdstest_tmp}
    (echo 'ea'; echo "${clearinghouse1}"; echo "2") >> ${cdstest_tmp}
    (echo 'ea'; echo "${clearinghouse1}"; echo "5") >> ${cdstest_tmp}
    (echo 'ea'; echo "${clearinghouse1}"; echo "7") >> ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
# | tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi
    rm -f ${cdstest_tmp}

    # enumerate directory attributes if we created any directories

    if [ ${do_directory} -ne 0 ]; then
        awk "BEGIN { \
	    for (i=1; i<=${iterations}; i++) \
              printf(\"ea\\n${testspace}/dir%08d\\n1\\n\", i); \
  	    exit }" > ${cdstest_tmp}

        ${CDSTEST} <${cdstest_tmp} >> ${logfile}
# | tee -a ${logfile}
        if [ $? -ne 0 ];
          then exit_status=1;
            (echo "";echo "${prefix}*** Exit status has been set";echo "") \
                 | tee -a ${logfile}
        fi

        awk "BEGIN { \
	    for (i=1; i<=${iterations}; i++) \
              printf(\"ea\\n${testspace}/dir%08d\\n3\\n\", i); \
  	    exit }" > ${cdstest_tmp}

        ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
        if [ $? -ne 0 ];
          then exit_status=1;
            (echo "";echo "${prefix}*** Exit status has been set";echo "") \
                 | tee -a ${logfile}
        fi

        awk "BEGIN { \
	    for (i=1; i<=${iterations}; i++) \
              printf(\"ea\\n${testspace}/dir%08d\\n7\\n\", i); \
  	    exit }" > ${cdstest_tmp}

        ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
        if [ $? -ne 0 ];
          then exit_status=1;
            (echo "";echo "${prefix}*** Exit status has been set";echo "") \
                 | tee -a ${logfile}
        fi
    fi
    rm -f ${cdstest_tmp}

    # enumerate object attributes if we created any objects

    if [ ${do_object} -ne 0 ]; then
        awk "BEGIN { \
	    for (i=1; i<=${iterations}; i++) \
	      printf(\"ea\\n${testspace}/obj%08d\\n2\\n\", i); \
	    exit }" > ${cdstest_tmp}

        ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
        if [ $? -ne 0 ];
          then exit_status=1;
            (echo "";echo "${prefix}*** Exit status has been set";echo "") \
                 | tee -a ${logfile}
        fi

        awk "BEGIN { \
	    for (i=1; i<=${iterations}; i++) \
	      printf(\"ea\\n${testspace}/obj%08d\\n7\\n\", i); \
	    exit }" > ${cdstest_tmp}

        ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
        if [ $? -ne 0 ];
          then exit_status=1;
            (echo "";echo "${prefix}*** Exit status has been set";echo "") \
                 | tee -a ${logfile}
        fi

        if [ ${do_directory} -ne 0 ]; then
            awk "BEGIN { \
	      for (i=1; i<=${iterations}; i++) \
	        printf(\"ea\\n${testspace}/dir%08d/obj%08d\\n2\\n\", i, i); \
	      exit }" > ${cdstest_tmp}

            ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
            if [ $? -ne 0 ];
              then exit_status=1;
                (echo "";echo "${prefix}*** Exit status has been set";echo "") \
                     | tee -a ${logfile}
            fi

            awk "BEGIN { \
	      for (i=1; i<=${iterations}; i++) \
	        printf(\"ea\\n${testspace}/dir%08d/obj%08d\\n7\\n\", i, i); \
	      exit }" > ${cdstest_tmp}

            ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
            if [ $? -ne 0 ];
              then exit_status=1;
                (echo "";echo "${prefix}*** Exit status has been set";echo "") \
                     | tee -a ${logfile}
            fi
        fi
    fi
    rm -f ${cdstest_tmp}

    # enumerate softlink attributes if we created any softlinks

    if [ ${do_softlink} -ne 0 ]; then

        if [ ${do_softlink_dir} -ne 0 ]; then
            awk "BEGIN { \
	      for (i=1; i<=${iterations}; i++) \
	        printf(\"ea\\n${testspace}/dir_lnk%08d\\n4\\n\", i); \
	      exit }" > ${cdstest_tmp}

            ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
            if [ $? -ne 0 ];
              then exit_status=1;
                   (echo "";echo "${prefix}*** Exit status has been set";echo "") \
                            | tee -a ${logfile}
            fi
        fi

        if [ ${do_softlink_obj} -ne 0 ]; then
            awk "BEGIN { \
	      for (i=1; i<=${iterations}; i++) \
	        printf(\"ea\\n${testspace}/obj_lnk%08d\\n4\\n\", i); \
	      exit }" > ${cdstest_tmp}

            ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
            if [ $? -ne 0 ];
              then exit_status=1;
                   (echo "";echo "${prefix}*** Exit status has been set";echo "") \
                            | tee -a ${logfile}
            fi
        fi
    fi
    rm -f ${cdstest_tmp}

    # read the attributes of some known objects and directories

    (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_CTS")                    >> ${cdstest_tmp}
    (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_UTS")                    >> ${cdstest_tmp}
    (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_ObjectUUID")             >> ${cdstest_tmp}
    (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_AllUpTo")                >> ${cdstest_tmp}
    (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_Convergence")            >> ${cdstest_tmp}
    (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_DirectoryVersion")       >> ${cdstest_tmp}
#    (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_InCHName")               >> ${cdstest_tmp}
#   (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_CHName")                 >> ${cdstest_tmp}
#   (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_CHLastAddress")          >> ${cdstest_tmp}
#   (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_CHState")                >> ${cdstest_tmp}
#   (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_CHDirectories")          >> ${cdstest_tmp}
    (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_Replicas")               >> ${cdstest_tmp}
    (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_ReplicaVersion")         >> ${cdstest_tmp}
    (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_ReplicaState")           >> ${cdstest_tmp}
    (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_ReplicaType")            >> ${cdstest_tmp}
    (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_LastSkulk")              >> ${cdstest_tmp}
    (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_LastUpdate")             >> ${cdstest_tmp}
    (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_RingPointer")            >> ${cdstest_tmp}
    (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_Epoch")                  >> ${cdstest_tmp}
    (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_GDAPointers")            >> ${cdstest_tmp}
#   (echo 'rv'; echo "${cellname}"; echo "1"; echo "CDS_NSCellname")             >> ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi
    rm -f ${cdstest_tmp}

    (echo 'rv'; echo "${clearinghouse1}"; echo "2"; echo "CDS_CTS")              >> ${cdstest_tmp}
    (echo 'rv'; echo "${clearinghouse1}"; echo "2"; echo "CDS_UTS")              >> ${cdstest_tmp}
    (echo 'rv'; echo "${clearinghouse1}"; echo "2"; echo "CDS_Class")            >> ${cdstest_tmp}
    (echo 'rv'; echo "${clearinghouse1}"; echo "2"; echo "CDS_ClassVersion")     >> ${cdstest_tmp}
    (echo 'rv'; echo "${clearinghouse1}"; echo "2"; echo "CDS_ObjectUUID")       >> ${cdstest_tmp}
    (echo 'rv'; echo "${clearinghouse1}"; echo "2"; echo "CDS_Towers")           >> ${cdstest_tmp}
#   (echo 'rv'; echo "${clearinghouse1}"; echo "2"; echo "CDS_CHDirectories")    >> ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi
    rm -f ${cdstest_tmp}

    (echo 'rv'; echo "${clearinghouse1}"; echo "5"; echo "CDS_CTS")              >> ${cdstest_tmp}
    (echo 'rv'; echo "${clearinghouse1}"; echo "5"; echo "CDS_UTS")              >> ${cdstest_tmp}
    (echo 'rv'; echo "${clearinghouse1}"; echo "5"; echo "CDS_ObjectUUID")       >> ${cdstest_tmp}
    (echo 'rv'; echo "${clearinghouse1}"; echo "5"; echo "CDS_AllUpTo")          >> ${cdstest_tmp}
    (echo 'rv'; echo "${clearinghouse1}"; echo "5"; echo "CDS_DirectoryVersion") >> ${cdstest_tmp}
    (echo 'rv'; echo "${clearinghouse1}"; echo "5"; echo "CDS_CHName")           >> ${cdstest_tmp}
    (echo 'rv'; echo "${clearinghouse1}"; echo "5"; echo "CDS_CHLastAddress")    >> ${cdstest_tmp}
    (echo 'rv'; echo "${clearinghouse1}"; echo "5"; echo "CDS_CHState")          >> ${cdstest_tmp}
#   (echo 'rv'; echo "${clearinghouse1}"; echo "5"; echo "CDS_CHDirectories")    >> ${cdstest_tmp}
    (echo 'rv'; echo "${clearinghouse1}"; echo "5"; echo "CDS_ReplicaVersion")   >> ${cdstest_tmp}
    (echo 'rv'; echo "${clearinghouse1}"; echo "5"; echo "CDS_NSCellname")       >> ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi
    rm -f ${cdstest_tmp}

    # read directory attributes if we created any directories

    if [ ${do_directory} -ne 0 ]; then
        awk "BEGIN { \
	    for (i=1; i<=${iterations}; i++) { \
              printf(\"rv\\n${testspace}/dir%08d\\n1\\nCDS_CTS\\n\", i); \
              printf(\"rv\\n${testspace}/dir%08d\\n1\\nCDS_UTS\\n\", i); \
              printf(\"rv\\n${testspace}/dir%08d\\n1\\nCDS_ObjectUUID\\n\", i); \
              printf(\"rv\\n${testspace}/dir%08d\\n1\\nCDS_AllUpTo\\n\", i); \
              printf(\"rv\\n${testspace}/dir%08d\\n1\\nCDS_Convergence\\n\", i); \
              printf(\"rv\\n${testspace}/dir%08d\\n1\\nCDS_DirectoryVersion\\n\", i); \
              printf(\"rv\\n${testspace}/dir%08d\\n1\\nCDS_Replicas\\n\", i); \
              printf(\"rv\\n${testspace}/dir%08d\\n1\\nCDS_ReplicaVersion\\n\", i); \
              printf(\"rv\\n${testspace}/dir%08d\\n1\\nCDS_ReplicaState\\n\", i); \
              printf(\"rv\\n${testspace}/dir%08d\\n1\\nCDS_ReplicaType\\n\", i); \
              printf(\"rv\\n${testspace}/dir%08d\\n1\\nCDS_LastSkulk\\n\", i); \
              printf(\"rv\\n${testspace}/dir%08d\\n1\\nCDS_LastUpdate\\n\", i); \
              printf(\"rv\\n${testspace}/dir%08d\\n1\\nCDS_RingPointer\\n\", i); \
              printf(\"rv\\n${testspace}/dir%08d\\n1\\nCDS_Epoch\\n\", i); \
            } \
	    exit }" > ${cdstest_tmp}

        ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
        if [ $? -ne 0 ];
          then exit_status=1;
            (echo "";echo "${prefix}*** Exit status has been set";echo "") \
                 | tee -a ${logfile}
        fi
    fi

    # read softlink attributes if we created any softlinks

    if [ ${do_softlink} -ne 0 ]; then

        if [ ${do_softlink_dir} -ne 0 ]; then
            awk "BEGIN { \
	      for (i=1; i<=${iterations}; i++) { \
	        printf(\"rv\\n${testspace}/dir_lnk%08d\\n4\\nCDS_CTS\\n\", i); \
              } \
	      exit }" > ${cdstest_tmp}

            ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
            if [ $? -ne 0 ];
              then exit_status=1;
                   (echo "";echo "${prefix}*** Exit status has been set";echo "") \
                            | tee -a ${logfile}
            fi
        fi

        if [ ${do_softlink_obj} -ne 0 ]; then
            awk "BEGIN { \
	      for (i=1; i<=${iterations}; i++) { \
	        printf(\"rv\\n${testspace}/obj_lnk%08d\\n4\\nCDS_CTS\\n\", i); \
              } \
	      exit }" > ${cdstest_tmp}

            ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
            if [ $? -ne 0 ];
              then exit_status=1;
                   (echo "";echo "${prefix}*** Exit status has been set";echo "") \
                            | tee -a ${logfile}
            fi
        fi
    fi

    # read object attributes if we created any objects

    if [ ${do_object} -ne 0 ]; then
        awk "BEGIN { \
	    for (i=1; i<=${iterations}; i++) { \
	      printf(\"rv\\n${testspace}/obj%08d\\n2\\nCDS_Class\\n\", i); \
	      printf(\"rv\\n${testspace}/obj%08d\\n2\\nCDS_ClassVersion\\n\", i); \
            } \
	    exit }" > ${cdstest_tmp}

        ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
        if [ $? -ne 0 ];
          then exit_status=1;
            (echo "";echo "${prefix}*** Exit status has been set";echo "") \
                 | tee -a ${logfile}
        fi

        if [ ${do_directory} -ne 0 ]; then
            awk "BEGIN { \
	      for (i=1; i<=${iterations}; i++) { \
	        printf(\"rv\\n${testspace}/dir%08d/obj%08d\\n2\\nCDS_Class\\n\", i, i); \
	        printf(\"rv\\n${testspace}/dir%08d/obj%08d\\n2\\nCDS_ClassVersion\\n\", i, i); \
              } \
	      exit }" > ${cdstest_tmp}

            ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
            if [ $? -ne 0 ];
              then exit_status=1;
                (echo "";echo "${prefix}*** Exit status has been set";echo "") \
                     | tee -a ${logfile}
            fi
        fi
    fi

    # add/read/test object attributes if we created any objects

    if [ ${do_object} -ne 0 ]; then
        awk "BEGIN { \
	    for (i=1; i<=${iterations}; i++) { \
              printf(\"av\\n${testspace}/obj%08d\\n2\\n1.10.12.13.14.15\\n0\\nAttrVal_10_%08d\\n\", i, i); \
              printf(\"av\\n${testspace}/obj%08d\\n2\\n1.11.12.13.14.15\\n0\\nAttrVal_11_%08d\\n\", i, i); \
              printf(\"av\\n${testspace}/obj%08d\\n2\\n1.12.12.13.14.15\\n0\\nAttrVal_12_%08d\\n\", i, i); \
              printf(\"av\\n${testspace}/obj%08d\\n2\\n1.13.12.13.14.15\\n0\\nAttrVal_13_%08d\\n\", i, i); \
              printf(\"av\\n${testspace}/obj%08d\\n2\\n1.14.12.13.14.15\\n0\\nAttrVal_14_%08d\\n\", i, i); \
              printf(\"av\\n${testspace}/obj%08d\\n2\\n1.15.12.13.14.15\\n0\\nAttrVal_15_%08d\\n\", i, i); \
              printf(\"av\\n${testspace}/obj%08d\\n2\\n1.16.12.13.14.15\\n0\\nAttrVal_16_%08d\\n\", i, i); \
              printf(\"av\\n${testspace}/obj%08d\\n2\\n1.17.12.13.14.15\\n0\\nAttrVal_17_%08d\\n\", i, i); \
              printf(\"av\\n${testspace}/obj%08d\\n2\\n1.18.12.13.14.15\\n0\\nAttrVal_18_%08d\\n\", i, i); \
              printf(\"av\\n${testspace}/obj%08d\\n2\\n1.19.12.13.14.15\\n0\\nAttrVal_19_%08d\\n\", i, i); \
            } \
	    exit }" > ${cdstest_tmp}

        (echo 'sd'; echo "${testspace}") >> ${cdstest_tmp}

        ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
        if [ $? -ne 0 ];
          then exit_status=1;
            (echo "";echo "${prefix}*** Exit status has been set";echo "") \
                 | tee -a ${logfile}
        fi

        awk "BEGIN { \
	    for (i=1; i<=${iterations}; i++) { \
              printf(\"tv\\n${testspace}/obj%08d\\n2\\n1.10.12.13.14.15\\nAttrVal_10_%08d\\n\", i, i); \
              printf(\"tv\\n${testspace}/obj%08d\\n2\\n1.11.12.13.14.15\\nAttrVal_11_%08d\\n\", i, i); \
              printf(\"tv\\n${testspace}/obj%08d\\n2\\n1.12.12.13.14.15\\nAttrVal_12_%08d\\n\", i, i); \
              printf(\"tv\\n${testspace}/obj%08d\\n2\\n1.13.12.13.14.15\\nAttrVal_13_%08d\\n\", i, i); \
              printf(\"tv\\n${testspace}/obj%08d\\n2\\n1.14.12.13.14.15\\nAttrVal_14_%08d\\n\", i, i); \
              printf(\"tv\\n${testspace}/obj%08d\\n2\\n1.15.12.13.14.15\\nAttrVal_15_%08d\\n\", i, i); \
              printf(\"tv\\n${testspace}/obj%08d\\n2\\n1.16.12.13.14.15\\nAttrVal_16_%08d\\n\", i, i); \
              printf(\"tv\\n${testspace}/obj%08d\\n2\\n1.17.12.13.14.15\\nAttrVal_17_%08d\\n\", i, i); \
              printf(\"tv\\n${testspace}/obj%08d\\n2\\n1.18.12.13.14.15\\nAttrVal_18_%08d\\n\", i, i); \
              printf(\"tv\\n${testspace}/obj%08d\\n2\\n1.19.12.13.14.15\\nAttrVal_19_%08d\\n\", i, i); \
            } \
	    exit }" > ${cdstest_tmp}

        ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
        if [ $? -ne 0 ];
          then exit_status=1;
            (echo "";echo "${prefix}*** Exit status has been set";echo "") \
                 | tee -a ${logfile}
        fi

        awk "BEGIN { \
	    for (i=1; i<=${iterations}; i++) { \
              printf(\"rv\\n${testspace}/obj%08d\\n2\\n1.10.12.13.14.15\\n\", i); \
              printf(\"rv\\n${testspace}/obj%08d\\n2\\n1.11.12.13.14.15\\n\", i); \
              printf(\"rv\\n${testspace}/obj%08d\\n2\\n1.12.12.13.14.15\\n\", i); \
              printf(\"rv\\n${testspace}/obj%08d\\n2\\n1.13.12.13.14.15\\n\", i); \
              printf(\"rv\\n${testspace}/obj%08d\\n2\\n1.14.12.13.14.15\\n\", i); \
              printf(\"rv\\n${testspace}/obj%08d\\n2\\n1.15.12.13.14.15\\n\", i); \
              printf(\"rv\\n${testspace}/obj%08d\\n2\\n1.16.12.13.14.15\\n\", i); \
              printf(\"rv\\n${testspace}/obj%08d\\n2\\n1.17.12.13.14.15\\n\", i); \
              printf(\"rv\\n${testspace}/obj%08d\\n2\\n1.18.12.13.14.15\\n\", i); \
              printf(\"rv\\n${testspace}/obj%08d\\n2\\n1.19.12.13.14.15\\n\", i); \
            } \
	    exit }" > ${cdstest_tmp}

        ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
        if [ $? -ne 0 ];
          then exit_status=1;
            (echo "";echo "${prefix}*** Exit status has been set";echo "") \
                 | tee -a ${logfile}
        fi
    fi
    rm -f ${cdstest_tmp}
fi

#
##############################################################################
#
# Expand fullnames.
#
# Note: I originally built one complete script with several
#       invocations of awk but some of stdout from cdstest would get
#       dropped by tee so I've broken this into several smaller
#       chunks.
#
##############################################################################
#

if [ ${do_expand} -ne 0 ]
  then
    (echo 'ne'; echo "${cellname}")   > ${cdstest_tmp}
    (echo 'ne'; echo "${testspace}") >> ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"ne\\n${localtestspace}/dir%08d\\n\", i); \
	exit }" > ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"ne\\n${localtestspace}/obj%08d\\n\", i); \
	exit }" > ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"ne\\n${localtestspace}/dir%08d/obj%08d\\n\", i, i); \
	exit }" > ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"ne\\n${localtestspace}/grp%08d\\n\", i); \
	exit }" > ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"ne\\n${localtestspace}/mbr%08d\\n\", i); \
	exit }" > ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"ne\\n${localtestspace}/obj_lnk%08d\\n\", i); \
	exit }" > ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"ne\\n${localtestspace}/dir_lnk%08d\\n\", i); \
	exit }" > ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi

    rm -f ${cdstest_tmp}
fi

#
##############################################################################
#
# Convert fullnames to opaque names.
#
# Note: I originally built one complete script with several
#       invocations of awk but some of stdout from cdstest would get
#       dropped by tee so I've broken this into several smaller
#       chunks.
#
##############################################################################
#

if [ ${do_convert} -ne 0 ]
  then
    (echo 'cn'; echo "${cellname}")   > ${cdstest_tmp}
    (echo 'cn'; echo "${testspace}") >> ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"cn\\n${testspace}/dir%08d\\n\", i); \
	exit }" | ${CDSTEST} >> ${logfile}
#| tee -a ${logfile}

    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"cn\\n${testspace}/obj%08d\\n\", i); \
	exit }" | ${CDSTEST} >> ${logfile}
#| tee -a ${logfile}

    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"cn\\n${testspace}/dir%08d/obj%08d\\n\", i, i); \
	exit }" | ${CDSTEST} >> ${logfile}
#| tee -a ${logfile}

    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"cn\\n${testspace}/grp%08d\\n\", i); \
	exit }" | ${CDSTEST} >> ${logfile}
#| tee -a ${logfile}

    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"cn\\n${testspace}/mbr%08d\\n\", i); \
	exit }" | ${CDSTEST} >> ${logfile}
#| tee -a ${logfile}

    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"cn\\n${testspace}/obj_lnk%08d\\n\", i); \
	exit }" | ${CDSTEST} >> ${logfile}
#| tee -a ${logfile}

    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi

    awk "BEGIN { \
	for (i=1; i<=${iterations}; i++) \
	  printf(\"cn\\n${testspace}/dir_lnk%08d\\n\", i); \
	exit }" | ${CDSTEST} >> ${logfile}
#| tee -a ${logfile}

    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi

#| tee -a ${logfile}
    ${CDSTEST} <<END >> ${logfile}
gn
/.../c=us/o=osf/ou=foo/fum
gn
/.../c=us,o=osf/cn=xxx
gn
/.../c=gb/o=X\/Open/ou=foo/fum
gn
/.../c=us/o=osf\,cn/ou=foo/fum
gn
/.../c=us/o=embed\\\\bs
gn
/.../c=us/o=embed\=eq/xxx
gn
/.../C=Us/o=oSf/Ou=foo/Fum
END

    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi
    rm -f ${cdstest_tmp}
fi

#
##############################################################################
#
# Delete everything we created.
#
##############################################################################
#

# Skulks are good.

if [ ${do_create_top} -eq 1 ];
  then
    (echo 'sd'; echo "${cellname}")  > ${cdstest_tmp}
    (echo 'sd'; echo "${testspace}") >> ${cdstest_tmp}
fi

# See if we have been requested to not delete everything we created

if [ ${nodelete} -eq 0 ];
  then

    if [ ${do_directory} -ne 0 ]
      then
        awk "BEGIN { \
    	    for (i=1; i<=${iterations}; i++) \
    	      printf(\"sd\\n${testspace}/dir%08d\\n\", i); \
    	    exit }" >> ${cdstest_tmp}
    fi

    if [ ${do_replica} -ne 0 ]
      then
        # Delete replicas
    
        awk "BEGIN { \
            for (i=1; i<=${iterations}; i++) \
              printf(\"dr\\n${testspace}/dir%08d\\n${clearinghouse2}\\n\", i); \
            exit }" >> ${cdstest_tmp}
    fi
    
    if [ do_${group_member} -ne 0 ]
      then
        # Delete group members.
    
        awk "BEGIN { \
            for (i=1; i<=${iterations}; i++) \
              printf(\"dm\\n${testspace}/grp00000001\\n${testspace}/mbr%08d\\nroot\\n\", i); \
            exit }" >> ${cdstest_tmp}
    
        awk "BEGIN { \
            for (i=2; i<=${iterations}; i++) \
              printf(\"dm\\n${testspace}/grp%08d\\n${testspace}/mbr%08d\\nroot\\n\", i, i); \
            exit }" >> ${cdstest_tmp}
    fi
    
    if [ ${do_group} -ne 0 ]
      then
        # Delete groups.
    
        awk "BEGIN { \
            for (i=1; i<=${iterations}; i++) \
              printf(\"dg\\n${testspace}/grp%08d\\n\", i); \
            exit }" >> ${cdstest_tmp}
    fi
    
    if [ ${do_softlink} -ne 0 ]
      then
        # Delete softlinks.
    
        awk "BEGIN { \
            for (i=1; i<=${iterations}; i++) \
              printf(\"ds\\n${testspace}/dir_lnk%08d\\n\", i); \
            exit }" >> ${cdstest_tmp}
    
        awk "BEGIN { \
            for (i=1; i<=${iterations}; i++) \
              printf(\"ds\\n${testspace}/obj_lnk%08d\\n\", i); \
            exit }" >> ${cdstest_tmp}
    fi
    
    if [ ${do_attribute} -ne 0 ]
      then
        # Delete attributes.
    
        if [ ${do_object} -ne 0 ]; then
            awk "BEGIN { \
                for (i=1; i<=${iterations}; i++) { \
                  printf(\"dv\\n${testspace}/obj%08d\\n2\\n1.10.12.13.14.15\\nAttrVal_0_%08d\\n\", i, i); \
                  printf(\"dv\\n${testspace}/obj%08d\\n2\\n1.11.12.13.14.15\\nAttrVal_1_%08d\\n\", i, i); \
                  printf(\"dv\\n${testspace}/obj%08d\\n2\\n1.12.12.13.14.15\\nAttrVal_2_%08d\\n\", i, i); \
                  printf(\"dv\\n${testspace}/obj%08d\\n2\\n1.13.12.13.14.15\\nAttrVal_3_%08d\\n\", i, i); \
                  printf(\"dv\\n${testspace}/obj%08d\\n2\\n1.14.12.13.14.15\\nAttrVal_4_%08d\\n\", i, i); \
                  printf(\"dv\\n${testspace}/obj%08d\\n2\\n1.15.12.13.14.15\\nAttrVal_5_%08d\\n\", i, i); \
                  printf(\"dv\\n${testspace}/obj%08d\\n2\\n1.16.12.13.14.15\\nAttrVal_6_%08d\\n\", i, i); \
                  printf(\"dv\\n${testspace}/obj%08d\\n2\\n1.17.12.13.14.15\\nAttrVal_7_%08d\\n\", i, i); \
                  printf(\"dv\\n${testspace}/obj%08d\\n2\\n1.18.12.13.14.15\\nAttrVal_8_%08d\\n\", i, i); \
                  printf(\"dv\\n${testspace}/obj%08d\\n2\\n1.19.12.13.14.15\\nAttrVal_9_%08d\\n\", i, i); \
                } \
                exit }" >> ${cdstest_tmp}
    
            awk "BEGIN { \
                for (i=1; i<=${iterations}; i++) { \
                  printf(\"da\\n${testspace}/obj%08d\\n2\\n1.10.12.13.14.15\\n0\\n\", i); \
                  printf(\"da\\n${testspace}/obj%08d\\n2\\n1.11.12.13.14.15\\n0\\n\", i); \
                  printf(\"da\\n${testspace}/obj%08d\\n2\\n1.12.12.13.14.15\\n0\\n\", i); \
                  printf(\"da\\n${testspace}/obj%08d\\n2\\n1.13.12.13.14.15\\n0\\n\", i); \
                  printf(\"da\\n${testspace}/obj%08d\\n2\\n1.14.12.13.14.15\\n0\\n\", i); \
                  printf(\"da\\n${testspace}/obj%08d\\n2\\n1.15.12.13.14.15\\n0\\n\", i); \
                  printf(\"da\\n${testspace}/obj%08d\\n2\\n1.16.12.13.14.15\\n0\\n\", i); \
                  printf(\"da\\n${testspace}/obj%08d\\n2\\n1.17.12.13.14.15\\n0\\n\", i); \
                  printf(\"da\\n${testspace}/obj%08d\\n2\\n1.18.12.13.14.15\\n0\\n\", i); \
                  printf(\"da\\n${testspace}/obj%08d\\n2\\n1.19.12.13.14.15\\n0\\n\", i); \
                } \
                exit }" >> ${cdstest_tmp}
        fi
    fi
    
    if [ ${do_object} -ne 0 ]
      then
        # Delete objects.
    
        awk "BEGIN { \
            for (i=1; i<=${iterations}; i++) \
              printf(\"do\\n${testspace}/obj%08d\\n\", i); \
            exit }" >> ${cdstest_tmp}
    
        if [ ${do_directory} -ne 0 ]
          then
            awk "BEGIN { \
              for (i=1; i<=${iterations}; i++) \
                printf(\"do\\n${testspace}/dir%08d/obj%08d\\n\", i, i); \
              exit }" >> ${cdstest_tmp}
        fi
    fi
    
    if [ ${do_directory} -ne 0 ]
      then
        # Skulks are good.
    
        (echo 'sd'; echo "${cellname}")  >> ${cdstest_tmp}
        (echo 'sd'; echo "${testspace}") >> ${cdstest_tmp}
    
        awk "BEGIN { \
            for (i=1; i<=${iterations}; i++) \
              printf(\"sd\\n${testspace}/dir%08d\\n\", i); \
            exit }" >> ${cdstest_tmp}
    
        # Delete directories
    
        awk "BEGIN { \
            for (i=1; i<=${iterations}; i++) \
              printf(\"dd\\n${testspace}/dir%08d\\n\", i); \
            exit }" >> ${cdstest_tmp}
    fi
    
    # Skulks are good.
    
    if [ ${do_create_top} -eq 1 ];
      then
        (echo 'sd'; echo "${cellname}")  >> ${cdstest_tmp}
        (echo 'sd'; echo "${testspace}") >> ${cdstest_tmp}
    fi

fi # end of if nodelete

if [ -f ${cdstest_tmp} ];
  then
    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
           (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi
    rm -f ${cdstest_tmp}
fi

##############################################################################
#
# TIMESTAMP Tests
#
##############################################################################
#

if [ ${do_timestamp} -ne 0 ]
  then

  ${CDSTEST} <<END >> ${logfile}
ti
12-34-56-78-90-ab-cd-ef-12-34-45-67-89-0a
ti
12345678900987654321abcdef01
END

    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi
    rm -f ${cdstest_tmp}
fi

##############################################################################
#
# Getmessage Tests
#
##############################################################################
#

if [ ${do_message} -ne 0 ]
  then

  #  
  # 282108910 - Cannot authenticate
  # 282108916 - Unknown entry
  # 282108921 - No replica in clearinghouse
  # 111111111 - No text for error code.
  #

  ${CDSTEST} <<END >> ${logfile}
gm
282108910
gm
282108916
gm
282108921
gm
111111111
END

    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi
    rm -f ${cdstest_tmp}
fi


##############################################################################
#
# OpaqueLength Tests
#
##############################################################################
#

if [ ${do_opaque} -ne 0 ]
  then

# 
# The opaque length will always be 2 greater than the length of the first simple name
#
# hosts - length: 7
# hosts/iowa - length: 7  Uses only the first simple name
# /sec/test  - length: 5
# /.:/sec    - length: 4  Uses .: as the first simple name
# /.../hosts/test - length 5  Uses ... as the first simple name
#

  ${CDSTEST} <<END >> ${logfile}
op
hosts
op
hosts/iowa
op
/sec/test
op
/.:/sec
op
/.../hosts/test
END

    if [ $? -ne 0 ];
      then exit_status=1;
        (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi
    rm -f ${cdstest_tmp}
fi

#
##############################################################################
#
# Delete the target directory for this test.
#
##############################################################################
#

if [ ${do_create_top} -eq 1 -a ${nodelete} -eq 0 ];
  then
    (echo 'dd'; echo "${testspace}") > ${cdstest_tmp}
    (echo 'sd'; echo "${cellname}") >> ${cdstest_tmp}

    ${CDSTEST} <${cdstest_tmp} >> ${logfile}
#| tee -a ${logfile}
    if [ $? -ne 0 ];
      then exit_status=1;
           (echo "";echo "${prefix}*** Exit status has been set";echo "") | tee -a ${logfile}
    fi
    rm -f ${cdstest_tmp}
fi

#
# Capture some more statistics
#

echo "${prefix}Capturing ending statistics ..."   | tee -a ${logfile}
echo ""                                                 >> ${logfile}
echo "#"                                                >> ${logfile}
echo "# `date` - Ending statistics"                     >> ${logfile}
echo "#"                                                >> ${logfile}
echo ""                                                 >> ${logfile}
#df                                                      >> ${logfile}
echo ""                                                 >> ${logfile}
#/etc/pstat -s                                           >> ${logfile}
echo ""                                                 >> ${logfile}
ipcs                                                    >> ${logfile}
echo ""                                                 >> ${logfile}
$PS $PSFLAGS | head -1                                        >> ${logfile}
$PS $PSFLAGS | grep 'cds'                                     >> ${logfile}

#
# display our overall status and exit with status
#

(echo "";echo "${prefix}Done testing.";echo "") | tee -a ${logfile}

if [ ${exit_status} -eq 0 ];
  then echo "${prefix}No errors were detected." | tee -a ${logfile}
  else echo "${prefix}Errors were detected." | tee -a ${logfile}
fi

exit ${exit_status}

