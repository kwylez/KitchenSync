/*
 * Copyright 2012-2013 StackMob
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 The following code was inspired by STLOAuthClient
 */

//
//  STLOAuthClient.h
//
//  Created by Marcelo Alves on 07/04/12.
//  Copyright (c) 2012 Some Time Left. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer. Redistributions in binary
//  form must reproduce the above copyright notice, this list of conditions and
//  the following disclaimer in the documentation and/or other materials
//  provided with the distribution. Neither the name of the Some Time Left nor
//  the names of its contributors may be used to endorse or promote products
//  derived from this software without specific prior written permission. THIS
//  SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//

#import <Foundation/Foundation.h>
#import "AFHTTPClient.h"

@interface SMOAuth1Client : AFHTTPClient

// designated initializer.
- (id) initWithBaseURL:(NSURL *)url consumerKey:(NSString *)consumerKey secret:(NSString *)consumerSecret;

- (void) setAccessToken:(NSString *)accessToken secret:(NSString *)secret;
- (void) setConsumerKey:(NSString *)consumerKey secret:(NSString *)secret;

@property (nonatomic) BOOL signRequests;
@property (nonatomic, copy) NSString *realm;
@property (copy,readonly) NSString *consumerKey;
@property (copy,readonly) NSString *consumerSecret;
@property (copy,readonly) NSString *tokenIdentifier;
@property (copy,readonly) NSString *tokenSecret;

- (NSURLRequest *) unsignedRequestWithMethod:(NSString *)method
                                        path:(NSString *)path
                                  parameters:(NSDictionary *)parameters;

- (NSURLRequest *) signedRequestWithMethod:(NSString *)method
                                      path:(NSString *)path
                                parameters:(NSDictionary *)parameters;

-(void)recordServerTimeDiffFromHeader:(NSString*)header;

@end
