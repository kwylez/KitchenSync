//
//  FFUserGroup.h
//  FatFractal
//
//  Copyright (c) 2012 FatFractal, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class FFUser;
@class FatFractal;

/*! \brief A special kind of FatFractal object for managing user groups in your application. */
/*! 
 This is the special class for managing user groups in the FatFractal Emergent Framework.
 */ 
@interface FFUserGroup : NSObject <NSCoding> {
    BOOL                 _usersLoaded;
    NSMutableDictionary *_usersDict;
    FatFractal          *ff;
}

/*!
 Standard initializer. No-args initializer calls this initializer with [FatFractal main] as parameter.
 */
- (id)initWithFF:(FatFractal *)_ff;

/*!
 The name of this group.
 @see FFUser::groupWithName:
 */
@property (strong, nonatomic) NSString *groupName;

/*!
 Add a user to this group's list of users
 @param FFUser - the user to be added
 @param NSError - will be set to non-nil if an error occurs
 */
- (void) addUser:(FFUser *)user error:(NSError **)outErr;

/*!
 Remove a user from this group's list of users
 @param FFUser - the user to be removed
 @param NSError - will be set to non-nil if an error occurs
 */
- (void) removeUser:(FFUser *)user error:(NSError **)outErr;

/*!
 Find a specific user in this group's list of users
 @param NSString - the user name
 @param NSError - will be set to non-nil if an error occurs
 @return FFUser - the user, if found, or nil otherwise
 */
- (FFUser *) userWithName:(NSString *)userName error:(NSError **)outErr;

/*!
 Get all of this group's users.
 @return NSArray - the array of users
 @param NSError - will be set to non-nil if an error occurs
 */
- (NSArray *) getUsersWithError:(NSError **)outErr;

/*!
 Set the FatFractal instance to be associated with this object.
 NOTE: This method is intended to be used after de-archiving via initWithCoder. As such, it will only have an effect when no FatFractal instance has been set.
 */
- (BOOL)setFF:(FatFractal *)ff;

@end
