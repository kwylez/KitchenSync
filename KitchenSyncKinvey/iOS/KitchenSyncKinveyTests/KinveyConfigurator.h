//
//  ParseConfigurator.h
//  KitchenSyncParse
//
//  Created by Cory D. Wiles on 6/2/13.
//

#import <Foundation/Foundation.h>

@interface KinveyConfigurator : NSObject

+ (KinveyConfigurator *)sharedInstance;
- (NSDictionary *)applicationConfiguration;

@end
