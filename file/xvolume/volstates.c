/* Copyright (C) 1994 Transarc Corporation - All rights reserved */

/*
 * HISTORY
 * $Log: volclient.c,v $
 * $EndLog$
 */

#include <dcedfs/stds.h>
#include <dcedfs/volume.h>

typedef struct vol_bits {
    char*		name;
    unsigned long	mask;
    unsigned long	value;
}	vol_bits_t;

static vol_bits_t volBits[] = {
    {"RW",		VOL_TYPEBITS,		VOL_RW},
    {"READONLY",	VOL_TYPEBITS,		VOL_READONLY},
    {"BUSY",		VOL_BUSY,		VOL_BUSY},
    {"OFFLINE",		VOL_OFFLINE,		VOL_OFFLINE},
    {"DELONSALVAGE",	VOL_DELONSALVAGE,	VOL_DELONSALVAGE},
    {"OUTOFSERVICE",	VOL_OUTOFSERVICE,	VOL_OUTOFSERVICE},
    {"DEADMEAT",	VOL_DEADMEAT,		VOL_DEADMEAT},
    {"LCLMOUNT",	VOL_LCLMOUNT,		VOL_LCLMOUNT},
    {"REP_RELEASE",	VOL_REPFIELD,		VOL_REP_RELEASE},
    {"REP_LAZY",	VOL_REPFIELD,		VOL_REP_LAZY},
    {"IS_COMPLETE",	VOL_IS_COMPLETE,	VOL_IS_COMPLETE},
    {"HAS_TOKEN",	VOL_HAS_TOKEN,		VOL_HAS_TOKEN},
    {"unused (0x4000)",	0x00004000,		0x00004000},
    {"NOEXPORT",	VOL_NOEXPORT,		VOL_NOEXPORT},
    {"TYPE_RW",		VOL_TYPEFIELD,		VOL_TYPE_RW},
    {"TYPE_RO",		VOL_TYPEFIELD,		VOL_TYPE_RO},
    {"TYPE_BK",		VOL_TYPEFIELD,		VOL_TYPE_BK},
    {"TYPE_TEMP",	VOL_TYPEFIELD,		VOL_TYPE_TEMP},
    {"GRABWAITING",	VOL_GRABWAITING,	VOL_GRABWAITING},
    {"LOOKUPWAITING",	VOL_LOOKUPWAITING,	VOL_LOOKUPWAITING},
    {"REPSERVER_MGD",	VOL_REPSERVER_MGD,	VOL_REPSERVER_MGD},
    {"MOVE_TARGET",	VOL_MOVE_TARGET,	VOL_MOVE_TARGET},
    {"MOVE_SOURCE",	VOL_MOVE_SOURCE,	VOL_MOVE_SOURCE},
    {"ZAPME",		VOL_ZAPME,		VOL_ZAPME},
    {"CLONEINPROG",	VOL_CLONEINPROG,	VOL_CLONEINPROG},
    {"IS_REPLICATED",	VOL_IS_REPLICATED,	VOL_IS_REPLICATED},
    {"OPENDONE",	VOL_OPENDONE,		VOL_OPENDONE},
    {NULL}
};


main(int argc, char* argv[])
{
    unsigned long	maskedBits;
    unsigned long	knownBits;
    char*		statesStr;
    unsigned long	statesVal;
    unsigned long	unknownBits;
    vol_bits_t*		vbp;

    while (--argc) {
	statesStr = *++argv;
	statesVal = strtol(statesStr, NULL, 0);

	(void)printf("%s=", statesStr);
	
	knownBits = 0;

	for (vbp = volBits; vbp->name != NULL; vbp++) {
	    maskedBits = (statesVal & vbp->mask);

	    if (maskedBits == vbp->value) {
		knownBits |= maskedBits;
		(void)printf("%s ", vbp->name);
	    }
	}

	unknownBits = (~knownBits & statesVal);
	if (unknownBits)
	    (void)printf("(unknown bits: %#lx)", unknownBits);

	(void)putchar('\n');
    }

    return 0;
}
