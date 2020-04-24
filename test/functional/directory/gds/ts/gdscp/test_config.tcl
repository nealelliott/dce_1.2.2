# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: test_config.tcl,v $
# Revision 1.1.4.2  1996/03/11  01:34:20  marty
# 	Update copyright years
# 	[1996/03/10  19:36:25  marty]
#
# Revision 1.1.4.1  1995/12/11  15:28:12  root
# 	Submit
# 	[1995/12/11  14:37:03  root]
# 
# Revision 1.1.2.3  1994/07/06  15:11:57  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:24:35  marrek]
# 
# Revision 1.1.2.2  1994/06/10  20:23:53  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:29  devsrc]
# 
# Revision 1.1.2.1  1994/05/10  14:35:49  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:16:54  marrek]
# 
# $EndLog$
# 
# $RCSfile: test_config.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 01:34:20 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : test_config.tcl                               	  #
# Description : This file contains the configuration details of   #
#	        the X.500 directory like the directory-id, the    #
#		dsa name, the PSAP address, etc.		  #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

# Directory configuration for normal tests
set directory_id 	"5"
set dsa_name 		"/C=de/O=dbp/OU=dap11/CN=dsa/CN=dsa-m1"
set psap_address 	"TS=Server,NA='TCP/IP!internet=192.35.18.18+port=21011'"


# Additional Directory configuration for referral handling. 
set directory_id_add 	"2"
set dsa_name_add 	"/C=de/O=dbp/OU=dap11/CN=dsa/CN=dsa-m2"
set psap_address_add 	"TS=Server,NA='TCP/IP!internet=192.35.18.18+port=21012'"
