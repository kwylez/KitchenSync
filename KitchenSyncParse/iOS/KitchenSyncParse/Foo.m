//
//  Foo.m
//  KitchenSyncParse
//
//

#import "Foo.h"
#import <Parse/PFObject+Subclass.h>

@implementation Foo

@synthesize fooParameter;

+ (NSString *)parseClassName {
    return @"Foo";
}

- (NSString*) description {
    return [[NSString alloc]
            initWithFormat:@"Foo[fooParameter[%@], parseClassName[%@]]",
            [self fooParameter],
            [self parseClassName]
            ];
}

@end
