//
//  Bar.m
//  KitchenSyncKinvey
//
//

#import "Bar.h"
#import <KinveyKit/KinveyKit.h>


@implementation Bar

@dynamic barParameter;
@dynamic kinveyObjectId;

- (NSDictionary *)hostToKinveyPropertyMapping {
    return @{
             @"barParameter" : @"barParameter",
             @"kinveyObjectId" : KCSEntityKeyId, //the required _id field
             @"metadata" : KCSEntityKeyMetadata //optional _metadata field
             };
}

+ (id)kinveyDesignatedInitializer:(NSManagedObjectContext *) context {
    Bar* b = [[self alloc]
              initWithEntity:[NSEntityDescription
                              entityForName:@"Bar"
                              inManagedObjectContext:context]
              insertIntoManagedObjectContext:context];
    return b;
}

@end
