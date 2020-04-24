# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: rpc.cds.3_chkcli.awk,v $
# Revision 1.1.6.2  1996/03/11  02:45:39  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:02  marty]
#
# Revision 1.1.6.1  1995/12/11  22:42:37  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:19:52  root]
# 
# Revision 1.1.4.1  1994/06/10  20:50:00  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:19:30  devsrc]
# 
# Revision 1.1.2.2  1993/03/12  04:27:05  mhickey
# 	Initial check in for the rpc.cds.3 system test.
# 	[1993/01/29  19:08:19  mhickey]
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

