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
 * $Log: data_test_manager.c,v $
 * Revision 1.1.7.3  1996/02/18  18:26:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:14  marty]
 *
 * Revision 1.1.7.2  1995/12/08  20:54:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:49:40  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:06:33  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:38:56  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/21  19:11:40  weir
 * 	Submit sample program sources (CR 12635)
 * 	[1994/10/21  19:10:53  weir]
 * 
 * $EndLog$
 */
/****************************************************************************/
/* [27.VI.94]                                                               */
/*                                                                          */
/* data_test_manager.c -- Implementation of "data_test" interface to        */
/*                        demonstrate various parameter passing techniques. */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*                            -77 cols-                                     */
/****************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <pthread.h>
#include <dce/dce_error.h>
#include "data_test.h"

#define SBUFFSIZE 256


/******
*
*
* test1 -- Pointer arithmetic example.
*
*
*   Illustrates how address space mirroring occurs. On return, the
*    client's version of num_ptr will point to memory that holds the
*    second element of the array. It will not point to the array itself,
*    however.
*
*
******/
void 
test1(
handle_t h,
num_ptr *client_ptr,
num_array client_array,
error_status_t *status
)
{
	fprintf(stdout, "\n\n\ndata_test_manager: Entering test1()...\n");
	*status = 0;
	*client_ptr = client_array;
	++(*client_ptr);
	fprintf(stdout, "data_test_manager: ...Exiting test1()\n");
}


/******
*
*
* test2 -- Linked list example.
*
*
*    Note that in this and the following example, we use
*     rpc_sm_allocate() to allocate data on the server side.
*     This gives the semantics you probably want for a dynamical-
*     ly allocated referent for a pointer parameter: on return, the
*     data is automatically deallocated on the server, and further
*     manager operations that access this data do so via a pointer
*     parameter passed by the client. Memory leaks in the server are
*     avoided.  
* 
*    An application must be very cautious if it attempts to use
*     pointer parameters in a way that contradicts such semantics:
*     for example, by returning a pointer to static global storage
*     on the server. In such a case, the server and client versions
*     of such storage can easily become inconsistent. A context handle,
*     which the client must not modify, is typically what you want in
*     such a case.
*
*
******/
void test2(
handle_t handle,
link_t *head,
error_status_t *status
)
{
	link_t *element;

	fprintf(stdout, "\n\n\ndata_test_manager: Entering test2()...\n");

	/* Add a new element to the end of a linked list. Note that         */
	/*  the head parameter has [in, out] semantics here: we must        */
	/*  pass in a pointer to a valid element. (The next example         */
	/*  shows how to implement an [out] parameter that is allocated     */
	/*  by the operation.)                                              */

	if (head)
	{
		/* Find the last element of the list...                     */
		element = head;
		while (element->next)
			element = element->next;
	
		/* Now add a new element to the list...                     */

		/* Allocate the element space...                            */
		element->next = (link_t *) rpc_sm_allocate(sizeof(link_t), status);
		/* Put a value in the element equal to twice the value of   */
		/*  its predecessor...                                      */
		element->next->value = element->value * 2;
		/* Mark the new end of the list...                          */
		element->next->next = NULL;

	}

	*status = error_status_ok;
	fprintf(stdout, "data_test_manager: ...Exiting test2()\n");
}


/******
*
*
* test3 -- allocation of an [out] parameter.
*
*
******/
void test3(
handle_t handle,
number_ptr *client_ptr,
error_status_t *status 
)
{
	number_ptr nptr;
	unsigned32 *nval;

	fprintf(stdout, "\n\n\ndata_test_manager: Entering test3()...\n");

	nptr = (number_ptr)rpc_sm_allocate(sizeof(number), status);
	nval = (unsigned32 *)rpc_sm_allocate(sizeof(unsigned32), status);
	*nval = 256;
	nptr->value = nval;
	*client_ptr = nptr;
	*status = error_status_ok;

	fprintf(stdout, "data_test_manager: ...Exiting test3()\n");

}


/******
*
*
* test4 -- Node deletion example.
*
*          "This is broken for some reason" [ORIGINAL NOTE]
*
*
******/
void
test4(
handle_t h,
unsigned32 *number,
error_status_t *status
)
{

	fprintf(stdout, "\n\n\ndata_test_manager: Entering test4()...\n");

	*number = 32;
	rpc_sm_free(number, status);
	*status = error_status_ok;

	fprintf(stdout, "data_test_manager: ...Exiting test4()\n");
}


/******
*
*
* test5 -- Array example.
*
*          Not currently called.
*
******/
void test5(
handle_t handle,
idl_char client_greeting[],
idl_char server_reply[REPLY_SIZE],
error_status_t *status 
)
{
	fprintf(stdout, "\n\n\ndata_test_manager: Entering test5()...\n");
	*status = error_status_ok;
	fprintf(stdout, "data_test_manager: ...Exiting test5()\n");
}


/******
*
*
* test51 -- Array example.
*
*           Simply receives a fixed array declared at the client,
*            and prints out the values of its first two members.
*
*
******/
void test51(
handle_t handle,
char5ptr a_pointer,
error_status_t *status 
)
{
	fprintf(stdout, "\n\n\ndata_test_manager: Entering test51()...\n");

	fprintf(stdout, "data_test_manager: Array test 1:\n");
	fprintf(stdout, "%c %c \n", (*a_pointer)[0], (*a_pointer)[1]);
	*status = error_status_ok;

	fprintf(stdout, "data_test_manager: ...Exiting test51()\n");
}


/******
*
*
* test52 -- Array example.
*
*           Prints out the first n elements of an_array, where n is
*            specified by the value of the second parameter (size)
*            passed.
*
*
******/
void test52(
handle_t handle,
unsigned32 size,
idl_char an_array[],
error_status_t *status
)
{
	unsigned32 i;

	fprintf(stdout, "\n\n\ndata_test_manager: Entering test52()...\n");

	fprintf(stdout, "data_test_manager: Array test 2:\n");
	for (i = 0; i < size; i++)
	{
		fprintf(stdout, "%c ", an_array[i]);
		fprintf(stdout, "\n");
	}
	*status = error_status_ok;

	fprintf(stdout, "data_test_manager: ...Exiting test52()\n");
}


/******
*
*
* test53 -- Array example.
*
*           This call will recieve an array that consists only of part
*            of the data specified as the 2nd argument to the remote
*            call to test53() in the client. Fields in the parameter
*            at the client specify how much of the data is to be trans-
*            mitted. The present routine simply prints out the entire
*            contents of the sub-array it recieves.
*
*
******/
void test53(
handle_t handle,
v_struct v_array,
error_status_t *status
)
{
	unsigned32 i;

	fprintf(stdout, "\n\n\ndata_test_manager: Entering test53()...\n");

	fprintf(stdout, "data_test_manager: Array test 3:\n");
	for (i = v_array.first; i < v_array.first + v_array.length; i++)
		fprintf(stdout, "subscript == %i value == %c\n", i, v_array.array[i]);
	*status = error_status_ok;
	fprintf(stdout, "data_test_manager: ...Exiting test53()\n");
}


/******
*
*
* test54 -- Array example.
*
*
******/
void test54(
handle_t handle,
cv_struct *cv_array,
error_status_t *status
)
{
	unsigned32 i;

	fprintf(stdout, "\n\n\ndata_test_manager: Entering test54()...\n");

	fprintf(stdout, "data_test_manager: Array test 4:\n");
	for (i = (*cv_array).first; i < (*cv_array).first + (*cv_array).length; i++)
		fprintf(stdout, "subscript == %i value == %c\n", i, (*cv_array).array[i]);
	*status = error_status_ok;
	fprintf(stdout, "data_test_manager: ...Exiting test54()\n");
}


/******
*
*
* test6 -- Out-pipe example. Tries to execute a shell command (specified on
*           the client command line; only "ls" or "ps" are allowed), and
*           pipes its output back to the caller.
*
*
******/
/****************************************************************************/
void 
test6(
handle_t binding_h,
idl_char *cmd,             /* Command to execute.                           */
test_pipe_t *test_pipe,    /* An out-pipe.                                  */
error_status_t *status 
)
{
  
	DIR *dir_ptr;
	struct dirent *directory;

	char buffer[SBUFFSIZE];
	FILE *str_ptr;
	int n;

	fprintf(stdout, "\n\n\ndata_test_manager: Entering test6()...\n");

	/* An elementary mechanism to execute a command and get the         */
	/*  output back. Note that popen() and fread() are thread-safe,     */
	/*  so there's no danger of our whole process blocking while the    */
	/*  call thread waits for them to return.                           */

	/* This is potentially a dangerous operation!                       */
	/*  Here we will only try either a "ps" or an "ls" command...       */

	if (!strcmp(cmd, "ps") || !strcmp(cmd, "ls"))
	{

		if (!strcmp(cmd, "ps"))
			fprintf(stdout, "data_test_manager: ps command specified\n");
		else if (!strcmp(cmd, "ls"))
			fprintf(stdout, "data_test_manager: ls command specified\n");

		/* Try executing the command, setting up a pipe for its     */
		/*  output...                                               */
		if ((str_ptr = popen((char *)cmd, "r")) == NULL)
		{
			fprintf(stdout, "data_test_manager: popen() failed\n");
			return;
		}

		/* Okay, now start getting the output of the command, and   */
		/*  send it back via our pipe to the client...              */
		while ((n = fread(buffer, sizeof(char), SBUFFSIZE, str_ptr)) > 0)
		{
			/* Push the stuff we read onto our pipe...          */
			(*(test_pipe->push))(test_pipe->state, (idl_byte *)buffer, n);
		}

		/* Not sure what the point of the following call is...      */
		/*  it seems to just push nothing...                        */
		(*(test_pipe->push))(test_pipe->state, (idl_byte *)buffer, 0);

		/* Close our own shell pipe...                              */
		fclose(str_ptr);

	}

	/* Here's another method: list an arbitrary directory.              */
	/*  This time, we buffer the directory names as null-terminated     */
	/*  strings of various lengths. The client will need to provide     */
	/*  formatting of the output stream, for example, by substituting   */
	/*  a CR for each NULL byte.                                        */
/*
	if ((dir_ptr = opendir(cmd)) == NULL)
	{
		fprintf(stdout, "data_test_manager: Can't open directory %s\n", cmd);
		return;
	}

	while ((directory = readdir(dir_ptr)) != NULL)
	{
		if (directory->d_ino == 0)
			continue;
		(*(test_pipe->push))(test_pipe->state, directory->d_name,
			strlen(directory->d_name) + 1);
	}
	(*(test_pipe->push))(test_pipe->state, directory->d_name, 0);
	closedir(dir_ptr);

*/
	*status = error_status_ok;

	fprintf(stdout, "data_test_manager: ...Exiting test6()\n");
}


/******
*
*
* test7 -- In-pipe example: get stuff from a file piped to
*          us from the client. The file is specified in the
*          state block, declared at the top of data_test_client.c.
*
*
******/
void 
test7(
handle_t binding_h,
test_pipe_t test_pipe,
error_status_t *status 
)
{
	char buffer[SBUFFSIZE];
	int count;
	char *cptr;

	fprintf(stdout, "\n\n\ndata_test_manager: Entering test7()...\n");

	/* Keep pulling stuff out of the pipe, and printing it to stdout,   */
	/*  until we have a count from "pull" of zero. The test_pull() rou- */
	/*  tine defined in the client, which is reading data from a file,  */
	/*  will set this parameter to zero as soon as it reaches the end   */
	/*  of the file...                                                  */
	do
	{
		/* Call the pull routine...                                 */
		(*(test_pipe.pull))(test_pipe.state, (idl_byte *)buffer, SBUFFSIZE, (idl_ulong_int *)&count);

		/* Now empty the buffer contents into stdout...             */
		for (cptr = buffer; cptr < buffer + count; cptr++)
			putchar(*cptr);

	} while (count > 0);  /* ...until there's nothing left in pipe.     */

	fprintf(stdout, "data_test_manager: ...Exiting test7()\n");
}


/******
*
*
* test8 -- Example using transmit_as.
*
*          This routine receives the non-zero values from a sparse
*          array, and prints them out. The significance of this is
*          that what's passed by the client is the entire sparse
*          array; the non-sparse elements which are all that appear
*          here were mined out of the array by the transmit_as rou-
*          tines declared in the data_test_xmit.c file.
*
*          The transmit_as type is defined in the IDL file; the type
*          (sparse_array_t) then has to have the following routines
*          defined for it:
*
*
*            sparse_array_t_to_xmit      A callback routine that allocates
*                                        space for the stuff to transmit,
*                                        and then places the stuff into
*                                        that space.
*
*            sparse_array_t_from_xmit    A callback routine that allocates
*                                        space (if required) for the stuff
*                                        to be transmitted back, and then
*                                        places the stuff into the desti-
*                                        nation space.
*
*            sparse_array_t_free_xmit    A callback routine that frees any
*                                        space that was allocated by the
*                                        _to_xmit callback.
*
*            sparse_array_t_free_inst    A callback routine that frees any
*                                        space that was allocated by the
*                                        _from_xmit callback.
*
*          --These routines are all declared in data_test_xmit.c. IDL asso-
*          ciates them (by name) with their type.
*
*
******/
void test8(
handle_t binding_h,
sparse_array_t *array,
error_status_t *status
)
{
	int i;

	fprintf(stdout, "\n\n\ndata_test_manager: Entering test8()...\n");

	/* Print the elements of the sparse array until we come to          */
	/*  a zero element.                                                 */

	for (i = 0; i < S_ARRAY_SIZE; i++)
	{
		fprintf(stdout, "%i\n", (*array)[i]);
	}
	*status = error_status_ok;

	fprintf(stdout, "data_test_manager: ...Exiting test8()\n");
}


/******
 *
 * print_manager_error-- Manager version. Prints text associated with bad status code.
 *
 *
 *
 ******/
void
print_manager_error(
char *caller,  /* String identifying the routine that received the error.   */
error_status_t status) /* the status we want to print the message for.      */
{
	dce_error_string_t error_string;
	int print_status;

	dce_error_inq_text(status, error_string, &print_status);
	fprintf(stderr," data_test_manager: %s: %s\n", caller, error_string);

}

