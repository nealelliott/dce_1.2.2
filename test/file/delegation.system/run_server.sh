#!/bin/sh

dirname=`dirname $0`

. $dirname/template.sh

dce_login $DCE_USER $DCE_PASSWD -e $SRC/delegated $PRINC $NS_ENTRY

exit 0
