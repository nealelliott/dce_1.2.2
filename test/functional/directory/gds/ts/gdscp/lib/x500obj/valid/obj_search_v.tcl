# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_search_v.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:35  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:40  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:43  root
# 	Submit
# 	[1995/12/11  14:36:50  root]
# 
# Revision 1.1.2.5  1994/07/06  15:11:46  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:24:17  marrek]
# 
# Revision 1.1.2.4  1994/06/21  14:29:31  marrek
# 	June 1994 code submission.
# 	[1994/06/16  07:50:30  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:23:09  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:11  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:14  marrek
# 	Updated.
# 	[1994/05/11  10:25:31  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:35:06  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:16:19  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_search_v.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:35 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_search_v.tcl                              	  #
# Description : This file contains all the valid test cases       #
#		for the search operation on the x500obj object    #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_search_01v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_search_01v"
    set command "x500obj search / -filter cn=naik -subtree"
    set exp_result "/C=de/O=sni/OU=ap11/CN=naik,OU=ap11"
    run_testcase_and_extract $test_case_name $command $exp_result 2
    remove_dit_objects
}

proc obj_search_02v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_search_02v"
    set command "x500obj search /c=de/o=sni/ou=ap11/cn=mueller -baseobject"
    set exp_result "/C=de/O=sni/OU=ap11/CN=mueller"
    run_testcase_and_extract $test_case_name $command $exp_result 1
    remove_dit_objects
}

proc obj_search_03v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_search_03v"
    set command "x500obj search /c=de/o=sni/ou=ap11 -filt {{cn=mue* || cn=mi*}} -subtree -typ"
    set exp_result "DSC"
    run_testcase_and_extract $test_case_name $command $exp_result 1 5
    remove_dit_objects
}

proc obj_search_04v {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "set gdscp_cwo /c=de"
    set test_case_name "obj_search_04v"
    set command "x500obj search -subtree"
    set exp_result "/C=de/O=sni/OU=ap11/CN=muller"
    run_testcase_and_extract $test_case_name $command $exp_result 4
    remove_dit_objects
    run_cmd_and_log "set gdscp_cwo /"
}

proc obj_search_05v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_search_05v"
    set command "x500obj search /c=de/o=sni/ou=ap11/cn=mueller -noaliases -types -subtree"
    set exp_result "SN"
    run_testcase_and_extract $test_case_name $command $exp_result 1 4
    remove_dit_objects
}

proc obj_search_06v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_search_06v"
    set command "x500obj search /c=de/o=sni/ou=ap11/cn=mueller -attr ocl -subtree"
    set exp_result "/C=de/O=sni/OU=ap11/CN=mueller"
    run_testcase_and_extract $test_case_name $command $exp_result 1 1
    remove_dit_objects
}

proc obj_search_07v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_search_07v"
    set command "x500obj search /c=de/o=sni/ou=ap11/cn=mueller -allattr -subtree"
    set exp_result "OCL=ORP;PER;TOP"
    run_testcase_and_extract $test_case_name $command $exp_result 1 2
    remove_dit_objects
}

proc obj_search_08v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_search_08v"
    set command "search /c=de/o=sni -allattr -onelevel"
    set exp_result "OU=ap11"
    run_testcase_and_extract $test_case_name $command $exp_result 1 3
    remove_dit_objects
}

proc obj_search_09v {} {
    bind_to_dsa
    create_dit_objects
    create_dit_mail_objects
    set test_case_name "obj_search_09v"
    set command "search /c=de/o=sni/ou=ap11 -filter {{cn=dsa || cn=mail1 || (cn=naik && tn=12345)}} -allattr -onelevel"
    set exp_result "MDS={PT=0,IND={DN={/C=de/O=sni},ADMD=admd,CN=common name,C=de,DT1=dt1,DT2=dt2,DT3=dt3,DT4=dt4,DV1=dv1,DV2=dv2,DV3=dv3,DV4=dv4,GEN=gen,GN=given,INI=init,O=orgname,OU1=ou1,OU2=ou2,OU3=ou3,OU4=ou4,PRMD=prmd,SN=surname}}"
    run_testcase_and_extract $test_case_name $command $exp_result 3 4
    remove_dit_mail_objects
    remove_dit_objects
}

proc obj_search_10v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_search_10v"
    set command "x500obj search /c=de/o=sni/ou=ap11/cn=mueller -b"
    set exp_result "/C=de/O=sni/OU=ap11/CN=mueller"
    run_testcase_and_extract $test_case_name $command $exp_result 1
    remove_dit_objects
}

proc obj_search_11v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_search_11v"
    set command "x500obj sea -filter cn=n* -sub"
    set exp_result "/C=de/O=sni/OU=ap11/CN=naik"
    run_testcase_and_extract $test_case_name $command $exp_result 1
    remove_dit_objects
}

proc obj_search_12v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_search_12v"
    set command "x500obj search /c=de/o=sni/ou=ap11/cn=mueller -n -t -subtree"
    set exp_result "SN"
    run_testcase_and_extract $test_case_name $command $exp_result 1 4
    remove_dit_objects
}

proc obj_search_13v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_search_13v"
    set command "x500obj search /c=de/o=sni/ou=ap11/cn=mueller -attribute {sn dsc} -subtree"
    set exp_result "DSC=Engineer"
    run_testcase_and_extract $test_case_name $command $exp_result 1 3
    remove_dit_objects
}

proc obj_search_14v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_search_14v"
    set command "search -filter {{(cn~=mueller) && ((OCL=ORP) || OCL=REP) && !(sn=henry) && (dsc=*) }} -subtree"
    set exp_result "/C=de/O=sni/OU=ap11/CN=miller"
    run_testcase_and_extract $test_case_name $command $exp_result 2
    remove_dit_objects
}

proc obj_search_15v {} {
    bind_to_dsa
    create_object_with_local_strings
    set test_case_name "obj_search_15v"
    set command "x500obj search /c=de/o=sni/ou=ap11/cn=müller -filter sn=müller -allattr -baseobject"
    set exp_result "{/C=de/O=sni/OU=ap11/CN=müller {OCL=ORP;PER;TOP} CN=müller SN=müller}"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_object_with_local_strings
}

proc obj_search_16v {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "set gdscp_t61 TRUE"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=m\\\\xc8uller -attribute ocl=orp sn=m\\\\xc8uller"
    run_cmd_and_log "set gdscp_t61 FALSE"
    set test_case_name "obj_search_16v"
    set command "x500obj search /c=de/o=sni/ou=ap11/cn=müller -filter sn=müller -allattr -baseobject"
    set exp_result "{/C=de/O=sni/OU=ap11/CN=müller {OCL=ORP;PER;TOP} CN=müller SN=müller}"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=müller"
    remove_dit_objects
}

proc obj_search_17v {} {
    bind_to_dsa
    run_cmd_and_log "set gdscp_t61 TRUE"
    log_message "Initializing ..."

    run_cmd_and_log "x500obj create /c=de -attribute {{OCL=C}  \
	{DSC=ABC\\x1B\\x24\\x29\\x42\\xB0\\xA1\\xF4\\xA4\\x1B\\x2C\\x46\\xC1\\xC2\\xC3\\x1B\\x29\\x75\\xC8A\\xC8O\\xC8UDEF}}"
#            lat[desig. seq.  kanji][1.charac. 2.charac][des.seq.greek] [alpha beta gamma][des.seq.latin][ AE  OE   UE]lat

    set test_case_name "obj_search_17v"
    set command "x500obj search /c=de -baseobject -allattr \
         -filter {{DSC=ABC\\x1B\\x24\\x29\\x42\\xB0\\xA1\\xF4\\xA4\\x1B\\x2C\\x46\\xC1\\xC2\\xC3\\x1B\\x29\\x75\\xC8A\\xC8O\\xC8UDEF}}"

    set exp_result "/C=de"
    run_testcase_and_extract $test_case_name $command $exp_result 1 1
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de"
    run_cmd_and_log "set gdscp_t61 FALSE"
}

proc obj_search_18v {} {
    bind_to_dsa
    run_cmd_and_log "set gdscp_t61 TRUE"
    log_message "Initializing ..."

    run_cmd_and_log "x500obj create /c=de -attribute {{OCL=C}  \
	{DSC=ABC\\x1B\\x24\\x29\\x42\\xB0\\xA1\\xF4\\xA4\\x1B\\x2C\\x46\\xC1\\xC2\\xC3\\x1B\\x29\\x75\\xC8A\\xC8O\\xC8UDEF}}"
#            lat[desig. seq.  kanji][1.charac. 2.charac][des.seq.greek] [alpha beta gamma][des.seq.latin][ AE  OE   UE]lat

    set test_case_name "obj_search_18v"
    set command "x500obj search /c=de -baseobject -allattr \
         -filter {{DSC=ABC\\x1B\\x24\\x29\\x42\\xB0\\xA1*}}"

    set exp_result "/C=de"
    run_testcase_and_extract $test_case_name $command $exp_result 1 1
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de"
    run_cmd_and_log "set gdscp_t61 FALSE"
}

proc obj_search_19v {} {
    bind_to_dsa
    run_cmd_and_log "set gdscp_t61 TRUE"
    log_message "Initializing ..."

    run_cmd_and_log "x500obj create /c=de -attribute {{OCL=C}  \
	{DSC=ABC\\x1B\\x24\\x29\\x42\\xB0\\xA1\\xF4\\xA4\\x1B\\x2C\\x46\\xC1\\xC2\\xC3\\x1B\\x29\\x75\\xC8A\\xC8O\\xC8UDEF}}"
#            lat[desig. seq.  kanji][1.charac. 2.charac][des.seq.greek] [alpha beta gamma][des.seq.latin][ AE  OE   UE]lat

    set test_case_name "obj_search_19v"
    set command "x500obj search /c=de -baseobject -allattr \
         -filter {{DSC=*\\x1B\\x2C\\x46\\xC1\\xC2\\xC3*}}"

    set exp_result "/C=de"
    run_testcase_and_extract $test_case_name $command $exp_result 1 1
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de"
    run_cmd_and_log "set gdscp_t61 FALSE"
}

proc obj_search_20v {} {
    bind_to_dsa
    run_cmd_and_log "set gdscp_t61 TRUE"
    log_message "Initializing ..."

    run_cmd_and_log "x500obj create /c=de -attribute {{OCL=C}  \
	{DSC=ABC\\x1B\\x24\\x29\\x42\\xB0\\xA1\\xF4\\xA4\\x1B\\x2C\\x46\\xC1\\xC2\\xC3\\x1B\\x29\\x75\\xC8A\\xC8O\\xC8UDEF}}"
#            lat[desig. seq.  kanji][1.charac. 2.charac][des.seq.greek] [alpha beta gamma][des.seq.latin][ AE  OE   UE]lat

    set test_case_name "obj_search_20v"
    set command "x500obj search /c=de -baseobject -allattr \
         -filter {{DSC=*\\x1B\\x24\\x29\\x42\\xF4\\xA4\\x1B\\x2C\\x46\\xC1\\xC2\\xC3\\x1B\\x29\\x75\\xC8A\\xC8O\\xC8UDEF}}"   

    set exp_result "/C=de"
    run_testcase_and_extract $test_case_name $command $exp_result 1 1
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de"
    run_cmd_and_log "set gdscp_t61 FALSE"
}

proc obj_search_21v {} {
    bind_to_dsa
    run_cmd_and_log "set gdscp_t61 TRUE"
    log_message "Initializing ..."

    run_cmd_and_log "x500obj create /c=de -attribute {{OCL=C}  \
	{DSC=ABC\\x1B\\x24\\x29\\x42\\xB0\\xA1\\xF4\\xA4\\x1B\\x2C\\x46\\xC1\\xC2\\xC3\\x1B\\x29\\x75\\xC8A\\xC8O\\xC8UDEF}}"
#            lat[desig. seq.  kanji][1.charac. 2.charac][des.seq.greek] [alpha beta gamma][des.seq.latin][ AE  OE   UE]lat

    set test_case_name "obj_search_21v"
    set command "x500obj search / -onelevel -allattr \
         -filter {{DSC=ABC\\x1B\\x24\\x29\\x42\\xB0\\xA1\\xF4\\xA4\\x1B\\x2C\\x46\\xC1\\xC2\\xC3\\x1B\\x29\\x75\\xC8A\\xC8O\\xC8UDEF}}"

    set exp_result "/C=de"
    run_testcase_and_extract $test_case_name $command $exp_result 1 1
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de"
    run_cmd_and_log "set gdscp_t61 FALSE"
}
