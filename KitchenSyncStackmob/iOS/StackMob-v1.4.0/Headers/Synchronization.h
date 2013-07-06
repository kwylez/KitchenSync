/*
 * Copyright 2012-2013 StackMob
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import "SMQuery.h"
#import "SMDataStore.h"

typedef void (^SynchronousQuerySuccessBlock)(NSArray *results);
typedef void (^SynchronousQueryFailureBlock)(NSError *error);

void synchronousQuery(SMDataStore *sm, SMQuery *query, SynchronousQuerySuccessBlock successBlock, SynchronousQueryFailureBlock failureBlock);

void syncWithSemaphore(void (^block)(dispatch_semaphore_t semaphore));

void syncReturn(dispatch_semaphore_t semaphore);

/**
 The `Synchronization` class provides helper methods for making synchronous calls to StackMob.  This is done because Core Data makes all calls to it's persistent store synchronously, therefore we must do the same with StackMob.
 */
@interface Synchronization : NSObject

@end

