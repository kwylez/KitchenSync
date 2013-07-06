//
//  Foo.h
//  KitchenSyncParse
//
//

#import <Foundation/Foundation.h>
#import <Parse/Parse.h>

@interface Foo : PFObject<PFSubclassing>

+ (NSString *)parseClassName;

@property (nonatomic, retain) NSString * fooParameter;

@end
