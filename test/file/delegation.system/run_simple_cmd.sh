#! /bin/sh
# Pass the following variables:
#	CMD		name of command to run
#	ARG1 thru ARG9	command line arguments for CMD
#
# Copyright (C) 1993 Transarc Corporation - All rights reserved.

. ./template.sh

if [ "$CMD" = "" ]; then
    echo "*error must supply a CMD= argument"
    exit 1
fi

ee $CMD $ARG1 $ARG2 $ARG3 $ARG4 $ARG5 $ARG6 $ARG7 $ARG8 $ARG9
ckerr $CMD returned bad status.

exit 0
