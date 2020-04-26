/* Generated from dhd.sams on 2020-04-26-00:42:48.000 */
/* Do not edit! */
#include <dce/dce.h>
dce_svc_subcomp_t dhd_svc_table[12] = {
    /* dhd_s_aclmgr */
    { (idl_char *)"aclmgr",
	(idl_char *)"The dced ACL managers",
	0x113db3e8 /* dhd_i_svc_aclmgr */, svc_c_debug_off },

    /* dhd_s_xattrschema */
    { (idl_char *)"xattrschema",
	(idl_char *)"The dced extended attribute schema service",
	0x113db3e9 /* dhd_i_svc_xattrschema */, svc_c_debug_off },

    /* dhd_s_general */
    { (idl_char *)"general",
	(idl_char *)"General dced facilities",
	0x113db3ea /* dhd_i_svc_general */, svc_c_debug_off },

    /* dhd_s_hostdata */
    { (idl_char *)"hostdata",
	(idl_char *)"The dced hostdata service",
	0x113db3eb /* dhd_i_svc_hostdata */, svc_c_debug_off },

    /* dhd_s_rkeytab */
    { (idl_char *)"rkeytab",
	(idl_char *)"The dced rkeytab service",
	0x113db3ec /* dhd_i_svc_rkeytab */, svc_c_debug_off },

    /* dhd_s_secval */
    { (idl_char *)"secval",
	(idl_char *)"The dced secval service",
	0x113db3ed /* dhd_i_svc_secval */, svc_c_debug_off },

    /* dhd_s_srvrconf */
    { (idl_char *)"srvrconf",
	(idl_char *)"The dced srvrconf service",
	0x113db3ee /* dhd_i_svc_srvrconf */, svc_c_debug_off },

    /* dhd_s_srvrexec */
    { (idl_char *)"srvrexec",
	(idl_char *)"The dced srvrexec service",
	0x113db3ef /* dhd_i_svc_srvrexec */, svc_c_debug_off },

    /* dhd_s_lock */
    { (idl_char *)"locks",
	(idl_char *)"Internal dced locking",
	0x113db3f0 /* dhd_i_svc_lock */, svc_c_debug_off },

    /* dhd_s_ep */
    { (idl_char *)"endpoint",
	(idl_char *)"The dced EPMAP service",
	0x113db3f1 /* dhd_i_svc_ep */, svc_c_debug_off },

    /* dhd_s_pkcache */
    { (idl_char *)"pkcache",
	(idl_char *)"The dced KDC public key cache service",
	0x113db3f2 /* dhd_i_svc_pkcache */, svc_c_debug_off },

    { 0 }
};
