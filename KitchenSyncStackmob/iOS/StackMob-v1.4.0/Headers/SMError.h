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
 `SMError` is a class which translates numeric error codes into readable errors, so you know what went wrong.
 */

extern NSString *const SMErrorDomain;
extern NSString *const HTTPErrorDomain;
extern NSString *const SMExceptionIncompatibleObject;
extern NSString *const SMExceptionUnknownSchema;
extern NSString *const SMExceptionAddPersistentStore;
extern NSString *const SMExceptionCannotFillRelationshipFault;
extern NSString *const SMExceptionCacheError;
extern NSString *const SMOriginalErrorCausingRefreshKey;
extern NSString *const SMRefreshErrorObjectKey;

typedef enum {
    //Special StackMob things
	SMErrorInvalidArguments = -100,
    SMErrorTemporaryPasswordResetRequired = -101,
    SMErrorNoCountAvailable = -102,
    SMErrorRefreshTokenInProgress = -103,
    SMErrorPasswordForUserObjectNotFound = -104,
    SMErrorNetworkNotReachable = -105,
    SMErrorCacheIDNotFound = -106,
    SMErrorCouldNotFillRelationshipFault = -107,
    SMErrorCoreDataSave = -108,
    SMErrorRefreshTokenFailed = -109,
    SMErrorLocationManagerFailed = -110,
    //Success messages. These shouldn't normally be encountered
    SMErrorOK = 200,
    SMErrorCreated = 201,
    SMErrorPartialContent = 206,
    SMErrorMovedPermanently = 301,
    SMErrorFound = 302,
    //HTTP errors
    SMErrorBadRequest = 400,
    SMErrorUnauthorized = 401,
    SMErrorForbidden = 403,
    SMErrorNotFound = 404,
    SMErrorTimeout = 408,
    SMErrorConflict = 409,
    SMErrorTeapot = 418,
    SMErrorChillOut = 420,
    SMErrorCensored = 451,
    SMErrorInternalServerError = 500,
    SMErrorNotImplemented = 501,
    SMErrorBadGateway = 502,
    SMErrorServiceUnavailable = 503,
    SMErrorGatewayTimeout = 504,
    
} SMError;
