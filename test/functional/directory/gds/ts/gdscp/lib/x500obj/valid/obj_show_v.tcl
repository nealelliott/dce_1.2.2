# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_show_v.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:37  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:42  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:45  root
# 	Submit
# 	[1995/12/11  14:36:51  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:47  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:24:20  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:23:10  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:12  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:14  marrek
# 	Updated.
# 	[1994/05/11  10:25:33  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:35:10  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:16:21  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_show_v.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:37 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_show_v.tcl                              	  #
# Description : This file contains all the valid test cases       #
#		for the show operation on the x500obj object      #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_show_01v {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "set gdscp_cwo /c=de/o=sni/ou=ap11/cn=naik"
    set test_case_name "obj_show_01v"
    set command "x500obj show -attribute cn"
    set exp_result "CN=naik"
    run_testcase_and_extract $test_case_name $command $exp_result 2
    remove_dit_objects
    run_cmd_and_log "set gdscp_cwo /"
}

proc obj_show_02v {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "set gdscp_cwo /c=de/o=sni/ou=ap11/cn=naik"
    set test_case_name "obj_show_02v"
    set command "x500obj show -a {cn tn}"
    set exp_result "TN=12345;369072;576268"
    run_testcase_and_extract $test_case_name $command $exp_result 3
    remove_dit_objects
    run_cmd_and_log "set gdscp_cwo /"
}

proc obj_show_03v {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "set gdscp_cwo /c=de/o=sni/ou=ap11/cn=naik"
    set test_case_name "obj_show_03v"
    set command "x500obj show"
    set exp_result "TN=12345;369072;576268"
    run_testcase_and_extract $test_case_name $command $exp_result 5
    remove_dit_objects
    run_cmd_and_log "set gdscp_cwo /"
}

proc obj_show_04v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_show_04v"
    set command "x500obj sho /c=de/o=sni/ou=ap11/cn=naik -t"
    set exp_result "UP"
    run_testcase_and_extract $test_case_name $command $exp_result 6
    remove_dit_objects
}

proc obj_show_05v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_show_05v"
    set command "show /c=de/o=sni/ou=ap11/cn=naik -attr tn sn ocl up"
    set exp_result "UP='\\x6e\\x61\\x69\\x6b'"
    run_testcase_and_extract $test_case_name $command $exp_result 5
    remove_dit_objects
}

proc obj_show_06v {} {
    bind_to_dsa
    create_dit_objects
    create_dit_mail_objects
    set test_case_name "obj_show_06v"
    set command "show /c=de/o=sni/ou=ap11/cn=mail1"
    set exp_result "MDS={PT=0,IND={DN={/C=de/O=sni},ADMD=admd,CN=common name,C=de,DT1=dt1,DT2=dt2,DT3=dt3,DT4=dt4,DV1=dv1,DV2=dv2,DV3=dv3,DV4=dv4,GEN=gen,GN=given,INI=init,O=orgname,OU1=ou1,OU2=ou2,OU3=ou3,OU4=ou4,PRMD=prmd,SN=surname}}"
    run_testcase_and_extract $test_case_name $command $exp_result 4
    remove_dit_mail_objects
    remove_dit_objects
}

proc obj_show_07v {} {
    bind_to_dsa
    create_object_with_local_strings
    set test_case_name "obj_show_07v"
    set command "x500obj show /c=de/o=sni/ou=ap11/cn=müller"
    set exp_result "/C=de/O=sni/OU=ap11/CN=müller {OCL=ORP;PER;TOP} CN=müller SN=müller"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_object_with_local_strings
}

proc obj_show_08v {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "set gdscp_t61 TRUE"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=m\\\\xc8uller -attribute ocl=orp sn=m\\\\xc8uller"
    run_cmd_and_log "set gdscp_t61 FALSE"
    set test_case_name "obj_show_08v"
    set command "x500obj show /c=de/o=sni/ou=ap11/cn=müller"
    set exp_result "/C=de/O=sni/OU=ap11/CN=müller {OCL=ORP;PER;TOP} CN=müller SN=müller"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=müller"
    remove_dit_objects
}
