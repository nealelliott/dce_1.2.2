#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: gdschdb.sh,v $
# Revision 1.1.12.2  1996/03/09  23:13:40  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:30:57  marty]
#
# Revision 1.1.12.1  1995/12/08  15:49:47  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:08:25  root]
# 
# Revision 1.1.10.2  1994/05/10  15:53:02  marrek
# 	April 1994 code submission.
# 	[1994/04/27  12:26:30  marrek]
# 
# Revision 1.1.10.1  1994/02/22  18:40:55  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  10:22:14  marrek]
# 
# Revision 1.1.8.1  1993/10/29  15:11:25  keutel
# 	CR 9261
# 	[1993/10/29  14:08:28  keutel]
# 
# Revision 1.1.6.2  1993/08/10  15:43:44  marrek
# 	July 1993 code drop.
# 	[1993/07/30  13:23:10  marrek]
# 
# Revision 1.1.4.3  1992/12/31  19:43:24  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:21:54  bbelch]
# 
# Revision 1.1.4.2  1992/10/05  16:50:48  marrek
# 	Insert #!/bin/sh .
# 	[1992/10/05  16:50:17  marrek]
# 
# Revision 1.1.2.2  1992/06/01  23:54:15  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  20:59:37  zeliff]
# 
# $EndLog$
#
# $RCSfile: gdschdb.sh,v $ $Revision: 1.1.12.2 $ $Date: 1996/03/09 23:13:40 $
#
#****************************************************************************
#                                                                           *
#        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
#                             ALL RIGHTS RESERVED                           *
#                                                                           *
#****************************************************************************

# @SOURCEWARNING@

DIRNEW=$1
shift
DIRLOG=$1
shift
CHDBDIR=$DIRNEW/chdb

if [ -f $DIRNEW/scheme ]
then                  # transfer data
  cd $CHDBDIR
  if [ $? != 0 ]      # chdb directory doesn't exist
  then
       exit 1
  fi

  gdsgendb $CHDBDIR $DIRLOG $*
  if  [ $? != 0 ]
  then
     rm *             # delete content of CHDBDIR
     exit 2
  fi

  gdstransfer $DIRNEW $CHDBDIR $DIRLOG $*
  if  [ $? != 0 ]
  then
     rm *
     exit 3
  fi
else                  # call was made after restoring data
		      # run only gdsgendb to produce scheme file
  gdsgendb $DIRNEW $DIRLOG $*
  if  [ $? != 0 ]
  then
     exit 4
  fi
fi

exit 0
