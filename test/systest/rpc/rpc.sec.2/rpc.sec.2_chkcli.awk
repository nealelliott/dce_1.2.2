#
# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: rpc.sec.2_chkcli.awk,v $
# Revision 1.1.67.2  1996/03/11  02:46:30  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:21  marty]
#
# Revision 1.1.67.1  1995/12/11  22:43:45  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:20:33  root]
# 
# Revision 1.1.65.1  1993/09/09  19:12:45  cmckeen
# 	 HP's TET'ized version
# 	[1993/09/09  19:11:43  cmckeen]
# 
# Revision 1.1.5.2  1993/08/16  14:25:01  eheller
# 	File tetified.
# 	[1993/07/23  17:42:30  eheller]
# 
# Revision 1.1.1.5  1993/05/04  22:22:03  mgm
# 	    Loading drop DCE1_0_2FNL
# 
# Revision 1.1.2.3  1993/02/05  15:22:19  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:43:09  cjd]
# 
# Revision 1.1.3.2  1993/02/04  22:11:52  cjd
# 	Embedded copyright notice
# 
# Revision 1.1.2.2  1992/12/03  19:53:26  mhickey
# 	Initial check submit for rpc.sec.2 system test.
# 
# 	Defect 5887.
# 	[1992/12/03  15:39:41  mhickey]
# 
# $EndLog$
# 
# rpc.sec.2_chk_cli.awk
# 
# ASSOCIATION
#    This file is part of the rpc.sec.2 DCE System Test.
# 
#    The rpc.sec.2 system test corresponds to test rpc.sec.2 in the DCE1.0.2
#    Core Component System Test plan.  For more information on the operation
#    and goals of the rpc.sec.2 system test, see the test plan, or the README
#    file in this directory.
#
#
# FUNCTION
#    Verifies that all clients are running by inspecting the list of lines of 
#    the form "Client( <seq_numb> ): READY to make calls" in the input stream.
#    The input stream must contain a line of the form "sequence = <numb>" that
#    defines the last sequence number that should be found.  Note that the 
#    input must be sorted by sequence number.  Outputs "okay" if all sequence
#    numbers are found, or a list of missing sequence numbers.
#
# CALLED BY
#    rpc.sec.2_runtest.sh
#
# CALLS INTO
#    nothing.
#
# PREREQUISITES
#    None.
#
#
BEGIN {
  expectseq=1;
  actualseq=0;
  lastseq=0;
  missingseqcnt=0;
}

{ if ($1 == "sequence")
     lastseq = $3;
  else
  {
     actualseq=$2;
     if (actualseq != expectseq)
     {
        for (i=expectseq; i<actualseq; i++)
           missingseq[missingseqcnt++] = i;
        expectseq = actualseq + 1;
     }
     else
        expectseq = expectseq + 1;
  }
}

END {
      if ( missingseqcnt == 0 && expectseq-1 == lastseq)
         print "okay"
      else
      {
         for (i = 0; i < missingseqcnt; i++ )
         {
            if (i > 0)
               printf(" ");
            printf("%s", missingseq[i]);
         }
         if ( actualseq != lastseq )
         {
            for ( i = actualseq+1; i <= lastseq; i++)
            {
               if (i > $actualseq+1)
                  printf(" ");
               printf("%s", i);
            }
         }
         printf("\n");
      }
}

