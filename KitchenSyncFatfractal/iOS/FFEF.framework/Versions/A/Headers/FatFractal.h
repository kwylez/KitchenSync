//
//  FatFractal.h
//  FatFractal
//
//  Copyright (c) 2012 FatFractal, Inc. All rights reserved.
//

/**
 * @mainpage  The FatFractal SDK for iOS
 *
 *            This documentation should be used for iOS client development using the FatFractal Platform.<br>
 *            <p/> <br/>
 */
#import <Foundation/Foundation.h>
#import "FFMetaData.h"
#import "FFHttpDelegate.h"
#import "FFUser.h"
#import "FFUserGroup.h"
#import "FFUtils.h"
#import "FFGeoLocation.h"
#import "FFNotificationID.h"
#import "FFQueuedOperation.h"
#import "FFLocalQueuedOperationStore.h"

extern NSString * const FF_SCRIPT_AUTH_SERVICE_FACEBOOK;
extern NSString * const FF_SCRIPT_AUTH_SERVICE_TWITTER;

/** \brief The FFQueueDelegate protocol. */
/**
 * The FFQueueDelegate protocol provides an interface that any class can use to manage a 
 * queued operation with the FatFractal Platform.
 */
@protocol FFQueueDelegate
- (void) operationCompleted:(FFQueuedOperation *)queuedOperation;
@end

@protocol FFAnnotation
@optional
- (BOOL) shouldSerialize:(NSString *)propertyName;
@end

/** \brief The primary FatFractal class. */
/** 
 * This class provides the main methods to interact with your app's backend on the FatFractal Platform.
 */
@interface FatFractal : NSObject
{
    /** Non-public instance variable used to help memory management for object references.
     */
    NSMutableDictionary     *references;

    /**
     The #classToClazzDict property (NSMutableDictionary) holds the mapping from Obj-C Class to 
     the 'class name' that your app's backend stores with the data.
     */
    NSMutableDictionary     *classToClazzDict;
    /**
     The #clazzToClassDict property (NSMutableDictionary) holds the mapping to Obj-C Class from 
     the 'class name' that your app's backend stores with the data.
     */
    NSMutableDictionary     *clazzToClassDict;
    /**
     The list of FFQueuedOperation(s) which are queued for delivery to the application's backend.
     @see #getQueuedOperations
     @see #dequeue:
     */
    NSMutableArray          *queuedOps;
    
    dispatch_queue_t        processQueuedOperationsQueue;
    
    NSMutableDictionary     *localToRealMetadataDict;
    
    NSMutableDictionary     *scriptAuthCallbackUris;
    NSMutableDictionary     *scriptAuthTokens;
    NSMutableDictionary     *scriptAuthSecrets;
    NSMutableDictionary     *scriptAuthRequestTokens;
    NSMutableDictionary     *scriptAuthRequestSecrets;
}

#pragma mark Properties

/**
 Set the baseUrl with either #initWithBaseUrl or #initWithBaseURL:SslUrl. the application uses baseUrl to reference all resources in your app's namespace. The baseUrl is structured as either:
 scheme://organization.fatfractal.com/applicationName or scheme://localhost/applicationName . Scheme may only be set to http or https. EXCEPTIONS: for security reasons, login and register methods will use the https scheme in all cases.
 <br>You may only set the baseUrl property via #initWithBaseUrl: or #initWithBaseUrl:sslUrl:
*/
@property (strong, nonatomic, readonly)   NSString              *baseUrl;

/**
 The #sslUrl is used by FatFractal's login and register methods.
 <br>You may only set the sslUrl property via #initWithBaseUrl:sslUrl:
 If not set explicitly by the application, this will default to being identical to baseUrl but with http:
 replaced by https:
 <br>We allow specific over-riding of the sslUrl so that you may test on your local machine using non-standard
 ports. For example your baseUrl could be:
 @code http://localhost:8080/appName
 @endcode
 and your sslUrl could be
 @code https://localhost:8443/appName
 @endcode
 */
@property (strong, nonatomic, readonly)   NSString              *sslUrl;

/**
 This property is set to true on successful execution of one of the FatFractal::loginWithUserName:andPassword:
 methods. The default value is false.
 */
@property (nonatomic, readonly)           BOOL                   loggedIn;

/**
 This property is set on successful execution of one of the FatFractal::loginWithUserName:andPassword:
 methods. It is the guid of the logged-in FFUser object. Currently FFUser.guid and FFUser.userName are the same - however this will not necessarily be the case in the future.
 */
@property (strong, nonatomic, readonly)   NSString              *loggedInUserGuid;

/**
 This property is set on successful execution of one of the FatFractal::loginWithUserName:andPassword:
 methods. The default value is nil.
 */
@property (strong, nonatomic, readonly)   FFUser                *loggedInUser;

/**
 This property is set on successful execution of one of the FatFractal::loginWithUserName:andPassword:
 methods. The default value is nil. The #loggedInSessionId is generated on the server on successful login and set as a cookie by the server; subsequent requests from the client include the session cookie so that the server knows it is a valid logged-in client which is sending the request. (Requests which are sent without the session cookie are treated as anonymous and will succeed for GET requests where the object(s) being requested are public-readable)
 */
@property (strong, nonatomic, readonly)   NSString              *loggedInSessionId;

/**
 The #debug property (BOOL) is the Boolean representation of debug mode for the FatFractal 
 class.  Set the debug property to true for verbose NSLog output. The default value is false.
 */
@property (nonatomic)                   BOOL                    debug;

/**
 When true, referred objects are automatically loaded when retrieving from the application's backend. Default is true.
 If set to false, then #loadRefsForObj:onComplete can be called to explicitly load the referred objects.
 @see #loadRefsForObj:onComplete:
 */
@property (nonatomic)                   BOOL                    autoLoadRefs;
/**
 When true, blobs are automatically loaded when retrieving from the application's backend. Default is true.
 If set to false, then #loadBlobsForObj:onComplete can be called to explicitly load the blobs.
 @see #loadBlobsForObj:onComplete:
 */
@property (nonatomic)                   BOOL                    autoLoadBlobs;

/**
 For ease of testing offline behaviour. When true, all attempts to make HTTP requests will fail as if the server were unreachable.
 */
@property (nonatomic)                   BOOL                    simulatingOffline;

/**
 For ease of testing behavior when requests fail. When greater than zero, the next HTTP request will fail, and this value will be decremented by 1.
 */
@property (atomic)                      int                     simulateHttpFailureForNextCalls;

/**
 The delegate's FFQueueDelegate::operationCompleted: method is called (on the main thread)
 when a previously-queued create / update / delete operation successfully completes.
 */
@property (strong, nonatomic)           id <FFQueueDelegate>    queueDelegate;

/**
 A string unique (within your application) to a particular logical FatFractal instance.
 Set using proper init method (#initWithBaseUrl:localQueuedOpStoreKey: or #initWithBaseUrl:sslUrl:localQueuedOpStoreKey:) to enable persistence of queued operations to a local database, which will allows queued operations to remain queued after your app restarts.
 <br><br><strong>Note:</strong> More than one FatFractal instance with the same key will result in unspecified behavior.
 @see #initWithBaseUrl:localQueuedOpStoreKey:, #initWithBaseUrl:sslUrl:localQueuedOpStoreKey:
 */
@property (strong, nonatomic, readonly) NSString *localQueuedOpStoreKey;

/**
 Get the callback URI for the specified ScriptAuth service.
 @param scriptAuthService   The ScriptAuth service
 @return The callback URI
 */
- (NSString *)callbackUriForScriptAuthService:(NSString *)scriptAuthService;

/**
 Set the callback URI for the specified ScriptAuth service.
 @param callbackUri         The callback URI
 @param scriptAuthService   The ScriptAuth service
 */
- (void)setCallbackUri:(NSString *)callbackUri forScriptAuthService:(NSString *)scriptAuthService;

/**
 Get the access token for the specified ScriptAuth service.
 @param scriptAuthService   The ScriptAuth service
 @return The access token
 @see #secretForScriptAuthService:
 */
- (NSString *)tokenForScriptAuthService:(NSString *)scriptAuthService;

/**
 Set the access token for the specified ScriptAuth service.
 @param token               The access token
 @param scriptAuthService   The ScriptAuth service
 @see #setToken:andSecret:forScriptAuthService:
 @see #setSecret:forScriptAuthService:
 */
- (void)setToken:(NSString *)token forScriptAuthService:(NSString *)scriptAuthService;

/**
 Get the access token secret for the specified ScriptAuth service.
 @param scriptAuthService   The ScriptAuth service
 @return The access token secret
 @see #tokenForScriptAuthService:
 */
- (NSString *)secretForScriptAuthService:(NSString *)scriptAuthService;

/**
 Set the access token secret for the specified ScriptAuth service.
 @param secret              The access token secret
 @param scriptAuthService   The ScriptAuth service
 @see #setToken:forScriptAuthService:
 @see #setToken:andSecret:forScriptAuthService:
 */
- (void)setSecret:(NSString *)secret forScriptAuthService:(NSString *)scriptAuthService;

/**
 Set the access token and secret for the specified ScriptAuth service.
 @param token               The access token
 @param secret              The access token secret
 @param scriptAuthService   The ScriptAuth service
 @see #setToken:forScriptAuthService:
 @see #setSecret:forScriptAuthService:
 */
- (void)setToken:(NSString *)token andSecret:(NSString *)secret forScriptAuthService:(NSString *)scriptAuthService;

/**
 Clear the access token and secret for the specified ScriptAuth service.
 @param scriptAuthService   The ScriptAuth service
 */
- (void)clearTokenForScriptAuthService:(NSString *)scriptAuthService;

#pragma mark Lifecycle

/**
 FatFractal is not a singleton class; however in most cases there will be only a single instance.
 This method returns the "main" instance, which defaults to being the first FatFractal instance
 which was created, but can also be explicitly set via #setMain:
 @return FatFractal * - returns a FatFractal instance
 */
+ (FatFractal *) main;

/**
 Explicitly set the FatFractal instance which #FatFractal::main returns.
 @param FatFractal _main : sets the FatFractal instance
 */
+ (void) setMain:(FatFractal *)_main;

/**
You will only need to use this method if you are testing your app's backend on a development machine and are 
 using non-standard ports - otherwise, use #initWithBaseUrl:
 <br><br>For illustration you might call:
 @code initWithBaseUrl:@"http://localhost:8080/AppName" sslUrl:@"https://localhost:8443/AppName"
 @endcode 
 If you wish all of your network traffic to be sent via SSL, you can also call:
 @code initWithBaseUrl:@"HTTPS://localhost:8443/AppName" sslUrl:@"https://localhost:8443/AppName"
 @endcode
 @param NSString url that will be used to set the #baseUrl property
 @param NSString sslUrl that will be used to set the #baseUrl property
 @return id - returns the initialized instance
 @see #initWithBaseUrl:
 */
- (id) initWithBaseUrl:(NSString *)url sslUrl:(NSString *)sslUrl;

/**
 Similar to #initWithBaseUrl:sslUrl:, but also enables the local queued operation store.
 This stores queued operations to a local database, so that queued operations are not lost in the event of app restart.
 <br><br><strong>Note:</strong> The local queued operation store requires your project to link to libsqlite3.dylib.
 @param url     The URL used to set the #baseUrl property
 @param sslUrl  The URL used to set the #sslUrl property
 @param key     A unique string used to identify this FatFractal instance. <strong>Only one instance with a given key should exist simultaneously.</strong>
 @return        A new FatFractal instance
 @see #initWithBaseUrl:sslUrl:
 */
- (id) initWithBaseUrl:(NSString *)url sslUrl:(NSString *)sslUrl localQueuedOpStoreKey:(NSString *)key;

/**
 The FatFractal class will set two properties on initialization: #baseUrl and #sslUrl. The #baseUrl property 
 will be set to the value of the url parameter which must be of the form:
 @code scheme://organization.fatfractal.com/applicationName
 @endcode
 or 
 @code scheme://localhost:port/applicationName
 @endcode
 Where the scheme must be either http or https. If you use this method, the #sslUrl property will be identical 
 to #baseUrl except for the scheme which will always be set to https.
 @param NSString
 @return id - returns the initialized instance
 @see #initWithBaseUrl:sslUrl
 */
- (id) initWithBaseUrl:(NSString *)url;

/**
 Similar to #initWithBaseUrl:, but also enables the local queued operation store.
 This stores queued operations to a local database, so that queued operations are not lost in the event of app restart.
 <br><br><strong>Note:</strong> The local queued operation store requires your project to link to libsqlite3.dylib.
 @param url     The URL used to set the #baseUrl property
 @param key     A unique string used to identify this FatFractal instance. <strong>Only one instance with a given key should exist simultaneously.</strong>
 @return        A new FatFractal instance
 @see #initWithBaseUrl:
 */
- (id) initWithBaseUrl:(NSString *)url localQueuedOpStoreKey:(NSString *)key;

#pragma mark Dynamic typing

/*!
 As you develop apps on multiple client platforms, you will need to maintain a mapping from canonical (server)
 'class' (which we call "clazz" in our API) to Obj-C class. To get started, you can ignore this; the "clazz"
 will default to the Obj-C Class and vice versa.
 @param Class _class (required) for the desired class to be registered with the FatFractal class. 
 @param NSString _clazz (required) is the String value which is added to objects when serializing for the application's backend,
 and used by the deserializer to construct an instance of this class when retrieving from the application's backend.
 */
- (void) registerClass:(Class)_class forClazz:(NSString *)_clazz;

#pragma mark Authentication and Authorization

/**
 Asynchronous method with an onComplete callback for registering an FFUser with the FatFractal backend with a password.
 @param FFUser the FFUser object to be registered
 @param NSString password the password for the user
 @param FFHttpMethodCompletion the block defined here will execute when the HTTP call completes
 @return <b>void</b> Currently a successful registration will also log the user in, and set the #loggedInSessionId, #loggedInUser and #loggedIn properties. This method does not return anything directly - response is via the FFHttpMethodCompletion block, which has these parameters:
 <br><b>(NSError *)</b> - non-nil if there is an error. An error will occur if a user with this userName already exists on your app's backend.
 <br><b>(id)</b> - the registered FFUser object, or nil if there is an error
 <br><b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 */
- (void) registerUser:(FFUser *)user password:(NSString *)password onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Asynchronous method with an onComplete callback for registering an FFUser with the FatFractal backend with a Facebook ID.
 @param FFUser an FFUser object with the username should be set to the user's Facebook ID.
 @param NSString access_token as provided by the Facebook API
 @param FFHttpMethodCompletion the block defined here will execute when the HTTP call completes
 @return <b>void</b> Currently a successful registration will also log the user in, and set the #loggedInSessionId, #loggedInUser and #loggedIn properties. This method does not return anything directly - response is via the FFHttpMethodCompletion block, which has these parameters:
 <br><b>(NSError *)</b> - non-nil if there is an error. An error will occur if a user with this userName already exists on your app's backend.
 <br><b>(id)</b> - the registered FFUser object, or nil if there is an error.
 <br><b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 */
- (void) registerUser:(FFUser *)user withFacebookToken:(NSString *)access_token onComplete:(FFHttpMethodCompletion)onComplete;

/*
 Asynchronous method with an onComplete callback for registering an FFUser with the FatFractal backend with a Twitter ID.
 <br>Will request user access to the twitter accounts on the device; the backend will then use the oauth echo mechanism to verify the oauth access token.
 @param FFUser an FFUser object. The username should be set to the user's Twitter ID.
 @param FFHttpMethodCompletion the block defined here will execute when the HTTP call completes
 @return <b>void</b> Currently a successful registration will also log the user in, and set the #loggedInSessionId, #loggedInUser and #loggedIn properties. This method does not return anything directly - response is via the FFHttpMethodCompletion block, which has these parameters:
 <br><b>(NSError *)</b> - non-nil if there is an error. An error will occur if a user with this userName already exists on your app's backend.
 <br><b>(id)</b> - the registered FFUser object, or nil if there is an error.
 <br><b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 */
- (void)    registerTwitterUser:(id)user
           xAuthServiceProvider:(NSString *)xAuthServiceProvider
xVerifyCredentialsAuthorization:(NSString *)xVerifyCredentialsAuthorization
                     onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Synchronous method with output parameter, error for registering an FFUser with the FatFractal backend with a password.
 <br>(For access to HTTP status codes, use #registerUser:password:onComplete: )
 @param FFUser user the FFUser object to be registered
 @param NSString password the password for the user
 @param NSError outErr will be non-nil if an error has occurred. An error will occur if a user with this userName already exists on your app's backend.
 @return <b>FFUser</b> - the registered user. Currently a successful registration will also log the user in, and set the #loggedInSessionId, #loggedInUser and #loggedIn properties.
 */
- (FFUser *) registerUser:(FFUser *)user password:(NSString *)password error:(NSError **)outErr;

/**
 Asynchronous method to register a new user via a ScriptAuth service.
 Saved token (and secret, if applicable) will be used as credentials.
 @param scriptAuthService   The ScriptAuth service with which to register
 @param onComplete          Completion block called when method has completed
 @see #registerWithScriptAuthService:error:
 @see #registerWithScriptAuthService:usingCredentials:onComplete:
 @see #registerWithScriptAuthService:usingCredentials:error:
 */
- (void)registerWithScriptAuthService:(NSString *)scriptAuthService onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Synchronous method to register a new user via a ScriptAuth service.
 Saved token (and secret, if applicable) will be used as credentials.
 @param[in] scriptAuthService   The ScriptAuth service with which to register
 @param[out] outErr             Error
 @see #registerWithScriptAuthService:onComplete:
 @see #registerWithScriptAuthService:usingCredentials:onComplete:
 @see #registerWithScriptAuthService:usingCredentials:error:
 @return Newly registered user if successful, nil otherwise
 */
- (FFUser *)registerWithScriptAuthService:(NSString *)scriptAuthService error:(NSError **)outErr;

/**
 Asynchronous method to register a new user via a ScriptAuth service with explicit credentials.
 @param scriptAuthService   The ScriptAuth service with which to register
 @param credentials         Credentials dictionary
 @param onComplete          Completion block called when method has completed
 @see #registerWithScriptAuthService:onComplete:
 @see #registerWithScriptAuthService:error:
 @see #registerWithScriptAuthService:usingCredentials:error:
 */
- (void)registerWithScriptAuthService:(NSString *)scriptAuthService usingCredentials:(NSDictionary *)credentials onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Synchronous method to register a new user via a ScriptAuth service with explicit credentials.
 @param[in] scriptAuthService   The ScriptAuth service with which to register
 @param[in] credentials         Credentials dictionary
 @param[out] outErr             Error
 @see #registerWithScriptAuthService:onComplete:
 @see #registerWithScriptAuthService:error:
 @see #registerWithScriptAuthService:usingCredentials:onComplete:
 @return Newly registered user if successful, nil otherwise
 */
- (FFUser *)registerWithScriptAuthService:(NSString *)scriptAuthService usingCredentials:(NSDictionary *)credentials error:(NSError **)outErr;

/**
 Asynchronous method with an onComplete callback for logging in a registered FFUser using a combination of userName and password.
 <br>Login will succeed if there exists a registered FFUser with this userName and password. If you have set AllowAutoRegistration to true in the application.ffdl configuration, then login will also succeed if this userName does not exist in your app's backend. See also #registerUser:password
 @param NSString userName for the user logging in
 @param NSString password for the user logging in
 @param FFHttpMethodCompletion onComplete : the block defined here will execute when the HTTP call completes 
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion block which has these parameters:
 <br><b>(NSError *)</b> - non-nil if there is an error
 <br><b>(id)</b> - the logged-in FFUser object, or nil if there is an error
 <br><b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 <br> If login was successful, the values for the following properties are set. 
 <br><b>NSString #loggedInSessionId</b> is set to the SessionId returned by your app's backend
 <br><b>BOOL #loggedIn</b> is set to true.
 @see #registerUser:password:
 @see FFHttpDelegate::onComplete
 */
- (void) loginWithUserName:(NSString *)userName 
               andPassword:(NSString *)password
                onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Asynchronous method with an onComplete callback for logging in a registered FFUser with a FaceBook ID and access token.
 <br>Login will succeed if there exists a registered FFUser which corresponds with this Facebook access token.
 @param NSString facebook_id : the Facebook ID
 @param NSString access_token : the Facebook access token
 @param FFHttpMethodCompletion onComplete : the block defined here will execute when the HTTP call completes
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion block which has these parameters:
 <br><b>(NSError *)</b> - non-nil if there is an error
 <br><b>(id)</b> - the logged-in FFUser object, or nil if there is an error
 <br><b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 <br> If login was successful, the values for the following properties are set. 
 <br><b>NSString #loggedInSessionId</b> is set to the SessionId returned by your app's backend
 <br><b>BOOL #loggedIn</b> is set to true.
 @see #registerUser:password:
 @see FFHttpDelegate::onComplete
 */
- (void) loginWithFacebookID:(NSString *)facebook_id andToken:(NSString *)access_token onComplete:(FFHttpMethodCompletion)onComplete;

/*
 Removed until fully implemented on backend
 Asynchronous method with an onComplete callback for logging in a registered FFUser using their Twitter ID.
 <br>Will request user access to the twitter accounts on the device; the backend will then use the oauth echo mechanism to verify the oauth access token.
 <br>Login will succeed if there exists a registered FFUser with this Twitter ID.
 @param NSString twitter_id : the Twitter ID
 @param FFHttpMethodCompletion onComplete : the block defined here will execute when the HTTP call completes
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion block which has these parameters:
 <br><b>(NSError *)</b> - non-nil if there is an error
 <br><b>(id)</b> - the logged-in FFUser object, or nil if there is an error
 <br><b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 <br> If login was successful, the values for the following properties are set. 
 <br><b>NSString #loggedInSessionId</b> is set to the SessionId returned by your app's backend
 <br><b>BOOL #loggedIn</b> is set to true.
 @see #registerUser:password:
 @see FFHttpDelegate::onComplete
 */
- (void)     loginWithTwitterID:(NSString *)twitter_id
           xAuthServiceProvider:(NSString *)xAuthServiceProvider
xVerifyCredentialsAuthorization:(NSString *)xVerifyCredentialsAuthorization
                     onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Synchronous method with output parameter, error for logging in a registered FFUser using a combination of userName and password.
 <br>(For access to HTTP status codes, use #loginWithUserName:andPassword:onComplete: )
 <br>Login will succeed if there exists a registered FFUser with this userName and password. If you have set AllowAutoRegistration to true in the application.ffdl configuration, then login will also succeed if this userName does not exist in your app's backend. See also #registerUser:password
 @param NSString userName of the user logging in
 @param NSString password of the user logging in
 @param NSError outErr : will be non-nil if an error has occurred
 @return <b>FFUser</b> - returns the logged-in FFUser, if the login was successful, nil otherwise.
 Additionally upon success, the values for some other properties that are accessible by your
 application are set:
 <br><b>NSString loggedInSessionId</b> is set to the SessionId returned by your app's backend
 <br><b>NSString loggedInUserName</b> is set to the UserName
 <br><b>BOOL loggedIn</b> is set to true.
 @see #registerUser:password:
 */
- (FFUser *) loginWithUserName:(NSString *)theUserName andPassword:(NSString *)thePassword error:(NSError **)outErr;

/**
 Synchronous method for logging in a registered FFUser using a combination of userName and password.
 <br>(For access to HTTP status codes, use #loginWithUserName:andPassword:onComplete: )
 <br>Login will succeed if there exists a registered FFUser with this userName and password. If you have set AllowAutoRegistration to true in the application.ffdl configuration, then login will also succeed if this userName does not exist in your app's backend. See also #registerUser:password
 @param NSString the userName of the user logging in
 @param NSString password of the user logging in
 @return <b>FFUser</b> - the logged-in FFUser, if the login was successful, nil otherwise.
 Additionally upon success, the values for some other properties that are accessible by your
 application are set:
 <br><b>NSString loggedInSessionId</b> is set to the SessionId returned by your app's backend
 <br><b>NSString loggedInUserName</b> is set to the UserName
 <br><b>BOOL loggedIn</b> is set to true.
 @see #registerUser:password:
 */
- (FFUser *) loginWithUserName:(NSString *)theUserName andPassword:(NSString *)thePassword;

/**
 Asynchronous method to log in via a ScriptAuth service. Saved token (and secret, if applicable) will be used as credentials.
 @param scriptAuthService   The ScriptAuth service with which to register
 @param onComplete          Completion block called when method has completed
 @see #loginWithScriptAuthService:error:
 @see #loginWithScriptAuthService:
 @see #loginWithScriptAuthService:usingCredentials:onComplete:
 @see #loginWithScriptAuthService:usingCredentials:error:
 @see #loginWithScriptAuthService:usingCredentials:
 */
- (void)loginWithScriptAuthService:(NSString *)scriptAuthService onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Synchronous method to log in via a ScriptAuth service. Saved token (and secret, if applicable) will be used as credentials.
 @param[in] scriptAuthService   The ScriptAuth service with which to register
 @param[out] outErr             Error
 @return User if successful, nil otherwise
 @see #loginWithScriptAuthService:onComplete:
 @see #loginWithScriptAuthService:
 @see #loginWithScriptAuthService:usingCredentials:onComplete:
 @see #loginWithScriptAuthService:usingCredentials:error:
 @see #loginWithScriptAuthService:usingCredentials:
 */
- (FFUser *)loginWithScriptAuthService:(NSString *)scriptAuthService error:(NSError **)outErr;

/**
 Synchronous method to log in via a ScriptAuth service. Saved token (and secret, if applicable) will be used as credentials.
 @param scriptAuthService   The ScriptAuth service with which to register
 @return User if successful, nil otherwise
 @see #loginWithScriptAuthService:onComplete:
 @see #loginWithScriptAuthService:error:
 @see #loginWithScriptAuthService:usingCredentials:onComplete:
 @see #loginWithScriptAuthService:usingCredentials:error:
 @see #loginWithScriptAuthService:usingCredentials:
 */
- (FFUser *)loginWithScriptAuthService:(NSString *)scriptAuthService;

/**
 Asynchronous method to log in via a ScriptAuth service with explicit credentials.
 @param scriptAuthService   The ScriptAuth service with which to register
 @param credentials         Credentials dictionary
 @param onComplete          Completion block called when method has completed
 @see #loginWithScriptAuthService:onComplete:
 @see #loginWithScriptAuthService:error:
 @see #loginWithScriptAuthService:
 @see #loginWithScriptAuthService:usingCredentials:error:
 @see #loginWithScriptAuthService:usingCredentials:
 */
- (void)loginWithScriptAuthService:(NSString *)scriptAuthService usingCredentials:(NSDictionary *)credentials onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Synchronous method to log in via a ScriptAuth service with explicit credentials.
 @param[in] scriptAuthService   The ScriptAuth service with which to register
 @param[in] credentials         Credentials dictionary
 @param[out] outErr             Error
 @return User if successful, nil otherwise
 @see #loginWithScriptAuthService:onComplete:
 @see #loginWithScriptAuthService:error:
 @see #loginWithScriptAuthService:
 @see #loginWithScriptAuthService:usingCredentials:onComplete:
 @see #loginWithScriptAuthService:usingCredentials:
 */
- (FFUser *)loginWithScriptAuthService:(NSString *)scriptAuthService usingCredentials:(NSDictionary *)credentials error:(NSError **)outErr;

/**
 Synchronous method to log in via a ScriptAuth service with explicit credentials.
 @param scriptAuthService   The ScriptAuth service with which to register
 @param credentials         Credentials dictionary
 @return User if successful, nil otherwise
 @see #loginWithScriptAuthService:onComplete:
 @see #loginWithScriptAuthService:error:
 @see #loginWithScriptAuthService:
 @see #loginWithScriptAuthService:usingCredentials:onComplete:
 @see #loginWithScriptAuthService:usingCredentials:error:
 */
- (FFUser *)loginWithScriptAuthService:(NSString *)scriptAuthService usingCredentials:(NSDictionary *)credentials;


/**
 Explicit logout from your app's backend.
 <br>Sets #loggedInSessionId to nil to nil, #loggedInUser to nil, #loggedIn to false
 */
- (void) logout;

/**
 Asynchronous method to get the authorization URI for the specified ScriptAuth service.
 @param scriptAuthService   The ScriptAuth service
 @param onComplete          Completion block called when method has completed
 @see #authUriForScriptAuthService:error:
 @see #authUriForScriptAuthService:
 */
- (void)authUriForScriptAuthService:(NSString *)scriptAuthService onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Synchronous method to get the authorization URI for the specified ScriptAuth service.
 @param[in] scriptAuthService   The ScriptAuth service with which to register
 @param[out] outErr             Error
 @return The authorization URI
 @see #authUriForScriptAuthService:onComplete:
 @see #authUriForScriptAuthService:
 */
- (NSString *)authUriForScriptAuthService:(NSString *)scriptAuthService error:(NSError **)outErr;

/**
 Synchronous method to get the authorization URI for the specified ScriptAuth service.
 @param scriptAuthService   The ScriptAuth service with which to register
 @return The authorization URI
 @see #authUriForScriptAuthService:onComplete:
 @see #authUriForScriptAuthService:error:
 */
- (NSString *)authUriForScriptAuthService:(NSString *)scriptAuthService;

/**
 Asynchronous method to retrieve an access token (and secret, if applicable) from the specified ScriptAuth service.
 Token is stored in-memory via #setToken:andSecret:forScriptAuthService:.
 @param scriptAuthService       The ScriptAuth service with which to register
 @param callbackUriWithVerifier The callback URI redirected to after OAuth authentication, which includes an OAuth verifier
 @param onComplete              Completion block called when method has completed
 */
- (void)retrieveAccessTokenForScriptAuthService:(NSString *)scriptAuthService callbackUriWithVerifier:(NSString *)callbackUriWithVerifier onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Synchronous method to retrieve an access token (and secret, if applicable) from the specified ScriptAuth service.
 Token is stored in-memory via #setToken:andSecret:forScriptAuthService:.
 @param[in] scriptAuthService       The ScriptAuth service with which to register
 @param[in] callbackUriWithVerifier The callback URI redirected to after OAuth authentication, which includes an OAuth verifier
 @param[out] outErr                 Error
 */
- (void)retrieveAccessTokenForScriptAuthService:(NSString *)scriptAuthService callbackUriWithVerifier:(NSString *)callbackUriWithVerifier error:(NSError **)outErr;

/**
 Synchronous method to retrieve an access token (and secret, if applicable) from the specified ScriptAuth service.
 Token is stored in-memory via #setToken:andSecret:forScriptAuthService:.
 @param scriptAuthService       The ScriptAuth service with which to register
 @param callbackUriWithVerifier The callback URI redirected to after OAuth authentication, which includes an OAuth verifier
 */
- (void)retrieveAccessTokenForScriptAuthService:(NSString *)scriptAuthService callbackUriWithVerifier:(NSString *)callbackUriWithVerifier;

/**
 Synchronous method with output parameter, error
 <br>Set an object-specific ACL. Permissions are granted to read or write an object by user and by group. Once permission is granted to read or write an object by user, that permission persists until the method is reissued. If users are added or taken out of groups, the corresponding permissions for that group obtain dynamically.
 @see #setDefaultPermissionOnObject:error:
 @param id obj : the object for which we are setting permissions
 @param NSArray readUsers   : must be an array of FFUser each of which is to be given read access to the object
 @param NSArray writeUsers  : must be an array of FFUser each of which is to be given write access to the object
 @param NSArray readGroups  : must be an array of FFUserGroup each of which is to be given read  access to the object
 @param NSArray writeGroups : must be an array of FFUserGroup each of which is to be given write access to the object
 @param NSError outErr : will be non-nil on return if an error has occurred
 */
- (void) setPermissionOnObject:(id)obj
                     readUsers:(NSArray *)readUsers
                    readGroups:(NSArray *)readGroups
                    writeUsers:(NSArray *)writeUsers
                   writeGroups:(NSArray *)writeGroups
                         error:(NSError **)outErr;

/**
 Synchronous method with output parameter, error that sets the default permissions for this object. If object has a specific ACL, it is removed 
 so that the defaults (PERMIT commands) apply. 
 @see #setPermissionOnObject:readUsers:readGroups:writeUsers:writeGroups:error:
 @param id obj : the object for which we are setting the default permissions
 @param NSError outErr : will be non-nil on return if an error has occurred
 */
- (void) setDefaultPermissionOnObject:(id)obj error:(NSError **)outErr;

#pragma mark Push support

/**
 Synchronous method that stores the push notification id locally. If logged in, will synchronize with your app's backend.
 @param NSString idValue : the notification identifier. If == nil, equivalent to calling unregisterNotificationID.
 @return <b>NSError</b> - returns non-nil if any error has occurred
 */
- (NSError *) registerNotificationID:(NSString *) idValue;

/**
 Synchronous method that removes the push notification id locally. If logged in, will synchronize with your app's backend.
 @return <b>NSError</b> returns non-nil if any error has occurred
 */
- (NSError *) unregisterNotificationID;

/**
 The push notification id which is in local storage.
 @return <b>NSString</b> - returns the notificationId else the empty string @"" if there is nothing stored locally.
 */
- (NSString *) getNotificationID;

/**
 The guid from your app's backend which corresponds to the notification id which is in local storage.
 @return <b>NSString</b> - the guid from your app's backend which corresponds to the notificationId else the empty 
 string @"" if there is nothing stored locally.
 */
- (NSString *) getBackendNotificationGuid;

#pragma mark Lifecycle support for your persistent objects, via HTTP to your Fat Fractal backend

/**
 Asynchronous method with an onComplete callback that will persist an object to the application's backend.
 @param id obj : the object to be persisted on your app's backend
 @param NSString atUri : the uri for the object relative to the #baseUrl property (which is set by FatFractal::initWithBaseUrl: )
 @param FFHttpMethodCompletion onComplete : the block defined here will execute when the HTTP call completes
 @return <b>void</b> does not return anything directly - response is via the FFHttpDelegate::onComplete block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - the object created and passed back, or nil if there is an error
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 <br>
 <br>on success, the object will have associated #FFMetaData
 @see FFHttpDelegate::onComplete
 */
- (void) createObj:(id)obj atUri:(NSString *)ffUrl onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Asynchronous method with an onComplete callback and an onOffline callback that will persist an object to the application's backend.
 @param id obj : the object to be created and persisted on your app's backend
 @param NSString atUri : the uri for the object relative to the #baseUrl property (which is set by FatFractal::initWithBaseUrl: )
 @param FFHttpMethodCompletion onComplete : the block defined here will execute when the HTTP call completes
 @param FFHttpMethodCompletion onOffline : the block defined here will execute if the application's backend is not reachable and the operation has been queued via
 #queueCreateObj:atUri:
 @return <b>void</b> does not return anything directly - response is via the FFHttpDelegate::onComplete onComplete 
 or #FFHttpDelegate::onOffline block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - nil if there is an error; the passed object, if it's an #onComplete block; or an #FFQueuedOperation, if it's an #onOffline block
 <br>   <b>(NSHTTPURLResponse *)</b> - the full #NSHTTPURLResponse
 <br>on success, the object will have associated #FFMetaData
 @see FFHttpDelegate::onComplete
 @see FFHttpDelegate::onOffline
 */
- (void) createObj:(id)obj atUri:(NSString *)ffUrl onComplete:(FFHttpMethodCompletion)onComplete onOffline:(FFHttpMethodCompletion)onOffline;

/**
 @deprecated - please use #createObj:atUri:onComplete:
 */
- (void) createObj:(id)obj atUrl:(NSString *)ffUrl onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Synchronous method that will persist an object to the application's backend.
 <br>(For access to HTTP status codes, use #createObj:atUri:onComplete: )
 @param id obj : the object to be created and persisted on your app's backend
 @param NSString atUri : the uri for the object relative to the #baseUrl property (which is set by FatFractal::initWithBaseUrl: )
 @return <b>id</b> - returns the object created and passed back, or nil if there is an error
 <br>on success, the object will have associated #FFMetaData
 */
- (id) createObj:(id)obj atUri:(NSString *)ffUrl;

/**
 @deprecated - please use #createObj:atUri:
 */
- (id) createObj:(id)obj atUrl:(NSString *)ffUrl;

/**
 Synchronous method with output parameter, error that will persist an object to the application's backend.
 <br>(For access to HTTP status codes, use #createObj:atUri:onComplete: )
 @param id obj : the object to be created and persisted on your app's backend
 @param NSString atUri : the uri for the object relative to the #baseUrl property (which is set by FatFractal::initWithBaseUrl: )
 @param NSError outErr : non-nil if there is an error
 @return <b>id</b> - returns the object created and passed back, or nil if there is an error
 <br>on success, the object will have associated #FFMetaData
 */
- (id) createObj:(id)obj atUri:(NSString *)ffUrl error:(NSError **)outErr;

/**
 @deprecated - please use #createObj:atUri:error:
 */
- (id) createObj:(id)obj atUrl:(NSString *)ffUrl error:(NSError **)outErr;

/**
 Asynchronous method with an onComplete callback which can be called to explicitly load blobs for an object from your
 application's backend when #autoLoadBlobs is set to false.
 @param id obj : the object for which the blobs will be loaded
 @param FFHttpMethodCompletion the block defined here will execute when the HTTP call completes
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - the object which was passed
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see #autoLoadBlobs
 @see FFHttpDelegate::onComplete
 */
- (void) loadBlobsForObj:(id)obj onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Asynchronous method with an onComplete callback which can be called to explicitly load references for an object 
 from the application's backend when #autoLoadRefs is set to false.
 @param id obj : the object for which the referred objects will be loaded
 @param FFHttpMethodCompletion the block defined here will execute when the HTTP call completes
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - the object which was passed
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see #autoLoadRefs
 @see FFHttpDelegate::onComplete
 */
- (void) loadRefsForObj:(id)obj onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Asynchronous method with an onComplete callback that will return an array of objects from a single collection 
 from the application's backend.
 @param NSString ffUrl : the uri for the array of objects relative to the #baseUrl property (which is set by FatFractal::initWithBaseUrl: )
 @param FFHttpMethodCompletion onComplete : the block defined here will execute when the HTTP call completes
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - the retrieved array of objects, or nil if there is an error
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 */
- (void) getArrayFromUri:(NSString *)ffUrl onComplete:(FFHttpMethodCompletion)onComplete;

/**
 @deprecated - please use #getArrayFromUri:onComplete:
 */
- (void) getArrayFromUrl:(NSString *)ffUrl onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Asynchronous method with an onComplete callback that expects an Array of objects to be returned from a server extension. 
 Identical in behaviour to #getArrayFromUri:onComplete: except will prefix the uri with /ff/ext/ instead of /ff/resources/
 @param NSString extensionUri : the name of the server extension
 @param FFHttpMethodCompletion onComplete : the block defined here will execute when the HTTP call completes
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - the retrieved array of objects, or nil if there is an error
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 */
- (void) getArrayFromExtension:(NSString *)extensionUri onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Asynchronous method with an onComplete callback that will POST some data to a server extension.
 @param id obj : the object to be seriaized and POSTed to your server exension
 @param NSString extensionUri : the name of the server extension
 @param FFHttpMethodCompletion onComplete : the block defined here will execute when the HTTP call completes
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - the response from the server, or nil if there is an error
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 */
- (void) postObj:(id)obj toExtension:(NSString *)extensionUri onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Asynchronous method with an onComplete callback and an onOffline callback that will POST some data to a server extension.
 @param id obj : the object to be seriaized and POSTed to your server exension
 @param NSString extensionUri : the name of the server extension
 @param FFHttpMethodCompletion onComplete : the block defined here will execute when the HTTP call completes 
 @param FFHttpMethodCompletion onOffline : the block defined here will execute if the application's backend is not reachable and the operation has been queued via
 #queueCreateObj:atUri:
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion onComplete or onOffline block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - nil if there is an error; the response from the server, if it's an onComplete block; or an #FFQueuedOperation, if it's an onOffline block
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 */
- (void) postObj:(id)obj toExtension:(NSString *)extensionUri onComplete:(FFHttpMethodCompletion)onComplete onOffline:(FFHttpMethodCompletion)onOffline;

/**
 Synchronous method that will return an array of objects from a single collection from the application's backend.
 <br>(For access to HTTP status codes, use #getArrayFromUri:onComplete: )
 @param NSString ffUrl : the uri for the array of objects relative to the #baseUrl property (which is set by FatFractal::initWithBaseUrl: )
 @return <b>NSArray</b> - the retrieved array of objects, or nil if there is an error
 <br>on success, each object in the array will have its own associated #FFMetaData
 */
- (NSArray *) getArrayFromUri:(NSString *)ffUrl;

/**
 @deprecated - please use #getArrayFromUri:
 */
- (NSArray *) getArrayFromUrl:(NSString *)ffUrl;

/**
 Synchronous method with output parameter, error that will return an array of objects from a single collection from the application's backend.
 <br>(For access to HTTP status codes, use #getArrayFromUri:onComplete: )
 @param NSString ffUrl : the uri for the array of objects relative to the #baseUrl property (which is set by FatFractal::initWithBaseUrl: )
 @param NSError outErr : non-nil if there is an error
 @return <b>NSArray</b> - the retrieved array of objects, or nil if there is an error
 <br>on success, each object in the array will have its own associated #FFMetaData
 */
- (NSArray *) getArrayFromUri:(NSString *)ffUrl error:(NSError **)outErr;
/**
 @deprecated - please use #getArrayFromUri:error:
 */
- (NSArray *) getArrayFromUrl:(NSString *)ffUrl error:(NSError **)outErr;

/**
 Asynchronous method with onComplete callback that will return an array of objects from a single collection from the application's backend.
 @param NSString ffUrl : the uri for the object relative to the #baseUrl property (which is set by FatFractal::initWithBaseUrl: )
 @param FFHttpMethodCompletion onComplete : the block defined here will execute when the HTTP call completes 
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - the retrieved object, or nil if there is an error
 <br>(NOTE: It is an error if zero, or more than one object, is found at this url)
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 */
- (void) getObjFromUri:(NSString *)ffUrl onComplete:(FFHttpMethodCompletion)onComplete;

/**
 @deprecated - please use #getObjFromUri:onComplete:
 */
- (void) getObjFromUrl:(NSString *)ffUrl onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Synchronous method with output parameter, error that will return a single object from a single collection from the application's backend.
 <br>(For access to HTTP status codes, use #getObjFromUri:onComplete: )
 @param NSString ffUrl : the uri for the object relative to the #baseUrl property (which is set by FatFractal::initWithBaseUrl: )
 @param NSError outErr : non-nil if there is an error
 @return <b>id</b> - returns the retrieved object, or nil if there is an error
 <br>(NOTE: It is an error if zero, or more than one object, is found at this url)
 <br>on success, the retrieved object will have associated #FFMetaData
 */
- (id) getObjFromUri:(NSString *)ffUrl error:(NSError **)outErr;

/**
 @deprecated - please use #getObjFromUri:error:
 */
- (id) getObjFromUrl:(NSString *)ffUrl error:(NSError **)outErr;

/**
 Synchronous method that will return a single object from a single collection from the application's backend.
 <br>(For access to HTTP status codes, use #getObjFromUri:onComplete: )
 @param NSString ffUrl : the uri for the object relative to the #baseUrl property (which is set by FatFractal::initWithBaseUrl: )
 @return <b>id</b> - returns the retrieved object, or nil if there is an error
 <br>(NOTE: It is an error if zero, or more than one object, is found at this url)
 <br>on success, the retrieved object will have associated #FFMetaData
 */
- (id) getObjFromUri:(NSString *)ffUrl;
/**
 @deprecated - please use #getObjFromUri:
 */
- (id) getObjFromUrl:(NSString *)ffUrl;

/**
 Asynchronous method with onComplete callback that will update a single object in a single collection to the 
 application's backend.
 @param id obj : the object to be updated on your app's backend
 @param FFHttpMethodCompletion onComplete : the block defined here will execute when the HTTP call completes
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - the updated object, or nil if there is an error
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 */
- (void) updateObj:(id)obj onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Asynchronous method with onComplete callback and onOffline callback that will update a single object to a single 
 collection on the application's backend.
 @param id obj : the object to be updated on your app's backend
 @param FFHttpMethodCompletion onComplete: the block defined here will execute when the HTTP call completes 
 @param FFHttpMethodCompletion onOffline : the block defined here will execute if the application's backend is not reachable and the operation has been queued via
 #queueUpdateObj:
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion onComplete or onOffline block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - nil if there is an error; the passed object, if it's an onComplete block; or an #FFQueuedOperation, if it's an onOffline block
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 */
- (void) updateObj:(id)obj onComplete:(FFHttpMethodCompletion)onComplete onOffline:(FFHttpMethodCompletion)onOffline;

/**
 Asynchronous method with onComplete callback that will update a binary data object (blob) to the application's backend.
 @param NSData data : the binary data to be saved on your app's backend
 @param NSString mimeType : the mime type of the data - for example, image/jpeg
 @param id obj : the object to which this blob belongs
 @param NSString memberName : the 'field name' - eg imageData
 @param FFHttpMethodCompletion onComplete: the block defined here will execute when the HTTP call completes
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - the updated object, or nil if there is an error
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 */
- (void) updateBlob:(NSData *)data
       withMimeType:(NSString *)mimeType
             forObj:(id)obj
         memberName:(NSString *)memberName
         onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Asynchronous method with onComplete callback and onOffline callback that will update a binary data object (blob) to the application's backend.
 @param NSData data : the binary data to be saved on your app's backend
 @param NSString mimeType : the mime type of the data - for example, image/jpeg
 @param id obj : the object to which this blob belongs
 @param NSString memberName : the 'field name' - eg imageData
 @param FFHttpMethodCompletion onComplete : the block defined here will execute when the HTTP call completes 
 @param FFHttpMethodCompletion onOffline : the block defined here will execute if the application's backend is not reachable and the operation has been queued via
 #queueUpdateBlob:withMimeType:forObj:memberName:
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion onComplete or onOffline block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - nil if there is an error; the passed object, if it's an onComplete block; or an #FFQueuedOperation, if it's an onOffline block
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 @see FFHttpDelegate::onOffline
 */
- (void) updateBlob:(NSData *)data
       withMimeType:(NSString *)mimeType
             forObj:(id)obj
         memberName:(NSString *)memberName
         onComplete:(FFHttpMethodCompletion)onComplete
          onOffline:(FFHttpMethodCompletion)onOffline;

/**
 Asynchronous method with onComplete callback that will delete a binary data object (blob) from the application's backend
 @param id obj : the object to which this blob belongs
 @param NSString memberName : the 'field name' - eg imageData
 @param FFHttpMethodCompletion onComplete: the block defined here will execute when the HTTP call completes
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - the updated object, or nil if there is an error
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 */
- (void) deleteBlobForObj:(id)obj
               memberName:(NSString *)memberName
               onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Asynchronous method with onComplete callback that will delete a binary data object (blob) from the application's backend
 @param id obj : the object to which this blob belongs
 @param NSString memberName : the 'field name' - eg imageData
 @param FFHttpMethodCompletion onComplete : the block defined here will execute when the HTTP call completes
 @param FFHttpMethodCompletion onOffline : the block defined here will execute if the application's backend is not reachable and the operation has been queued via
 #queueDeleteBlobForObj:memberName:
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion onComplete or onOffline block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - nil if there is an error; the passed object, if it's an onComplete block; or an #FFQueuedOperation, if it's an onOffline block
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 @see FFHttpDelegate::onOffline
 */
- (void) deleteBlobForObj:(id)obj
         memberName:(NSString *)memberName
         onComplete:(FFHttpMethodCompletion)onComplete
          onOffline:(FFHttpMethodCompletion)onOffline;


/**
 Synchronous method that will update a binary data object (blob) to the application's backend.
 <br>(For access to HTTP status codes, use #updateObj:onComplete: )
 @param id obj : the object to be updated on your app's backend
 @return id - returns the updated object, or nil if there is an error
 */
- (id) updateObj:(id)obj;

/**
 Synchronous method with output parameter, error
 <br>(For access to HTTP status codes, use #updateObj:onComplete: )
 @param id obj : the object to be updated on your app's backend
 @param NSError outErr : non-nil if there is an error
 @return id - returns the updated object, or nil if there is an error
 */
- (id) updateObj:(id)obj error:(NSError **)outErr;

/**
 Asynchronous method with onComplete callback that will delete an object from the application's backend.
 @param id obj : the object to be deleted from your app's backend
 @param FFHttpMethodCompletion onComplete : the block defined here will execute when the HTTP call completes
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - the deleted object, or nil if there is an error
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 */
- (void) deleteObj:(id)obj onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Asynchronous method with onComplete callback and onOffline callback that will delete an object from 
 the application's backend.
 @param id obj : the object to be deleted from your app's backend
 @param FFHttpMethodCompletion onComplete: the block defined here will execute when the HTTP call completes
 @param FFHttpMethodCompletion onOffline : the block defined here will execute if the application's backend is not reachable and the operation has been queued via
 #queueDeleteObj:
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion onComplete or onOffline block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - nil if there is an error; the passed object, if it's an onComplete block; or an #FFQueuedOperation, if it's an onOffline block
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 @see FFHttpDelegate::onOffline
 */
- (void) deleteObj:(id)obj onComplete:(FFHttpMethodCompletion)onComplete onOffline:(FFHttpMethodCompletion)onOffline;

/**
 Synchronous method that will delete an object from the application's backend.
 <br>(For access to HTTP status codes, use #deleteObj:onComplete: )
 @param id obj : the object deleted from your app's backend
 @return id - returns the deleted object, or nil if there is an error
 */
- (id) deleteObj:(id)obj;

/**
 Synchronous method with output parameter, error that will delete an object from the application's backend.
 <br>(For access to HTTP status codes, use #deleteObj:onComplete: )
 @param id obj : the object to be deleted from your app's backend
 @param NSError outErr : non-nil if there is an error
 @return id - returns the deleted object, or nil if there is an error
 */
- (id) deleteObj:(id)obj error:(NSError **)outErr;

#pragma mark object metadata

/**
 Get the metadata for an object.
 Metadata is only created for an object during the current session. Therefore,
 the method will only return metadata for objects that you have retrieved or created
 during the current session.
 @param id obj : the object for which FFMetaData is required
 @return FFMetaData - the FFMetaData for the object
 */
- (FFMetaData *) metaDataForObj:(id)obj;

/**
 Have this FatFractal object forget about this object. Subsequent calls to #metaDataForObj: will return nil.
 */
- (void) forgetObj:(id)obj;

#pragma mark Support for "grab bags"

/**
 Synchronous method with output parameter, error that will add an object(item) to a named grabbag of another object (obj).
 <br><br>For example, 
 @code [ff grabBagAdd:anOrderLine to:anOrder grabBagName:@"OrderLines" error:&err];
 @endcode
 @param id item : the object to be added to the grabbag
 @param id obj : the object which has the named grabbag to be added to
 @param NSString name : the name of the grabbag to be added to
 @param NSError outErr : error object set upon an error, else nil
 @return id - returns the deleted object, or nil if there is an error
 */
- (id) grabBagAdd:(id)item
               to:(id)obj
      grabBagName:(NSString *)name
            error:(NSError **)outErr;

/**
 Synchronous method with output parameter, error that will remove an object(item) from a named grabbag of another object (obj).
 <br><br>For example, 
 @code [ff grabBagRemove:anOrderLine from:anOrder grabBagName:@"OrderLines" error:&err];
 @endcode
 @param id item : the object to be removed from the grabbag
 @param id obj : the object which has the named grabbag to be removed from
 @param NSString name : the name of the grabbag to be removed from
 @param NSError outErr : error object set upon an error, else nil
 @return id - returns the removed object, or nil if there is an error
 */
- (id) grabBagRemove:(id)item
                from:(id)obj
         grabBagName:(NSString *)name
               error:(NSError **)outErr;

/**
 Synchronous method with output parameter, error that will get an Array of all objects from a named grabbag of another object (obj).
 <br><br>For example, 
 @code NSArray* orderLines = [ff grabBagGetAllForObj:anOrder grabBagName:@"OrderLines" error:&err];
 @endcode
 @param id obj : the object with the grabbag to be retrieved from
 @param NSString name : the name of the grabbag to be retrieved from
 @param NSError outErr : error object set upon an error, else nil
 @return NSArray - returns an Array of objects from the grabbag
 */
- (NSArray *) grabBagGetAllForObj:(id)obj
                      grabBagName:(NSString *)name
                            error:(NSError **)outErr;

/**
 Asynchronous method with onComplete callback that will get an Array of all objects from a named grabbag of another object (obj).
 <br><br>For example,
 @code 
 [ff grabBagGetAllForObj:anOrder grabBagName:@"OrderLines" 
    onComplete:^(NSError *err, id orderLines, NSHTTPURLResponse *httpResponse) {
    ...
 }];
 @endcode
 @param id obj : the object with the grabbag to be retrieved from
 @param NSString name : the name of the grabbag to be retrieved from
 @param FFHttpMethodCompletion onComplete : the block defined here will execute when the HTTP call completes
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - the deleted object, or nil if there is an error
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 */
- (void) grabBagGetAllForObj:(id)obj
                 grabBagName:(NSString *)name
                  onComplete:(FFHttpMethodCompletion)onComplete;

/**
 Synchronous method with output parameter, error that will get an Array of objects (item) that matches the query from a named grabbag of another object.
 <br><br>For example, 
 @code NSArray* orderLines = [ff grabBagGetAllForObj:anOrder grabBagName:@"OrderLines" withQuery:@"(orderLineValue gt 1000)" error:&err];
 @endcode
 @param id obj : the object with the grabbag to be retrieved from
 @param NSString name : the name of the grabbag to be retrieved from
 @param NSError outErr : error object set upon an error, else nil
 @return NSArray - returns an Array of objects from the grabbag
 */
- (NSArray *) grabBagGetAllForObj:(id)obj
                      grabBagName:(NSString *)name
                        withQuery:(NSString *)query
                            error:(NSError **)outErr;

/**
 Asynchronous method with onComplete callback that will get an Array of objects (item) that matches the query from a named grabbag of another object.
 <br>Get items from the object's named grab bag which also match the query.
 <br>For example,
 @code 
 [ff grabBagGetAllForObj:anOrder grabBagName:@"OrderLines" withQuery:@"(orderLineValue gt 1000)" 
    onComplete:^(NSError *err, id orderLines, NSHTTPURLResponse *httpResponse) {
    ...
 }];
 @endcode
 @param id obj : the object with the grabbag to be retrieved from
 @param NSString name : the name of the grabbag to be retrieved from
 @param NSString query : the query string to be applied to the items in the grabbag
 @param FFHttpMethodCompletion onComplete : the block defined here will execute when the HTTP call completes
 @return <b>void</b> does not return anything directly - response is via the FFHttpMethodCompletion block which has these parameters:
 <br>   <b>(NSError *)</b> - non-nil if there is an error
 <br>   <b>(id)</b> - the deleted object, or nil if there is an error
 <br>   <b>(NSHTTPURLResponse *)</b> - the full NSHTTPURLResponse
 @see FFHttpDelegate::onComplete
 */
- (void) grabBagGetAllForObj:(id)obj
                 grabBagName:(NSString *)name
                   withQuery:(NSString *)query
                  onComplete:(FFHttpMethodCompletion)onComplete;


#pragma mark Support for offline modality

/**
 This method will return every operation which is currently queued for delivery to your app's backend.
 @return NSArray - an array of #FFQueuedOperation objects.
 */
- (NSArray *) getQueuedOperations;

/**
 This method will de-queue a previously-queued FFQueueOperation.
 @param FFQueuedOperation the operation to be de-queued.
 */
- (void) dequeue:(FFQueuedOperation *)queuedOperation;

/**
 This method will create an object on the application's backend when the network is next available.
 @param id obj : the object to be created and persisted on your app's backend
 @param NSString ffUrl : the uri for the object relative to the #baseUrl property (which is set by FatFractal::initWithBaseUrl: )
 @see #createObj:atUri:onComplete:
 @see #setFFQueueDelegate:
 @return <b>FFQueuedOperation</b>
 */
- (FFQueuedOperation *) queueCreateObj:(id)obj atUri:(NSString *)ffUrl;

/**
 This method will post an object to this extension when the network is next available.
 @param id obj : the object to be seriaized and POSTed to your server exension
 @param NSString extensionUri : the name of the server extension
 @see #postObj:toExtension:onComplete:
 @see #setFFQueueDelegate:
 @return <b>FFQueuedOperation</b>
 */
- (FFQueuedOperation *) queuePostObj:(id)obj toExtension:(NSString *)extensionUri;

/**
 This method will update an object on the application's backend when the network is next available.
 @param id obj : the object to be updated on your app's backend
 @see #updateObj:onComplete:
 @see #setFFQueueDelegate:
 @return <b>FFQueuedOperation</b>
 */
- (FFQueuedOperation *) queueUpdateObj:(id)obj;

/**
 This method will update your blob data on the application's backend when the network is next available.
 @param NSData the binary data to be saved on your app's backend
 @param NSString the mime type of the data - for example, image/jpeg
 @param id obj : the object to which this blob belongs
 @param NSString the 'field name' - eg imageData
 @see #updateBlob:withMimeType:forObj:memberName:onComplete:
 @see #setFFQueueDelegate:
 @return <b>FFQueuedOperation</b>
 */

- (FFQueuedOperation *) queueUpdateBlob:(NSData *)data
            withMimeType:(NSString *)mimeType
                  forObj:(id)obj
              memberName:(NSString *)memberName;

/**
 This method will delete your blob data from the application's backend when the network is next available.
 @param id obj : the object to which this blob belongs
 @param NSString the 'field name' - eg imageData
 @see #deleteBlobForObj:memberName:onComplete:
 @see #setFFQueueDelegate:
 @return <b>FFQueuedOperation</b>
 */

- (FFQueuedOperation *) queueDeleteBlobForObj:(id)obj
                             memberName:(NSString *)memberName;

/**
 This method will delete an object from the application's backend when the network is next available.
 @param id obj : the object deleted from your app's backend
 @see #deleteObj:onComplete:
 @see #setFFQueueDelegate:
 @return <b>FFQueuedOperation</b>
 */
- (FFQueuedOperation *) queueDeleteObj:(id)obj;

/**
 This method simulates a restart of the app, at least insofar as queued operations are concerned.
 */
- (void) simulateRestart;
- (void) setMetaData:(FFMetaData *)md forObj:(id)obj;

@end
