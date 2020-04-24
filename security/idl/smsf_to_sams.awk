# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: smsf_to_sams.awk,v $
# Revision 1.1.4.2  1996/03/09  23:29:25  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:44:47  marty]
#
# Revision 1.1.4.1  1995/12/08  17:38:34  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:56:17  root]
# 
# Revision 1.1.2.2  1994/06/17  18:42:38  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:40:19  devsrc]
# 
# Revision 1.1.2.1  1994/03/25  19:03:05  sekhar
# 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
# 	[1994/03/25  19:02:13  sekhar]
# 
# $EndLog$

# File : smsf_to_sams.awk
#
#    The purpose of this awk script is to convert a SMSF ( Symbolic
#    Message Source File ) defining status codes into a sams file
#    which is required for DCE1.1 Servceability work.
#
#    Since this script is currently used to convert only sec.msg
#    (the SMSF file for "sec" component) this script has hard-coded
#    information for producing the sams file for the "sec" component.
#
#    The resulting sams file is *NOT* the version of the SAMS file
#    which is to be shipped since some of the entries in the sams
#    file are either not filled in or filled with dummy values.
#    These will be filled/modified in the final version of the SAMS 
#    file.
#

BEGIN {
      status_num = 0
      printf("#  This file has been automatically generated\n")
      printf("#  Do not edit !! \n\n")
      tech       = "dce"
      co_name    = "sec"
      co_table   = "sec__table"
      printf("component      %s\n",   co_name)
      printf("table          %s\n",   co_table)
      printf("technology     %s\n\n", tech ) 

      #  The actual sub-components will be filled in later on.

      printf("serviceability table %s_svc_table handle %s_svc_handle\n",co_name,co_name)
      printf("start\n")
      printf("    sub-component sec_s_general \"general\" sec_i_svc_general\n")
      printf("    sub-component sec_s_events  \"events\"  sec_i_svc_events\n")
      printf("end\n")
      printf("\n\n")
}

#  Strip blank lines

/^[  ]*$/ {
	next
}

#  Skip comment lines

/^[#]/ {
	next
}

/^[A-Za-z]/ {
       if (status_num > 0) {
           #   This version of awk does not use the "set value = "
           #   construct to skip over reserved messages. The final
           #   version of the sams file will do that ( I (Sekhar) 
           #   will extend  this tool to do this later. )

           print "start\ncode    " $1
           printf("text    \"")
           for (i = 2; i < NF ; i++)
           {
               printf("%s ", $i)
           }
           printf("%s\"\n", $NF)
           printf("explanation    \"?\"\n")
           printf("action         \"None required\"\n")
           printf("end\n\n")
       }
       status_num++
       next
}

END {
    printf("start		!intable undocumented\n")
    printf("code		sec_i_svc_general\n")
    printf("text		\"General server administration\"\n")
    printf("end\n")
    printf("\n\n")
    printf("start		!intable undocumented\n")
    printf("code		sec_i_svc_events\n")
    printf("text		\"Events received and acted upon\"\n")
    printf("end\n")
    printf("\n\n")
}
