#!/bin/sh

VERBOSE=${VERBOSE:-0}
if [ $VERBOSE -ne 0 ]
then
	set -x
fi

# test name
FSETTESTNAME=$0

# Configurable parameters 
AGGR1=${AGGR1:-/dev/epi1}
AGGR2=${AGGR1:-/dev/epi2}
MNTDIR=${MNTDIR:-/tmp/fset}
BLKSIZE=${BLKSIZE:-8192}
FRAGSIZE=${FRAGSIZE:-1024}

CTHONDIR=${CTHONDIR:-/afs/tr/usr/rajesh/dfs/test/cthon/rios/bin}
FSETLOGDIR=${FSETLOGDIR:-/afs/tr/usr/rajesh/epi/test/recovery/fset/log}

# Aggregate 1 filesets
BASE_FSETID_AGGR1="100"
BASE_FSETNAME_AGGR1="fset_1_"

# Aggregate 2 filesets
BASE_FSETID_AGGR2="200"
BASE_FSETNAME_AGGR2="fset_2_"

# Fileset clone ids
OFFSET_FACTOR_CLONE_FSET_ID="1000"

# Command names
NEWAGGR=${NEWAGGR:-newaggr}
EFTS=${EFTS:-efts}
EPIMOUNT=${EPIMOUNT:-epimount}
EPIUNMOUNT=${EPIUNMOUNT:-epiunmount}
SALVAGE=${SALVAGE:-salvage}

#
FATAL=1;
NONFATAL=0;
FAILCNT=0;
 
export VERBOSE FSETTESTNAME AGGR1 AGGR2 MNTDIR BLKSIZE FRAGSIZE CTHONDIR \
 BASE_FSETID_AGGR1 BASE_FSETNAME_AGGR1 BASE_FSETID_AGGR2 BASE_FSETNAME_AGGR2 \
 OFFSET_FACTOR_CLONE_FSET_ID NEWAGGR EFTS EPIMOUNT EPIUNMOUNT SALVAGE FATAL \
 NONFATAL FAILCNT FSETLOGDIR

. "${FSETTESTDIR}/fsettestfunc"
##############################################################################

# Emacs settings for this file follow.
# Local Variables:
# tab-width:4
# End:
