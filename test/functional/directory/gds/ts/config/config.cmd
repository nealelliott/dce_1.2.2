# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: config.cmd,v $
# Revision 1.1.4.2  1996/03/11  02:34:04  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:39:30  marty]
#
# Revision 1.1.4.1  1995/12/11  15:23:56  root
# 	Submit
# 	[1995/12/11  14:35:27  root]
# 
# Revision 1.1.2.2  1994/07/22  11:41:04  marrek
# 	Fix typo in version comment.
# 	[1994/07/22  08:17:52  marrek]
# 
# Revision 1.1.2.1  1994/07/20  14:06:28  marrek
# 	Add OSF copyright notice.
# 	[1994/07/20  14:00:07  marrek]
# 
# 	Overtaken from DCE1.1 tree and slightly changed
# 	[1994/07/20  13:41:51  marrek]
# 
# $EndLog$
#
# $RCSfile: config.cmd,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:34:04 $
#
#
#
# This file is an example of what you can pass to dce_config
# via the -c switch.  It it sourced in to dce_config, and
# can contain shell script commands if you wish.

#
# install commands
#
#-----------------------------------------------------
#
#  install <what>
#  <what> :=            sec     gds     appdev          sec-replica
#                       cds     dts     cdsbrowser
#                       client  nidl_to_idl
#
#  install dfs  <which>
#               <which> := client|scm|privatefs|fs|fldb
#

#install sec			# Security Server
#install cds			# CDS Server
#install dts			# DTS Server
#install gds			# GDS Server (no config yet)
#install client			# DCE Client
#install appdev			# Application Development Environment
#install sec-replica		# Replica Security Server
#install cdsbrowser		# Install cdsbrowser
#install nidl_to_idl		# Install nidl_to_idl

#
# DFS install
#
#install dfs client
#
# You do not need to install dfs client if you install one of the servers.
#
#install dfs scm
#install dfs privatefs
#install dfs fs
#install dfs fldb


#
# config commands
#
#-----------------------------------------------------
#  config  <component> <how>
#
#  <component> :=
#              client
#              sec     <how> :=        client|server|replica
#              cds     <how> :=        client|server|replica
#              gda
#
#              dts     <how> :=        clerk|local|global|server|none
#                                      ntp-provider|null-provider
#
#              dfs     <how> :=        client|scm|privatefs|fs|fldb
#
#config client			# Same as:
				#  config sec client
				#  config cds client
				#  config dts $DTS_CONFIG
#
# Security
#
# Can only pick one, server implies client.
#config sec client		# Security Client
config sec server		# Security Server
#config sec replica		# Security Replica


#
# CDS
#
# Can only pick one, server implies client.
#config cds client		# CDS Client
config cds server		# CDS Server
#config cds replica		# Addtional CDS server on this machine

#
# GDA
#
#config gda			# Run a gdad on this machine

#
# Audit subsystem
#
#config audit			# Fire up auditd

#
# DTS
#
# Can only pick one, server implies client.
#config dts clerk		# DTS Clerk
#config dts local		# DTS Local Server
#config dts global		# DTS Global Server

#
# DTS Time providers
#
# Can only pick one
#config dts ntp-provider	# Run NTP provider on this node
#config dts null-provider	# Run Null provider on this node

#
# DFS
#
# You may pick one of these three server types
#config dfs fldb		# file Location Database server
#config dfs fs			# File Server
#config dfs privatefs		# Private File Server

# Any of the above can be a SCM.
#config dfs scm			# System Control Machine

#
# Client must be configured after server
#config dfs client		# DFS Client
