//
//  KitchenSyncKinveyTests.m
//  KitchenSyncKinveyTests
//
//

#import "KitchenSyncKinveyTests.h"
#import "Foo.h"
#import "Bar.h"
#import "KinveyConfigurator.h"

@interface KitchenSyncKinveyTests()

@property (nonatomic, assign) BOOL showLogs;
@property (nonatomic, assign) BOOL usingSecureBackend;
@property (nonatomic, assign) BOOL runCoreDataTests;
@property (nonatomic, copy) void (^testForCompletionBlock)(BOOL *testCompleted);

- (void)configureBackend:(BOOL)overrideConfig;
- (void)deleteAllTestData;
- (void)setupCoreDataEnvironment;
- (void)deleteAllFoo;
- (NSString *)randomString:(int)numCharacters;

@end

@implementation KitchenSyncKinveyTests

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
  
  NSDictionary *kinveyAppConfiguration = [[KinveyConfigurator sharedInstance] applicationConfiguration];
  
  _showLogs           = [kinveyAppConfiguration[@"ShowLogs"] boolValue];
  _usingSecureBackend = [kinveyAppConfiguration[@"UseSecure"] boolValue];
  _runCoreDataTests   = [kinveyAppConfiguration[@"RunCoreDataTests"] boolValue];
  
  // Set-up code here.
    [self setupCoreDataEnvironment];
    [self configureBackend:NO];
    [self deleteAllTestData];
}

/*!
 * Setting up the backend and configuration.
 */
- (void)configureBackend:(BOOL)overrideConfig {
  
    NSDictionary *kinveyAppConfiguration = [[KinveyConfigurator sharedInstance] applicationConfiguration];

    /**
     * This will force the secure backend to be used. This is useful for certain
     * tests.
     */
    if (overrideConfig) {
      _usingSecureBackend = YES;
    }
  
    // KCSClient is a singleton, so have to use two configs...
    if(_usingSecureBackend) {
       (void) [[KCSClient sharedClient] initializeKinveyServiceForAppKey:kinveyAppConfiguration[@"SecureAppKey"]
                                                           withAppSecret:kinveyAppConfiguration[@"SecureAppSecrect"]
                                                            usingOptions:nil];
    } else {
        (void) [[KCSClient sharedClient] initializeKinveyServiceForAppKey:kinveyAppConfiguration[@"OpenAppKey"]
                                                            withAppSecret:kinveyAppConfiguration[@"OpenAppSecrect"]
                                                             usingOptions:nil];
    }
    KCSCollection* foos = [KCSCollection
                           collectionFromString:@"Foo"
                           ofClass:[Foo class]];
    KCSCollection* bars = [KCSCollection
                           collectionFromString:@"Bar"
                           ofClass:[Bar class]];
    _fooStore = [KCSCachedStore
                storeWithCollection:foos
                options:@{ KCSStoreKeyCachePolicy : @(KCSCachePolicyNetworkFirst)}];
    _barStore = [KCSLinkedAppdataStore
                storeWithCollection:bars
                options:nil];
}

- (void)setupCoreDataEnvironment {
    // Destroy objects
    _persistentStoreCoordinator = nil;
    _managedObjectModel         = nil;
    _managedObjectContext       = nil;

    // Setup Core Data
    _managedObjectModel         = [NSManagedObjectModel mergedModelFromBundles:nil];
    _persistentStoreCoordinator = [[NSPersistentStoreCoordinator alloc]
                                  initWithManagedObjectModel:_managedObjectModel];
    NSError *error;
    // Setup an in memory store
    if(![_persistentStoreCoordinator addPersistentStoreWithType:NSInMemoryStoreType
                                                  configuration:nil
                                                            URL:nil
                                                        options:nil
                                                          error:&error]) {

      STFail(@"Could not create store coordinator: %@", [error localizedDescription]);
    }
  
    _managedObjectContext = [[NSManagedObjectContext alloc] init];

    [_managedObjectContext setPersistentStoreCoordinator:_persistentStoreCoordinator];
    [_managedObjectContext setUndoManager:nil];
}

#pragma mark - Teardown

- (void)tearDown {
    [super tearDown];
    [self deleteAllTestData];
}

/*!
 This method deletes all Test data from the backend.
 */
- (void)deleteAllTestData {
    @try {
        [self deleteAllFoo];
        [self deleteAllBar];
    } @catch (NSException * e) {
        NSLog(@"deleteAllTestData failed: %@", [e description]);
    }
}

/*!
 This method deletes all Foo data from the backend.
 */
- (void)deleteAllFoo {
    @try {
      __block BOOL testCompleted = NO;
        [_fooStore queryWithQuery:[KCSQuery query] withCompletionBlock:^(NSArray *objectsOrNil, NSError *errorOrNil) {
          STAssertNil(errorOrNil, @"error should be nil: %@", [errorOrNil description]);
          for (id obj in objectsOrNil) {
          [_fooStore removeObject:obj
              withCompletionBlock:^(NSArray *objectsOrNil, NSError *errorOrNil){
                testCompleted = YES;
                STAssertNil(errorOrNil, @"error should be nil: %@", [errorOrNil description]);
              }
                withProgressBlock:nil];
          }
          testCompleted = YES;
        } withProgressBlock:nil];
      _testForCompletionBlock(&testCompleted);
    } @catch (NSException * e) {
        NSLog(@"deleteAllFoo failed: %@", [e description]);
    }
}

/*!
 This method deletes all Bar data from the backend.
 */
- (void)deleteAllBar {
    @try {
      __block BOOL testCompleted = NO;
        [_barStore queryWithQuery:[KCSQuery query] withCompletionBlock:^(NSArray *objectsOrNil, NSError *errorOrNil) {
          STAssertNil(errorOrNil, @"error should be nil: %@", [errorOrNil description]);

          for (id obj in objectsOrNil) {
            
            [_barStore removeObject:obj
                withCompletionBlock:^(NSArray *objectsOrNil, NSError *errorOrNil){
                  testCompleted = YES;
                  STAssertNil(errorOrNil, @"error should be nil: %@", [errorOrNil description]);
                }
                withProgressBlock:nil];
          }
          testCompleted = YES;
        } withProgressBlock:nil];
      
      _testForCompletionBlock(&testCompleted);
    } @catch (NSException * e) {
        NSLog(@"deleteAllBar failed: %@", [e description]);
    }
}

#pragma mark - Setup and configuration tests

/*!
 This method deletes all Test data from the backend.
 */
- (void)test_000_Delete_All_Test_Data {
  @try {
    [self deleteAllFoo];
    [self deleteAllBar];
  } @catch (NSException * e) {
    STAssertNil(e, @"test failed: %@", [e description]);
  }
}

/*!
 * Tests the ability to access data without authentication key if allowed.
 */
- (void)test_100_Public_Access {
  STFail(@"kinvey doesn't support anonymous access");
//    @try {
//        // create a foo object
//        Foo * f1 = [[Foo alloc] init];
//        f1.fooParameter = @"Testing public access";
//        if(_showLogs) NSLog(@"test_100_Public_Access Foo:foo before save = %@",
//                           [f1 description]);
//      
//        __block BOOL testCompleted = NO;
//        // create an object to make sure backend is really working
//        [_fooStore saveObject:f1 withCompletionBlock:^(NSArray *objectsOrNil,
//                                                      NSError *createError) {
//          STAssertNil(createError, @"create error should be nil: %@", [createError description]);
//            if (createError == nil) {
//                // verify that the object can be accessed without authentication
//                // TODO - need to check if this is the actual client auth credential
//                NSString *kinveyAppId = _usingSecureBackend ? @"kid_VPArUQbBtf" : @"kid_TT-0IZbKn5";
//
//                NSURLCredential *kc = [[NSURLCredential alloc]
//                                       initWithUser:kinveyAppId
//                                       password:nil persistence:NSURLCredentialPersistenceNone];
//                [[KCSClient sharedClient] setAuthCredentials:kc];
//                Foo *f2 = (Foo *)objectsOrNil[0];
//                STAssertNotNil(f2.kinveyObjectId, @"f2 guid is null");
//                if(_showLogs) NSLog(@"\r\n\t test_100_Public_Access Foo:f1 after save = %@, \r\n\tMetadata: %@",[f1 description], [f1 metadata]);
//                if(_showLogs) NSLog(@"\r\n\t test_100_Public_Access Foo:f2 after save = %@, \r\n\tMetadata: %@",[f2 description], f2.metadata);
//                [_fooStore loadObjectWithID:f2.kinveyObjectId
//                       withCompletionBlock:^(NSArray *objectsOrNil, NSError *readError) {
//                           if(!readError) {
//                               Foo *f3 = objectsOrNil[0];
//                               if(_showLogs) NSLog(@"\r\n\t test_100_Public_Access Foo:f3 after read = %@, \r\n\tMetadata: %@",[f3 description], [f3 metadata]);
//                           } else
//                               STAssertNil(readError,
//                                           @"Should not have been able to read object from the backend: %@",
//                                           [readError localizedDescription]);
//                       } withProgressBlock:nil];
//
//              testCompleted = YES;
//
//            } else {
//              
//              testCompleted = YES;
//              
//              STAssertNil(createError, @"saveObject failed: %@",
//                          [createError localizedDescription]);
//            }
//        } withProgressBlock:nil];
//      
//      _testForCompletionBlock(&testCompleted);
//    } @catch (NSException * e) {
//        STAssertNil(e, @"test failed: %@", [e description]);
//    }
}

/*!
 * Tests the ability to require secure access to access data.
 */
- (void)test_101_Secure_Access {
    @try {
      [self configureBackend:YES];
        Foo * f1 = [[Foo alloc] init];
        f1.fooParameter = @"Testing secure access";
        if(_showLogs) NSLog(@"test_100_Public_Access Foo : foo before save = %@",
                           [f1 description]);
      
        __block BOOL testCompleted = NO;

        // create an object to make sure backend is really working
        [_fooStore saveObject:f1
         withCompletionBlock:^(NSArray *objectsOrNil,
                               NSError *createError) {
             if (createError == nil) {
                 Foo *f2 = objectsOrNil[0];
                 STAssertNotNil(f2.kinveyObjectId, @"f2 guid is null");

                 if(_showLogs) NSLog(@"\r\n\t test_100_Public_Access f1 after save = %@, \r\n\tMetadata: %@",[f1 description], [f1 metadata]);
                 if(_showLogs) NSLog(@"\r\n\t test_100_Public_Access f2 after save = %@, \r\n\tMetadata: %@",[f2 description], f2.metadata);

                 testCompleted = YES;
               
             } else {

               testCompleted = YES;
               
               STAssertNil(createError, @"saveObject failed: %@",
                           [createError localizedDescription]);
             }
         } withProgressBlock:nil];
      
      _testForCompletionBlock(&testCompleted);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability for a schemaless backend to learn a new Collection automatically.
 */
- (void)test_102_New_Class_Allowed {
        STFail(@"Kinvey does not support collections.");
}

/*!
 * Tests that a backend with a locked schema will not allow adding a new Collection automatically.
 */
- (void)test_103_New_Class_Disallowed {
        STFail(@"Kinvey does not support collections.");
}

/*!
 * Tests the ability for a schemaless backend to learn a new Class automatically.
 * Since we are not able to talk to multiple backends, we will use the usingSecureBackend
 * setting to run the test if true, and skip if false.
 */
- (void)test_104_New_Collection_Allowed {
    @try {
      
        __block BOOL testCompleted = NO;
      
        if(!_usingSecureBackend) {
            NSString * clazz = [self randomString:5];
            NSString * cs = [NSString stringWithFormat:@"NSDictionary with clazz %@",clazz];
            KCSCollection * c = [KCSCollection collectionFromString:clazz ofClass:nil];
            NSDictionary * dic = [NSDictionary dictionaryWithObjectsAndKeys:
                                  cs,@"newParameter",
                                  @"a schemaless addition",@"newFooParameter",
                                  nil];
            if(_showLogs) NSLog(@"\r\n\t test_104_New_Class_Allowed obj after read = %@, \r\n\tMetadata: %@",
                               [dic description], [dic valueForKey:@"metadata"]);
          
            [dic saveToCollection:c withCompletionBlock:^(NSArray *objectsOrNil,
                                                          NSError *createError) {
                testCompleted = YES;
                STAssertNil(createError, @"test failed: %@",
                            [createError localizedDescription]);
                if(_showLogs) NSLog(@"\r\n\t test_104_New_Class_Allowed obj after read = %@, \r\n\tMetadata: %@",
                                   [dic description], [dic valueForKey:@"metadata"]);
            } withProgressBlock:nil];
        } else if(_showLogs) {
          testCompleted = YES;
          NSLog(@"test_104_New_Class_Allowed could not run with this configuration, skipping test");
        }
      
      _testForCompletionBlock(&testCompleted);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests that a backend with a locked schema will not allow adding a new Class automatically.
 * Since we are not able to talk to multiple backends, we will use the usingSecureBackend
 * setting to run the test if false, and skip if true based on how the backend is configured.
 */
- (void)test_105_New_Collection_Disallowed {
    STFail(@"kinvey doesn't prevent new collections from being created via client");
//    @try {
//      
//       __block BOOL testCompleted = NO;
//        if(_usingSecureBackend) {
//            NSString * clazz = [self randomString:5];
//            NSString * cs = [NSString stringWithFormat:@"NSDictionary with clazz %@",clazz];
//            KCSCollection * c = [KCSCollection collectionFromString:clazz ofClass:nil];
//            NSDictionary * dic = [NSDictionary dictionaryWithObjectsAndKeys:
//                                  cs,@"newParameter",
//                                  @"a schemaless addition",@"newFooParameter",
//                                  nil];
//            if(_showLogs) NSLog(@"\r\n\t test_105_New_Class_Disallowed obj after read = %@, \r\n\tMetadata: %@",
//                               [dic description], [dic valueForKey:@"metadata"]);
//            [dic saveToCollection:c withCompletionBlock:^(NSArray *objectsOrNil,
//                                                          NSError *createError) {
//                testCompleted = YES;
//                STAssertNotNil(createError, @"test failed: %@",
//                               [createError localizedDescription]);
//                if(!createError)
//                    STFail(@"Should not have been able to create this on the backend.");
//            } withProgressBlock:nil];
//        } else if(_showLogs) {
//            testCompleted = YES;
//            NSLog(@"test_105_New_Class_Disallowed could not be run with this configuration, skipping test");
//        }
//      _testForCompletionBlock(&testCompleted);
//    } @catch (NSException * e) {
//        STAssertNil(e, @"test failed: %@", [e description]);
//    }
}

/*!
 * Tests the ability to deploy and access a local development backend for from the client.
 */
- (void)test_106_Local_Backend {
        STFail(@"Kinvey does not support localhost development.");
}

/*!
 * Tests the ability to access multiple backends from the client SDK.
 */
- (void)test_107_Multiple_Backends {
    @try {
      
        NSDictionary *kinveyAppConfiguration = [[KinveyConfigurator sharedInstance] applicationConfiguration];
      
        NSError * crudError;
        [self configureBackend:NO];
        Foo * f1 = [[Foo alloc] init];
        f1.fooParameter = @"Some string in a Foo : NSObject object";
        if(_showLogs) NSLog(@"test_107_Multiple_Backends Foo : f1 before save = %@",
                           [f1 description]);
      
        __block BOOL testCompleted = NO;
        // create the object
        [_fooStore saveObject:f1 withCompletionBlock:^(NSArray *objectsOrNil,
                                                      NSError *createError) {
            testCompleted = YES;
            STAssertNil(createError, @"test failed: %@",
                        [createError localizedDescription]);
            if(_showLogs) NSLog(@"\r\n\t test_104_New_Class_Allowed obj after save = %@, \r\n\tMetadata: %@",
                               [f1 description], [f1 metadata]);
        } withProgressBlock:nil];
        Foo * f2 = [[Foo alloc] init];
        f2.fooParameter = @"Some string in a Foo : NSObject object";
        if(_showLogs) NSLog(@"test_107_Multiple_Backends Foo : f2 before save = %@",
                           [f2 description]);

        // create the object
      /**
       * In order to utilize multiple backends you we need to swap credentials for 
       * the other backend _not_ being used.
       */
      NSString *user     = _usingSecureBackend ? kinveyAppConfiguration[@"OpenAppKey"] : kinveyAppConfiguration[@"SecureAppKey"];
      NSString *password = _usingSecureBackend ? kinveyAppConfiguration[@"OpenAppSecrect"] : kinveyAppConfiguration[@"SecureAppSecrect"];

        crudError = nil;
        KCSCollection* foo1 = [KCSCollection
                               collectionFromString:@"Foo"
                               ofClass:[Foo class]];
        NSURLCredential *kc = [[NSURLCredential alloc]
                               initWithUser:user
                               password:password
                               persistence:NSURLCredentialPersistenceNone];
        [[KCSClient sharedClient] setAuthCredentials:kc];
        [f2 saveToCollection:foo1 withCompletionBlock:^(NSArray *objectsOrNil,
                                                        NSError *createError) {
           testCompleted = YES;
            STAssertNil(createError, @"test failed: %@",
                        [createError localizedDescription]);
            if(_showLogs) NSLog(@"\r\n\t test_104_New_Class_Allowed obj after save = %@, \r\n\tMetadata: %@",
                               [f2 description], [f2 metadata]);
        } withProgressBlock:nil];
      
        _testForCompletionBlock(&testCompleted);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to access a metadata description of your backend.
 */
- (void)test_108_API_Metadata {
    STFail(@"Kinvey does not provide API metadata.");
}

#pragma mark - Object Construction tests

/*!
 * Tests the ability to create an object from a defined object class without having to create a client schema.
 */
- (void)test_200_Foo_ObjCClass {
    @try {
      [self configureBackend:NO];
        Foo * f1 = [[Foo alloc] init];
        f1.fooParameter = @"Foo : NSObject object";
        // or, if you prefer
        [f1 setFooParameter:@"Foo : NSObject object"];
        if(_showLogs) NSLog(@"test_200_Foo_ObjCClass Foo : f1 before save = %@",
                           [f1 description]);
      
        __block BOOL testCompleted = NO;
        // create the object
        [_fooStore saveObject:f1 withCompletionBlock:^(NSArray *objectsOrNil,
                                                      NSError *createError) {
          
          testCompleted = YES;
             STAssertNil(createError, @"saveObject failed: %@",
                         [createError localizedDescription]);
             Foo *f2 = (Foo *)objectsOrNil[0];
             if(_showLogs) NSLog(@"\r\n\t test_200_Create_KitchenSinkMin f2 after save = %@, \r\n\tMetadata: %@",
                                [f2 description], f2.metadata);
         } withProgressBlock:nil];
      
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
      [self configureBackend:NO];
        NSDictionary * f1 = [NSDictionary dictionaryWithObjectsAndKeys:
                             @"NSDictionary Foo",@"fooParameter",
                             KCSEntityKeyId,@"kinveyObjectId",
                             nil];
        if(_showLogs) NSLog(@"test_201_Foo_Schemaless NSDictionary : foo before save = %@",
                           [f1 description]);
      
        __block BOOL testCompleted = NO;
        [_fooStore saveObject:f1
         withCompletionBlock:^(NSArray *objectsOrNil, NSError *createError) {
           testCompleted = YES;
             STAssertNil(createError, @"saveObject failed: %@",
                         [createError localizedDescription]);
             NSDictionary *f2 = objectsOrNil[0];
             if(_showLogs) NSLog(@"\r\n\t test_200_Create_KitchenSinkMin f2 after save = %@, \r\n\tMetadata: %@",
                                [f2 description], [f2 valueForKey:@"metadata"]);
         } withProgressBlock:nil];
      
      _testForCompletionBlock(&testCompleted);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to create an NSManagedObject object with a defined client class for a defined client schema.
 */
#warning "Because of the architecture Kinvey choose when dealing with NSManagedObject entites these tests will CRASH the target. BEWARE"
- (void)test_202_Bar_NSManagedObject_SubClass {
  if (_runCoreDataTests) {
    @try {
        [self configureBackend:NO];
        [self setupCoreDataEnvironment];
        Bar * b1 = [NSEntityDescription
                    insertNewObjectForEntityForName:@"Bar"
                    inManagedObjectContext:_managedObjectContext];
        b1.barParameter = @"Bar : NSManagedObject object";
        // or, if you prefer
        [b1 setBarParameter:@"Bar : NSManagedObject object"];
        if(_showLogs) NSLog(@"test_202_Bar_NSManagedObject Bar : bar before save = %@",
                           [b1 description]);
        __block BOOL testCompleted = NO;
        // save the object locally
        NSError * error;
        if (![_managedObjectContext save:&error]) {
          STAssertNil(error, @"context save error should be nil: %@", error);
          abort();
        }
        // create the object on the backend
        [_barStore saveObject:b1
         withCompletionBlock:^(NSArray *objectsOrNil, NSError *createError) {
           testCompleted = YES;
             STAssertNil(createError, @"saveObject failed: %@",
                         [createError localizedDescription]);
             Bar * b2 = objectsOrNil[0];
             if(_showLogs) NSLog(@"\r\n\t test_202_Bar_NSManagedObject_SubClass f2 after save = %@, \r\n\tMetadata: %@",[b2 description], [b2 valueForKey:@"metadata"]);
         } withProgressBlock:nil];
      
      _testForCompletionBlock(&testCompleted);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
  }
}

/*!
 * Tests the ability to create an object for a defined client schema without having to write a class file.
 */
#warning "Because of the architecture Kinvey choose when dealing with NSManagedObject entites these tests will CRASH the target. BEWARE"
- (void)test_203_NSManagedObject_Called_Bar {
  if (_runCoreDataTests) {
    @try {
        [self configureBackend:NO];
        [self setupCoreDataEnvironment];
        NSManagedObject *b1 = [NSEntityDescription
                               insertNewObjectForEntityForName:@"Bar"
                               inManagedObjectContext:_managedObjectContext];
        [b1 setValue:@"NSManagedObject for Entity named Bar" forKey:@"barParameter"];
        if(_showLogs) NSLog(@"test_203_NSManagedObject_Called_Bar Bar : bar before save = %@",
                           [b1 description]);
      
        __block BOOL testCompleted = NO;
        NSError * error;
        if (![_managedObjectContext save:&error]) {
          STAssertNil(error, @"context save error should be nil: %@", error);
          abort();
        }
      
        // create the object
        [_barStore saveObject:b1
         withCompletionBlock:^(NSArray *objectsOrNil, NSError *createError) {
             Bar * b2 = objectsOrNil[0];
           testCompleted = YES;
             if(_showLogs) NSLog(@"\r\n\t test_203_NSManagedObject_Called_Bar f2 after save = %@, \r\n\tMetadata: %@",[b2 description], [b2 valueForKey:@"metadata"]);
             STAssertNil(createError, @"saveObject failed: %@",
                         [createError localizedDescription]);
         } withProgressBlock:nil];
      
      _testForCompletionBlock(&testCompleted);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
  }
}

/*!
 * Tests the ability for a schemaless backend to automatically add a new member to
 * a Class if allowed by configuration.
 * Since we are not able to talk to multiple backends, we will use the usingSecureBackend
 * setting to run the test if true, and skip if false.
 */
- (void)test_204_New_Member_Allowed {
    @try {
        if(!_usingSecureBackend) {
            [self configureBackend:NO];
            NSString * memberName = [self randomString:5];
            NSDictionary * f1 = [NSDictionary dictionaryWithObjectsAndKeys:
                                 @"NSDictionary with class Foo",@"fooParameter",
                                 @"a schemaless addition",memberName,
                                 nil];
            if(_showLogs) NSLog(@"test_204_New_Member_Allowed NSDictionary : f1 before save = %@",
                               [f1 description]);
          
            __block BOOL testCompleted = NO;
            [_fooStore saveObject:f1
             withCompletionBlock:^(NSArray *objectsOrNil, NSError *createError) {
                 NSDictionary *f2 = objectsOrNil[0];
               testCompleted = YES;
                 if(_showLogs) NSLog(@"\r\n\t test_204_New_Member_Allowed f2 after save = %@, \r\n\tMetadata: %@",[f2 description], [f2 valueForKey:@"metadata"]);
                 STAssertNil(createError, @"saveObject failed: %@",
                             [createError localizedDescription]);
             } withProgressBlock:nil];
          
          _testForCompletionBlock(&testCompleted);
        } else if(_showLogs)
            NSLog(@"test_204_New_Member_Allowed could not be run with this configuration, skipping test");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests that a backend will not allow a new member propertry of a Class definition
 * to be added by the client if disallowed by configuration.
 * Since we are not able to talk to multiple backends, we will use the usingSecureBackend
 * setting to run the test if false, and skip if true.
 */
- (void)test_205_New_Member_Disallowed {
    @try {
          __block BOOL testCompleted = NO;
        if(_usingSecureBackend) {
            NSString * memberName = [self randomString:5];
            NSDictionary * f1 = [NSDictionary dictionaryWithObjectsAndKeys:
                                 @"NSDictionary with class Foo",@"fooParameter",
                                 @"a schemaless addition",memberName,
                                 nil];
            if(_showLogs) NSLog(@"test_205_New_Member_Disallowed NSDictionary : f1 before save = %@",
                               [f1 description]);

            [_fooStore saveObject:f1
             withCompletionBlock:^(NSArray *objectsOrNil, NSError *createError) {
               testCompleted = YES;
                 STAssertNotNil(createError, @"test failed: %@", [createError localizedDescription]);
                 if(!createError)
                     STFail(@"Should not have been able to create object to backend.");
             } withProgressBlock:nil];
          
          _testForCompletionBlock(&testCompleted);
        } else if(_showLogs) {
          testCompleted = YES;
            NSLog(@"test_205_New_Member_Disallowed could not be run with this configuration, skipping test");
        }
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests if and what additional information about your object (metadata) is provided
 */
- (void)test_206_Metadata_Provided {
    @try {
        __block BOOL testCompleted = NO;
        Foo * f1 = [[Foo alloc] init];
        f1.fooParameter = @"Foo : NSObject object";
        if(_showLogs) NSLog(@"\r\n\t test_206_Metadata_Provided f1 before save = %@",[f1 description]);
        // create the object
        [_fooStore saveObject:f1
         withCompletionBlock:^(NSArray *objectsOrNil, NSError *createError) {
           testCompleted = YES;
             STAssertNil(createError, @"test failed: %@",
                         [createError localizedDescription]);
             Foo * f2 = objectsOrNil[0];
             STAssertNotNil(f2.metadata, @"no metadata: %@",
                         [createError localizedDescription]);
             NSLog(@"metadata: %@",[f2.metadata description]);
         } withProgressBlock:nil];
      
      _testForCompletionBlock(&testCompleted);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

#pragma mark - Sample data utilities

/*!
 * Generates a random string of up to 1000 characters in length. Generates a random
 * length up to 1000 if numCharacters is set to 0.
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
