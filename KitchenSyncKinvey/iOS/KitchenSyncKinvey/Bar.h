//
//  Bar.h
//  KitchenSyncKinvey
//
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
#import <KinveyKit/KinveyKit.h>

@interface Bar : NSManagedObject <KCSPersistable>

@property (nonatomic, retain) NSString * barParameter;
@property (nonatomic, retain) NSString * kinveyObjectId;

@end
