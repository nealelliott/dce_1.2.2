#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: gdscrontab.sh,v $
# Revision 1.1.11.2  1996/03/09  23:12:57  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:08  marty]
#
# Revision 1.1.11.1  1995/12/08  15:25:32  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:54:01  root]
# 
# Revision 1.1.9.1  1994/02/22  18:05:14  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:48:44  marrek]
# 
# Revision 1.1.7.2  1993/08/11  14:17:38  marrek
# 	July 1993 code drop.
# 	[1993/08/02  13:10:21  marrek]
# 
# Revision 1.1.5.3  1992/12/31  18:39:09  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:11:43  bbelch]
# 
# Revision 1.1.5.2  1992/09/21  14:45:02  marrek
# 	Corrections for bug 5206.
# 	[1992/09/16  15:13:22  marrek]
# 
# Revision 1.1.2.2  1992/06/01  20:10:07  melman
# 	New GDS merged drop
# 	[1992/05/28  21:04:22  melman]
# 
# $EndLog$
#
# $RCSfile: gdscrontab.sh,v $ $Revision: 1.1.11.2 $ $Date: 1996/03/09 23:12:57 $
#
#***************************************************************************
#
#        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991
#                             ALL RIGHTS RESERVED
#
#***************************************************************************

# shell script for updating the 'cron'-table
# input parameter: $1 = name of the directory system update handler program
#                  $2 = directory identifier
#                  $3 = <dce_local>
#                  $4 = time-stamp for next activation (min)
#                  $5 = time-stamp for next activation (hour)
#                  $6 = time-stamp (day)
#                  $7 = time-stamp (month)
#                  $8 = time-stamp (day of the week)

CL_PATH=$3/var/adm/directory/gds

#remove all (GDS specific) crontab entries 
crontab -l | sort > $CL_PATH/.crontab1.$$
cat $CL_PATH/.crontab | sort > $CL_PATH/.crontab2.$$
comm -23 $CL_PATH/.crontab1.$$ $CL_PATH/.crontab2.$$ > $CL_PATH/.crontab3.$$

# remove specific job entry from (GDS specific) crontab
grep -v "$1 $2" $CL_PATH/.crontab > $CL_PATH/.crontab1.$$
mv $CL_PATH/.crontab1.$$ $CL_PATH/.crontab

if [ $# -gt 3 ]
then
    # append new job entry at the end of the (GDS specific) crontab
    echo " $4 $5 $6 $7 $8 $1 $2 $3" >> $CL_PATH/.crontab
fi

# add (GDS specific) crontab entries to the global crontab
if [ -s $CL_PATH/.crontab ]
then
	cat $CL_PATH/.crontab >> $CL_PATH/.crontab3.$$
else
	rm $CL_PATH/.crontab
fi

if [ -s $CL_PATH/.crontab3.$$ ]
then
    crontab $CL_PATH/.crontab3.$$
else
    crontab -r
fi
rm $CL_PATH/.crontab[1-3].$$
