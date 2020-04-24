#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: dns_status_map.awk,v $
# Revision 1.1.4.2  1996/03/09  23:11:57  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:15  marty]
#
# Revision 1.1.4.1  1995/12/08  15:15:50  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:43:00  root]
# 
# Revision 1.1.2.2  1994/06/09  18:38:58  devsrc
# 	cr10871 - expand copyright
# 	[1994/06/09  18:11:37  devsrc]
# 
# Revision 1.1.2.1  1994/03/12  22:02:53  peckham
# 	DEC serviceability and i18n drop
# 	[1994/03/12  19:31:41  peckham]
# 
# $EndLog$
#
# COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1992-1994. ALL RIGHTS RESERVED.
#
# THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
# ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
# INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
# COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
# OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
# TRANSFERRED.
#
# THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
# AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
# CORPORATION.
#
# DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
# SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
#
# Module: dns_status_map.awk
#
#
# Function:
#
#  Produce an include file from dnsmessage.h, defining an array containing
#  the status codes and corresponding ascii strings.  Used to translate
#  status values to printable strings by deb_event_mgr.c.
#
# Command:
#
#  awk -f dns_status_map.awk dnsmessage.h > dns_status_map.c
#
# The DNS_UNIXERRORS extension set is ultrix specific, but is built
# to take any values out that are not part of the target system.
#
BEGIN	{ print "/* used by deb_event_mgr.c */"
	  print ""
	  print "#ifndef DNS_SUCCESS"
	  print "# include <dce/dnsmessage.h>"
	  print "#endif"
	  print ""
	  print "#ifndef _DNS_STATUS_T_"
	  print "# define _DNS_STATUS_T_"
	  print "typedef unsigned int\tdns_status_t;"
	  print "#endif"
	  print ""
	  print "#ifdef unix"
	  print "# include <errno.h>"
	  print "#endif"
	  print ""
	  print "typedef struct {"
	  print "  dns_status_t value;"
	  print "  char *name_p;"
	  print "} dns_status_map_t;"
	  print ""
	  print "static dns_status_map_t dns_status_map[] = {"
	}
/^#def.*DNS_UNIXERRORS/ { print "#ifdef unix"
	  errno[1] = "EPERM"
	  errno[2] = "ENOENT"
	  errno[3] = "ESRCH"
	  errno[4] = "EINTR"
	  errno[5] = "EIO"
	  errno[6] = "ENXIO"
	  errno[7] = "E2BIG"
	  errno[8] = "ENOEXEC"
	  errno[9] = "EBADF"
	  errno[10] = "ECHILD"
	  errno[11] = "EAGAIN"
	  errno[12] = "ENOMEM"
	  errno[13] = "EACCES"
	  errno[14] = "EFAULT"
	  errno[15] = "ENOTBLK"
	  errno[16] = "EBUSY"
	  errno[17] = "EEXIST"
	  errno[18] = "EXDEV"
	  errno[19] = "ENODEV"
	  errno[20] = "ENOTDIR"
	  errno[21] = "EISDIR"
	  errno[22] = "EINVAL"
	  errno[23] = "ENFILE"
	  errno[24] = "EMFILE"
	  errno[25] = "ENOTTY"
	  errno[26] = "ETXTBSY"
	  errno[27] = "EFBIG"
	  errno[28] = "ENOSPC"
	  errno[29] = "ESPIPE"
	  errno[30] = "EROFS"
	  errno[31] = "EMLINK"
	  errno[32] = "EPIPE"
	  errno[33] = "EDOM"
	  errno[34] = "ERANGE"
	  errno[35] = "EWOULDBLOCK"
	  errno[36] = "EINPROGRESS"
	  errno[37] = "EALREADY"
	  errno[38] = "ENOTSOCK"
	  errno[39] = "EDESTADDRREQ"
	  errno[40] = "EMSGSIZE"
	  errno[41] = "EPROTOTYPE"
	  errno[42] = "ENOPROTOOPT"
	  errno[43] = "EPROTONOSUPPORT"
	  errno[44] = "ESOCKTNOSUPPORT"
	  errno[45] = "EOPNOTSUPP"
	  errno[46] = "EPFNOSUPPORT"
	  errno[47] = "EAFNOSUPPORT"
	  errno[48] = "EADDRINUSE"
	  errno[49] = "EADDRNOTAVAIL"
	  errno[50] = "ENETDOWN"
	  errno[51] = "ENETUNREACH"
	  errno[52] = "ENETRESET"
	  errno[53] = "ECONNABORTED"
	  errno[54] = "ECONNRESET"
	  errno[55] = "ENOBUFS"
	  errno[56] = "EISCONN"
	  errno[57] = "ENOTCONN"
	  errno[58] = "ESHUTDOWN"
	  errno[59] = "ETOOMANYREFS"
	  errno[60] = "ETIMEDOUT"
	  errno[61] = "ECONNREFUSED"
	  errno[62] = "ELOOP"
	  errno[63] = "ENAMETOOLONG"
	  errno[64] = "EHOSTDOWN"
	  errno[65] = "EHOSTUNREACH"
	  errno[66] = "ENOTEMPTY"
	  errno[67] = "EPROCLIM"
	  errno[68] = "EUSERS"
	  errno[69] = "EDQUOT"
	  errno[70] = "ESTALE"
	  errno[71] = "EREMOTE"
	  errno[72] = "ENOMSG"
	  errno[73] = "EIDRM"
	  errno[74] = "EALIGN"
	  errno[75] = "ENOLCK"
	  errno[76] = "ENOSYS"
	  errno[77] = "EACTIVE"
	  errno[78] = "ENOACTIVE"
	  errno[79] = "ENORESOURCES"
	  errno[80] = "ENOSYSTEM"
	  errno[81] = "ENODUST"
	  errno[82] = "EDUPNOCONN"
	  errno[83] = "EDUPNODISCONN"
	  errno[84] = "EDUPNOTCNTD"
	  errno[85] = "EDUPNOTIDLE"
	  errno[86] = "EDUPNOTWAIT"
	  errno[87] = "EDUPNOTRUN"
	  errno[88] = "EDUPBADOPCODE"
	  errno[89] = "EDUPINTRANSIT"
	  errno[90] = "EDUPTOOMANYCPUS"
	  errno_max = 0
	  for (num in errno)
	    if (errno_max < num)
	      errno_max = num
	  for (num = 1; num <= errno_max; num++)
	    if (errno[num] != "") {
	      print "# ifdef "errno[num]
	      print "  {DNS_UNIXERRORS+"errno[num]", \"DNS_UNIX_"errno[num]"\"},"
	      print "# endif"
	    }
	  print "#endif /* unix */"
	  next
	}
/^#def.*DNS_/ { print "  {"$2", \""$2"\"}," }
END	{ print "  {0, 0}"
	  print "};"
	}
