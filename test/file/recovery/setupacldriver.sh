
#
# @OSF_COPYRIGHT@
#

#
# HISTORY
# $Log: setupacldriver.sh,v $
# Revision 1.1.2.2  1993/01/28  21:36:02  shl
# 	Transarc delta: rajesh-ot6318-acl-recovery-test 1.7
# 	  Selected comments:
# 	    Test recovery for correct restoration of acls.
# 	    Another checkpoint since I am upgrading to dfs-102-2.2.
# 	    Needed to import another delta.
# 	    Since the acl tests should not be run as root, but checkaggr needs
# 	    to be run as root, modifications were needed. The main program acldriver
# 	    is to be invoked as root, with the name of the local/DCE user to run the
# 	    ACL tests as specified via an environment variable. The user's DCE passwd
# 	    is specified likewise. The remaining programs are invoked as necessary.
# 	    A example helper script that sets the most often used enviromnent variables
# 	    for the acl recovery test. A similiar script should be executed before
# 	    running acldriver.
# 	    Add support for running multiple copies of ACL tests on different
# 	    filesets in the same aggregate simultaneously.
# 	    Debug support code added in previous revision of this delta.
# 	    Modify to reflect new log dir for logs and bad aggrs and
# 	    the new acl execution directory.
# 	    Miscellaneous changes to increase robustness and quality.
# 	    Changes in values of environment variables to suit the new aggregate
# 	    devices I used. This file exists only to provide an example setting
# 	    of the env. vars.
# 	[1993/01/27  23:21:38  shl]
#
# $EndLog$
#

setenv SRCDEV /dev/epi-acl1
setenv SCRATCHDEV1 /dev/epi-acl2
setenv SCRATCHDEV2 /dev/epi-acl3
setenv DRIVERDIR /opt/dcelocal/bin
setenv ACLTESTDIR /usr/acl
setenv TESTER rajesh
setenv TESTER_DCEPASSWD rajesh
setenv DEBUG 1
setenv BADAGGRDIR ~rajesh/epi/test/recovery/acl/log
setenv NUMTIMES 1
setenv NUMSETS 1



