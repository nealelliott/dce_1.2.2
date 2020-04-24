# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: tp.sh,v $
# Revision 1.1.7.2  1996/03/11  01:45:59  marty
# 	Update OSF copyright years.
# 	[1996/03/11  01:35:02  marty]
#
# Revision 1.1.7.1  1995/12/08  22:11:12  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:44:45  root]
# 
# Revision 1.1.4.2  1993/01/12  18:31:31  bbelch
# 	Embedding copyright notice
# 	[1993/01/07  01:15:05  bbelch]
# 
# Revision 1.1.2.2  1992/06/08  19:59:46  weisman
# 		Original submission.
# 	[1992/06/08  19:50:33  weisman]
# 
# $EndLog$

#!/bin/sh
myname=`basename $0`
myhome=`pwd`

###############################################################################
#
# tp.sh - Top-Level Shell Script to run the CDS tests
#
# revision history:
# -----------------
# 09-feb-92 mbs original version
# 17-feb-92 mbs /opt/dcelocal/etc/cdsd.pid is no longer used
#               /opt/dcelocal/var/rpc/rpcd_mumble.dat files too
# 19-feb-92 mbs add "tee" to ${logfile}
# 26-feb-92 mbs moved test directory to /opt/dcelocal/test
#               conditionally use -pid for cdstest.sh
# 05-mar-92 mbs 'untabify' our usage messages
#               tighten-up our switch statement
#               accommodate new cdstest.sh commandline
# 06-mar-92 mbs fix exit status handling
###############################################################################

#
# only root can run this script
#

if [ "`whoami`" != "root" ]; then
    echo "You must be 'root' to run this script!!!"
    exit 1
fi

#
# we need to come to a consensus around where we should expect to
# execute this test from
#
dce_tstdir="/opt/dcelocal/test"
echo "Test directory is ${dce_tstdir}"
if [ ! -d ${dce_tstdir} ]; then
    echo "${dce_tstdir} directory must exist to run this script!!!"
    exit 1
fi
cd ${dce_tstdir}

#
# ensure we have our PATH setup correctly
#

PATH=.:/opt/dcelocal/bin:/bin:/usr/bin:/etc:/usr/ucb:/usr/local/bin
export PATH

###############################################################################
#
# Check invocation
#
###############################################################################

if [ $# -eq 0 ];
    then parsing="false"
    else parsing="true"
fi

while ${parsing}
  do

    case $1 in

	-ch1)	    clearinghouse1_arg=$2;      shift;shift;;
	-ch2)	    clearinghouse2_arg=$2;      shift;shift;;
	-ch3)	    clearinghouse3_arg=$2;      shift;shift;;
	-cell)	    cellname_arg=$2;            shift;shift;;
	-dir)       toptestdir_arg=$2;          shift;shift;;
        -inet)      inetaddr_arg=$2;            shift;shift;;

	-pid)       pid=$$
		    prefix="${pid}>"
                    pid_switch="-pid"
		    shift
		    ;;

        -disable)   disable_arg=1;      shift;;
	-noshow)    noshow_arg=1;       shift;;
	-noskulk)   noskulk_arg=1;      shift;;
	-nodeldir)  nodeldir_arg=1;     shift;;
	-noch)      noch_arg=1;         shift;;
	-noch1)     noch1_arg=1;        shift;;
	-noch2)     noch2_arg=1;        shift;;
	-noch3)     noch3_arg=1;        shift;;
	-nopipe)    nopipe_arg=1;       shift;;

        *)  echo ""
            echo "usage: ${myname} [-switches] testname"
            echo ""
            echo "    valid switches are:"
            echo ""
            echo "      -cell name      specify cellname"
            echo "      -ch1  name      specify clearinghouse (primary)"
            echo "      -ch2  name      specify clearinghouse (secondary)"
            echo "      -ch3  name      specify create clearinghouse name"
            echo "      -dir  dirname   specify toplevel test directory"
            echo "      -inet address   specify internet address"
            echo "                      eg. -inet 16.20.16.32"
            echo ""
            echo "      -noshow         strip show commands from scripts"
            echo "      -noskulk        strip skulk commands from scripts"
            echo "      -nodeldir       strip deldir commands from scripts"
            echo "      -noch           strip clearinghouse stuff (all)"
            echo "      -noch1          strip clearinghouse stuff (primary)"
            echo "      -noch2          strip clearinghouse stuff (secondary)"
            echo "      -noch3          strip clearinghouse stuff (create)"
            echo "      -nopipe         don't pipe commands to 'cdscp'"
            echo ""
            echo "      -disable        don't strip disable commands from scripts"
            echo "      -pid            use 'pid' \$\$ to ensure unique filenames"
            echo ""
            exit 1
	    ;;

    esac

    if [ $# -eq 0 ];
        then parsing="false"
    fi
  done

###############################################################################
#
# Top-Level Test Configuration
#
###############################################################################

#
# Some definitions
#

workdir=`pwd`
logfile=${myhome}/${myname}${pid}.log
whoami=`whoami`

#
# Initialize our status variables
#

status_overall=0
status_cdstest=0
status_killer=0
status_dcesx=0

#
# Initialize our logfile
#

echo "${prefix}Initializing logfile (${logfile}) ..."
echo "# Invocation: $0 $*" | tee ${logfile}
echo "# Date:       `date`" | tee -a ${logfile}
echo "" | tee -a ${logfile}

#
# Prepare to cleanup ourselves
#

#trap "rm -f ${testscript} ${testlines}; exit 1" 1 2 3 9 15

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

ourhostname_prefix="`hostname | cut -f1 -d.`"
echo "${prefix}hostname prefix: ${ourhostname_prefix}" | tee -a ${logfile}

ouraddress=`arp $ourhostname | sed 's/.*(\(.*\)).*/\1/'`
#ouraddress=`nslookup ${ourhostname} | \
#        sed -e "1,/${ourhostname}/d" -e "s/Address:  //g"`
#
inetaddr=${inetaddr_arg="${ouraddress}"}
echo "${prefix}internet_addr  : ${inetaddr}" | tee -a ${logfile}

#
# Setup the clearinghouse references
#

clearinghouse1=${clearinghouse1_arg="`hostname | cut -f1 -d.`_ch"}
echo "${prefix}clearinghouse1 : ${clearinghouse1}" | tee -a ${logfile}

clearinghouse2=${clearinghouse2_arg="barf_ch"}
echo "${prefix}clearinghouse2 : ${clearinghouse2}" | tee -a ${logfile}

clearinghouse3=${clearinghouse3_arg=`hostname | cut -f1 -d.`2_ch}
echo "${prefix}clearinghouse3 : ${clearinghouse3}" | tee -a ${logfile}

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

nopipe=${nopipe_arg=0}
disable=${disable_arg=0}
if [ ${disable} -ne 0 ];
  then nodisable=0
  else nodisable=1
fi

#
# Capture some statistics
#

echo ""
echo -n "${prefix}Capturing starting statistics ..."
echo "#"                                                >> ${logfile}
echo "# `date` - Starting statistics"                   >> ${logfile}
echo "#"                                                >> ${logfile}
echo ""                                                 >> ${logfile}
df                                                      >> ${logfile}
echo ""                                                 >> ${logfile}
/etc/pstat -s                                           >> ${logfile}
echo ""                                                 >> ${logfile}
ipcs                                                    >> ${logfile}
echo ""                                                 >> ${logfile}
ps -aux | head -1                                       >> ${logfile}
ps -aux | grep 'cds'                                    >> ${logfile}
echo " done."

###############################################################################
#
(echo "";echo "Installation/Configuration Verification") | tee -a ${logfile}
#
###############################################################################

#
# define some directories
#

dce_topdir="/opt/dcelocal"

dce_bindir="/opt/dcelocal/bin"
dce_etcdir="/opt/dcelocal/etc"
dce_nlsdir="/opt/dcelocal/nls"

dce_admdir="/opt/dcelocal/var/adm/directory/cds"
dce_vardir="/opt/dcelocal/var/directory/cds"
dce_rpcdir="/opt/dcelocal/var/rpc"
dce_catdir="/opt/dcelocal/nls/msg/en_US.ASCII"

#
# define some file lists
#

test_executables="\
        ${dce_bindir}/cadump \
        ${dce_bindir}/catraverse \
        ${dce_bindir}/cdsadv \
        ${dce_bindir}/cdscp \
        ${dce_bindir}/cdsd \
        ${dce_bindir}/cdsd_diag \
        ${dce_bindir}/cdstest \
        ${dce_bindir}/gdad \
        ${dce_bindir}/rpcd \
        "

test_configfiles="\
        ${dce_topdir}/dce_cf.db \
        ${dce_etcdir}/cds_attributes \
        ${dce_etcdir}/cds_globalnames \
        ${dce_etcdir}/cdscp.bpt \
        ${dce_etcdir}/cdscp.mbf \
        ${dce_catdir}/dcecds.cat \
        "

test_configfiles_opt="\
        ${dce_etcdir}/cds_config \
        "

test_testfiles="\
        ${dce_tstdir}/dcecheck \
        ${dce_tstdir}/dcesx \
        ${dce_tstdir}/cp_killer.sh \
        ${dce_tstdir}/cp_test.sh \
        ${dce_tstdir}/cdstest.sh \
        "

dcecheck="${dce_tstdir}/dcecheck"
dcesx="${dce_tstdir}/dcesx"
killer_sh="${dce_tstdir}/cp_killer.sh"
cp_test_sh="${dce_tstdir}/cp_test.sh"
cdstest_sh="${dce_tstdir}/cdstest.sh"

test_testfiles_opt="\
        ${dce_tstdir}/cp_childpointer.tests \
        ${dce_tstdir}/cp_clearinghouse.tests \
        ${dce_tstdir}/cp_clerk.tests \
        ${dce_tstdir}/cp_credir.tests \
        ${dce_tstdir}/cp_directory.tests \
        ${dce_tstdir}/cp_misc.tests \
        ${dce_tstdir}/cp_object.tests \
        ${dce_tstdir}/cp_replica.tests \
        ${dce_tstdir}/cp_server.tests \
        ${dce_tstdir}/cp_softlink.tests \
        ${dce_tstdir}/cp_misc_do.cdscp \
        "

###############################################################################
#
# verify the existance of the executables
#

(echo "";echo "Checking executables ...") | tee -a ${logfile}

for f in ${test_executables}
  do
    if [ ! -f $f ];
      then echo " ... missing $f ... aborting!!!" | tee -a ${logfile}
           exit 1
      else echo " ... found $f" | tee -a ${logfile}
    fi
  done

###############################################################################
#
# verify the existance of the configuration files
#

(echo "";echo "Checking mandatory configuration files ...") | tee -a ${logfile}

for f in ${test_configfiles}
  do
    if [ ! -f $f ];
      then echo " ... missing $f ... aborting!!!" | tee -a ${logfile}
           exit 1
      else echo " ... found $f" | tee -a ${logfile}
    fi
  done

(echo "";echo "Checking optional configurations files ...") | tee -a ${logfile}

for f in ${test_executables}
  do
    if [ ! -f $f ];
      then echo " ... missing $f ... continuing!!!" | tee -a ${logfile}
      else echo " ... found $f" | tee -a ${logfile}
    fi
  done


###############################################################################
#
# verify the existance of our test executables and scripts
#

(echo "";echo "Checking mandatory test files and utilities ...") \
 | tee -a ${logfile}

for f in ${test_testfiles}
  do
    if [ ! -f $f ];
      then echo " ... missing $f ... aborting!!!" | tee -a ${logfile}
           exit 1
      else echo " ... found $f" | tee -a ${logfile}
    fi
  done

(echo "";echo "Checking optional test files and utilities ...") \
 | tee -a ${logfile}

for f in ${test_testfiles_opt}
  do
    if [ ! -f $f ];
      then echo " ... missing $f ... continuing!!!" | tee -a ${logfile}
      else echo " ... found $f" | tee -a ${logfile}
    fi
  done

###############################################################################
#
(echo "";echo "Daemon Startup Verification";echo "") | tee -a ${logfile}
#
###############################################################################

###############################################################################
#
# start any daemons that need starting
#

#
# check rpcd
#

${dcecheck} rpcd
rpcd_status=$?

if [ ${rpcd_status} -ne 0 ];
    then
        echo -n "Starting rpcd ..." | tee -a ${logfile}
        ${dce_bindir}/rpcd;
        echo -n " sleep 10 ..." | tee -a ${logfile}
        sleep 10;
        echo "" | tee -a ${logfile}
        ${dcecheck} rpcd
        rpcd_status=$?
        if [ ${rpcd_status} -ne 0 ]; then
           echo "Can't start rpcd ... aborting!!!" | tee -a ${logfile}
           exit 1
        fi
    else
        echo "rpcd is already running ... continuing." | tee -a ${logfile}
fi

#
# check cdsadv
#

${dcecheck} cdsadv
cdsadv_status=$?

if [ ${cdsadv_status} -ne 0 ];
    then
        echo -n "Starting cdsadv ..." | tee -a ${logfile}
        ${dce_bindir}/cdsadv;
        echo -n " sleep 30" | tee -a ${logfile}
        sleep 30;
        echo "" | tee -a ${logfile}
        ${dcecheck} cdsadv
        cdsadv_status=$?
        if [ ${cdsadv_status} -ne 0 ]; then
           echo "Can't start cdsadv ... aborting!!!" | tee -a ${logfile}
           exit 1
        fi
    else
        echo "cdsadv is already running ... continuing." | tee -a ${logfile}
fi

#
# check cdsd
#

${dcecheck} cdsd
cdsd_status=$?

if [ ${cdsd_status} -ne 0 ];
    then
        echo -n "Starting cdsd ..." | tee -a ${logfile}
        ${dce_bindir}/cdsd -a;
        echo -n " sleep 60" | tee -a ${logfile}
        sleep 60;
        echo "" | tee -a ${logfile}
        ${dcecheck} cdsd
        cdsd_status=$?
        if [ ${cdsd_status} -ne 0 ]; then
           echo "Can't start cdsd ... aborting!!!" | tee -a ${logfile}
           exit 1
        fi
    else
        echo "cdsd is already running ... continuing." | tee -a ${logfile}
fi

#
# check gdad
#

${dcecheck} gdad
gdad_status=$?

if [ ${rpcd_status} -ne 0 ];
    then
        echo -n "Starting gdad ..." | tee -a ${logfile}
        ${dce_bindir}/gdad;
        echo -n " sleep 10" | tee -a ${logfile}
        sleep 10;
        echo "" | tee -a ${logfile}
        ${dcecheck} gdad
        gdad_status=$?
        if [ ${gdad_status} -ne 0 ]; then
           echo "Can't start gdad ... continuing." | tee -a ${logfile}
           exit 1
        fi
    else
        echo "gdad is already running ... continuing." | tee -a ${logfile}
fi

###############################################################################
#
(echo "";echo "Operational/Runtime File Startup Verification";echo "") \
  | tee -a ${logfile}
#
###############################################################################

#
# determine our cellname prefix
#

ourcellname_prefix="`cat ${dce_topdir}/dce_cf.db | grep cellname | cut -f3 -d\/`"
echo "${prefix}our cellname prefix     : ${ourcellname_prefix}" \
    | tee -a ${logfile}

###############################################################################
#
# define some file lists
#

startup_files_parse="\
        ${dce_topdir}/dce_cf.db \
        ${dce_etcdir}/cds_config \
        ${dce_vardir}/cds_files \
        ${dce_admdir}/cds_cache.version \
        ${dce_vardir}/${ourcellname_prefix}#${clearinghouse1}.version \
        "

startup_files_parse_opt="\
        ${dce_vardir}/${ourcellname_prefix}#${clearinghouse2}.version \
        "

#
# verify the existance of the runtime files
#

(echo "";echo "Checking runtime files (part #1) ...") | tee -a ${logfile}

for f in ${startup_files_parse}
  do
    if [ ! -f $f ];
      then echo " ... missing $f ... aborting!!!" | tee -a ${logfile}
           exit 1
      else echo " ... found $f" | tee -a ${logfile}
    fi
  done

(echo "";echo "Checking optional runtime files (part #1) ...") \
 | tee -a ${logfile}

for f in ${startup_files_parse_opt}
  do
    if [ ! -f $f ];
      then echo " ... missing $f ... continuing." | tee -a ${logfile}
      else echo " ... found $f" | tee -a ${logfile}
    fi
  done

###############################################################################
#
# parse those runtime configuration files
#

#
# common parsing
#

cache_version="`cat ${dce_admdir}/cds_cache.version`"
echo "${prefix}cache version           : ${cache_version}" | tee -a ${logfile}

#
# clearinghouse1 parsing
#

clearinghouse1_version="`cat ${dce_vardir}/${ourcellname_prefix}#${clearinghouse1}.version`"
echo "${prefix}clearinghouse 1 version : ${clearinghouse1_version}" \
    | tee -a ${logfile}

checkpoint1_version=${clearinghouse1_version}
echo "${prefix}checkpoint 1 version    : ${checkpoint1_version}" \
    | tee -a ${logfile}

tlog1_version=${clearinghouse1_version}
echo "${prefix}tlog 1 version          : ${tlog1_version}" \
    | tee -a ${logfile}

#
# clearinghouse2 parsing (optional)
#

if [ -f ${dce_vardir}/${ourcellname_prefix}#${clearinghouse2}.version ]; then
    clearinghouse2_version="`cat ${dce_vardir}/${ourcellname_prefix}#${clearinghouse2}.version`"
    echo "${prefix}clearinghouse 2 version : ${clearinghouse2_version}" \
        | tee -a ${logfile}

    checkpoint2_version=${clearinghouse2_version}
    echo "${prefix}checkpoint 2 version    : ${checkpoint2_version}" \
        | tee -a ${logfile}

    tlog2_version=${clearinghouse2_version}
    echo "${prefix}tlog 2 version          : ${tlog2_version}" \
        | tee -a ${logfile}
fi

###############################################################################
#
# define some more file lists
#

startup_files_check="\
        ${dce_admdir}/cds_cache.${cache_version} \
        ${dce_vardir}/${ourcellname_prefix}#${clearinghouse1}.checkpoint${checkpoint1_version} \
        ${dce_vardir}/${ourcellname_prefix}#${clearinghouse1}.tlog${tlog1_version} \
        "

startup_files_check_rpc="\
        ${dce_rpcdir}/rpcdep.dat \
        ${dce_rpcdir}/rpcdllb.dat \
        "

startup_files_check_opt="\
        ${dce_vardir}/${ourcellname_prefix}#${clearinghouse2}.checkpoint${checkpoint2_version} \
        ${dce_vardir}/${ourcellname_prefix}#${clearinghouse2}.tlog${tlog2_version} \
        "

#
# verify the existance of the runtime files
#

(echo "";echo "Checking runtime files (part #2) ...") | tee -a ${logfile}

for f in ${startup_files_check}
  do
    if [ ! -f $f ];
      then echo " ... missing $f ... aborting!!!" | tee -a ${logfile}
           exit 1
      else echo " ... found $f" | tee -a ${logfile}
    fi
  done

(echo "";echo "Checking runtime files (part #2) - rpc ...") | tee -a ${logfile}

for f in ${startup_files_check_rpc}
  do
    if [ ! -f $f ];
      then f_old="/tmp/`basename $f`"
           echo " ... missing $f ... checking /tmp ." | tee -a ${logfile}
           if [ ! -f $f_old ]
             then echo " ... missing $f_old ... aborting !!!" | tee -a ${logfile}
             else echo " ... found $f_old" | tee -a ${logfile}
           fi
      else echo " ... found $f" | tee -a ${logfile}
    fi
  done

(echo "";echo "Checking runtime files (part #2) - optional ...") \
 | tee -a ${logfile}

for f in ${startup_files_check_opt}
  do
    if [ ! -f $f ];
      then echo " ... missing $f ... continuing." | tee -a ${logfile}
      else echo " ... found $f" | tee -a ${logfile}
    fi
  done

###############################################################################
#
(echo "";echo "Library API Testing (cdstest)";echo "") | tee -a ${logfile}
#
###############################################################################

${cdstest_sh} ${pid_switch} | tee -a ${logfile}
status_cdstest=$?
(echo ""; echo -n "*** Library API Testing (cdstest) completed - ") \
 | tee -a ${logfile}
if [ ${status_cdstest} -eq 0 ];
  then echo "no errors" | tee -a ${logfile}
  else echo "errors were detected!!!" | tee -a ${logfile}
fi
echo "" | tee -a ${logfile}

###############################################################################
#
(echo "";echo "Component Tests";echo "") | tee -a ${logfile}
#
###############################################################################

(echo "";echo "Not implemented.";echo "") | tee -a ${logfile}

###############################################################################
#
(echo "";echo "Control Program Tests (killer)";echo "") | tee -a ${logfile}
#
###############################################################################

${killer_sh} | tee -a ${logfile}
status_killer=$?
(echo ""; echo -n "*** Control Program Testing (killer) completed - ") \
 | tee -a ${logfile}
if [ ${status_killer} -eq 0 ];
  then echo "no errors" | tee -a ${logfile}
  else echo "errors were detected!!!" | tee -a ${logfile}
fi
echo "" | tee -a ${logfile}

###############################################################################
#
(echo "";echo "System Exerciser (dcesx)";echo "") | tee -a ${logfile}
#
###############################################################################

${dcesx} -i 10 -p 10 -r 3 -t 10 -K -V -L | tee -a ${logfile}
status_dcesx=$?
(echo ""; echo -n "*** System Exercisor Testing (dcesx) completed - ") \
 | tee -a ${logfile}
if [ ${status_dcesx} -eq 0 ];
  then echo "no errors" | tee -a ${logfile}
  else echo "errors were detected!!!" | tee -a ${logfile}
fi
echo "" | tee -a ${logfile}

###############################################################################
#
(echo "";echo "Security Tests";echo "") | tee -a ${logfile}
#
###############################################################################

(echo "";echo "Not implemented.";echo "") | tee -a ${logfile}

###############################################################################
#
# Test Termination
#
###############################################################################

#
# Capture some more statistics
#

echo ""
echo -n "${prefix}Capturing ending statistics ..."
echo ""                                                 >> ${logfile}
echo "#"                                                >> ${logfile}
echo "# `date` - Ending statistics"                     >> ${logfile}
echo "#"                                                >> ${logfile}
echo ""                                                 >> ${logfile}
df                                                      >> ${logfile}
echo ""                                                 >> ${logfile}
/etc/pstat -s                                           >> ${logfile}
echo ""                                                 >> ${logfile}
ipcs                                                    >> ${logfile}
echo ""                                                 >> ${logfile}
ps -aux | head -1                                       >> ${logfile}
ps -aux | grep 'cds'                                    >> ${logfile}
echo " done."

#
# Give a test result summary
#

echo ""                                                    | tee -a ${logfile}
echo "=============================================="      | tee -a ${logfile}
echo "          Test Results Summary"                      | tee -a ${logfile}
echo "=============================================="      | tee -a ${logfile}
echo "Test Description                     : Result"       | tee -a ${logfile}
echo "----------------------------------------------"      | tee -a ${logfile}

if [ ${status_cdstest} -eq 0 ];
  then echo "CDS API Library tests                :  PASS" | tee -a ${logfile}
  else echo "CDS API Library tests                :  FAIL" | tee -a ${logfile}
fi
if [ ${status_killer} -eq 0 ];
  then echo "Control Program (cdscp) Killer tests :  PASS" | tee -a ${logfile}
  else echo "Control Program (cdscp) Killer tests :  FAIL" | tee -a ${logfile}
fi
if [ ${status_dcesx} -eq 0 ];
  then echo "DCE System Excerciser tests          :  PASS" | tee -a ${logfile}
  else echo "DCE System Excerciser tests          :  FAIL" | tee -a ${logfile}
fi

#
# Resolve the various status variables into one overall test status
#

if [ ${status_cdstest} -eq 0 -a \
     ${status_killer}  -eq 0 -a \
     ${status_dcesx}   -eq 0 ];
  then status_overall=0
  else status_overall=1
fi

echo ""                                 | tee -a ${logfile}
if [ ${status_overall} -eq 0 ];
  then echo "All tests passed!!!"       | tee -a ${logfile}
  else echo "Errors were detected!!!"   | tee -a ${logfile}
fi
echo ""                                 | tee -a ${logfile}

#
# Cleanup
#

cd ${myhome}

exit ${status_overall}
