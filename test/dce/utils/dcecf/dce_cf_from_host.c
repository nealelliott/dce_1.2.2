/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dce_cf_from_host.c,v $
 * Revision 1.1.15.2  1996/02/18  00:23:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:18  marty]
 *
 * Revision 1.1.15.1  1995/12/08  22:08:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:43:21  root]
 * 
 * Revision 1.1.13.2  1994/06/10  20:18:49  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:11  devsrc]
 * 
 * Revision 1.1.13.1  1993/12/29  19:22:40  rsalz
 * 	Move dce_cf_XXX from config to dce/utils/dcecf (OT CR 9663).
 * 	[1993/12/29  18:37:29  rsalz]
 * 
 * $EndLog$
 */

main()
{
   char *hostname1 = "hosts/sbushnel";
   char *hostname2 = '\0';
   char *prin_name;
   char *entryname1 = "hosts/sbushnel";
   char *entryname2 = '\0';
   char *entry_name;
   char *profname1 = "hosts/sbushnel";
   char *profname2 = '\0';
   char *prof_name;
   unsigned long status;

   dce_cf_prin_name_from_host(hostname1, &prin_name, &status);
   printf("dce_cf_prin_name_from_host(valid,valid) returns :%s\n",prin_name);

   dce_cf_prin_name_from_host(hostname2, &prin_name, &status);
   printf("dce_cf_prin_name_from_host(NULL,valid) returns :%s\n",prin_name);

   dce_cf_binding_entry_from_host(entryname1, &entry_name, &status);
   printf("dce_cf_binding_entry_from_host(valid,valid) returns :%s\n",entry_name);

   dce_cf_binding_entry_from_host(entryname2, &entry_name, &status);
   printf("dce_cf_binding_entry_from_host(NULL,valid) returns :%s\n",entry_name);

   dce_cf_profile_entry_from_host(profname1, &prof_name, &status);
   printf("dce_cf_profile_entry_from_host(valid,valid) returns :%s\n",prof_name);

   dce_cf_profile_entry_from_host(profname2, &prof_name, &status);
   printf("dce_cf_profile_entry_from_host(NULL,valid) returns :%s\n",prof_name);
}
