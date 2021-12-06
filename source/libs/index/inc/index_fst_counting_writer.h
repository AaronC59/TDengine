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

#ifndef __INDEX_FST_COUNTING_WRITER_H__
#define __INDEX_FST_COUNTING_WRITER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tfile.h"


#define DefaultMem  1024*1024

static char tmpFile[] = "./index";
typedef enum WriterType {TMemory, TFile} WriterType;   

typedef struct WriterCtx {
  int (*write)(struct WriterCtx *ctx, uint8_t *buf, int len);
  int (*read)(struct WriterCtx *ctx, uint8_t *buf, int len);
  int (*flush)(struct WriterCtx *ctx);
  WriterType type; 
  union {
    int fd;
    void *mem;
  };
  int32_t offset;
  int32_t limit;
} WriterCtx;

static int writeCtxDoWrite(WriterCtx *ctx, uint8_t *buf, int len);
static int writeCtxDoRead(WriterCtx *ctx, uint8_t *buf, int len);
static int writeCtxDoFlush(WriterCtx *ctx);

WriterCtx* writerCtxCreate(WriterType type);
void writerCtxDestroy(WriterCtx *w);

typedef uint32_t CheckSummer;


typedef struct FstCountingWriter {
  void*    wrt;  // wrap any writer that counts and checksum bytes written 
  uint64_t count; 
  CheckSummer summer;  
} FstCountingWriter;

int fstCountingWriterWrite(FstCountingWriter *write, uint8_t *buf, uint32_t bufLen); 

int fstCountingWriterFlush(FstCountingWriter *write);


uint32_t fstCountingWriterMaskedCheckSum(FstCountingWriter *write);

FstCountingWriter *fstCountingWriterCreate(void *wtr);
void fstCountingWriterDestroy(FstCountingWriter *w); 


void fstCountingWriterPackUintIn(FstCountingWriter *writer, uint64_t n,  uint8_t nBytes);
uint8_t fstCountingWriterPackUint(FstCountingWriter *writer, uint64_t n);


#define FST_WRITER_COUNT(writer) (writer->count)
#define FST_WRITER_INTER_WRITER(writer) (writer->wtr)
#define FST_WRITE_CHECK_SUMMER(writer) (writer->summer)

#ifdef __cplusplus
}
#endif

#endif


