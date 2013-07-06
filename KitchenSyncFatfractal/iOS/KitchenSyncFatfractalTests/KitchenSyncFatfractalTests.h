//
//  KitchenSyncFatfractalTests.h
//  KitchenSyncFatfractalTests
//
//

#import <SenTestingKit/SenTestingKit.h>
#import <FFEF/FatFractal.h>

/*!
 * The KitchenSinkTests.
 */
@interface KitchenSyncFatfractalTests : SenTestCase

@property(nonatomic, strong) FatFractal *ff_local;
@property(nonatomic, strong) FatFractal *ff_open;
@property(nonatomic, strong) FatFractal *ff_secure;
@property(nonatomic, strong) NSManagedObjectModel *managedObjectModel;
@property(nonatomic, strong) NSPersistentStoreCoordinator *persistentStoreCoordinator;
@property(nonatomic, strong) NSManagedObjectContext *managedObjectContext;

@end
