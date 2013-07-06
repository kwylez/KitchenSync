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

#import <Security/Security.h>
#import <CommonCrypto/CommonHMAC.h>

// KeychainWrapper methods inspired by Chris Lowe's tutorial on iOS Security.

@interface KeychainWrapper : NSObject

// Generic exposed method to search the keychain for a given value. Limit one result per search.
+ (NSData *)searchKeychainCopyMatchingIdentifier:(NSString *)identifier;

// Calls searchKeychainCopyMatchingIdentifier: and converts to a string value.
+ (NSString *)keychainStringFromMatchingIdentifier:(NSString *)identifier;

// Default initializer to store a value in the keychain.
// Associated properties are handled for you - setting Data Protection Access, Company Identifer (to uniquely identify string, etc).
+ (BOOL)createKeychainValue:(NSString *)value forIdentifier:(NSString *)identifier;

// Updates a value in the keychain. If you try to set the value with createKeychainValue: and it already exists,
// this method is called instead to update the value in place.
+ (BOOL)updateKeychainValue:(NSString *)value forIdentifier:(NSString *)identifier;

// Delete a value in the keychain.
+ (void)deleteItemFromKeychainWithIdentifier:(NSString *)identifier;

@end
