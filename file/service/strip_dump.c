/* =========================================================================
 * Copyright (C) 1995, 1994 Transarc Corporation - All rights reserved
 * =========================================================================
 *     John Morin
 *     Transarc Corporation
 */
/* Converted to DFS -ota 950317

/* **************************************************************************
 ** README:
 ** This program reads from ** stdin ** an "DFS backup dump" of a volume and 
 ** strips all backup information from the file. The resulting data is send to 
 ** stdout ** and should contain a fts restorable file. 
 * 
 ** README:
 * USE AT YOUR OWN RISK. I threw this together as a personal learning
 * experiment and debugging tool for a specific platform and it is NOT even
 * close to being productisable.
 *
 * ************************************************************************* */

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/service/RCS/strip_dump.c,v 1.1 1995/04/03 20:05:55 restore Exp $ */

#define BSIZE 16384

main ()
{
  int  BMSize, VTSize, size, s, ds, pbuf, buf, code;
  char buffer[2][BSIZE];
  int  bufferHasData[2];

  buf              = 0;
  BMSize           = 24;
  VTSize           = 356;		/* was 208 in AFS */
  bufferHasData[0] = 0;
  bufferHasData[1] = 0;

  while (1)
  {
      /* Read block of data */
      pbuf = (buf?0:1);
      size = fread(&buffer[buf][0], 1, BSIZE, stdin);
      if (size != BSIZE)
      {
	  fprintf (stderr, "Read only %d bytes. Error = %d\n", 
		   size, ((size < 0) ? errno : size));
	  break;                                             /* Will exit */
      }

      /******/
      /* TL */
      /******/
      if ((*(int *)&buffer[buf][0]) == 1100000019)
      {
	  fprintf(stderr, "Tape Label - will continue \n");
      }

      /*********/
      /* SW FM */
      /*********/
      else if ((*(int *)&buffer[buf][4]) == 1000000017)
      {
	  if (bufferHasData[pbuf]) break; /* A FME: Will exit */
      }

      /* Datablock - Could be a Volume Header, Data, or Data with a volume
       * trailer */

      else if ((*(int *)&buffer[buf][4]) == 1100000015) {

	  ds = *(int *)&buffer[buf][0]; /* Amount of data in buffer */

	  /* Check if the VT spans a block */
	  if (ds < VTSize) {
	      VTSize -= ds;		/* Last fwrite will be good */
	      ds = (BSIZE - BMSize);
	      break;			/* All done: Will exit */
	  }
	  else if ((ds > VTSize) || bufferHasData[pbuf]) {
	      
	      /* Write out the previous buffer */
	      if (bufferHasData[pbuf]) {
		  s = (BSIZE - BMSize);
		  size = fwrite(&buffer[pbuf][BMSize], 1, s, stdout);
		  if (size != s) goto werror;
		  bufferHasData[pbuf] = 0;
	      }
	      
	      bufferHasData[buf] = 1;	/* The buffer contains data */
	      buf = (buf?0:1);		/* Step to next free buffer */

	      if (ds < (BSIZE - BMSize))
		  break;		/* Not full: Will exit */
	  }
      }

      /********************/
      /* An unknown block */
      /********************/
      else 
      {
	  fprintf(stderr, "Unknown Block\n");
	  break;                                       /* Will exit */
      }
  }

  /* Write out the last block minus the volume trailer */
  pbuf = (buf?0:1);
  if (bufferHasData[pbuf]) {
      s = ds - VTSize;
      size = fwrite(&buffer[pbuf][BMSize], 1, s, stdout);
      if (size != s) goto werror;
      bufferHasData[pbuf] = 0;
  }

 done:
  return;

 werror:
  fprintf (stderr, "Wrote only %d bytes of %d. Error = %d\n", size, s, errno);
  return;
}
