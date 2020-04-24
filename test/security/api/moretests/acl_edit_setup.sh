#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: acl_edit_setup.sh,v $
# Revision 1.1.8.2  1996/03/11  02:34:15  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:17  marty]
#
# Revision 1.1.8.1  1995/12/11  21:48:03  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/04/05  22:58 UTC  burati
# 	Merge pe_site change fix
# 
# 	HP revision /main/KK_FIX/1  1995/02/21  19:41 UTC  burati
# 	Fix binding retrieval to work with KK+ GTE fix for pe_site files
# 	[1995/12/11  20:52:05  root]
# 
# Revision 1.1.6.1  1994/01/31  17:40:22  burati
# 	acl_server no longer needs (or accepts) useless ip option
# 	[1994/01/31  16:27:34  burati]
# 
# Revision 1.1.4.6  1993/03/19  22:36:28  cuti
# 	Add "remove /.:/myvilesystem fromcds DB"
# 	[1993/03/19  22:34:06  cuti]
# 
# Revision 1.1.4.5  1993/02/25  22:05:36  cuti
# 	Add "site -u" to rgy_edit commands to make sure working on master site.
# 	[1993/02/25  18:57:27  cuti]
# 
# Revision 1.1.4.4  1992/12/31  17:28:14  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:53:42  htf]
# 
# Revision 1.1.4.3  1992/11/12  12:39:26  sekhar
# 	[OT 6033]  kill acl_server if already running before setup.
# 	[1992/11/11  19:16:45  sekhar]
# 
# Revision 1.1.4.2  1992/11/05  19:35:15  sekhar
# 	[OT 5937]  Remove principal aclmgr and its key from /tmp/aclmgr_keytab
# 	           which may be left over from a previous run of acl_edit.
# 	[1992/11/05  16:44:03  sekhar]
# 
# Revision 1.1.2.4  1992/06/09  13:16:16  cuti
# 	Add #!/ in the first line which for some reason geto eaten.
# 	[1992/06/08  20:57:30  cuti]
# 
# Revision 1.1.2.3  1992/06/02  21:10:31  cuti
# 	Add acl_server startup script for handling posix acl mask.
# 	[1992/06/02  18:28:13  cuti]
# 
# Revision 1.1.1.2  1992/05/05  16:16:09  cuti
# 	 Use rgy_setup.sh to do flintstone's account checking and creation.
# 
# Revision 1.1  1992/01/19  04:08:40  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# 
#set -x
rgy_setup.sh

read tokena BINDINGS tokenb < /opt/dcelocal/etc/security/pe_site
BINDING=`echo $BINDINGS | sed -e "s/\[\].*//"`

/opt/dcelocal/bin/acl_edit -addr $BINDING principal/flintstone -m user_obj:rcDnfmaug
/opt/dcelocal/bin/acl_edit -addr $BINDING principal/flintstone -l

# remove aclmgr key and principal from registry if it exists
# The key version will always be 1 because a key if it exists
# is deleted before attempting to add it.
#

cat > /tmp/aclmgr.script << !
site -u
ktdel -p aclmgr -v 1 -f /tmp/aclmgr_keytab
do p
del aclmgr
quit
!
/opt/dcelocal/bin/rgy_edit </tmp/aclmgr.script > /tmp/aclmgr.out

# Add acl_server startup.
echo "Add acl_server to rgy"

rgy_edit <<!
site -u
do p
add aclmgr
do a
add aclmgr -g none -o none -pw aclmgr -mp -dce-
kta -p aclmgr -pw aclmgr -f /tmp/aclmgr_keytab
quit
!

# kill the acl_server if already running. The acl_edit.sh also kill acl_server
# at the end of the acl_edit test.  So the killing the acl_server here allows
# acl_edit_setup.sh to be run multiple times without starting up a new instance
# of acl_server each time the setup is done.

ps -ef | grep acl_server | grep -v "grep acl_server" | awk '{ print $2 }' | xargs kill -9

# Remove /.:/myfilesystem from cds

cdscp << EOF
list obj /.:/myfilesystem
del obj /.:/myfilesystem
q
EOF

# Start acl_server  
echo "Start acl_server"

acl_server -e -f /tmp/aclmgr_keytab &


