//
//  ParseConfigurator.m
//  KitchenSyncParse
//
//  Created by Cory D. Wiles on 6/2/13.
//

#import "StackMobConfigurator.h"

@implementation StackMobConfigurator

+ (StackMobConfigurator *)sharedInstance {

  static StackMobConfigurator *_sharedInstance = nil;
  static dispatch_once_t oncePredicate;
  
  dispatch_once(&oncePredicate, ^{
    _sharedInstance = [[StackMobConfigurator alloc] init];
  });
  
  return _sharedInstance;
}

- (NSDictionary *)applicationConfiguration {
  
  static NSDictionary *configurationDict = nil;
  static dispatch_once_t oncePredicate;
  
  dispatch_once(&oncePredicate, ^{
    
    NSString *dataPath = [[NSBundle bundleForClass:[self class]] pathForResource:@"StackMobConfiguration"
                                                                          ofType:@"plist"];
    
    configurationDict = [NSDictionary dictionaryWithContentsOfFile:dataPath];
  });
  
  return configurationDict;
}

@end
