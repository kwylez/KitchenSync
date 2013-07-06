//
//  FFGeoLocation.h
//  FF-IOS-Framework
//
//  Copyright (c) 2012 FatFractal, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import "FatFractal.h"

/** \brief The model class for working with location information and the FatFractal Platform. */
/*! The FFGeoLocation object model includes all of the parameters that can 
 * be specified regarding location information with the FatFractal Platform.
 */
@interface FFGeoLocation : NSObject <NSCoding>

/*! A double value representing the latitude of the location. */
@property (nonatomic) double latitude;
/*! A double value representing the longitude of the location. */
@property (nonatomic) double longitude;
/*! A double value representing the altitude of the location. */
@property (nonatomic) double altitude;
/*! A double value representing the accuracy of the location. */
@property (nonatomic) double accuracy;
/*! A double value representing the altitudeAccuracy of the location. */
@property (nonatomic) double altitudeAccuracy;
/*! A double value representing the heading of the location. */
@property (nonatomic) double heading;
/*! A double value representing the speed of the location. */
@property (nonatomic) double speed;

/*! Initializer that accepts a standard CLLocation parameter.
 * @param CLLocation - the user value of the location object
 */
- (id) initWithCLLocation:(CLLocation *) clLocation;

/*! Initializer that accepts latitude and longitude as parameters.
 * @param double - the latitude value of the location object
 * @param double - the longitude value of the location object
 */
- (id) initWithLatitude:(double)theLatitude
              longitude:(double)theLongitude;

/*! Initializer that accepts latitude, longitude and altitude as parameters.
 * @param double - the latitude value of the location object
 * @param double - the longitude value of the location object
 * @param double - the altitude value of the location object
 */
- (id) initWithLatitude:(double)theLatitude
              longitude:(double)theLongitude
               altitude:(double)theAltitude;

/*! Initializer that accepts latitude, longitude, altitude, accuracy, altitudeAccuracy, heading and speed as parameters.
 * @param double - the latitude value of the location object
 * @param double - the longitude value of the location object
 * @param double - the altitude value of the location object
 * @param double - the accuracy value of the location object
 * @param double - the altitudeAccuracy value of the location object
 * @param double - the heading value of the location object
 * @param double - the speed value of the location object
 */
- (id) initWithLatitude:(double)theLatitude
              longitude:(double)theLongitude
               altitude:(double)theAltitude
               accuracy:(double)theAccuracy
       altitudeAccuracy:(double)theAltitudeAccuracy
                heading:(double)theHeading
                  speed:(double)theSpeed;

@end
