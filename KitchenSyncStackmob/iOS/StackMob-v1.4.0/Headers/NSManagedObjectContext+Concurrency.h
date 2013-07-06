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

#import <CoreData/CoreData.h>
#import "StackMob.h"

/**
 Category which provides methods for performing asynchronous callback-based saves and fetches.  Synchronous versions are also provided which wait for operations to complete before returning.
 
 **Important:** These methods are designed for use with managed object context instances obtained from `SMCoreDataStore,` as they take advantage of the parent / child context pattern.
 
 
 ## Saves ##
 
 The <saveOnSuccess:onFailure:> method is a callback-based method which will perform the save asynchronously, off of the main thread.  Callbacks will be performed on the main thread.  To specify queues to callbacks, use <saveWithSuccessCallbackQueue:failureCallbackQueue:onSuccess:onFailure:>.
 
 Saves methods work by nesting performBlock: calls, pushing save requests to the top of the chain, a private queue parent context with an initialized persistent store coordinator.
 
 The <saveAndWait:> method works similarly to the Core Data save: method, taking the parent / child pattern into account.
 
 ## Fetches ##
 
 The <executeFetchRequest:onSuccess:onFailure:> method is a callback-based method which will perform the fetch asynchronously, off of the main thread.  Callbacks will be performed on the main thread.
 
 Fetch methods work by copying the fetch over to a background context, which operates on a differnt queue and returns `NSManagedObjectID` isntances to the calling context.  Those IDs are then translated into faulted instances of `NSManagedObject` by the calling context, unless otherwise specified.  
 
 To specify whether to return instances of `NSManagedObject` or `NSManagedObjectID`, use <executeFetchRequest:returnManagedObjectIDs:onSuccess:onFailure:>.
 
 To specify queues to callbacks, use <executeFetchRequest:returnManagedObjectIDs:successCallbackQueue:failureCallbackQueue:onSuccess:onFailure:>.
 
 The <executeFetchRequestAndWait:error:> and <executeFetchRequestAndWait:returnManagedObjectIDs:error:> methods work similarly to the Core Data executeFetchRequest:error: method.
 
 ## Observing Contexts ##
 
 <observeContext:> and <stopObservingContext:> are helper methods which simply add / remove observers for `NSManagedObjectContextDidSaveNotification`, if you need to implement manual merging.
 
 ## Hooking Up to the Chain Of Contexts ##
 
 If you create your own context and make it a child of a context provided by <SMCoreDataStore>, and you plan to save on your created context, use <setContextShouldObtainPermanentIDsBeforeSaving:> so that permanent IDs for newly inserted objects are created on your child context level.  Otherwise objects in your context will appear to have temporary IDs even after they have been saved!
 <a name="pr_options"></a>
 ## Per Request Options ##
 
 Each type of method (asynchronous/synchronous save/fetch) has an overloadeded method declaration with a parameter that takes an instance of <SMRequestOptions>.  The parameter name in all methods is called <i>options</i>.  This allows you to provide a custom SMRequestOptions instance that will be applied to all calls in that request. For example, provide a SMRequestOptions instance with the <i>isSecure</i> property set to YES if you wanted a specific save request to run over SSL i.e. all inserts/updates/deletes for that request will be sent over SSL.
 
 **Note:** Not all options provided by the SMRequestOptions class are taken into account during save/fetch requests.  The following options are currently safe to customize and will override the default for the duration of the request:
 
 * isSecure property (HTTPS)
 
 Customizing other options can result in unexpected requests, which can lead to save/fetch failures.
 
 */
@interface NSManagedObjectContext (Concurrency)

/**
 Asynchronous save method.
 
 A callback based save method which pushes changes to private parent context and saves in the background, off of the main thread.  Callbacks are performed on the main thread.  Use <saveWithSuccessCallbackQueue:failureCallbackQueue:onSuccess:onFailure:> to specify queues to perform callbacks.
 
 @param successBlock <i>typedef void (^SMSuccessBlock)())</i> A block object to call on the main thread upon successful save of the managed object context.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i> A block object to call on the main thread upon unsuccessful save.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)saveOnSuccess:(SMSuccessBlock)successBlock onFailure:(SMFailureBlock)failureBlock;

/**
 Asynchronous save method with parameters for callback queues.
 
 A callback based save method which pushes changes to private parent context and saves in the background, off of the main thread.
 
 @param successCallbackQueue Upon successful save, the queue to perform the success block on.
 @param failureCallbackQueue Upon unsuccessful save, the queue to perform the failure block on.
 @param successBlock <i>typedef void (^SMSuccessBlock)())</i> A block object to call on the main thread upon successful save of the managed object context.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i> A block object to call on the main thread upon unsuccessful save.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)saveWithSuccessCallbackQueue:(dispatch_queue_t)successCallbackQueue failureCallbackQueue:(dispatch_queue_t)failureCallbackQueue onSuccess:(SMSuccessBlock)successBlock onFailure:(SMFailureBlock)failureBlock;

/**
 Asynchronous save method with parameters for callback queues and request options.
 
 A callback based save method which pushes changes to private parent context and saves in the background, off of the main thread.
 
 For more information on per request options, see the <a href="#pr_options">Per Request Options</a> section above.
 
 @param successCallbackQueue Upon successful save, the queue to perform the success block on.
 @param failureCallbackQueue Upon unsuccessful save, the queue to perform the failure block on.
 @param options Request options.
 @param successBlock <i>typedef void (^SMSuccessBlock)())</i> A block object to call on the main thread upon successful save of the managed object context.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i> A block object to call on the main thread upon unsuccessful save.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)saveWithSuccessCallbackQueue:(dispatch_queue_t)successCallbackQueue failureCallbackQueue:(dispatch_queue_t)failureCallbackQueue options:(SMRequestOptions *)options onSuccess:(SMSuccessBlock)successBlock onFailure:(SMFailureBlock)failureBlock;

/**
 Synchronous save method.
 
 This method works like the NSManagedObjectContext <i>save:</i> method, but pushes changes to private parent context which in turns saves to the persistent store.
 
 @param error Points to the error object if the save is unsuccessful.
 
 @return Whether the save was successful or not.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (BOOL)saveAndWait:(NSError *__autoreleasing*)error;

/**
 Synchronous save method with parameter for request options.
 
 This method works like the NSManagedObjectContext <i>save:</i> method, but pushes changes to private parent context which in turns saves to the persistent store.
 
 For more information on per request options, see the <a href="#pr_options">Per Request Options</a> section above.
 
 @param error Points to the error object if the save is unsuccessful.
 @param options Request options.
 
 @return Whether the save was successful or not.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (BOOL)saveAndWait:(NSError *__autoreleasing *)error options:(SMRequestOptions *)options;

/**
 Asynchronous fetch method.
 
 A callback based fetch method which executes fetch on a background context, off of the main thread.  Managed object IDs that are returned are converted to instances of NSManagedObject by the calling context.
 
 Callbacks are performed on the main thread.  Use <executeFetchRequest:returnManagedObjectIDs:successCallbackQueue:failureCallbackQueue:onSuccess:onFailure:> to specify queues to perform callbacks on.
 
 @param request The fetch request to perform against the database.
 @param successBlock <i>typedef void (^SMResultsSuccessBlock)(NSArray *results)</i> A block object to call on the main thread upon successful save of the managed object context, containing an array of results as instances of NSManagedObject.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i> A block object to call on the main thread upon unsuccessful save.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)executeFetchRequest:(NSFetchRequest *)request onSuccess:(SMResultsSuccessBlock)successBlock onFailure:(SMFailureBlock)failureBlock;

/**
 Asynchronous fetch method with the option of returning instances of NSManagedObjectID.
 
 A callback based fetch method which executes fetch on a background context, off of the main thread.  If returnIDs is YES, managed object IDs that are returned by the fetch are converted to instances of NSManagedObject by the calling context.
 
 Callbacks are performed on the main thread.  Use <executeFetchRequest:returnManagedObjectIDs:successCallbackQueue:failureCallbackQueue:onSuccess:onFailure:> to specify queues to perform callbacks on.
 
 @param request The fetch request to perform against the database.
 @param returnIDs Whether or not results should contain instances of NSManagedObject or NSManagedObjectID.
 @param successBlock <i>typedef void (^SMResultsSuccessBlock)(NSArray *results)</i> A block object to call on the main thread upon successful save of the managed object context.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i> A block object to call on the main thread upon unsuccessful save.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)executeFetchRequest:(NSFetchRequest *)request returnManagedObjectIDs:(BOOL)returnIDs onSuccess:(SMResultsSuccessBlock)successBlock onFailure:(SMFailureBlock)failureBlock;

/**
 Asynchronous fetch method with the option of returning instances of NSManagedObjectID as well as specifying callback queues.
 
 A callback based fetch method which executes fetch on a background context, off of the main thread.  If returnIDs is YES, managed object IDs that are returned by the fetch are converted to instances of NSManagedObject by the calling context.
 
 @param request The fetch request to perform against the database.
 @param returnIDs Whether or not results should contain instances of NSManagedObject or NSManagedObjectID.
 @param successCallbackQueue Upon successful fetch, the queue to perform the success block on.
 @param failureCallbackQueue Upon unsuccessful fetch, the queue to perform the failure block on.
 @param successBlock <i>typedef void (^SMResultsSuccessBlock)(NSArray *results)</i> A block object to call on the main thread upon successful save of the managed object context.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i> A block object to call on the main thread upon unsuccessful save.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)executeFetchRequest:(NSFetchRequest *)request returnManagedObjectIDs:(BOOL)returnIDs successCallbackQueue:(dispatch_queue_t)successCallbackQueue failureCallbackQueue:(dispatch_queue_t)failureCallbackQueue onSuccess:(SMResultsSuccessBlock)successBlock onFailure:(SMFailureBlock)failureBlock;

/**
 Asynchronous fetch method with the option of returning instances of NSManagedObjectID as well as specifying callback queues and providing request options.
 
 A callback based fetch method which executes fetch on a background context, off of the main thread.  If returnIDs is YES, managed object IDs that are returned by the fetch are converted to instances of NSManagedObject by the calling context.
 
 For more information on per request options, see the <a href="#pr_options">Per Request Options</a> section above.
 
 @param request The fetch request to perform against the database.
 @param returnIDs Whether or not results should contain instances of NSManagedObject or NSManagedObjectID.
 @param successCallbackQueue Upon successful fetch, the queue to perform the success block on.
 @param failureCallbackQueue Upon unsuccessful fetch, the queue to perform the failure block on.
 @param options Request options.
 @param successBlock <i>typedef void (^SMResultsSuccessBlock)(NSArray *results)</i> A block object to call on the main thread upon successful save of the managed object context.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i> A block object to call on the main thread upon unsuccessful save.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)executeFetchRequest:(NSFetchRequest *)request returnManagedObjectIDs:(BOOL)returnIDs successCallbackQueue:(dispatch_queue_t)successCallbackQueue failureCallbackQueue:(dispatch_queue_t)failureCallbackQueue options:(SMRequestOptions *)options onSuccess:(SMResultsSuccessBlock)successBlock onFailure:(SMFailureBlock)failureBlock;

/**
 Synchronous fetch method.
 
 This method works like the NSManagedObjectContext <i>executeFetchRequest:error:</i> method, but executes fetch request on background context.  Managed object IDs that are returned are converted to managed objects on the calling context.
 
 @param request The fetch to perform on the database.
 @param error Points to the error object if the fetch is unsuccessful.
 
 @return An array of NSManagedObject instances matching the request, nil if there was an error.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (NSArray *)executeFetchRequestAndWait:(NSFetchRequest *)request error:(NSError *__autoreleasing *)error;

/**
 Synchronous fetch method with the option to return results as instances of NSManagedObjectID.
 
 This method works like the NSManagedObjectContext <i>executeFetchRequest:error:</i> method, but executes fetch request on background context.  Managed object IDs that are returned are converted to managed objects on the calling context.
 
 @param request The fetch to perform on the database.
 @param returnIDs Whether or not results should contain instances of NSManagedObject or NSManagedObjectID.
 @param error Points to the error object if the fetch is unsuccessful.
 
 @return An array of NSManagedObject or NSManagedObjectID instances matching the request, nil if there was an error.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (NSArray *)executeFetchRequestAndWait:(NSFetchRequest *)request returnManagedObjectIDs:(BOOL)returnIDs error:(NSError *__autoreleasing *)error;

/**
 Synchronous fetch method with the option to return results as instances of NSManagedObjectID as well as provide request options.
 
 This method works like the NSManagedObjectContext <i>executeFetchRequest:error:</i> method, but executes fetch request on background context.  Managed object IDs that are returned are converted to managed objects on the calling context.
 
 For more information on per request options, see the <a href="#pr_options">Per Request Options</a> section above.
 
 @param request The fetch to perform on the database.
 @param returnIDs Whether or not results should contain instances of NSManagedObject or NSManagedObjectID.
 @param options Request options.
 @param error Points to the error object if the fetch is unsuccessful.
 
 @return An array of NSManagedObject or NSManagedObjectID instances matching the request, nil if there was an error.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (NSArray *)executeFetchRequestAndWait:(NSFetchRequest *)request returnManagedObjectIDs:(BOOL)returnIDs options:(SMRequestOptions *)options error:(NSError *__autoreleasing *)error;

/**
 Allows context to be notified when contextToObserve posts the NSManagedObjectContextDidSaveNotification notification.
 
 When notification arrives, mergeChangesFromContextDidSaveNotification: is called.
 
 @param contextToObserve The object to observe for notification posts.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)observeContext:(NSManagedObjectContext *)contextToObserve;

/**
 Removes context from observing NSManagedObjectContextDidSaveNotification notifications from contextToStopObserving.
 
 @param contextToStopObserving The object to stop observing for notification posts.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)stopObservingContext:(NSManagedObjectContext *)contextToStopObserving;

/**
 Adds/Removes observer for NSManagedObjectContextWillSaveNotification notification.
 
 When using the child / parent context pattern, child contexts should obtain permanent IDs for newly inserted objects before pushing save requests to parent contexts.  This method is automatically set to YES for mainThreadContext as well as private queue contexts that are created by contextForCurrentThread.
 
 @param value If YES, adds an observer for NSManagedObjectContextWillSaveNotification, which upon receiving a notification calls obtainPermanentIDsForObjects:, passing the context's inserted objects. 
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)setContextShouldObtainPermanentIDsBeforeSaving:(BOOL)value;

@end
