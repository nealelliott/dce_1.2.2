/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: form.h,v $
 * Revision 1.2.6.2  1996/03/11  13:27:04  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:59  marty]
 *
 * Revision 1.2.6.1  1995/12/08  18:00:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:21  root]
 * 
 * Revision 1.2.4.2  1992/12/29  12:40:57  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:35:59  zeliff]
 * 
 * Revision 1.2.2.2  1992/04/03  20:02:30  burati
 * 	CR2419 Add support for local registry.
 * 	[1992/03/31  20:46:42  burati]
 * 
 * Revision 1.2  1992/01/19  22:13:43  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
*/
/*
 *  OSF DCE Version 1.0
 */

/*  form.h V=9 02/10/92 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit kernel form management - public header file
**
*/

#ifndef form_h_included
#define form_h_included

#include <dce/nbase.h>

#include <Pointer.h>

#define form_null_function 0        /* a null function pointer */
#define form_null_header   0        /* a null header pointer */
#define form_null_form     0        /* a null form pointer */

typedef unsigned long int form_field_offset_t ; /* which field in the form? */

typedef enum {form_field_valid_data,
                      /* field contains data that passed validation */
                  form_field_has_data,
                      /* field contains data that wasn't validated */
                  form_field_no_data
                      /* field contains no data at all */
} form_field_status_t;

/*
 * forward declare the form_t so that the following prototype
 * references the global form_t..
 */

struct form_t;

/* validation functions a return a error_status_t and are optionally
 * invoked when a
 * field is written.
 */
typedef error_status_t (*form_validate_t) (
#ifdef __STDC__
    struct form_t   *form,
    char            *name
#endif
);

typedef unsigned long int form_bitset_t;

/* the attribute member in the field structure allows the user to specify
 * arbitrary attributes for a field.  The forms module was developed for use
 * by the rgy_edit kernel. By way of example, the rgy_edit kernel defines the
 * following attribute masks:
 *
 *    readonly, readwrite, required, unused
 */

typedef struct form_field_t {
    char                   *buff;        /* buffer for field data */
    long int               buffsize;     /* the current length of the buffer */
    long int               data_len;     /* the length of the data in buffer */
    form_bitset_t         attributes;    /* user defined */
    form_field_status_t   field_status;  /* has the field been written? */
    form_validate_t       validate;      /* pointer to a validation function -
                                          * gets invoked when form_write is
                                          * called.
                                          */
} form_field_t;

typedef struct form_t {
    Pointer        header;               /* bit stream whose contents are defined
                                             by the user */
    long int       hdr_len;              /* number of bytes in the header
                                          * data
                                          */
    long int       num_fields;           /* number of fields in the form */
    form_field_t  *fields;               /* a vector of data fields */
}
form_t;



/* ROUTINES */

/* The underlying ndr_ types may conflict with the local type when form
 *  routines are called.  To remedy this, each function that accepts or returns
 * a string has a wrapper that casts the parameters into the local type.
 */


#define form_alloc(num_fields,header_size) \
            form_real_alloc((long int)(num_fields), (long int)(header_size))

form_t  *form_real_alloc (
#ifdef __STDC__
    long int   num_fields,   /* IN - how many data fields in the form */
    long int   header_size   /* IN - how many bytes to allocate for the header */
#endif
);


void form_release(
#ifdef __STDC__
    form_t  *form
#endif
);

void form_write_header (
#ifdef __STDC__
    form_t   *form,
    Pointer  header_info
#endif
);


void form_read_header (
#ifdef __STDC__
    form_t   *form,
    Pointer  header_info
#endif
);


void form_init_field (
#ifdef __STDC__
    form_t               *form,            /* IN - pointer to form */
    form_field_offset_t  field_offset,     /* IN - offset to field */
    form_bitset_t        field_attributes, /* IN - attributes for the field */
    form_validate_t      validation_func   /* IN - pointer to the validation
                                            *      function for the field
                                            */
#endif
);


void form_clear (
#ifdef __STDC__
    form_t  *form             /* IN - pointer to the form to be cleared */
#endif
);


#define form_write(form,field,data,data_len,validate,status) \
        form_real_write((form),(field),(char *)(data),(long int)(data_len),\
                         (validate),(status))

void  form_real_write (
#ifdef __STDC__
    form_t               *form,        /* IN - pointer to the form  */
    form_field_offset_t  field_offset, /* IN - offset of field being written */
    char                 *data,        /* IN - character string to be written */
    long int             data_len,     /* IN - the length of the data  */
    boolean32            validate,     /* IN - run the validation routine? */
    error_status_t       *stp          /* OUT - status returns from field's
                                        *       validation routine
                                        */
#endif
);

#define form_read(form,field,user_data,max_user_data,field_len) \
        form_real_read((form),(field),(char *)(user_data),\
                       (long int)(max_user_data),(long int *)(field_len))

void  form_real_read (
#ifdef __STDC__
    form_t               *form,           /* IN - pointer to the form*/
    form_field_offset_t  field_offset,    /* IN - offset of field being read */
    char                 *data_buff,      /* OUT - buffer to write field in */
    long int             max_user_data,   /* IN - buffer can this many chars */
    long int             *field_len       /* OUT - length of data in field? */
#endif
);


void form_num_fields (
#ifdef __STDC__
    form_t    *form,       /* IN - pointer to form */
    long int  *num_fields  /* OUT - number of fields in the form */
#endif
);

void form_field_status (
#ifdef __STDC__
    form_t               *form,        /* IN - pointer to form */
    form_field_offset_t  field_offset, /* IN - offset to field */
    form_field_status_t  *field_status /* OUT - no_data or has_data */
#endif
);


void form_field_attributes (
#ifdef __STDC__
    form_t               *form,            /* IN - pointer to form */
    form_field_offset_t  field_offset,     /* IN - offset to field */
    form_bitset_t        *field_attributes /* OUT - attribute bitset */
#endif
);

#endif
