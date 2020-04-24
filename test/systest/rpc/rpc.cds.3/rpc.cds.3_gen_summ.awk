#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: rpc.cds.3_gen_summ.awk,v $
# Revision 1.1.6.2  1996/03/11  02:45:48  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:04  marty]
#
# Revision 1.1.6.1  1995/12/11  22:42:50  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:20:00  root]
# 
# Revision 1.1.4.1  1994/06/10  20:50:10  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:19:41  devsrc]
# 
# Revision 1.1.2.3  1993/03/12  04:55:58  mhickey
# 	Submitted wrong version.
# 	[1993/03/12  04:55:20  mhickey]
# 
# Revision 1.1.2.2  1993/03/12  04:27:43  mhickey
# 	*** empty log message ***
# 	[1993/01/29  20:26:32  mhickey]
# 
# Revision 1.1.2.3  1993/01/22  20:44:50  mhickey
# 	Added code to print the starting report line.
# 	[1993/01/20  20:00:01  mhickey]
# 
# Revision 1.1.2.2  1992/12/03  19:53:32  mhickey
# 	Fixed a typo in error message.
# 	[1992/12/03  19:10:23  mhickey]
# 
# 	Initial check submit for rpc.sec.2 system test.
# 
# 	Defect 5887.
# 	[1992/12/03  15:39:52  mhickey]
# 
# $EndLog$
#
#
# Looks for lines like
#  Cli: 1 STARTING Host: name O/S: OSF1 Time: time Protcl: Calls: Pass: Fail:
#  Min: sec nsec Max: sec nsec Avg: sec id_refr last
# in the input stream, saves each one, then buffers the lines following it
# until the next such line or EOI.  When EOI is encountered, the buffered 
# lines are dumped.  The effect is to strip off the interesting stuff at the
# end of the client logs for processing by the rpc.sec.2_gen_rep.awk script.
#
# if lines of the form Client( x ): FAILED ... are encountered, the script
# checks for FAILED and SUCCEEDED messages and prints a line that contains
# the statistics it compiles.
#
BEGIN {
   nudp=0; ntcp=0; nudp_pass=0; ntcp_pass=0; nudp_fail=0; ntcp_fail=0;
   nclients=0; nsucceed=0; nfail=0; nbusy=0; nudp_busy=0; ntcp_busy=0;
   nothr=0; nudp_othr=0; ntcp_othr=0; last_summ=""; lincnt=0; last_start="";
}
$1 ~ /Client\(/ && $1 !~ /Client\([0-9]/  { nclients++;
                   if ( $6 == "SUCCEEDED" )
                   {
                      nsucceed++;
                      if ( $5 == "ncacn_ip_tcp" )
                      {
                         ntcp++;
                         ntcp_pass++;
                      }
                      else
                      {
                         nudp++;
                         nudp_pass++;
                      }
                   } 
                   else
                   {
                      nfail++;
                      if ( $5 == "ncacn_ip_tcp" )
                      {
                         ntcp++;
                         ntcp_fail++;
                         if ( $11 == "BUSY" )
                         {
                            nbusy++;
                            ntcp_busy++;
                         }
                         else
                         {
                            nothr++;
                            ntcp_othr++;
                         }
                      }
                      else
                      {
                         nudp++;
                         nudp_fail++;
                         if ( $11 == "BUSY" )
                         {
                            nbusy++;
                            nudp_busy++;
                         }
                         else
                         {
                            nothr++;
                            nudp_othr++;
                         }
                      }
                   }
                 }

$1 ~ /Cli:/ && $3 == "STARTING"  {
                                   if ($2 != last_start)
                                   {
                                      print $0;
                                      last_start=$2;
                                   }
                                 }
                  
$1 ~ /Cli:/      {
                   last_summ=$0;
                   lincnt=0;
                 }
                
                 {
                   if ( last_summ != "" && last_summ != $0 )
                      lines[lincnt++] = $0;
                 }
END {
   if ( nclients > 0 )
      print nclients, nudp, ntcp, nsucceed, nudp_pass, ntcp_pass, nfail, nbusy, nothr, nudp_fail, nudp_busy, nudp_othr, ntcp_fail, ntcp_busy, ntcp_othr
   else
   {
      if (last_summ == "" )
         print "No client information lines in input";
      else
      {
         print last_summ;
         for (i=0; i < lincnt; i++)
            print lines[i];
      }
   }
}
