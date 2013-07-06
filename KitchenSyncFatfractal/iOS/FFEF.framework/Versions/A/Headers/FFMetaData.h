//
//  FFMetaData.h
//  FatFractal
//
//  Copyright (c) 2012 FatFractal, Inc. All rights reserved.
//
/*! \brief Framework-generated data about your objects you can access. */
/*! 
 * FFMetaData contains essential information about your object that is stored on your app's backend
 * This can be accessed via FatFractal::metaDataForObj:
 */
#import <Foundation/Foundation.h>

#define FF_METADATA_LOCAL_PREFIX @"#local/"

@interface FFMetaData: NSObject <NSCoding>

/*! An NSString that contains the url (relative to the application base url) of this object*/
@property (strong, nonatomic) NSString          *ffUrl;
/*! An NSString that contains the guid of this object. Guids must be unique within a given collection. */
@property (strong, nonatomic) NSString          *guid;
/*! An NSString that contains the resource location (collection) of this object. */
@property (strong, nonatomic) NSString          *ffRL;
/*! An NSString that contains the name of the class (clazz) of this object. */
@property (strong, nonatomic) NSString          *clazz;
/*! The object's version. Objects are created with version 1, version increments on every update */
@property (strong, nonatomic) NSNumber          *objVersion;
/*! An NSString that contains guid of the FFUser that created the object. */
@property (strong, nonatomic) NSString          *createdBy;
/*! An NSDate with the date/time stamp when the object was created as set by your app's backend */
@property (strong, nonatomic) NSDate            *createdAt;
/*! An NSString that contains the guid of the FFUser that last updated the object. */
@property (strong, nonatomic) NSString          *updatedBy;
/*! An NSDate with the date/time stamp when the object was last updated as set by your app's backend */
@property (strong, nonatomic) NSDate            *updatedAt;
/*! An array containing metadata about any blobs or referred objects that this objct has */
@property (strong, nonatomic) NSArray           *ffRefs;
/* When objects are retrieved from the backend, this flag lets the client know whether this user has permission to modify it. */
@property (nonatomic)         BOOL              ffUserCanEdit;

/*! The prettyPrint method is used in many of our sample apps to generate log messages that are easy to read. */
- (NSString*) prettyPrint;
/*! The isEqisEqualToMetaDatamethod determines if all member data are equal. */
- (BOOL) isEqualToMetaData:(FFMetaData *)that;

/*! Makes this a local metadata instance. Requires 'guid' to be set and 'ffUrl' not to be set. */
- (NSError *)makeLocal;
/*! Whether this metadata is local or not. */
- (BOOL)isLocal;
/*! Whether the provided ffUrl is consistent with local metadata ffUrl scheme. */
+ (BOOL)isLocalFfUrl:(NSString *)ffUrl;

@end
