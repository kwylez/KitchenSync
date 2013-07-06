//
//  FFLocalQueuedOperationStore.h
//  FF-IOS-Framework
//
//  Copyright (c) 2012 FatFractal, Inc. All rights reserved.
//
//

#import <Foundation/Foundation.h>
#import <sqlite3.h>
#import "FFQueuedOperation.h"


#define SQL_DB_FILENAME         @"queuedOperations"
#define SQL_TABLE               @"OPERATIONS"
#define SQL_TIMESTAMP_FIELD     @"TIMESTAMP"
#define SQL_METHOD_FIELD        @"METHOD"
#define SQL_OBJ_FIELD           @"OBJECT"
#define SQL_OBJBLOBS_FIELD      @"OBJBLOBS"
#define SQL_URI_FIELD           @"URI"
#define SQL_BLOB_SIZE_FIELD     @"BLOBSIZE"
#define SQL_BLOB_FIELD          @"BLOB"
#define SQL_MIME_FIELD          @"MIME"
#define SQL_MEMBERNAME_FIELD    @"MEMBERNAME"
#define SQL_SESSIONID_FIELD     @"SESSIONID"
#define SQL_USERGUID_FIELD      @"USERGUID"

typedef enum {
    kSQLRowPositionTimestamp = 0,
    kSQLRowPositionMethod,
    kSQLRowPositionObj,
    kSQLRowPositionObjBlobs,
    kSQLRowPositionUri,
    kSQLRowPositionBlobSize,
    kSQLRowPositionBlob,
    kSQLRowPositionMimeType,
    kSQLRowPositionMemberName,
    kSQLRowPositionSessionId,
    kSQLRowPositionUserGuid
} SQLRowPosition;

@class FatFractal;

/** \brief Class which interacts with a local queued operation store database.
 
 This class works with a FatFractal instance to persist queued operations to a local database.
 */
@interface FFLocalQueuedOperationStore : NSObject

@property (readonly)    NSString    *localQueueDBPath;

/**
 Designated initializer.
 @param ff  FatFractal instance associated with the store
 @param key Unique key used to identify the database on disk
 */
- (id)initWithFF:(FatFractal *)ff key:(NSString *)key;

- (void)wipeDB;

/**
 Load operations from the local database.
 @return Array of FFQueuedOperation objects read from the local database.
 */
- (NSMutableArray *)loadQueuedOperationsFromLocalStorage;

/**
 Store an operation to the local database.
 @param op  The operation to store.
 */
- (void)storeQueuedOperationToLocalStorage:(FFQueuedOperation *)op;

/**
 Remove an operation from the local database.
 @param op  The operation to remove.
 */
- (void)removeQueuedOperationFromLocalStorage:(FFQueuedOperation *)op;

@end
