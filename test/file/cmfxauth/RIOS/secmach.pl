# Copyright (C) 1996 Transarc Corporation - All rights reserved.

# Test Suite For DCE DFS Security Enhancements - Platform-dependent Functions
#
#   This file implements functions that are likely to require porting
#   to each platform.

# ----------------------------------------------------------------------------

# ResetPAG() - reset PAG for process so that it will be considered
#     unauthenticated by DFS.
#	dfscore_ent - afs_syscall() entry point for DFS
#	afscall_resetpag - doing afs_syscall(AFSCALL_RESETPAG,...)
#
#     Returns: 1 - successful; 0 - failed
#
sub ResetPAG {
#
# syscall() not exported from AIX kernel, so AIX does not support
# man 2 syscall, so perl does not support syscall()
#
# AIX should be taken out and shot
#
#    local($dfscore_ent, $afscall_resetpag, $goterror);
#
#    $dfscore_ent      = 31;
#    $afscall_resetpag = 20;
#
#    $goterror = syscall($dfscore_ent, $afscall_resetpag, 0, 0, 0, 0, 0);
#
#    !($goterror);

	return 1;
}


1; # needed for perl require()
