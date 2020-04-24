#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: config.cmd,v $
# Revision 1.1.6.2  1996/03/09  23:09:34  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:27:38  marty]
#
# Revision 1.1.6.1  1995/12/07  21:41:49  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  20:57:44  root]
# 
# Revision 1.1.4.2  1994/08/19  11:31:42  keutel
# 	OT 10954 - gds client installation
# 	[1994/08/19  10:29:30  keutel]
# 
# Revision 1.1.4.1  1994/03/15  23:14:45  zeliff
# 	OT9625: added config audit
# 	[1994/03/09  21:06:23  zeliff]
# 
# Revision 1.1.2.2  1993/10/14  21:01:07  tom
# 	Bug 8904 - Improve comments.
# 	[1993/10/14  21:01:01  tom]
# 
# Revision 1.1.2.1  1993/09/17  21:27:02  tom
# 	Initial checkin.
# 	[1993/09/17  21:25:43  tom]
# 
# $EndLog$


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
#  <what> :=            sec     appdev          sec-replica
#                       cds     dts     cdsbrowser
#                       client  nidl_to_idl
#
#  install gds  <which>
#               <which> := client|server
#
#  install dfs  <which>
#               <which> := client|scm|privatefs|fs|fldb
#

#install sec			# Security Server
#install cds			# CDS Server
#install dts			# DTS Server
#install client			# DCE Client
#install appdev			# Application Development Environment
#install sec-replica		# Replica Security Server
#install cdsbrowser		# Install cdsbrowser
#install nidl_to_idl		# Install nidl_to_idl

#
# GDS install
#
#install gds client
#
# You do not need to install gds client if you install the gds server.
#
#install gds server

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
#config sec server		# Security Server
#config sec replica		# Security Replica


#
# CDS
#
# Can only pick one, server implies client.
#config cds client		# CDS Client
#config cds server		# CDS Server
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
