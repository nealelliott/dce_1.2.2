# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_bind_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:35:51  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:05  marty]
#
# Revision 1.1.4.1  1995/12/11  15:26:46  root
# 	Submit
# 	[1995/12/11  14:36:30  root]
# 
# Revision 1.1.2.5  1994/07/06  15:11:24  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:23:29  marrek]
# 
# Revision 1.1.2.4  1994/06/21  14:29:22  marrek
# 	June 1994 code submission.
# 	[1994/06/16  07:50:03  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:22:18  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:42  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:34:55  marrek
# 	Updated.
# 	[1994/05/11  10:24:41  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:09  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:15:21  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_bind_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:35:51 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_bind_i.tcl                              	  #
# Description : This file contains all the invalid test cases     #
#		for the bind operation on the x500obj object      #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_bind_01i {} {
    set test_case_name "obj_bind_01i"
    set command "x500obj bind -dirid"
    set exp_result "Missing directory identifier."
    set exp_error_code "GDSCP -503"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_02i {} {
    set test_case_name "obj_bind_02i"
    set command "x500obj bind -attribute"
    set exp_result "Unknown option \"-attribute\"."
    set exp_error_code "GDSCP -502"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_03i {} {
    set test_case_name "obj_bind_03i"
    set command "x500obj bind -cache some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_04i {} {
    set test_case_name "obj_bind_04i"
    set command "x500obj bind -dirid 0"
    set exp_result "Invalid directory identifier \"0\"."
    set exp_error_code "GDSCP -516"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_05i {} {
    set test_case_name "obj_bind_05i"
    set command "x500obj bind -dirid 1 -dirid"
    set exp_result "Conflicting option \"-dirid\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_06i {} {
    set test_case_name "obj_bind_06i"
    set command "x500obj bind -cache -dirid -dsa"
    set exp_result "Missing directory identifier."
    set exp_error_code "GDSCP -503"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_07i {} {
    set test_case_name "obj_bind_07i"
    set command "x500obj bind -dsa"
    set exp_result "Missing Directory Service Agent name."
    set exp_error_code "GDSCP -507"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_08i {} {
    set test_case_name "obj_bind_08i"
    set command "x500obj bind -cache -dsa"
    set exp_result "Conflicting option \"-dsa\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_09i {} {
    set test_case_name "obj_bind_09i"
    set command "x500obj bind -d"
    set exp_result "Ambiguous option \"-d\": -dirid -dsa."
    set exp_error_code "GDSCP -532"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_10i {} {
    set test_case_name "obj_bind_10i"
    set command "x500obj bind -p"
    set exp_result "Ambiguous option \"-p\": -password -psap."
    set exp_error_code "GDSCP -532"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_11i {} {
    set test_case_name "obj_bind_11i"
    set command "x500obj bind -dsa -dsa"
    set exp_result "Missing Directory Service Agent name."
    set exp_error_code "GDSCP -507"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_12i {} {
    set test_case_name "obj_bind_12i"
    set command "x500obj bind -ds /c=de -dsa"
    set exp_result "Conflicting option \"-dsa\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_13i {} {
    set test_case_name "obj_bind_13i"
    set command "x500obj bind -dsa /c=de -psap TS=Server,NA='TCP/IP!internet=127.0.0.1+port=25015'"
    set exp_result "Conflicting option \"-psap\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_14i {} {
    set test_case_name "obj_bind_14i"
    set command "x500obj bind -psap TS=Server,NA'TCP/IP!internet=127.0.0.1+port=25015'"
    set exp_result "Missing equal operator - \"TS=Server,NA'TCP/IP!internet=127.0.0.1+port=25015'\" : Error position <13>."
    set exp_error_code "GDSCP -337"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_15i {} {
    set test_case_name "obj_bind_15i"
    set command "x500obj bind -password"
    set exp_result "Missing password."
    set exp_error_code "GDSCP -510"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_16i {} {
    set test_case_name "obj_bind_16i"
    set command "x500obj bind -cache -password"
    set exp_result "Conflicting option \"-password\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_17i {} {
    set test_case_name "obj_bind_17i"
    set command "bind -password -dsa"
    set exp_result "Missing password."
    set exp_error_code "GDSCP -510"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_18i {} {
    set test_case_name "obj_bind_18i"
    set command "bind -password naik -password"
    set exp_result "Conflicting option \"-password\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_19i {} {
    set test_case_name "obj_bind_19i"
    set command "bind -user"
    set exp_result "Missing user name."
    set exp_error_code "GDSCP -509"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_20i {} {
    set test_case_name "obj_bind_20i"
    set command "bind -cache -user"
    set exp_result "Conflicting option \"-user\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_21i {} {
    set test_case_name "obj_bind_21i"
    set command "bind -authentication"
    set exp_result "Missing authentication type."
    set exp_error_code "GDSCP -534"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_22i {} {
    set test_case_name "obj_bind_22i"
    set command "bind -authentication -dirid 1"
    set exp_result "Missing authentication type."
    set exp_error_code "GDSCP -534"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_23i {} {
    set test_case_name "obj_bind_23i"
    set command "bind -user /c=de/o=sni/ou=ap11/cn=naik -dirid 1"
    set exp_result "Missing authentication type."
    set exp_error_code "GDSCP -534"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_24i {} {
    set test_case_name "obj_bind_24i"
    set command "bind -user /c=de/o=sni/ou=ap11/cn=naik -pass naik -dirid 1"
    set exp_result "Missing authentication type."
    set exp_error_code "GDSCP -534"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_25i {} {
    set test_case_name "obj_bind_25i"
    set command "bind -auth SIMPLE -user /c=de/o=sni/ou=ap11/cn=naik"
    set exp_result "Missing password."
    set exp_error_code "GDSCP -510"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_26i {} {
    set test_case_name "obj_bind_26i"
    set command "bind -auth SIMPLE -password naik"
    set exp_result "Missing user name."
    set exp_error_code "GDSCP -509"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_27i {} {
    set test_case_name "obj_bind_27i"
    set command "bind -auth STRONG"
    set exp_result "Missing user name."
    set exp_error_code "GDSCP -509"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_28i {} {
    set test_case_name "obj_bind_28i"
    set command "bind -authentication some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_29i {} {
    set test_case_name "obj_bind_29i"
    set command "bind -dirid 1 -cache -auth SIMPLE"
    set exp_result "Conflicting option \"-auth\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_30i {} {
    set test_case_name "obj_bind_30i"
    set command "bind -authentication SIMPLE -authentication STRONG"
    set exp_result "Conflicting option \"-authentication\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_31i {} {
    set test_case_name "obj_bind_31i"
    set command "bind -user -user"
    set exp_result "Missing user name."
    set exp_error_code "GDSCP -509"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_32i {} {
    set test_case_name "obj_bind_32i"
    set command "bind -user /c=de/o=sni/ou=ap11/cn=naik -user"
    set exp_result "Conflicting option \"-user\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_33i {} {
    set test_case_name "obj_bind_33i"
    set command "bind -authentication STRONG -user \"  \""
    set exp_result "Missing user name."
    set exp_error_code "GDSCP -509"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_34i {} {
    set test_case_name "obj_bind_34i"
    set command "bind -psap"
    set exp_result "Missing Presentation Service Access Point address."
    set exp_error_code "GDSCP -508"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_35i {} {
    set test_case_name "obj_bind_35i"
    set command "bind -cache -psap"
    set exp_result "Conflicting option \"-psap\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_36i {} {
    set test_case_name "obj_bind_36i"
    set command "bind -dir -ps"
    set exp_result "Missing directory identifier."
    set exp_error_code "GDSCP -503"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_37i {} {
    set test_case_name "obj_bind_37i"
    set command "bind -psap -psap"
    set exp_result "Missing Presentation Service Access Point address."
    set exp_error_code "GDSCP -508"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_38i {} {
    set test_case_name "obj_bind_38i"
    set command "bind -dirid 15 -dsa \"\{/c=de/o=sni\""
    set exp_result "unmatched open brace in list"
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_39i {} {
    set test_case_name "obj_bind_39i"
    set command "bind -dirid 15 -dsa {{/c=de/o=sni} {-psap}}"
    set exp_result "Invalid argument \"{/c=de/o=sni} {-psap}\"."
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_40i {} {
    set test_case_name "obj_bind_40i"
    set command "bind -dsa /c=de/o="
    set exp_result "Missing attribute value - \"/c=de/o=\" : Error position <9>."
    set exp_error_code "GDSCP -338"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_41i {} {
    set test_case_name "obj_bind_41i"
    set command "bind -dsa \"   \""
    set exp_result "Missing Directory Service Agent name."
    set exp_error_code "GDSCP -507"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_42i {} {
    set test_case_name "obj_bind_42i"
    set command "bind -authentication STRONG -user /c=de/o=sni/ou=ap11/cn=naik,"
    set exp_result "Missing Attribute Value Assertion - \"/c=de/o=sni/ou=ap11/cn=naik,\" : Error position <29>."
    set exp_error_code "GDSCP -331"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_43i {} {
    set test_case_name "obj_bind_43i"
    set command "bind -psap TS=Server,NA='TCP/IP!internet=127.0.0.1+port=25015' -psap"
    set exp_result "Conflicting option \"-psap\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_44i {} {
    set test_case_name "obj_bind_44i"
    set command "bind -psap TS=Server,NA='TCP/IP!internet=127.0.0.1+port=25015' -dsa /c=de"
    set exp_result "Conflicting option \"-dsa\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_45i {} {
    set test_case_name "obj_bind_45i"
    set command "bind -ps TS=Server,NA='TCP/IP!internet=127.0.0.1+port=25015' -ds /c=de"
    set exp_result "Conflicting option \"-ds\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_46i {} {
    set test_case_name "obj_bind_46i"
    set command "bind -psap \"    \""
    set exp_result "Missing Presentation Service Access Point address."
    set exp_error_code "GDSCP -508"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_47i {} {
    set test_case_name "obj_bind_47i"
    set command "bind -dsa /c=de/o=sni/ou=ap11/cn=dsa/cn=dsa15 -dirid 15 -authentication SIMPLE -user /c=de/o=sni/ou=ap11/cn=naik -password \"       \""
    set exp_result "Missing password."
    set exp_error_code "GDSCP -510"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_48i {} {
    set test_case_name "obj_bind_48i"
    set command "bind -dsa /c=de/o=sni/ou=ap11/cn=dsa/cn=dsa15 -dirid 15 -authentication SIMPLE -user /c=de/o=sni/ou=ap11/cn=naik -password 'nai"
    set exp_result "Mismatched quotes - \"'nai\" : Error position <5>."
    set exp_error_code "GDSCP -524"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_49i {} {
    set test_case_name "obj_bind_49i"
    set command "bind -dsa /c=de/o=sni/ou=ap11/cn=dsa/cn=dsa15 -dirid 15 -authentication SIMPLE -user /c=de/o=sni/ou=ap11/cn=naik -password 'nai'k'"
    set exp_result "Format error - \"'nai'k'\" : Error position <6>."
    set exp_error_code "GDSCP -523"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_50i {} {
    global dsa_name directory_id;

    set test_case_name "obj_bind_50i"
    set command "bind -dsa $dsa_name -dirid $directory_id -authentication SIMPLE -user /c=de/o=sni/ou=ap11/cn=naik -password {{  hello there  }}"
    set exp_result "The requester's credentials are invalid."
    set exp_error_code "GDSCP -24"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_51i {} {
    set test_case_name "obj_bind_51i"
    set command "bind -dsa /c=de/o=sni/ou=ap11/cn=dsa/cn=dsa15 -dirid 15 -authentication SIMPLE -user /c=de/o=sni/ou=ap11/cn=naik -password {{nai\\x}}"
    set exp_result "Format error - \"nai\\x\" : Error position <6>."
    set exp_error_code "GDSCP -523"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_52i {} {
    set test_case_name "obj_bind_52i"
    set command "bind -dsa /c=de/o=sni/ou=ap11/cn=dsa/cn=dsa15 -dirid 15 -authentication SIMPLE -user /c=de/o=sni/ou=ap11/cn=naik -password {{nai\\xa}}"
    set exp_result "Format error - \"nai\\xa\" : Error position <6>."
    set exp_error_code "GDSCP -523"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_53i {} {
    set test_case_name "obj_bind_53i"
    set command "bind -dirid 15"
    set exp_result "Missing Directory Service Agent name."
    set exp_error_code "GDSCP -507"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_54i {} {
    set test_case_name "obj_bind_54i"
    set command "bind -dirid 15 -authentication SIMPLE -user /c=de/o=sni/ou=ap11/cn=naik -password naik"
    set exp_result "Missing Directory Service Agent name."
    set exp_error_code "GDSCP -507"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_55i {} {
    set test_case_name "obj_bind_55i"
    set command "bind -authentication STRONG -user /c=de/o=sni/ou=ap11/cn=naik"
    set exp_result "Missing Directory Service Agent name."
    set exp_error_code "GDSCP -507"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_56i {} {
    set test_case_name "obj_bind_56i"
    set command "x500obj bind -dsa /c=de/o=sni/ou=ap11/cn=dsa/cn=müller, -dirid 15"
    set exp_result "Missing Attribute Value Assertion - \"/c=de/o=sni/ou=ap11/cn=dsa/cn=müller,\" : Error position <38>."
    set exp_error_code "GDSCP -331"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_57i {} {
    global dsa_name directory_id;

    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_bind_57i"
    set command "bind -dsa $dsa_name -dirid $directory_id -user /c=de/o=sni/ou=ap11/cn=naik -authentication STRONG"
    set exp_result "A miscellaneous error occurred while interacting with the Directory Service."
    set exp_error_code "GDSCP -28"
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_dit_objects
}
