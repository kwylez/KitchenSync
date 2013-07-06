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

/**
 The `SMCustomCodeRequest` class is for initializing instances of requests for custom code methods.
 
 Use the init method that corresponds to the HTTP verb you want to use for the request.  To execute the request, call `performCustomCodeRequest:onSuccess:onFailure:` on your <SMClient> `dataStore` instance, passing the instance of `SMCustomCodeRequest`.
 
 You can optionally add request headers and/or a retry block in the case of a 503 `SMErrorServiceUnavailable` response by creating an instance of <SMRequestOptions> and passing it to `performCustomCodeRequest:options:onSuccess:onFailure:`.
 */
@interface SMCustomCodeRequest : NSObject

///-------------------------------
/// @name Properties
///-------------------------------

/**
 An `NSMutableArray` containing parameters for the query string in the form key=value.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property (nonatomic, strong) NSMutableArray *queryStringParameters;

/**
 The name of the custom code method.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property (nonatomic, strong) NSString *method;

/**
 An optional body to send to the custom code method.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property (nonatomic, strong) NSString *requestBody;

/**
 The HTTP Verb to use in the request to the custom code method.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property (nonatomic, strong) NSString *httpVerb;

///-------------------------------
/// @name Initialize
///-------------------------------

/**
 Initialize a POST request to a custom code method.
 
 @param method The name of the method.
 @param body An optional body to be sent with the request.  Pass `nil` for no body.
 
 @return A new POST request for the specified custom code method.
 @since Available in iOS SDK 1.0.0 and later.
 */
- (id)initPostRequestWithMethod:(NSString *)method body:(NSString *)body;

/**
 Initialize a PUT request to a custom code method.
 
 @param method The name of the method.
 @param body An optional body to be sent with the request.  Pass `nil` for no body.
 
 @return A new PUT request for the specified custom code method.
 @since Available in iOS SDK 1.0.0 and later.
 */
- (id)initPutRequestWithMethod:(NSString *)method body:(NSString *)body;

/**
 Initialize a GET request to a custom code method.
 
 @param method The name of the method.
 
 @return A new GET request for the specified custom code method.
 @since Available in iOS SDK 1.0.0 and later.
 */
- (id)initGetRequestWithMethod:(NSString *)method;

/**
 Initialize a DELETE request to a custom code method.
 
 @param method The name of the method.
 
 @return A new DELETE request for the specified custom code method.
 @since Available in iOS SDK 1.0.0 and later.
 */
- (id)initDeleteRequestWithMethod:(NSString *)method;

///-------------------------------
/// @name Adding Query String Parameters
///-------------------------------

/**
 Adds a new parameter to the request query string in the form key=value.
 
 If your method is hello_world and you call
 
    [newRequest addQueryStringParameterWhere:@"param1" equals:@"yo"];
 
 The request path would be /hello_world?param=yo.
 
 @note This method should only be used if you have designed a custom code method which accepts parameters.
 
 @param key The left side of the query string parameter.
 @param value The right side of the query string parameter.
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)addQueryStringParameterWhere:(NSString *)key equals:(NSString *)value;

@end
