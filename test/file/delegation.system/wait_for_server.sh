#!/bin/sh

. ./template.sh

platform=`uname`

no_op()
{
	return
}

if test "$platform" = "AIX"
then
	CDSPING=no_op
	echo "$platform: wait 5 min for server to start"
	SLEEPTIME=300
else
	CDSPING=cdsping
fi

#$CDSPING $SERVER > /dev/null 2>&1
$CDSPING $SERVER

while [ $? -ne 0 ]
do
	sleep $SLEEPTIME
#	$CDSPING $SERVER > /dev/null 2>&1
	$CDSPING $SERVER
	if test "$platform" = "AIX"
	then
		false
	fi
done
