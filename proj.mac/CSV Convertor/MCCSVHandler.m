//
//  MCCSVHandler.m
//  Military Confrontation
//
//  Created by 江宇英 on 13-3-21.
//  Copyright (c) 2013年 Bullets in a Burning Box, Inc. All rights reserved.
//

#import "MCCSVHandler.h"
#import "JSONKit.h"

@interface MCCSVHandler ()
{
    NSMutableDictionary *root_;
}

@end

@implementation MCCSVHandler

- (id)init
{
    self = [super init];
    if (self) {
        root_ = [[NSMutableDictionary alloc] init];
    }
    
    return self;
}

- (void)dealloc
{
    [root_ release];
    [super dealloc];
}

+ (NSString *)filename
{
    return nil;
}

+ (NSString *)sourceFilename
{
    return nil;
}

- (void)willHandle
{
}

- (void)handle:(NSString *)aLine
{
    [self willHandleLine:aLine];
    [self handleLine:aLine];
    [self didHandleLine:aLine];
}

- (void)didHandle
{
}

- (void)willHandleLine:(NSString *)aLine
{
}

- (void)handleLine:(NSString *)aLine
{
}

- (void)didHandleLine:(NSString *)aLine
{
}

- (void)setObject:(NSObject *)anObject forKey:(NSString *)aKey
{
    [root_ setObject:anObject forKey:aKey];
}

- (void)writeTo:(NSString *)anOutputFilepath
{
    NSString *data = [root_ JSONString];
    [data writeToFile:anOutputFilepath atomically:YES encoding:NSUTF8StringEncoding error:nil];
}
    
@end
