//
//  ParseConfigurator.h
//  KitchenSyncParse
//
//  Created by Cory D. Wiles on 6/2/13.
//

#import <Foundation/Foundation.h>

@interface AzureConfigurator : NSObject

+ (AzureConfigurator *)sharedInstance;
- (NSDictionary *)applicationConfiguration;

@end
