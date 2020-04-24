/* ______________________________________________________________________ * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * debug code - prints various structures
 * ______________________________________________________________________
 */

#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/assert.h>
#include <dce/aclif.h>
#include <dce/utc.h>
#include <dce/id_epac.h>
#include <dce/pgo.h>
#include <dce/binding.h>
#include <dce/idl_es.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "mos_q.h"
#include "mos_mgmt.h"

#define bool2yn(b)	(b?'y':'n')
#define yn2bool(yn)	((yn=='n')?FALSE:TRUE)		/* default = T */
#define persist2pvm(p)	\
   ((p==mos_c_que_attr_volatile)?'v':((p==mos_c_que_attr_persistent)?'p':'m'))
#define persist2pv(p)	((p==mos_c_msg_attr_volatile)?'v':'p')
#define pvm2persist(p)	\
   ((p=='v')?mos_c_que_attr_volatile:\
     ((p=='p')?mos_c_que_attr_persistent:mos_c_que_attr_msgpersistence))

static void prin_id_to_name(uuid_t *, char *, signed32);
void Print_ack( mos_datatype_bytearray_t *);

/* ______________________________________________________________________ */

void
Print_uuid(char *s, uuid_t *u)
{
    error_status_t	st;
    unsigned_char_t	*ucp;

    uuid_to_string(u, &ucp, &st);
    if (s) fputs(s, stdout);
    puts((char *)ucp);
    free(ucp);
}

void
Print_uuid_with_name(char *s, uuid_t *u, signed32 domain)
{
    error_status_t	st;
    unsigned_char_t	*ucp;

    uuid_to_string(u, &ucp, &st);
    if (s) fputs(s, stdout);
    fputs((char *)ucp, stdout);
    free(ucp);

    if (domain >= 0) {
	char	name[100];
	prin_id_to_name(u, name, domain);
	putc(' ', stdout);
	fputs(name, stdout);
    }
    putc('\n', stdout);
}

/* ______________________________________________________________________ */

void
Print_utc(char *s, utc_t *t, int rel)
{
    char buf[UTC_MAX_STR_LEN], *bp;
    int i;
    if (rel) i = utc_ascreltime(buf, UTC_MAX_STR_LEN, t);
    else i = utc_ascgmtime(buf, UTC_MAX_STR_LEN, t);
    if (i) bp = "invalid time value";
    else bp = buf;
    printf("%s %s\n", s, bp);
}

/* ______________________________________________________________________ */

void
Print_sec_id(char *s, sec_id_t *sid, signed32 domain)
{
    fputs(s, stdout);
    if (sid->name) printf("(%s) ", &sid->name);
    Print_uuid_with_name("", &sid->uuid, domain);
}

void
Print_sec_id_pa(char *s, sec_id_pa_t *pa)
{
    char buf[100], *cp;
    printf("%s\n", s);
    Print_sec_id("  Realm: ", &pa->realm, -1);
    Print_sec_id("  Prin: ", &pa->principal, sec_rgy_domain_person);
    Print_sec_id("  Group: ", &pa->group, sec_rgy_domain_group);
    if (pa->num_groups)
	printf("   (+ %d groups)\n", pa->num_groups);
    if (pa->num_foreign_groupsets)
	printf("   (+ %d foreign groupsets)\n", pa->num_foreign_groupsets);
}

/* ______________________________________________________________________ */

void
Print_qattr(mos_qattr_t *qa)
{

    printf("%s\n", qa->name);
    Print_uuid("  ID: ", &qa->id);
    if (qa->annotation) printf("  Annotation: %s\n", qa->annotation);
    if (qa->aliases.count) {
	int i;
	printf("  Aliases: %d\n", qa->aliases.count);
	for (i=0; i<qa->aliases.count; i++)
	    printf("    %d: %s\n", i, qa->aliases.list[i]);
    }
    printf("  Max-Length: %d\n", qa->max_qlen);
    printf("  Max-Msg-Size: %d\n", qa->max_msgsize);
    printf("  Length: %d\n", qa->len);
    printf("  Persistence: %c\n", (persist2pvm(qa->persistence)));
    printf("  Enabled-enq/deq: %c/%c\n",
	(bool2yn(qa->enq_enabled)), (bool2yn(qa->deq_enabled)));
/*    printf("  Flags: 0x%x\n", qa->flags);*/
    Print_utc("  Created:", &qa->created, TRUE);
    Print_utc("  Last-Activity:", &qa->last_activity, FALSE);
    Print_utc("  Idle-Timeout:", &qa->empty_timeout, TRUE);
    Print_uuid("  ACL UUID: ", &qa->acl);
}

void
Print_qattr_1line_head(void)
{
    puts("Queue              Max  Max   Curr Enq Deq");
    puts("Name               Len  Size   Len On  On  Pers Aliases");
    puts("-------------------------------------------------------");
}
void
Print_qattr_1line(mos_qattr_t *qa)
{
    printf("%-16.16s %5d %5d %5d  %c   %c   %c",
	qa->name, qa->max_qlen, qa->max_msgsize, qa->len,
	bool2yn(qa->enq_enabled), bool2yn(qa->deq_enabled),
	persist2pvm(qa->persistence) );
    if (qa->aliases.count) {
	int i;
	printf("   %s\n", qa->aliases.list[0]);
	for (i=1; i<qa->aliases.count; i++)
	    printf("                                                %s\n",
		qa->aliases.list[i]);
    }
    else puts("");
}

/* ______________________________________________________________________ */

void
Print_mattr(mos_mattr_t *ma)
{
    Print_uuid("ID:" , &ma->id);
    Print_uuid("  Datatype: ", &ma->datatype);
    Print_sec_id_pa("  Sender:", &ma->sender);
    printf("  Msg-type: %d\n", ma->msgtype);
/*    printf("  Flags: 0x%x\n", ma->flags);*/
    printf("  Persistence: %c\n", (persist2pv(ma->persistence)));
    printf("  Priority: %d\n", ma->priority);
    printf("  Notice-opts: 0x%x\n", ma->notice_opts);
    if (ma->notice_dest) printf("  Reply-queue: %s\n", ma->notice_dest);
    Print_utc("  Enqueued:", &ma->time_enqueued, FALSE);
    Print_utc("  Expires:", &ma->expire_time, FALSE);
    Print_utc("  Valid Starting:", &ma->valid_time, FALSE);
    Print_uuid("  Queue-ID: ", &ma->qid);
}

void
Print_mattr_1line_head(void)
{
    printf("%4s %3s %3s %3s %-12s %s\n",
	"Mtyp", "Per", "Pri", "Not", "Sender", "Message ID");
}
void
Print_mattr_1line(mos_mattr_t *ma)
{
    char sender_name[100];

    prin_id_to_name(&ma->sender.principal.uuid, sender_name,
	sec_rgy_domain_person);
    printf("%3d   %c  %3d %3x %-12s", ma->msgtype, persist2pv(ma->persistence),
	ma->priority, ma->notice_opts, sender_name);
    Print_uuid(" " , &ma->id);
/*    Print_uuid("  Datatype: ", &ma->datatype);*/
/*    Print_utc("  Enqueued:", &ma->time_enqueued, FALSE);*/
}

/* ______________________________________________________________________ */

void
Print_body(mos_datatype_bytearray_t *ba, int flag)
{
    int		i, j;
    idl_char	*dp;
    char	buf1[60], buf2[20], *bp;

    if (flag == 0) {
	for (i=0,dp=ba->data; i<ba->size; i++,dp++)
	    putc(*dp, stdout);
	putc('\n', stdout);
    }
    else {		/* hex dump */
	bp = buf1;
	for (i=0,j=0,dp=ba->data; i<ba->size; i++,j++,dp++) {
	    if ( i && (i & 0xF) == 0 ) {
		buf2[j] = 0;
		printf("%-48.48s -  %s\n", buf1, buf2);
		j = 0;
		bp = buf1;
	    }
	    sprintf(bp, "%02x ", *dp);
	    bp += strlen(bp);
	    if (isprint(*dp)) buf2[j] = *dp;
	    else buf2[j] = '.';
	}
	buf2[j] = 0;
	printf("%-48.48s -  %s\n", buf1, buf2);
	Print_ack(ba);
    }
}

/* ______________________________________________________________________ */

void
Print_acl(sec_acl_t *acl, uuid_t *u)
{
    sec_acl_entry_t	*ep;
    int			i;
    sec_acl_entry_type_t t;

    Print_uuid("ACL  ID:" , u);
    Print_uuid("  Mgr-Type: ", &acl->sec_acl_manager_type);
    printf("  %d entries:\n", acl->num_entries);
    for (i=acl->num_entries, ep=acl->sec_acl_entries; --i>=0; ep++) {
	t = ep->entry_info.entry_type;
	printf("    %d type: %2d", i, t);
	    if ( sec_acl_e_type_user == t ||
		    sec_acl_e_type_group == t ||
		    sec_acl_e_type_foreign_other == t ||
		    sec_acl_e_type_user_deleg == t ||
		    sec_acl_e_type_group_deleg == t ||
		    sec_acl_e_type_for_other_deleg == t ) 
		Print_sec_id("      ", &ep->entry_info.tagged_union.id,
			sec_rgy_domain_person);
	    else if ( sec_acl_e_type_unauthenticated == t )
		printf("      Unauthen\n");
	    else if ( sec_acl_e_type_user_obj == t )
		printf("      UserObj\n");
	    else if ( sec_acl_e_type_any_other == t )
		printf("      AnyOther\n");
    }
}

/* ______________________________________________________________________ */

static void
prin_id_to_name(uuid_t *id, char *name, signed32 domain)
{
    static sec_rgy_handle_t	rh;
    static boolean		bound=0;
    sec_rgy_bind_auth_info_t	auth;
    sec_rgy_name_t		prin_in_rgy;
    error_status_t		st;

    if (!bound) {
	auth.info_type = sec_rgy_bind_auth_none;
	sec_rgy_site_bind(NULL, &auth, &rh, &st);
	if (st != error_status_ok) {
	    strcpy(name, "???");
	    return;
	}
    }
    sec_rgy_pgo_id_to_name(rh, domain, id, prin_in_rgy, &st);
    if (st != error_status_ok) {
	strcpy(name, "???");
	return;
    }
    else
	strcpy(name, (char *)prin_in_rgy);
}

/* ______________________________________________________________________ */

void
Print_ack( mos_datatype_bytearray_t *body )
{
    mos_datatype_ack_t		ack;
    idl_es_handle_t		eh;
    idl_ulong_int		esize;
    idl_byte			*ep;
    error_status_t		st;

    ep    = (idl_byte *)body->data;
    esize = (idl_ulong_int)body->size;
    idl_es_decode_buffer(ep, esize, &eh, &st);
    if (st != error_status_ok) return;

    mos_ack_convert(eh, &ack, &st);
    if (st != error_status_ok) {
	idl_es_handle_free(&eh, &st);
	return;
    }

    Print_uuid("  Ack-message: ", &ack.msg_id);
/*    printf("  Notice-Opt: %d\n", ack.notice_option);*/
    printf("  Notice-Opt:");
    if (ack.notice_option & mos_c_msg_attr_notice_enqueue) printf(" ENQ");
    if (ack.notice_option & mos_c_msg_attr_notice_dequeue) printf(" DEQ");
    printf("\n");
    printf("  Status: %d\n", ack.status);
    idl_es_handle_free(&eh, &st);
}

/* ______________________________________________________________________ */

