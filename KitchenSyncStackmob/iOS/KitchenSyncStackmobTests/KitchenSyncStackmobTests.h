//
//  KitchenSyncStackmobTests.h
//  KitchenSyncStackmobTests
//
//

#import <SenTestingKit/SenTestingKit.h>
#import "StackMob.h"

@class SMClient;

@interface KitchenSyncStackmobTests : SenTestCase

@property (nonatomic, strong) SMClient *sm_open;
@property (nonatomic, strong) SMClient *sm_secure;
@property (nonatomic, strong) SMClient *sm_local;
@property (nonatomic, strong) NSManagedObjectModel *managedObjectModel;
@property (nonatomic, strong) NSPersistentStoreCoordinator *persistentStoreCoordinator;
@property (nonatomic, strong) NSManagedObjectContext *sm_local_Context;
@property (nonatomic, strong) NSManagedObjectContext *sm_open_Context;
@property (nonatomic, strong) NSManagedObjectContext *sm_secure_Context;

@end
