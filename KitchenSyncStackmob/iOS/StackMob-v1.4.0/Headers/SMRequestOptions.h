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

#import "SMResponseBlocks.h"

/**
 `SMRequestOptions` is a class designed to supply various choices to requests, including:
 
 * Whether or not it is secure (https).
 * Extra headers to add to the request
 * Select and expand choices to control the data being returned to you
 * The ability to disable automatic login refresh
 
 */
@interface SMRequestOptions : NSObject

///-------------------------------
/// @name Properties
///-------------------------------

/**
 Optional request headers where the key is the header field name.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property(nonatomic, strong) NSDictionary *headers;

/**
 Whether the request should be sent over HTTPS. Default is `NO`.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property(nonatomic, readwrite) BOOL isSecure;

/**
 In the case that a 401 `SMErrorUnauthorized` response is returned, whether to try and refresh the session. Default is `YES`.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property(nonatomic, readwrite) BOOL tryRefreshToken;

/**
 In the case that a 503 `SMErrorServiceUnavailable` response is returned, the number of times to retry.  The default is 3 times.
 
 The default retry action is to send the original request, resigned with up to date arguments. If a <retryBlock> has been added it is used in place of the default.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property(nonatomic, readwrite) NSInteger numberOfRetries;

/**
 An optional block to call if the response returns a 503 `SMErrorServiceUnavailable`. Use <addSMErrorServiceUnavailableRetryBlock:> to set.
 
 `SMFailureRetryBlock` is defined as:
 
    typedef void (^SMFailureRetryBlock)(NSURLRequest *request, NSHTTPURLResponse *response, NSError *error, id JSON, SMRequestOptions *options, SMFullResponseSuccessBlock successBlock, SMFullResponseFailureBlock failureBlock);
 
 @since Available in iOS SDK 1.0.0 and later.
 
 */
@property (nonatomic, strong) SMFailureRetryBlock retryBlock;

///-------------------------------
/// @name Initialize
///-------------------------------

/**
 Empty options with no special settings
 
 @return An `SMRequestOptions` object with the default options.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
+ (SMRequestOptions *)options;

/**
 Options that will add additional headers to a request.
 
 @param headers A dictionary of headers to be attached to a request.
 
 @return An `SMRequestOptions` object with headers set to the supplied dictionary.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
+ (SMRequestOptions *)optionsWithHeaders:(NSDictionary *)headers;

/**
 Options that will enable https for a request
 
 @return An `SMRequestOptions` object with isSecure set to `YES`.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
+ (SMRequestOptions *)optionsWithHTTPS;

/**
 Options that will add a expand depth entry to the headers.  This is for returning full related objects rather than just reference IDs.
 
 @param depth The depth by which to recursively expand related objects.  Maximum is 3.
 
 @return An `SMRequestOptions` object with an expand depth entry in the headers dictionary.
 
 @since Available in iOS SDK 1.1.2 and later.
 */
+ (SMRequestOptions *)optionsWithExpandDepth:(NSUInteger)depth;

/**
 Options that will add an entry to the headers to restrict the response to the specified fields.
 
 @param fields An array of strings specifying the fields to return.
 
 @return An `SMRequestOptions` object with a field restriction entry in the headers dictionary.
 
 @since Available in iOS SDK 1.1.2 and later.
 */
+ (SMRequestOptions *)optionsWithReturnedFieldsRestrictedTo:(NSArray *)fields;

#pragma mark - Expanding relationships
///-------------------------------
/// @name Expanding Relationships
///-------------------------------

/**
 Expand relationships by `depth` levels.
 
 For example, if `depth` is 1, include any direct child objects nested inside the query results. The option is currently only honored for login calls, Datastore reads, and queries.
 
 @param depth The depth to expand to, maximum 3.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)setExpandDepth:(NSUInteger)depth;


#pragma mark - Limiting returned properties
///-------------------------------
/// @name Limiting Returned Properties
///-------------------------------

/**
 Return a subset of the schema's fields.
 
 See [the docs](https://stackmob.com/devcenter/docs/Datastore-API#a-selecting_fields_to_return) for details on the format. The option is currently only honored for login calls, Datastore reads, and queries
 
 @param fields An array containing the names of the fields to return.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)restrictReturnedFieldsTo:(NSArray *)fields;

/**
 Adds a retry block to be used in the event of a 503 `SMErrorServiceUnavailable` response.
 
 @param retryBlock An instance of <SMFailureRetryBlock>.
 
 When designing your retry block, consider the following:
 
 * The parameter `options` passed to the block will contain the original request `SMRequestOptions` instance, with `numberOfRetries` decremented by 1. This instance should be passed to any subsequent requests so that the retry block is called a finite amount of times.
 * To resend the request, call `retryCustomCodeRequest:options:onSuccess:onFailure:` on your `SMClient` `dataStore` instance.  The default values passed to this method should be the parameters passed to the block, unless you are intentionally redefining one of them.
 
 @param retryBlock The block to be used in place of the default retry action, which is resigning and resending the original request.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)addSMErrorServiceUnavailableRetryBlock:(SMFailureRetryBlock)retryBlock;

@end
