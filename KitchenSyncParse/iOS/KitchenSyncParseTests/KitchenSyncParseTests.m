//
//  KitchenSinkTests.m
//  KitchenSinkTests
//
//

#import "KitchenSyncParseTests.h"
#import "Foo.h"
#import "Bar.h"
#import "ParseConfigurator.h"

@interface KitchenSyncParseTests()

@property (nonatomic, assign) BOOL showLogs;
@property (nonatomic, assign) BOOL usingSecureBackend;

- (void)deleteAllTestData;
- (void)deleteAllFoo;
- (void)deleteAllBar;
- (NSString *)randomString:(int)numCharacters;
+ (void)initializeParse;

@end

@implementation KitchenSyncParseTests

#pragma mark - Setup and Init

/*!
 * You've got to call this method
 */
+ (void)initializeParse {
  
  NSDictionary *parseAppConfiguration = [[ParseConfigurator sharedInstance] applicationConfiguration];
  
  BOOL usingSecureBackend = [parseAppConfiguration[@"UseSecure"] boolValue];
  
  // Parse is a static class, so have to use two configs...
  if(usingSecureBackend) {
    [Parse setApplicationId:parseAppConfiguration[@"SecureApplicationId"]
                  clientKey:parseAppConfiguration[@"SecureClientKey"]];
  } else {
    [Parse setApplicationId:parseAppConfiguration[@"OpenApplicationId"]
                  clientKey:parseAppConfiguration[@"OpenClientKey"]];
  }
}

- (void)setUp {

    [super setUp];
    
    NSDictionary *parseAppConfiguration = [[ParseConfigurator sharedInstance] applicationConfiguration];

    _showLogs           = [parseAppConfiguration[@"ShowLogs"] boolValue];
    _usingSecureBackend = [parseAppConfiguration[@"UseSecure"] boolValue];

    [Foo registerSubclass];

    [self deleteAllTestData];
}

#pragma mark - Teardown

- (void)tearDown {
    [super tearDown];
    //[self deleteAllTestData];
}

/*!
 This method deletes all Test data from the backend.
 */
- (void) deleteAllTestData {
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
- (void) deleteAllFoo {
    @try {
        PFQuery *query = [PFQuery queryWithClassName:@"Foo"];
        NSArray *foos = [query findObjects];
        for (id obj in foos) {
            [obj delete];
        }
    } @catch (NSException * e) {
        NSLog(@"deleteAllFoo failed: %@", [e description]);
    }
}

/*!
 This method deletes all Bar objects from the backend.
 */
- (void) deleteAllBar {
    @try {
        PFQuery *query = [PFQuery queryWithClassName:@"Bar"];
        NSArray *bars = [query findObjects];
        for (id obj in bars) {
            [obj delete];
        }
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
        // create a foo object
        Foo * foo = [[Foo alloc] init];
        foo.fooParameter = @"Testing public access";
        if(_showLogs) NSLog(@"test_100_Public_Access Foo : foo before save = %@",
                           [foo description]);
        // create an object to make sure backend is really working
        NSError * crudError;
        [foo save: &crudError];
        STAssertNil(crudError, @"test failed: %@", [crudError description]);
        if(_showLogs) NSLog(@"test_100_Public_Access Foo : foo after save = %@",
                           [foo description]);
        // verify that the object can be accessed without authentication key
        NSString *fooId = [foo objectId];
        PFQuery *query = [PFQuery queryWithClassName:@"Foo"];
        //Note: you need to cast the query result to get back a Foo
        Foo * publicFoo = (Foo *)[query getObjectWithId:fooId error:&crudError];
        STAssertNil(crudError, @"test failed: %@", [crudError description]);
        if(_showLogs) NSLog(@"test_100_Public_Access Foo : publicFoo after read = %@",
                           [publicFoo description]);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to require secure access to access data.
 */
- (void)test_101_Secure_Access {
  
    [KitchenSyncParseTests initializeParse];
  
    @try {
        // create a foo object
        Foo * foo = [[Foo alloc] init];
        foo.fooParameter = @"Testing secure access";
        if(_showLogs) NSLog(@"test_101_Secure_Access Foo : foo before save = %@",
                           [foo description]);
        // create an object to make sure backend is really working
        NSError * crudError;
        [foo save: &crudError];
        STAssertNil(crudError, @"test failed: %@", [crudError description]);
        // verify that the object cannot be accessed without authentication
        NSString *fooId = [foo objectId];
        PFQuery *query = [PFQuery queryWithClassName:@"Foo"];
        Foo * publicFoo = (Foo *)[query getObjectWithId:fooId error:&crudError];
        STAssertNil(crudError, @"test failed: %@", [crudError description]);
        if(_showLogs) NSLog(@"test_101_Secure_Access localFoo : foo after save = %@",
                           [publicFoo description]);
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
        STFail(@"Parse does not support collections.");
}

/*!
 * Tests that a backend with a locked schema will not allow adding a new Collection automatically.
 * Since we are not able to talk to multiple backends, we will use the newCollectionsAllowed
 * setting to run the test if false, and skip if true.
 */
- (void)test_103_New_Class_Disallowed {
        STFail(@"Parse does not support collections.");
}

/*!
 * Tests the ability for a schemaless backend to learn a new Class automatically.
 * Since we are not able to talk to multiple backends, we will use the newClassesAllowed
 * setting to run the test if true, and skip if false.
 */
- (void)test_104_New_Collection_Allowed {

    [KitchenSyncParseTests initializeParse];
  
    @try {
        if(!_usingSecureBackend) {
            NSString * clazz = [self randomString:5];
            PFObject * obj = [PFObject objectWithClassName:clazz];
            [obj setValue:@"Some String in a PFObject with ClassName Foo"
                   forKey:@"newParameter"];
            if(_showLogs) NSLog(@"test_104_New_Class_Allowed PFObject * obj before save = %@",
                               [obj description]);
            NSError * crudError;
            [obj save: &crudError];
            STAssertNil(crudError, @"test failed: %@",
                        [crudError description]);
            if(_showLogs) NSLog(@"test_104_New_Class_Allowed Foo : foo after save = %@",
                               [obj description]);
        } else if(_showLogs)
            NSLog(@"test_104_New_Class_Allowed could not be run with this configuration, skipping test");
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
  
    [KitchenSyncParseTests initializeParse];
  
    @try {
        if(_usingSecureBackend) {
            NSString * clazz = [self randomString:5];
            PFObject * obj = [PFObject objectWithClassName:clazz];
            [obj setValue:@"Some String in a PFObject with ClassName Foo"
                   forKey:@"newParameter"];
            if(_showLogs) NSLog(@"test_105_New_Class_Disallowed PFObject * obj before save = %@",
                               [obj description]);
            NSError * crudError;
            [obj save: &crudError];
            STAssertNotNil(crudError, @"test failed: %@", [crudError description]);
            if(!crudError)
                STFail(@"Should not have been able to create object to backend.");
        } else if(_showLogs)
            NSLog(@"test_105_New_Class_Disallowed could not be run with this configuration, skipping test");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to deploy and access a local development backend for from the client.
 */
- (void)test_106_Local_Backend {
    @try {
        if(_parse_local) {
        } else STAssertNotNil(_parse_local, @"no local backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to access multiple backends from the client.
 */
- (void)test_107_Multiple_Backends {
  
    [KitchenSyncParseTests initializeParse];
  
    @try {
        Foo * f1 = [[Foo alloc] init];
        f1.fooParameter = @"Some string in a Foo : NSObject object";
        if(_showLogs) NSLog(@"test_101_Multiple_Backends Foo : f1 before save = %@",
                           [f1 description]);
        // create the object
        NSError * crudError;
        [f1 save: &crudError];
        STAssertNil(crudError, @"test failed: %@", [crudError localizedDescription]);
        if(_showLogs) NSLog(@"test_101_Multiple_Backends localFoo : f1 after save = %@",
                           [f1 description]);
        if(_parse_secure) {
            Foo * f2 = [[Foo alloc] init];
            f2.fooParameter = @"Some string in a Foo : NSObject object";
            if(_showLogs) NSLog(@"test_101_Multiple_Backends Foo : f2 before save = %@",
                               [f2 description]);
            // create the object
            crudError = nil;
            [f2 save: &crudError];
            STAssertNil(crudError, @"test failed: %@", [crudError localizedDescription]);
            if(_showLogs) NSLog(@"test_101_Multiple_Backends Foo : f2 after save = %@",
                               [f2 description]);
        } else STAssertNotNil(_parse_secure, @"no additional backend");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to access a metadata description of your backend.
 */
- (void)test_108_API_Metadata {
    STFail(@"Parse does not provide API metadata.");
}


#pragma mark - Object Construction tests

/*!
 * Tests the ability to create an object from a defined object class without having to create a client schema.
 */
- (void)test_200_Foo_ObjCClass {
  
    [KitchenSyncParseTests initializeParse];
  
    @try {
        Foo * foo = [[Foo alloc] init];
        foo.fooParameter = @"Foo : NSObject object";
        // or, if you prefer
        [foo setFooParameter:@"Foo : NSObject object"];
        if(_showLogs) NSLog(@"test_200_Foo_ObjCClass Foo : foo before save = %@",
                           [foo description]);
        // create the object
        NSError * crudError;
        [foo save: &crudError];
        if(_showLogs) NSLog(@"test_200_Foo_ObjCClass Foo : foo after save = %@",
                           [foo description]);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to create an object without having to write a class file or create a client schema.
 */
- (void)test_201_Foo_Schemaless {

    [KitchenSyncParseTests initializeParse];
  
    @try {
        PFObject * foo = [PFObject objectWithClassName:@"Foo"];
        [foo setValue:@"PFObject with ClassName Foo" forKey:@"fooParameter"];
        if(_showLogs) NSLog(@"test_201_Foo_Schemaless Foo * foo before save = %@",
                           [foo description]);
        // create the object
        NSError * crudError;
        [foo save: &crudError];
        STAssertNil(crudError, @"test failed: %@", [crudError localizedDescription]);
        if(_showLogs) NSLog(@"test_201_Foo_Schemaless Foo : foo after save = %@",
                           [foo description]);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests the ability to create an object with a defined client class for a defined client schema.
 ` */
- (void)test_202_Bar_NSManagedObject_SubClass {
    STFail(@"Parse does not support Core Data");
}

/*!
 * Tests the ability to create an object for a defined client schema without having to write a class file .
 */
- (void)test_203_NSManagedObject_Called_Bar {
    STFail(@"Parse does not support Core Data");
}

/*!
 * Tests that a backend will add a new member propertry of a Class definition
 * to be added by the client if allowed by configuration.
 * Since we are not able to talk to multiple backends, we will use the newMembersAllowed
 * setting to run the test if true, and skip if false.
 */
- (void)test_204_New_Member_Allowed {
  
    [KitchenSyncParseTests initializeParse];

    @try {
        if(!_usingSecureBackend) {
            NSString * memberName = [self randomString:5];
            PFObject * foo = [PFObject objectWithClassName:@"Foo"];
            [foo setValue:@"PFObject with ClassName Foo" forKey:@"fooParameter"];
            [foo setValue:@"a schemaless addition" forKey:memberName];
            if(_showLogs) NSLog(@"test_204_New_Member_Allowed NSDictionary : foo before save = %@",
                               [foo description]);
            NSError * crudError;
            [foo save: &crudError];
            STAssertNil(crudError, @"test failed: %@",
                        [crudError localizedDescription]);
            if(_showLogs) NSLog(@"test_204_New_Member_Allowed Foo : foo after save = %@",
                               [foo description]);
        } else if(_showLogs)
            NSLog(@"test_204_New_Member_Allowed could not be run with this configuration, skipping test");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests that a backend will not allow a new member propertry of a Class definition
 * to be added by the client if disallowed by configuration.
 * Since we are not able to talk to multiple backends, we will use the newMembersAllowed
 * setting to run the test if false, and skip if true.
 */
- (void)test_205_New_Member_Disallowed {

  [KitchenSyncParseTests initializeParse];
  
    @try {
        if(_usingSecureBackend) {
            NSString * memberName = [self randomString:5];
            PFObject * foo = [PFObject objectWithClassName:@"Foo"];
            [foo setValue:@"PFObject with ClassName Foo" forKey:@"fooParameter"];
            [foo setValue:@"a schemaless addition" forKey:memberName];
            if(_showLogs) NSLog(@"test_205_New_Member_Disallowed NSDictionary : foo before save = %@",
                               [foo description]);
            NSError * crudError;
            [foo save: &crudError];
            STAssertNotNil(crudError, @"should have gotten an error: %@",
                           [crudError description]);
            if(!crudError)
                STFail(@"Should not have been able to create object to backend ");
        } else if(_showLogs)
            NSLog(@"test_205_New_Member_Disallowed could not be run with this configuration, skipping test");
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests if and what additional information about your object (metadata) is provided.
 */
- (void)test_206_Metadata_Provided_ObjC_Class {

  [KitchenSyncParseTests initializeParse];
  
    @try {
        Foo * foo = [[Foo alloc] init];
        foo.fooParameter = @"Foo : NSObject object";
        if(_showLogs) NSLog(@"\r\n\t test_206_Metadata_Provided f1 before save = %@",[foo description]);
        // create the object
        NSError * crudError;
        [foo save: &crudError];
        STAssertNil(crudError, @"test failed: %@",
                    [crudError localizedDescription]);
        STAssertNotNil(foo.parseClassName, @"no metadata: %@",
                       foo.parseClassName);
        NSLog(@"metadata parseClassName: %@",foo.parseClassName);
        STAssertNotNil(foo.objectId, @"no metadata: %@",
                       foo.objectId);
        NSLog(@"metadata objectId: %@",foo.objectId);
        STAssertNotNil(foo.updatedAt, @"no metadata: %@",
                       foo.updatedAt);
        NSLog(@"metadata updatedAt: %@",foo.updatedAt);
        STAssertNotNil(foo.createdAt, @"no metadata: %@",
                       foo.createdAt);
        NSLog(@"metadata createdAt: %@",foo.createdAt);
        STAssertNotNil(foo.ACL, @"no metadata: %@",
                       foo.ACL);
        NSLog(@"metadata ACL: %@",foo.ACL);
    } @catch (NSException * e) {
        STAssertNil(e, @"test failed: %@", [e description]);
    }
}

/*!
 * Tests if and what additional information about your object (metadata) is provided.
 */
- (void)test_208_Metadata_Provided_CoreData {
    STFail(@"Parse does not support Core Data");
}

#pragma mark - Sample data utilities

/*!
 Generates a random string of up to 1000 characters in length. Generates a random length up to 1000 if numCharacters is set to 0.
 */
-(NSString *)randomString:(int)numCharacters {
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
