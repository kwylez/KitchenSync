//
//  ParseConfigurator.m
//  KitchenSyncParse
//
//  Created by Cory D. Wiles on 6/2/13.
//

#import "AzureConfigurator.h"

@implementation AzureConfigurator

+ (AzureConfigurator *)sharedInstance {

  static AzureConfigurator *_sharedInstance = nil;
  static dispatch_once_t oncePredicate;
  
  dispatch_once(&oncePredicate, ^{
    _sharedInstance = [[AzureConfigurator alloc] init];
  });
  
  return _sharedInstance;
}

- (NSDictionary *)applicationConfiguration {
  
  static NSDictionary *configurationDict = nil;
  static dispatch_once_t oncePredicate;
  
  dispatch_once(&oncePredicate, ^{

    NSString *dataPath = [[NSBundle bundleForClass:[self class]] pathForResource:@"AzureConfiguration"
                                                                          ofType:@"plist"];
    
    configurationDict = [NSDictionary dictionaryWithContentsOfFile:dataPath];
  });
  
  return configurationDict;
}

@end
