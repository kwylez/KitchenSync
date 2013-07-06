//
//  KitchenSyncFatfractalTests.m
//  KitchenSyncFatfractalTests
//
//

#import "KitchenSyncFatfractalTests.h"
#import "FatFractalConfigurator.h"
#import "Foo.h"
#import "Bar.h"

//TODO: Add in synchronization code for NSManagedObject

@interface KitchenSyncFatfractalTests()

@property (nonatomic, assign) BOOL showLogs;
@property (nonatomic, assign) BOOL usingSecureBackend;
@property (nonatomic, copy) void (^testForCompletionBlock)(BOOL *testCompleted);

- (void)configureBackend;
- (void)setupCoreDataEnvironment;
- (void)authenticateLocal;
- (void)authenticateOpen;
- (void)authenticateSecure;
- (NSString *)randomString:(int)numCharacters;

@end

@implementation KitchenSyncFatfractalTests

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
    
    NSDictionary *fatfractalAppConfiguration = [[FatFractalConfigurator sharedInstance] applicationConfiguration];
    
    _showLogs           = [fatfractalAppConfiguration[@"ShowLogs"] boolValue];
    _usingSecureBackend = [fatfractalAppConfiguration[@"UseSecure"] boolValue];
  
    [self setupCoreDataEnvironment];
    [self configureBackend];
}


/*!
 * Setting up the backend and configuration.
 */
- (void)configureBackend {
  
    NSDictionary *fatfractalAppConfiguration = [[FatFractalConfigurator sharedInstance] applicationConfiguration];

    if (! _ff_open) {
        _ff_open = [[FatFractal alloc]
                   initWithBaseUrl:fatfractalAppConfiguration[@"OpenURL"]];
        [_ff_open setDebug:NO];
    }
    if (! _ff_secure) {
        _ff_secure = [[FatFractal alloc]
                     initWithBaseUrl:fatfractalAppConfiguration[@"SecureURL"]];
        [_ff_secure setDebug:NO];
    }
    if (! _ff_local) {
        _ff_local = [[FatFractal alloc]
                    initWithBaseUrl:fatfractalAppConfiguration[@"LocalURL"]];
        [_ff_local setDebug:NO];
        NSError * crudError;
        @try {
            // verify localhost is installed
            [_ff_local loginWithUserName:@"test_user" andPassword:@"test_user" error:&crudError];
            STAssertNil(crudError, @"test failed: %@",
                        [crudError localizedDescription]);
        }
        @catch (NSException *exception) {
            STAssertNotNil(_ff_secure, @"no secure backend");
            if(crudError) _ff_local = nil;
        }
        @finally {
            if(crudError) _ff_local = nil;
        }
    }
}

- (void)setupCoreDataEnvironment {
    // Destroy objects
    _persistentStoreCoordinator = nil;
    _managedObjectModel         = nil;
    _managedObjectContext       = nil;
    
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
    
    _managedObjectContext = [[NSManagedObjectContext alloc] init];

    [_managedObjectContext setPersistentStoreCoordinator:_persistentStoreCoordinator];
    [_managedObjectContext setUndoManager:nil];
    
}

- (void)authenticateLocal {
    [_ff_local loginWithUserName:@"test_user" andPassword:@"test_user"];
}

- (void)authenticateOpen {
    [_ff_open loginWithUserName:@"test_user" andPassword:@"test_user"];
}

- (void)authenticateSecure {
    [_ff_secure loginWithUserName:@"test_user" andPassword:@"test_user"];
}

#pragma mark - Teardown

- (void)tearDown {
  [super tearDown];
}

/*!
 This method deletes all Foo objects from the backend.
 */
- (void)deleteAllFoo {
    @try {
        [self authenticateOpen];
        NSArray *openFoos = [_ff_open getArrayFromUrl:[NSString stringWithFormat:@"/ff/resources/Foo/"]];
        for (id obj in openFoos) {
            [_ff_open deleteObj:obj];
        }
        [self authenticateSecure];
        NSArray *secureFoos = [_ff_secure getArrayFromUrl:[NSString stringWithFormat:@"/ff/resources/Foo/"]];
        for (id obj in secureFoos) {
            [_ff_secure deleteObj:obj];
        } if(_ff_local) {
            [self authenticateLocal];
            NSArray *localFoos = [_ff_local getArrayFromUrl:[NSString stringWithFormat:@"/ff/resources/Foo/"]];
            for (id obj in localFoos) {
                [_ff_local deleteObj:obj];
            }
        }
    } @catch (NSException * e) {
        NSLog(@"deleteAllFoo failed: %@", [e description]);
    }
}

/*!
 This method deletes all Bar objects from the backend.
 */
- (void)deleteAllBar {
    @try {
        [self authenticateOpen];
        NSArray *openBars = [_ff_open getArrayFromUrl:[NSString stringWithFormat:@"/ff/resources/Bar/"]];
        for (id obj in openBars) {
            [_ff_open deleteObj:obj];
        }
        [self authenticateSecure];
        NSArray *secureBars = [_ff_secure getArrayFromUrl:[NSString stringWithFormat:@"/ff/resources/Bar/"]];
        for (id obj in secureBars) {
            [_ff_secure deleteObj:obj];
        } if(_ff_local) {
            [self authenticateLocal];
            NSArray *localBars = [_ff_local getArrayFromUrl:[NSString stringWithFormat:@"/ff/resources/Bar/"]];
            for (id obj in localBars) {
                [_ff_local deleteObj:obj];
            }
        }
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
    NSLog(@"deleteAllFoo failed: %@", [e description]);
  }
}

/*!
 * Tests the ability to access data without authentication if allowed.
 */
- (void)test_100_Public_Access {
    @try {
        if(_ff_open) {
            [self authenticateOpen];
            // create a foo object
            Foo * foo = [[Foo alloc] init];
            foo.fooParameter = @"Testing public access";
            if(_showLogs) NSLog(@"\r\n\t test_100_Public_Access foo before save = %@",[foo description]);
            // create an object to make sure backend is really working
            NSError * crudError;
            [_ff_open createObj:foo atUri:@"/Foos" error:&crudError];
            STAssertNil(crudError, @"test failed: %@", [crudError localizedDescription]);
            // verify that the object can be accessed without authentication
            [_ff_open logout];
            Foo  * publicFoo = [_ff_open getObjFromUri:[[_ff_open metaDataForObj:foo] ffUrl]
                                                error:&crudError];
            STAssertNil(crudError, @"test failed: %@", [crudError localizedDescription]);
            if(_showLogs) NSLog(@"\r\n\t test_100_Public_Access publicFoo after save = %@, \r\n\tMetadata: %@",[publicFoo description],
                               [[_ff_open metaDataForObj:publicFoo] description]);
        } else STAssertNotNil(_ff_open, @"no open backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to require secure access to access data.
 */
- (void)test_101_Secure_Access {
    @try {
        if(_ff_secure) {
            [self authenticateSecure];
            // create a foo object
            Foo * foo = [[Foo alloc] init];
            foo.fooParameter = @"Testing secure access";
            if(_showLogs)
                NSLog(@"\r\n\t test_101_Secure_Access foo before save = %@",
                      [foo description]);
            // create an object to make sure backend is really working
            NSError * crudError;
            [_ff_secure createObj:foo atUri:@"/Foos" error:&crudError];
            STAssertNil(crudError, @"test failed: %@",
                        [crudError localizedDescription]);
            [_ff_secure logout];
            // verify that the object cannot be accessed without authentication
            [_ff_secure getObjFromUri:[[_ff_secure metaDataForObj:foo]
                                      ffUrl] error:&crudError];
            STAssertNotNil(crudError, @"test failed: %@",
                           [crudError localizedDescription]);
            if(!crudError)
                STFail(@"Should not have been able to read object from the backend.");
        } else STAssertNotNil(_ff_secure, @"no secure backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability for a schemaless backend to learn a new Collection automatically.
 */
- (void)test_102_New_Collection_Allowed {
    @try {
        if(_ff_open) {
            [self authenticateOpen];
            NSString * coll = [NSString stringWithFormat:@"/%@",[self randomString:5]];
            NSString * cs = [NSString stringWithFormat:@"NSDictionary in coll %@",coll];
            NSDictionary * foo =
            [NSDictionary dictionaryWithObjectsAndKeys:
             @"Foo",@"clazz",
             cs,@"fooParameter",
             nil];
            if(_showLogs) NSLog(@"\r\n\t test_102_New_Collection_Allowed foo before save = %@",[foo description]);
            NSError * crudError;
            foo = [_ff_open createObj:foo atUri:coll error:&crudError];
            STAssertNil(crudError, @"test failed: %@",
                        [crudError localizedDescription]);
            if(_showLogs) NSLog(@"\r\n\t test_102_New_Collection_Allowed f1 before save = %@, \r\n\tMetadata: %@",[foo description],
                               [[_ff_open metaDataForObj:foo] description]);
        } else STAssertNotNil(_ff_open, @"no open backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests that a backend with a locked schema will not allow a new Collection
 * to be added by the client if disallowed by configuration.
 */
- (void)test_103_New_Collection_Disallowed {
    @try {
        if(_ff_secure) {
            [self authenticateSecure];
            NSString * coll = [NSString stringWithFormat:@"/%@",[self randomString:5]];
            NSDictionary * foo =
            [NSDictionary dictionaryWithObjectsAndKeys:
             @"Foo",@"clazz",
             [NSString stringWithFormat:@"NSDictionary in coll %@",coll],@"fooParameter",
             nil];
            if(_showLogs) NSLog(@"\r\n\t test_103_New_Collection_Disallowed foo before save = %@",[foo description]);
            NSError * crudError;
            [_ff_secure createObj:foo atUri:coll error:&crudError];
            STAssertNotNil(crudError, @"test failed: %@",
                           [crudError localizedDescription]);
            if(!crudError)
                STFail(@"Should not have been able to create object to backend.");
        } else STAssertNotNil(_ff_secure, @"no secure backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability for a schemaless backend to learn a new Class automatically.
 */
- (void)test_104_New_Class_Allowed {
    @try {
        if(_ff_open) {
            [self authenticateOpen];
            NSString * clazz = [self randomString:5];
            NSString * coll = [NSString stringWithFormat:@"/%@",clazz];
            NSString * cs = [NSString stringWithFormat:@"NSDictionary with clazz %@",clazz];
            NSDictionary * obj =
            [NSDictionary dictionaryWithObjectsAndKeys:
             clazz,@"clazz",
             cs,@"newParameter",
             nil];
            if(_showLogs) NSLog(@"\r\n\t test_104_New_Class_Allowed NSDictionary * obj before save = %@",[obj description]);
            NSError * crudError;
            [_ff_open createObj:obj atUri:coll error:&crudError];
            STAssertNil(crudError, @"test failed: %@",
                        [crudError localizedDescription]);
            if(_showLogs) NSLog(@"\r\n\t test_104_New_Class_Allowed obj after save = %@, \r\n\tMetadata: %@",[obj description],
                               [[_ff_open metaDataForObj:obj] description]);
        } else STAssertNotNil(_ff_open, @"no open backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests that a backend with a locked schema will not allow a new Class definition
 * to be added by the client if disallowed by configuration.
 */
- (void)test_105_New_Class_Disallowed {
    @try {
        if(_ff_secure) {
            [self authenticateSecure];
            NSString * clazz = [NSString stringWithFormat:@"/%@",[self randomString:5]];
            NSString * coll = [NSString stringWithFormat:@"/%@",clazz];
            NSString * cs = [NSString stringWithFormat:@"NSDictionary with clazz %@",clazz];
            NSDictionary * obj =
            [NSDictionary dictionaryWithObjectsAndKeys:
             clazz,@"clazz",
             cs,@"newParameter",
             nil];
            if(_showLogs) NSLog(@"\r\n\t test_105_New_Class_Disallowed foo before save = %@",[obj description]);
            NSError * crudError;
            [_ff_secure createObj:obj atUri:coll error:&crudError];
            STAssertNotNil(crudError, @"test failed: %@",
                           [crudError localizedDescription]);
            if(!crudError)
                STFail(@"Should not have been able to create object to backend.");
        } else STAssertNotNil(_ff_secure, @"no secure backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to deploy and access a local development backend for from the client.
 */
- (void)test_106_Local_Backend {
    @try {
        if(_ff_open) {
            [self authenticateLocal];
            Foo * foo = [[Foo alloc] init];
            foo.fooParameter = @"Some string in a Foo : NSObject object";
            if(_showLogs) NSLog(@"\r\n\t test_106_Local_Backend foo before save = %@",[foo description]);
            // create an object to make sure backend is really working
            NSError * crudError;
            [_ff_open createObj:foo atUri:@"/Foos" error:&crudError];
            STAssertNil(crudError, @"test failed: %@",
                        [crudError localizedDescription]);
            // make sure backend is localhost
            NSString * url = _ff_open.baseUrl;
            STAssertTrue(([url rangeOfString:@"localhost"].location >= 0),
                         @"wrong backend %@", url);
            if(_showLogs) NSLog(@"\r\n\t test_106_Local_Backend foo after save = %@, \r\n\tMetadata: %@",[foo description],
                               [[_ff_open metaDataForObj:foo] description]);
        } else STAssertNotNil(_ff_open, @"no local backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to access multiple backends from the client SDK.
 */
- (void)test_107_Multiple_Backends {
    @try {
        NSError * crudError;
        if(_ff_open) {
            [self authenticateOpen];
            Foo * f1 = [[Foo alloc] init];
            f1.fooParameter = @"Some string in a Foo : NSObject object";
            if(_showLogs) NSLog(@"\r\n\t test_107_Multiple_Backends f1 before save = %@",[f1 description]);
            // create the object
            [_ff_open createObj:f1 atUri:@"/Foos" error:&crudError];
            STAssertNil(crudError, @"test failed: %@",
                        [crudError localizedDescription]);
            if(_showLogs) NSLog(@"\r\n\t test_107_Multiple_Backends f1 after save = %@, \r\n\tMetadata: %@",[f1 description],
                               [[_ff_open metaDataForObj:f1] description]);
        } else STAssertNotNil(_ff_open, @"no open backend");
        if(_ff_secure) {
            crudError = nil;
            [self authenticateSecure];
            Foo * f2 = [[Foo alloc] init];
            f2.fooParameter = @"Some string in a Foo : NSObject object";
            if(_showLogs) NSLog(@"test_107_Multiple_Backends Foo : f2 before save = %@",[f2 description]);
            // create the object
            crudError = nil;
            [_ff_secure createObj:f2 atUri:@"/Foos" error:&crudError];
            STAssertNil(crudError, @"test failed: %@",
                        [crudError localizedDescription]);
            if(_showLogs) NSLog(@"\r\n\t test_107_Multiple_Backends f2 after save = %@, \r\n\tMetadata: %@",[f2 description],
                               [[_ff_secure metaDataForObj:f2] description]);
        } else STAssertNotNil(_ff_secure, @"no secure backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to access a metadata description of your backend.
 */
- (void)test_108_API_Metadata {
    @try {
        if(_ff_secure) {
            NSError * crudError;
            NSDictionary * md = [_ff_secure getObjFromUri:@"/ff/metadata" error:&crudError];
            STAssertNil(crudError, @"test failed: %@",
                        [crudError localizedDescription]);
            STAssertNotNil(md, @"not able to get backend metadata");
            if(_showLogs) NSLog(@"\r\n\t test_108_API_Metadata md = %@",
                               [md description]);
        } else STAssertNotNil(_ff_secure, @"no ff_secure backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

#pragma mark - Object Construction tests

/*!
 * Tests the ability to create an object from a defined object class without having to create a client schema.
 */
- (void)test_200_Foo_ObjCClass {
    @try {
        if(_ff_open) {
            [self authenticateLocal];
            Foo * foo = [[Foo alloc] init];
            foo.fooParameter = @"Some string in a Foo : NSObject object";
            // or, if you prefer
            [foo setFooParameter:@"Foo : NSObject object"];
            if(_showLogs) NSLog(@"\r\n\t test_200_Foo_ObjCClass f1 before save = %@",[foo description]);
            // create the object
            [_ff_open createObj:foo atUri:@"/Foo"];
            if(_showLogs) NSLog(@"\r\n\t test_200_Foo_ObjCClass foo after save = %@, \r\n\tMetadata: %@",[foo description],
                               [[_ff_open metaDataForObj:foo] description]);
        } else STAssertNotNil(_ff_open, @"no ff_open backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to create an object without having to write a class file or create a client schema.
 */
- (void)test_201_Foo_Schemaless {
    @try {
        if(_ff_open) {
            [self authenticateOpen];
            NSDictionary * foo = [NSDictionary dictionaryWithObjectsAndKeys:
                                  @"Foo",@"clazz",
                                  @"NSDictionary with clazz Foo",@"fooParameter",
                                  nil];
            if(_showLogs) NSLog(@"test_201_Foo_Schemaless NSDictionary : foo before save = %@",
                               [foo description]);
            foo = [_ff_open createObj:foo atUri:@"/Foos"];
            if(_showLogs) NSLog(@"\r\n\t test_201_Foo_Schemaless foo after save = %@, \r\n\tMetadata: %@",[foo description],
                               [[_ff_open metaDataForObj:foo] description]);
        } else STAssertNotNil(_ff_open, @"no ff_open backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to create an object with a defined client class for a defined client schema.
 */
- (void)test_202_Bar_NSManagedObject_SubClass {
    @try {
        if(_ff_open) {
            [self setupCoreDataEnvironment];
            [self authenticateLocal];
            Bar * b1 = [NSEntityDescription
                        insertNewObjectForEntityForName:@"Bar"
                        inManagedObjectContext:_managedObjectContext];
            b1.barParameter = @"Some string in a Bar : NSObject object";
            // or, if you prefer
            [b1 setBarParameter:@"Bar : NSManagedObject object"];
            if(_showLogs) NSLog(@"test_202_Bar_NSManagedObject_SubClass Bar:b1 before save = %@",
                               [b1 description]);
            NSError * crudError;
            // save to the backend
            [_ff_open createObj:b1 atUri:@"/Bars" error:&crudError];
            if(!crudError) {
                if(_showLogs) NSLog(@"\r\n\t test_202_Bar_NSManagedObject_SubClass Bar:b1 after save = %@, \r\n\tMetadata: %@",[b1 description],
                                   [[_ff_open metaDataForObj:b1] description]);
                // save the object locally
                [_managedObjectContext save:&crudError];
                if(!crudError) {
                    //make sure object can be retrieved from local store
                    Bar *b2 = (Bar *)[_managedObjectContext
                                      existingObjectWithID:[b1 objectID]
                                      error:&crudError];
                    if(!crudError) {
                        if(_showLogs) NSLog(@"test_202_Bar_NSManagedObject_SubClass Bar:b2 after local fetch = %@",
                                           [b2 description]);
                        Bar *b3 = [_ff_open getObjFromUri:b2.ffUrl
                                                    error:&crudError];
                        STAssertNil(crudError, @"test failed: %@",
                                    [crudError localizedDescription]);
                        if(_showLogs) NSLog(@"test_202_Bar_NSManagedObject_SubClass Bar:b3 after backend read = %@",
                                           [b3 description]);
                    } else STAssertNil(crudError, @"test failed: %@",
                                       [crudError localizedDescription]);
                } else STAssertNil(crudError, @"test failed: %@",
                                   [crudError localizedDescription]);
            } else STAssertNil(crudError, @"test failed: %@",
                               [crudError localizedDescription]);
        } else STAssertNotNil(_ff_open, @"no ff_open backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to create an object for a defined client schema without having to write a class file.
 */
- (void)test_203_NSManagedObject_Called_Bar {
    @try {
        if(_ff_open) {
            [self setupCoreDataEnvironment];
            [self authenticateLocal];
            NSManagedObject *b1 = [NSEntityDescription
                                   insertNewObjectForEntityForName:@"Bar"
                                   inManagedObjectContext:_managedObjectContext];
            [b1 setValue:@"NSManagedObject for Entity named Bar"
                  forKey:@"barParameter"];
            if(_showLogs) NSLog(@"test_203_NSManagedObject_Called_Bar NSManagedObject:b1 before save = %@",
                               [b1 description]);
            NSError * crudError;
            // save to the backend
            [_ff_open createObj:b1 atUri:@"/Bars" error:&crudError];
            if(!crudError) {
                if(_showLogs) NSLog(@"\r\n\t test_203_NSManagedObject_Called_Bar NSManagedObject:b1 after save = %@, \r\n\tMetadata: %@",[b1 description],
                                   [[_ff_open metaDataForObj:b1] description]);
                // save the object locally
                [_managedObjectContext save:&crudError];
                if(!crudError) {
                    //make sure object can be retrieved from local store
                    NSManagedObject *b2 = [_managedObjectContext
                                           existingObjectWithID:[b1 objectID]
                                           error:&crudError];
                    if(!crudError) {
                        if(_showLogs) NSLog(@"test_203_NSManagedObject_Called_Bar NSManagedObject:b2 after local fetch = %@",
                                           [b2 description]);
                        NSManagedObject *b3 = [_ff_open
                                               getObjFromUri:[b2 valueForKey:@"ffUrl"]
                                               error:&crudError];
                        STAssertNil(crudError, @"test failed: %@",
                                    [crudError localizedDescription]);
                        if(_showLogs) NSLog(@"test_203_NSManagedObject_Called_Bar NSManagedObject:b3 after backend read = %@",
                                           [b3 description]);
                    } else STAssertNil(crudError, @"test failed: %@",
                                       [crudError localizedDescription]);
                } else STAssertNil(crudError, @"test failed: %@",
                                   [crudError localizedDescription]);
            } else STAssertNil(crudError, @"test failed: %@",
                               [crudError localizedDescription]);
            STAssertNil(crudError, @"test failed: %@",
                        [crudError localizedDescription]);
            // save the object locally
            if(!crudError) {
                [_managedObjectContext save:&crudError];
            } else STAssertNil(crudError, @"test failed: %@",
                               [crudError localizedDescription]);
        } else STAssertNotNil(_ff_open, @"no ff_open backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests that a backend will add a new member propertry of a Class definition
 * to be added by the client if allowed by configuration.
 */
- (void)test_204_New_Member_Allowed {
    @try {
        if(_ff_open) {
            [self authenticateOpen];
            NSString * memberName = [self randomString:5];
            NSDictionary * f1 = [NSDictionary dictionaryWithObjectsAndKeys:
                                 @"Foo",@"clazz",
                                 @"NSDictionary with clazz Foo",@"fooParameter",
                                 @"a schemaless addition",memberName,
                                 nil];
            if(_showLogs) NSLog(@"test_204_New_Member_Allowed NSDictionary : foo before save = %@",
                               [f1 description]);
            NSError * crudError;
            Foo * f2 = [_ff_open createObj:f1 atUri:@"/Foos" error:&crudError];
            STAssertNil(crudError, @"test failed: %@",
                        [crudError localizedDescription]);
            if(_showLogs) NSLog(@"\r\n\t test_204_New_Member_Allowed bar after save = %@, \r\n\tMetadata: %@",[f2 description],
                               [[_ff_open metaDataForObj:f2] description]);
        } else STAssertNotNil(_ff_open, @"no ff_open backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests that a backend will not allow a new member propertry of a Class definition
 * to be added by the client if disallowed by configuration.
 */
- (void)test_205_New_Member_Disallowed {
    @try {
        if(_ff_secure) {
            [self authenticateSecure];
            NSString * memberName = [self randomString:5];
            NSDictionary * foo = [NSDictionary dictionaryWithObjectsAndKeys:
                                  @"Foo",@"clazz",
                                  @"NSDictionary with clazz Foo",@"fooParameter",
                                  @"a schemaless addition",memberName,
                                  nil];
            if(_showLogs) NSLog(@"test_205_New_Member_Disallowed NSDictionary : foo before save = %@",
                               [foo description]);
            NSError * crudError;
            [_ff_secure createObj:foo atUri:@"/Foos" error:&crudError];
            STAssertNotNil(crudError, @"should have gotten an error: %@",
                           [crudError description]);
            if(!crudError)
                STFail(@"Should not have been able to create object to backend.");
        } else STAssertNotNil(_ff_secure, @"no ff_secure backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests if and what additional information about your object (metadata) is provided
 */
- (void)test_206_Metadata_Provided {
    @try {
        if(_ff_open) {
            [self authenticateLocal];
            Foo * foo = [[Foo alloc] init];
            foo.fooParameter = @"Some string in a Foo : NSObject object";
            if(_showLogs) NSLog(@"\r\n\t test_206_Metadata_Provided_ObjC_Class f1 before save = %@",[foo description]);
            // create the object
            [_ff_open createObj:foo atUri:@"/Foos"];
            STAssertNotNil([_ff_open metaDataForObj:foo], @"metadata not available");
            NSLog(@"metadata: %@",[[_ff_open metaDataForObj:foo] prettyPrint]);
        } else STAssertNotNil(_ff_open, @"no ff_open backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

#pragma mark - Sample data utilities

/*!
 Generates a random string of up to 1000 characters in length. Generates a random length up to 1000 if numCharacters is set to 0.
 */
- (NSString *)randomString:(int)numCharacters {
    //static char const possibleChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    static char const possibleChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
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
