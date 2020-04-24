# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: result_check.sh,v $
# Revision 1.1.4.2  1996/03/11  02:06:24  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:50:18  marty]
#
# Revision 1.1.4.1  1995/12/11  19:50:38  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:26:07  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  18:17:28  root]
# 
# Revision 1.1.2.5  1994/09/27  18:47:30  mori_m
# 	CR 11041:  Added new tests for conformant array ops.
# 	[1994/09/27  18:36:39  mori_m]
# 
# Revision 1.1.2.4  1994/09/23  19:31:50  mori_m
# 	Merged with changes from 1.1.2.3
# 	[1994/09/23  19:31:45  mori_m]
# 
# 	CR 11041/11044
# 	        Deleted the tests based on the wrong assumption (code sets i14y
# 	        cannot be done from the stubs), and added more tests for varying array.
# 	[1994/09/23  18:29:19  mori_m]
# 
# Revision 1.1.2.3  1994/09/23  18:43:46  mori_m
# 	CR 11041/11044
# 	        Deleted the tests based on the wrong assumption (code sets i14y
# 	        cannot be done from the stubs), and added more tests for varying array.
# 	[1994/09/23  18:29:19  mori_m]
# 
# Revision 1.1.2.2  1994/06/10  20:43:59  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:15:59  devsrc]
# 
# Revision 1.1.2.1  1994/05/04  21:54:33  mori_m
# 	CR 9701: RPC runtime I18N extension functional test drop.
# 	[1994/05/04  21:35:40  mori_m]
# 
# $EndLog$
#!csh
echo i18n_method_fixed_result_file
diff i18n_expected_result i18n_method_fixed_result_file
echo i18n_method_copy_fixed_result_file
diff i18n_expected_result i18n_method_copy_fixed_result_file
echo i18n_method_vary_result_file
diff i18n_expected_result i18n_method_vary_result_file
echo i18n_tags_fixed_result_file
diff i18n_expected_result i18n_tags_fixed_result_file
echo i18n_tags_copy_fixed_result_file
diff i18n_expected_result i18n_tags_copy_fixed_result_file
echo i18n_tags_fixed_cmir_result_file
diff i18n_expected_result i18n_tags_fixed_cmir_result_file
echo i18n_tags_fixed_smir_result_file
diff i18n_expected_result i18n_tags_fixed_smir_result_file
echo i18n_tags_vary_result_file
diff i18n_expected_result i18n_tags_vary_result_file
echo i18n_tags_vary_cmir_result_file
diff i18n_expected_result i18n_tags_vary_cmir_result_file
echo i18n_tags_vary_smir_result_file
diff i18n_expected_result i18n_tags_vary_smir_result_file
echo i18n_method_conf_result_file
diff i18n_expected_result i18n_method_conf_result_file
echo i18n_method_conf_vary_result_file
diff i18n_expected_result i18n_method_conf_vary_result_file
echo i18n_tags_conf_vary_result_file
diff i18n_expected_result i18n_tags_conf_vary_result_file
echo i18n_tags_conf_vary_cmir_result_file
diff i18n_expected_result i18n_tags_conf_vary_cmir_result_file
echo i18n_tags_conf_vary_smir_result_file
diff i18n_expected_result i18n_tags_conf_vary_smir_result_file
echo i18n_tags_conf_result_file
diff i18n_expected_result i18n_tags_conf_result_file
echo i18n_tags_conf_cmir_result_file
diff i18n_expected_result i18n_tags_conf_cmir_result_file
echo i18n_tags_conf_smir_result_file
diff i18n_expected_result i18n_tags_conf_smir_result_file
