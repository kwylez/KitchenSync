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

@class SMRequestOptions;
@class SMGeoPoint;

/**
 A success block that returns nothing.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
typedef void (^SMSuccessBlock)();

/**
 The block parameters expected for a success response which returns an `NSDictionary`.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
typedef void (^SMResultSuccessBlock)(NSDictionary *result);

/**
 The block parameters expected for a success response which returns an `NSArray`.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
typedef void (^SMResultsSuccessBlock)(NSArray *results);

/**
 The block parameters expected for any failure response.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
typedef void (^SMFailureBlock)(NSError *error);

/**
 The block parameters expected for a success response which returns an `SMGeoPoint`.
 
 @since Available in iOS SDK 1.3.0 and later.
 */
typedef void (^SMGeoPointSuccessBlock)(SMGeoPoint *geoPoint);

/**
 The block parameters expected for a success response that needs the raw request, response, and JSON result.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
typedef void (^SMFullResponseSuccessBlock)(NSURLRequest *request, NSHTTPURLResponse *response, id JSON);

/**
 The block parameters expected for a failure response that needs the raw request, response, and JSON result.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
typedef void (^SMFullResponseFailureBlock)(NSURLRequest *request, NSHTTPURLResponse *response, NSError *error, id JSON);

/**
 The block parameters expected for a success response from a call to the Datastore which returns the full object and schema. 
 
 @param theObject An updated dictionary representation of the requested object.
 @param schema The schema to which the object belongs.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
typedef void (^SMDataStoreSuccessBlock)(NSDictionary* theObject, NSString *schema);

/**
 The block parameters expected for a success response from a call to the Datastore which returns the object ID and schema.
 
 @param theObjectId The object id used in this operation.
 @param schema The schema to which the object belongs.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
typedef void (^SMDataStoreObjectIdSuccessBlock)(NSString* theObjectId, NSString *schema);

/** 
 The block parameters expected for a failure response from a call to the Datastore which returns the error, full object and schema.
 
 @param theError An error object describing the failure.
 @param theObject The dictionary representation of the object sent as part of the failed operation.
 @param schema The schema to which the object belongs.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
typedef void (^SMDataStoreFailureBlock)(NSError *theError, NSDictionary* theObject, NSString *schema);

/** 
 The block parameters expected for a failure response from a call to the Datastore which returns the error, object ID and schema.
 
 @param theError An error object describing the failure.
 @param theObjectId The object id sent as part of the failed operation.
 @param schema The schema to which the object belongs.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
typedef void (^SMDataStoreObjectIdFailureBlock)(NSError *theError, NSString* theObjectId, NSString *schema);

/** 
 The block parameters expected for a success response from query count call.
 
 @param count The number of objects returned by the query.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
typedef void (^SMCountSuccessBlock)(NSNumber *count);

/**
 When executing custom code requests, you can optionally define your own retry blocks in the event of a 503 `SMServiceUnavailable` response.  To do this pass a `SMFailureRetryBlock` instance to <SMRequestOptions> method `addSMErrorServiceUnavailableRetryBlock:`.
 
 @param request The original request in `NSURLRequest` form.
 @param response The response from the server.
 @param error The error, if any.
 @param JSON the JSON result from the response.
 @param options The SMRequestOption instance passed to the request.
 @param successBlock The block to invoke on success.
 @param failureBlock The block to invoke on failure.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
typedef void (^SMFailureRetryBlock)(NSURLRequest *request, NSHTTPURLResponse *response, NSError *error, id JSON, SMRequestOptions *options, SMFullResponseSuccessBlock successBlock, SMFullResponseFailureBlock failureBlock);

/**
 Used internally for requests that fail during a core data save.
 
 @param theRequest The original request in `NSURLRequest` form.
 @param theError The error, if any.
 @param theObject The original object being saved.
 @param theOptions The SMRequestOption instance passed to the request.
 @param originalSuccessBlock The block passed to original request.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
typedef void (^SMCoreDataSaveFailureBlock)(NSURLRequest *theRequest, NSError *theError, NSDictionary *theObject, SMRequestOptions *theOptions, SMResultSuccessBlock originalSuccessBlock);

