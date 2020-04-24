# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: structured_v.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:35:27  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:40:44  marty]
#
# Revision 1.1.4.1  1995/12/11  15:26:15  root
# 	Submit
# 	[1995/12/11  14:36:20  root]
# 
# Revision 1.1.2.3  1994/07/06  15:54:07  marrek
# 	Add version string in July 1994 code drop.
# 	[1994/07/06  15:42:26  marrek]
# 
# Revision 1.1.2.2  1994/06/10  20:21:32  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:25  devsrc]
# 
# Revision 1.1.2.1  1994/05/10  14:33:41  marrek
# 	Updated from old test subdir.
# 	[1994/05/10  09:35:44  marrek]
# 
# 	Created in April 1994 submission.
# 	[1994/05/03  12:14:53  marrek]
# 
# $EndLog$
#
# $RCSfile: structured_v.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:35:27 $
#

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : structured_v.tcl                              	  #
# Description : This file contains all the valid test cases       #
#		which result in a structured result output.       #
#		These include the test cases for the "-pretty"    #
#		switch and test cases for the "help" operation.   #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

# source in the dsa configuration file. If not present try in current directory
if {[catch {source ../../../test_config.tcl}] != 0} {
    source test_config.tcl
}

# source in the utilities file. If not present try in current directory
if {[catch {source ../../../test_utils.tcl}] != 0} {
    source test_utils.tcl
}

puts stdout "TEST CASES FOR STRUCTURED OUTPUT"
puts stdout "--------------------------------"
puts stdout "Initializing ..."

set status [catch "x500obj bind -dirid $directory_id -dsa $dsa_name" result]
if {$status != 0} {
    puts stdout "x500obj bind -dirid $directory_id -dsa $dsa_name"
    puts stdout "ERROR: $result"
}

run_test_case_and_display "x500obj create /c=de -attribute ocl=c"
run_test_case_and_display "x500obj create /c=de/o=sni -attribute ocl=org"
run_test_case_and_display "x500obj create /c=de/o=sni/ou=ap11 -attribute ocl=ou"
run_test_case_and_display "x500obj create /c=de/o=sni/ou=ap11/cn=naik -attribute ocl=orp sn=naik \"tn=12345;369072;576268\" up=naik"
run_test_case_and_display "x500obj create /c=de/o=sni/ou=ap11/cn=naik,ou=ap11 -attribute ocl=orp sn=naik tn=99999"
run_test_case_and_display "x500obj create /c=de/o=sni/ou=ap11/cn=muller -attribute {ocl=orp sn=John {dsc=Software Professional}}"
run_test_case_and_display "x500obj create /c=de/o=sni/ou=ap11/cn=miller -attribute ocl=orp sn=Peter dsc=Engineer"
run_test_case_and_display "x500obj create /c=de/o=sni/ou=ap11/cn=mueller -attribute ocl=orp sn=Henry dsc=Engineer"
run_test_case_and_display "x500obj create /c=de/o=sni/ou=ap11/cn=meller -attribute ocl=orp sn=John"
run_test_case_and_display "x500obj create /c=de/o=sni/ou=ap11/cn=mail1 -attribute ocl=mdl {{mds={pt=0, ind={dn={/c=de/o=sni}, admd=admd, cn='common name', c=de,dt1=dt1,dt2=dt2,dt3=dt3,dt4=dt4,dv1=dv1,dv2=dv2,dv3=dv3,dv4=dv4,gen=gen,gn=given,ini=init,o=orgname,ou1=ou1,ou2=ou2,ou3=ou3,ou4=ou4,prmd=prmd,sn=surname}}}} moa={admd=admd,c=de,prmd=prmd}" 
run_test_case_and_display "x500obj create /c=de/o=sni/ou=ap11/cn=mail2 -attribute ocl=mdl {{mds={pt=0, ind={dn={/c=de/o=sni/ou=ap11/cn=ind1}, admd=admd, c=de,o=orgname,prmd=prmd}};{pt=0,ind={dn={/c=de/o=sni/ou=ap11/cn=Submit},admd=admd,c=de,o=orgname,prmd=prmd}}}} \"moa={admd=admd,c=de,o=orgname,ou1=ou1,ou2=ou2,prmd=prmd}\" \"mdm={dn={/c=de/o=sni/ou=ap11},admd=admd,c=de,prmd=prmd}\""
run_test_case_and_display "x500obj create /c=de/o=sni/ou=ap11/cn=Brown -attribute ocl=mdl {{dsc=Software Consultant}} {{mds={pt=0, ind={dn={/c=de/o=sni}, c=de, admd=dbp, prmd=sni, o=Siemens Nixdorf, ou1=Munich, ou2=P1, ou3=P4, ou4=ap113, cn=Alfred Brown, gn=Alfred, ini=P., sn=Brown, gen=396, dt1=MS MAIL, dv1=Brown}}}} moa={admd=admd,c=de,prmd=prmd}" 
run_test_case_and_display "x500obj create /c=de/o=sni/ou=ap11/cn=Andrews -attribute ocl=mdl {{dsc=Project Leader}} {{mds={pt=0, ind={dn={/c=de/o=sni}, c=de, admd=dbp, prmd=sni, o=Siemens Nixdorf, ou1=Munich, ou2=P1, ou3=P4, ou4=ap113, cn=Peter Andrews, gn=Peter, ini=P., sn=Andrews, gen=396, dt1=MS MAIL, dv1=Peter}}}} moa={admd=admd,c=de,prmd=prmd}" 
run_test_case_and_display "x500obj create /c=de/o=sni/ou=ap11/cn=dsa -attribute ocl=app"
run_test_case_and_display "x500obj create /c=de/o=sni/ou=ap11/cn=dsa/cn=dsa15 -attribute ocl=ape \"PSA={NA='TCP/IP!internet=127.0.0.1+port=25015',TS=Server}\""


puts stdout "\nVALID TEST CASES FOR x500obj LIST"
puts stdout "---------------------------------"
run_test_case_and_display "x500obj list / -pretty"
run_test_case_and_display "set gdscp_cwo /c=de/o=sni/ou=ap11"
run_test_case_and_display "x500obj l -pretty"
run_test_case_and_display "set gdscp_cwo /c=de/o=sni"
run_test_case_and_display "x500obj li ou=ap11 -pretty"
run_test_case_and_display "list /c=de -pretty"
run_test_case_and_display "x500obj list /c=de/o=sni -p"
run_test_case_and_display "list /c=de/o=sni/ou=ap11 -pre"
run_test_case_and_display "set gdscp_cwo /"


puts stdout "\nVALID TEST CASES FOR x500obj SEARCH"
puts stdout "-----------------------------------"
run_test_case_and_display "x500obj search / -filter cn=naik -subtree -p"
run_test_case_and_display "x500obj search /c=de/o=sni/ou=ap11/cn=mueller -b -pret"
run_test_case_and_display "x500obj search /c=de/o=sni/ou=ap11 -f {{cn=mue* || cn=mi*}} -subtree -types -pretty"
run_test_case_and_display "set gdscp_cwo /c=de"
run_test_case_and_display "x500obj se -subtree -pretty"
run_test_case_and_display "x500obj sear -filter cn=n* -subtree -pretty"
run_test_case_and_display "x500obj search /c=de/o=sni/ou=ap11/cn=mueller -n -t -subtree -pretty"
run_test_case_and_display "x500obj search /c=de/o=sni/ou=ap11/cn=mueller -at ocl -subtree -pretty"
run_test_case_and_display "x500obj search /c=de/o=sni/ou=ap11/cn=mueller -allattr -subtree -pretty"
run_test_case_and_display "x500obj search /c=de/o=sni/ou=ap11/cn=mueller -attribute sn dsc -subtree -pretty"
run_test_case_and_display "x500obj search /c=de/o=sni/ou=ap11/cn=mueller -attribute {sn dsc} -subtree -pretty"
run_test_case_and_display "search /c=de/o=sni/ou=ap11 -filter {{cn=m*  && dsc=*}} -subtree -pretty"
run_test_case_and_display "search /c=de/o=sni/ou=ap11 -allattr -subtree -pretty"
run_test_case_and_display "search /c=de/o=sni -allattr -o -pretty"
run_test_case_and_display "search /c=de/o=sni -allattr -baseobject -pretty"
run_test_case_and_display "search /c=de/o=sni -allattr -s -pretty"
run_test_case_and_display "search -filter {{(cn~=mueller) && ((OCL=ORP) || OCL=REP) && !(sn=henry) && (dsc=*) }} -subtree -pretty"
run_test_case_and_display "search /c=de/o=sni/ou=ap11 -filter {{cn=dsa || cn=mail1 || (cn=naik && tn=12345)}} -allattr -onelevel -pretty"
run_test_case_and_display "set gdscp_cwo /"


puts stdout "\nVALID TEST CASES FOR x500obj SHOW"
puts stdout "---------------------------------"
run_test_case_and_display "set gdscp_cwo /c=de/o=sni/ou=ap11/cn=naik"
run_test_case_and_display "x500obj show -attribute cn -pretty"
run_test_case_and_display "x500obj show -attribute {cn tn} -pretty"
run_test_case_and_display "x500obj show -p"
run_test_case_and_display "x500obj sh /c=de/o=sni/ou=ap11/cn=naik -pretty"
run_test_case_and_display "x500obj sho /c=de/o=sni/ou=ap11/cn=naik -types -pretty"
run_test_case_and_display "show /c=de/o=sni/ou=ap11/cn=naik -attr tn sn ocl up -pret"
run_test_case_and_display "show /c=de/o=sni/ou=ap11/cn=mail1 -pretty"
run_test_case_and_display "show /c=de/o=sni/ou=ap11/cn=mail2 -pretty"
run_test_case_and_display "set gdscp_cwo /"


puts stdout "\nVALID TEST CASES FOR PARTIAL RESULTS DUE TO SIZE LIMIT"
puts stdout "------------------------------------------------------"
run_test_case_and_display "x500svc modify -sizelimit 1"
run_test_case_and_display "search / -subtree -pretty"
run_test_case_and_display "list /c=de/o=sni/ou=ap11 -pretty"
run_test_case_and_display "x500svc modify -default"


puts stdout "\nVALID TEST CASES FOR x500svc MODIFY & SHOW"
puts stdout "------------------------------------------"
run_test_case_and_display "x500svc modify -default"
run_test_case_and_display "x500svc show -pretty"
run_test_case_and_display "x500svc modify -timelimit INFINITE"
run_test_case_and_display "x500svc show -pretty"
run_test_case_and_display "x500svc modify -sizelimit INFINITE"
run_test_case_and_display "x500svc show -pretty"
run_test_case_and_display "x500svc m -cacheclass NONE"
run_test_case_and_display "x500svc s -pretty"
run_test_case_and_display "x500svc mod -scopeofreferral UNLIMITED"
run_test_case_and_display "x500svc sh -pretty"
run_test_case_and_display "x500svc modify -timelimit 100"
run_test_case_and_display "x500svc show -pretty"
run_test_case_and_display "x500svc modify -timelimit INFINITE"
run_test_case_and_display "x500svc show -pretty"
run_test_case_and_display "x500svc modify -sizelimit 100"
run_test_case_and_display "x500svc show -pretty"
run_test_case_and_display "x500svc modify -sizelimit INFINITE"
run_test_case_and_display "x500svc show -pretty"
run_test_case_and_display "x500svc modify -cacheclass PRIVILEGE"
run_test_case_and_display "x500svc show -pretty"
run_test_case_and_display "x500svc modify -ca NONE"
run_test_case_and_display "x500svc show -pretty"
run_test_case_and_display "x500svc modify -scopeofreferral COUNTRY"
run_test_case_and_display "x500svc show -pretty"
run_test_case_and_display "x500svc modify -scopeofreferral {UNLIMITED}"
run_test_case_and_display "x500svc show -pretty"
run_test_case_and_display "x500svc modify -automaticcontinuation FALSE -cacheclass PRIVILEGE -chainingprohibited FALSE -dontdereferencealias TRUE -dontusecopy FALSE -duacache TRUE -duafirst TRUE -localscope TRUE -preferadmfunctions TRUE -preferchain TRUE -priority LOW -scopeofreferral DMD -timelimit 100 -sizelimit 500 -usedsa FALSE"
run_test_case_and_display "x500svc show -pretty"
run_test_case_and_display "x500svc modify -a TRUE -cacheclass NORMAL -ch TRUE -dontderef FALSE -dontuse TRUE -duac FALSE -duaf FALSE -l FALSE -preferadm FALSE -preferch FALSE -prio MEDIUM -sc UNLIMITED -t INFINITE -siz INFINITE -u TRUE"
run_test_case_and_display "x500svc show -p"
run_test_case_and_display "x500svc modify -def"
run_test_case_and_display "x500svc show -pret"
run_test_case_and_display "x500svc modify -cacheclass RESIDENT -priority HIGH"
run_test_case_and_display "x500svc show -pretty"
run_test_case_and_display "x500svc modify -default"
run_test_case_and_display "x500svc show -pretty"


puts stdout "\nVALID TEST CASES FOR x500abbr SHOW"
puts stdout "----------------------------------"
run_test_case_and_display "x500abbr show -p"


puts stdout "\nVALID TEST CASES FOR x500obj HELP"
puts stdout "---------------------------------"
run_test_case_and_display "x500obj help"
run_test_case_and_display "x500obj help -verbose"
run_test_case_and_display "x500obj help -v"
run_test_case_and_display "x500obj h bind"
run_test_case_and_display "x500obj hel compare"
run_test_case_and_display "help create"
run_test_case_and_display "help delete"
run_test_case_and_display "help help"
run_test_case_and_display "help list"
run_test_case_and_display "help operations"
run_test_case_and_display "help modify"
run_test_case_and_display "help search"
run_test_case_and_display "help show"


puts stdout "\nVALID TEST CASES FOR x500svc HELP"
puts stdout "---------------------------------"
run_test_case_and_display "x500svc help"
run_test_case_and_display "x500svc help -verbose"
run_test_case_and_display "x500svc h -v"
run_test_case_and_display "x500svc help help"
run_test_case_and_display "x500svc help modify"
run_test_case_and_display "x500svc help operations"
run_test_case_and_display "x500svc help show"


puts stdout "\nVALID TEST CASES FOR x500abbr HELP"
puts stdout "----------------------------------"
run_test_case_and_display "x500abbr help"
run_test_case_and_display "x500abbr help -verbose"
run_test_case_and_display "x500abbr h -v"
run_test_case_and_display "x500abbr help help"
run_test_case_and_display "x500abbr help operations"
run_test_case_and_display "x500abbr help show"


puts stdout "Cleaning up ..."
run_test_case_and_display "x500obj delete /c=de/o=sni/ou=ap11/cn=dsa/cn=dsa15"
run_test_case_and_display "x500obj delete /c=de/o=sni/ou=ap11/cn=dsa"
run_test_case_and_display "x500obj delete /c=de/o=sni/ou=ap11/cn=naik"
run_test_case_and_display "x500obj delete /c=de/o=sni/ou=ap11/cn=naik,ou=ap11"
run_test_case_and_display "x500obj delete /c=de/o=sni/ou=ap11/cn=muller"
run_test_case_and_display "x500obj delete /c=de/o=sni/ou=ap11/cn=miller"
run_test_case_and_display "x500obj delete /c=de/o=sni/ou=ap11/cn=mueller"
run_test_case_and_display "x500obj delete /c=de/o=sni/ou=ap11/cn=meller"
run_test_case_and_display "x500obj delete /c=de/o=sni/ou=ap11/cn=mail1"
run_test_case_and_display "x500obj delete /c=de/o=sni/ou=ap11/cn=mail2"
run_test_case_and_display "x500obj delete /c=de/o=sni/ou=ap11/cn=Brown"
run_test_case_and_display "x500obj delete /c=de/o=sni/ou=ap11/cn=Andrews"
run_test_case_and_display "x500obj delete /c=de/o=sni/ou=ap11"
run_test_case_and_display "x500obj delete /c=de/o=sni"
run_test_case_and_display "x500obj delete /c=de"
