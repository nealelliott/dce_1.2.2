/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 *  m o s _ p r o t o . h
 *
 *  Private prototypes to
 *  Message Oriented Service Application Programming Interface(MOS-API).
 * ______________________________________________________________________
 *
 * $Log: mos_proto.h,v $
 * Revision 1.2  1996/11/12  17:43:19  bowe
 * Use proper include for mos_base.h
 *
 * Revision 1.1  1996/10/14  17:03:42  salamone
 * Initial revision
 *
 * $EndLog$
 * ______________________________________________________________________
 */
#ifndef _MOS_PROTO_H
#define _MOS_PROTO_H

#include <mos_base.h>

/*
 * MACROS
 */
#define GOOD_STATUS(stp)          ((stp)==NULL || (*stp) == error_status_ok)
#define BAD_STATUS(stp)           ((*stp) != error_status_ok)
#define SET_STATUS(stp, val)      ((*stp) = val)
#define CLEAR_STATUS(stp)         ((*stp) = error_status_ok)
#define STATUS_EQUAL(stp, st_val) ((*stp) == (st_val))

#define EMPTY_LIST(listp)         ((listp)->count == 0 || !((listp)->list))

/* 
 * PROTOTYPES 
 */
extern boolean
mos_is_principal_valid(
                       sec_rgy_name_t        /* prin name */, 
                       sec_rgy_name_t        /* group name */, 
                       error_status_t *      /* status    */
                       );
extern boolean mos_is_logged_in(void);
extern void mos_string_list_free_ptrs(mos_string_list_t *);
extern void mos_string_list_free(mos_string_list_t **);
extern void mos_uuid_list_free_ptrs(mos_uuid_list_t *);
extern void mos_uuid_list_free(mos_uuid_list_t **);

#endif   /* _MOS_PROTO_H */
