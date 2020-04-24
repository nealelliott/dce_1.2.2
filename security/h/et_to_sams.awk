# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: et_to_sams.awk,v $
# Revision 1.1.4.2  1996/03/09  23:27:33  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:43:02  marty]
#
# Revision 1.1.4.1  1995/12/08  17:27:03  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:53:19  root]
# 
# Revision 1.1.2.2  1994/06/17  18:42:00  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:08:37  devsrc]
# 
# Revision 1.1.2.1  1994/03/25  13:56:54  sekhar
# 	[ OT 10164 ]  Initial Version. SVC code drop ( sec_svc_bl1 )
# 	[1994/03/18  21:34:24  sekhar]
# 
# $EndLog$

# File : smsf_to_sams.awk
#
#    The purpose of this awk script is to convert a file defining
#    status codes (in a format which can be processed by compile_et)
#    into a sams file which is required for DCE1.1 Servceability work.
#
#    Since this script is currently used to convert only sad_err.et
#    (the file for "sad" component) this script has hard-coded
#    information for producing the sams file for the "sad" component.
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
      co_name    = "sad"
      co_table   = "sad__table"
      printf("component      %s\n",   co_name)
      printf("table          %s\n",   co_table)
      printf("technology     %s\n\n", tech)

      #   A client strictly speaking should not require a sub-component
      #   table since sub-components are not required by a client. However
      #   the sams compiler will generate an error unless a sub-component
      #   is present.

      printf("serviceability table sad_svc_table handle sad_svc_handle\n")
      printf("start\n")
      printf("    sub-component sad_s_dummy \"dummy\" sad_i_svc_dummy\n")
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

#  skip  the error_table.
/^error_table/ {
	next
}

/^[A-Za-z]/ {
       if (status_num > 0) {
           #   This version of awk does not use the "set value = "
           #   construct to skip over reserved messages. The final
           #   version of the sams file will do that ( I (Sekhar) 
           #   will extend  this tool to do this later. )

           if ( $1 == "ec" ) {
               err_code = $2
               #  Strip the trailing comma from err_code
               sub(",","",err_code) 
               print "start\ncode    " err_code
               printf("text    ")
               for (i = 3 ; i <= NF ; i++)
               {
                   printf("%s ", $i)
               }
               printf("\n")
	       printf("explanation    \"?\"\n")
	       printf("action         \"None required\"\n")
	       printf("end\n\n")
           }
       }
       status_num++
       next
}

END {
     
    printf("start		!intable undocumented\n")
    printf("code		sad_i_svc_dummy\n")
    printf("text		\"Dummy - not used \"\n")
    printf("end\n")
    printf("\n\n")
}
