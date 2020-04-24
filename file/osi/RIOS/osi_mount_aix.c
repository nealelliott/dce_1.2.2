/*
 * HISTORY
 * $Log: osi_net.h,v $
 * $EndLog$
 */
/* Copyright (C) 1993 Transarc Corporation - All rights reserved */

#include <sys/vfs.h>
#include <sys/statfs.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_mount.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/osi/RIOS/RCS/osi_mount_aix.c,v 1.1 1993/11/10 14:11:10 jdp Exp $")

/* Round a size up to a 4 byte boundary */
#define	ROUNDUP(x)	(((x) + 3) & ~3)

static char* hasopt(const char* optStr, const char* opt);

int
osi_mount(const char* devName, const char* mp, int type, int flags,
	  const char* options, const void* data, unsigned dataSize)
{
    int			code = 0;
    char*		comma = "";
    short		devNameSize;
    char		localOptions[1024] = "";
    short		mpSize;
    short		nextOffset;
    short		optionsSize;
    struct vmount*	vmtBufP;
    struct vmt_data*	vmtData;
    ulong		vmtBufSize;

    if (options != NULL && options[0] != '\0') {
	strcpy(localOptions, options);
	comma = ",";
    }

    if ((flags & OSI_MOUNT_FLAGS_READONLY)) {
	if (hasopt(localOptions, "ro") == NULL){
	    (void)strcpy(localOptions, comma);
	    (void)strcpy(localOptions, "ro");
	}
    } else {
	if (hasopt(localOptions, "rw") == NULL) {
	    (void)strcpy(localOptions, comma);
	    (void)strcpy(localOptions, "rw");
	}
    }

    /*
     * build vmount structure
     * size is the size of the vmount structure and of the following
     * data items, each of which is starting on long word boundary.
     */
    devNameSize = ROUNDUP(strlen(devName) + 1);
    mpSize = ROUNDUP(strlen(mp) + 1);
    optionsSize = ROUNDUP(strlen(localOptions) + 1);
    vmtBufSize = sizeof(*vmtBufP)
	         + devNameSize + mpSize + dataSize + optionsSize;

    if ((vmtBufP = osi_Alloc(vmtBufSize)) == NULL)
	return errno;

    vmtBufP->vmt_revision = VMT_REVISION;
    vmtBufP->vmt_length = vmtBufSize;
    vmtBufP->vmt_flags = flags;
    vmtBufP->vmt_gfstype = type;

    nextOffset = sizeof(*vmtBufP);

    /* Device name */
    vmtData = vmtBufP->vmt_data;
    vmtData[VMT_OBJECT].vmt_off = nextOffset;
    vmtData[VMT_OBJECT].vmt_size = devNameSize;
    (void)strcpy(vmt2dataptr(vmtBufP, VMT_OBJECT), devName);
    nextOffset += devNameSize;

    /* Mount point */
    vmtData[VMT_STUB].vmt_off = nextOffset;
    vmtData[VMT_STUB].vmt_size = mpSize;
    (void)strcpy(vmt2dataptr(vmtBufP, VMT_STUB), mp);
    nextOffset += mpSize;

    /* Host and host name */
    vmtData[VMT_HOST].vmt_off = vmtData[VMT_HOST].vmt_size = 0;
    vmtData[VMT_HOSTNAME].vmt_off = vmtData[VMT_HOSTNAME].vmt_size = 0;

    /* FS-specific data */
    vmtData[VMT_INFO].vmt_off = nextOffset;
    vmtData[VMT_INFO].vmt_size = dataSize;
    (void)bcopy(data, vmt2dataptr(vmtBufP, VMT_INFO), dataSize);
    nextOffset += dataSize;

    /* Options */
    vmtData[VMT_ARGS].vmt_off = nextOffset;
    vmtData[VMT_ARGS].vmt_size = optionsSize;
    strcpy(vmt2dataptr(vmtBufP, VMT_ARGS), localOptions);

    /* Do the mount */
    if (vmount(vmtBufP, vmtBufSize) == -1)
	code = errno;

    osi_Free(vmtBufP, vmtBufSize);

    return code;
}	/* osi_mount() */


int
osi_umount(const char* mp, int flags)
{
    struct statfs	statfsBuf;

    if (statfs(mp, &statfsBuf) == -1)
	return errno;

    if (uvmount(statfsBuf.f_vfsnumber, flags) == -1)
	return errno;

    return 0;
}	/* osi_unmount() */


static char*
hasopt(const char* optStr, const char* opt)
{
    unsigned	optlen = strlen(opt);
    char*	c = optStr - 1;		/* Account for first ++ below */

    do {
	if (strncmp(opt, ++c, optlen) == 0 &&
	    (c[optlen] == '\0' || c[optlen] == ','))
	    return c;
    } while (c = strchr(c, ','));

    return NULL;
}	/* hasopt() */
