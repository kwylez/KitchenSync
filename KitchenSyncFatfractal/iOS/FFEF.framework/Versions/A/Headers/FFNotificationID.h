//
//  FFNotificationID.h
//  FF-IOS-Framework
//
//  Copyright (c) 2012 FatFractal, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

/** \brief The model class for working with push notification credentials and the FatFractal Platform. */
/*! The FFNotificationID object model includes parameters regarding cross-platform push notifications 
 * with the FatFractal Platform.
 */
@interface FFNotificationID : NSObject

/*! An NSString with the type of notification id (will be either "IOS" or "Android".*/
@property (strong, nonatomic) NSString * idType;
/*! An NSString with the value of the notification id as returned from the respective notification service.*/
@property (strong, nonatomic) NSString * idValue;

/*! Initializer that accepts a notificationId and sets the type to "IOS".
 * @param NSString - the notificationId to be added
 */
- initWithIdValue:(NSString *)value;

@end
