//
//  ParseConfigurator.m
//  KitchenSyncParse
//
//  Created by Cory D. Wiles on 6/2/13.
//

#import "KinveyConfigurator.h"

@implementation KinveyConfigurator

+ (KinveyConfigurator *)sharedInstance {

  static KinveyConfigurator *_sharedInstance = nil;
  static dispatch_once_t oncePredicate;
  
  dispatch_once(&oncePredicate, ^{
    _sharedInstance = [[KinveyConfigurator alloc] init];
  });
  
  return _sharedInstance;
}

- (NSDictionary *)applicationConfiguration {
  
  static NSDictionary *configurationDict = nil;
  static dispatch_once_t oncePredicate;
  
  dispatch_once(&oncePredicate, ^{
    
    NSString *dataPath = [[NSBundle bundleForClass:[self class]] pathForResource:@"KinveyConfiguration"
                                                                          ofType:@"plist"];
    
    configurationDict = [NSDictionary dictionaryWithContentsOfFile:dataPath];
  });
  
  return configurationDict;
}

@end
