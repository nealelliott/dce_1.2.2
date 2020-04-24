/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsnewepoch.c,v $
 * Revision 1.1.7.2  1996/02/18  19:34:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:27  marty]
 *
 * Revision 1.1.7.1  1995/12/08  15:25:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:31  root]
 * 
 * Revision 1.1.4.5  1994/08/16  20:01:28  jd
 * 	Fixed type made during last change. ifSecon.... -> if(Secon.....
 * 	[1994/08/13  18:36:05  jd]
 * 
 * 	11674 From shu@hp. cdsNewEpoch needs to check SecondaryCH_p and ReadOnlyCH_p
 * 	for NULL.
 * 	[1994/08/13  18:23:00  jd]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:23  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:02:11  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:59  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:11  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:54:27  peckham
 * 	Remove unnecessary conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:24:53  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:07:18  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:31:14  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:58:04  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:13:24  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsnewepoch.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 */
  
#include <dce/dce.h>
#include <dce/cdsclerk.h>
#include <dcecdssvc.h>
#include <uc_parse.h>
#include <uc_clerk.h>

/* ----------------------------------------------------------------------
 *  Reconstruct replicas of a directory
 * ----------------------------------------------------------------------
 */
int 
cdsNewEpoch (cds_full_name_t     *DirName_p,
             cds_full_name_t     *MasterCH_p,
             cds_full_name_t     *SecondaryCH_p[],
             cds_full_name_t     *ReadOnlyCH_p[],
             cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    cds_full_name_t name;
    cds_full_name_t master;
    register cds_full_name_t **fnm_pp;
    register cds_full_name_t *p;
    register int SecondaryCount = 0, SecondaryLen = 0;
    register int ReadOnlyCount = 0, ReadOnlyLen = 0;
    int status;

   DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug3,
        " >cdsNewEpoch()"));

    switch (Flags_p->fsNameType) {
    case cdsStrXDS:
    case cdsStrDCE:
    case 0:
	if ((DirName_p = cds_fulltoopq(DirName_p, &name, Flags_p)) == NULL) {
	    status = CDS_ERROR;
	    goto leave_cdsNewEpoch;
	}
	if ((MasterCH_p = cds_fulltoopq(MasterCH_p, &master, Flags_p)) == NULL) {
	    status = CDS_ERROR;
	    goto leave_cdsNewEpoch;
	}
    case cdsOpqDNA:
    case cdsStrDNA:
	break;
    default:
	status = cds_status_clerk(Flags_p, CDS_INVALIDARGUMENT);
	goto leave_cdsNewEpoch;
    }
    /*
     * If we are getting string names, convert them here and sum them
     * up. Othterwise fall thru into common code, assuming opaque names
     */
    switch (Flags_p->fsNameType) {
    case cdsStrXDS:
    case cdsStrDCE:
	if (SecondaryCH_p) {
	    for (fnm_pp = &SecondaryCH_p[0]; *fnm_pp; ++fnm_pp) {
		p = (cds_full_name_t *)malloc(sizeof(cds_full_name_t));
		*fnm_pp = cds_fulltoopq(*fnm_pp, p, Flags_p);
		SecondaryLen += LEN_FullName(*fnm_pp);
		SecondaryCount++;
	    }
	}
	break;

    default:
	break;
    }
    /*
     * SecondaryCount will be non-zero if we got opaque names
     */
    if (SecondaryCH_p && SecondaryCount == 0)
	for (fnm_pp = &SecondaryCH_p[0]; *fnm_pp; ++fnm_pp) {
	    SecondaryLen += LEN_FullName(*fnm_pp);
	    SecondaryCount++;
	}

    /*
     * Do the same with the names of the read-only replicas
     */
    switch (Flags_p->fsNameType) {
    case cdsStrXDS:
    case cdsStrDCE:
	if (ReadOnlyCH_p) {
	    for (fnm_pp = &ReadOnlyCH_p[0]; *fnm_pp; ++fnm_pp) {
		p = (cds_full_name_t *)malloc(sizeof(cds_full_name_t));
		*fnm_pp = cds_fulltoopq(*fnm_pp, p, Flags_p);
		ReadOnlyLen += LEN_FullName(*fnm_pp);
		ReadOnlyCount++;
	    }
	}
	break;
   
    default:
	break;
    }
    /*
     * ReadOnlyCount will be non-zero if we got opaque names
     */
    if (ReadOnlyCH_p && ReadOnlyCount == 0)
	for (fnm_pp = &ReadOnlyCH_p[0]; *fnm_pp; ++fnm_pp) {
	    ReadOnlyLen += LEN_FullName(*fnm_pp);
	    ReadOnlyCount++;
	}

    PRM_build(OP_NewEpoch, Flags_p);
      PRM_IN_FullName(DirName_p);
      PRM_IN_FullName(MasterCH_p);
      /* Set record */
      PRM_begin();
	PRM_Set_begin(SecondaryCount);
        if(SecondaryCH_p) {
	  for (fnm_pp = &SecondaryCH_p[0]; *fnm_pp;) {
	    register cds_full_name_t *fnm_p = *fnm_pp++;

	    PRM_Set_member(1, NullTimestamp); /* present */
	    /* AtomicValue record */
	    PRM_word(LEN_FullName(fnm_p));
	    PRM_byte(VT_FullName);
	      PRM_FullName(fnm_p);
	    }
        }
	PRM_Set_end();
	PRM_end();
      /* Set record */
      PRM_begin();
	PRM_Set_begin(ReadOnlyCount);
        if (ReadOnlyCH_p) {
	  for (fnm_pp = &ReadOnlyCH_p[0]; *fnm_pp;) {
	    register cds_full_name_t *fnm_p = *fnm_pp++;

	    PRM_Set_member(1, NullTimestamp); /* present */
	    /* AtomicValue record */
	    PRM_word(LEN_FullName(fnm_p));
	    PRM_byte(VT_FullName);
	      PRM_FullName(fnm_p);
	    }
         }
	PRM_Set_end();
	PRM_end();
    PRM_send(status);

    switch (Flags_p->fsNameType) {
    case cdsStrXDS:
    case cdsStrDCE:
        if (SecondaryCH_p)
	  for (fnm_pp = &SecondaryCH_p[0]; *fnm_pp; ++fnm_pp)
		free((char *) *fnm_pp);
        if (ReadOnlyCH_p)
	  for (fnm_pp = &ReadOnlyCH_p[0]; *fnm_pp; ++fnm_pp)
		free((char *) *fnm_pp);
	break;

    default:
	break; 
    }

leave_cdsNewEpoch:

   DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug3,
        " <cdsNewEpoch() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
