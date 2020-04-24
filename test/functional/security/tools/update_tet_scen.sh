#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: update_tet_scen.sh,v $
# Revision 1.1.6.1  1996/08/09  12:15:27  arvind
# 	Add INT_ONLY scenario
# 	[1996/08/01  19:38 UTC  bartf  /main/DCE_1.2.2/bartf_pk_1.2.2/1]
#
# 	add INT_ONLY
# 	[1996/07/31  19:56 UTC  bartf  /main/DCE_1.2.2/1]
#
# 	create HP mainline branch, Davis, merge international scen
# 	[1995/11/07  16:21 UTC  bartf  /main/bartf_sec_kinit.10/3]
#
# 	fix comments
# 	[1995/11/07  15:35 UTC  bartf  /main/bartf_sec_kinit.10/2]
#
# 	update for international scenario
# 	[1995/11/06  15:11 UTC  bartf  /main/bartf_sec_kinit.10/1]
#
# 	update for international scenario
# 	[1994/10/06  20:31:42  agd  1.1.2.2]
#
# Revision 1.1.4.2  1996/03/11  02:06:53  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:30  marty]
# 
# Revision 1.1.4.1  1995/12/11  19:52:16  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:26:43  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  18:18:09  root]
# 
# Revision 1.1.2.2  1994/10/06  20:31:42  agd
# 	expand copyright
# 	[1994/10/06  14:28:40  agd]
# 
# Revision 1.1.2.1  1994/08/04  16:15:16  mdf
# 	Initial checkin
# 	[1994/08/04  14:11:28  mdf]
# 
# $EndLog$
#

# Note:
# To get the 'international' scenario components, the Makefile for the
# particular test that doesn't belong in this scenario needs to specify
# "DOM_ONLY" as the first argument to the update_tet_scen.sh script.
# Otherwise leave it empty.

case `/bin/uname` in
       OSF1) GREP="/usr/bin/grep -q";;
     ULTRIX) GREP="/bin/grep -s";;
	  *) GREP="/bin/grep -q";;
esac

Update_Token()
{
	FILE=$1
	if [ -f ${FILE} ]
	then 
		chmod +w ${FILE}
		TOKEN="%$2%"
		eval EXPANDED_TOKEN=\$$2
		EXPANDED_TOKEN="${EXPANDED_TOKEN}"
		${GREP} ${TOKEN} ${FILE} && {
			ed - ${FILE} <<- _END_OF_EDIT_
				/${TOKEN}
				i
				${EXPANDED_TOKEN}
				.
				w
				q
			_END_OF_EDIT_
		    }
		chmod -w ${FILE}
	fi
}

MODE=$1
if [ $MODE = DOM_ONLY -o $MODE = INT_ONLY ]
then
	shift 1
fi

FILE=$1
if [ ! -f ${FILE} ]
then
	cp ${FILE}.dat ${FILE}
fi

COMPONENT_DIR=$2
shift 2
for TARGET in $*
do
    ${GREP} ${TARGET} ${FILE} || {
	TEST_SUITE_COMPONENT="	${COMPONENT_DIR}/${TARGET}"
	INTERNATIONAL_TEST_SUITE_COMPONENT="	${COMPONENT_DIR}/${TARGET}"
	INDIVIDUAL_COMPONENT="
${TARGET}
	\"Starting ${TARGET} Test Suite\"
${TEST_SUITE_COMPONENT}
"
	if [ $MODE != INT_ONLY ]
	then
	  Update_Token ${FILE} TEST_SUITE_COMPONENT >/dev/null 2>&1
	fi
	if [ $MODE != DOM_ONLY ]
	then
	  Update_Token ${FILE} INTERNATIONAL_TEST_SUITE_COMPONENT >/dev/null 2>&1
	fi
	Update_Token ${FILE} INDIVIDUAL_COMPONENT >/dev/null 2>&1
    }
done
