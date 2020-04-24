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
 * $Log: data_test_client.c,v $
 * Revision 1.1.7.3  1996/02/18  18:25:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:13  marty]
 *
 * Revision 1.1.7.2  1995/12/08  20:54:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:49:38  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:06:31  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:38:55  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/21  19:11:38  weir
 * 	Submit sample program sources (CR 12635)
 * 	[1994/10/21  19:10:52  weir]
 * 
 * $EndLog$
 */
/****************************************************************************/
/* [27.VI.94]                                                               */
/*                                                                          */
/* data_test_client.c -- A minimal client for the data_test test interface. */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*                            -77 cols-                                     */
/****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/file.h>
#include <dce/nbase.h>
#include <dce/dce_error.h>
#include <dce/rpc.h>

#include "data_test.h"


/****************************************************************************/
/* For pointer arithmetic example, test1()...                               */

num_ptr test1_client_ptr;
num_array client_array = {25, 50, 75};

/****************************************************************************/
/* For linked list example, test2()...                                      */

link_t first, *element;
int i;

/****************************************************************************/
/* For allocation of an [out] parameter, test3()...                         */

number_ptr test3_client_ptr = NULL;

/****************************************************************************/
/* For node deletion example, test4()...                                    */

unsigned32 *num;

/****************************************************************************/
/* For array examples, test51(), test52(), test53(), test54()...            */

char5array fixed_array = {'a', 'b', 'c', 'd', 'e'};

/* In the following array, which is defined in data_test.idl, the first number */
/*  specifies the ordinal number of the first element of the to-be-trans-   */
/*  mitted array, the second number specifies the number of array elements  */
/*  to be transmitted, and the list of quoted characters specifies the ar-  */
/*  ray that these actions are to be taken on...                            */
v_struct varying_array = {3, 4, {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'}};


struct {
	unsigned32 size;    /* Number of elements.                          */
	unsigned32 first;   /* First element, ordinal number (from zero).   */
	unsigned32 length;  /* Number of elements.                          */
	char array[10];     /* The elements.                                */
} cv_array = {10, 4, 5, {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'}};

/****************************************************************************/
/* For pipe examples, test6(), test7()...                                   */


#define CBUFFSIZE 1024
char client_buffer[CBUFFSIZE];

/* The pipe type is defined in the IDL file. Here we just declare and ini-  */
/*  tialize the variables we'll need...                                     */

/* A state block to use with the [in] pipe example...                       */
typedef struct test_pipe_state_t {
	idl_char *filename;
	idl_boolean file_open;
	int file_handle;
} test_pipe_state_t;

test_pipe_state_t in_test_pipe_state = {"Test_File", false, 0};
test_pipe_state_t out_test_pipe_state = {"", false, 0};

test_pipe_t test_pipe;

/****************************************************************************/
/* For transmit_as example, test8(). The array type is defined in the IDL   */
/*  file...                                                                 */

sparse_array_t test_array;


/****************************************************************************/
/* ANSI-C style prototypes for functions private to this module...          */

void *myfree(
	idl_void_p_t addr);

void test_alloc(
	rpc_ss_pipe_state_t,
	unsigned32,
	idl_char **,
	unsigned32 *);

void test_pull(
	test_pipe_state_t *,
	idl_char *,
	unsigned int,
	unsigned int *);

void test_push(
	rpc_ss_pipe_state_t *,
	idl_char *,
	unsigned32 count);

void print_error(
	char *,
	error_status_t);

/****************************************************************************/
/******
*
*
* main -- 
*
*
*
******/
int
main(
	int argc,
	char *argv[]
)
{
	handle_t binding_h;
	unsigned_char_t *string_binding; /* For string binding conversions. */
	rpc_ns_handle_t import_context;  /* Context for importing bindings. */
	error_status_t status;

/****************************************************************************/
	/* Start importing servers. Note that the contents of the environ-  */
	/*  ment variable RPC_DEFAULT_ENTRY are used to determine the entry */
	/*  to import from...                                               */
	fprintf(stdout, "data_test_client: Calling rpc_ns_binding_import_begin()...\n");
	rpc_ns_binding_import_begin(
		rpc_c_ns_syntax_default,
		NULL,                         /* Use the RPC_DEFAULT_ENTRY. */
		data_test_v1_0_c_ifspec,
		NULL,
		&import_context,
		&status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_ns_binding_import_begin()", status);
		exit(1);
	}

	/* Import the first server (we could iterate here, but we'll just   */
	/*  take the first one)...                                          */
	fprintf(stdout, "data_test_client: Calling rpc_ns_binding_import_next()...\n");
	rpc_ns_binding_import_next(import_context, &binding_h, &status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_ns_binding_import_next()", status);
		exit(1);
	}

	/* Free the import context...                                       */
	fprintf(stdout, "data_test_client: Calling rpc_ns_binding_import_done()...\n");
	rpc_ns_binding_import_done(&import_context, &status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_ns_binding_import_done()", status);
		exit(1);
	}

	/* Convert the binding to a readable string...                      */
	fprintf(stdout, "data_test_client: Calling rpc_binding_to_string_binding()...\n");
	rpc_binding_to_string_binding(binding_h, &string_binding, &status);
	if (status != rpc_s_ok)
	{
		print_error("rpc_binding_to_string_binding()", status);
		exit(1);
	}

	/* Print it...                                                      */
	fprintf(stdout, "data_test_client: Imported resolved binding == %s\n", string_binding);

	/* Free the string binding space...                                 */
	fprintf(stdout, "data_test_client: Calling rpc_string_free()...\n");
	rpc_string_free(&string_binding, &status);
	if (status != rpc_s_ok)
	{	
		print_error("rpc_string_free()", status);
		exit(1);
	}

/****************************************************************************/
	/* Execute the examples...                                          */

/****************************************************************************/
	/* Pointer arithmetic example:                                      */
	{
		/* An array of integers is declared in the client. The ar-  */
		/*  ray type, along with a pointer to it, is also defined   */
		/*  in the IDL file. The pointer is initialized to NULL,    */
		/*  and then the pointer and the array are passed to the    */
		/*  server; there the pointer is re-initialized to point to */
		/*  the array and then incremented. When the remote test1() */
		/*  call returns, the pointer will be found to be pointing  */
		/*  to the second element in the array...                   */

		test1_client_ptr = NULL;
		fprintf(stdout, "\n\n\ndata_test_client: Calling test1()...\n");
		test1(binding_h, &test1_client_ptr, client_array, &status);

		/* The test function pointed the client pointer to the      */
		/*  second array element. On return, this points to memory  */
		/*  that holds this value.                                  */

		fprintf(stdout, "data_test_client: Client pointer points to %i\n", *test1_client_ptr);

		/* However, if we now increment the pointer, it             */
		/*  points to uninitialized memory. This shows the          */
		/*  limits of mirroring.                                    */

		test1_client_ptr++;
		fprintf(stdout, "data_test_client: Client pointer now points to %i\n", *test1_client_ptr);

	}

/****************************************************************************/
	/* Linked list example.                                             */
	/* The first element of a linked list is declared in the client.    */
	/*  The list is also defined in the IDL file. On each call to the   */
	/*  remote routine test2(), a new element is allocated and added to */
	/*  the list at the server, and the whole list is then passed back  */
	/*  to the client. The example thus shows how to dynamically and    */
	/*  remotely add elements to a data structure that is to be used by */
	/*  both client and server.                                         */
	/*  After some elements have been added, the list is printed out... */
	{
		first.value = 2;
		first.next = NULL; 
		fprintf(stdout, "\n\n\n");

		/* Call the remote procedure eight times, thus adding eight */
		/*  elements to the list...                                 */
		for (i = 0; i < 8; i++)
		{
			fprintf(stdout, "data_test_client: Calling test2(%d)...\n", i);
			test2(binding_h, &first, &status);
		}

		/* Now get the front of the list and print the whole thing  */
		/*  out here. We should now see a nine-element list, con-   */
		/*  sisting of the original first element that was declared */
		/*  here, followed by the eight additional elements that    */
		/*  were added by the server...                             */
		element = &first;
		while(element->next)
		{
			fprintf(stdout, "data_test_client: element->value == %i\n", element->value);
			element = element->next;
		}
		fprintf(stdout, "data_test_client: element->value == %i\n", element->value);		
	}

/****************************************************************************/
	/* In contrast to the previous example, the following shows how you */
	/*  can allocate space for a completely new variable in a remote    */
	/*  routine, and then "pass" the space and a pointer to it back to  */
	/*  the client. The difference between this example and the previ-  */
	/*  ous one is that here the variable doesn't exist when the call   */
	/*  is made (although a pointer to the data type is passed in to    */
	/*  the routine)...                                                 */

	/* Allocation of an [out] parameter...                              */
	{
		fprintf(stdout, "\n\n\ndata_test_client: Calling test3()...\n");

		/* Pass the null pointer to the server routine...           */
		test3(binding_h, &test3_client_ptr, &status);

		/* After the remote routine has returned, we should now be  */
		/*  pointing to something that can be printed out; this     */
		/*  space was allocated at the server and the value assign- */
		/*  there...                                                */
		fprintf(stdout, "data_test_client: Value == %i\n", (unsigned32) * (test3_client_ptr->value));
	}

/****************************************************************************/
	/* The following example shows how memory that was allocated (via   */
	/*  the stub management mechanism) at the client can be deallocated */
	/*  at the server...                                                */

	/* Node deletion example. This is broken for some reason !!         */
	{
		/* Before we can use the rpc_sm_allocate() call here in the */
		/*  client, we have to enable the mechanism; that's what    */
		/*  the following call does. Note that this isn't necessary */
		/*  in the server...                                        */
		rpc_sm_enable_allocate(&status);

		/* Establish the allocation and freeing routines that the   */
		/*  client stub memory management mechanism should use...   */
		rpc_sm_set_client_alloc_free((idl_void_p_t (*)(idl_size_t))malloc, (void (*)(idl_void_p_t))myfree, &status);

		/* Now allocate space for a variable...                     */
		num = (unsigned32 *)rpc_sm_allocate(sizeof(unsigned32), &status);

		/* Assign the variable a value...                           */
		*num = 64;

		/* Call the remote routine...                               */
		fprintf(stdout, "\n\n\ndata_test_client: Calling test4()...\n");
		test4(binding_h, num, &status);

		/* We should now find that the variable doesn't exist any-  */
		/*  more, i.e. that the value of the pointer we passed is   */
		/*  once again NULL...                                      */

	}

/****************************************************************************/
	/* Array examples:                                                  */
	{
		/* The first call will simply receive the array we are      */
		/*  passing and will print out its first two elements...    */
		fprintf(stdout, "\n\n\ndata_test_client: Calling test51()...\n");
		test51(binding_h, &fixed_array, &status);

		/* The next call will receive the fixed array and print out */
		/*  its first n elements, where n is the second argument    */
		/*  passed to the remote call...                            */
		fprintf(stdout, "\n\n\ndata_test_client: Calling test52()...\n");
		test52(binding_h, 5, fixed_array, &status);

		/* The next call recieves an array whose bounds are deter-  */
		/*  mined by the parameters of the call itself; the remote  */
		/*  procedure will get an array consisting of a sub-array   */
		/*  transmitted from the contents of varying_array, which   */
		/*  is a complex data type defined in the IDL file; the     */
		/*  parameter contains fields that specify the bounds of    */
		/*  the sub-array to be transmitted...                      */
		fprintf(stdout, "\n\n\ndata_test_client: Calling test53()...\n");

		/* All that the remote procedure will do is print out the   */
		/*  entire contents of the sub-array it recieves...         */
		test53(binding_h, varying_array, &status);

		/* The following example is similar to the previous one;    */
		/*  the only difference is that here both the bounds and    */
		/*  the size of the transmitted array are determined at run */
		/*  time. The remote call simply prints out the contents of */
		/*  the array it recieves...                                */
		fprintf(stdout, "\n\n\ndata_test_client: Calling test54()...\n");
		test54(binding_h, (cv_struct *)&cv_array, &status);
	}

/****************************************************************************/
	/* Pipe examples:                                                   */
	{

		/* The pipe struct, which was defined in the IDL file and   */
		/*  declared at the top of this file, consists of several   */
		/*  slots that have to be filled in as follows:
		/*
		/*  1. Pointer to an "alloc" routine: this is a callback    */
		/*     that will be executed to get buffer space, as needed,*/
		/*     in which to put the data that is to be transmitted   */
		/*     to the server routine. Our version of this routine,  */
		/*     test_alloc(), can be found below.                    */
		/*  2. Pointer to a "pull" routine: this is a routine that  */
		/*     will be executed repeatedly by the called remote     */
		/*     routine to get from the pipe the stuff we want sent. */
		/*     Our version is test_pull(), which can be found be-   */
		/*     low.                                                 */
		/*  3. Pointer to a "push" routine: this is a routine that  */
		/*     will be executed repeatedly by the called remote     */
		/*     routine to put the specified data into the pipe for  */
		/*     the client. From the client's point of view this     */
		/*     routine is like a callback, because it is executed   */
		/*     asynchronously as far as it's concerned and its fi-  */
		/*     nal result will be that the pushed data is present   */
		/*     wherever it's supposed to be. In our example,        */
		/*     test_push(), the processing consists simply of       */
		/*     putchar()s to stdout, so that when the program is    */
		/*     executed the pushed data will simply appear in the   */
		/*     client window as it's being pushed. However, the     */
		/*     processing could have consisted, say, of writing the */
		/*     data to some file.                                   */
		/*  4. Pointer to a "state" structure that contains infor-  */
		/*     mation about the current state of processing that    */
		/*     can be useful to (and updated by) the client or ser- */
		/*     ver routines.                                        */

		test_pipe.alloc = (void (*)())test_alloc;

		/* For [out] pipe test, test6()...                          */
		test_pipe.push = (void (*)())test_push;
		test_pipe.state = (rpc_ss_pipe_state_t)&out_test_pipe_state;

		/* For [in] pipe test, test7()...                           */
		test_pipe.pull = (void (*)())test_pull;

		/* If something was specified on the client's command line, */
		/*  get it and send it off to our remote call, on the as-   */
		/*  sumption that it's a valid shell command that the rou-  */
		/*  tine can try to execute and pipe the output of back to  */
		/*  us...                                                   */
		if(argc == 2)
		{
			fprintf(stdout, "\n\n\ndata_test_client: Calling test6()...\n");
			test6(binding_h, (idl_byte *)argv[1], &test_pipe, &status);
		}

		/* Call the in-pipe example routine. This will read in, at  */
		/*  the server, the contents of a file whose name is speci- */
		/*  fied in the state block; the name is filled in at the   */
		/*  top of this file...                                     */
		test_pipe.state = (rpc_ss_pipe_state_t)&in_test_pipe_state;
		fprintf(stdout, "\n\n\ndata_test_client: Calling test7()...\n");
		test7(binding_h, test_pipe, &status);

	}

/****************************************************************************/
	/* Transmit as example:                                             */
	{
		/* Set up a sparse array with only three non-zero           */
		/*  members. First, zero out the array...                   */
		memset(test_array, 0, sizeof(unsigned32) * S_ARRAY_SIZE);
		/* And now, stick a few values in...                        */
		test_array[0] = 2;
		test_array[10] = 4;
		test_array[20] = 8;

		/* And finally, transmit what's left to the remote routine, */
		/*  which will print the values...                          */
		fprintf(stdout, "\n\n\ndata_test_client: Calling test8()...\n");
		test8(binding_h, &test_array, &status);

	}

	fprintf(stdout, "\n\n\ndata_test_client: Exiting.\n");

}



/******
*
*
* myfree -- This is the freeing routine that is registered as to be used
*           by the stub management mechanism for freeing memory; the reg-
*           istering is done by the call to rpc_sm_set_client_alloc_free(),
*           above.
*
*
******/
void *myfree(
idl_void_p_t addr
)
{
	fprintf(stdout, "data_test_client: entering myfree()...\n");
	free(addr);
	fprintf(stdout, "data_test_client: ...exiting myfree()\n");
}


/******
*
* test_alloc -- Simply returns the same statically allocated buffer
*               over and over again.
*
* 
*    Note that the chunk size need not be the same as the one specified
*     by the server (which is only a request anyway).
*
*
******/
void test_alloc(
rpc_ss_pipe_state_t state,
unsigned32 bsize,
idl_char **buf,
unsigned32 *bcount
)
{
	fprintf(stdout, "data_test_client: entering test_alloc()...\n");
	*buf = (idl_char *)client_buffer;
	*bcount = CBUFFSIZE;
	fprintf(stdout, "data_test_client: ...exiting test_alloc()\n");
}


/******
*
*
* test_pull -- reads a file one buffer at a time.
*
*
*       Uses the state block to determine the state of the file and takes
*        the appropriate action. Remember, the state block is purely local
*        to the client.  
*
******/
void test_pull(
test_pipe_state_t *state,
idl_char *buf,
unsigned int esize,
unsigned int *ecount
)
{
	fprintf(stdout, "data_test_client: entering test_pull()...\n");
	if ( !state->file_open )
	{
		state->file_handle = (int)open((char *)state->filename, O_RDONLY);
		if (state->file_handle == -1)
		{
			fprintf(stdout, "data_test_client: Client couldn't open %s\n", state->filename);
			exit(0);
		}
		state->file_open = true;
	}
	*ecount = read( state->file_handle, buf, esize );
	if (*ecount == 0)
	{
		close(state->file_handle);
		state->file_open = false;
	}
	fprintf(stdout, "data_test_client: ...exiting test_pull()\n");
}


/******
*
*
* test_push -- prints each recieved buffer-full.
*
*
*
******/
void test_push(
rpc_ss_pipe_state_t *state, /* Pipe state block; nothing is done with this. */
idl_char *buf,              /* Stuff to push into the pipe.                 */
unsigned32 count            /* Amount of stuff to push.                     */
)
{
	unsigned_char_t *cptr;

	fprintf(stdout, "data_test_client: entering test_push()...\n");
	for (cptr = buf; cptr < buf + count; cptr++)
	{
		putchar(*cptr);
	}
	fprintf(stdout, "data_test_client: ...exiting test_push()\n");
}


/******
 *
 * print_error-- Client version. Prints text associated with bad status code.
 *
 *
 *
 ******/
void
print_error(char *caller, /* Routine that received the error.               */
error_status_t status)    /* The status we want to print the message for.   */
{
	dce_error_string_t error_string;
	int print_status;

	dce_error_inq_text(status, error_string, &print_status);
	fprintf(stderr," data_test_client: %s: %s\n", caller, error_string);
}

