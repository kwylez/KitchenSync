//
//  ParseConfigurator.h
//  KitchenSyncParse
//
//  Created by Cory D. Wiles on 6/2/13.
//

#import <Foundation/Foundation.h>

@interface StackMobConfigurator : NSObject

+ (StackMobConfigurator *)sharedInstance;
- (NSDictionary *)applicationConfiguration;

@end
