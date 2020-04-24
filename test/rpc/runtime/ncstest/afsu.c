/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: afsu.c,v $
 * Revision 1.1.8.2  1996/02/17  23:23:38  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:06  marty]
 *
 * Revision 1.1.8.1  1995/12/11  20:14:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:47:25  root]
 * 
 * Revision 1.1.6.1  1993/10/15  17:00:25  tom
 * 	Bug 8303 - Change ncstest_afs_foo to be a globalref to avoid
 * 	conflict with globaldef in afs_cstub.c.
 * 	[1993/10/14  22:48:50  tom]
 * 
 * Revision 1.1.4.2  1993/01/13  19:54:55  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:22:45  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  13:48:51  rsalz
 * 	Added as part of rpc6 drop.
 * 	[1992/04/30  23:22:53  rsalz]
 * 
 * Revision 1.2  1992/01/19  22:14:12  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
 */
/*
 *  OSF DCE Version 1.0 
 */
/*
 * Some ncstest afs test utility stuff...
 */

#include <afsu.h>

/*
 * This is some grunge to deal with a nidl bug (see afs.acf).
 */
globalref handle_t ncstest_afs_foo;


/*
 * Some canned data...
 */

globaldef afsHyper ncstest_afsHyper_arg = {
    /* unsigned long high; */               0x11111110,
    /* unsigned long low; */                0x01000001
};

globaldef afsFid ncstest_afsFid_arg = {
    /* afsHyper Cell; */                   {0x22222220,
                                            0x02000001},
    /* afsHyper Volume; */                 {0x02000002,
                                            0x02000003}, 
    /* unsigned long Vnode; */              0x02000004,
    /* unsigned long Unique; */             0x02000005
};

globaldef afsToken ncstest_afsToken_arg = {
    /* afsHyper         tokenID; */        {0x33333330,
                                            0x03000001},
    /* unsigned long    expirationTime; */  0x03000002,
    /* afsHyper         type; */           {0x03000003,
                                            0x03000004},
    /* unsigned long    beginRange; */      0x03000005,
    /* unsigned long    endRange; */        0x03000006
};

globaldef NameString_t ncstest_NameString_arg = {"This is a NameString String"};

globaldef PathString_t ncstest_PathString_arg = {"This is a PathString String"};

globaldef afsVolSync ncstest_afsVolSync_arg = {
    /* afsHyper         VolID; */          {0x44444440,
                                            0x04000001},
    /* afsHyper         VV; */             {0x04000002,
                                            0x04000003},
    /* unsigned long    VVAge; */           0x04000001,
    /* unsigned long    VVPingAge; */       0x04000002
};

globaldef afsFetchStatus ncstest_afsFetchStatus_arg = {
    /* unsigned long    interfaceVersion;*/ 0x55555550,
    /* unsigned long    fileType; */        0x05000001,
    /* unsigned long    linkCount; */       0x05000002,
    /* unsigned long    length; */          0x05000003,
    /* afsHyper         dataVersion; */    {0x05000004,
                                            0x05000005},
    /* unsigned long    author; */          0x05000006,
    /* unsigned long    owner; */           0x05000007,
    /* unsigned long    group; */           0x05000008,
    /* unsigned long    callerAccess; */    0x05000009,
    /* unsigned long    anonymousAccess; */ 0x0500000a,
    /* unsigned long    mode; */            0x0500000b,
    /* unsigned long    parentVnode; */     0x0500000c,
    /* unsigned long    parentUnique; */    0x0500000d,
    /* afsTimeval       modTime; */        {0x0500000e,
                                            0x0500000f},
    /* afsTimeval       changeTime; */     {0x05000010,
                                            0x05000011},
    /* afsTimeval       accessTime; */     {0x05000012,
                                            0x05000013},
    /* afsTimeval       serverModTime; */  {0x05000014,
                                            0x05000015}, 
    /* afsUUID          typeUUID; */        {0x45e50998, 
                                            0xc152, 0x11c9, 
                                            0xa8, 0x5f, {0x8, 0x0, 
                                            0x1e, 0x2, 0x1b, 0x34}},
    /* afsUUID          objectUUID; */     {0xcdfebef0, 
                                            0xc151, 0x11c9, 
                                            0x8f, 0xc1, {0x8, 0x0, 
                                            0x1e, 0x2, 0x29, 0x36}}, 
    /* unsigned long    spare0; */          0x05000016,
    /* unsigned long    spare1; */          0x05000017,
    /* unsigned long    spare2; */          0x05000018,
    /* unsigned long    spare3; */          0x05000019,
    /* unsigned long    spare4; */          0x0500001a,
    /* unsigned long    spare5; */          0x0500001b,
    /* unsigned long    spare6; */          0x0500001c,
    /* unsigned long    spare7; */          0x0500001d
};

globaldef afsStoreStatus ncstest_afsStoreStatus_arg = {
    /* unsigned long    mask; */            0x66666660,
    /* afsTimeval       modTime; */        {0x06000001,
                                            0x06000002},
    /* afsTimeval       accessTime; */     {0x06000003,
                                            0x06000004},
    /* afsTimeval       changeTime; */     {0x06000005,
                                            0x06000006},
    /* unsigned long    owner; */           0x06000007,
    /* unsigned long    group; */           0x06000008,
    /* unsigned long    mode; */            0x06000009,
    /* unsigned long    length; */          0x0600000a,
    /* afsUUID          typeUUID; */       {0x49e43770, 
                                            0xc153, 0x11c9, 
                                            0xb5, 0x4a, {0x8, 0x0, 
                                            0x1e, 0x1, 0xd6, 0xd5}},
    /* unsigned long    spare1; */          0x0600000b,
    /* unsigned long    spare2; */          0x0600000c,
    /* unsigned long    spare3; */          0x0600000d,
    /* unsigned long    spare4; */          0x0600000e,
    /* unsigned long    spare5; */          0x0600000f
};


/* the following are initialized elsewhere */
static afsVolSync ncstest_afsBulkVVs_VolSync[3];
globaldef afsBulkVVs ncstest_afsBulkVVs_arg;
static afsHyper ncstest_afsBulkVolIDs_Hyper[3];
globaldef afsBulkVolIDs ncstest_afsBulkVolIDs_arg;


/*
 * Some data type comparison routines...
 * (could be more efficient, but this way allows easier debugging of
 * broken clients / servers)
 */

int ncstest_afsTimeval_diff(ds1,ds2)
afsTimeval  *ds1, *ds2;
{
    int r = 0;

    r = r || (ds1->sec != ds2->sec);
    r = r || (ds1->usec != ds2->usec);

    return r;
}

int ncstest_afsHyper_diff(ds1,ds2)
afsHyper  *ds1, *ds2;
{
    int r = 0;

    r = r || (ds1->high != ds2->high);
    r = r || (ds1->low != ds2->low);

    return r;
}

int ncstest_afsUUID_diff(ds1,ds2)
afsUUID  *ds1, *ds2;
{
    int r = 0;
    unsigned32 st;

    r = !uuid_equal(ds1, ds2, &st);

    return r;
}

int ncstest_afsFid_diff(ds1,ds2)
afsFid  *ds1, *ds2;
{
    int r = 0;

    r = r || ncstest_afsHyper_diff(&ds1->Cell, &ds2->Cell);
    r = r || ncstest_afsHyper_diff(&ds1->Volume, &ds2->Volume);
    r = r || (ds1->Vnode != ds2->Vnode);
    r = r || (ds1->Unique != ds2->Unique);

    return r;
}

int ncstest_afsFetchStatus_diff(ds1,ds2)
afsFetchStatus  *ds1, *ds2;
{
    int r = 0;

    r = r || (ds1->interfaceVersion != ds2->interfaceVersion);
    r = r || (ds1->fileType != ds2->fileType);
    r = r || (ds1->linkCount != ds2->linkCount);
    r = r || (ds1->length != ds2->length);
    r = r || ncstest_afsHyper_diff(&ds1->dataVersion, &ds2->dataVersion);
    r = r || (ds1->author != ds2->author);
    r = r || (ds1->owner != ds2->owner);
    r = r || (ds1->group != ds2->group);
    r = r || (ds1->callerAccess != ds2->callerAccess);
    r = r || (ds1->anonymousAccess != ds2->anonymousAccess);
    r = r || (ds1->mode != ds2->mode);
    r = r || (ds1->parentVnode != ds2->parentVnode);
    r = r || (ds1->parentUnique != ds2->parentUnique);
    r = r || ncstest_afsTimeval_diff(&ds1->modTime, &ds2->modTime);
    r = r || ncstest_afsTimeval_diff(&ds1->changeTime, &ds2->changeTime);
    r = r || ncstest_afsTimeval_diff(&ds1->accessTime, &ds2->accessTime);
    r = r || ncstest_afsTimeval_diff(&ds1->serverModTime, &ds2->serverModTime);
    r = r || ncstest_afsUUID_diff(&ds1->typeUUID, &ds2->typeUUID);
    r = r || ncstest_afsUUID_diff(&ds1->objectUUID, &ds2->objectUUID);
    r = r || (ds1->spare0 != ds2->spare0);
    r = r || (ds1->spare1 != ds2->spare1);
    r = r || (ds1->spare2 != ds2->spare2);
    r = r || (ds1->spare3 != ds2->spare3);
    r = r || (ds1->spare4 != ds2->spare4);
    r = r || (ds1->spare5 != ds2->spare5);
    r = r || (ds1->spare6 != ds2->spare6);
    r = r || (ds1->spare7 != ds2->spare7);

    return r;
}

int ncstest_afsStoreStatus_diff(ds1,ds2)
afsStoreStatus  *ds1, *ds2;
{
    int r = 0;

    r = r || (ds1->mask != ds2->mask);
    r = r || ncstest_afsTimeval_diff(&ds1->modTime, &ds2->modTime);
    r = r || ncstest_afsTimeval_diff(&ds1->accessTime, &ds2->accessTime);
    r = r || ncstest_afsTimeval_diff(&ds1->changeTime, &ds2->changeTime);
    r = r || (ds1->owner != ds2->owner);
    r = r || (ds1->group != ds2->group);
    r = r || (ds1->mode != ds2->mode);
    r = r || (ds1->length != ds2->length);
    r = r || ncstest_afsUUID_diff(&ds1->typeUUID, &ds2->typeUUID);
    r = r || (ds1->spare1 != ds2->spare1);
    r = r || (ds1->spare2 != ds2->spare2);
    r = r || (ds1->spare3 != ds2->spare3);
    r = r || (ds1->spare4 != ds2->spare4);
    r = r || (ds1->spare5 != ds2->spare5);

    return r;
}

int ncstest_afsToken_diff(ds1,ds2)
afsToken  *ds1, *ds2;
{
    int r = 0;

    r = r || ncstest_afsHyper_diff(&ds1->tokenID, &ds2->tokenID);
    r = r || (ds1->expirationTime != ds2->expirationTime);
    r = r || ncstest_afsHyper_diff(&ds1->type, &ds2->type);
    r = r || (ds1->beginRange != ds2->beginRange);
    r = r || (ds1->endRange != ds2->endRange);

    return r;
}

int ncstest_afsVolSync_diff(ds1,ds2)
afsVolSync  *ds1, *ds2;
{
    int r = 0;

    r = r || ncstest_afsHyper_diff(&ds1->VolID, &ds2->VolID);
    r = r || ncstest_afsHyper_diff(&ds1->VV, &ds2->VV);
    r = r || (ds1->VVAge != ds2->VVAge);
    r = r || (ds1->VVPingAge != ds2->VVPingAge);

    return r;
}

int ncstest_PathString_diff(ds1,ds2)
PathString_t  ds1, ds2;
{
    return (strcmp(ds1, ds2));
}

int ncstest_NameString_diff(ds1,ds2)
NameString_t  ds1, ds2;
{
    return (strcmp(ds1, ds2));
}

int ncstest_afsBulkVVs_diff(ds1,ds2)
afsBulkVVs  *ds1, *ds2;
{
    int i;

    if (ds1->afsBulkVVs_len != ds2->afsBulkVVs_len)
        return 1;

    for (i = 0; i < ds1->afsBulkVVs_len; i++)
        if (ncstest_afsVolSync_diff(&ds1->afsBulkVVs_val[i],
                                    &ds2->afsBulkVVs_val[i]))
            return 1;

    return 0;
}

int ncstest_afsBulkVolIDs_diff(ds1,ds2)
afsBulkVolIDs  *ds1, *ds2;
{
    int i;

    if (ds1->afsBulkVolIDs_len != ds2->afsBulkVolIDs_len)
        return 1;

    for (i = 0; i < ds1->afsBulkVolIDs_len; i++)
        if (ncstest_afsHyper_diff(&ds1->afsBulkVolIDs_val[i],
                                  &ds2->afsBulkVolIDs_val[i]))
            return 1;

    return 0;
}


/*
 * Some test initialization stuff...
 */

void ncstest_afs_BulkVVs_3_init(ds)
afsBulkVVs *ds;
{
    int i;

    ncstest_afsBulkVVs_VolSync[0] = ncstest_afsVolSync_arg;
    ncstest_afsBulkVVs_VolSync[0].VolID.high = 0x77777770;

    ncstest_afsBulkVVs_VolSync[1] = ncstest_afsVolSync_arg;
    ncstest_afsBulkVVs_VolSync[1].VolID.high = 0x88888880;

    ncstest_afsBulkVVs_VolSync[2] = ncstest_afsVolSync_arg;
    ncstest_afsBulkVVs_VolSync[2].VolID.high = 0x99999990;

    ds->afsBulkVVs_len = 3;
    for (i=0; i<ds->afsBulkVVs_len; i++)
        ds->afsBulkVVs_val[i] = ncstest_afsBulkVVs_VolSync[i];
}

void ncstest_afs_BulkVolIDs_3_init(ds)
afsBulkVolIDs *ds;
{
    int i;

    ncstest_afsBulkVolIDs_Hyper[0] = ncstest_afsHyper_arg;
    ncstest_afsBulkVolIDs_Hyper[0].high = 0xaaaaaaa0;

    ncstest_afsBulkVolIDs_Hyper[1] = ncstest_afsHyper_arg;
    ncstest_afsBulkVolIDs_Hyper[1].high = 0xbbbbbbb0;

    ncstest_afsBulkVolIDs_Hyper[2] = ncstest_afsHyper_arg;
    ncstest_afsBulkVolIDs_Hyper[2].high = 0xccccccc0;

    ds->afsBulkVolIDs_len = 3;
    for (i=0; i<ds->afsBulkVolIDs_len; i++)
        ds->afsBulkVolIDs_val[i] = ncstest_afsBulkVolIDs_Hyper[i];
}


static ncstest_afs_needs_init = true;
void ncstest_afstest_init(is_server)
int is_server;
{
    if (!ncstest_afs_needs_init)
        return;

    ncstest_afs_needs_init = false;
    ncstest_afs_BulkVVs_3_init(&ncstest_afsBulkVVs_arg);
    ncstest_afs_BulkVolIDs_3_init(&ncstest_afsBulkVolIDs_arg);
}
