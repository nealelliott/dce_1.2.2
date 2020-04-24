/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pgo_util.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:26  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:49:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:40  root]
 * 
 * Revision 1.1.2.2  1992/12/31  17:31:16  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:56:38  htf]
 * 
 * Revision 1.1  1992/01/19  04:07:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

extern void get_pgo
(    
#ifdef __STDC__
CtxBlock           *ctx,
error_status_t	   estatus,
sec_rgy_domain_t   domain,
sec_rgy_name_t     pgo_name,
sec_rgy_pgo_item_t	   *pgo,
error_status_t     *tstatus	
#endif
)  

extern void reset_tstatus
(
#ifdef __STDC__
   error_status_t      *tstatus
#endif
)
   

extern void compare_pgo 
(
#ifdef __STDC__
    CtxBlock            *ctx,
	char                *api_name,  
    sec_rgy_name_t      pname1,
    sec_rgy_pgo_item_t  *p1,
    sec_rgy_name_t      pname2,
    sec_rgy_pgo_item_t  *p2,
    error_status_t      *tstatus
#endif 
);   



extern void add_pgo 
( 
#ifdef __STDC__
CtxBlock          *ctx,
ndr_long_int       subcase,
sec_rgy_domain_t domain, 
sec_rgy_name_t pgo_name, 
sec_rgy_pgo_item_t *pgo,
error_status_t *tstatus,
error_status_t estatus
#endif
)     

extern void do_add_pgo
(    
#ifdef __STDC__
CtxBlock          *ctx,
ndr_long_int       subcase,
error_status_t     estatus,	
sec_rgy_domain_t   domain,
sec_rgy_name_t     pgo_name,
long			   unix_num,
boolean32          is_an_alias, 
error_status_t     *tstatus	
#endif
)
     

 
extern void do_delete_pgo
(    
#ifdef __STDC__
CtxBlock          *ctx,
ndr_long_int       subcase,
error_status_t     estatus,	
sec_rgy_domain_t   domain,
sec_rgy_name_t     pgo_name,
error_status_t     *tstatus	
#endif
)


