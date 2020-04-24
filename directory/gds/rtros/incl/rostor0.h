/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rostor0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:12  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:58  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:36:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:11  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:01  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:42  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:36  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:58:35  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:44  keutel
 * 	creation
 * 	[1994/03/21  13:28:04  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROSTOR0_H
#define _ROSTOR0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rostor0_rcsid[] = { "@(#) $RCSfile: rostor0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:12 $" } ;
#endif

/**********************************************************************\
*						                       *
*  * Copyright (c) 1992 by					       *       *
*  * SIEMENS NIXDORF INFORMATIONSSYSTEME AG  		               *       *
*  * All rights reserved					       *       *
*  *								       *       *
*  * This software is furnished under licence and may be used only in  *       *
*  * accordance with the terms of that licence and with the inclusion  *
*  * of the above copyright notice. This software may not be provided  *
*  * or otherwise made available to, or used by, any other person.     *
*  * No title to or ownership of the software is hereby transferred.   *                *
*								       *       *
\**********************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME         : rostor0.h                                           */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : NOV-28th-1991                                       */
/*                                                                    */
/* COMPONENT    : Store common utilities                              */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0 / MAVROS-V2.2                              */
/*                                                                    */
/* DESCRIPTION  : Store common utilities                              */
/*                                                                    */
/*                - Read/Write operation to a buffered Store          */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rostor0.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/*  1.00    | 91/11/01|  original                      | gj |         */
/*  1.01    | 91/11/02|  new error codes introduced    | gj |         */
/*  1.02    | 91/11/14|  new MAVROS function           | gj |         */
/*  1.03    | 92/02/19|  more detailed comments now    | gj |         */
/*  1.04    | 92/05/26|  new Open/CloseStFile          | gj |         */
/*datoff **************************************************************/

/* ********************************************************************/
/* **********              Store facility                  ********** */
/* ********************************************************************/

#define ST_OPEN         1              /* always open the storefile   */
#define ST_CLOSED       0              /* open the storefile only     */
				       /* when the buffer is full     */

#define ST_USEMAXBUF   -1              /* use the value of MAXBULEN   */
				       /* as default for buffersize   */

#define ST_NO           0              /* means <not>                 */
#define ST_YES          1              /* means <yes>                 */

#define ST_MINERRVAL  -50              /* lower limit for Store-Error */
				       /* codes -- has to be updated  */
				       /* when limit is reached       */

#define STFOPENE      -10           /* error from fopen command    */
#define STBUMALE      -11           /* no storebuffer is allocated */
#define STPOSERR      -12           /* invalid store position      */
#define STBUEXCE      -13           /* storebuffer exceeded        */
#define STFSEEKE      -14           /* error from fseek command    */
#define STFWRITE      -15           /* error from fwrite command   */
#define STFREADE      -16           /* error from fread command    */
#define STFEXCEE      -17           /* store file limit exceeded   */
#define STFCLOSE      -18           /* error from fclose command   */

#define STGALLCE      -19           /* global alloc error          */
#define STGLOCKE      -20           /* global lock error           */
#define STGULOKE      -21           /* global unlock error         */
#define STGFREEE      -22           /* global free error           */
#define STGRALLE      -23           /* global realloc error        */
#define STHDLERR      -24           /* invalid Storehandle         */
#define STASNERR      -25           /* en/decode error by MAVROS   */
#define STFNAMER      -26           /* wrong filename              */
#define STMADRER      -27           /* invalid mem-address NULL    */
#define STLENERR      -28           /* length zero or negative     */
#define STBULENE      -29           /* bufferlength 0 or negative  */
#define STMODEER      -30           /* mode must be 0 or 1         */
#define STDBLIDL      -31           /* multiple call of storeidle  */
#define STNWSZER      -32           /* Store-newsize out of range  */
#define STNAMERR      -33           /* Unknown Store filename      */
#define STFNOTOP      -34           /* Close of a non open stfile  */
#define STTMPNME      -35           /* error from tmpnam command   */
#define STREMOVE      -36           /* error from remove command   */


/* ------------------------------------------------------------------ */

unsigned long int PASCAL ro0060_newstore PROTO((long length,
                                          int mode,
					  signed short int FAR *error));

       /*******************************************************/
       /* DESCRIP:  Creates a new Store                       */
       /* RETURN:    NULL : if error (-> no store installed ) */
       /*         <> NULL : Handle to the Store               */
       /* INPUT:   length : desired buffer size for the Store */
       /*                   (will be cut if too large)        */
       /*                   if the value is ST_USEMAXBUF,then */
       /*                   MAXBUFLEN will be used            */
       /*                   will be used.                     */
       /*            mode : if ST_OPEN  -> don't open a file  */
       /*                   'til the buffer is full.          */
       /*                   if ST_CLOSED -> open a storefile  */
       /*                   during initializing even if the   */
       /*                   buffer isn't full yet.            */
       /* OUTPUT:    error: success information of the Store- */
       /*                   Installation( 0=ok, <>0 = error   */
       /*******************************************************/

/* ------------------------------------------------------------------ */

long PASCAL ro0061_stbuflen PROTO((unsigned long int hdl));

       /*******************************************************/
       /* DESCRIP:  Gets the current size of internal buffer  */
       /* RETURN:    >=0  : Size of the Store-Buffer          */
       /*             <0  : type of error                     */
       /* INPUT:      hdl : Handle to the Store               */
       /*******************************************************/

/* ------------------------------------------------------------------ */

long PASCAL ro0062_storelength PROTO((unsigned long int hdl));

       /*******************************************************/
       /* DESCRIP:  Updates the total length of the Store     */
       /* RETURN:    >=0  : actual store length               */
       /*             <0  : type of error                     */
       /* INPUT:      hdl : Handle to the Store               */
       /*******************************************************/

/* ------------------------------------------------------------------ */

signed short int PASCAL ro0063_sterror PROTO((unsigned long int hdl));

       /*******************************************************/
       /* DESCRIP:  Gets the internal error status of Store   */
       /* RETURN:      0  : If no Error                       */
       /*             <0  : type of error                     */
       /* INPUT:      hdl : Handle to the Store               */
       /*******************************************************/

/* ------------------------------------------------------------------ */

signed short int PASCAL ro0064_stflush PROTO((unsigned long int hdl));

       /*******************************************************/
       /* DESCRIP:  Writes buffer to disk if changes made     */
       /* RETURN:      0  : If no Error                       */
       /*             <0  : Type of error                     */
       /* INPUT:      hdl : Handle to the Store               */
       /*******************************************************/

/* ------------------------------------------------------------------ */

long PASCAL ro0065_stbufread PROTO((unsigned long int hdl));

       /*******************************************************/
       /* DESCRIP:  Fills the buffer (from internal offset)   */
       /* RETURN:   >= 0  : Number of bytes that were read    */
       /*             <0  : Type of error                     */
       /* INPUT:      hdl : Handle to the Store               */
       /*******************************************************/


/* ------------------------------------------------------------------ */

signed short int PASCAL ro0066_storepos PROTO((unsigned long int hdl, 
					       long pos));

       /*******************************************************/
       /* DESCRIP:  Moves to an absolute Store-Position       */
       /* RETURN:      0  : If no Error                       */
       /*             <0  : Type of error                     */
       /* INPUT:      hdl : Handle to the Store               */
       /*             pos : Absolute Store position where to  */
       /*                   move to (counting starts with 0)  */
       /*******************************************************/

/* ------------------------------------------------------------------ */

signed short int PASCAL ro0067_stputc PROTO((unsigned long int hdl,
					     unsigned char databyte));

       /*******************************************************/
       /* DESCRIP:  Writes one byte to Store + moves forward  */
       /* RETURN:      0  : If no Error                       */
       /*             <0  : Type of error                     */
       /* INPUT:      hdl : Handle to the Store               */
       /*        databyte : Byte that shall be written to     */
       /*                   the Store                         */
       /*******************************************************/

/* ------------------------------------------------------------------ */

signed short int PASCAL ro0068_stputm PROTO((unsigned long int hdl,
                                       char FAR * madr,
                                       long length));

       /*******************************************************/
       /* DESCRIP:  Copies a datablock from memory to Store   */
       /* RETURN:      0  : If no Error                       */
       /*             <0  : Type of error                     */
       /* INPUT:      hdl : Handle to the Store               */
       /*            madr : Address from where bytes shall be */
       /*                   copied.                           */
       /*          length : Number of bytes to copy           */
       /*******************************************************/

/* ------------------------------------------------------------------ */

char FAR * PASCAL ro0069_stputs PROTO((unsigned long int hdl, long length));

       /*******************************************************/
       /* DESCRIP:  Gets an address within buffer for writing */
       /* RETURN:   NULL  : If Error                          */
       /*         <>NULL  : Memory address inside the buffer  */
       /* INPUT:      hdl : Handle to the Store               */
       /*          length : Number of bytes be inserted       */
       /*******************************************************/

/* ------------------------------------------------------------------ */

signed short int PASCAL ro0070_stgetc PROTO((unsigned long int hdl));

       /*******************************************************/
       /* DESCRIP:  Reads one byte from Store + moves forward */
       /* RETURN:    >=0  : One Byte from the actual Store-   */
       /*                   position                          */
       /*             <0  : Type of error                     */
       /* INPUT:      hdl : Handle to the Store               */
       /*******************************************************/

/* ------------------------------------------------------------------ */

long PASCAL ro0071_stgetm PROTO((unsigned long int hdl,
                           char FAR * madr, long length));

       /*******************************************************/
       /* DESCRIP:  Copies a datablock from Store to memory   */
       /* RETURN:    >=0  : Number of copied bytes            */
       /*             <0  : Type of error                     */
       /* INPUT:      hdl : Handle to the Store               */
       /*            madr : Address to where bytes shall be   */
       /*                   copied.                           */
       /*          length : Number of bytes to copy           */
       /*******************************************************/

/* ------------------------------------------------------------------ */

char FAR * PASCAL ro0072_stgets PROTO((unsigned long int hdl, long length));

       /*******************************************************/
       /* DESCRIP:  Gets an address within buffer for reading */
       /* RETURN:   NULL  : If Error                          */
       /*         <>NULL  : Memory address inside the buffer  */
       /* INPUT:      hdl : Handle to the Store               */
       /*          length : Number of bytes that will be      */
       /*                   read from the Store (may not ex-  */
       /*                   ceed buffer size nor storesize)   */
       /*******************************************************/

/* ------------------------------------------------------------------ */

signed short int PASCAL ro0073_storeidle PROTO((unsigned long int hdl));

       /*******************************************************/
       /* DESCRIP:  Puts Store to idle-mode                   */
       /* RETURN:      0  : If no Error                       */
       /*             <0  : Type of error                     */
       /* INPUT:      hdl : Handle to the Store               */
       /*******************************************************/

/* ------------------------------------------------------------------ */

signed short int PASCAL ro0074_storeactive PROTO((unsigned long int hdl, long pos));

       /*******************************************************/
       /* DESCRIP:  Puts Store to active-mode                 */
       /* RETURN:      0  : If no Error                       */
       /*             <0  : Type of error                     */
       /* INPUT:      hdl : Handle to the Store               */
       /*******************************************************/

/* ------------------------------------------------------------------ */

signed short int PASCAL ro0075_deletestore PROTO((unsigned long int hdl));

       /*******************************************************/
       /* DESCRIP:  Removes the store - no revival possible   */
       /* RETURN:      0  : If no Error                       */
       /*             <0  : Type of error                     */
       /* INPUT:      hdl : Handle to the Store               */
       /*******************************************************/

/* ------------------------------------------------------------------ */

long PASCAL ro0076_stwhere PROTO((unsigned long int hdl));

       /*******************************************************/
       /* DESCRIP:  Tells the current absolute Position       */
       /* RETURN:    >=0  : Absolute actual Store Position    */
       /*             <0  : Type of error                     */
       /* INPUT:      hdl : Handle to the Store               */
       /*******************************************************/

/* ------------------------------------------------------------------ */

signed short int PASCAL ro0077_stgetcp PROTO((unsigned long int hdl));

       /*******************************************************/
       /* DESCRIP:  Reads one byte without movement           */
       /* RETURN:    >=0  : Byte value at the present Store-  */
       /*                   position (but no skipping)        */
       /*             <0  : Type of error                     */
       /* INPUT:      hdl : Handle to the Store               */
       /*******************************************************/

/* ------------------------------------------------------------------ */

int PASCAL ro0078_stlink  PROTO((unsigned long int hdl));

       /*******************************************************/
       /* DESCRIP:  Raises the internal linkcounter by one    */
       /* RETURN:    >=0  : Number of installed links to this */
       /*                   Store                             */
       /*             <0  : Type of error                     */
       /* INPUT:      hdl : Handle to the Store               */
       /*******************************************************/

/* ------------------------------------------------------------------ */

signed short int PASCAL ro0079_stcat PROTO((unsigned long int hdl1,
					    unsigned long int hdl2,
					    long offset, long length));

       /*******************************************************/
       /* DESCRIP:  Appends a Store to an other Store         */
       /* RETURN:      0  : If no Error                       */
       /*             <0  : Type of error                     */
       /* INPUT:     hdl1 : Handle to the target store        */
       /*            hdl2 : Handle to the source store        */
       /*          offset : Absolute position in source store */
       /*                   from where bytes shall be appended*/
       /*                   to the target store.              */
       /*          length : Number of bytes that shall be     */
       /*                   appended.                         */
       /*******************************************************/

/* ------------------------------------------------------------------ */

signed short int PASCAL ro0080_seterror PROTO((unsigned long int hdl,
                                         signed short int error));

       /*******************************************************/
       /* DESCRIP:  Sets the internal error value             */
       /* RETURN:    <=0  : Old Error before calling          */
       /* INPUT:      hdl : Handle to the target store        */
       /*           error : New error that shall be set       */
       /*******************************************************/

/* ------------------------------------------------------------------ */

signed short int PASCAL ro0081_stmove PROTO((unsigned long int hdl,
				      long destend, long sourceend,
				      long length, long newpos));

       /*******************************************************/
       /* DESCRIP:  Moves a Store-Area within the same Store  */
       /* RETURN:      0  : If no Error                       */
       /*             <0  : Type of error                     */
       /* INPUT:      hdl : Handle to the target store        */
       /*         destend : 1 byte behind where the moved     */
       /*                 : area shall end                    */
       /*       sourceend : 1 byte behind the area that shall */
       /*                 : be moved                          */
       /*          length : Number of bytes to move           */
       /*          newpos : New r/w position after move       */
       /*******************************************************/

/* ------------------------------------------------------------------ */

unsigned long int PASCAL ro0082_FileToStore  PROTO((char FAR *name,
						    long buflen));

       /*******************************************************/
       /* DESCRIP:  Converts an ordinary file to a Store      */
       /* RETURN:      0  : If Error                          */
       /*             >0  : Handle to the new store           */
       /* INPUT:     name : Name of the file on disk that     */
       /*                   shall be converted to a Store     */
       /*          buflen : Desired bufferlength (will be cut */
       /*                   if too long)                      */
       /*******************************************************/

/* ------------------------------------------------------------------ */

long PASCAL  ro0083_stbufresize PROTO(( unsigned long int hdl));

       /*******************************************************/
       /* DESCRIP:  Change of internal buffersize             */
       /* RETURN:     <0  : If Error                          */
       /*             >0  : Size of the new buffer            */
       /* INPUT:     hdl  : Handle to the desired Store       */
       /*******************************************************/

/* ------------------------------------------------------------------ */

long PASCAL  ro0084_stStoreResize PROTO(( unsigned long int hdl,
					  long newsize ));

       /*******************************************************/
       /* DESCRIP:  Change of Storesize after stgets/stputs   */
       /* RETURN:     <0  : If error                          */
       /*             >0  : Size of the new store             */
       /* INPUT:     hdl  : Handle to the desired Store       */
       /*         newsize : Desired new size of truncated     */
       /*                   Store (Attention! must be within  */
       /*                   current buffer range)             */
       /*******************************************************/

/* ------------------------------------------------------------------ */

signed short int PASCAL  ro0085_OpenStFile PROTO(( unsigned long int hdl ));

       /*******************************************************/
       /* DESCRIP:  Creates a temporary storefile             */
       /* RETURN:     <0  : If error                          */
       /*              0  : If file could be created          */
       /* INPUT:     hdl  : Handle to the desired Store       */
       /*******************************************************/

/* ------------------------------------------------------------------ */

signed short int PASCAL ro0086_CloseStFile PROTO(( unsigned long int hdl ));

       /*******************************************************/
       /* DESCRIP:  Closes the storefile logically            */
       /* RETURN:     <0  : If error                          */
       /*              0  : If close was successful           */
       /* INPUT:     hdl  : Handle to the desired Store       */
       /*******************************************************/

/* ------------------------------------------------------------------ */

/* sanjay ++ */

signed short int PASCAL ro0087_StoreToFile PROTO  ((
		unsigned long int ,     /* hdl   */
		char FAR *              /* name  */
		));

       /*******************************************************/
       /* DESCRIP: Converts a Store to a File                 */
       /* RETURN:     <0  : If error                          */
       /*              0  : If conversion was successful      */
       /* INPUT:     hdl  : Handle to the desired Store       */
       /*            name : A char array into which the file  */
       /*                   name is written.                  */
       /*******************************************************/

/* sanjay  -- */

/* --------------------------------------------------------------------- */

signed short int PASCAL ro0088_StoreToLog PROTO  ((
		unsigned long int      /* hdl   */
		));

       /*******************************************************/
       /* DESCRIP: Writes the Store to log file               */
       /* RETURN:     <0  : If error                          */
       /*              0  : If successful                     */
       /* INPUT:     hdl  : Handle to the desired Store       */
       /*******************************************************/

/* --------------------------------------------------------------------- */

#endif	/* _ROSTOR0_H */
