#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY  
# $Log: local_login.sh,v $
# Revision 1.1.6.2  1996/03/11  02:34:34  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:30  marty]
#
# Revision 1.1.6.1  1995/12/11  21:48:38  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:21  root]
# 
# Revision 1.1.4.2  1992/12/31  17:29:12  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:54:46  htf]
# 
# Revision 1.1.2.3  1992/05/28  20:17:43  cuti
# 	Trimmed down one entry from size 130 chars to 26 chars.
# 	[1992/05/28  18:32:06  cuti]
# 
# Revision 1.1.2.2  1992/05/19  21:24:07  cuti
# 	 Add ./ to local executible files
# 	[1992/05/19  20:25:14  cuti]
# 
# 	 Created to test local registry.
# 	[1992/05/19  18:47:04  cuti]
# 
# $EndLog$

#set -x

choice=1

if [ $1 ]
then
	choice=$1
fi

   
case "$choice" in 

	1)

        echo "binlogin flintstone yabadabado"
        ../moretests/binlogin flintstone yabadabado
        echo "rgy_setup_init.sh jeffl -dce27-"
        rgy_setup_init.sh jeffl -dce27-
        echo "rgy_setup_init.sh ort -dce28-"
        rgy_setup_init.sh ort -dce28-
        echo "rgy_setup_init.sh fogal -dce29-"
        rgy_setup_init.sh fogal -dce29-
        echo "rgy_setup_init.sh waldo  -dce30-"
        rgy_setup_init.sh waldo  -dce30-
        echo "binlogin a tidas"
        ../moretests/binlogin a tidas
        echo "binlogin abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz"  
        ../moretests/binlogin abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz
        echo "binlogin mishkin  -dce4-"                                            
        ../moretests/binlogin mishkin  -dce4-
        echo "binlogin pato  -dce5-"
        ../moretests/binlogin pato  -dce5-
        echo "binlogin dineen  -dce6-"
        ../moretests/binlogin dineen  -dce6-
        echo "binlogin wyant  -dce7-"
        ../moretests/binlogin wyant  -dce7-
        echo "binlogin pcl  -dce8-"
        ../moretests/binlogin pcl  -dce8-
        echo "binlogin emartin  -dce9-"
        ../moretests/binlogin emartin  -dce9-
        echo "binlogin nacey  -dce10-"
        ../moretests/binlogin nacey  -dce10-
        echo "binlogin dale  -dce11-"
        ../moretests/binlogin dale  -dce11-
        echo "binlogin markar -dce12-"
        ../moretests/binlogin markar -dce12-
        echo "binlogin greg  -dce13-"
        ../moretests/binlogin greg  -dce13-
        echo "binlogin burati -dce14-"
        ../moretests/binlogin burati -dce14-
        echo "binlogin ahop  -dce15-"
        ../moretests/binlogin ahop  -dce15-
        echo "binlogin sommerfeld  -dce16-"
        ../moretests/binlogin sommerfeld  -dce16-
        echo "binlogin tbl  -dce17-"
        ../moretests/binlogin tbl  -dce17-
        echo "binlogin frisco  -dce18-"
        ../moretests/binlogin frisco  -dce18-
        echo "binlogin mk  -dce19-"
        ../moretests/binlogin mk  -dce19-
        echo "binlogin cuti  -dce20-"
        ../moretests/binlogin cuti  -dce20-
        echo "binlogin roc  -dce21-"
        ../moretests/binlogin roc  -dce21-
        echo "binlogin kumar  -dce22-"
        ../moretests/binlogin kumar  -dce22-
        echo "binlogin kathy  -dce23-"
        ../moretests/binlogin kathy  -dce23-
        echo "binlogin kumar_k  -dce24-"
        ../moretests/binlogin kumar_k  -dce24-
        echo "binlogin gream  -dce25-"
        ../moretests/binlogin gream  -dce25-
        echo "binlogin marcus_s  -dce26-"
        ../moretests/binlogin marcus_s  -dce26-  
	;;

	2)

        echo "binlogin dineen  -dce6-"
        ../moretests/binlogin dineen  -dce6-
        echo "binlogin wyant  -dce7-"
        ../moretests/binlogin wyant  -dce7-
        echo "binlogin pcl  -dce8-"
        ../moretests/binlogin pcl  -dce8-
        echo "binlogin emartin  -dce9-"
        ../moretests/binlogin emartin  -dce9-
        echo "binlogin nacey  -dce10-"
        ../moretests/binlogin nacey  -dce10-
        echo "binlogin dale  -dce11-"
        ../moretests/binlogin dale  -dce11-
        echo "binlogin markar -dce12-"
        ../moretests/binlogin markar -dce12-
        echo "binlogin greg  -dce13-"
        ../moretests/binlogin greg  -dce13-
        echo "binlogin burati -dce14-"
        ../moretests/binlogin burati -dce14-
        echo "binlogin ahop  -dce15-"
        ../moretests/binlogin ahop  -dce15-
        echo "binlogin sommerfeld  -dce16-"
        ../moretests/binlogin sommerfeld  -dce16-
        echo "binlogin tbl  -dce17-"
        ../moretests/binlogin tbl  -dce17-
        echo "binlogin frisco  -dce18-"
        ../moretests/binlogin frisco  -dce18-
        echo "binlogin mk  -dce19-"
        ../moretests/binlogin mk  -dce19-
        echo "binlogin cuti  -dce20-"
        ../moretests/binlogin cuti  -dce20-
	;;

	3)

        echo "binlogin flintstone yabadabado"
        ../moretests/binlogin flintstone yabadabado
        echo "binlogin a tidas"
        ../moretests/binlogin a tidas
        echo "binlogin \
        abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz"  
        ../moretests/binlogin abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz
        echo "binlogin mishkin  -dce4-"
        ../moretests/binlogin mishkin  -dce4-
        echo "binlogin pato  -dce5-"
        ../moretests/binlogin pato  -dce5-
        echo "binlogin dineen  -dce6-"
        ../moretests/binlogin dineen  -dce6-
        echo "binlogin wyant  -dce7-"
        ../moretests/binlogin wyant  -dce7-
        echo "binlogin pcl  -dce8-"
        ../moretests/binlogin pcl  -dce8-
        echo "binlogin emartin  -dce9-"
        ../moretests/binlogin emartin  -dce9-
        echo "binlogin nacey  -dce10-"
        ../moretests/binlogin nacey  -dce10-

	;;
esac



