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

#import <CoreLocation/CoreLocation.h>
#import "SMResponseBlocks.h"
#import "SMRequestOptions.h"


/**
 Category on `NSDictionary` which provides helper methods to get the latitude and longitude from a GeoPoint. 
 
 @since Available in iOS SDK 1.3.0 and later.
 */
@interface NSDictionary (GeoPoint)

/**
 Return the latitude.
 
 @return An `NSNumber` representing the latitude of the GeoPoint.
 
 @since Available in iOS SDK 1.3.0 and later.
 */
- (NSNumber *)latitude;

/**
 Return the longitude.
 
 @return An `NSNumber` representing the longitude of the GeoPoint.
 
 @since Available in iOS SDK 1.3.0 and later.
 */
- (NSNumber *)longitude;

@end

/**
 `SMGeoPoint` is a subclass of `NSDictionary`, with helper methods to build geo point dictionaries that are specific to the StackMob API.
 
 The [NSDictionary+GeoPoint](http://stackmob.github.com/stackmob-ios-sdk/Categories/NSDictionary+GeoPoint.html) category provides the methods to retrieve the latitude and longitude from the GeoPoint.
 
 **Important:** GeoPoint data types are not inferred by StackMob. Make sure your configure the proper fields beforehand on the <a href="https://dashboard.stackmob.com/" target="_blank">StackMob dashboard</a>.
 
 ## Using SMGeoPoint with Core Data ##
 
 GeoPoints are stored in Core Data using the `Transformable` attribute type. To save an `SMGeoPoint` in Core Data, it must be archived into `NSData`:
 
    NSNumber *lat = [NSNumber numberWithDouble:37.77215879638275];
    NSNumber *lon = [NSNumber numberWithDouble:-122.4064476357965];
 
    SMGeoPoint *location = [SMGeoPoint geoPointWithLatitude:lat longitude:lon];
 
    NSData *data = [NSKeyedArchiver archivedDataWithRootObject:location];
 
 To query using SMGeoPoint, use the special predicate methods provided by the [SMPredicate](http://stackmob.github.com/stackmob-ios-sdk/Classes/SMPredicate.html) class:
 
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] initWithEntityName:@"EntityName"];
 
    // Fisherman's Wharf
    CLLocationCoordinate2D coordinate;
    coordinate.latitude = 37.810317;
    coordinate.longitude = -122.418167;
 
    SMGeoPoint *geoPoint = [SMGeoPoint geoPointWithCoordinate:coordinate];
    SMPredicate *predicate = [SMPredicate predicateWhere:@"geopoint" isWithin:3.5 milesOfGeoPoint:geoPoint];
    [fetchRequest setPredicate:predicate];
 
 Once you've made a fetch request, make sure to unarchive the NSData:
    
    // Execute fetch request
    [self.managedObjectContext executeFetchRequest:fetchRequest onSuccess:^(NSArray *results) {
        
        NSManagedObject *object = [results objectAtIndex:0];
 
        NSData *data = [object objectForKey:@"location"];
        
        SMGeoPoint *geoPoint = [NSKeyedUnarchiver unarchiveObjectWithData:data]
 
    } onFailure:^(NSError *error) {
 
            NSLog(@"Error: %@", error);
    }];
 
 **Important:** Fetching from the cache using `SMPredicate` is not supported, and will return an empty array of results. Similarly, when a fetch is performed from the network (StackMob), any results are not cached.
 
 ## Using SMGeoPoint with the Datastore API ##
 
 You can make an `SMGeoPoint` with a latitude and a longitude:
 
    NSNumber *lat = [NSNumber numberWithDouble:37.77215879638275];
    NSNumber *lon = [NSNumber numberWithDouble:-122.4064476357965];

    SMGeoPoint *location = [SMGeoPoint geoPointWithLatitude:lat longitude:lon];
 
 Alternatively, you can use a `CLLocationCoordinate2D` coordinate:
 
    CLLocationCoordinate2D renoCoordinate = CLLocationCoordinate2DMake(39.537940, -119.783936);

    SMGeoPoint *reno = [SMGeoPoint geoPointWithCoordinate:renoCoordinate];

 To save an SMGeoPoint, store it in a dictionary of arguments to be uploaded to StackMob:
 
    CLLocationCoordinate2D renoCoordinate = CLLocationCoordinate2DMake(39.537940, -119.783936);
      
    SMGeoPoint *location = [SMGeoPoint geoPointWithCoordinate:renoCoordinate];
     
    NSDictionary *arguments = [NSDictionary dictionaryWithObjectsAndKeys:@"My Location", @"name", location, @"location", nil];
     
    [[[SMClient defaultClient] dataStore] createObject:arguments inSchema:@"todo" onSuccess:^(NSDictionary *theObject, NSString *schema) {
        NSLog(@"Created object %@ in schema %@", theObject, schema);
     
    } onFailure:^(NSError *theError, NSDictionary *theObject, NSString *schema) {
        NSLog(@"Error creating object: %@", theError);
    }];
 
 */
@interface SMGeoPoint : NSDictionary

///-------------------------------
/// @name Creating an SMGeoPoint
///-------------------------------

/**
 Returns an instance of `SMGeoPoint` with the latitude and longitude provided.
 
 @param latitude The latitude, represented as an `NSNumber`.
 @param longitude The longitude, represented as an `NSNumber`.
 
 @return An instance of `SMGeoPoint`, for use as an attribute or as part of a query.
 
 @since Available in iOS SDK 1.3.0 and later.
 */
+ (SMGeoPoint *)geoPointWithLatitude:(NSNumber *)latitude longitude:(NSNumber *)longitude;

/**
 Returns an instance of `SMGeoPoint` with the `CLLocationCoordinate2D` provided
 
 @param coordinate The `CLLocationCoordinate2D` coordinate
 
 @return An instance of `SMGeoPoint`, for use as an attribute or as part of a query.
 
 @since Available in iOS SDK 1.3.0 and later.
 */
+ (SMGeoPoint *)geoPointWithCoordinate:(CLLocationCoordinate2D)coordinate;

///-------------------------------
/// @name Retrieving the Current Location
///-------------------------------

/**
 Returns an instance of `SMGeoPoint` with coordinates provided by `SMLocationManager`
 
 @param successBlock <i>typedef void (^SMGeoPointSuccessBlock)(SMGeoPoint *geoPoint)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 
 @since Available in iOS SDK 1.3.0 and later.
  */
+ (void)getGeoPointForCurrentLocationOnSuccess:(SMGeoPointSuccessBlock)successBlock onFailure:(SMFailureBlock) failureBlock;

/**
 Returns an instance of `SMGeoPoint` with coordinates provided by `SMLocationManager`
 
 @param options An options object that contains configurations for this request.
 @param successBlock <i>typedef void (^SMGeoPointSuccessBlock)(SMGeoPoint *geoPoint)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 
 @since Available in iOS SDK 1.3.0 and later.
  */
+ (void)getGeoPointForCurrentLocationWithOptions:(SMRequestOptions *)options
                                       onSuccess:(SMGeoPointSuccessBlock)successBlock
                                       onFailure:(SMFailureBlock)failureBlock;

/**
 Returns an instance of `SMGeoPoint` with coordinates provided by `SMLocationManager`
 
 @param options An options object that contains configurations for this request.
 @param successCallbackQueue The dispatch queue used to execute the success block. If nil is passed, the main queue is used.
 @param failureCallbackQueue The dispatch queue used to execute the failure block. If nil is passed, the main queue is used.
 @param successBlock <i>typedef void (^SMGeoPointSuccessBlock)(SMGeoPoint *geoPoint)</i>. A block object to execute upon success.
 @param failureBlock <i>typedef void (^SMFailureBlock)(NSError *error)</i>. A block object to execute upon failure.
 
 @since Available in iOS SDK 1.3.0 and later.
  */
+ (void)getGeoPointForCurrentLocationWithOptions:(SMRequestOptions *)options
                            successCallbackQueue:(dispatch_queue_t)successCallbackQueue
                            failureCallbackQueue:(dispatch_queue_t)failureCallbackQueue
                                       onSuccess:(SMGeoPointSuccessBlock)successBlock
                                       onFailure:(SMFailureBlock)failureBlock;



@end

