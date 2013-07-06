//
//  KitchenSyncKinveyTests.h
//  KitchenSyncKinveyTests
//
//

#import <SenTestingKit/SenTestingKit.h>
#import <KinveyKit/KinveyKit.h>

@interface KitchenSyncKinveyTests : SenTestCase

@property (nonatomic, strong) NSManagedObjectModel *managedObjectModel;
@property (nonatomic, strong) NSPersistentStoreCoordinator *persistentStoreCoordinator;
@property (nonatomic, strong) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, strong) KCSAppdataStore *fooStore;
@property (nonatomic, strong) KCSAppdataStore *barStore;

@end
