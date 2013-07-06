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

/**
 SMLocationManager is a built-in `CLLocationManager` singleton for use in retrieving `CLLocationCoordinate2D` points. 
 
 Many apps make use of geo location data; `SMLocationManager` aides in this process by eliminating the boilerplate code needed to build a `CLLocationManager` singleton.
 
 ## Using SMLocationManager ##
 
 First, import the class where needed: `#import "SMLocationManager.h"`.
 
 To start retrieving data, first prompt `SMLocationManager` to start listening for updates from the GPS:
 
    [[[SMLocationManager sharedInstance] locationManager] startUpdatingLocation];
 
 Next, retrieve coordinates as needed, using the `SMGeoPoint` method <i>getGeoPointForCurrentLocationOnSuccess:onFailure:</i>, which will pass back an instance of [SMGeoPoint](http://stackmob.github.com/stackmob-ios-sdk/Classes/SMGeoPoint.html) in the success block or an `NSError` should the method fail.
 
    [SMGeoPoint getGeoPointForCurrentLocationOnSuccess:^(SMGeoPoint *geoPoint) {
 
        // Do something with geoPoint.latitude and geoPoint.longitude
 
    } onFailure:^(NSError *) {
 
        // Handle error
 
    }];
 
 When you are finished, tell the location manager to stop listening for GPS updates.
 
    [[[SMLocationManager sharedInstance] locationManager] stopUpdatingLocation];
 
 ## Manually Pulling the Latitude and Longitude ##
 
 If you wish to pull the latitude and longitude data manually from the location manager, do so with the following:
 
    NSNumber *latitude = [[NSNumber alloc] initWithDouble:[[[[SMLocationManager sharedInstance] locationManager] location] coordinate].latitude];
    NSNumber *longitude = [[NSNumber alloc] initWithDouble:[[[[SMLocationManager sharedInstance] locationManager] location] coordinate].longitude];
 
 Often times it may take a few run loops for the location manager to start receiving actual data after <i>startUpdatingLocation</i> is called.  You may need to implement the necessary logic to ensure you are not pulling nil data.

 ## Subclassing SMLocationManager ##
 
 Out of the box, `SMLocationManager` gives you the ability to start/stop pulling GPS location data, as well as retrieve the current latitude and longitude of the device.  If you would like more control and customization, it's recommended you subclass `SMLocationManager`. In the init method of your subclass, you can configure the properties of the `CLLocationManager` as needed.
 
 ## References ##
 
 [Apple's CLLocationManager class reference](https://developer.apple.com/library/mac/#documentation/CoreLocation/Reference/CLLocationManager_Class/CLLocationManager/CLLocationManager.html)
 */
@interface SMLocationManager : NSObject <CLLocationManagerDelegate>

///-------------------------------
/// @name Properties
///-------------------------------

/**
 locationManager is the `CLLocationManager` this singleton uses to receive updates.
 
 @since Available in iOS SDK 1.3.0 and later.
 */
@property (nonatomic, strong) CLLocationManager* locationManager;

/**
 locationManagerError is a property to store errors that the `CLLocationManager` returns.
 
 @since Available in iOS SDK 1.3.0 and later.
*/
@property (nonatomic, strong) NSError *locationManagerError;

///-------------------------------
/// @name Shared Instance
///-------------------------------

/**
 Returns the instance on `SMLocationManager`.
 
 @return The shared instance of `SMLocationManager`.
 
 @since Available in iOS SDK 1.3.0 and later.
 */
+ (SMLocationManager *)sharedInstance;

@end
