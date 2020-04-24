#!/bin/sh
#
# @SOURCEWARNING@
#
# @OSF_COPYRIGHT@
# Copyright (c) 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: dcecf_postproc.sh,v $
# Revision 1.1.4.2  1996/03/09  23:07:09  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:25:54  marty]
#
# Revision 1.1.4.1  1995/12/08  20:53:42  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  20:49:06  root]
# 
# Revision 1.1.2.2  1994/09/15  15:18:41  bowe
# 	Combine multiple cell_aliases lines into one. [CR 12195]
# 	[1994/09/15  15:18:29  bowe]
# 
# Revision 1.1.2.1  1994/08/26  20:14:14  bowe
# 	Initial implementation. [CR 11705]
# 	[1994/08/26  20:14:09  bowe]
# 
# $EndLog$

# Set cellname, hostname, or cellaliases in the dce_cf.db file.
# Requirement: these files must exist in directory from which where
# you run this program: cell_name, host_name, cell_aliases.
# (DCED will run this from its directory.)

dcelocal_path="/opt/dcelocal"
if [ -n "$DCELOCAL" ] ; then
    dcelocal_path="$DCELOCAL"
fi
dbfile="$dcelocal_path/dce_cf.db"

usage="Usage: `basename $0` {cell_name | host_name | cell_aliases}"

if [ $# -ne 1 ] ; then
    echo "$usage" 1>&2
    exit 1
fi
file="$1"

# arg must be one of these hd entries
case $file in
    cell_name)		name="cellname"		;;
    host_name)		name="hostname"		;;
    cell_aliases)	name="cellaliases"	;;
    *)			echo "$usage" 1>&2 ; exit 1 ;;
esac

if [ ! -f $file ] ; then
    echo "New name file '$file' does not exist." 1>&2
    exit 1
fi

if [ ! -f $dbfile ] ; then
    echo "DCE host config file '$dbfile' does not exist." 1>&2
    exit 1
fi

new=$dbfile.NEW

# save old config file
cp $dbfile $dbfile.OLD

# copy lines that are not our "target" to the new file
grep -v "^$name[ 	]" $dbfile > $new
# see if the hd file has anything useful in it
nwords=`sed -e '/^#/d' -e '/^$/d' $file | wc -w`
if [ $nwords -eq 0 ] ; then
    # this only happens if the input file is not valid (for safety)
    grep "^$name[ 	]" $dbfile >> $new
else
    # copy non-comments to new file
    case $name in
	cellaliases)
	    echo $name `sed -e '/^#/d' -e '/^$/d' $file` >> $new
	    ;;
	*)
	    sed -e '/^#/d' -e '/^$/d' -e "s/^/$name	/" -e 1q $file >> $new
	    ;;
    esac
fi

# should do some checking before clobbering old file
# XXX

mv $new $dbfile

exit 0

