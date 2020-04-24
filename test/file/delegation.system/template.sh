#!/bin/sh -x
#
#  /bin/sh script meant to be "."ed by invoked test scripts
#
# Copyright (C) 1993 Transarc Corporation --- All rights reserved.
#
# Functions:
#	parse arguments.  Any argument of the form xxx=yyy will be
#		executed as a shell variable assignment.
#	if DCEUSER is in the enviroment, we dce_login as $DCEUSER $DCEPASSWD
#	if CONFFILE is in the environment, we "." it.
#	if WORKINGDIR is in the environment, we cd there, otherwise we
#		cd to TESTDIR.
#	if DASHX is set to "y", we "set -x"


# look for arguments containing '=' and eval them.
# set the -a flag so they are automatically exported.
set -a
for i in $*; do
    eval `echo "$i" | grep '=' | sed -e 's/__/ /g' \
	 | sed -e 's/\([^=]*\)=\(.*\)/\1="\2"/'`
done
set +a

# do a dce login if a DCEUSER is supplied.
if [ "$DCEUSER" != "" ]; then
    if [ "$KRB5CCNAME" = "" ]; then
	echo "Warning, no current DCE context, can't run kinit"
    else
	echo $DCEPASSWD | kinit $DCEUSER
    fi
fi

# read the optional configuration file
if [ "$CONFFILE" != "" ]; then
    if [ -f $CONFFILE ]; then
	. $CONFFILE
    elif [ -f ${TESTDIR}/$CONFFILE ]; then
	. ${TESTDIR}/$CONFFILE
    else
	echo "Can't find configuration file $CONFFILE" >&2
	exit 1
    fi
fi

# cd to the appropriate directory
if [ "$WORKINGDIR" != "" ]; then
    if [ ! -d $WORKINGDIR ]; then
	mkdir $WORKINGDIR
    fi
    cd $WORKINGDIR
elif [ "$TESTDIR" != "" ]; then
    cd $TESTDIR
fi

# define subroutines
ee() {
    if [ "$VERBOSE" = "y" ]; then
	echo + $*
    fi
    "$@"
}

ckerr() {
    if [ $? != 0 ]; then
	echo "*error $*";
	exit 1;
    fi;
}

raise_descriptors() {
	if [ "`uname -s`" = "SunOS" ]
	then
		if [ $2 -ne 0 ]
		then
			RD_NUMBER_OF_FILES=`expr $1 / $2 + 10`
			if [ $RD_NUMBER_OF_FILES -gt 1014 ]
			then
				echo "Not enough file descriptors"
				exit 1
			fi

			if [ `ulimit -n` -lt $RD_NUMBER_OF_FILES ]
			then
				ulimit -n $RD_NUMBER_OF_FILES
				echo "Number of file descriptors"
				echo "raised to $RD_NUMBER_OF_FILES"
			fi

			unset RD_NUMBER_OF_FILES
		fi
	fi
}

# finally, set -x if needed.
if [ "$DASHX" = "y" ]; then
    set -x
fi

