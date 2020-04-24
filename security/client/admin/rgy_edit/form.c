/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: form.c,v $
 * Revision 1.1.4.2  1996/03/11  13:27:03  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:58  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:00:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:19  root]
 * 
 * Revision 1.1.2.2  1992/12/29  12:40:52  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:12:37  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:45:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  form.c V=10 04/29/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit kernel form management routines 
** 
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)form.c	10 - 04/29/91";
#endif

#include <form.h>
#include <un_maloc.h>
#include <bstring.h>
#include <macros.h>

#define NULL_BUFFER 0

PRIVATE void local_init_form
#ifndef __STDC__
    ( new_form ) 
    form_t  *new_form;
#else
  (
    form_t  *new_form
  )
#endif
{

    form_field_offset_t  cur_field;

    for (cur_field = 0; cur_field < new_form->num_fields; cur_field++) {
	new_form->fields[cur_field].buff = NULL_BUFFER;
	new_form->fields[cur_field].buffsize = 0;
	new_form->fields[cur_field].data_len = 0;
	new_form->fields[cur_field].attributes = 0;
	new_form->fields[cur_field].field_status = form_field_no_data;
	new_form->fields[cur_field].validate =
	    (form_validate_t) form_null_function;
    }	
}


/* FUNCTION form_real_alloc
 *
 * Allocates space for  form with "num_fields" fields, and a header with 
 * "header_size" bytes, and returns a pointer to the the allocated form.  If a 
 * form can't be  "form_null_form" is returned.
 */
PUBLIC form_t *form_real_alloc
#ifndef __STDC__
    ( num_fields, header_size ) 
    long int   num_fields;
    long int   header_size;
#else
  (
    long int   num_fields,
    long int   header_size
  )
#endif
{
    form_t *alloc_form;


    alloc_form = (form_t *)malloc(sizeof(form_t));
    if (alloc_form == 0)
	return  (form_t *)form_null_form;
    
        /* allocate header space unless the header_size parameter is null */
    if (header_size != form_null_header) {
	alloc_form->header = (Pointer) malloc((unsigned int)header_size);
	if (alloc_form->header == 0) {
	    free((char *)alloc_form);
	    return  (form_t *)form_null_form;
	} 
    } else
	alloc_form->header = form_null_header;

        /* alllocate space for the fields */
    alloc_form->fields = (form_field_t *) malloc((unsigned) 
                                (num_fields * sizeof(form_field_t)));
    if (alloc_form->fields == 0) {
	free ((char *)alloc_form->header);
	free((char *)alloc_form);
	return (form_t *)form_null_form;
    }

    alloc_form->hdr_len = header_size;
    alloc_form->num_fields = num_fields;
    local_init_form (alloc_form);
    return alloc_form;
}


/* FUNCTION form_release
 *
 * Frees all storage associated with a form. 
 */
PUBLIC void form_release
#ifndef __STDC__
    ( form ) 
    form_t  *form;
#else
  (
    form_t  *form
  )
#endif
{
    form_field_offset_t cur_field;
#ifdef DEBUG
    char *zz;
#   define FREE(x) free(x); zz = malloc(4); free(zz)
#else
#   define FREE(x) free(x)
#endif

    if (form != form_null_form) {
	    /* free all the field buffers */
	for (cur_field = 0; cur_field < form->num_fields; cur_field++) {
	    if (form->fields[cur_field].buff != NULL_BUFFER)
		FREE (form->fields[cur_field].buff);
	}
	if (form->header != form_null_header)    /* FREE the header */
	    FREE ((char *)form->header);
	FREE ((char *)form->fields);              /* FREE the fields */
	FREE ((char *)form);                      /* FREE the form */
    }
}

	

/* FUNCTION form_write_header
 *
 * writes the data pointed to by "header_info" into the header space of the form
 * pointed to by "form".  Header length is constant throughout the life of a form
 * and is established by the user with the form_alloc call, so we don't need
 * a header length parameter.   The caller had better remember what the header
 * length is.
 */
PUBLIC void form_write_header
#ifndef __STDC__
    ( form, header_info ) 
    form_t   *form;
    Pointer  header_info;
#else
  (
    form_t   *form,
    Pointer  header_info
  )
#endif
{
    bcopy ((char *)header_info, (char *)(form->header), (int)(form->hdr_len));
}



/* FUNCTION form_read_header
**
** does just the opposite of form_write_header.  
*/
void form_read_header
#ifndef __STDC__
    (form, header_info) 
    form_t  *form;
    Pointer  header_info;
#else
  (
    form_t  *form,
    Pointer  header_info
  )
#endif
{
    bcopy ((char *)(form->header), (char *)header_info, (int)(form->hdr_len));
}


/* FUNCTION form_num_fields
 *
 * Returns the number of fields in "form" in the "num_fields" parameter.  Useful 
 * for doing range checks on form_field_offset parameters before making any calls
 * that require a field offset.  
 * 
 */
PUBLIC void form_num_fields
#ifndef __STDC__
    ( form, num_fields ) 
    form_t  *form  ;
    long int *num_fields ;
#else
  (
    form_t  *form  ,
    long int *num_fields 
  )
#endif
{
    *num_fields = form->num_fields;
};


/* FUNCTION form_clear
 *
 *  destroys the data in all fields of a form.
 *
 */
PUBLIC void form_clear
#ifndef __STDC__
    ( form ) 
    form_t *form;
#else
  (
    form_t *form
  )
#endif
{
    form_field_offset_t cur_field;

    for (cur_field = 0; cur_field < form->num_fields; cur_field++) {
	form->fields[cur_field].data_len = 0;
	form->fields[cur_field].field_status = form_field_no_data;
    }

}
	


/* FUNCTION form_init_field
 *
 * Sets a field's attribute bitset and validation function to user_supplied
 * values.  Doesn't check the field offset for validity.  If the caller isn't
 * sure what they're doing they can use the form_num_fields routine to 
 * determine if the field offset is valid before calling form_init_field.
 */
PUBLIC void form_init_field
#ifndef __STDC__
    ( form, field_offset, field_attributes, validation_func ) 
    form_t                  *form;
    form_field_offset_t     field_offset;
    form_bitset_t           field_attributes;
    form_validate_t         validation_func;
#else
  (
    form_t                  *form,
    form_field_offset_t     field_offset,
    form_bitset_t           field_attributes,
    form_validate_t         validation_func
  )
#endif
{
	form->fields[field_offset].attributes = field_attributes;
	form->fields[field_offset].validate = validation_func;
}


PUBLIC void form_real_write
#ifndef __STDC__
    ( form, field_offset, data, data_len, validate, status )    
    form_t               *form;
    form_field_offset_t  field_offset;
    char                 *data;
    long int             data_len;
    boolean32            validate;
    error_status_t       *status;
#else
  (
    form_t               *form,
    form_field_offset_t  field_offset,
    char                 *data,
    long int             data_len,
    boolean32            validate,
    error_status_t       *status
  )
#endif
{

    boolean32            validated;

        /* local variables */
    int i;
    char *buff_ptr;
    form_field_t  *write_field;   /* for convenient field access */

    SET_STATUS(status, error_status_ok);

    write_field = &(form->fields[field_offset]);


       /* if the field has a validation function, AND the caller want's
	* validation then do it
	*/
    if ((write_field->validate != (form_validate_t) form_null_function) &&
	(validate == true)) {
	*status = (*(write_field->validate)) (form, data);
	if (BAD_STATUS(status))         /* r  etn if data is invalid */
	    return;
        else 
	    validated= true;
        data_len = strlen(data);        /* in case validate rtn expanded it */
    } else
	validated = false;

        /* if we got this far then the data is good, so store it in the
	 * field buffer
	 */

        /* if the existing buffer length is too small then reallocate */
    if (write_field->buffsize < data_len + 1) {
	if (write_field->buff != NULL_BUFFER) {
	    free (write_field->buff);
	}
        write_field->buff = malloc((unsigned)(data_len + 1));
	write_field->buffsize = data_len + 1;  
    }

    write_field->data_len = data_len;
    if (validated)
	write_field->field_status = form_field_valid_data;
    else
	write_field->field_status = form_field_has_data;

        /* copy the data into the field buffer */
    buff_ptr = write_field->buff;
    for (i = 0; i < data_len; i++) {
	*buff_ptr++ = *data++;
    }
    *buff_ptr = '\0';   /* null terminate the data */
    return;
}


PUBLIC void  form_real_read
#ifndef __STDC__
    ( form, field_offset, user_data, max_user_data, field_len ) 
    form_t               *form;
    form_field_offset_t  field_offset;
    char                 *user_data;
    long int             max_user_data;
    long int             *field_len;
#else
  (
    form_t               *form,
    form_field_offset_t  field_offset,
    char                 *user_data,
    long int             max_user_data,
    long int             *field_len
  )
#endif
{

    form_field_t  *read_field;   /* for convenient field access */
    boolean32  write_null;    /* enough space in data_buff for null terminator? */
    int i, num_copied;

    read_field = &(form->fields[field_offset]);

       /* if the field hasn't been written by anyone, then there's no data */
    if (read_field->field_status == form_field_no_data) {
	*user_data = '\0';   	/* give the user back a zero length string */
        *field_len = 0;
	return;
    }

        /* copy as much of the data as possible to the caller's buffer */
    write_null = false;
    if (read_field->data_len < max_user_data) {  
	/* caller's buffer can hold all the data */
	num_copied = read_field->data_len; 
	write_null = true;                  /* and there's room for a NULL */
    } else
        /* no room for null and possibly truncated */
	num_copied = max_user_data; 

        /* do the copy */
    for (i = 0; i < num_copied; i++) {
	user_data[i] = read_field->buff[i];
    }

    /* and null-terminate if possible */
    if (write_null)
	user_data[num_copied] = '\0';

   *field_len = read_field->data_len; 
   return;
}

	

PUBLIC void form_field_status
#ifndef __STDC__
    ( form, field_offset, field_status ) 
    form_t               *form;
    form_field_offset_t  field_offset;
    form_field_status_t  *field_status;
#else
  (
    form_t               *form,
    form_field_offset_t  field_offset,
    form_field_status_t  *field_status
  )
#endif
{
    *field_status = form->fields[field_offset].field_status;
}


PUBLIC void form_field_attributes
#ifndef __STDC__
    ( form, field_offset, field_attributes ) 
    form_t               *form;
    form_field_offset_t  field_offset;
    form_bitset_t        *field_attributes;
#else
  (
    form_t               *form,
    form_field_offset_t  field_offset,
    form_bitset_t        *field_attributes
  )
#endif
{
    *field_attributes = form->fields[field_offset].attributes;
}
