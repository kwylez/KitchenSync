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

@class SMClient;

/**
 Inherit from SMUserManagedObject when your managed object subclass is defining user objects.
 
 This class provides a method <setPassword:> used to securely set a password for the user object, without directly setting any attributes in Core Data.  When a save call is made to Core Data, the password is persisted along with the object to StackMob.
 
 `SMUserManagedObject` references an instance of <SMClient> for returning the primary key field name. In your managed object subclass's init method you should be calling Core Data's designated initializer `initWithEntity:insertIntoManagedObjectContext:`. This will reference `[SMClient defaultClient]`. Optionally, if you wish to reference a specific client instance, use the <initWithEntity:client:insertIntoManagedObjectContext:> initializer instead.
 */
@interface SMUserManagedObject : NSManagedObject

///-------------------------------
/// @name Initialize
///-------------------------------

/**
 Initializes a new managed object with a reference to `[SMClient defaultClient]`.
 
 @param entity The description of this managed object's entity.
 @param context The managed object context to insert the object into.
 
 @return An newly instantiated managed object to define a user.
 
 @since Available in iOS SDK 1.1.0 and later.
 */
- (id)initWithEntity:(NSEntityDescription *)entity insertIntoManagedObjectContext:(NSManagedObjectContext *)context;

/**
 Initializes a new managed object with a reference to the provided `SMClient` instance.
 
 @param entity The description of this managed object's entity.
 @param client The `SMClient` instance to reference for the primary key field name, taken from client.userPrimaryKeyField.
 @param context The managed object context to insert the object into.
 
 @return An newly instantiated managed object to define a user.
 
 @since Available in iOS SDK 1.1.0 and later.
 */
- (id)initWithEntity:(NSEntityDescription *)entity client:(SMClient *)client insertIntoManagedObjectContext:(NSManagedObjectContext *)context;

///-------------------------------
/// @name Setting and Removing Password
///-------------------------------

/**
 Sets the password to associate with the user object.
 
 @param password The password associated with the user object to be used for authentication.
 
 @since Available in iOS SDK 1.1.0 and later.
 */
- (void)setPassword:(NSString *)password;

/**
 Removes the password.
 
 This method is called upon successful persistence of the user object on StackMob to delete the password any prevent further accessing.
 
 **Important:** Call this method if the creation of a user object fails and you delete or undo the original insert of the user object.
 
 @since Available in iOS SDK 1.1.0 and later.
 */
- (void)removePassword;

@end
