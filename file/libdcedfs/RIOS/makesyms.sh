#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: makesyms.sh,v $
# Revision 1.1.9.1  1996/10/02  17:54:09  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:42:01  damon]
#
# Revision 1.1.4.1  1994/06/09  14:13:02  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:26:18  annie]
# 
# Revision 1.1.2.2  1993/01/19  16:07:45  cjd
# 	embedded copyright notice
# 	[1993/01/19  15:11:07  cjd]
# 
# Revision 1.1  1992/01/19  02:59:18  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#
echo "VC_CreateVolume
VC_DeleteVolume
VC_BackupVolume
VC_DumpVolume
VC_RestoreVolume
VC_MoveVolume
VC_ReleaseVolume
VC_ListVolumes
VC_VolumeStatus
VC_SyncVldb
VC_RenameVolume
VC_SyncServer
VC_VolserStatus
VC_VolumeZap
VC_EndTrans
VC_SetQuota
VL__v4_0_c_ifspec
VL_GetEntryByID
VL_GetEntryByName
VL_Probe
VL_GetCellInfo
VL_GetNextServersByID
VL_GetNextServersByName
VL_GetSiteInfo
VL_GetCEntryByID
VL_GetCEntryByName
VL_GetCNextServersByID
VL_GetCNextServersByName
VL_ExpandSiteCookie
VL_GetServerInterfaces
VL_CreateEntry
VL_DeleteEntry
VL_GetNewVolumeId
VL_ReplaceEntry
VL_UpdateEntry
VL_SetLock
VL_ReleaseLock
VL_ListEntry
VL_ListByAttributes
VL_GetStats
VL_AddAddress
VL_RemoveAddress
VL_ChangeAddress
VL_GenerateSites
VL_GetNewVolumeIds
VL_CreateServer
VL_AlterServer
FTSERVER_v4_0_c_ifspec
FTSERVER_CreateTrans
FTSERVER_DeleteTrans
FTSERVER_CreateVolume
FTSERVER_DeleteVolume
FTSERVER_Dump
FTSERVER_Restore
FTSERVER_Forward
FTSERVER_Clone
FTSERVER_ReClone
FTSERVER_GetFlags
FTSERVER_SetFlags
FTSERVER_GetStatus
FTSERVER_SetStatus
FTSERVER_ListVolumes
FTSERVER_ListAggregates
FTSERVER_AggregateInfo
FTSERVER_Monitor
FTSERVER_GetOneVolStatus
FTSERVER_GetServerInterfaces
FTSERVER_v4_0_s_ifspec
BOSSVR_v0_0_c_ifspec
BOSSVR_ServerPlaceHolder
BOSSVR_CreateBnode
BOSSVR_DeleteBnode
BOSSVR_SetStatus
BOSSVR_GetStatus
BOSSVR_EnumerateInstance
BOSSVR_GetInstanceInfo
BOSSVR_GetInstanceParm
BOSSVR_AddSUser
BOSSVR_DeleteSUser
BOSSVR_ListSUsers
BOSSVR_ListKeys
BOSSVR_AddKey
BOSSVR_DeleteKey
BOSSVR_GenerateKey
BOSSVR_GarbageCollectKeys
BOSSVR_GetCellName
BOSSVR_SetTStatus
BOSSVR_ShutdownAll
BOSSVR_RestartAll
BOSSVR_StartupAll
BOSSVR_SetNoAuthFlag
BOSSVR_ReBossvr
BOSSVR_Restart
BOSSVR_Install
BOSSVR_UnInstall
BOSSVR_GetDates
BOSSVR_Exec
BOSSVR_Prune
BOSSVR_SetRestartTime
BOSSVR_GetRestartTime
BOSSVR_GetLog
BOSSVR_WaitAll
BOSSVR_SetDebug
BOSSVR_GetServerInterfaces
afs_syscall
pioctl" > libdcedfs.syms
