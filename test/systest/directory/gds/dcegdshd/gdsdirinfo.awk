#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: gdsdirinfo.awk,v $
# Revision 1.1.5.2  1996/03/11  02:41:33  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:06:28  marty]
#
# Revision 1.1.5.1  1995/12/13  21:55:25  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/13  21:45:04  root]
# 
# Revision 1.1.2.1  1993/09/23  20:24:21  pellis
# 	Moved from ./systest/tools/*.
# 	[1993/09/23  20:18:50  pellis]
# 
# Revision 1.1.5.3  1993/04/03  05:05:33  mhickey
# 	Rewritten.  Now takes sorted output from gdsdirinfo, minus
# 	the comments, and instead of worrying about how many DSA's,
# 	and what order the lines are in, just ascertains that the
# 	minimal set of processes for the specified configuration are
# 	up and in a non-waiting state.
# 
# 	Defect 7071
# 	[1993/04/03  05:03:10  mhickey]
# 
# Revision 1.1.5.2  1993/02/05  15:28:01  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:49:05  cjd]
# 
# Revision 1.1.3.2  1993/02/05  13:18:21  cjd
# 	Embedded copyright notice
# 
# Revision 1.1.2.2  1992/07/27  18:56:37  mhickey
# 	Fixed some funcrtional errors
# 
# 	Defect 4817.
# 	[1992/07/27  17:25:55  mhickey]
# 
# 	Genesis.  Created as a tool to verify the state of the
# 	directory service after setup.  Called by gdsSetup.
# 
# 	Defect 4817
# 	[1992/07/27  16:53:12  mhickey]
# 
# $EndLog$
#
# parse the output from gdsdirinfo to verify the state of the directory
# service.  Input file looks like
#
#   cli_only or cli_srv_dir1 or cli_srv
#     < output from gdsdirinfo | grep -v "^#" | sort >
#   EOINPUT
#
# Prints 1 if the output from gdsdirinfo is correct for a quiescent directory
# service of the type indicated by the first line of the input file; 
# ie: cli_srv = client server, cli_only = client only,  
#     cli_srv_dir1 = client server for dir id 1; else prints 0.
# Called by gdsSetup to verify the state of the directory service after
# it is initialized. 
#
BEGIN {
        got_dsa1=0; got_dsa2=0; got_sstub1=0; got_sstub2=0; got_monitor=0;
        got_cache=0; got_cstub=0; good_setup=0;
      }

/cli_/ { TYPE = $1 }

$1 == "Monitor" { if ($5 == "-")
                    got_monitor=1;
                }

$1 == "DUA-Cache" { if ($5 == "-")
                      got_cache=1;
                  }

$1 == "C-Stub" { if ($5 == "-")
                      got_cstub=1;
               }

$1 == "S-Stub" { if ($5 == "-")
                 {
                    if ($3 == 1)
                      got_sstub1=1;
                    else
                      got_sstub2=1;
                 }
               }

$1 == "DSA"    { if ($5 == "-")
                 {
                    if ($3 == 1)
                      got_dsa1=1;
                    else
                      got_dsa2=1;
                 }
               }

$1 == "EOINPUT" { 
                  #
                  # All installations need these three processes
                  #
                  if ( got_monitor && got_cache && got_cstub)
                  {
                     #
                     # check a client installation - actually, if we are in 
                     # this block, then the setup is okay. :-)
                     #
                     if (TYPE == "cli_only")
                        good_setup = 1;
                  
                     #
                     # check the setup of a client/server installation for 
                     # dir id 1.  This means an s-stub and a dsa process.
                     #
                     if (TYPE=="cli_srv_intl_dir1"||TYPE=="cli_srv_other_dir1")
                     {
                        if (got_sstub1 && got_dsa1)
                           good_setup = 1;
                     }

                     #
                     # check the setup of a client/server installation for 
                     # dir id 1 & 2.  This means two s-stub's and two dsa 
                     # processes.
                     #
                     if (TYPE=="cli_srv_intl_dir1"||TYPE=="cli_srv_other_dir1")
                     {
                        if (got_sstub1 && got_sstub2 && got_dsa1 && got_dsa2)
                           good_setup = 1;
                     }
                  }  ### End of if (got_monitor...)  ###

                  #
                  # Report the setup state
                  #
                  print good_setup;
                 
                }  ### End of action for EOINPUT ###

