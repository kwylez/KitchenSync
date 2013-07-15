//
//  KitchenSyncAzureTests.m
//  KitchenSyncAzureTests
//
//  Created by Cory Wiles on 7/10/13.
//  Copyright (c) 2013 Cory Wiles. All rights reserved.
//

#import "KitchenSyncAzureTests.h"
#import "AzureConfigurator.h"
#import "Foo.h"
#import "Bar.h"

@interface KitchenSyncAzureTests()

@property (nonatomic, assign) BOOL showLogs;
@property (nonatomic, copy) void (^testForCompletionBlock)(BOOL *testCompleted);
@property (nonatomic, strong) MSClient *azureClientSecure;
@property (nonatomic, strong) MSClient *azureClientOpen;

- (void)deleteAllTestData;
- (void)deleteAllFoo;
- (void)deleteAllBar;
- (NSString *)randomString:(int)numCharacters;
- (void)initializeAzure;
- (void)setupCoreDataEnvironment;

@end

@implementation KitchenSyncAzureTests

- (void)initializeAzure {
  
  NSDictionary *azureAppConfiguration = [[AzureConfigurator sharedInstance] applicationConfiguration];
    
  _azureClientSecure = [MSClient clientWithApplicationURLString:azureAppConfiguration[@"SecureURL"]
                                               applicationKey:azureAppConfiguration[@"SecureApplicationId"]];


  _azureClientOpen   = [MSClient clientWithApplicationURLString:azureAppConfiguration[@"OpenURL"]
                                                 applicationKey:azureAppConfiguration[@"OpenApplicationId"]];
}

- (void)setUp {

  [super setUp];
  
  _testForCompletionBlock = ^(BOOL *testCompleted) {
    
    while (!*testCompleted) {
      
      NSDate *cycle = [NSDate dateWithTimeIntervalSinceNow:0.01];
      
      [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
                               beforeDate:cycle];
    }
  };
  
  NSDictionary *azureAppConfiguration = [[AzureConfigurator sharedInstance] applicationConfiguration];
  
  _showLogs = [azureAppConfiguration[@"ShowLogs"] boolValue];

  [self initializeAzure];
  [self deleteAllTestData];
}

- (void)tearDown {  
  [super tearDown];
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
 This method deletes all Foo objects from the backend.
 */
- (void)deleteAllFoo {

  @try {

  } @catch (NSException * e) {
    NSLog(@"deleteAllFoo failed: %@", [e description]);
  }
}

/*!
 This method deletes all Bar objects from the backend.
 */
- (void)deleteAllBar {

  @try {

  } @catch (NSException * e) {
    NSLog(@"deleteAllBar failed: %@", [e description]);
  }
}

#pragma mark - Setup and configuration tests

/*!
 * Tests the ability to access data without authentication key if allowed.
 */
- (void)test_100_Public_Access {
  
  @try {

    __block BOOL testCompleted = NO;
    __block NSDictionary *returnedFooDictionary = nil;

    // create a foo object
    NSDictionary *item = @{@"fooParameter" : @"Testing public access"};

    MSTable *fooTable = [_azureClientOpen tableWithName:@"Foo"];
    
    [fooTable insert:item completion:^(NSDictionary *insertedItem, NSError *error) {

      testCompleted         = YES;
      returnedFooDictionary = insertedItem;
      
      STAssertNil(error, @"test failed: %@", [error description]);

      if (_showLogs) {
        NSLog(@"test_100_Public_Access Foo : foo after save = %@", [insertedItem description]);
      }
    }];
    
    _testForCompletionBlock(&testCompleted);

    // verify that the object can be accessed without authentication key
    NSString *fooId            = [NSString stringWithFormat:@"(id eq %@)", returnedFooDictionary[@"id"]];
    NSString *escapedUrlString = [fooId stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    NSString *filterURL        = [NSString stringWithFormat:@"%@/tables/Foo?$filter=%@", _azureClientOpen.applicationURL, escapedUrlString];

    __block BOOL readTestCompleted = NO;
    
    [NSURLConnection sendAsynchronousRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:filterURL]]
                                       queue:[NSOperationQueue mainQueue]
                           completionHandler:^(NSURLResponse *response, NSData *data, NSError *error) {

                             readTestCompleted = YES;
                             
                             STAssertNil(error, @"test failed: %@", [error description]);
                             
                             if (_showLogs) {

                               id responseDiction = [NSJSONSerialization JSONObjectWithData:data
                                                                                    options:NSJSONReadingMutableContainers
                                                                                      error:nil];
                               
                               NSLog(@"test_100_Public_Access Foo : publicFoo after read = %@", [responseDiction description]);
                             }
                           }];

    _testForCompletionBlock(&readTestCompleted);

  } @catch (NSException * e) {
   
    STAssertNil(e, @"test failed: %@", [e description]);
  }
}

- (void)test_101_Secure_Access {
  STFail(@"See report for further explaination.");
}

/*!
 * Tests the ability for a schemaless backend to learn a new Collection automatically.
 * Since we are not able to talk to multiple backends, we will use the newClassesAllowed
 * setting to run the test if true, and skip if false.
 */
- (void)test_102_New_Collection_Allowed {
  STFail(@"Azure does not support collections.");
}

/*!
 * Tests that a backend with a locked schema will not allow adding a new Collection automatically.
 * Since we are not able to talk to multiple backends, we will use the newClassesAllowed
 * setting to run the test if false, and skip if true.
 */
- (void)test_103_New_Collection_Disallowed {
  STFail(@"Azure does not support collections.");
}

/*!
 * Tests the ability for a schemaless backend to learn a new Collection automatically.
 * Since we are not able to talk to multiple backends, we will use the newCollectionsAllowed
 * setting to run the test if true, and skip if false.
 */
- (void)test_104_New_Class_Allowed {
  STFail(@"See report for further explaination.");
}

/*!
 * Tests that a backend with a locked schema will not allow a new Collection
 * to be added by the client if disallowed by configuration.
 */
- (void)test_105_New_Class_Disallowed {
  
  @try {
    
    __block BOOL testCompleted = NO;
    __block NSDictionary *returnedFooDictionary = nil;
    
    NSString *class = [self randomString:5];
    NSDictionary *randomColumn = @{@"randomClassColumnName": [self randomString:10]};
    
    if (_showLogs) {
      NSLog(@"\r\n\t test_103_New_Class_Disallowed random class before save = %@",[randomColumn description]);
    }
    
    MSTable *randomTable = [_azureClientSecure tableWithName:class];
    
    [randomTable insert:randomColumn completion:^(NSDictionary *result, NSError *error) {
      
      testCompleted         = YES;
      returnedFooDictionary = result;
      
      STAssertNotNil(error, @"test failed: %@", [error description]);
    }];
    
    _testForCompletionBlock(&testCompleted);
    
    if (_showLogs) {
      NSLog(@"\r\n\t test_102_New_Class_Allowed random class before save = %@, \r\n\tMetadata isn't supported with Azure", [returnedFooDictionary description]);
    }
    
    STAssertNotNil(self.azureClientOpen, @"no open backend");
    
  } @catch (NSException * e) {
    STAssertNil(e, @"test failed: %@", [e description]);
  }
}

- (void)test_106_Local_Backend {
  STFail(@"Azure does not support local engine.");
}

/*!
 * Tests the ability to access multiple backends from the client SDK.
 */
- (void)test_107_Multiple_Backends {
 STFail(@"Azure does not support multiple backends. See report for further explaination.");
}

/*!
 * Tests the ability to access a metadata description of your backend.
 */
- (void)test_108_API_Metadata {
  STFail(@"Azure does not provide API metadata.");
}

#pragma mark - Object Construction tests

/*!
 * Tests the ability to create an object from a defined object class without 
 * having to create a client schema.
 */
- (void)test_200_Foo_ObjCClass {
  STFail(@"Azure does not provide support for native objects");
}

/*!
 * Tests the ability to create an object without having to write a class file 
 * or create a client schema.
 */
- (void)test_201_Foo_Schemaless {
  
  __block BOOL testCompleted = NO;
  
  @try {

    if (_azureClientOpen) {
      
      // create a foo object
      NSDictionary *item = @{@"fooParameter" : @"test_201_Foo_Schemaless foo string"};
      
      if (_showLogs) {
        NSLog(@"\r\n\t test_201_Foo_Schemaless f1 before save = %@", [item description]);
      }
      
      MSTable *fooTable = [_azureClientOpen tableWithName:@"Foo"];
      
      [fooTable insert:item completion:^(NSDictionary *insertedItem, NSError *error) {
        
        testCompleted = YES;
        
        STAssertNil(error, @"test failed: %@", [error description]);
        
        if (_showLogs) {
          NSLog(@"\r\n\t test_201_Foo_Schemaless f1 after save = %@, \r\n\tMetadata isn't support by Azure", [insertedItem description]);
        }
      }];
      
      _testForCompletionBlock(&testCompleted);

    } else {
      STAssertNotNil(_azureClientOpen, @"no _azureClientOpen backend");
    }
  } @catch (NSException * e) {
    STAssertNil(e, @"test failed: %@", [e description]);
  }
}

/*!
 * Tests the ability to create an object with a defined client class for a 
 * defined client schema.
 */
- (void)test_202_Bar_NSManagedObject_SubClass {
  STFail(@"Azure does not provide support for nsmanagedobject subclasses.");
}

/*!
 * Tests the ability to create an object for a defined client schema without 
 * having to write a class file.
 */
- (void)test_203_NSManagedObject_Called_Bar {
  STFail(@"Azure does not provide support for nsmanagedobject subclasses.");
}

/*!
 * Tests that a backend will add a new member propertry of a Class definition
 * to be added by the client if allowed by configuration.
 */
- (void)test_204_New_Member_Allowed {
  STFail(@"Azure does not provide support for nsmanagedobject subclasses. See report for further explaination.");
}

/*!
 * Tests that a backend will not allow a new member propertry of a Class definition
 * to be added by the client if disallowed by configuration.
 */
- (void)test_205_New_Member_Disallowed {
  STFail(@"See report for further explaination.");
}

/*!
 * Tests if and what additional information about your object (metadata) is provided
 */
- (void)test_206_Metadata_Provided {
  STFail(@"See report for further explaination.");
}

#pragma mark - Sample data utilities

/*!
 Generates a random string of up to 1000 characters in length. Generates a random length up to 1000 if numCharacters is set to 0.
 */
- (NSString *)randomString:(int)numCharacters {
  
  static char const possibleChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  int len;
  
  if (numCharacters > 1000 || numCharacters == 0) {
    len = (int)rand() % (1000);
  } else {
    len = numCharacters;
  }
  
  unichar characters[len];
  
  for( int i = 0; i < len; ++i) {
    characters[i] = possibleChars[arc4random_uniform(sizeof(possibleChars) - 1)];
  }

  return [NSString stringWithCharacters:characters length:len] ;
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

@end
