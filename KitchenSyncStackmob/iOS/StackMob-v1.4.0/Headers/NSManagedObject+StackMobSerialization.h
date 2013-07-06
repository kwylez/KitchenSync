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

#import <CoreData/CoreData.h>

/**
 The methods in this category serve as the bread and butter for translating instances of `NSManagedObject` into StackMob equivalent objects.
 */
@interface NSManagedObject (StackMobSerialization)

/**
 Returns the StackMob equivalent schema for the entity name.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (NSString *)SMSchema;

/**
 Returns the unique StackMob ID for this object using the designated primary key attribute for the `NSManagedObject` instance entity.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (NSString *)SMObjectId;

/**
 Assigns a unique ID to the `NSManagedObject` instance.
 
 The id is placed in the object's primary key attribute, retrieved from <primaryKeyField>. It is used as the reference when assigning a permanent ID to the Core Data `NSManagedObject` so that Core Data and StackMob are referencing the same key.
 
 @note When creating an `NSManagedObject`, you must call this method and set it's return string as the value for the primary key field.  A call to `save:` on the managed object context will fail if any newly inserted object has a nil value for its primary key field.  To avoid this, when you are done setting other values simply add the line (assuming your new object is called newManagedObject):
 
    [newManagedObject setValue:[newManagedObject assignObjectId] forKey:[newManagedObject primaryKeyField]];
 
 @since Available in iOS SDK 1.1.1 and later.
 
 */
- (NSString *)assignObjectId;

/**
 Converts the value returned from <primaryKeyField> to its StackMob equivalent field.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (NSString *)SMPrimaryKeyField;

/**
 Returns the primary key field name for this entity.
 
 lowercaseEntityNameId or lowercaseEntityName_id is returned, if found as an attribute.
 
 @note If lowercaseEntityNameId or lowercaseEntityName_id (i.e. personId or person_id for entity Person) is not one of the entity's attributes, a `SMExceptionIncompatibleObject` exception is thrown.
 
 @since Available in iOS SDK 1.1.1 and later.
 */
- (NSString *)primaryKeyField;

/**
 Converts an `NSManagedObject` into an equivalent dictionary form for StackMob to process.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (NSDictionary *)SMDictionarySerialization;

/**
 Use to retrieve the value of a relationship.  
 
 If an error is provided, it will come back non-nil if the relationship fault could not be filled. The only time this would happen is if the value of the relationship is not in memory and the network is not reachable.  The error code in this case will be -107 (SMErrorCouldNotFillRelationshipFault).
 
 Example usage:
 
        // Assume obj is the parent managed object
        NSError *error = nil;
        NSManagedObject *relatedObject = [obj valueForRelationshipKey:@"toOne" error:&error];
        if (error) {
            // handle error
            // relatedObject will equal nil but [obj hasFaultForRelationshipNamed:@"toOne"] will return NO
        }

        error = nil;
        NSSet *relatedObjects = [obj valueForRelationshipKey:@"toMany" error:&error];
        if (error) {
            // handle error
            // relatedObjects will remain a fault (instance of _NSFaultingMutableSet), [obj hasFaultForRelationshipNamed:@"toMany"] will return YES
        }
 
 @param key The relationship name.
 @param error The error address.
 
 @return The value for the relationship. If an error occurs this method will return nil for a to-one relationship and a to-many relationship will stay faulted.
 
 @since Available in iOS SDK 1.0.0 and later.
 */
- (id)valueForRelationshipKey:(NSString *)key error:(NSError *__autoreleasing*)error;

@end
