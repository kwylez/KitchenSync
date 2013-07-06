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

#import "SMDataStore.h"

extern NSString *const SMSetCachePolicyNotification;
extern BOOL SM_CACHE_ENABLED;

typedef enum {
    SMCachePolicyTryNetworkOnly = 0,
    SMCachePolicyTryCacheOnly  = 1,
    SMCachePolicyTryNetworkElseCache = 2,
    SMCachePolicyTryCacheElseNetwork = 3,
} SMCachePolicy;

@class SMIncrementalStore;

/**
 The `SMCoreDataStore` class provides all the necessary properties and methods to interact with StackMob's Core Data integration.
 
 ## Using SMCoreDataStore ##
 
 With your `SMCoreDataStore` object you can retrieve a managed object context configured with a `SMIncrementalStore` as it's persistent store to allow communication to StackMob from Core Data.  Obtain a managed object context for your thread using <contextForCurrentThread>.  You can obtain the managed object context for the main thread at any time with <mainThreadContext>.
 
 When saving or fetching from the context, use methods from the <NSManagedObjectContext+Concurrency> category to ensure proper asynchronous saving and fetching off of the main thread.
 
 If you want to do your own context creation, use the <persistentStoreCoordinator> property to ensure your objects are being saved to the StackMob server.
 
 The default merge policy set for all contexts created by this class is NSMergeByPropertyObjectTrumpMergePolicy.  Use <setDefaultMergePolicy:applyToMainThreadContextAndParent:> to change the default.
 
 ## Using the Cache ##
 
 The Core Data integration in version 1.2.0 includes a caching system to allow for local fetching of objects which have previously been fetched from the server.  It is also used to fill faulted objects which have been previously fetched.
 
 The caching system is off by default.  To turn it on, simply set the SM_CACHE_ENABLED flag to YES in your App Delegate file, before you initialize a Core Data store.
 
 ### How It Works ###
 
 The cache itself is a Core Data stack, equipped with its own private managed object context and persistent store coordinator.  It uses SQLite as the persistent store, which is what most standard applications using Core Data use as the local persistent store.  By implementing the cache as a Core Data stack, results from fetches performed on the network can be cached independently of their original request. You can then perform subsequent local fetches that are able to return subsets of the originally cached data.  For example, suppose you are building a To-Do application which has the option to filter tasks by date, subject, etc.  These filters would translate to conditional fetches on the same list of tasks.  Rather than needing to execute a fetch on the network every time your query conditions change, you can instead grab all the tasks with one network call at the beginning and perform the conditional fetches on that data locally, without needing to fetch from the network again. 
 
 After successfully performing a fetch from the StackMob database, an equivalent fetch is performed locally on the cache and those results are replaced with the up-to-date objects from the server.  The results are returned as faulted managed objects and accessing an object's values will cause Core Data to fill the fault using the cache.
 
 **Important:** Regardless of the cache policy, results from fetches on the network are cached and used internally to fill faults.  This prevents a network call from happening everytime an object's values are brought into memory by Core Data.
 
 **Also Important:** There are a few scenarios where filling faults require network calls.  The first is when an object is faulted but does not have an entry in the cache.  This happens when a managed object context obtains a managed object ID reference to a faulted object that is not in the cache.  The second is when trying to access values of related objects which themselves have not been cached.  If either of these situations arise and there is no network connection Core Data may throw the "Core Data could not fulfill a fault" exception.  
 
 ### Caching Policies ###
 
 There are 4 policies
 
 * SMCachePolicyTryNetworkOnly - This is the default policy, the equivalent of not fetching from the cache at all.
 * SMCachePolicyTryCacheOnly - This policy directs all fetches to the cache, never trying the network.
 * SMCachePolicyTryNetworkElseCache - This policy will try to fetch from the network, and if an error occurs because there is no network connection the fetch is performed on the cache.
 * SMCachePolicyTryCacheElseNetwork - This policy will start by performing the fetch on the cache and returning if there are results.  If there are no results, the fetch is performed on the network and those up-to-date results from the server will be cached and returned.  If there is an error because there is no network connection the original empty array is returned.
 
 ### How To Change the Caching Policy ###
 
 You can change the caching policy for your SMCoreDataStore instance at any time by setting its cachePolicy property.
 
 If you know you want one cache policy when you are online and one when you are offline, you can use the SMNetworkReachability method setNetworkStatusChangeBlockWithCachePolicyReturn: and return which cache policy to set based on the network status.  An SMNetworkReachability instance is initialized in the user session, under the networkMonitor property.  For example, to set a network status change block after you initialize your client, do the following:
 
        SMClient *client = [[SMClient alloc] initWithAPIVersion:@"0" publicKey:@"XXXX"];
        SMCoreDataStore *coreDataStore = [client coreDataStoreWithManagedObjectModel:myModel];
        [client.session.networkMonitor setNetworkStatusChangeBlockWithCachePolicyReturn:^SMCachePolicy(SMNetworkStatus status) {
                if (status == Reachable) {
                    return SMCachePolicyTryNetworkElseCache;
                } else {
                    return SMCachePolicyTryCacheOnly;
                }
        }];
 
 ### Manually Purging the Cache ###
 
 At any time you can purge the cache manually of:
 
 * An object by providing it's managed object ID.
 * A group of objects by providing an array of managed object IDs.
 * A group of objects by providing an array of managed objects, which are converted to IDs before processed on a background thread.
 * A group of objects by providing the name of their entity.
 * The entire cache (Reset the thing).
 
 Check out the Manually Purging the Cache section for all the methods.
 
 ### Turning Off the Cache Completely ###
 
 To turn off all caching functionality, including internal caching used for fault filling, set the **SM_CACHE_ENABLED** flag to NO in your App Delegate file, before you initialize a instance of SMCoreDataStore.
 
 
 @note You should not have to initialize an instance of this class directly.  Instead, initialize an instance of <SMClient> and use the method <coreDataStoreWithManagedObjectModel:> to retrieve an instance completely configured and ready to communicate to StackMob.
 */
@interface SMCoreDataStore : SMDataStore

///-------------------------------
/// @name Properties
///-------------------------------

/**
 An instance of `NSPersistentStoreCoordinator` with the `SMIncrementalStore` class as it's persistent store type.
 
 Uses the `NSManagedObjectModel` passed to the `coreDataStoreWithManagedObjectModel:` method in <SMClient>.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property(nonatomic, strong) NSPersistentStoreCoordinator *persistentStoreCoordinator;

/**
 An instance of `NSManagedObjectContext` set to use on the main thread.
 
 This managed object context has a private queue parent context set to ensure proper parent/child asynchronous saving.  The persistent store coordinator is set on the parent context. Merge policy is set to NSMergeByPropertyObjectTrumpMergePolicy.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
@property (nonatomic, strong) NSManagedObjectContext *mainThreadContext;

/**
 An instance of `NSManagedObjectContext` set to use on the main thread.
 
 This property is deprecated. Use <contextForCurrentThread> to obtain a properly initialized managed object context.
 
 @since Available in iOS SDK 1.0.0 and later.
 @note Deprecated in version 1.2.0. Use <contextForCurrentThread>.
 */
@property (nonatomic, strong) NSManagedObjectContext *managedObjectContext __attribute__((deprecated));

/**
 The cache policy to adhere by during fetch requests.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
@property (nonatomic) SMCachePolicy cachePolicy;

/**
 An instance of SMRequestOptions that will be used as the default for all save and fetch calls.
 
 When an instance of this class is initialized, this property is assigned a value of [SMRequestOptions options].  Set this property to customize default options.
 
 **Note:** Not all options provided by the SMRequestOptions class are taken into account during save/fetch requests.  The following options are currently safe to customize:
 
 * isSecure property (HTTPS)
 
 Customizing other options can result in unexpected requests, which can lead to save/fetch failures.
 
 @since Available in iOS SDK 1.3.0 and later.
 */
@property (nonatomic, strong) SMRequestOptions *globalRequestOptions;


///-------------------------------
/// @name Initialize
///-------------------------------

/**
 Initializes an `SMCoreDataStore`.
 
 @param apiVersion The API version of your StackMob application.
 @param session The session containing the credentials to use for requests made to StackMob by Core Data.
 @param managedObjectModel The managed object model to set to the persistent store coordinator.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (id)initWithAPIVersion:(NSString *)apiVersion session:(SMUserSession *)session managedObjectModel:(NSManagedObjectModel *)managedObjectModel;

///-------------------------------
/// @name Obtaining a Managed Object Context
///-------------------------------

/**
 Returns an initialized context for the current thread.
 
 Merge policy is set to NSMergeByPropertyObjectTrumpMergePolicy.
 
 If the current thread is the main thread, returns a context initialized with a NSMainQueueConcurrencyType.  Otherwise, returns a context initialized with a NSPrivateQueueConcurrencyType, with the mainThreadContext as its parent.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (NSManagedObjectContext *)contextForCurrentThread;

/**
 Sets the merge policy that is set by default to any context returned from <contextForCurrentThread>.
 
 If apply is YES, sets the merge policy of mainThreadContext and its private parent context to mergePolicy.
 
 @param mergePolicy The default merge policy to use going forward.
 @param apply Whether or not to set mergePolicy as the merge policy for the existing mainThreadContext and its private parent context.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)setDefaultMergePolicy:(id)mergePolicy applyToMainThreadContextAndParent:(BOOL)apply;

///-------------------------------
/// @name Manually Purging the Cache
///-------------------------------

/**
 Removes the cache entry for the provided NSManagedObjectID.
 
 @param objectID The managed object ID of the object to remove from the cache, if an entry exists.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)purgeCacheOfMangedObjectID:(NSManagedObjectID *)objectID;

/**
 Removes the cache entries for the provided array of NSManagedObject instances.
 
 @param managedObjects An array of managed objects to remove from the cache, if entries exist.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)purgeCacheOfMangedObjects:(NSArray *)managedObjects;

/**
 Removes the cache entries for the provided array of NSManagedObjectID instances.
 
 @param managedObjectIDs An array of managed object IDs whose objects should be removed from the cache, if entries exist.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)purgeCacheOfManagedObjectsIDs:(NSArray *)managedObjectIDs;

/**
 Removes all the cache entries whose entity matches the provided entity name.
 
 @param entityName The name of the entity.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)purgeCacheOfObjectsWithEntityName:(NSString *)entityName;

/**
 Clears the cache of all entries.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)resetCache;



@end
