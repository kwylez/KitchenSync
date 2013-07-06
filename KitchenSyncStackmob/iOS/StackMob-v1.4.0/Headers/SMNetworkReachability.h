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

#import <Foundation/Foundation.h>
#import "AFHTTPClient.h"
#import "SMDataStore.h"
#import "SMCoreDataStore.h"

extern NSString * SMNetworkStatusDidChangeNotification;
extern NSString * SMCurrentNetworkStatusKey;

typedef enum {
    Unknown = -1,
    NotReachable  = 0,
    Reachable = 1,
} SMNetworkStatus;

/**
 `SMNetworkReachability` provides an interface to monitor the network reachability from the device to StackMob.
 
 Network reachability checks are already built into the SDK.  When the network is not reachable and a request is made, an error with domain SMError and status code -105 (SMNetworkNotReachable) will be returned.
 
 An instance of `SMNetworkReachability` is created during the initialization of an `SMUserSession`, accessible by the `networkMonitor` property.
 
 Thus, assuming you have an instance of `SMClient` called `client`, you can access the `SMNetworkReachability` instance with `client.session.networkMonitor`.
 
 ## Checking the Current Network Status ##
 
 To manually check the current network status, use the <currentNetworkStatus> method.
 
 This method will return an SMNetworkStatus, defined as:
 
        typedef enum {
            Unknown = -1,
            NotReachable  = 0,
            Reachable = 1,
        } SMNetworkStatus;
 
 
 * Reachable - the device has a network connection and can successfully reach StackMob.
 * Not Reachable - StackMob is not reachable either because there is no network connection on the device or the service is down.
 * Unknown - Typically this status arises during in-between times of network connection initialization.
 
 An example of testing reachability before sending a request would look like this:
 
        if ([client.session.networkMonitor currentNetworkStatus] == Reachable) {
            // send request
        }
 
 You can also handle each state case by case in a switch statement:
 
        switch([client.session.networkMonitor currentNetworkStatus]) {
            case  Reachable:
                // do Reachable stuff
                break;
            case NotReachable:
                // do NotReachable stuff
                break;
            case Unknown:
                // do Unknown stuff
                break;
            default:
                break;
        }
 
 
 ## Registering For Network Status Change Notifications ##
 
 You can register to receive notifications when the network status changes by simply adding an observer for the notification name `SMNetworkStatusDidChangeNotification`.  The notification will have a `userInfo` dictionary containing one entry with key `SMCurrentNetworkStatusKey` and `NSNumber` representing the `SMNetworkStatus` value.
 
 In order to access the value of `SMCurrentNetworkStatusKey` in a format for comparing to specific states or use in a switch statement, retrieve the intValue like this:
 
        if ([[[notification userInfo] objectForKey:SMCurrentNetworkStatusKey] intValue] == Reachable) {
            // do Reachable stuff
        }
 
 **Important:** Remember to remove your notification observer before the application terminates.
 
 
 ## Executing a Block Whenever the Network Status Changes ## 
 
 You also have the option of setting a block that will be executed every time the network status changes.  To do this, use the <setNetworkStatusChangeBlock:> method like this:
 
        [client.session.networkMonitor setNetworkStatusChangeBlock:^(SMNetworkStatus status){
            
            // maybe log some stuff
            // maybe notify some objects
 
        }];
 
 */
@interface SMNetworkReachability : AFHTTPClient

/**
 Initializes an instance of `SMNetworkReachability` which can be used to monitor the network reachability from the device to StackMob.
 
 @return A new instance of `SMNetworkReachability`.
 
 @since Available in iOS SDK 1.1.3 and later.
 */
- (id)init;

/**
 The current status of the device's network connection and reachability to StackMob.
 
 @return Reachable if the device has a network connection and can successfully reach StackMob, NotReachable if StackMob is not reachable either because there is no network connection on the device or the service is down, Unknown during in-between times of network connection initialization.
 
 @since Available in iOS SDK 1.1.3 and later.
 */
- (SMNetworkStatus)currentNetworkStatus;

/**
 Provide a block to execute whenever there is a change in network reachability. 
 @param block The block to execute when the network status changes.
 
 @since Available in iOS SDK 1.1.3 and later.
 */
- (void)setNetworkStatusChangeBlock:(void (^)(SMNetworkStatus status))block;

/**
 Provide a block to execute whenever there is a change in network reachability.
 
 The block must return an SMCachePolicy.  This method is useful when you want to set the cache policy based on the network status.
 
 @param block The block to execute when the network status changes.
 
 @since Available in iOS SDK 1.1.3 and later.
 */
- (void)setNetworkStatusChangeBlockWithCachePolicyReturn:(SMCachePolicy (^)(SMNetworkStatus status))block;

@end
