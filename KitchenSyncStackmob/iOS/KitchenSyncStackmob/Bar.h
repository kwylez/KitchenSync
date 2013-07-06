//
//  Bar.h
//  KitchenSyncStackmob
//
//  Created by Cory D. Wiles on 6/2/13.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>


@interface Bar : NSManagedObject

@property (nonatomic, retain) NSString * barParameter;
@property (nonatomic, retain) NSDate * createddate;
@property (nonatomic, retain) NSDate * lastmoddate;
@property (nonatomic, retain) NSString * objectId;
@property (nonatomic, retain) NSString * sm_owner;
@property (nonatomic, retain) NSString * barId;

@end
