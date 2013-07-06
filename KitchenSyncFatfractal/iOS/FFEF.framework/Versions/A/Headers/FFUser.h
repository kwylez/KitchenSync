//
//  FFUser.h
//  FatFractal
//
//  Copyright (c) 2012 FatFractal, Inc. All rights reserved.
//
#import <Foundation/Foundation.h>

@class FFUserGroup;
@class FatFractal;

/*! \brief A special kind of FatFractal object for managing users of your application. */
/*! 
 This is the special class for managing users in the FatFractal Emergent Framework.
 */ 
@interface FFUser : NSObject <NSCoding> {
    /*! The FatFractal instance which is managing this FFUser object */
    FatFractal          *ff;
    NSMutableDictionary *_groupsDict;
    BOOL                 _groupsLoaded;
}

/*! An NSString with the unique username for the user. */
@property (strong, nonatomic) NSString          *userName;
/*! An NSString with the first name of the user. */
@property (strong, nonatomic) NSString          *firstName;
/*! An NSString with the last name of the user. White spaces and special characters are allowed*/
@property (strong, nonatomic) NSString          *lastName;
/*! An NSString with the email of the user. This string is validated for proper formatting. */
@property (strong, nonatomic) NSString          *email;
/*! Boolean - is this user 'active' or not. Inactive users may be able to log in and read data, but will be unable to modify data. */
@property (nonatomic)         BOOL              active;

/*!
 Standard initializer. No-args initializer calls this initializer with [FatFractal main] as parameter.
 */
- (id)initWithFF:(FatFractal *)_ff;

/*!
 Add a group to this user's list of groups
 @param FFUserGroup - the user group to be added
 @param NSError - will be set to non-nil if an error occurs
 */
- (void) addGroup:(FFUserGroup *)group error:(NSError **)outErr;
/*!
 Remove a group from this user's list of groups
 @param FFUserGroup - the user group to be removed
 @param NSError - will be set to non-nil if an error occurs
 */
- (void) removeGroup:(FFUserGroup *)group error:(NSError **)outErr;
/*!
 Find a specific group in this user's list of groups
 @param NSString - the group's name
 @param NSError - will be set to non-nil if an error occurs
 @return FFUserGroup - the group, if found, or nil otherwise
 */
- (FFUserGroup *) groupWithName:(NSString *)groupName error:(NSError **)outErr;
/*!
 Get all of this user's groups.
 @param NSError - will be set to non-nil if an error occurs
 @return NSArray - the array of groups
 */
- (NSArray *) getGroupsWithError:(NSError **)outErr;
/*!
 Set the FatFractal instance to be associated with this object.
 NOTE: This method is intended to be used after de-archiving via initWithCoder. As such, it will only have an effect when no FatFractal instance has been set.
 */
- (BOOL)setFF:(FatFractal *)ff;

@end
