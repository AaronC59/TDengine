/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _TD_VNODE_TQ_H_
#define _TD_VNODE_TQ_H_

#include "vnodeInt.h"

#include "executor.h"
#include "os.h"
#include "thash.h"
#include "tmsg.h"
#include "tqueue.h"
#include "trpc.h"
#include "ttimer.h"
#include "wal.h"

#ifdef __cplusplus
extern "C" {
#endif

// tqDebug ===================
// clang-format off
#define tqFatal(...) do { if (tqDebugFlag & DEBUG_FATAL) { taosPrintLog("TQ  FATAL ", DEBUG_FATAL, 255, __VA_ARGS__); }}     while(0)
#define tqError(...) do { if (tqDebugFlag & DEBUG_ERROR) { taosPrintLog("TQ  ERROR ", DEBUG_ERROR, 255, __VA_ARGS__); }}     while(0)
#define tqWarn(...)  do { if (tqDebugFlag & DEBUG_WARN)  { taosPrintLog("TQ  WARN ", DEBUG_WARN, 255, __VA_ARGS__); }}       while(0)
#define tqInfo(...)  do { if (tqDebugFlag & DEBUG_INFO)  { taosPrintLog("TQ  ", DEBUG_INFO, 255, __VA_ARGS__); }}            while(0)
#define tqDebug(...) do { if (tqDebugFlag & DEBUG_DEBUG) { taosPrintLog("TQ  ", DEBUG_DEBUG, tqDebugFlag, __VA_ARGS__); }} while(0)
#define tqTrace(...) do { if (tqDebugFlag & DEBUG_TRACE) { taosPrintLog("TQ  ", DEBUG_TRACE, tqDebugFlag, __VA_ARGS__); }} while(0)

// clang-format on

typedef struct STqOffsetStore STqOffsetStore;

// tqPush

typedef struct {
  // msg info
  int64_t consumerId;
  int64_t reqOffset;
  int64_t processedVer;
  int32_t epoch;
  // rpc info
  int64_t        reqId;
  SRpcHandleInfo rpcInfo;
  tmr_h          timerId;
  int8_t         tmrStopped;
  // exec
  int8_t       inputStatus;
  int8_t       execStatus;
  SStreamQueue inputQ;
  SRWLatch     lock;
} STqPushHandle;

// tqExec

typedef struct {
  char* qmsg;   // SubPlanToString
} STqExecCol;

typedef struct {
  int64_t suid;
} STqExecTb;

typedef struct {
  SHashObj* pFilterOutTbUid;
} STqExecDb;

typedef struct {
  int8_t      subType;
  STqReader*  pExecReader;
  qTaskInfo_t task;
  union {
    STqExecCol execCol;
    STqExecTb  execTb;
    STqExecDb  execDb;
  };
  int32_t numOfCols;  // number of out pout column, temporarily used
  bool         stop;  // denote if needs to be stopped or not
} STqExecHandle;

typedef struct {
  char          subKey[TSDB_SUBSCRIBE_KEY_LEN];
  int64_t       consumerId;
  int32_t       epoch;
  int8_t        fetchMeta;
  int64_t       snapshotVer;
  SWalReader*   pWalReader;
  SWalRef*      pRef;
  STqPushHandle pushHandle;    // push
  STqExecHandle execHandle;    // exec
} STqHandle;

typedef struct {
  SMqDataRsp     dataRsp;
  char           subKey[TSDB_SUBSCRIBE_KEY_LEN];
  SRpcHandleInfo pInfo;
} STqPushEntry;

struct STQ {
  SVnode* pVnode;
  char*   path;
  int64_t walLogLastVer;

  SRWLatch pushLock;

  SHashObj* pPushMgr;    // consumerId -> STqPushEntry
  SHashObj* pHandle;     // subKey -> STqHandle
  SHashObj* pCheckInfo;  // topic -> SAlterCheckInfo

  STqOffsetStore* pOffsetStore;

  TDB* pMetaDB;
  TTB* pExecStore;
  TTB* pCheckStore;

  SStreamMeta* pStreamMeta;
};

typedef struct {
  int8_t inited;
  tmr_h  timer;
} STqMgmt;

static STqMgmt tqMgmt = {0};

int32_t tEncodeSTqHandle(SEncoder* pEncoder, const STqHandle* pHandle);
int32_t tDecodeSTqHandle(SDecoder* pDecoder, STqHandle* pHandle);

// tqRead
int32_t tqScanTaosx(STQ* pTq, const STqHandle* pHandle, STaosxRsp* pRsp, SMqMetaRsp* pMetaRsp, STqOffsetVal* offset);
int32_t tqScanData(STQ* pTq, const STqHandle* pHandle, SMqDataRsp* pRsp, STqOffsetVal* pOffset);
int64_t tqFetchLog(STQ* pTq, STqHandle* pHandle, int64_t* fetchOffset, SWalCkHead** pHeadWithCkSum);

// tqExec
int32_t tqTaosxScanLog(STQ* pTq, STqHandle* pHandle, SPackedData submit, STaosxRsp* pRsp);
// int32_t tqTaosxScanLog(STQ* pTq, STqHandle* pHandle, SSubmitReq* pReq, STaosxRsp* pRsp);
int32_t tqAddBlockDataToRsp(const SSDataBlock* pBlock, SMqDataRsp* pRsp, int32_t numOfCols, int8_t precision);
int32_t tqSendDataRsp(STQ* pTq, const SRpcMsg* pMsg, const SMqPollReq* pReq, const SMqDataRsp* pRsp);
int32_t tqPushDataRsp(STQ* pTq, STqPushEntry* pPushEntry);

// tqMeta
int32_t tqMetaOpen(STQ* pTq);
int32_t tqMetaClose(STQ* pTq);
int32_t tqMetaSaveHandle(STQ* pTq, const char* key, const STqHandle* pHandle);
int32_t tqMetaDeleteHandle(STQ* pTq, const char* key);
int32_t tqMetaRestoreHandle(STQ* pTq);
int32_t tqMetaSaveCheckInfo(STQ* pTq, const char* key, const void* value, int32_t vLen);
int32_t tqMetaDeleteCheckInfo(STQ* pTq, const char* key);
int32_t tqMetaRestoreCheckInfo(STQ* pTq);

typedef struct {
  int32_t size;
} STqOffsetHead;

STqOffsetStore* tqOffsetOpen();
void            tqOffsetClose(STqOffsetStore*);
STqOffset*      tqOffsetRead(STqOffsetStore* pStore, const char* subscribeKey);
int32_t         tqOffsetWrite(STqOffsetStore* pStore, const STqOffset* pOffset);
int32_t         tqOffsetDelete(STqOffsetStore* pStore, const char* subscribeKey);
int32_t         tqOffsetCommitFile(STqOffsetStore* pStore);

// tqSink
int32_t tqBuildDeleteReq(SVnode* pVnode, const char* stbFullName, const SSDataBlock* pDataBlock,
                         SBatchDeleteReq* deleteReq);
void    tqSinkToTablePipeline2(SStreamTask* pTask, void* vnode, int64_t ver, void* data);

// tqOffset
char*   tqOffsetBuildFName(const char* path, int32_t fVer);
int32_t tqOffsetRestoreFromFile(STqOffsetStore* pStore, const char* fname);

// tqStream
int32_t tqExpandTask(STQ* pTq, SStreamTask* pTask, int64_t ver);

#ifdef __cplusplus
}
#endif

#endif /*_TD_VNODE_TQ_H_*/
