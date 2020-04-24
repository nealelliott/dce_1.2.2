# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: dsa12_objects.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:34:17  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:39:42  marty]
#
# Revision 1.1.4.1  1995/12/11  15:24:28  root
# 	Submit
# 	[1995/12/11  14:35:40  root]
# 
# Revision 1.1.2.1  1994/07/06  15:10:15  marrek
# 	Created in July 1994 code drop.
# 	[1994/07/06  13:02:17  marrek]
# 
# $EndLog$
#
# $RCSfile: dsa12_objects.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:34:17 $
#
#######################################################################
#    FILE : dsa_objects2.tcl                                          #
#    TYPE : gdscp script                                              #
#    DESCRIPTION : creates the objects in dsa2. Creates DSA name tree #
# 				   and the objects necessary for sec tests.			  #
#######################################################################

# Procedure to execute the gdscp commands
# Input argument is the command.
proc eval_cmd command {

    # Declare the global variables
    global errorCode gdscp_cwo gdscp_t61

    # Display the command on stdout
    puts stdout $command

    # Evaluate the command and trap the error return and error string
    set status [catch {eval $command} result]

    # If an error has occurred
    if {$status == 1} {
	puts -nonewline stdout "Error: "
    }

    # If result string is present, display it.
    if {$result != ""} {
	puts stdout $result
    }

    # If an error has occurred and errorCode variable has been set, display it.
    if {$status == 1 && $errorCode != "NONE"} {
	puts stdout "errorCode = $errorCode"
	exit
    }
}


# perform an anomymous bind  to dsa2
eval_cmd "bind -dirid 12 -dsa /c=re/o=sni/ou=buba/cn=dsa/cn=dsa2"

# set PREF_ADM_FUNC service control.
eval_cmd "x500svc modify -preferadmfunctions TRUE"

# add  /c=re/O=sni/ou=ap2
eval_cmd "create /c=re/O=sni/ou=ap2 -attribute OCL=OU"

# add  /c=re/O=sni/ou=ap2/cn=two    
eval_cmd "create /c=re/O=sni/ou=ap2/cn=two -attribute SN=two TN=222 UP=secret2 OCL=ORP ACL={MPUB={INT=1,USR={/c=re/o=sni}},RSTD={INT=1,USR={/}},MSTD={INT=1,USR={/}},RSEN={INT=1,USR={/c=re/o=sni}},MSEN={INT=1,USR={/c=re/o=sni}}}" 

# add  /c=re/O=sni/ou=ap2/cn=three    
eval_cmd "create /c=re/O=sni/ou=ap2/cn=three -attribute SN=three TN=333 UP=secret3 OCL=ORP ACL={MPUB={INT=1,USR={/c=re/o=sni}},RSTD={INT=1,USR={/}},MSTD={INT=1,USR={/}},RSEN={INT=1,USR={/c=re/o=sni}},MSEN={INT=1,USR={/c=re/o=sni}}}" 

# add  /c=re/O=sni/ou=buba/cn=dsa/cn=dsa2    
eval_cmd "create /c=re/O=sni/ou=buba/cn=dsa/cn=dsa2 -attribute AM=5 PN=/.../c=ie/o=digital/gds-dsa2 OCL=DSA PSA={TS=Server,NA='TCP/IP!internet=127.0.0.1+port=21121'}"

# add 2nd value for the AM recurring attribute
eval_cmd "modify /c=re/O=sni/ou=buba/cn=dsa/cn=dsa2 -addattr AM=2" 

