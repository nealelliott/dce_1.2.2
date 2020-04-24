# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: result_check.sh,v $
# Revision 1.1.4.2  1996/03/11  02:06:14  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:14  marty]
#
# Revision 1.1.4.1  1995/12/11  19:49:23  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:25:39  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  18:16:59  root]
# 
# Revision 1.1.2.5  1994/09/23  19:40:21  mori_m
# 	Merged with changes from 1.1.2.4
# 	[1994/09/23  19:40:14  mori_m]
# 
# 	CR 11041/11044
# 	        Deleted the tests based on the wrong assumption (code sets i14y
# 	        cannot be done from the stubs), and added more tests for varying array.
# 	[1994/09/23  18:28:43  mori_m]
# 
# Revision 1.1.2.4  1994/09/23  18:43:29  mori_m
# 	CR 11041/11044
# 	        Deleted the tests based on the wrong assumption (code sets i14y
# 	        cannot be done from the stubs), and added more tests for varying array.
# 	[1994/09/23  18:28:43  mori_m]
# 
# Revision 1.1.2.3  1994/08/02  20:06:55  mori_m
# 	CR 9701:  rpc_binding_copy() takes care of code sets i14y.  Added new
# 		  functional tests: method_copy_fixed.c and tags_copy_fixed.c
# 	[1994/08/02  20:00:18  mori_m]
# 
# Revision 1.1.2.2  1994/06/10  20:43:27  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:15:34  devsrc]
# 
# Revision 1.1.2.1  1994/05/04  21:53:55  mori_m
# 	CR 9701: RPC runtime I18N extension functional test drop.
# 	[1994/05/04  21:28:33  mori_m]
# 
# $EndLog$
#
#!csh
echo i18n_method_fixed_result_file
diff i18n_expected_result i18n_method_fixed_result_file
echo i18n_method_copy_fixed_result_file
diff i18n_expected_result i18n_method_copy_fixed_result_file
echo i18n_method_fixed_cmir_result_file
diff i18n_expected_result i18n_method_fixed_cmir_result_file
echo i18n_method_fixed_smir_result_file
diff i18n_expected_result i18n_method_fixed_smir_result_file
echo i18n_method_conf_result_file        
diff i18n_expected_result i18n_method_conf_result_file        
echo i18n_method_conf_cmir_result_file   
diff i18n_expected_result i18n_method_conf_cmir_result_file   
echo i18n_method_conf_smir_result_file   
diff i18n_expected_result i18n_method_conf_smir_result_file   
echo i18n_method_vary_result_file        
diff i18n_expected_result i18n_method_vary_result_file        
echo i18n_method_vary_cmir_result_file   
diff i18n_expected_result i18n_method_vary_cmir_result_file   
echo i18n_method_vary_smir_result_file   
diff i18n_expected_result i18n_method_vary_smir_result_file   
echo i18n_method_conf_vary_result_file        
diff i18n_expected_result i18n_method_conf_vary_result_file        
echo i18n_method_conf_vary_cmir_result_file   
diff i18n_expected_result i18n_method_conf_vary_cmir_result_file   
echo i18n_method_conf_vary_smir_result_file   
diff i18n_expected_result i18n_method_conf_vary_smir_result_file   
echo i18n_tags_fixed_result_file
diff i18n_expected_result i18n_tags_fixed_result_file
echo i18n_tags_copy_fixed_result_file
diff i18n_expected_result i18n_tags_copy_fixed_result_file
echo i18n_tags_fixed_cmir_result_file
diff i18n_expected_result i18n_tags_fixed_cmir_result_file
echo i18n_tags_fixed_smir_result_file
diff i18n_expected_result i18n_tags_fixed_smir_result_file
echo i18n_tags_conf_result_file
diff i18n_expected_result i18n_tags_conf_result_file
echo i18n_tags_conf_cmir_result_file
diff i18n_expected_result i18n_tags_conf_cmir_result_file
echo i18n_tags_conf_smir_result_file
diff i18n_expected_result i18n_tags_conf_smir_result_file
echo i18n_tags_vary_result_file
diff i18n_expected_result i18n_tags_vary_result_file
echo i18n_tags_vary_cmir_result_file
diff i18n_expected_result i18n_tags_vary_cmir_result_file
echo i18n_tags_vary_smir_result_file
diff i18n_expected_result i18n_tags_vary_smir_result_file
echo i18n_tags_conf_vary_result_file
diff i18n_expected_result i18n_tags_conf_vary_result_file
echo i18n_tags_conf_vary_cmir_result_file
diff i18n_expected_result i18n_tags_conf_vary_cmir_result_file
echo i18n_tags_conf_vary_smir_result_file
diff i18n_expected_result i18n_tags_conf_vary_smir_result_file
