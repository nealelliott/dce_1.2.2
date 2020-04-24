/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1996, 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_pioctl.h -- 
 *    Declarations for functions in itl_pioctl.c.
 */

#ifndef ITL_PIOCTL_H
#define ITL_PIOCTL_H


#include <assert.h>
#include <sys/time.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/socket.h>
#include <sys/errno.h>
#include <dce/aclbase.h>
#include <dcedfs/osi.h>
#include <dcedfs/ioctl.h>
#include <dcedfs/osi_dfserrors.h>
#include <dcedfs/common_data.h>

#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_vectype.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_stdtypes.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_commonHyper.h>
#include <utils/itl/itl_commonTimeval.h>
#include <utils/itl/itl_commonFid.h>
#include <utils/itl/itl_commonSockaddr.h>

/*
 *  Define all the useful PIOCTL commands.
 */

#define ITL_PIOCTL_CMD                                             \
 {"pioctl", itlPIOCTL_Pioctl, FALSE, FALSE, COMMAND_MODE_ALL,           \
      "pioctl interface call",                                              \
      "pioctl",                                                              \
      ""}

/*
 *  ITL_PIOCTL_CHECK_ERROR() --
 *    Macro for checking return codes from PIOCTL calls.
 */
#define ITL_PIOCTL_CHECK_ERROR(_errCode)                                    \
        MACRO_BEGIN                                                         \
          if ((_errCode) != 0) {                                            \
            int __saved_err = errno;                                        \
            if (! itlu_CheckResult(errno)) {                                \
              itl_Error("PIOCTL call failure: errno is %d\n", __saved_err); \
            }                                                               \
	  }                                                                 \
        MACRO_END

#define PIOCTL_INIT_AFS_IOCTL(_ioctlDataP)  \
 MACRO_BEGIN                                                         \
  ((_ioctlDataP)->in) = ((_ioctlDataP)->out) = (char *)NULL;     \
  ((_ioctlDataP)->in_size) = ((_ioctlDataP)->out_size) = 0 ;      \
 MACRO_END

/*
 * External data types.
 */

extern itl_commandTable_t itlPIOCTL_CommandTable[];

/*
 *  Exported Functions
 */

extern void  itlPIOCTL_AddPIOCTLCommands _TAKES((
					void
					));

extern boolean_t  itlPIOCTL_Pioctl _TAKES((
					void
					));

extern char * hostutil_GetNameByINet _TAKES((
                                             struct in_addr       addr
                                           ));


#endif   /* ITL_PIOCTL_H */


