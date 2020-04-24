/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ids_free.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:23  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:22:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:15  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:03  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:25:55  marrek]
 * 
 * Revision 1.1.8.2  1994/05/10  15:50:24  marrek
 * 	April 1994 submission.
 * 	[1994/04/29  10:52:12  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  18:35:05  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:44:57  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:08:58  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:30:00  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:11:45  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:08:44  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  16:27:27  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  13:11:52  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:03:23  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:49:37  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ids_free.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:34 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

#include <gds.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/******** LOCAL FUNCTIONS ********/

static void i_psap_free ( ids_psap * );
static void i_utc_time_free ( ids_utc_time * );
static void i_att_type_free ( ids_att_type * );
static void i_att_value_free ( asn1_field );
static void i_att_free ( ids_att * );
static void i_assertion_free ( ids_assertion * );
static void i_rdname_free ( ids_rdname * );
static void i_serv_controls_free ( ids_serv_controls * );
static void i_extension_free ( ids_extension * );
static void i_common_arg_free ( ids_common_arg * );
static void i_common_res_free ( ids_common_res * );
static void i_entry_info_sel_free ( ids_entry_info_sel * );
static void i_entry_info_free ( ids_entry_info * );
static void i_substring_free ( ids_substring * );
static void i_filter_item_free ( ids_filter_item * );
static void i_filter_free ( ids_filter * );
static void i_acc_pt_free ( ids_acc_pt * );
static void i_cross_ref_free ( ids_cross_ref * );
static void i_trace_infos_free ( ids_trace_infos * );
static void i_validity_free ( ids_validity * );
static void i_part_oq_free ( ids_part_oq * );
static void i_op_prog_free ( ids_op_prog * );

#define i_name_free(dist_name) i_dist_name_free(dist_name)

static void i_psap_free(
  ids_psap * psap)
{
signed32 i;

if((*psap).psel_defined != FALSE){
free((*psap).psel.contents);
}
if((*psap).ssel_defined != FALSE){
free((*psap).ssel.contents);
}
if((*psap).tsel_defined != FALSE){
free((*psap).tsel.contents);
}
if((*psap).nb_nsap != 0){

for (i=0; i <(*psap).nb_nsap; i++){
free((*psap).nsap[i].contents);
}
free((*psap).nsap);
}
}

static void i_utc_time_free(
  ids_utc_time * x)
{
free((*x).contents);
}

static void i_att_type_free(
  ids_att_type * x)
{
free((*x).v);
}

static void i_att_value_free(
  asn1_field val)
{
free(val.v);
}


static void i_att_free(
  ids_att * att)
{
signed32 i;

i_att_type_free(&(*att).type);
for (i=0; i <(*att).nb_values; i++){

i_att_value_free((*att).values[i]);
}
free((*att).values);
}

static void i_assertion_free(
  ids_assertion * assertion)
{
i_att_type_free(&(*assertion).type);
i_att_value_free((*assertion).value);
}

static void i_rdname_free(
  ids_rdname * rdname)
{
signed32 i;

for (i=0; i <(*rdname).nb_assertions; i++){

i_assertion_free(&(*rdname).assertions[i]);
}
free((*rdname).assertions);
}

void i_dist_name_free(
  ids_dist_name * dist_name)
{
signed32 i;

for (i=0; i <(*dist_name).nb_rdnames; i++){

i_rdname_free(&(*dist_name).rdnames[i]);
}
free((*dist_name).rdnames);
}

static void i_serv_controls_free(
  ids_serv_controls * serv_controls)
{
if((*serv_controls).options != IDS_UNDEFINED){
}
if((*serv_controls).priority != MEDIUM_PRIOR){
}
if((*serv_controls).time_limit_defined != FALSE){
}
if((*serv_controls).size_limit_defined != FALSE){
}
if((*serv_controls).scope_ref_defined != FALSE){
}
}

static void i_extension_free(
  ids_extension * extension)
{
if((*extension).critical != FALSE){
}
free((*extension).item.v);
}

static void i_common_arg_free(
  ids_common_arg * arg)
{
signed32 i;

if((*arg).controls.options != IDS_UNDEFINED ||
(*arg).controls.priority != MEDIUM_PRIOR ||
(*arg).controls.time_limit_defined != FALSE ||
(*arg).controls.size_limit_defined != FALSE ||
(*arg).controls.scope_ref_defined != FALSE){

i_serv_controls_free(&(*arg).controls);
}
if((*arg).requestor_defined != FALSE){

i_dist_name_free(&(*arg).requestor);
}
if((*arg).op_prog.name_res_ph != NOTSTARTED){

i_op_prog_free(&(*arg).op_prog);
}
if((*arg).alias_rdn_defined != FALSE){
}
if((*arg).extensions_defined != FALSE){

for (i=0; i <(*arg).nb_extensions; i++){

i_extension_free(&(*arg).extensions[i]);
}
free((*arg).extensions);
}
}

static void i_common_res_free(
  ids_common_res * res)
{
if((*res).requestor_defined != FALSE){
i_dist_name_free(&(*res).requestor);
}
if((*res).alias_deref != FALSE){
}
}

static void i_entry_info_sel_free(
  ids_entry_info_sel * entry_info_sel)
{
signed32 i;

if((*entry_info_sel).is_all_att != TRUE){

switch ((*entry_info_sel).is_all_att){
case TRUE: break;
default:
if ((*entry_info_sel).nb_types != 0){
for (i=0; i <(*entry_info_sel).nb_types; i++){

i_att_type_free(&(*entry_info_sel).types[i]);
}
free((*entry_info_sel).types);}
break;}
}
if((*entry_info_sel).info_type_mode != ATT_TYPE_AND_VALUE){
}
}

static void i_entry_info_free(
  ids_entry_info * entry_info)
{
signed32 i;

i_dist_name_free(&(*entry_info).dist_name);
if((*entry_info).from_entry != TRUE){
}
if((*entry_info).infos_defined != FALSE){
for (i=0; i <(*entry_info).nb_info; i++){

switch ((*entry_info).infos[i].type_only){

case FALSE: i_att_free(&(*entry_info).infos[i].contents.attribute);
break;

default: i_att_type_free(&(*entry_info).infos[i].contents.type);
break;}
}
free((*entry_info).infos);
}
}

static void i_substring_free(
  ids_substring * substring)
{
signed32 i;

i_att_type_free(&(*substring).type);
for (i=0; i <(*substring).nb_pattern; i++){

switch ((*substring).patterns[i].mode){

case PATTERN_INITIAL: i_att_value_free((*substring).patterns[i].value);
break;
case PATTERN_ANY: i_att_value_free((*substring).patterns[i].value);
break;
case PATTERN_FINAL: i_att_value_free((*substring).patterns[i].value);
break;
default:break;}
}
free((*substring).patterns);
}

static void i_filter_item_free(
  ids_filter_item * filter_item)
{
switch ((*filter_item).mode){
case FILTER_ITEM_EQUAL: i_assertion_free(&(*filter_item).value.equal);
break;
case FILTER_ITEM_SUBSTRING: i_substring_free(&(*filter_item).value.substring);
break;
case FILTER_ITEM_GREATER_EQUAL: i_assertion_free(&(*filter_item).value.gequal);
break;
case FILTER_ITEM_LESS_EQUAL: i_assertion_free(&(*filter_item).value.lequal);
break;
case FILTER_ITEM_PRESENT: i_att_type_free(&(*filter_item).value.present);
break;
case FILTER_ITEM_APPROXIMATE: i_assertion_free(&(*filter_item).value.approx_match);
break;
default:break;}
}

static void i_filter_free(
  ids_filter * filter)
{
signed32 i;

switch ((*filter).mode){
case FILTER_ITEM: i_filter_item_free(&(*filter).value.fil_item);
break;

case FILTER_AND: for (i=0; i <(*filter).value.fil_and.nb_filters; i++){

i_filter_free(&(*filter).value.fil_and.filters[i]);
}
free((*filter).value.fil_and.filters);
break;

case FILTER_OR: for (i=0; i <(*filter).value.fil_or.nb_filters; i++){

i_filter_free(&(*filter).value.fil_or.filters[i]);
}
free((*filter).value.fil_or.filters);
break;

case FILTER_NOT: i_filter_free(&(*filter).value.fil_not[0]);
free((*filter).value.fil_not);
break;
default:break;}
}

static void i_op_prog_free(
  ids_op_prog * op_prog)
{
if((*op_prog).next_rdn_defined != FALSE){
}
}

static void i_acc_pt_free(
  ids_acc_pt * acc_pt)
{
i_name_free(&(*acc_pt).name);
i_psap_free(&(*acc_pt).addr);
}

static void i_cross_ref_free(
  ids_cross_ref * cross_ref)
{
i_dist_name_free(&(*cross_ref).ctx_prefix);
i_acc_pt_free(&(*cross_ref).acc_pt);
}

static void i_trace_infos_free(
  ids_trace_infos * trace_infos)
{
signed32 i;

for (i=0; i <(*trace_infos).nb; i++){

i_name_free(&(*trace_infos).list[i].dsa);
if((*trace_infos).list[i].target.nb_rdnames != IDS_UNDEFINED){

i_name_free(&(*trace_infos).list[i].target);
}

i_op_prog_free(&(*trace_infos).list[i].op_prog);
}
free((*trace_infos).list);
}

void i_cont_reference_free(
  ids_cont_reference * cont_ref)
{
signed32 i;

i_name_free(&(*cont_ref).target);
if((*cont_ref).alias_rdn_defined != FALSE){
}

i_op_prog_free(&(*cont_ref).op_prog);
if((*cont_ref).rdn_resolved_defined != FALSE){
}
if((*cont_ref).ref_type_defined != FALSE){
}

for (i=0; i <(*cont_ref).nb_acc_pt; i++){

i_acc_pt_free(&(*cont_ref).acc_pts[i]);
}
free((*cont_ref).acc_pts);
}

static void i_validity_free(
  ids_validity * validity)
{
if((*validity).time1_defined != FALSE){

i_utc_time_free(&(*validity).time1);
}
if((*validity).time2_defined != FALSE){

i_utc_time_free(&(*validity).time2);
}
if((*validity).random1_defined != FALSE){
free((*validity).random1.contents);
}
if((*validity).random2_defined != FALSE){
free((*validity).random2.contents);
}
}

void i_bind_arg_free(
  ids_bind_arg * arg)
{
if((*arg).cred_defined != FALSE){

switch ((*arg).cred_kind){
case CREDENTIAL_SIMPLE: 
i_dist_name_free(&(*arg).credentials.simple.name);
if((*arg).credentials.simple.validity_defined != FALSE){

i_validity_free(&(*arg).credentials.simple.validity);
}
if((*arg).credentials.simple.password_defined != FALSE){
  if ((*arg).credentials.simple.password.nb != 0)
	free((*arg).credentials.simple.password.contents);
}
break;

case CREDENTIAL_STRONG: break;
case CREDENTIAL_EXTERNAL: 
if ((*arg).credentials.external.v != NULL)
  {
  free((*arg).credentials.external.v);
  }
break;
default:break;}
}
if((*arg).version != V1988){
}
}

void i_bind_res_free(
  ids_bind_res * arg)
{
if((*arg).cred_defined != FALSE){

switch ((*arg).cred_kind){
case CREDENTIAL_SIMPLE: 
i_dist_name_free(&(*arg).credentials.simple.name);
if((*arg).credentials.simple.validity_defined != FALSE){

i_validity_free(&(*arg).credentials.simple.validity);
}
if((*arg).credentials.simple.password_defined != FALSE){
free((*arg).credentials.simple.password.contents);
}
break;

case CREDENTIAL_STRONG: break;
case CREDENTIAL_EXTERNAL:
if ((*arg).credentials.external.v != NULL)
  {
  free((*arg).credentials.external.v);
  }
break;
default:break;}
}
if((*arg).version != V1988){
}
}

void i_ch_arg_free(
  ids_ch_arg * ch_arg,
  asn1_field arg)
{
if((*ch_arg).originator.nb_rdnames != IDS_UNDEFINED){
i_dist_name_free(&(*ch_arg).originator);
}
if((*ch_arg).target.nb_rdnames != IDS_UNDEFINED){

i_dist_name_free(&(*ch_arg).target);
}
if((*ch_arg).op_prog.name_res_ph != NOTSTARTED){

i_op_prog_free(&(*ch_arg).op_prog);
}

i_trace_infos_free(&(*ch_arg).trace_infos);
if((*ch_arg).alias_deref != FALSE){
}
if((*ch_arg).aliased_rdn != FALSE){
}
if((*ch_arg).rt_cross_refs != FALSE){
}
if((*ch_arg).ref_type != SUPERIOR){
}
if((*ch_arg).domain_info.l != IDS_UNDEFINED){
free((*ch_arg).domain_info.v);
}
if((*ch_arg).time_limit.nb != IDS_UNDEFINED){

i_utc_time_free(&(*ch_arg).time_limit);
}
if(arg.l != 0){
free(arg.v);
}
}

void i_ch_res_free(
  ids_ch_res * ch_res,
  asn1_field res)
{
signed32 i;

if((*ch_res).domain_info.l != IDS_UNDEFINED){
free((*ch_res).domain_info.v);
}
if((*ch_res).nb_cross_ref != IDS_UNDEFINED){

for (i=0; i <(*ch_res).nb_cross_ref; i++){

i_cross_ref_free(&(*ch_res).cross_refs[i]);
}
free((*ch_res).cross_refs);
}
free(res.v);
}

void i_ch_null_res_free(
  ids_ch_res * ch_res)
{
signed32 i;

if((*ch_res).domain_info.l != IDS_UNDEFINED){
free((*ch_res).domain_info.v);
}
if((*ch_res).nb_cross_ref != IDS_UNDEFINED){

for (i=0; i <(*ch_res).nb_cross_ref; i++){

i_cross_ref_free(&(*ch_res).cross_refs[i]);
}
free((*ch_res).cross_refs);
}
}

void i_read_arg_free(
  ids_read_arg * arg,
  ids_common_arg * common_arg)
{
i_name_free(&(*arg).object);
if((*arg).selection.is_all_att != TRUE ||
(*arg).selection.info_type_mode != ATT_TYPE_AND_VALUE){

i_entry_info_sel_free(&(*arg).selection);
}

i_common_arg_free(common_arg);
}

void i_read_res_free(
  ids_read_res * res,
  ids_common_res * common_res)
{
i_entry_info_free(&(*res).entry_info);
i_common_res_free(common_res);
}

void i_compare_arg_free(
  ids_compare_arg * arg,
  ids_common_arg * common_arg)
{
i_name_free(&(*arg).object);
i_assertion_free(&(*arg).purported);
i_common_arg_free(common_arg);
}

void i_compare_res_free(
  ids_compare_res * res,
  ids_common_res * common_res)
{
if((*res).dname_defined != FALSE){

i_dist_name_free(&(*res).dname);
}
if((*res).from_entry != TRUE){
}

i_common_res_free(common_res);
}

void i_abandon_arg_free(
  signed32 arg)
{
}

void i_list_arg_free(
  ids_list_arg * arg,
  ids_common_arg * common_arg)
{
i_name_free(&(*arg).object);
i_common_arg_free(common_arg);
}

static void i_part_oq_free(
  ids_part_oq * part_op)
{
signed32 i;

if((*part_op).limit_pb_defined != FALSE){
}
if((*part_op).unexplored_defined != FALSE){

for (i=0; i <(*part_op).nb_unexplored; i++){

i_cont_reference_free(&(*part_op).unexplored[i]);
}
free((*part_op).unexplored);
}
if((*part_op).un_crit_ext != FALSE){
}
}

void i_list_res_free(
  ids_list_res * res,
  ids_common_res * common_res)
{
signed32 i;

switch ((*res).uncorrelated){
case FALSE: if((*res).info.list_info.dname_defined != FALSE){
i_dist_name_free(&(*res).info.list_info.dname);
}

for (i=0; i <(*res).info.list_info.nb_subordinate; i++){


i_rdname_free(&(*res).info.list_info.subordinates[i].rdname);
if((*res).info.list_info.subordinates[i].alias_entry != FALSE){
}
if((*res).info.list_info.subordinates[i].from_entry != TRUE){
}
}
if((*res).info.list_info.nb_subordinate > 0)
  {
free((*res).info.list_info.subordinates);
  }
if((*res).info.list_info.part_oq_defined != FALSE){

i_part_oq_free(&(*res).info.list_info.part_oq);
}

i_common_res_free(common_res);
break;
case TRUE: for (i=0; i <(*res).info.uncorr_list_info.nb; i++){

i_list_res_free(&(*res).info.uncorr_list_info.list[i],common_res);
}
if((*res).info.uncorr_list_info.nb > 0)
  {
free((*res).info.uncorr_list_info.list);
  }
break;
default:break;}
}

void i_search_arg_free(
  ids_search_arg * arg,
  ids_common_arg * common_arg)
{
i_name_free(&(*arg).base_object);
if((*arg).subset_mode != BASE_OBJECT){
}
if((*arg).filter.mode != 1 ||
(*arg).filter.value.fil_and.nb_filters != 0){

i_filter_free(&(*arg).filter);
}
if((*arg).alias != TRUE){
}
if((*arg).selection.is_all_att != TRUE ||
(*arg).selection.info_type_mode != ATT_TYPE_AND_VALUE){

i_entry_info_sel_free(&(*arg).selection);
}

i_common_arg_free(common_arg);
}

void i_search_res_free(
  ids_search_res * res,
  ids_common_res * common_res)
{
signed32 i;

switch ((*res).uncorrelated){
case FALSE: if((*res).info.search_info.dname_defined != FALSE){

i_dist_name_free(&(*res).info.search_info.dname);
}

for (i=0; i <(*res).info.search_info.nb_info; i++){

i_entry_info_free(&(*res).info.search_info.infos[i]);
}
free((*res).info.search_info.infos);
if((*res).info.search_info.part_oq_defined != FALSE){

i_part_oq_free(&(*res).info.search_info.part_oq);
}

i_common_res_free(common_res);
break;

case TRUE: for (i=0; i <(*res).info.uncorr_search_info.nb; i++){

i_search_res_free(&(*res).info.uncorr_search_info.list[i],common_res);
}
free((*res).info.uncorr_search_info.list);
break;
default:break;}
}

void i_add_arg_free(
  ids_add_arg * arg,
  ids_common_arg * common_arg)
{
signed32 i;

i_dist_name_free(&(*arg).object);
for (i=0; i <(*arg).nb_att; i++){

i_att_free(&(*arg).atts[i]);
}
free((*arg).atts);

i_common_arg_free(common_arg);
}

void i_remove_arg_free(
  ids_remove_arg * arg,
  ids_common_arg * common_arg)
{
i_dist_name_free(&(*arg).object);
i_common_arg_free(common_arg);
}

void i_modify_arg_free(
  ids_modify_arg * arg,
  ids_common_arg * common_arg)
{
signed32 i;

i_dist_name_free(&(*arg).object);
for (i=0; i <(*arg).nb_change; i++){

switch ((*arg).changes[i].modif_kind){

case ENTRYMOD_ADD_ATT: i_att_free(&(*arg).changes[i].modif.add_att);
break;
case ENTRYMOD_REMOV_ATT: i_att_type_free(&(*arg).changes[i].modif.rm_att);
break;
case ENTRYMOD_ADD_VAL: i_att_free(&(*arg).changes[i].modif.add_val);
break;
case ENTRYMOD_REMOV_VAL: i_att_free(&(*arg).changes[i].modif.rm_val);
break;
default:break;}
}
free((*arg).changes);

i_common_arg_free(common_arg);
}

void i_modify_rdn_arg_free(
  ids_modify_rdn_arg * arg,
  ids_common_arg * common_arg)
{
i_dist_name_free(&(*arg).object);
i_rdname_free(&(*arg).new_rdn);
if((*arg).delet != FALSE){
}

i_common_arg_free(common_arg);
}

void i_att_error_free(
  ids_att_error * att_error)
{
signed32 i;

i_name_free(&(*att_error).object);
for (i=0; i <(*att_error).nb_pb; i++){


i_att_type_free(&(*att_error).att_pb_list[i].att_type);
if((*att_error).att_pb_list[i].att_value_defined != FALSE){

i_att_value_free((*att_error).att_pb_list[i].att_value);
}
}
free((*att_error).att_pb_list);
}

void i_name_error_free(
  ids_name_error * name_error)
{
i_name_free(&(*name_error).matched);
}


void i_dsa_referral_free(
  ids_dsa_referral * referral)
{
i_cont_reference_free(&(*referral).candidate);
if((*referral).ctx_pref.nb_rdnames != IDS_UNDEFINED){

i_dist_name_free(&(*referral).ctx_pref);
}
}

void i_bind_error_free(
  ids_bind_error * bind_error)
{
if((*bind_error).version != V1988){
}

switch ((*bind_error).is_service_error){
case TRUE: break;
case FALSE: break;
default:break;}
}

