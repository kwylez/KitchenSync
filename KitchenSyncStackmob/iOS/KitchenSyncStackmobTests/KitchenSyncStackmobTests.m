//
//  KitchenSyncStackmobTests.m
//  KitchenSyncStackmobTests
//
//

#import "KitchenSyncStackmobTests.h"
#import "StackMobConfigurator.h"
#import "StackMob.h"
#import "Foo.h"
#import "Bar.h"

@interface KitchenSyncStackmobTests()

@property (nonatomic, assign) BOOL showLogs;
@property (nonatomic, assign) BOOL usingSecureBackend;
@property (nonatomic, copy) void (^testForCompletionBlock)(BOOL *testCompleted);

- (void)setupCoreDataEnvironment;
- (void)deleteAllOpenBar;
- (void)deleteAllSecureBar;
- (NSString *)randomString:(int)numCharacters;
- (void)configureSecureBackend;

@end

@implementation KitchenSyncStackmobTests

#pragma mark - Setup and Init

- (void)setUp {

    [super setUp];
  
    _testForCompletionBlock = ^(BOOL *testCompleted) {

      while (!*testCompleted) {
        
        NSDate *cycle = [NSDate dateWithTimeIntervalSinceNow:0.01];
        
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                                 beforeDate:cycle];
      }
    };
  
    NSDictionary *stackmobAppConfiguration = [[StackMobConfigurator sharedInstance] applicationConfiguration];
    
    _showLogs           = [stackmobAppConfiguration[@"ShowLogs"] boolValue];
    _usingSecureBackend = [stackmobAppConfiguration[@"UseSecure"] boolValue];

    [self setupCoreDataEnvironment];
    [self configureBackend];
}

- (void)setupCoreDataEnvironment {

  // Destroy objects
  _persistentStoreCoordinator = nil;
  _managedObjectModel         = nil;
  
  // Setup Core Data
  _managedObjectModel         = [NSManagedObjectModel mergedModelFromBundles:nil];
  _persistentStoreCoordinator = [[NSPersistentStoreCoordinator alloc] initWithManagedObjectModel:_managedObjectModel];
  
  NSError *error;
  
  // Setup an in memory store
  if(![_persistentStoreCoordinator addPersistentStoreWithType:NSInMemoryStoreType
                                                configuration:nil
                                                          URL:nil
                                                      options:nil
                                                        error:&error]) {
    STFail(@"Could not create store coordinator: %@", [error localizedDescription]);
  }
}

- (void)configureBackend {
  
    NSDictionary *stackmobAppConfiguration = [[StackMobConfigurator sharedInstance] applicationConfiguration];
  
    BOOL useProduction = [stackmobAppConfiguration[@"UseProduction"] boolValue];

    if(!_sm_open) {
      
      NSString *apiVersion = nil;
      NSString *publicKey  = nil;

      if (useProduction) {
        
        apiVersion = stackmobAppConfiguration[@"OpenProductionAPIVersion"];
        publicKey  = stackmobAppConfiguration[@"OpenProductionPublicKey"];

      } else {

        apiVersion = stackmobAppConfiguration[@"OpenDevelopmentAPIVersion"];
        publicKey  = stackmobAppConfiguration[@"OpenDevelopmentPublicKey"];
      }
        _sm_open = [[SMClient alloc] initWithAPIVersion:apiVersion publicKey:publicKey];

        SMCoreDataStore *coreDataStore = [_sm_open coreDataStoreWithManagedObjectModel:_managedObjectModel];

        _sm_open_Context = [coreDataStore contextForCurrentThread];
    }
    if (!_sm_secure) {
      
      NSString *apiVersion = nil;
      NSString *publicKey  = nil;
      
      if (useProduction) {

        apiVersion = stackmobAppConfiguration[@"SecureProductionAPIVersion"];
        publicKey  = stackmobAppConfiguration[@"SecureProductionPublicKey"];
        
      } else {

        apiVersion = stackmobAppConfiguration[@"SecureDevelopmentAPIVersion"];
        publicKey  = stackmobAppConfiguration[@"SecureDevelopmentPublicKey"];
      }
        _sm_secure = [[SMClient alloc] initWithAPIVersion:apiVersion publicKey:publicKey];

        SMCoreDataStore *coreDataStore = [_sm_secure coreDataStoreWithManagedObjectModel:_managedObjectModel];

      _sm_secure_Context = [coreDataStore contextForCurrentThread];
    }
    if (!_sm_local) {
        // Stackmob does not support local development.
    }
}

- (void)configureSecureBackend {

  NSString *apiVersion = nil;
  NSString *publicKey  = nil;
  
  NSDictionary *stackmobAppConfiguration = [[StackMobConfigurator sharedInstance] applicationConfiguration];
  
  BOOL useProduction = [stackmobAppConfiguration[@"UseProduction"] boolValue];

  if (!_sm_secure) {
    
    if (useProduction) {
      
      apiVersion = stackmobAppConfiguration[@"SecureProductionAPIVersion"];
      publicKey  = stackmobAppConfiguration[@"SecureProductionPublicKey"];
      
    } else {
      
      apiVersion = stackmobAppConfiguration[@"SecureDevelopmentAPIVersion"];
      publicKey  = stackmobAppConfiguration[@"SecureDevelopmentPublicKey"];
    }
    _sm_secure = [[SMClient alloc] initWithAPIVersion:apiVersion publicKey:publicKey];
    
    SMCoreDataStore *coreDataStore = [_sm_secure coreDataStoreWithManagedObjectModel:_managedObjectModel];

    _sm_secure_Context = [coreDataStore contextForCurrentThread];
  }  
}

#pragma mark - Teardown

- (void)tearDown {
    // Tear-down code here.
    [super tearDown];
}

- (void)deleteAllOpenBar {
    @try {
      
      __block BOOL testCompleted = NO;

        NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
        NSEntityDescription *entity = [NSEntityDescription entityForName:@"Bar" inManagedObjectContext:_sm_open_Context];
        [fetchRequest setEntity:entity];
        [_sm_open_Context executeFetchRequest:fetchRequest onSuccess:^(NSArray *results) {
            for (id obj in results) {
                [_sm_open_Context deleteObject:obj];
                [_sm_open_Context saveOnSuccess:^{
                } onFailure:^(NSError *error) {
                    STAssertNil(error, @"There was an error! %@", error);
                }];
            }
          testCompleted = YES;
        } onFailure:^(NSError *error) {
          testCompleted = YES;
          STAssertNil(error, @"Error fetching! %@", error);
        }];
      
      _testForCompletionBlock(&testCompleted);
    } @catch (NSException * e) {
        NSLog(@"deleteAllOpenBar failed: %@", [e description]);
    }
}

- (void)deleteAllSecureBar {
    @try {
      
      __block BOOL testCompleted = NO;
        NSFetchRequest *fetchRequest = [[NSFetchRequest alloc] init];
        NSEntityDescription *entity = [NSEntityDescription entityForName:@"Bar" inManagedObjectContext:_sm_secure_Context];
        [fetchRequest setEntity:entity];
        [_sm_secure_Context executeFetchRequest:fetchRequest onSuccess:^(NSArray *results) {
            for (id obj in results) {
                [_sm_secure_Context deleteObject:obj];
                [_sm_secure_Context saveOnSuccess:^{
                } onFailure:^(NSError *error) {
                    STAssertNil(error, @"There was an error! %@", error);
                }];
            }
          testCompleted = YES;
        } onFailure:^(NSError *error) {
          testCompleted = YES;
            STAssertNil(error, @"Error fetching! %@", error);
        }];
      
      _testForCompletionBlock(&testCompleted);
    } @catch (NSException * e) {
        NSLog(@"deleteAllSecureBar failed: %@", [e description]);
    }
}

#pragma mark - Setup and configuration tests

/*!
 This method deletes all Test data from the backend.
 */
- (void)test_000_Delete_All_Test_Data {
  @try {
    [self deleteAllOpenBar];
    [self deleteAllSecureBar];
  } @catch (NSException * e) {
    STAssertNil(e, @"test failed: %@", [e description]);
  }
}

/*!
 * Tests the ability to access data without authentication key if allowed.
 */
- (void)test_100_Public_Access {
    @try {
        Bar * b1 = [NSEntityDescription
                    insertNewObjectForEntityForName:@"Bar"
                    inManagedObjectContext:_sm_open_Context];
        [b1 setBarParameter:@"Testing public access"];
        [b1 setValue:[b1 assignObjectId] forKey:[b1 primaryKeyField]];
        if(_showLogs) NSLog(@"test_100_Public_Access Bar:b1 before save = %@",
                           [b1 description]);

        __block BOOL testCompleted = NO;
      
        [_sm_open_Context saveOnSuccess:^{
          testCompleted = YES;
          if(_showLogs) NSLog(@"You created a new object!");
        } onFailure:^(NSError *error) {
          testCompleted = YES;
          STAssertNil(error, @"test failed: %@", [error localizedDescription]);
        }];
      
        _testForCompletionBlock(&testCompleted);

        if(_showLogs) NSLog(@"test_100_Public_Access Bar:b1 after save = %@",
                           [b1 description]);
        // verify that the object can be accessed without authentication
        NSError *crudError;
        Bar *b2 = (Bar *)[_sm_open_Context existingObjectWithID:[b1 objectID] error:&crudError];
        STAssertNotNil(b2, @"test failed: %@", [b2 description]);
        STAssertNil(crudError, @"test failed: %@", [crudError localizedDescription]);
        if(_showLogs) NSLog(@"test_100_Public_Access Bar:b2 after read = %@",
                           [b2 description]);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to require secure access to access data.
 */
- (void)test_101_Secure_Access {
    @try {

        Bar * b1 = [NSEntityDescription
                    insertNewObjectForEntityForName:@"Bar"
                    inManagedObjectContext:_sm_secure_Context];
        [b1 setBarParameter:@"Testing secure access"];
        [b1 setValue:[b1 assignObjectId] forKey:[b1 primaryKeyField]];
        if(_showLogs) NSLog(@"test_101_Secure_Access Bar:b1 before save = %@",
                           [b1 description]);

        __block BOOL testCompleted = NO;
      
        [_sm_secure_Context saveOnSuccess:^{
          testCompleted = YES;
          if(_showLogs) NSLog(@"You created a new object!");
        } onFailure:^(NSError *error) {
          testCompleted = YES;
          STAssertNil(error, @"test failed: %@", [error localizedDescription]);
        }];
      
        _testForCompletionBlock(&testCompleted);
        // verify that the object can be accessed
        NSError *crudError;
        Bar *b2 = (Bar *)[_sm_secure_Context
                          existingObjectWithID:[b1 objectID]
                          error:&crudError];
        STAssertNotNil(b2, @"test failed: %@", [b2 description]);
        STAssertNil(crudError, @"test failed: %@", [crudError localizedDescription]);
        if(_showLogs) NSLog(@"test_101_Secure_Access localFoo : foo after save = %@",
                           [b2 description]);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability for a schemaless backend to learn a new Collection automatically.
 * Since we are not able to talk to multiple backends, we will use the newCollectionsAllowed
 * setting to run the test if true, and skip if false.
 */
- (void)test_102_New_Class_Allowed {
    STFail(@"Stackmob does not support collections.");
}

/*!
 * Tests that a backend with a locked schema will not allow adding a new Collection automatically.
 * Since we are not able to talk to multiple backends, we will use the newCollectionsAllowed
 * setting to run the test if false, and skip if true.
 */
- (void)test_103_New_Class_Disallowed {
    STFail(@"Stackmob does not support collections.");
}

/*!
 * Tests the ability for a schemaless backend to learn a new Class automatically.
 * Since we are not able to talk to multiple backends, we will use the newClassesAllowed
 * setting to run the test if true, and skip if false.
 */
- (void)test_104_New_Collection_Allowed {
    @try {
      
        NSString * clazz = [self randomString:5];
        NSManagedObject *b1 = [NSEntityDescription
                               insertNewObjectForEntityForName:clazz
                               inManagedObjectContext:_sm_open_Context];
        STAssertNotNil(b1, @"test_104_New_Class_Allowed random entity can't be nil");
        if (_showLogs) NSLog(@"b1: %@", b1);
        [b1 setValue:@"NSManagedObject for Entity named Bar" forKey:@"barParameter"];
        if(_showLogs) NSLog(@"test_104_New_Class_Allowed Bar : bar before save = %@",
                           [b1 description]);

      __block BOOL testCompleted = NO;
          // create the object
        [_sm_open_Context saveOnSuccess:^{
          if(_showLogs) NSLog(@"You created a new object via new runtime class!");
          testCompleted = YES;
        } onFailure:^(NSError *error) {
          testCompleted = YES;
          STAssertNil(error, @"test failed: %@", [error localizedDescription]);
        }];
      
        _testForCompletionBlock(&testCompleted);
        // verify that the object can be accessed
        NSError *crudError;
        Bar *b2 = (Bar *)[_sm_open_Context
                          existingObjectWithID:[b1 objectID]
                          error:&crudError];
        STAssertNotNil(b2, @"test failed: %@", [b2 description]);
        STAssertNil(crudError, @"test failed: %@", [crudError localizedDescription]);
        if(_showLogs) NSLog(@"test_104_New_Class_Allowed localFoo : foo after save = %@",
                           [b2 description]);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests that a backend with a locked schema will not allow adding a new Class automatically.
 * Since we are not able to talk to multiple backends, we will use the newClassesAllowed
 * setting to run the test if false, and skip if true.
 */
- (void)test_105_New_Collection_Disallowed {
    @try {
      
        __block BOOL testCompleted = NO;
      
        NSString * clazz = [self randomString:5];
        NSManagedObject *b1 = [NSEntityDescription
                               insertNewObjectForEntityForName:clazz
                               inManagedObjectContext:_sm_secure_Context];
        [b1 setValue:@"NSManagedObject for Entity named random" forKey:@"randomParameter"];
        [b1 setValue:[b1 assignObjectId] forKey:[b1 primaryKeyField]];
        if(_showLogs) NSLog(@"test_105_New_Class_Disallowed Bar : bar before save = %@",
                           [b1 description]);

        // create the object
        [_sm_secure_Context saveOnSuccess:^{
          testCompleted = YES;
          STFail(@"You created a new object via new runtime class and shouldn't have");
        } onFailure:^(NSError *error) {
          testCompleted = YES;
          STAssertNotNil(error, @"test failed: %@", [error localizedDescription]);
        }];
      
        _testForCompletionBlock(&testCompleted);

        if(_showLogs) NSLog(@"test_105_New_Class_Disallowed Bar : bar after save = %@",
                           [b1 description]);

    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to deploy and access a local development backend for from the client.
 */
- (void)test_106_Local_Backend {
    STFail(@"Stackmob does not support localhost development.");
}

/*!
 * Tests the ability to access multiple backends from the client SDK.
 */
- (void)test_107_Multiple_Backends {
    @try {

        if(_sm_open) {
          
            __block BOOL testCompleted = NO;
          
            Bar * bar = [NSEntityDescription
                         insertNewObjectForEntityForName:@"Bar"
                         inManagedObjectContext:_sm_open_Context];
            [bar setValue:@"NSManagedObject Bar:bar" forKey:@"barParameter"];
            [bar setValue:[bar assignObjectId] forKey:[bar primaryKeyField]];
            if(_showLogs) NSLog(@"test_107_Multiple_Backends Bar : bar before save = %@",
                               [bar description]);
            // create the object
            [_sm_open_Context saveOnSuccess:^{
                                    testCompleted = YES;
                                  }
                                  onFailure:^(NSError *error) {
                                    testCompleted = YES;
                                    STAssertNil(error, @"test failed: %@", [error localizedDescription]);
                                  }];
            _testForCompletionBlock(&testCompleted);
          
            if(_showLogs) NSLog(@"test_105_New_Class_Disallowed Bar : bar after save = %@",
                               [bar description]);
        } else STAssertNotNil(_sm_open, @"no open backend");

        if(_sm_secure) {
          
            __block BOOL testCompleted = NO;          
            Bar * bar = [NSEntityDescription
                         insertNewObjectForEntityForName:@"Bar"
                         inManagedObjectContext:_sm_secure_Context];
            [bar setValue:@"NSManagedObject Bar:bar" forKey:@"barParameter"];
            [bar setValue:[bar assignObjectId] forKey:[bar primaryKeyField]];
            if(_showLogs) NSLog(@"test_107_Multiple_Backends Foo : f2 before save = %@",[bar description]);
            // create the object
            [_sm_secure_Context saveOnSuccess:^{
                                    testCompleted = YES;
                                  }
                                  onFailure:^(NSError *error) {
                                    testCompleted = YES;
                                    STAssertNil(error, @"test failed: %@", [error localizedDescription]);
                                  }];

            _testForCompletionBlock(&testCompleted);          
          
            if(_showLogs) NSLog(@"test_105_New_Class_Disallowed Bar : bar after save = %@",
                               [bar description]);
        } else STAssertNotNil(_sm_secure, @"no secure backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to access a metadata description of your backend.
 */
- (void)test_108_API_Metadata {
    STFail(@"Stackmob does not provide API metadata.");
}

#pragma mark - Object Construction tests

/*!
 * Tests the ability to create an object from a defined object class without having to create a client schema.
 */
- (void)test_200_Foo_ObjCClass {
    @try {
        __block BOOL testCompleted = NO;
        Foo * f1 = [[Foo alloc] init];
        f1.fooParameter = @"Foo : NSObject object";
        [[_sm_secure dataStore] createObject:f1 inSchema:@"Foo" onSuccess:
         ^(NSDictionary *theObject,
           NSString *schema) {
            testCompleted = YES;
             Foo * f2 = (Foo *)theObject;
             NSLog(@"Created foo %@ in schema %@", f2, schema);
         } onFailure:^(NSError *theError,
                       NSDictionary *theObject,
                       NSString *schema) {
           testCompleted = YES;
            STAssertNil(theError, @"test failed: %@",
                         [theError localizedDescription]);
             NSLog(@"Error creating object: %@", theError);
         }];
      _testForCompletionBlock(&testCompleted);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to create an object without having to write a class file or create a client schema.
 */
- (void)test_201_Foo_Schemaless {
    @try {
        __block BOOL testCompleted = NO;
        NSDictionary * f1 = [NSDictionary dictionaryWithObjectsAndKeys:
                             @"NSDictionary Foo",@"fooParameter",
                             nil];
        if(_showLogs) NSLog(@"test_201_Foo_Schemaless NSDictionary : foo before save = %@",
                           [f1 description]);
        [[_sm_secure dataStore]
         createObject:f1 inSchema:@"Foo"
         onSuccess:^(NSDictionary *theObject, NSString *schema) {
            testCompleted = YES;
             NSLog(@"Created foo %@ in schema %@", theObject, schema);
         } onFailure:^(NSError *theError, NSDictionary *theObject, NSString *schema) {
            testCompleted = YES;
            STAssertNil(theError, @"test failed: %@",
                         [theError localizedDescription]);
             NSLog(@"Error creating object: %@", theError);
         }];
      _testForCompletionBlock(&testCompleted);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to create an object with a defined client class for a defined client schema.
 */
- (void)test_202_Bar_NSManagedObject_SubClass {
    @try {
        __block BOOL testCompleted = NO;
        Bar * b1 = [NSEntityDescription
                    insertNewObjectForEntityForName:@"Bar"
                    inManagedObjectContext:_sm_secure_Context];
        b1.barParameter = @"Bar : NSManagedObject object";
        // or, if you prefer
        [b1 setBarParameter:@"Bar : NSManagedObject object"];
        [b1 setValue:[b1 assignObjectId] forKey:[b1 primaryKeyField]];
        if(_showLogs) NSLog(@"test_202_Bar_NSManagedObject Bar : bar before save = %@",
                           [b1 description]);

        [_sm_secure_Context saveOnSuccess:^{
                                testCompleted = YES;
                                  if(_showLogs) NSLog(@"test_202_Bar_NSManagedObject Bar : bar saved to remote");
                                }
                                onFailure:^(NSError *error) {
                                  testCompleted = YES;
                                  STAssertNil(error, @"test failed: %@", [error localizedDescription]);
                                }];
      
      _testForCompletionBlock(&testCompleted);

    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to create an object for a defined client schema without having to write a class file.
 */
- (void)test_203_NSManagedObject_Called_Bar {
    @try {
        __block BOOL testCompleted = NO;
        NSManagedObject *b1 = [NSEntityDescription
                               insertNewObjectForEntityForName:@"Bar"
                               inManagedObjectContext:_sm_secure_Context];
        [b1 setValue:@"NSManagedObject : Bar" forKey:@"barParameter"];
        [b1 setValue:[b1 assignObjectId] forKey:[b1 primaryKeyField]];
        if(_showLogs) NSLog(@"test_203_NSManagedObject_Called_Bar NSManagedObject : b1 before save = %@",
                           [b1 description]);

        [_sm_secure_Context saveOnSuccess:^{
                                  testCompleted = YES;
                                  if(_showLogs) NSLog(@"test_203_NSManagedObject_Called_Bar NSManagedObject : b1 saved to remote");
                                }
                                onFailure:^(NSError *error) {
                                  testCompleted = YES;
                                  STAssertNil(error, @"test failed: %@", [error localizedDescription]);
                                }];
      _testForCompletionBlock(&testCompleted);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability for a schemaless backend to automatically add a new member to
 * a Class if allowed by configuration.
 * to be added by the client if allowed by configuration.
 * Since we are not able to talk to multiple backends, we will use the newMembersAllowed
 * setting to run the test if true, and skip if false.
 */
- (void)test_204_New_Member_Allowed {
    @try {
        __block BOOL testCompleted = NO;
        NSString * memberName = [self randomString:5];
        NSManagedObject *b1 = [NSEntityDescription
                               insertNewObjectForEntityForName:@"Bar"
                               inManagedObjectContext:_sm_open_Context];
        [b1 setValue:@"a schemaless addition" forKey:memberName];
        [b1 setValue:[b1 assignObjectId] forKey:[b1 primaryKeyField]];
        if(_showLogs) NSLog(@"test_204_New_Member_Allowed NSManagedObject : b1 before save = %@",
                           [b1 description]);
        [_sm_secure_Context saveOnSuccess:^{
                                testCompleted = YES;
                                if(_showLogs) NSLog(@"test_204_New_Member_Allowed NSManagedObject : b1 saved to remote");
                              }
                              onFailure:^(NSError *error) {
                                testCompleted = YES;
                                STAssertNil(error, @"test failed: %@", [error localizedDescription]);
                              }];
      _testForCompletionBlock(&testCompleted);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests that a backend will add a new member propertry of a Class definition
 * to be added by the client if allowed by configuration.
 * Since we are not able to talk to multiple backends, we will use the newMembersAllowed
 * setting to run the test if false, and skip if true.
 */
- (void)test_205_New_Member_Disallowed {
    @try {
        __block BOOL testCompleted = NO;
        NSString * memberName = [self randomString:5];
        NSManagedObject *b1 = [NSEntityDescription
                               insertNewObjectForEntityForName:@"Bar"
                               inManagedObjectContext:_sm_secure_Context];
        [b1 setValue:@"a schemaless addition" forKey:memberName];
        [b1 setValue:[b1 assignObjectId] forKey:[b1 primaryKeyField]];
        if(_showLogs) NSLog(@"test_205_New_Member_Disallowed NSManagedObject : b1 before save = %@",
                           [b1 description]);

        [_sm_secure_Context saveOnSuccess:^{
                              testCompleted = YES;
                              if(_showLogs) NSLog(@"test_205_New_Member_Disallowed NSManagedObject : b1 saved to remote");
                            }
                            onFailure:^(NSError *error) {
                              testCompleted = YES;
                              STAssertNil(error, @"test failed: %@", [error localizedDescription]);
                            }];
      _testForCompletionBlock(&testCompleted);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests if and what additional information about your object (metadata) is provided.
 */
- (void)test_206_Metadata_Provided {
    @try {
        __block BOOL testCompleted = NO;
        Bar * b1 = [NSEntityDescription
                    insertNewObjectForEntityForName:@"Bar"
                    inManagedObjectContext:_sm_secure_Context];
        b1.barParameter = @"Some string in a Bar : NSObject object";
        // or, if you prefer
        [b1 setBarParameter:@"Bar : NSManagedObject object"];
        [b1 setValue:[b1 assignObjectId] forKey:[b1 primaryKeyField]];
        if(_showLogs) NSLog(@"test_206_Metadata_Provided Bar : bar before save = %@",
                           [b1 description]);

      [_sm_secure_Context saveOnSuccess:^{
                            testCompleted = YES;
                            if(_showLogs) NSLog(@"test_206_Metadata_Provided NSManagedObject : b1 saved to remote");
                          }
                          onFailure:^(NSError *error) {
                            testCompleted = YES;
                            STAssertNil(error, @"test failed: %@", [error localizedDescription]);
                          }];
      
        _testForCompletionBlock(&testCompleted);
      
        STAssertNotNil(b1.sm_owner, @"no sm_owner: %@",b1.sm_owner);
        NSLog(@"metadata: sm_owner %@",b1.sm_owner);
        STAssertNotNil(b1.objectID, @"no objectID: %@",b1.objectID);
        NSLog(@"metadata: objectID %@",b1.objectID);
        STAssertNotNil(b1.createddate, @"no createddate: %@",b1.createddate);
        NSLog(@"metadata: createddate %@",b1.createddate);
        STAssertNotNil(b1.lastmoddate, @"no lastmoddate: %@",b1.lastmoddate);
        NSLog(@"metadata: lastmoddate %@",b1.lastmoddate);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

#pragma mark - Sample data utilities

/*!
 Generates a random string of up to 1000 characters in length. Generates a random length up to 1000 if numCharacters is set to 0.
 */
- (NSString *)randomString:(int)numCharacters {
    static char const possibleChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int len;
    if(numCharacters > 1000 || numCharacters == 0) len = (int)rand() % (1000);
    else len = numCharacters;
    unichar characters[len];
    for( int i=0; i < len; ++i ) {
        characters[i] = possibleChars[arc4random_uniform(sizeof(possibleChars)-1)];
    }
    return [NSString stringWithCharacters:characters length:len] ;
}

@end
