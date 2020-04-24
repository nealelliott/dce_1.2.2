#! /bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: local_rgy_setup.sh,v $
# Revision 1.1.7.2  1996/03/11  02:34:38  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:33  marty]
#
# Revision 1.1.7.1  1995/12/11  21:48:44  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:24  root]
# 
# Revision 1.1.5.4  1993/02/25  22:06:19  cuti
# 	Add "site -u" to rgy_edit commands to make sure working on master site.
# 	[1993/02/25  19:02:02  cuti]
# 
# Revision 1.1.5.3  1993/01/22  21:57:01  sekhar
# 	[OT 4887]  Delete /tmp/rgy_edit.script before usage.
# 	[1993/01/22  21:43:28  sekhar]
# 
# Revision 1.1.5.2  1992/12/31  17:29:23  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:54:56  htf]
# 
# Revision 1.1.2.4  1992/06/10  14:43:52  cuti
# 	Add -f option to rm command.
# 	[1992/06/09  13:22:20  cuti]
# 
# Revision 1.1.2.3  1992/05/28  20:21:00  cuti
# 	Add "a tidas" and trimmed down one entry from size 130 alphabets to 26 chars.
# 	[1992/05/28  18:32:39  cuti]
# 
# Revision 1.1.2.2  1992/05/19  20:49:17  cuti
# 	 Created to create some rgy accounts for testing local registry.
# 	[1992/05/19  18:46:22  cuti]
# 
# $EndLog$

#set -x 

rgy_setup_init()
{
echo "site -u" >> /tmp/rgy_edit.script
echo "do p" >> /tmp/rgy_edit.script
echo "add $1 -f \"$2\"" >> /tmp/rgy_edit.script
echo "do acc" >>/tmp/rgy_edit.script
echo "add $1 -g none -o none -mp -dce- -pw $3 " >>/tmp/rgy_edit.script
}


/bin/rm -f /opt/dcelocal/var/security/lrgy_data
/bin/rm -f /opt/dcelocal/var/security/lrgy_tgts

/bin/rm -f /tmp/rgy_edit.script
touch /tmp/rgy_edit.script

rgy_setup_init flintstone "fred flintstone" yabadabado
rgy_setup_init a "1 char" tidas
rgy_setup_init abcdefghijklmnopqrstuvwxyz "26 chars" abcdefghijklmnopqrstuvwxyz
rgy_setup_init mishkin "rpc" -dce4-
rgy_setup_init pato "sec" -dce5-
rgy_setup_init dineen "idl" -dce6-
rgy_setup_init wyant "cps" -dce7-
rgy_setup_init pcl "rpc" -dce8-
rgy_setup_init emartin "replication" -dce9-
rgy_setup_init nacey "rtw" -dce10-
rgy_setup_init dale "rpc" -dce11-
rgy_setup_init markar "thread" -dce12-
rgy_setup_init greg "sec" -dce13-
rgy_setup_init burati "sec" -dce14-
rgy_setup_init ahop "sec" -dce15-
rgy_setup_init sommerfeld "auth" -dce16-
rgy_setup_init tbl "idl" -dce17-
rgy_setup_init frisco "sec" -dce18-
rgy_setup_init mk "rpc" -dce19-
rgy_setup_init cuti "rpc+sec" -dce20-
rgy_setup_init roc "mgr" -dce21-
rgy_setup_init kumar "mgr" -dce22-
rgy_setup_init kathy "market" -dce23-
rgy_setup_init kumar_k "tech" -dce24-
rgy_setup_init gream "mgr" -dce25-
rgy_setup_init marcus_s "market" -dce26-
echo "q" >>/tmp/rgy_edit.script


/opt/dcelocal/bin/rgy_edit </tmp/rgy_edit.script
/bin/rm /tmp/rgy_edit.script


EXIT=$?

exit $EXIT

