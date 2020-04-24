# 
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# @OSF_COPYRIGHT@
# 
# HISTORY
# $Log: config.env.sed,v $
# Revision 1.1.4.2  1996/03/11  02:34:06  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:39:31  marty]
#
# Revision 1.1.4.1  1995/12/11  15:23:58  root
# 	Submit
# 	[1995/12/11  14:35:28  root]
# 
# Revision 1.1.2.3  1994/08/17  11:51:33  marrek
# 	Use uname -n .
# 	[1994/08/17  07:37:00  marrek]
# 
# Revision 1.1.2.2  1994/07/22  11:41:06  marrek
# 	Fix typo in version comment.
# 	[1994/07/22  08:17:54  marrek]
# 
# Revision 1.1.2.1  1994/07/20  14:06:30  marrek
# 	Add OSF copyright notice.
# 	[1994/07/20  14:00:09  marrek]
# 
# 	Overtaken from DCE1.1 tree (config.env) and slightly changed
# 	[1994/07/20  13:42:36  marrek]
# 
# $EndLog$
#
# $RCSfile: config.env.sed,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:34:06 $
#
#
# This file contain most of the variable used by dce_config.
# You can pass this on the command line via the -e switch
# or dce_config will look at /etc/dce_config.conf by default.
#
# This is a shell script sourced in to dce_config at startup
# and can contain shell commands if you wish.
#

#
# General config values
#

EXIT_ON_ERROR=n		# y/n Exit if fatal error ecountered, default n
DO_CHECKS=n		# y/n Prompt for continue after warnings, default y

CELL_ADMIN=cell_admin	# Principal name of Cell Administrator
CELL_ADMIN_PW=-dce-	# Password for CELL_ADMIN account

#TOLERANCE_SEC=120	# Number of seconds clients can differ from sec server
check_time=y		# y/n if you want to check times, default y
SYNC_CLOCKS=y		# y/n Sync our clock to sec servers?


#
# Install
#
REMOVE_PREV_INSTALL=y		# y/n Remove previous install before 
				# installing anything.
mach=`uname`
case $mach in
	OSF1)	machine=at386;;
	AIX)	machine=rios;;
	HP-UX)	machine=hp800;;
esac
# path to install area
#FILESYSTEM="install/${machine}/opt/dce1.1"
#MEDIA="<device>"			# device to read tape from

CP_OR_SYMLINK=copy		# Use copy or sym-links(link) to install
USE_DEF_MSG_PATH=y		# y/n use default message catalog path
USE_ETHER_FILE=y		# y/n Trust /etc/ieee_802_addr file (OSF/1)
DO_LIB_ADMIN=y			# y/n Install libdce.so on OSF/1 system

#
# DFS install
#
INSTALL_OPT_SERS=y		# y/n install optional servers (bak, butc, etc)
INSTALL_OPT_CLIENT=y		# y/n install optional client binaries (cm, etc)


#
# General config variables
#
REMOVE_PREV_CONFIG=y	# y/n Remove previous config before config-ing anything

CELL_NAME="CELLNAME"	# Name of cell to configure

#HOST_NAME_IP="<ip_address>"	# IP address of current host (if getip fails)


#
# DCE Client config
#
DTS_CONFIG=clerk		# When doing DCE client config, what should
				# we be? clerk, local, global, none

#
# Security Config
#
SEC_SERVER="`uname -n`"		# Hostname of security server
#SEC_SERVER_IP="<ip-address>"	# fallback if getip program doesn't work
KEYSEED="garBageMan"		# Keeyseed for initial database master

#SEC_REPLICA="<hostname>"	# Name of the security replica database


#
# CDS Config
#
CACHE_CDS_SERVER="`uname -n`"	# Name of a cds server to cache
#CACHE_CDS_SERVER_IP="<ip_address>" # fallback if getip program doesn't work
MULTIPLE_LAN=n			# y/n do you have multiple lans
#LAN_NAME="<name>"		# Name of lan if MULTIPLE_LAN=y

#REP_CLEARINGHOUSE="<name_ch>"	# Name for new replica clearing house
#DIR_REPLICATE="n"		# y/n manually type in more directories 
				# to replicate.

#
# GDA
#

#
# DTS Config
#
NTP_HOST="<hostname>"		# Name of ntp server

#
# DFS Config
#

AGG_FS_TYPE=native		# native/episode aggregate fs type to export
AGG_DEV_NAME="<device>"		# device name for the aggregate to be exported
AGG_MOUNT_PATH="<path>"		# mount path for aggregate
AGG_NAME="<name>"			# Name of aggregate
AGG_ID="<number>"			# numerical id of aggregate

CACHE_SIZE_RAM=10000		# number of bytes for memory cache
CACHE_SIZE_DISK=10000		# number of bytes for disk cache
CACHE_DIR_DISK="/opt/dcelocal/var/adm/dfs/cache" # pathname of disk cache
CLIENT_CACHE_LOC=disk		# mem/disk where is the cache

EPI_FORMAT_PART=n		# y/n format partition as episode
EPI_FORCE_INIT=n		# y/n force initialization, even if data loss
INIT_LFS=n			# y/n initialize the LFS (using epiinit)?
LOAD_LFS_KEXT=n			# y/n load LFS kernel extentions

ROOT_FILESET_NM="<name>"	# Root fileset name
SCM_NAME="<hostname>"		# System Control Machine name

