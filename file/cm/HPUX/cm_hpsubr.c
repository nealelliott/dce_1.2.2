/*
 * Copyright (C) 1994 Transarc Corporation
 * All rights reserved.
 */

/*
 *			Subroutines for HP/UX integration
 */

#include <dcedfs/param.h>		/* Should be always first */
#include "cm.h"				/* Cm-based standard headers */

/*
 * Discard VM pages when a file is truncated.
 * Called with no locks held.
 */
void
cm_VMTruncate(scp, length)
    struct cm_scache *scp;
    long length;
{
    if (length < scp->m.Length && (scp->v.v_flag & VMMF)) {
	mtrunc (SCTOV(scp), length);
    }
}

