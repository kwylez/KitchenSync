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

#define TOKEN_TYPE_IOS @"ios"
#define TOKEN_TYPE_ANDROID_C2DM @"android"
#define TOKEN_TYPE_ANDROID_GCM @"androidgcm"

/**
 Represents the info needed to register a device for push with StackMob
 
 This corresponds to what's sored about each token on server: token string paired with a platform and some supplemental information. 
 
 @note In most cases you'll want to just use the device token string you get from apple directly rather than using one of these.
 
 */
@interface SMPushToken : NSObject

@property(nonatomic, copy) NSString *tokenString;
@property(nonatomic, copy) NSString *type;
@property(nonatomic, copy) NSDate *registrationTime;


/**
 Initialize with a device token string. Assumes the type is ios
 
 @param tokenString The device token string.
 */
-(id)initWithString:(NSString *)tokenString;

/**
 Initialize with a device token string and a type.
 
 @param tokenString The device token string.
 @param type The platform the token is for. Should be one of ios, android, or androidgcm
 */
-(id)initWithString:(NSString *)tokenString type:(NSString *)type;


@end
