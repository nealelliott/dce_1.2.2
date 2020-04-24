#! /bin/sh
# Execute a script as a different DCE user.  Strips off the
# SUBCMD, DCEUSER and DCEPASSWD variables.  Passes all others through.
#	SUBCMD		name of command to run
#	DCEUSER		user to authenticate as
#	DCEPASSWD	passwd of user
#
# Copyright (C) 1994 Transarc Corporation - All rights reserved.

ARGLIST=""
for i do
    case $i in
	DCEUSER=* | DCEPASSWD=* | SUBCMD=*)
	    eval `echo "$i" | grep '=' \
		 | sed -e 's/__/ /g' \
		 | sed -e 's/\([^=]*\)=\(.*\)/\1="\2"/'`
	    ;;
	*)
	    ARGLIST="$i $ARGLIST"
	    ;;
    esac
done
	

if [ "$SUBCMD" = "" ]; then
    echo "*error must supply a SUBCMD= argument"
    exit 1
fi
if [ "$DCEUSER" = "" -o "$DCEPASSWD" = "" ]; then
    echo "*error must supply DCEUSER= and DCEPASSWD= arguments."
    exit 1
fi

echo "runas.sh: dce_login $DCEUSER $DCEPASSWD -e $SUBCMD $ARGLIST"
dce_login $DCEUSER $DCEPASSWD -e $SUBCMD $ARGLIST
