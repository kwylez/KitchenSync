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
#import "AFHTTPClient.h"

typedef void (^SMTokenRefreshFailureBlock)(NSError *error, SMFailureBlock originalFailureBlock);

@class SMNetworkReachability;
@class SMOAuth2Client;
@class SMRequestOptions;

/**
 An `SMUserSession` holds all the OAuth2 credentials and configurations for the current client.  It is responsible for:
 
 * Saving and clearing credentials.
 * Re-authenticating a session using a refresh token.
 * Declaring whether a given request should be https or not.
 
 @note You should not need to instantiate your own `SMUserSession` instance.  One is initialized when creating an <SMClient> and is used to monitor authentication for all requests sent through that client.
 
 */
@interface SMUserSession : NSObject

/**
 Oauth client used for network requests over http.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property (nonatomic, readwrite, strong) SMOAuth2Client *regularOAuthClient;

/**
 Oauth client used for network requests over https.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property (nonatomic, readwrite, strong) SMOAuth2Client *secureOAuthClient;

/**
 HTTP client used for access token requests.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property (nonatomic, readwrite, strong) AFHTTPClient *tokenClient;

/**
 Monitor for tracking current network availability.
 
 @since Available in iOS SDK 1.1.3 and later.
 */
@property (nonatomic, readwrite, strong) SMNetworkReachability *networkMonitor;

/**
 Map used for identifying newly created users.
 
 @since Available in iOS SDK 1.1.0 and later.
 */
@property (nonatomic, strong) NSMutableDictionary *userIdentifierMap;

/**
 The name of the schema on StackMob used for user objects.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property (nonatomic, copy) NSString *userSchema;

/**
 The primary key field name of the user object schema on StackMob.
 
 @since Available in iOS SDK 1.1.1 and later.
 */
@property (nonatomic, copy) NSString *userPrimaryKeyField;

/**
 The password field name of the user object schema on StackMob.
 
 @since Available in iOS SDK 1.1.1 and later.
 */
@property (nonatomic, copy) NSString *userPasswordField;

/**
 The expiration date of the current session access token.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property (nonatomic, copy) NSDate *expiration;

/**
 The refresh token for the current session.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property (nonatomic, copy) NSString *refreshToken;

/**
 A boolean flag for whether an access token refresh is in progress.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
@property (atomic) BOOL refreshing;

/**
 Optional block to be executed whenever a token refresh request fails.
 
 @since Available in iOS SDK 1.4.0 and later.
 */
@property (readonly, nonatomic, copy) SMTokenRefreshFailureBlock tokenRefreshFailureBlock;

#pragma mark Init
///-------------------------------
/// @name Initialize
///-------------------------------

/**
 Makes a request to refresh the current user session using the refresh token.
 
 @param successCallbackQueue The queue to perform successBlock on.
 @param failureCallbackQueue The queue to perform failureBlock on.
 @param successBlock Upon success provides the user object.
 @param failureBlock Upon failure to refresh the session, provides the error.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (void)refreshTokenWithSuccessCallbackQueue:(dispatch_queue_t)successCallbackQueue failureCallbackQueue:(dispatch_queue_t)failureCallbackQueue onSuccess:(void (^)(NSDictionary *userObject))successBlock onFailure:(void (^)(NSError *theError))failureBlock;

/**
 Initialize a user session.
 
 @param version The API version of your StackMob application which this client instance should use.
 @param apiHost The host to connect to for API requests.
 @param publicKey Your StackMob application's OAuth2 public key.
 @param userSchema The StackMob schema that has been flagged as a user object. Default is `@"user"`.
 @param userPrimaryKeyField The StackMob primary key field name for the user object schema. Default is `@"username"`.
 @param userPasswordField The StackMob field name for the password. Default is `@"password"`.
 @return An instance of `SMUserSession` configured with the provided settings.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (id)initWithAPIVersion:(NSString *)version apiHost:(NSString *)apiHost publicKey:(NSString *)publicKey userSchema:(NSString *)userSchema userPrimaryKeyField:(NSString *)userPrimaryKeyField userPasswordField:(NSString *)userPasswordField;

#pragma mark Internal
///-------------------------------
/// @name Refreshing Session
///-------------------------------

/**
 Clears out all OAuth2 associated keys.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)clearSessionInfo;

/**
 Makes a request to refresh the current user session using the refresh token.
 
 Callback blocks are performed on the main thread.
 
 @param successBlock Upon success provides the user object.
 @param failureBlock Upon failure to refresh the session, provides the error.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)refreshTokenOnSuccess:(void (^)(NSDictionary *userObject))successBlock
                    onFailure:(void (^)(NSError *theError))failureBlock;

/**
 Set a block to be executed whenever a token refresh request fails.  Applies only to asynchronous methods with callbacks, called instead of the original failure block.
 
 The SDK has automatic token refresh built in, but sometimes this refresh fails because the refresh token is no longer valid, etc.  At this point the client needs to reset the session, the easiest way being prompting the user to re-login. This convenience method provides the ability to define a block to be executed whenever an attempt to refresh the current session access token fails.  This block will be executed in place of the original failure block provided.
 
 It is recommended in your block to clear the local session information and present the user with a login screen.  Once logged in, dismiss the login screen and continue normal execution of the application.  Doing so using this block avoids code redundancy throughout the application.
 
 The block passes two parameters, the error (some type of token refresh error), and the original failure block converted to `SMFailureBlock` form.  This gives you the option to call the original failure block if you wish.
 
 In the following example, if the fetch failed because there was an error refreshing the session access token, the refresh token failure block would get called instead:
 
    // Set block, probably during application initialization
    [self.client setTokenRefreshFailureBlock:^(NSError *error, SMFailureBlock originalFailureBlock) {
        // Reset local session info
        [self.client.session clearSessionInfo];
 
        // Show custom login screen
        [self showLoginScreen];
 
        // Optionally call original failure block
        originalFailureBlock(error);
    }];
 
    // Somewhere in application execution
    NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] initWithEntityName:@"Todo"];
 
    // execute the request
    [self.managedObjectContext executeFetchRequest:fetchRequest onSuccess:^(NSArray *results) {
        [self doSomethingWithResults:results];
    } onFailure:^(NSError *error) {
        NSLog(@"Error fetching: %@", error);
    }];
 
 **Important:** Block execution applies only to asynchronous methods with callbacks.
 
 @param block An SMTokenRefreshFailureBlock instance.
 
 @since Available in iOS SDK 1.4.0 and later.
 */
- (void)setTokenRefreshFailureBlock:(void (^)(NSError *error, SMFailureBlock originalFailureBlock))block;

#pragma mark Internal
///-------------------------------
/// @name Internal
///-------------------------------

/**
 Returns an instance of `SMOAuth2Client` configured to make requests over http or https.
 
 @param https `YES` is the client should be configured to make requests over https, `NO` for http.
 
 @return An instance of `SMOAuth2Client` configured according to the value of https.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (id)oauthClientWithHTTPS:(BOOL)https;

/**
 Sends a request to get an access token from the server for a given user session.
 
 @param endpoint The endpoint to hit depending on whether we are asking for a new access token or refreshing a current session.
 @param credentials The credentials needed to authenticate the user.
 @param options An instance of SMRequestOptions.
 @param successCallbackQueue The queue to perform successBlock on.
 @param failureCallbackQueue The queue to perform failureBlock on.
 @param successBlock Upon success provides the user object.
 @param failureBlock Upon failure to refresh the session, provides the error. 
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)doTokenRequestWithEndpoint:(NSString *)endpoint
                       credentials:(NSDictionary *)credentials
                           options:(SMRequestOptions *)options
              successCallbackQueue:(dispatch_queue_t)successCallbackQueue
              failureCallbackQueue:(dispatch_queue_t)failureCallbackQueue
                         onSuccess:(void (^)(NSDictionary *userObject))successBlock
                         onFailure:(void (^)(NSError *theError))failureBlock;

/**
 Parses the access token information out of an /accessToken or /refreshToken response.
 
 @param result The `NSDictionary` to parse.
 
 @return A new `NSDictionary` containing the access token information.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (NSDictionary *)parseTokenResults:(NSDictionary *)result;

/**
 Save the access token information from an /accessToken or /refreshToken response locally to the device.
 
 @param result The response to save.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)saveAccessTokenInfo:(NSDictionary *)result;

/**
 Signs the request.
 
 @param request The request to sign.
 
 @return The signed request.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (NSURLRequest *)signRequest:(NSURLRequest *)request;

/**
 Whether the calling session is eligible to perform an access token refresh request.
 
 @param options The tryRefreshToken property is used.
 
 @return Whether the calling session is eligible to perform an access token refresh request.
 
 @since Available in iOS SDK 1.2.0 and later.
 */
- (BOOL)eligibleForTokenRefresh:(SMRequestOptions *)options;

/**
 Internal method used to read a file which maps users to unique strings.
 
 @since Available in iOS SDK 1.1.0 and later.
 */
- (void)SMReadUserIdentifierMap;

/**
 Internal method used to save a file which maps users to unique strings.
 
 @since Available in iOS SDK 1.1.0 and later.
 */
- (void)SMSaveUserIdentifierMap;

/**
 Internal method used by `SMUserSession` to check if the expiration date on the current access token has expired.
 
 @return `YES` if the current access token has expired, otherwise `NO`.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (BOOL)accessTokenHasExpired;

@end
