/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cdscp.h,v $
 * Revision 1.1.4.2  1996/02/18  23:33:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:32  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:06:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:22  root]
 * 
 * Revision 1.1.2.9  1994/09/15  15:22:36  mccann
 * 	change CDS_CellAlias field strings
 * 	[1994/09/15  15:12:58  mccann]
 * 
 * Revision 1.1.2.8  1994/08/03  19:00:12  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:36:17  mccann]
 * 
 * Revision 1.1.2.7  1994/06/30  19:15:16  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:36:02  mccann]
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.2.6  1994/06/23  18:29:29  mccann
 * 	sams cleanup drop1
 * 	[1994/06/23  18:07:47  mccann]
 * 
 * Revision 1.1.2.5  1994/06/09  16:08:25  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:53:11  devsrc]
 * 
 * Revision 1.1.2.4  1994/04/19  17:05:23  mccann
 * 	More code cleanup
 * 	[1994/04/19  16:25:32  mccann]
 * 
 * Revision 1.1.2.3  1994/04/04  20:05:54  peckham
 * 	Fixes for sams file cleanup
 * 	[1994/04/04  18:04:42  peckham]
 * 
 * Revision 1.1.2.2  1994/03/22  19:59:55  griffin
 * 	HCell BL3 Support: add/remove cellname commands
 * 	[1994/03/22  17:45:54  griffin]
 * 
 * Revision 1.1.2.1  1994/03/12  21:57:34  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  20:01:38  peckham]
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1993-1994. ALL RIGHTS RESERVED.
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
/* common header file for cdscp */

#if !defined(_CDSCP_H)
#define _CDSCP_H

#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/dce_msg.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <cdscpmsg.h>
#include <stdlib.h>
#include <stdio.h>
#include <msgblk.h>
#include <op_list_defs.h>
#include <ncl_environment.h>

#include <dce/dns_record.h>
#include <dce/attributes.h>
#include <dce/dnsclerk.h>
#include <dce/dnsmanpar.h>

#define LEN_mbc(p) mblen(p, MB_CUR_MAX)
#define COPY_mbc_and_skip(s,d) \
    { int xxlen = LEN_mbc(s); if (0 < xxlen) do *d++ = *s++; while (--xxlen); }


/*
 * Universal externs
 */
extern int debug;
#if defined(DCE_SEC)
  extern int Authentication;
#endif

struct prs;

/* accum_cmd.c */

int accum_cmd(char *, char **);

/* argv.c */

int argv_size(int, char **, int);
char *reassemble_command(int, char **, int, char *);

/* attrgroup_actions.c */

int set_attr(struct prs *);
struct attrblk *attrblk_pointer(void);
void attrblk_init(struct attrblk *);
int save_attr(struct prs *);

/* attributes_list.c */

void init_singlevalueblk(struct singlevalueblk *);
struct attrblk *copyattr_new(struct attrblk *);
void linkattr_after(struct attrblk *, struct attrblk *);
void linkattr_before(struct attrblk *, struct attrblk *);
int copyattr_before(struct attrblk *, struct attrblk *);
void delete_1_attr(struct attrblk *);
void delete_attr_list(struct attrblk *);
struct attrblk *find_attr(struct attrblk *, dns_opq_sname *);

/* attrname_actions_cds.c */

struct attrblk *attrname_pointer(void);
int istimeout(struct prs *);
int isconvergence(struct prs *);
int isrptype(struct prs *);
int isrpstate(struct prs *);
int isinch(struct prs *);
int ischstate(struct prs *);
int isversion(struct prs *);
unsigned char *scan_string(unsigned char *, unsigned char *, unsigned char *);
int save_any_attr(struct attrblk *, unsigned char *, AtomicValue_t *, int, int);
int save_named_attr(struct prs *);
int saveverattr(struct prs *);
int savecvattr(struct prs *);
int savecnattr(struct prs *);
int savelnktarget(struct prs *);
int savelnktimeout(struct prs *);
int savetoattr(struct prs *);
int save1byteattr(struct prs *);
int savecdsattr(struct prs *);
int savesmallattr(struct prs *);
int saveshortattr(struct prs *);
int savelongattr(struct prs *);
int savetsattr(struct prs *);
int savetowerattr(struct prs *);
int savenameattr(struct prs *);
int savesimpleattr(struct prs *);
int saveuuidattr(struct prs *);
int savenoneattr(struct prs *);
int savecharattr(struct prs *);

/* baddest.c */

int baddest(int, int);

/* cdscp_help.c */

dns_status_t cdscp_help(void);

/* cmdblk_init.c */

void cmdblk_init(struct cmdblk *,struct cmdblk *);

/* confidence.c */

int *get_conf_ptr(void);
int get_conf(void);
int saveconf(struct prs *);
dns_status_t set_conf(struct envblk *,struct cmdblk *,struct op_blk *);
dns_status_t show_conf(struct envblk *,struct cmdblk *,struct op_blk *);

/* cp_cell.c */

dns_status_t showcell(int,struct entblk *,struct envblk *,struct op_blk *);
dns_status_t cp_hiercell(int,struct entblk *,struct envblk *,struct op_blk *);

/* cprpc_clerk.c */

int cp_clerk_cmd(int,struct entblk *,struct attrblk *,struct envblk *,struct op_blk *);

/* cprpc_emit_clerk.c */

struct _cprpc_emit_parms;

void emit_clerk_create_time(struct _cprpc_emit_parms *);
void emit_clerk_auth_count(struct _cprpc_emit_parms *);
void emit_clerk_read_ops_count(struct _cprpc_emit_parms *);
void emit_clerk_cache_hits_count(struct _cprpc_emit_parms *);
void emit_clerk_cache_bypass_count(struct _cprpc_emit_parms *);
void emit_clerk_write_ops_count(struct _cprpc_emit_parms *);
void emit_clerk_misc_ops_count(struct _cprpc_emit_parms *);
void emit_cached_clh_info(struct _cprpc_emit_parms *);
void emit_cached_ns_info(struct _cprpc_emit_parms *);

/* cprpc_emit_server.c */

void emit_server_create_time(struct _cprpc_emit_parms *);
void emit_server_future_skew(struct _cprpc_emit_parms *);
void emit_server_known_clearinghouses(struct _cprpc_emit_parms *);
void emit_server_read_access_count(struct _cprpc_emit_parms *);
void emit_server_write_access_count(struct _cprpc_emit_parms *);
void emit_server_skulks_initiated_count(struct _cprpc_emit_parms *);
void emit_server_skulks_completed_count(struct _cprpc_emit_parms *);
void emit_server_tree_broken_count(struct _cprpc_emit_parms *);
void emit_server_crucial_replica_count(struct _cprpc_emit_parms *);
void emit_server_child_pointer_count(struct _cprpc_emit_parms *);
void emit_server_security_count(struct _cprpc_emit_parms *);
void emit_clearinghouse_counters(struct _cprpc_emit_parms *);

/* cprpc_server.c */

int cp_server_cmd(int,struct entblk *,struct attrblk *,struct envblk *,struct op_blk *);

/* default_ent.c */

void init_default_entity(void);
int reset_default_entity(struct envblk *,struct cmdblk *,struct op_blk *);
void allow_default_entity(struct entblk *);

/* dnscp_args.c */

int debug_requested(void);
char *startup_file_requested(void);
int process_args(int,char **);

/* dnscp_credir.c */

int do_dir_cmd(int,struct entblk *,struct attrblk *,struct envblk *,struct cmdblk *,struct op_blk *);

/* dnscp_crekid.c */

int crekid(struct entblk *,struct attrblk *,struct envblk *,struct cmdblk *,struct op_blk *);

/* dnscp_creobj.c */

int creobj(struct entblk *,struct attrblk *,struct envblk *,struct cmdblk *,struct op_blk *);

/* dnscp_cresl.c */

int cresl(struct entblk *,struct attrblk *,struct envblk *,struct cmdblk *,struct op_blk *);

/* dnscp_delkid.c */

int delkid(struct entblk *,struct attrblk *,struct envblk *,struct cmdblk *,struct op_blk *);

/* dnscp_delobj.c */

int delobj(struct entblk *,struct attrblk *,struct envblk *,struct cmdblk *,struct op_blk *);

/* dnscp_delsl.c */

int delsl(struct entblk *,struct attrblk *,struct envblk *,struct cmdblk *,struct op_blk *);

/* dnscp_replica.c */

int do_replica_cmd(int, struct entblk *,struct attrblk *,struct envblk *,struct cmdblk *,struct op_blk *);

/* dnsinitflagstat.c */

void dnsInitFlagStat(struct dnsFlagStat *);

/* dumpclerkcachecmd.c */

int dumpclerkcachecmd(void);

/* emit_cds_values.c */

void emit_cds_values(struct op_blk *,char *,dns_attrvalue_t *);
void emit_cds_sname(struct op_blk *,char *,dns_attrvalue_t *);
void emit_cds_tower(struct op_blk *,char *,dns_attrvalue_t *);
void emit_cds_chstate(struct op_blk *,char *,dns_attrvalue_t *);
void emit_cds_convergence(struct op_blk *,char *,dns_attrvalue_t *);
void emit_cds_inch(struct op_blk *,char *,dns_attrvalue_t *);
void emit_cds_reptype(struct op_blk *,char *,dns_attrvalue_t *);
void emit_cds_repstate(struct op_blk *,char *,dns_attrvalue_t *);
void emit_towerset_lines(struct op_blk *,Set_t *);
void emit_cds_cellaliases(struct op_blk *,char *,dns_attrvalue_t *);

/* emit_exp_line.c */

void emit_exp_line(struct op_blk *,struct utc *);

/* emit_ext_line.c */

void emit_ext_line(struct op_blk *,struct utc *);

/* emit_group_line.c */

void emit_group_line(struct op_blk *,unsigned char *);

/* emit_member_lines.c */

void emit_member_lines(struct op_blk *,unsigned char *);

/* emit_myname_line.c */

void emit_myname_line(struct op_blk *,unsigned char *);

/* emit_one_line.c */

void emit_line_fmt(struct op_blk *, unsigned32, char *, ...);
void emit_hdr(struct op_blk *, unsigned32, char *);
void emit_hdr_string(struct op_blk *, unsigned32, char *, char *);
void emit_name_string(struct op_blk *, char *, char *);
void emit_attr_string(struct op_blk *, unsigned32, char *, char *);
void emit_attr_number(struct op_blk *, unsigned32, char *, int);

/* emit_ref_hdr.c */

void emit_refhdr_atline(struct op_blk *);
void emit_reference_header(struct op_blk *,struct entblk *,unsigned32,char *);

struct entblk *entblk_pointer(void);
void ent_one_onetime(struct entblk *,int);
void ent_onetime(void);
int move_name_to_entblk(unsigned char *,struct entblk *);
int entity(struct prs *);
int objent(struct prs *);
int grpent(struct prs *);
int slent(struct prs *);
int dirent(struct prs *);
int cellent(struct prs *);
int clhent(struct prs *);
int kident(struct prs *);
int repent(struct prs *);
int clear_ent_list(struct prs *);
int exclude(struct prs *);
int filename1(struct prs *);
int filename2(struct prs *);
int wilddir1ent(struct prs *);
int move_entblk_to_entblk(struct entblk *,struct entblk *);

/* entities_list.c */

struct entblk *copyent_new(struct entblk *);
void linkent_after(struct entblk *,struct entblk *);
void linkent_before(struct entblk *,struct entblk *);
int copyent_before(struct entblk *,struct entblk *);
void delete_1_ent(struct entblk *);
void delete_ent_list(struct entblk *);

/* err_list.c */

struct err_blk;

void err_list_hdr_init(struct err_blk *);
struct err_blk *insert_err_list_element_before(struct err_blk *,struct err_blk *);
struct err_blk *insert_err_list_element_after(struct err_blk *,struct err_blk *);
struct err_blk *create_err_list_element(unsigned char *,struct dnsFlagStat *,char *);
struct err_blk *nonapi_err_list_element(unsigned char *,int);
void delete_one_err_list_element(struct err_blk *);
void delete_all_err_list_elements(struct err_blk *);

/* err_report.c */

void emit_error_report(struct err_blk *,struct  op_blk *);

/* exec_docmd.c */

int exec_docmd(char *,struct entblk *,struct attrblk *,struct envblk *,struct cmdblk *,struct op_blk *);

/* exec_mgmt_cmd_cds.c */

int exec_cmd(struct cmdblk *,struct op_blk *,struct logblk *,int *);

/* filespec_actions.c */

struct fsblk *get_fsblk_ptr(void);
void fsblk_clrspec(struct fsblk *);
void fsblk_init(struct fsblk *);
void fsblk_freespec(struct fsblk *);
void fsblk_copyspec(struct fsblk *,struct fsblk *);
void fsblk_movespec(struct fsblk *,struct fsblk *);
int stash_filespec(struct prs *);
int set_access(struct prs *);
int save_tofile(struct prs *);
int atsigntest(struct prs *);

/* find_kind.c */

int find_kind(unsigned char *);

/* flags_actions.c */

unsigned char *flags_pointer(void);
int flags_actions(struct prs *);

/* fmt_uid_for_print.c */

void fmt_uid_for_print(unsigned char *,char *);

/* fopen2.c */

FILE *fopen2(char *,char *);
char *fopen2aux1(char *);
char *fopen2aux2(char *);
void fopen2aux3(char *);

/* format.c */

void format(unsigned char *,int,char *);

/* format_chstate_for_print.c */

void format_chstate_for_print(char *,int);

/* format_classversion_for_print.c */

void format_classversion_for_print(char *,ClassVersion_t *);

/* format_confidence_for_print.c */

void format_confidence_for_print(char *,int);

/* format_convergence_for_print.c */

void format_convergence_for_print(char *,unsigned int);

/* format_fullname_for_print.c */

void format_fullname_for_print(FullName_t *,char *);

/* format_inch_for_print.c */

void format_inch_for_print(char *,unsigned int);

/* format_reptyp_for_print.c */

void format_reptyp_for_print(char *,unsigned int);

/* format_simplename_for_print.c */

void format_simplename_for_print(char *,dns_opq_sname *);

/* format_tower.c */

int format_tower(char **,MemberValue_t *);

/* format_utcabs_for_print.c */

void format_utcabs_for_print(char *,utc_t *);

/* get_command.c */

char *get_command_line(struct cmdblk *);
int get_command(struct cmdblk *);

/* hash.c */

int hash(unsigned char *,int,int);

/* inch_actions.c */

unsigned char *inch_pointer(void);
int saveinch(struct prs *);

/* load_parse_file.c */

void load_parse_file(struct prs *,char *,int);

/* logging.c */

struct logparmblk;

int init_logblk(struct logblk *);
int log_command_now(struct logblk *,struct cmdblk *);
int log_command(struct logblk *,struct cmdblk *,struct op_blk *);
int log_output_now(struct logparmblk *,struct op_blk *,unsigned char *);
int log_output(void *,struct op_blk *,unsigned char *);

/* mgmt_main.c */

int main(int,char *[]);
struct op_blk *get_op_blk_ptr(void);
struct err_blk *get_err_blk_ptr(void);
struct envblk *get_envblk_ptr(void);
struct entblk *entitylist_pointer(void);
struct attrblk *get_attrblk_ptr(void);

/* msg.c */

int main(int,char *[]);

/* msgfil.c */

/* msgfnd.c */

void cds_printf (unsigned32, char *, ...);
void cds_fprintf (FILE *, unsigned32, char *, ...);
void cds_sprintf (char *, unsigned32, char *, ...);
void cds_fputs (unsigned32, char *, FILE *);
void cds_strcpy (char *, unsigned32, char *);
void cds_strcat (char *, unsigned32, char *);

/* names_actions_cds.c */

unsigned char *name_pointer(void);
unsigned char *aname_pointer(void);
int aname_type(void);
unsigned char *sname_pointer(void);
unsigned char *wild_sname_pointer(void);
int names_onetime_init(void);
int scan_name(struct prs *,unsigned char *);
int simplename(struct prs *);
int wildsimplename(struct prs *);
int nowildfullname(struct prs *);
int rootfullname(struct prs *);
int wildfullname(struct prs *);
int attributename(struct prs *);
int nsnwildfullname(struct prs *);

/* op_list.c */

void simple_header_routine(struct op_blk *);
void init_op_list_element(struct op_blk *);
void op_list_hdr_init(struct op_blk *);
struct op_blk *insert_op_list_element_before(struct op_blk *,struct op_blk *);
struct op_blk *insert_op_list_element_after(struct op_blk *,struct op_blk *);
struct op_blk *create_op_list_element(char *,int,int,int,struct f_blk *);
struct op_blk *create_op_list_element_before(char *,int,int,int,struct f_blk *,struct op_blk *);
struct op_blk *create_op_list_element_after(char *,int,int,int,struct f_blk *,struct op_blk *);
void unlink_op_list_blk(struct op_blk *);
int delete_one_op_list_element(struct op_blk *);
int delete_all_op_list_elements(struct op_blk *);
int open_all_files(struct op_blk *);
void close_all_files(struct op_blk *);
int close_one_file(struct op_blk *);
void emit_line(struct op_blk *,char *);
void emit_one_copy_of_line(struct op_blk *,char *);
void emit_blank_line(struct op_blk *);
int valid_access_code(int);
int valid_status_code(int);
void set_default_format(struct f_blk *);
int op_list_empty(struct op_blk *);
void set_default_stdout(struct op_blk *);
struct f_blk *default_format_pointer(void);
struct op_blk *default_op_blk_pointer(void);
void op_default_init(void);
void use_default_op_element(struct op_blk *,struct op_blk *);
void use_default_op_list(struct op_blk *);
void allow_default_op_list(struct op_blk *);
void new_page(struct op_blk *);
void set_top_of_form(struct op_blk *);

/* operator_actions.c */

int *relop_pointer(void);
int save_relop(struct prs *);
int *levelop_pointer(void);
int save_levelop(struct prs *);

/* parse_command.c */

void parse_init(struct envblk *,struct cmdblk *,struct prs *,struct op_blk *);
int parse_command(struct cmdblk *);
int set_func(struct prs *);
int qjnop(struct prs *);

/* parser_aid_2.c */

struct op_blk *get_op_blk_ptr(void);
int username(struct prs *);
int main(int,char *[]);

/* parsing_support.c */

void prsblk_init(struct prs *,char *,int);

/* preferred_clh.c */

dns_opq_fname *preferred_clh_pointer(void);
int show_preferred_clh(struct envblk *,struct cmdblk *,struct op_blk *);
int set_preferred_clh(struct envblk *,struct cmdblk *,struct op_blk *);
int save_pfd_clh(struct prs *);

/* pretend_not_seen.c */

int pretend_not_seen(struct prs *);

/* process_commands.c */

int process_commands(struct cmdblk *,struct op_blk *,struct entblk *,struct attrblk *,struct logblk *);

/* prs.c */

int main(int,char *[]);

/* prscmd.c */

int pr_command(struct prs *);

/* prsrea.c */

int pr_read(struct prs *,char *);

/* recognize_filespec.c */

int recognize_unix_filespec(unsigned char *);

/* replica_actions.c */

unsigned char *reptyp_pointer(void);
int save_reptype(struct prs *);
struct entblk *repblk_pointer(void);
int save_rep(struct prs *);

/* report_parse_error.c */

void report_parse_error(struct cmdblk *,int);

/* savemajorminor.c */

unsigned char *majorminor_pointer(void);
int savemajorminor(struct prs *);

/* singlevalue_actions.c */

int svblk_onetime(struct prs *);
int svblk_init(struct prs *);
struct singlevalueblk *atomvblk_pointer(struct prs *);
int parse_single_value(struct prs *);

/* singlevalue_support.c */

int sv_length(unsigned char *,int *,int *);
int sv_form(unsigned char *,AtomicValue_t *);

/* spaces.c */

char *spaces(int);
void spaces_init(void);

/* startup_file.c */

void use_startup_file(void);

/* time_actions.c */

utc_t *abstime_pointer(void);
utc_t *deltatime_pointer(void);
int parse_abstime(struct prs *);
int parse_deltatime(struct prs *);

/* visit_cds.c */

struct dnsFlagStat *init_flagstatus(struct dnsFlagStat *,enum dnsConf,int *);
struct knownattr *get_known_attribute(dns_opq_sname *,attribute_descriptor_t **);
int visit_attr_list(struct entblk *,struct attrblk *,struct envblk *,struct cmdblk *,struct op_blk *);
int visit_entlist(struct entblk *,struct attrblk *,struct attrblk *,struct envblk *,struct cmdblk *,struct op_blk *);
int set_attr_line(struct entblk *,struct attrblk *,struct envblk *,struct cmdblk *);
int enum_things(struct entblk *,struct attrblk *,struct envblk *,struct cmdblk *,struct op_blk *);
int find_leftmost_wild(dns_opq_fname *);

/* with.c */

struct attrblk *withlist_pointer(void);
void withblock_init(struct attrblk *);
void withhead_init(struct attrblk *);
void make_withlist_empty(struct attrblk *);
int after_this_is_with(struct prs *);
void cut_after_paste_after_attr(struct attrblk *,struct attrblk *);
void form_with_list(void);


/* ORPHANS */

/* utc_comptime */

#endif /* _CDSCP_H */
