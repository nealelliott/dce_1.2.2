#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: rpc.cds.3_gen_rep.awk,v $
# Revision 1.1.6.2  1996/03/11  02:45:44  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:03  marty]
#
# Revision 1.1.6.1  1995/12/11  22:42:49  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:20:00  root]
# 
# Revision 1.1.4.1  1994/06/10  20:50:07  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:19:39  devsrc]
# 
# Revision 1.1.2.2  1993/03/12  04:27:38  mhickey
# 	Initial check in of the rpc.cds.3  system test.
# 	[1993/01/29  19:58:36  mhickey]
# 
# 	Initial submit of the final report generation awk script.
# 	[1993/01/06  20:43:09  mhickey]
# 
# Revision 1.1.4.4  1993/01/22  20:44:52  mhickey
# 	Added code to print start time, avoid printing thread exit
# 	messages.
# 	[1993/01/20  19:56:34  mhickey]
# 
# Revision 1.1.4.2  1993/01/13  19:57:06  mhickey
# 	Initial submit for final report generation script.
# 	[1993/01/13  19:54:04  mhickey]
# 
# $EndLog$
#
# Takes the summaries produced by rpc.sec.2_gen_summ.awk and 
# formats them into something more humanly readable.  Also prints
# some totals at the end of the run.
#
BEGIN {
  max_cli_run=0; min_tcp_run=0; min_udp_run=0; avg_cli_run=0; avg_tcp_run=0;
  avg_udp_run=0; max_cli_run=0; max_tcp_run=0; max_udp_run=0; errcnt=0;
  id_refer=0; min_call=0; min_tcp=0; min_udp=0; avg_call=0; avg_tcp=0; 
  avg_udp=0;  max_call=0; max_tcp=0; max_udp=0; did_err_hdr=0;
}

$1 == "Cli:" { 
               if ($3 != "STARTING" )
               { 
                  printf("   Exit Status: %s at %s %s %s %s\n", $3, $9, $10,
                                                                     $11, $12);
                  printf("\n   Call Statistics\n   ---------------\n");
                  printf("   Passed: %s      Failed: %s     Total: %s\n", $19, $21, $17);
                  if ($38 != 0)
                  {
                     printf("\n   Timing Statistics\n   -----------------\n");
                     printf("   %s %s %s %s %s\n   %s %s %s %s %s\n   %s %s %s %s %s\n   %s %s  %s/call\n",  
                              $22, $23, $24, $25, $26, $27, $28, $29, $30, $31,
                              $32, $33, $34, $35, $36, $37, $38, $39);
                  }
                  printf("\n   Identity Refresh Statistics\n");
                  printf("   ---------------------------\n");
                  if ($41 == 0)
                     printf("   No identity refreshes done.\n");
                  else
                  { 
                     printf("   Refreshes: %d    Last at: %s %s %s %s %s\n",
                       $41,$43, $44, $45, $46, $47);
                  }

                  did_err_hdr=0;
                }
                else
                {
                  # print a header
                  printf("\n=====================================================\n");
                  printf("\nClient: %s\n-----------\n", $2);
                  printf("   Environment\n   -----------\n   %s %s   %s %s   Protocol: %s\n",$4, $5, $6, $7, $15);
                  printf("   Started at: %s %s %s %s\n", $9, $10,$11, $12);
                }
             }               

$1 != "Cli:" && $2 != "RUNTIME:" && $2 != "freeing" && $3 !~ "thread" && $1 ~ /[a-zA-Z]/ {
              # Report Errors
              if (did_err_hdr == 0)
              {
                 printf("\n   Errors Encountered\n   ------------------\n");
                 did_err_hdr=1;
              }
              printf("     %s\n",$0);
            }
$2 == "RUNTIME:" {
              printf("\n   Duration of Run: %s %s %s %s %s %s\n",
                       $3, $4, $5, $6, $7, $8);
                }

END { printf("\n=====================================================\n"); }
