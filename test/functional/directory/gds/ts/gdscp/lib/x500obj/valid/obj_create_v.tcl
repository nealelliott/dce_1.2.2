# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_create_v.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:19  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:31  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:22  root
# 	Submit
# 	[1995/12/11  14:36:44  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:37  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:24:04  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:22:49  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:02  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:09  marrek
# 	Updated.
# 	[1994/05/11  10:25:17  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:45  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:16:02  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_create_v.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:19 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_create_v.tcl                              	  #
# Description : This file contains all the valid test cases       #
#		for the create operation on the x500obj object    #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_create_01v {} {
    bind_to_dsa
    set test_case_name "obj_create_01v"
    set command "x500obj create /c=de -attribute ocl=c"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de"
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_create_02v {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"
    set test_case_name "obj_create_02v"
    set command "x500obj create /c=de/o=sni -attribute ocl=org"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni"
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de/o=sni"
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_create_03v {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute ocl=org"
    run_cmd_and_log "set gdscp_cwo /c=de/o=sni"
    set test_case_name "obj_create_03v"
    set command "x500obj cr ou=ap11 -a ocl=ou"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11"
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11"
    run_cmd_and_log "x500obj delete /c=de/o=sni"
    run_cmd_and_log "x500obj delete /c=de"
    run_cmd_and_log "set gdscp_cwo /"
}

proc obj_create_04v {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute ocl=org"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11 -attribute ocl=ou"
    run_cmd_and_log "set gdscp_cwo /c=de/o=sni"
    set test_case_name "obj_create_04v"
    set command "x500obj crea ou=ap11/cn=naik -attribute ocl=orp sn=naik \"tn=12345;369072;576268 \" {{up=\\x6e\\x61\\x69\\x6b}}"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=naik"
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=naik"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11"
    run_cmd_and_log "x500obj delete /c=de/o=sni"
    run_cmd_and_log "x500obj delete /c=de"
    run_cmd_and_log "set gdscp_cwo /"
}

proc obj_create_05v {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute ocl=org"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11 -attribute ocl=ou"
    set test_case_name "obj_create_05v"
    set command "x500obj create /c=de/o=sni/ou=ap11/cn=naik,ou=ap11 -attribute {ocl=orp sn=naik tn=99999}"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=naik,ou=ap11"
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=naik,ou=ap11"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11"
    run_cmd_and_log "x500obj delete /c=de/o=sni"
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_create_06v {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute ocl=org"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11 -attribute ocl=ou"
    set test_case_name "obj_create_06v"
    set command "x500obj create /c=de/o=sni/ou=ap11/cn=muller -attribute ocl=orp {sn=John {dsc=Software Professional}}"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=muller"
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=muller"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11"
    run_cmd_and_log "x500obj delete /c=de/o=sni"
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_create_07v {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute ocl=org"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11 -attribute ocl=ou"
    set test_case_name "obj_create_07v"
    set command "x500obj create /c=de/o=sni/ou=ap11/cn=mueller -attr ocl=orp sn=Henry \"dsc=Engineer\""
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=mueller"
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=mueller"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11"
    run_cmd_and_log "x500obj delete /c=de/o=sni"
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_create_08v {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute ocl=org"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11 -attribute ocl=ou"
    set test_case_name "obj_create_08v"
    set command "create /c=de/o=sni/ou=ap11/cn=mail1 -attribute ocl=mdl {{mds={pt=0, ind={dn={/c=de/o=sni}, admd=admd, cn='common name', c=de,dt1=dt1,dt2=dt2,dt3=dt3,dt4=dt4,dv1=dv1,dv2=dv2,dv3=dv3,dv4=dv4,gen=gen,gn=given,ini=init,o=orgname,ou1=ou1,ou2=ou2,ou3=ou3,ou4=ou4,prmd=prmd,sn=surname}}}} moa={admd=admd,c=de,prmd=prmd}"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=mail1"
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=mail1"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11"
    run_cmd_and_log "x500obj delete /c=de/o=sni"
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_create_09v {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute ocl=org"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11 -attribute ocl=ou"
    set test_case_name "obj_create_09v"
    set command "create /c=de/o=sni/ou=ap11/cn=mail2 -attribute ocl=mdl {{mds={pt=0, ind={dn={/c=de/o=sni/ou=ap11/cn=ind1}, admd=admd, c=de,o=orgname,prmd=prmd}};{pt=0,ind={dn={/c=de/o=sni/ou=ap11/cn=Submit},admd=admd,c=de,o=orgname,prmd=prmd}}}} \"moa={admd=admd,c=de,o=orgname,ou1=ou1,ou2=ou2,prmd=prmd}\" \"mdm={dn={/c=de/o=sni/ou=ap11},admd=admd,c=de,prmd=prmd}\""
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=mail2"
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=mail2"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11"
    run_cmd_and_log "x500obj delete /c=de/o=sni"
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_create_10v {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute ocl=org"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11 -attribute ocl=ou"
    set test_case_name "obj_create_10v"
    set command "create /c=de/o=sni/ou=ap11/cn=Brown -attribute ocl=mdl {{dsc=Software Consultant}} {{mds={pt=0, ind={dn={/c=de/o=sni}, c=de, admd=dbp, prmd=sni, o=Siemens Nixdorf, ou1=Munich, ou2=P1, ou3=P4, ou4=ap113, cn=Alfred Brown, gn=Alfred, ini=P., sn=Brown, gen=396, dt1=MS MAIL, dv1=Brown}}}} moa={admd=admd,c=de,prmd=prmd}"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=Brown"
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=Brown"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11"
    run_cmd_and_log "x500obj delete /c=de/o=sni"
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_create_11v {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute ocl=org"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11 -attribute ocl=ou"
    set test_case_name "obj_create_11v"
    set command "create /c=de/o=sni/ou=ap11/cn=Andrews -attribute ocl=mdl {{dsc=Project Leader}} {{mds={pt=0, ind={dn={/c=de/o=sni}, c=de, admd=dbp, prmd=sni, o=Siemens Nixdorf, ou1=Munich, ou2=P1, ou3=P4, ou4=ap113, cn=Peter Andrews, gn=Peter, ini=P., sn=Andrews, gen=396, dt1=MS MAIL, dv1=Peter}}}} moa={admd=admd,c=de,prmd=prmd}"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=Andrews"
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=Andrews"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11"
    run_cmd_and_log "x500obj delete /c=de/o=sni"
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_create_12v {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute ocl=org"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11 -attribute ocl=ou"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=dsa -attribute ocl=app"
    set test_case_name "obj_create_12v"
    set command "create /c=de/o=sni/ou=ap11/cn=dsa/cn=dsa15 -attribute ocl=ape \"PSA={NA='TCP/IP!internet=127.0.0.1+port=25015',TS=Server}\""
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=dsa/cn=dsa15"
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=dsa/cn=dsa15"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=dsa"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11"
    run_cmd_and_log "x500obj delete /c=de/o=sni"
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_create_13v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_create_13v"
    set command "x500obj create /c=de/o=sni/ou=ap11/cn=müller -attribute ocl=orp sn=müller"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=müller"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=müller"
    remove_dit_objects
}

proc obj_create_14v {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "set gdscp_t61 TRUE"
    set test_case_name "obj_create_14v"
    set command "x500obj create /c=de/o=sni/ou=ap11/cn=m\\\\xc8uller -attribute ocl=orp sn=m\\\\xc8uller"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=m\\\\xc8uller"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=m\\\\xc8uller"
    run_cmd_and_log "set gdscp_t61 FALSE"
    remove_dit_objects
}
