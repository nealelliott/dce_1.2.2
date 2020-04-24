/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dpeaclconvert.c,v $
 * Revision 1.1.7.2  1996/02/18  19:34:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:02  marty]
 *
 * Revision 1.1.7.1  1995/12/08  15:26:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:17  root]
 * 
 * Revision 1.1.5.4  1994/08/25  19:30:47  proulx
 * 	Delegation support.
 * 	[1994/08/25  19:19:24  proulx]
 * 
 * Revision 1.1.5.3  1994/06/30  19:17:47  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:03:57  mccann]
 * 
 * Revision 1.1.5.2  1994/04/29  15:55:22  peckham
 * 	Use dcecdsmac.h macros for dce_svc_printf().
 * 	[1994/04/29  14:31:49  peckham]
 * 
 * Revision 1.1.5.1  1994/03/12  22:08:27  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:34:07  peckham]
 * 
 * Revision 1.1.3.3  1992/12/30  14:16:52  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:16:23  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  18:52:41  devsrc
 * 	[OT 5373] SNI/SVR4 merge UPDATE:  changed all instances of mem_type to memtype.
 * 	[1992/09/23  21:03:26  sekhar]
 * 
 * Revision 1.1  1992/01/19  15:18:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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
/*
**  Copyright (c) 1991 by
**	Hewlett-Packard Company, Palo Alto, Ca. &
**	Digital Equipment Corporation, Maynard, Mass.
**
**  @OSF_COPYRIGHT@
**
*/

#include <dce/dce.h>
#include <dce/assert.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <dpeacl.h>
#include <string.h>

/*
 ** INTERNAL FUNCTION PROTOTYPES
 */
static unsigned int 
determine_acl_size(
		   sec_acl_p_t  ,
		   int 		* );



/* d p e _ a c l _ w i r e _ t o _ f l a t
**
** This function receives as input (wire_acl) a pointer to an initialized acl 
** data structure and returns as output (flat_acl) a pointer to a contiguous 
** acl block representative of the input parameter. 
** The output parameter (size) will correspond to the size of the created acl 
** block (flat_acl). 
** 
** At this time it will be the callers responsibility to both free the input 
** and output data structures. 
** 
** ERROR CODES: 
**      dpe_acl_success 
**      dpe_acl_failure 
**
**
*/
unsigned int 
dpe_acl_wire_to_flat (sec_acl_p_t  wire_acl,
                      int          mem_type,
                      char         **flat_acl,
                      int          *size)
{
   int    		  num,len;
   sec_acl_entry_t      *flat_entry_p,*wire_entry_p;
   sec_acl_t		*flat_acl_p;
   sec_acl_extend_info_t *extend_p;
   char                  *tmp_p;


   DCE_SVC_DEBUG((
       cds__svc_handle,
       cds_svc_library,
       svc_c_debug2,
       " >dpe_acl_flat_to_wire: wire_acl=0x%x",
       wire_acl));

   /*
   ** Determine acl size
   */
   determine_acl_size(wire_acl, size); 

   /*
   ** Malloc space for acl
   */
   *flat_acl = DPE_ACL_MALLOC (*size, char *, mem_type);
   flat_acl_p = (sec_acl_t *)*flat_acl;
   if (*flat_acl == NULL)
   {
       DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_library,
           svc_c_debug2,
           " <dpe_acl_flat_to_wire: return(dpe_acl_failure=%d)",
           dpe_acl_failure));

       return(dpe_acl_failure);
   }
   


   /*
   ** Copy acl base structure (sec_acl_t) into flat acl
   ** (44 bytes).
   */
   *flat_acl_p = *wire_acl;

   flat_entry_p = (sec_acl_entry_t *)(flat_acl_p + 1);
   flat_entry_p = (sec_acl_entry_t *)ALIGN_PTR(flat_entry_p);
   /*
   ** Copy acl entries as is into flat buffer 
   */
   memcpy((void *)flat_entry_p,
          (void *)wire_acl->sec_acl_entries, 
	  (size_t)(sizeof(sec_acl_entry_t) * wire_acl->num_entries));

   /*
   ** Set up pointer to storage area for variable length data
   */
   tmp_p = (char *)(flat_entry_p + wire_acl->num_entries);

   /* 
   ** Copy default realm first, advance ptr.
   */
   if (wire_acl->default_realm.name) {
     flat_acl_p->default_realm.name = (idl_char *)(tmp_p - *flat_acl);
     strcpy(tmp_p, (char *)(wire_acl->default_realm.name));
     tmp_p += strlen((char *)(wire_acl->default_realm.name));
     tmp_p += sizeof (char);
   } else
     flat_acl_p->default_realm.name = NULL;

   /*
   ** Copy pointed to data to the flat acl. 
   ** An acl entry has the following fields:
   **
   ** entry class ............ sec_acl_entry_class_t (4)
   ** params ................. sec_acl_permset_t     (4)
   ** entry_type ............. sec_acl_entry_type_t  (4)
   **
   ** id ..................... sec_id_t              [20]
   **     name ............... char *
   **
   ** foreign_id ............. sec_id_foreign_t      [40]
   **     foreign_id_name .... char *
   **     foreign_realm_name . char *
   **
   ** extension_info ......... pointer               [4]
   **     variable length array
   **
   */

   /*
   ** index into entry array
   */
   for (wire_entry_p = wire_acl->sec_acl_entries, num = 0;
        num < wire_acl->num_entries; ++num, ++wire_entry_p, ++flat_entry_p) 
     {
	 /*
	  ** Copy pointer data associated with entry  
	  */
 
       switch (wire_entry_p->entry_info.entry_type){
       
        case sec_acl_e_type_extended :
	  /*
	   ** Copy variable length extended info array to flat acl
	   ** This structure should be aligned
	   */
	  tmp_p = (char *)ALIGN_PTR(tmp_p);
	  flat_entry_p->entry_info.tagged_union.extended_info = 
	    (sec_acl_extend_info_t *)(tmp_p - *flat_acl);

	  /*
	   ** compute size of extend_info array
	   */
	  extend_p = wire_entry_p->entry_info.tagged_union.extended_info;
	  len = sizeof(sec_acl_extend_info_t) + 
	    (extend_p->num_bytes * sizeof(idl_byte));
	  
	  memcpy((void *)tmp_p, (void *)extend_p, (size_t)len);
	  
	  tmp_p += len;
	  break;

        case sec_acl_e_type_user:
        case sec_acl_e_type_group:
        case sec_acl_e_type_foreign_other:
        case sec_acl_e_type_user_deleg:
        case sec_acl_e_type_group_deleg:
        case sec_acl_e_type_for_other_deleg:
	  /*
	   ** copy id name to flat acl 
	   */
	  if (wire_entry_p->entry_info.tagged_union.id.name) {
	      flat_entry_p->entry_info.tagged_union.id.name = 
	       	                            (idl_char *) (tmp_p - *flat_acl);
	      strcpy(tmp_p, (char *)(wire_entry_p->
                                            entry_info.tagged_union.id.name)); 
	      tmp_p += strlen(tmp_p) + sizeof(idl_char);
	  } else
	      flat_entry_p->entry_info.tagged_union.id.name = NULL;

	  break;
        case sec_acl_e_type_foreign_user :
        case sec_acl_e_type_foreign_group :
        case sec_acl_e_type_for_user_deleg :
        case sec_acl_e_type_for_group_deleg :
	  /*
	   ** copy foreign id name offset to flat acl 
	   */
	  flat_entry_p->entry_info.tagged_union.foreign_id.id.name =
	    (idl_char *)(tmp_p - *flat_acl);
	  if (wire_entry_p->entry_info.tagged_union.foreign_id.id.name) {
	      flat_entry_p->entry_info.tagged_union.foreign_id.id.name =
		(idl_char *)(tmp_p - *flat_acl);
	      strcpy(tmp_p, 
		     (char *)(wire_entry_p->
                                  entry_info.tagged_union.foreign_id.id.name)); 
	  
	      tmp_p += strlen(tmp_p) + sizeof(idl_char);
	  }
	  else 	 
	    flat_entry_p->entry_info.tagged_union.foreign_id.id.name = NULL;
                
	  /*
	   ** copy foreign realm name to flat acl 
	   */
              
	  if (wire_entry_p->entry_info.tagged_union.foreign_id.realm.name) {
	      flat_entry_p->entry_info.tagged_union.foreign_id.realm.name = 
		(idl_char *)(tmp_p - *flat_acl);
	      strcpy(tmp_p, 
		     (char *)(wire_entry_p->
                               entry_info.tagged_union.foreign_id.realm.name)); 
	      tmp_p += strlen(tmp_p) + sizeof(idl_char);
	  } else
	  flat_entry_p->entry_info.tagged_union.foreign_id.realm.name = NULL;
	  break;

      default:
	  break;
	  
      } /* end switch */
  
   } /* end for */
   
   dce_assert(cds__svc_handle, tmp_p <= (*size + (char *)*flat_acl));
   
   DCE_SVC_DEBUG((
       cds__svc_handle,
       cds_svc_library,
       svc_c_debug2,
       " <dpe_acl_flat_to_wire: @ end - return(dpe_acl_success=%d)",
       dpe_acl_success));
   
   return(dpe_acl_success);
  
}




/* d p e _ a c l _ f l a t _ t o _ w i r e
**
** This function receives as input (flat_acl) a pointer to a contiguous acl 
** structure and returns as an output (wire_acl) a pointer to an initialized 
** acl data structure.
** 
** At this time it will be the callers responsibility to both free the input 
** and output data structures. 
** 
** ERROR CODES: 
**      dpe_acl_success 
**      dpe_acl_failure 
**
*/

unsigned int 
dpe_acl_flat_to_wire (char         *flat_acl,
                      int          mem_type,
                      sec_acl_p_t  *new_wire_acl)
{  
   int                   num;
   int                   size;
   sec_acl_extend_info_t *extend_p;
   sec_acl_t             *wire_acl_p, *flat_acl_p;
   sec_acl_entry_t       *wire_entry_p, *flat_entry_p;
   char                  *tmp_p;


   DCE_SVC_DEBUG((
       cds__svc_handle,
       cds_svc_library,
       svc_c_debug2,
       " >dpe_acl_flat_to_wire"));

   /*
   ** Allocate a sec_acl_t block
   */
   *new_wire_acl = 
     (sec_acl_p_t)DPE_ACL_MALLOC (sizeof(sec_acl_t), sec_acl_p_t, mem_type);
   if (*new_wire_acl == NULL)
   {
       DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_library,
           svc_c_debug2,
           " <dpe_acl_flat_to_wire: (1) return(dpe_acl_failure)"));

       return(dpe_acl_failure);
   }
   
   /*
   ** Assign wire_acl to address on new_wire acl pointer
   */
   wire_acl_p = *new_wire_acl;
   flat_acl_p = (sec_acl_t *)flat_acl;

   *wire_acl_p = *flat_acl_p;

   /* 
   ** Allocate space for sec_acl_entry_t array 
   */
   wire_acl_p->sec_acl_entries =
         DPE_ACL_MALLOC ((sizeof(sec_acl_entry_t) * flat_acl_p->num_entries),
		         sec_acl_entry_t *,
	                 mem_type);
   if (wire_acl_p->sec_acl_entries == NULL)
   {
         DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_library,
           svc_c_debug2,
           " <dpe_acl_flat_to_wire: (2) return(dpe_acl_failure)"));

       return(dpe_acl_failure);
   }       

   /*
   ** Copy acl entries from flat to wire, fix up ptrs later
   */
   flat_entry_p = (sec_acl_entry_t *)(flat_acl_p + 1);
   flat_entry_p = (sec_acl_entry_t *)ALIGN_PTR(flat_entry_p);
   wire_entry_p = wire_acl_p->sec_acl_entries;

   memcpy((void *)wire_entry_p,
          (void *)flat_entry_p, 
	  (size_t)(sizeof(sec_acl_entry_t) * flat_acl_p->num_entries));

   tmp_p = (char *)(flat_entry_p + flat_acl_p->num_entries);

   /* 
   ** Allocate a realm_name block 
   */ 
   if (flat_acl_p->default_realm.name) {
       size = strlen(tmp_p) + sizeof(idl_char);
       wire_acl_p->default_realm.name = 
	 DPE_ACL_MALLOC(size, idl_char *, mem_type); 
       if (wire_acl_p->default_realm.name == NULL)
       {
	   DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_library,
              svc_c_debug2,
              " <dpe_acl_flat_to_wire: (3) return(dpe_acl_failure)"));

	   return(dpe_acl_failure);
       }
       
       strcpy ((char *)(wire_acl_p->default_realm.name), tmp_p);
       tmp_p += size;
   } else
     wire_acl_p->default_realm.name = NULL;

   /*
   ** 
   ** Index down flat acl retrieving variable length data
   **
   */

   for (num=0;
        num <  flat_acl_p->num_entries; ++num, ++flat_entry_p, ++wire_entry_p)
   {
    
       switch (flat_entry_p->entry_info.entry_type) {
       
         case sec_acl_e_type_extended :

	     DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_library,
		 svc_c_debug5,
		 "dpe_acl_flat_to_wire: Extension Block found"));

	   /*
	    ** Get extension block
	    */ 
	   tmp_p = (char *)ALIGN_PTR(tmp_p);
	   extend_p = (sec_acl_extend_info_t *)tmp_p;
	   size = sizeof(sec_acl_extend_info_t) + 
	     (extend_p->num_bytes * sizeof(idl_byte));

            /*
            ** Allocate space for sec_acl_extend_info_t block
            */
            wire_entry_p->entry_info.tagged_union.extended_info = 
                   DPE_ACL_MALLOC (size,
				   sec_acl_extend_info_t *,
				   mem_type); 
	     if (wire_entry_p->entry_info.tagged_union.extended_info == NULL)
	     {
		 DCE_SVC_DEBUG((
		     cds__svc_handle,
		     cds_svc_library,
		     svc_c_debug2,
  		     " <dpe_acl_flat_to_wire: (4) return(dpe_acl_failure)"));

		 return(dpe_acl_failure);
	     }
	     

            /*
            ** Copy flat acl extension block into wire acl
            */
	   memcpy((void *)wire_entry_p->entry_info.tagged_union.extended_info,
		  (void *)tmp_p,
                  (size_t)size);
	   tmp_p += size;
	   break;
        
        case sec_acl_e_type_user:
        case sec_acl_e_type_group:
        case sec_acl_e_type_foreign_other:
        case sec_acl_e_type_user_deleg:
        case sec_acl_e_type_group_deleg:
        case sec_acl_e_type_for_other_deleg:
	   /*
            ** Allocate space for id name
            */
	   if (flat_entry_p->entry_info.tagged_union.id.name) {
	       size = strlen(tmp_p) + sizeof(idl_char );
	       wire_entry_p->entry_info.tagged_union.id.name =
			    DPE_ACL_MALLOC(size, idl_char *, mem_type);
	       if (wire_entry_p->entry_info.tagged_union.id.name == NULL)
	       {
		   DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_library,
		       svc_c_debug2,
		       " <dpe_acl_flat_to_wire: (5) return(dpe_acl_failure)"));

		   return(dpe_acl_failure);
	       }
	       
	       strcpy((char *)(wire_entry_p->entry_info.tagged_union.id.name),
                      tmp_p);
	       tmp_p += size;
	   } else
	     wire_entry_p->entry_info.tagged_union.id.name = NULL;
            break;
        case sec_acl_e_type_foreign_user:
        case sec_acl_e_type_foreign_group:
        case sec_acl_e_type_for_user_deleg:
        case sec_acl_e_type_for_group_deleg:

	    DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_library,
		 svc_c_debug5,
		 "dpe_acl_flat_to_wire: Foreign Block found"));

	   /*
            ** Allocate space for foreign id name
            */
	   if (flat_entry_p->entry_info.tagged_union.foreign_id.id.name) {
	       size = strlen(tmp_p) + sizeof(idl_char);
	       wire_entry_p->entry_info.tagged_union.foreign_id.id.name =
			    DPE_ACL_MALLOC (size, idl_char *, mem_type);
	       if (wire_entry_p->
		   entry_info.tagged_union.foreign_id.id.name == NULL)
	       {
		   DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_library,
		       svc_c_debug2,
		       " <dpe_acl_flat_to_wire: (6) return(dpe_acl_failure)"));

		   return(dpe_acl_failure);
	       }
	       
	       strcpy((char *)(wire_entry_p->
	                          entry_info.tagged_union.foreign_id.id.name),
		      tmp_p);
	       tmp_p += size;
	   } else
	     wire_entry_p->entry_info.tagged_union.foreign_id.id.name = NULL;

            /*
            ** Allocate space for foreign realm name
            */
	   if (flat_entry_p->entry_info.tagged_union.foreign_id.realm.name) {
	       size = strlen(tmp_p) + sizeof(idl_char);
	       wire_entry_p->entry_info.tagged_union.foreign_id.realm.name =
                    DPE_ACL_MALLOC (size, idl_char *, mem_type);
	       if (wire_entry_p->
		   entry_info.tagged_union.foreign_id.realm.name == NULL)
	       {
		   DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_library,
		       svc_c_debug2,
		       " <dpe_acl_flat_to_wire: (7) return(dpe_acl_failure)"));

	           return(dpe_acl_failure);
	       }
	       
	       strcpy((char *)(wire_entry_p->
                               entry_info.tagged_union.foreign_id.realm.name),
	  	      tmp_p);
	       tmp_p += size;
	   } else
	     wire_entry_p->entry_info.tagged_union.foreign_id.realm.name =NULL;
            break;                       

	 default:
	     break;
	     
        } /* end switch */

   } /* end for */

   DCE_SVC_DEBUG((
       cds__svc_handle,
       cds_svc_library,
       svc_c_debug2,
       " <dpe_acl_flat_to_wire: @ end - return(dpe_acl_success)"));

   return(dpe_acl_success);
}





/* d p e _ a c l _ w i r e _ t o _ w i r e
**
** This function receives as input (wire_acl1) a pointer to an initialized acl 
** structure and returns as output (wire_acl2) a pointer to an initialized 
** acl data structure which was copied from the first.
** 
** At this time it will be the callers responsibility to both free the input 
** and output data structures. 
** 
** ERROR CODES: 
**      dpe_acl_success 
**      dpe_acl_failure 
**
*/

unsigned int 
dpe_acl_wire_to_wire (sec_acl_p_t  wire_acl1,
                      int          mem_type,
                      sec_acl_p_t  *wire_acl2)
{
   int           		  name_size;
   int           		  name_size2;
   sec_acl_t			* wire_acl;
   sec_acl_entry_t		* acl_entry_ptr;
   int                            num;
   char 			* ptr;
   sec_acl_t  			* tmp_acl_ptr; 
   size_t			  extend_info_size;
   sec_acl_extend_info_t	* extend_blk_ptr;


   DCE_SVC_DEBUG((
       cds__svc_handle,
       cds_svc_library,
       svc_c_debug2,
       " >dpe_acl_wire_to_wire"));

   /*
   ** Allocate a sec_acl_t block
   */
   *wire_acl2= DPE_ACL_MALLOC (sizeof(sec_acl_t), sec_acl_p_t, mem_type);
   if (*wire_acl2 == NULL)
   {
       DCE_SVC_DEBUG((
         cds__svc_handle,
         cds_svc_library,
         svc_c_debug2,
         " <dpe_acl_wire_to_wire: (1) return(dpe_acl_failure)"));

      return(dpe_acl_failure);
   }

   /*
   ** Assign wire_acl to address on new_wire acl pointer
   */
   wire_acl = * wire_acl2;

 
   /*
   ** Assign flat acl pointer to a sec_acl_t pointer
   */
   tmp_acl_ptr = (sec_acl_p_t) wire_acl1;



   /*
   ** Copy sec_acl_t from flat acl to wire
   */
   memcpy((void *)wire_acl,
          (void *)tmp_acl_ptr,
          sizeof(sec_acl_t));  

   /* 
   ** Allocate a realm_name block 
   */ 
   if (tmp_acl_ptr->default_realm.name) {
       name_size = strlen((char *)tmp_acl_ptr->default_realm.name) + 
	 sizeof(idl_char);
       wire_acl->default_realm.name = DPE_ACL_MALLOC (name_size,
						  idl_char *,
						  mem_type); 
       if (wire_acl->default_realm.name == NULL)
       {
	   DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_library,
              svc_c_debug2,
              " <dpe_acl_wire_to_wire: (2) return(dpe_acl_failure)"));

	   return(dpe_acl_failure);
       }
       

       /*
	** Copy Realm_name from flat_acl to wire_acl 
	*/

       strcpy ((char *)(wire_acl->default_realm.name),
	       (char *)(tmp_acl_ptr->default_realm.name));
   };

   /* 
   ** Allocate space for sec_acl_entry_t array 
   */
   wire_acl->sec_acl_entries =
           DPE_ACL_MALLOC((sizeof(sec_acl_entry_t) * tmp_acl_ptr->num_entries),
			  sec_acl_entry_t *,
			  mem_type);
   if (wire_acl->sec_acl_entries == NULL)
   {
       
       DCE_SVC_DEBUG((
         cds__svc_handle,
         cds_svc_library,
         svc_c_debug2,
         " <dpe_acl_wire_to_wire: (3) return(dpe_acl_failure)"));

       return(dpe_acl_failure);
   }
   

   /*
   ** Point to first entry in wire_acl1 array 
   */
   ptr = (char *) tmp_acl_ptr->sec_acl_entries;  

   /*
   ** 
   ** Index down wire_acl1 retrieving acl entries
   **
   */

   for (acl_entry_ptr = (sec_acl_entry_t *)wire_acl->sec_acl_entries, num=0; 
            num < ((unsigned32) tmp_acl_ptr->num_entries); 
                acl_entry_ptr++, num++)  
   {
       /*
	** Copy sec_acl_entry from wire_acl1 to wire acl2
	*/ 
       memcpy((void *)acl_entry_ptr, (void *)ptr, sizeof(sec_acl_entry_t));

       switch (((sec_acl_entry_t *)ptr)->entry_info.entry_type) {
       
         case sec_acl_e_type_extended :

	     DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_library,
		 svc_c_debug5,
		 "dpe_acl_wire_to_wire: Extension Block found"));

	   /*
	    ** Get extension block
	    */ 
	   extend_blk_ptr =  
	     (((sec_acl_entry_t *)ptr)->entry_info.tagged_union.extended_info);
            
	   extend_info_size  =  sizeof(uuid_t)                +
	     sizeof(ndr_format_t)          +
	       sizeof(unsigned32)            +
		 (extend_blk_ptr->num_bytes * sizeof(idl_byte));
	   /*
            ** Allocate space for sec_acl_extend_info_t block
            */
	   acl_entry_ptr->entry_info.tagged_union.extended_info = 
	     DPE_ACL_MALLOC (extend_info_size,
			     sec_acl_extend_info_t *,
			     mem_type); 
	     if (acl_entry_ptr->entry_info.tagged_union.extended_info == NULL)
	     {
		 DCE_SVC_DEBUG((
		     cds__svc_handle,
		     cds_svc_library,
		     svc_c_debug2,
		     " <dpe_acl_wire_to_wire: (4) return(dpe_acl_failure)"));

		 return(dpe_acl_failure);
	     }
	     

	   /*
            ** Copy flat acl extension block into wire acl
            */
	   memcpy((void *)(acl_entry_ptr->
                                       entry_info.tagged_union.extended_info),
	          (void *)(((sec_acl_entry_t *)ptr)->
                                       entry_info.tagged_union.extended_info),
                  (size_t)extend_info_size); 

	   break;
        case sec_acl_e_type_user:
        case sec_acl_e_type_group:
        case sec_acl_e_type_foreign_other:
        case sec_acl_e_type_user_deleg:
        case sec_acl_e_type_group_deleg:
        case sec_acl_e_type_for_other_deleg:
	   /*
            ** Allocate space for id name
            */
	   if (!((sec_acl_entry_t *)ptr)->entry_info.tagged_union.id.name) 
	       break;
	   name_size = strlen((char *)(((sec_acl_entry_t *)ptr)->
                                            entry_info.tagged_union.id.name))+
	               sizeof(idl_char);
	   acl_entry_ptr->entry_info.tagged_union.id.name =
	                      DPE_ACL_MALLOC (name_size, idl_char *, mem_type);
	     if (acl_entry_ptr->entry_info.tagged_union.id.name == NULL)
	     {
		  DCE_SVC_DEBUG((
		     cds__svc_handle,
		     cds_svc_library,
		     svc_c_debug2,
		     " <dpe_acl_wire_to_wire: (5) return(dpe_acl_failure)"));

		 return(dpe_acl_failure);
	     }
	     

	   strcpy ((char *)(acl_entry_ptr->entry_info.tagged_union.id.name),
		   (char *)(((sec_acl_entry_t *)ptr)->
                                         entry_info.tagged_union.id.name));
	   break;
        case sec_acl_e_type_foreign_user:
        case sec_acl_e_type_foreign_group:
        case sec_acl_e_type_for_user_deleg:
        case sec_acl_e_type_for_group_deleg:

	   DCE_SVC_DEBUG((
	       cds__svc_handle,
	       cds_svc_library,
	       svc_c_debug5,
	       "dpe_acl_wire_to_wire: Foreign Block found"));

	   /*
            ** Allocate space for foreign id name
            */
	   if (!((sec_acl_entry_t *)ptr)->
                                 entry_info.tagged_union.foreign_id.id.name)
	       break;
	   name_size = strlen((char *)(((sec_acl_entry_t *)ptr)->
                                 entry_info.tagged_union.foreign_id.id.name)) +
                       sizeof(idl_char);
	   acl_entry_ptr->entry_info.tagged_union.foreign_id.id.name =
	                      DPE_ACL_MALLOC (name_size, idl_char *, mem_type);
	   if (acl_entry_ptr->entry_info.tagged_union.foreign_id.id.name ==
	       NULL)
	   {
	       DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_library,
		   svc_c_debug2,
		   " <dpe_acl_wire_to_wire: (6) return(dpe_acl_failure)"));

	       return(dpe_acl_failure);
	   }
	     
	   strcpy((char *)(acl_entry_ptr->
                                  entry_info.tagged_union.foreign_id.id.name),
		  (char *)(((sec_acl_entry_t *)ptr)->
                                  entry_info.tagged_union.foreign_id.id.name));
	   
	   /*
            ** Allocate space for foreign realm name
            */
	   if (!((sec_acl_entry_t *)ptr)->entry_info.tagged_union.foreign_id.realm.name) break;
	   name_size2 = strlen((char *)(((sec_acl_entry_t *)ptr)->
                              entry_info.tagged_union.foreign_id.realm.name)) +
                        sizeof(idl_char);
	   acl_entry_ptr->entry_info.tagged_union.foreign_id.realm.name =
	                    DPE_ACL_MALLOC (name_size2, idl_char *, mem_type);
	   if (acl_entry_ptr->entry_info.tagged_union.foreign_id.realm.name ==
	       NULL)
	   {
	        DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_library,
		   svc_c_debug2,
		   " <dpe_acl_wire_to_wire: (7) return(dpe_acl_failure)"));

	       return(dpe_acl_failure);
	   }
                  
	  strcpy ((char *)(acl_entry_ptr->
	                       entry_info.tagged_union.foreign_id.realm.name),
		  (char *)(((sec_acl_entry_t *)ptr)->
	                       entry_info.tagged_union.foreign_id.realm.name));
	   break;      

	 default:
	     break;                 

        } /* end switch */
       ptr = ptr + sizeof(sec_acl_entry_t);
   } /* end for */

   DCE_SVC_DEBUG((
       cds__svc_handle,
       cds_svc_library,
       svc_c_debug2,
       " <dpe_acl_wire_to_wire: @ end - return(dpe_acl_success)"));

   return(dpe_acl_success);
 }


static unsigned int 
determine_acl_size (sec_acl_p_t  wire_acl,
                    int          *size)
{
   int			acl_base  = 0;
   int			count;
   sec_acl_entry_t	* ptr;
   sec_acl_t            acl;
   sec_acl_t            *acl_p = &acl+1;


   DCE_SVC_DEBUG((
       cds__svc_handle,
       cds_svc_library,
       svc_c_debug2,
       " >determine_acl_size"));

   /*
   ** size of ACL base structure
   */
   if (wire_acl->default_realm.name) 
       acl_base  = strlen ((char *)(wire_acl->default_realm.name)) +
                   sizeof(idl_char);
   acl_base += sizeof(sec_acl_t) + 
          (sizeof(sec_acl_entry_t) * wire_acl->num_entries);

   acl_base += ((char *)ALIGN_PTR(acl_p) - (char *)acl_p);

   /*
   ** Step through entries to determine length of pointed to data
   */ 
   for (ptr = wire_acl->sec_acl_entries, count=0;
        count < wire_acl->num_entries; count++, ptr++)
   {
      
      /*
      **
      ** Select on entry type
      **
      */
      switch (ptr->entry_info.entry_type){
      	case sec_acl_e_type_extended :
	  /* This is the only varialbe data that may need to be aligned
	   * to be accessed directly
           */
	  acl_base += sizeof(sec_acl_extend_info_t) + ALIGN_SIZE +
	    ((ptr->entry_info.tagged_union.extended_info->num_bytes) *
	     sizeof(idl_byte));
	  break;
        case sec_acl_e_type_foreign_user :
        case sec_acl_e_type_foreign_group :
        case sec_acl_e_type_for_user_deleg :
        case sec_acl_e_type_for_group_deleg :
	    if (ptr->entry_info.tagged_union.foreign_id.id.name)
		acl_base += strlen((char *)(ptr->
                                 entry_info.tagged_union.foreign_id.id.name)) +
		            sizeof(idl_char);

	  if (ptr->entry_info.tagged_union.foreign_id.realm.name)
	      acl_base += 
		  strlen((char *)(ptr->
                             entry_info.tagged_union.foreign_id.realm.name)) +
		         sizeof(idl_char);

	  break;
        case sec_acl_e_type_user:
        case sec_acl_e_type_group:
        case sec_acl_e_type_foreign_other:
        case sec_acl_e_type_user_deleg:
        case sec_acl_e_type_group_deleg:
        case sec_acl_e_type_for_other_deleg:
	  if (ptr->entry_info.tagged_union.id.name)
	      acl_base += strlen((char *)(ptr->
                                            entry_info.tagged_union.id.name)) +
		          sizeof(idl_char);
	default :
	  break;
      }
    }
   
   DCE_SVC_DEBUG((
       cds__svc_handle,
       cds_svc_library,
       svc_c_debug5,
       "determine_acl_size: Flat acl size  %d", acl_base));

   *size = acl_base;

   DCE_SVC_DEBUG((
       cds__svc_handle,
       cds_svc_library,
       svc_c_debug2,
       " <determine_acl_size: @ end - return(dpe_acl_success=%d)",
       dpe_acl_success));

   return(dpe_acl_success);
}


/*  DEC/CMS REPLACEMENT HISTORY, Element DPEACLCONVERT.C */
/*  *8    18-NOV-1991 17:46:21 COMUZZI "Fixed lint problem" */
/*  *7    18-NOV-1991 17:33:49 COMUZZI "Merged in CDS changes" */
/*  *6    11-NOV-1991 16:21:24 COMUZZI "Fixed more lint" */
/*  *5     6-NOV-1991 22:10:56 COMUZZI "Fix lint problems" */
/*  *4    15-OCT-1991 16:23:19 B_BAILEY "get Janet's fixes for the alignment problem" */
/*  *3    23-SEP-1991 13:32:00 B_BAILEY "Add switch to select malloc or rpc_ss_allocate" */
/*  *2    18-SEP-1991 14:50:46 B_BAILEY "fix the header comments" */
/*  *1    18-SEP-1991 14:19:25 B_BAILEY "Acl storage conversion" */
/*  DEC/CMS REPLACEMENT HISTORY, Element DPEACLCONVERT.C */
