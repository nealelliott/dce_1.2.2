/*
 * Copyright (c) 1994, Transarc Corp.
 */                                                                               

#ifndef TRANSARC_TKM_VOL_H
#define TRANSARC_TKM_VOL_H
#include "tkm_internal.h"

extern tkm_vol_t *tkm_FindVol(afsFid *id, int add);
extern void tkm_AddFileToVolume(tkm_file_t *file, tkm_vol_t  *vol);
extern void tkm_RemoveFileFromVolume(tkm_file_t *file);
extern void tkm_HoldVol(tkm_vol_t *vol);
extern void tkm_ReleVol(tkm_vol_t *vol);
extern void tkm_InitVol();
extern int tkm_SetVolMax(int newMax);
extern tkm_vol_t *tkm_GetNextVol(tkm_vol_t *volP);
extern tkm_vol_t *tkm_GetFirstVol();
extern void tkm_FastHoldVol(tkm_vol_t *vol);
extern void tkm_FastReleVol(tkm_vol_t *vol);
#endif /*TRANSARC_TKM_VOL_H*/






