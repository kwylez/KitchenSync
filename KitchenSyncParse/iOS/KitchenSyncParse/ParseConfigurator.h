//
//  ParseConfigurator.h
//  KitchenSyncParse
//
//  Created by Cory D. Wiles on 6/2/13.
//

#import <Foundation/Foundation.h>

@interface ParseConfigurator : NSObject

+ (ParseConfigurator *)sharedInstance;
- (NSDictionary *)applicationConfiguration;

@end
