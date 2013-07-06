//
//  FFHttpDelegate.h
//  FatFractal
//
//  Copyright (c) 2012 FatFractal, Inc. All rights reserved.
//
#import <Foundation/Foundation.h>

@class FFQueuedOperation;

typedef void (^FFHttpMethodCompletion) /*!< Type definition of the block which is a parameter to all asynchronous FatFractal methods */
(
    NSError * theErr,              /*!< passed with a non-nil value if there is an error */
    id theObj,                     /*!< the object which was retrieved / created / updated / deleted, or nil if there is an error, or an FFQueuedOperation if this was an 'onOffline' block parameter */
    NSHTTPURLResponse *theResponse /*!< the full NSHTTPURLResponse object (with HTTP response status code etc) */
 );

/*! \brief Implements the NSURLConnectionDelegate protocol for asynchronous methods.
 */
/*! 
 This class implements NSURLConnectionDelegate to support asynchronous methods.
 */
@interface FFHttpDelegate : NSObject <NSURLConnectionDelegate>

/*!
 The onComplete property is passed as a parameter to all asynchronous #FatFractal methods
 @param (NSError*) - non-nil if there is an error
 @param (id) - the object which was created / retrieved / updated / deleted, or nil if there is an error
 @param (NSHTTPURLResponse*) - the full NSHTTPURLResponse
 */
@property (strong, nonatomic)   FFHttpMethodCompletion  onComplete;
/*!
 The onOffline property is passed as a parameter to all asynchronous #FatFractal methods
 @param (NSError*) - non-nil if there is an error
 @param (id) - the object which was created / retrieved / updated / deleted, or nil if there is an error
 @param (NSHTTPURLResponse*) - the full NSHTTPURLResponse
 */
@property (strong, nonatomic)   FFHttpMethodCompletion  onOffline;
/*!
 The NSError, if there is one, which is passed via #onComplete to your code.
 */
@property (strong, nonatomic)   NSError                 *error;
/*!
 The raw data of the HTTP response body.
 */
@property (strong, nonatomic)   NSMutableData           *responseData;
/*!
 The HTTP response object.
 */
@property (strong, nonatomic)   NSHTTPURLResponse       *httpResponse;

- (id) initWithOnComplete:(FFHttpMethodCompletion)onComplete;

/*!
 Need to call this method if you are using self-issued SSL certs.
 By default, localhost is a trusted host.
 */
+ (void) addTrustedHost:(NSString *)host;

@end
