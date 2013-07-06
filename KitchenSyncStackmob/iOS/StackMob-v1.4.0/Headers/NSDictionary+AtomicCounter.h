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
 Category on `NSDictionary` for atomic counter functionality.
 */
@interface NSDictionary (AtomicCounter)

/**
 Returns a new dictionary with a new entry representing an atomic counter update for a field.
 
 Updating an atomic counter means incrementing or decrementing a value atomically such that you don't need to worry about concurrency issues. This might power a thumbs-up button counter or the score in a game. The dictionary you get from this method can then be passed into <SMDataStore> methods.
 
 @param field The field to increment.
 @param value The amount to increment by. Can be positive or negative.
 @since Available in iOS SDK 1.0.0 and later.
 */
- (NSDictionary *)dictionaryByAppendingCounterUpdateForField:(NSString *)field by:(int)value;
@end

/**
 Category on `NSMutableDictionary` for atomic counter functionality.
 */
@interface NSMutableDictionary (AtomicCounter)

/**
 Set a new entry in the dictionary representing an atomic counter update for a field.
 
 Updating an atomic counter means incrementing or decrementing a value atomically such that you don't need to worry about concurrency issues. This might power a thumbs-up button counter or the score in a game. After you call this method, the dictionary can be passed into <SMDataStore> methods.
 
 @param field The field to increment.
 @param value The amount to increment by. Can be positive or negative.
 @since Available in iOS SDK 1.0.0 and later.
 */
- (void)updateCounterForField:(NSString *)field by:(int)value;
@end

