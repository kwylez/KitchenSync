//
//  Foo.h
//  KitchenSyncKinvey
//
//

#import <Foundation/Foundation.h>
#import <KinveyKit/KinveyKit.h>

@interface Foo : NSObject <KCSPersistable>

@property (nonatomic, retain) NSString * fooParameter;

@property (nonatomic, copy)   NSString *    kinveyObjectId; //Kinvey entity _id
@property (nonatomic, retain) KCSMetadata * metadata;  //Kinvey metadata, optional

@end
