//
//  ParseConfigurator.m
//  KitchenSyncParse
//
//  Created by Cory D. Wiles on 6/2/13.
//

#import "ParseConfigurator.h"

@implementation ParseConfigurator

+ (ParseConfigurator *)sharedInstance {

  static ParseConfigurator *_sharedInstance = nil;
  static dispatch_once_t oncePredicate;
  
  dispatch_once(&oncePredicate, ^{
    _sharedInstance = [[ParseConfigurator alloc] init];
  });
  
  return _sharedInstance;
}

- (NSDictionary *)applicationConfiguration {
  
  static NSDictionary *configurationDict = nil;
  static dispatch_once_t oncePredicate;
  
  dispatch_once(&oncePredicate, ^{
    
    NSString *dataPath = [[NSBundle mainBundle] pathForResource:@"ParseConfiguration"
                                                         ofType:@"plist"];
    
    configurationDict = [NSDictionary dictionaryWithContentsOfFile:dataPath];
  });
  
  return configurationDict;
}

@end
