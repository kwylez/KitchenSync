//
//  KitchenSyncParseTests.h
//  KitchenSyncParseTests
//
//

#import <SenTestingKit/SenTestingKit.h>
#import <Parse/Parse.h>


@interface KitchenSyncParseTests : SenTestCase

@property (nonatomic, strong) Parse *parse_open;
@property (nonatomic, strong) Parse *parse_secure;
@property (nonatomic, strong) Parse *parse_local;

@end
