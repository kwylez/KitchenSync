//
//  KitchenSyncAzureTests.h
//  KitchenSyncAzureTests
//
//  Created by Cory Wiles on 7/10/13.
//  Copyright (c) 2013 Cory Wiles. All rights reserved.
//

#import <SenTestingKit/SenTestingKit.h>
#import <WindowsAzureMobileServices/WindowsAzureMobileServices.h>

@interface KitchenSyncAzureTests : SenTestCase

@property(nonatomic, strong) NSManagedObjectModel *managedObjectModel;
@property(nonatomic, strong) NSPersistentStoreCoordinator *persistentStoreCoordinator;
@property(nonatomic, strong) NSManagedObjectContext *managedObjectContext;

@end
