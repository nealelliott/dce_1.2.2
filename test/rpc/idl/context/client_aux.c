/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: client_aux.c,v $
 * Revision 1.1.6.2  1996/02/17  23:13:16  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:10  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:00:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:30  root]
 * 
 * Revision 1.1.3.3  1993/01/07  19:49:15  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:30:02  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:20:47  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:46:10  weisman]
 * 
 * Revision 1.1  1992/01/19  03:24:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#ifdef VMS
#include <file.h>
#elif defined (SNI_SVR4) 
#include <fcntl.h>
#else
#include <sys/file.h>
#endif
#include <context.h>
#include <errno.h>
#include <test_common.h>

#define SYS_CALL(result, function) \
    do \
        result = function; \
    while (result == -1  &&  errno == EINTR)

#ifdef VMS
#define SUCCESS_STATUS(x) (((x) & 1) == 1)  /* LBS */
#else
#define SUCCESS_STATUS(x) ((x) == 0)
#endif

static char *in_filename = "context.h";
static char *out_filename = "filecopy.out";
static char *s;
static int file_handle;

int file_diff(a,b)
      char *a, *b;
{
      FILE *af = fopen(a,"r");
      FILE *bf = fopen(b,"r");
      char ab[1000],bb[1000];
      int num = 0;
      int status = 1;

      while (1)
      {
          char *aval,*bval;          
          num++;
          aval = fgets(ab,1000,af);
          bval = fgets(bb,1000,bf);
          if ((aval == 0) && (bval==0)) break;
          if ((aval == 0) && (bval!=0)) {
              fprintf(stderr,"Files %s and (eof) %s differ in size\n",a,b);
              status = 0;
	      break;
              }
          if ((aval != 0) && (bval==0)) {
              fprintf(stderr,"Files (eof) %s and %s differ in size\n",a,b);
              status = 0;
	      break;
              }


          if (strcmp(ab,bb) != 0) {
              fprintf(stderr,"file_diff: %s <> %s @ %d\n%s:%s\n%s:%s\n",
                  a,b,num,a,ab,b,bb);
              status = 0;
	      break;
              }
      }

      fclose(af);
      fclose(bf);
      return status;
}

int client_aux(h)
handle_t h;
{ 
    ndr_void_p_t ctx_h;
    long count;
    ndr_byte buffer[BUFFSIZE];
    char command[100];
    int status;

	if (s = getenv("C_IN_FILENAME"))
		in_filename = s;

        file_handle = open(in_filename,O_RDONLY);
        if (file_handle == -1)
        {
            printf("Client couldn't open %s\n", in_filename);
            exit(0);
        } 

    wr_open( h, &ctx_h );
    while (true)
    {
        count = read( file_handle, buffer, BUFFSIZE );
        if (count == 0) break;
        wr_write( ctx_h, count, buffer );
    }
    wr_close( &ctx_h );
    close( file_handle );

	if (s = getenv("C_OUT_FILENAME"))
		out_filename = s;


        file_handle = open(out_filename, 
                                  O_CREAT | O_TRUNC | O_WRONLY, 0644 );
        if (file_handle == -1)
        {
            printf("Couldn't create %s\n",out_filename);
            exit(0);
        }

    rd_open( h, &ctx_h );
    while (true)
    {
        rd_read( ctx_h, &count, buffer );
        if (count == 0) break;
        if (write(file_handle,buffer,count) != count)
        {
            printf("Error writing %s\n", out_filename);
            exit(0);
        }
    }
    rd_close( &ctx_h );
    close( file_handle );

   if (file_diff(in_filename,out_filename))  return(0);
   else return(1);

}

