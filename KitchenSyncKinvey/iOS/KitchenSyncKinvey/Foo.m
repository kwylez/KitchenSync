//
//  Foo.m
//  KitchenSyncKinvey
//
//

#import "Foo.h"

@implementation Foo

- (NSDictionary *)hostToKinveyPropertyMapping {
    return @{
             @"fooParameter" : @"fooParameter",
             @"kinveyObjectId" : KCSEntityKeyId, //the required _id field
             @"metadata" : KCSEntityKeyMetadata //optional _metadata field
             };
}

@end
