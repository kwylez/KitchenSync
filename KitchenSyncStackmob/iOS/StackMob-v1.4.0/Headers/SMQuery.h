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
#import <CoreLocation/CoreLocation.h>
#import "SMGeoPoint.h"

/**
 `SMQuery` exposes an interface for defining a query against StackMob's Datastore API.
 
 `SMQuery` uses StackMob's terminology:
 - Queries are performed against a specific _schema_ (usually also the name of a model class or of an entity in a managed object model)
 - Queries may select or order based on the _values_ of the _fields_ of an object belonging to their _schema_.
 */
@interface SMQuery : NSObject

///-------------------------------
/// @name Properties
///-------------------------------

/** 
 requestParameters holds the HTTP request parameters i.e. for a request that looks like `GET /something?key=value`, it holds a dictionary entry mapping `key` to `value`.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property (nonatomic, strong) NSDictionary *requestParameters;

/**
 requestHeaders holds the HTTP headers, which affect things like sort order and pagination/limiting.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property (nonatomic, strong) NSDictionary *requestHeaders;

/**
 schemaName is the name of the StackMob schema against which this query will be executed.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property (readonly) NSString *schemaName;

/**
 entity is the Entity Description of the Core Data Entity provided when initialized using <initWithEntity:>.

 @return `nil` if <initWithSchema:> was used for initialization, otherwise the Entity Description.
 @since Available in iOS SDK 1.0.0 and later.
 */
@property (readonly) NSEntityDescription *entity;

#pragma mark - Creating queries
///-------------------------------
/// @name Initialize
///-------------------------------

/**
 Initializes an `SMQuery` object from the provided entity.

 @param entity A Core Data `NSEntityDescription` for which we assume a corresponding schema named [[entity name] lowercaseString].
 @since Available in iOS SDK 1.0.0 and later.
 */
- (id)initWithEntity:(NSEntityDescription *)entity;

/**
 Initializes an `SMQuery` object from the provided schema.
 
 @param schema The StackMob schema to run the query against.
 @since Available in iOS SDK 1.0.0 and later.
 */
- (id)initWithSchema:(NSString *)schema;



#pragma mark - Where clauses
///-------------------------------
/// @name Where Clauses
///-------------------------------

/**
 Add the query criteria: field == value.
 
 @param field The field in the StackMob schema that is to be compared.
 @param value A constant value to be compared.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)where:(NSString *)field isEqualTo:(id)value;

/**
 Add the query criteria: field != value.
 
 @param field The field in the StackMob schema that is to be compared.
 @param value A constant value to be compared.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)where:(NSString *)field isNotEqualTo:(id)value;

/**
 Add the query criteria: field < value.
 
 @param field The field in the StackMob schema that is to be compared.
 @param value A constant value to be compared.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)where:(NSString *)field isLessThan:(id)value;

/**
 Add the query criteria: field <= value.
 
 @param field The field in the StackMob schema that is to be compared.
 @param value A constant value to be compared.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)where:(NSString *)field isLessThanOrEqualTo:(id)value;

/**
 Add the query criteria: field > value.
 
 @param field The field in the StackMob schema that is to be compared.
 @param value A constant value to be compared.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)where:(NSString *)field isGreaterThan:(id)value;

/**
 Add the query criteria: field >= value.
 
 @param field The field in the StackMob schema that is to be compared.
 @param value A constant value to be compared.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)where:(NSString *)field isGreaterThanOrEqualTo:(id)value;

/**
 Add the query criteria: field is in the array valuesArray.
 
 @param field The field in the StackMob schema that is to be compared.
 @param valuesArray An array containing values with which to compare `field`.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)where:(NSString *)field isIn:(NSArray *)valuesArray;

/**
 Add the query criteria: `field`'s location is within `miles` of `point`.
 
 @note StackMob will generate a field `distance` and insert it into the response. This field is the distance between the query `field`'s location and `point`.
 
 @param field The geo field in the StackMob schema that is to be compared.
 @param miles Distance in miles.
 @param point The point around which to search.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)where:(NSString *)field isWithin:(double)miles milesOf:(CLLocationCoordinate2D)point;

/**
 Add the query criteria: `field`'s location is within `miles` of `geoPoint`.
 
 @note StackMob will generate a field `distance` and insert it into the response. This field is the distance between the query `field`'s location and `geoPoint`.
 
 @param field The geo field in the StackMob schema that is to be compared.
 @param miles Distance in miles.
 @param geoPoint The SMGeoPoint around which to search.
 
 @since Available in iOS SDK 1.3.0 and later.
 */

- (void)where:(NSString *)field isWithin:(double)miles milesOfGeoPoint:(SMGeoPoint *)geoPoint;

/**
 Add the query criteria: `field`'s location is within `kilometers` of `point`.
 
 @note StackMob will generate a field `distance` and insert it into the response. This field is the distance between the query `field`'s location and `point`.
 
 @param field The geo field in the StackMob schema that is to be compared.
 @param kilometers Distance in kilometers.
 @param point The point around which to search.
 
 @since Available in iOS SDK 1.1.2 and later.
 */
- (void)where:(NSString *)field isWithin:(CLLocationDistance)kilometers kilometersOf:(CLLocationCoordinate2D)point;

/**
 Add the query criteria: `field`'s location is within `kilometers` of `geoPoint`.
 
 @note StackMob will generate a field `distance` and insert it into the response. This field is the distance between the query `field`'s location and `geoPoint`.
 
 @param field The geo field in the StackMob schema that is to be compared.
 @param kilometers Distance in kilometers.
 @param geoPoint The SMGeoPoint around which to search.
 
 @since Available in iOS SDK 1.3.0 and later.
 */
- (void)where:(NSString *)field isWithin:(CLLocationDistance)kilometers kilometersOfGeoPoint:(SMGeoPoint *)geoPoint;

/**
 Add the query criteria: `field`'s location falls within the bounding box with corners `sw` and `ne`.
 
 @param field The geo field in the StackMob schema that is to be compared.
 @param sw Location of the bounding box's southwest corner.
 @param ne Location of the bounding box's northeast corner.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)where:(NSString *)field isWithinBoundsWithSWCorner:(CLLocationCoordinate2D)sw andNECorner:(CLLocationCoordinate2D)ne;

/**
 Add the query criteria: `field`'s location falls within the bounding box with geoPoints `sw` and `ne`.
 
 @param field The geo field in the StackMob schema that is to be compared.
 @param sw Location of the bounding box's southwest SMGeoPoint.
 @param ne Location of the bounding box's northeast SMGeoPoint.
 
 @since Available in iOS SDK 1.3.0 and later.
 */
- (void)where:(NSString *)field isWithinBoundsWithSWGeoPoint:(SMGeoPoint *)sw andNEGeoPoint:(SMGeoPoint *)ne;

/**
 StackMob will insert a field `distance` and insert it into the response. This field is the distance between the query `field`'s location and `location`.
 
 @note You probably want to apply a limit when including this clause or you may end up with more results than intended.
 
 @param field The geo field in the StackMob schema that is to be compared.
 @param location The reference location.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)where:(NSString *)field near:(CLLocationCoordinate2D)location;

/**
 StackMob will insert a field `distance` and insert it into the response. This field is the distance between the query `field`'s location and `geoPoint`.
 
 @note You probably want to apply a limit when including this clause or you may end up with more results than intended.
 
 @param field The geo field in the StackMob schema that is to be compared.
 @param geoPoint The reference SMGeoPoint.
 
 @since Available in iOS SDK 1.3.0 and later.
 */
- (void)where:(NSString *)field nearGeoPoint:(SMGeoPoint *)geoPoint;

#pragma mark - Pagination / Limiting
///-------------------------------
/// @name Pagination / Limiting
///-------------------------------

/**
 Return a subset of the result set.
 
 @param start Offset of the first result to return.
 @param end Offset of the last result to return.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)fromIndex:(NSUInteger)start toIndex:(NSUInteger)end;

/**
 Limit the result set.
 
 @param count The number of objects to return.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)limit:(NSUInteger)count;

#pragma mark - Order-by clause
///-------------------------------
/// @name Order By
///-------------------------------

/**
 Order the result set.
 Note: You can apply multiple ordering criteria by invoking this multiple times.
 
 @param field The field to order by.
 @param ascending `YES` for ascending, `NO` for descending.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)orderByField:(NSString *)field ascending:(BOOL)ascending;

#pragma mark - And/Or
///-------------------------------
/// @name And/Or
///-------------------------------

/**
 Create an AND query.
 
 If you are not using OR, you do not need to use this method.  You can achieve a query with condition A & B & C by simply adding multiple where clauses to one `SMQuery` instance.
 
 This method is primarily used when chaining together a combination of ORs and ANDs.  See <or:> for usage examples. 
 
 @param query The query to AND to the receiving query.
 @return An instance of `SMQuery` with parameters updated as a result of the AND.
 
 @since Available in iOS SDK 1.4.0 and later.
 */
- (SMQuery *)and:(SMQuery *)query;

/**
 Create an OR query.
 
 Multiple `SMQuery` instances can be OR'ed together to create a query with conditions "A" OR "B" OR "C". Those conditions themselves can consist of ANDs, i.e. ("A1" AND "A2") OR "B" OR "C".  Finally, the entire OR query can be chained with another `SMQuery` instance containing multiple conditions to produce something that make look like "A" AND "B" AND ( ("C1" AND "C2") OR "D" OR "E" ).
 
 **Important:** Only one `SMQuery` instance containing ORs can be AND'ed with an `SMQuery` instance.  For example:
 
 Right: "A" AND "B" AND ( ("C1" AND "C2") OR "D" OR "E" )
 
 Wrong: "A" AND "B" AND ( ("C1" AND "C2") OR "D" OR "E" ) AND ( "F" OR "G" )  // The second OR containing F/G is not allowed.
 
 A Practical Example:
 
     // Person where:
     // armor_class < 17 AND ((first_name == "Jonah" AND last_name == "Williams) OR first_name == "Jon" OR company == "Carbon Five")
     
     SMQuery *rootQuery = [[SMQuery alloc] initWithSchema:@"People"];
     [rootQuery where:@"armor_class" isLessThan:[NSNumber numberWithInt:17]];
     
     SMQuery *subQuery = [[SMQuery alloc] initWithSchema:@"People"];
     [subQuery where:@"first_name" isEqualTo:@"Jonah"];
     [subQuery where:@"last_name" isEqualTo:@"Williams"];
     
     SMQuery *subQuery2 =[[SMQuery alloc] initWithSchema:@"People"];
     [subQuery2 where:@"first_name" isEqualTo:@"Jon"];
     
     SMQuery *subQuery3 =[[SMQuery alloc] initWithSchema:@"People"];
     [subQuery3 where:@"company" isEqualTo:@"Carbon Five"];
     
     [rootQuery and:[[subQuery or:subQuery2] or:subQuery3]];
     
     // Perform rootQuery on database
 
 @param query The query to OR to the receiving query.
 @return An instance of `SMQuery` with parameters updated as a result of the OR.
 @since Available in iOS SDK 1.4.0 and later.
 */
- (SMQuery *)or:(SMQuery *)query;



@end
