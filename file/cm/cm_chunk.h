/* Copyright (C) 1994, 1989 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log$
 * $EndLog$
 */

/*
 * Chunk number/offset computations
 */

#define cm_chunkoffset(offset) 			\
    (((offset) < cm_firstcsize) ? (offset) :	\
    (((offset) - cm_firstcsize) & (cm_othercsize - 1)))

#define cm_chunk(offset)			\
    (((offset) < cm_firstcsize) ? 0 :		\
    ((((offset) - cm_firstcsize) >> cm_logchunk) + 1))

#define cm_chunkbase(offset)			\
    (((offset) < cm_firstcsize) ? 0 :		\
    ((((offset) - cm_firstcsize) & ~(cm_othercsize - 1)) + cm_firstcsize))

#define cm_chunksize(offset)			\
    (((offset) < cm_firstcsize) ? cm_firstcsize : cm_othercsize)

#define cm_chunktobase(chunk)			\
    (((chunk) == 0) ? 0 : (cm_firstcsize + (((chunk) - 1) << cm_logchunk)))

#define cm_chunktosize(chunk)			\
    (((chunk) == 0) ? cm_firstcsize : cm_othercsize)
