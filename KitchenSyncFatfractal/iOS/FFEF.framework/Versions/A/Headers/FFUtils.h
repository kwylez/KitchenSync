//
//  FFUtils.h
//  FatFractal
//
//  Copyright (c) 2012 FatFractal, Inc. All rights reserved.
//
#import <Foundation/Foundation.h>

/*! \brief A set of utility methods used by the FatFractal Emergent Framework for iOS. */
/*! 
 This class provides a set of utility methods used by the FatFractal Emergent Framework for iOS
 */ 
@interface FFUtils : NSObject

/*!
 The set of properties for this Class (including its superclasses up to NSObject).
 The set of properties is cached.
 @param class the class
 @return NSDictionary a dictionary with the properties
 */
+ (NSDictionary *) propertiesForClass:(Class)class;

/*!
 Check equality by checking the properties of the objects for equality using key-value-coding.
 @param o1 object 1
 @param o2 object 2
 @param reason an out parameter which, if the objects are not equal, contains the reason why
 @return BOOL true if the properties are equal, false otherwise
 */
+ (BOOL) object:(id)o1 hasEqualValuesTo:(id)o2 notEqualReason:(NSString **)reason;

@end
