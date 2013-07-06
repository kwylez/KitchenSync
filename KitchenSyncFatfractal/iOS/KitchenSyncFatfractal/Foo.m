//
//  Foo.m
//  KitchenSink
//
//

#import "Foo.h"

@implementation Foo

- (NSString*) description {
    return [[NSString alloc]
            initWithFormat:@"Foo[fooParameter[%@]]",
            [self fooParameter]
            ];
}

@end
