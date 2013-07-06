<h2>StackMob iOS SDK Change Log</h2>

<h3>v1.4.0 - March 27, 2013</h3>

**Features**

* OR query support, allowing, for example, the ability to query "todo where A AND (B OR (C AND D) OR E)".  See <a href="http://developer.stackmob.com/tutorials/ios/Advanced-Queries" target="_blank">Advanced Query Tutorial</a> for all details.
* Support for querying where field equals / does not equal the empty string. Using core data, use [NSPredicate predicateWithFormat:@"field == ''"]. Using lower level datastore API, use [query where:field isEqualTo:@""].
* Add loginWithFacebookToken:createUserIfNeeded:usernameForCreate:onSuccess:onFailure:. Allows login to StackMob using Facebook credentials and optionally create a StackMob user if one doesn't already exist that is linked to the credentials used.
* Add loginWithTwitterToken:twitterSecret:createUserIfNeeded:usernameForCreate:onSuccess:onFailure:. Allows login to StackMob using Twitter credentials and optionally create a StackMob user if one doesn't already exist that is linked to the credentials used.
* Token unlinking for Facebook and Twitter - See <a href="http://stackmob.github.com/stackmob-ios-sdk/Classes/SMClient.html" target="_blank">SMClient class reference</a> for method definitions.
* Add SMClient / SMUserSession method setTokenRefreshFailureBlock:.  See <a href="http://stackmob.github.com/stackmob-ios-sdk/Classes/SMUserSession.html#//api/name/setTokenRefreshFailureBlock:" target="_blank">SMUserSession class reference</a> for method definition and examples.
* Every SMClient method now has an additional method definition which has parameter options for success and failure callback queues.  This is used a lot internally so callbacks are not run on the main thread, but is exposed so you can do the same.
* Update AFNetworking dependency to 1.1.0 for Xcode 4.6 support.
* Add Availability section to all methods in API reference.

**Fixes**

* Deprecate SMClient method loginWithFacebookToken:options:onSuccess:onFailure:, use loginWithFacebookToken:createUserIfNeeded:usernameForCreate:options:onSuccess:onFailure:.
* Deprecate SMClient method loginWithTwitterToken:twitterSecret:options:onSuccess:onFailure:, use loginWithTwitterToken:twitterSecret:createUserIfNeeded:usernameForCreate:options:onSuccess:onFailure:.

<h3>v1.3.0 - February 14, 2013</h3>

**Features**

* Support for saving and querying geo points through the Core Data integration.  Be sure to include the `CoreLocation` framework in your project.
* `SMGeoPoint` class for easily working with geo points through StackMob.
* `SMLocationManager` class for abstracting out `CLLocationManager` boiler plate code when working with geo data.
* Save and Fetch methods with `SMRequestOptions` parameter (options) to allow for custom options per Core Data request - See `NSManagedObjectContext+Concurrency.h` for method details.
* New `globalRequestOptions` property in `SMCoreDataStore` to set default request options used globally during StackMob calls from Core Data.
* Additional support for building source code with OSX targets (issue #34).
* Login support using Gigya credentials.

**Fixes**

* Fix in internal cache fetch method.
* Predicates with `NSDate` values works properly (issue #30).


<h3>v1.2.0 - January 24, 2013</h3>

All the details on the features of this release can be found in <a href="https://blog.stackmob.com/2013/01/ios-sdk-v1-2-0-released/">this blogpost</a>.

**Features**

* Caching system to allow for local fetching of objects which have previously been fetched from the server.  See SMCoreDataStore class reference for how to.
* Introduce additional methods for interacting with Core Data.  See new methods in SMCoreDataStore and NSManagedObjectContext+Concurrency class references.
* Update to internal network request algorithms for improved performance of Core Data saves and fetches.
* All NSDate attributes are saved on StackMob as integers in ms, rather than seconds.  This allows there not be a mismatch in translation when you also have attributes for createddate and lastmoddate.
* Every Datastore API method (SMDataStore class reference) now has an additional method definition which has parameter options for success and failure callback queues.  This is used a lot internally so callbacks are not run on the main thread, but is exposed so you can do the same.

**Fixes**
  
* Fix in updateTwitterStatusWithMessage:onSuccess:onFailure method.
* Fix in createUserWithFacebookToken:username:onSuccess:onFailure: to properly send request as a POST operation.
* Fix in encoding query string parameters when logged in to ensure proper request signature.

<h3>v1.1.3 - Nov 20, 2012</h3>

**Features**

* Integrate Push Notifications into core SDK.  Separate Push SDK still exists for those using StackMob only for push notifications.
* Expose [SMNetworkReachability](http://stackmob.github.com/stackmob-ios-sdk/Classes/SMNetworkReachability.html) interface for developers to manually monitor network status changes.
* Requests will return SMError with code -105 (SMNetworkNotReachable) when device has no network connection.
* Now dependent on SystemConfiguration and MobileCoreServices frameworks, make sure to add them to the "Link Binary With Libraries" section of your target's Build Phases as well as import them somewhere in your project (such as your .pch file).

**Fixes**

* Support nil success and failure blocks.
* Update to Core Data fetch algorithm to populate internal storage with retrieved attribute values.
* URL encode primary key values on read/update/delete to support special characters.
* Add expand depth support for queries.

<h3>v1.1.2 - Oct 29, 2012</h3>

**Features**

* Updated SMQuery Geopoint method to take distance in kilometers instead of meters.  New method is _where:isWithin:kilometersOf:_.
* Add _optionsWithExpandDepth:_ and _optionsWithReturnedFieldsRestrictedTo:_ methods to SMRequestOptions class. 
* Provide error message when creating an object with null values for fields.  If you receive an error which states that a field was sent with a value of null and the type cannot be inferred, simply go online to the Manage Schemas section of your StackMob Dashboard and manually add the field with correct data type.  This happens occasionally when working with number types in Core Data.

**Fixes**

* Allow setting of Core Data relationships to nil.
* Add proper SMRequestOptions headers dictionary initialization.   
* Change default merge policy of SMCoreDataStore _managedObjectContext_ property to NSMergeByPropertyObjectTrumpMergePolicy.  This translates to "client wins" when there are conflicts for particular objects.  Feel free to change the merge policy at any time by calling the _setMergePolicy:_ method on the managed object context of your SMCoreDataStore instance.

<h3>v1.1.1 - Oct 24, 2012</h3>

**Features**

* Full support for Core Data Boolean Data Types.  A Core Data Boolean attribute will map to a Boolean field on StackMob.
* Removal of 'sm_' prefix for NSManagedObject category helper methods.  Use [managedObject assignObjectId] and [managedObject primaryKeyField].
* Update SMClient user schema field property names. **userPrimaryKeyField** and **userPasswordField** describe the primary key and password field names for the user schema, respectively.

**Fixes**

* Update fetch request algorithm to support SMUserManagedObject change.
* Update Core Data object serialization algorithm.  Serialized dictionary sent in request now includes only fields which have been updated since insert or the last call to save.
* Update SMRequestOptions for proper headers dictionary initialization.

<h3>v1.1.0 - Oct 17, 2012</h3>

**Features**

* Removal of SMModel Protocol.  
* Addition of SMUserManagedObject. Your managed object subclass corresponding to user objects should inherit from this class.  SMUserManagedObject provides methods to securely set passwords for user objects without storing them in Core Data attributes. For all the information on how to update your current code [see this blogpost](http://blog.stackmob.com/?p=3547).
* Built for armv7 and armv7s architectures.

<h3>v1.0.1 - Oct 1, 2012</h3>

**Features**

* Can query whether fields are or are not nil. Thanks to combinatorial for the pull request.

**Fixes**

* Address error in serialization algorithm for one-to-one relationship camel cased attributes.
* Address error in request sent when reading from schemas with permissions set.

<h3>v1.0.0 - Sep 26, 2012</h3>

**Features**

* Support for iOS preferred camelCase Core Data property names.
* Support non case-sensitive schema names in datastore API.
* Support Core Data Date attribute type.
* API version and public key provided to SMClient during initialization must be correct format and non-nil.
* Core Data integration debug logging available by setting SM\_CORE\_DATA\_DEBUG = YES. See the Debugging section on the main page of the iOS SDK Reference for more info.

**Fixes**

* Edits to dictionary serialization algorithms for improved performance.
* NewValueForRelationship incremental store method correctly returns empty array for to-many with no objects.

<h3>v1.0.0beta.3 - Aug 24, 2012</h3>

**Fixes** 

* The method save: to the managed object context will return NO if StackMob calls fail.
* Fetch requests not returning errors.

<h3>v1.0.0beta.2 - Aug 20, 2012</h3>

**Features**

* Performing custom code methods is now available through the `SMCustomCodeRequest` class.
* Binary Data can be converted into an NSString using the `SMBinaryDataConversion` class and persisted to a StackMob field with Binary Data type.


<h3>v1.0.0beta.1 - Aug 10, 2012</h3>

**Features**

* Initial release of new and improved iOS SDK.  Core Data integration serves as the biggest change to the way developers interact with the SDK. See [iOS SDK v1.0 beta](https://www.stackmob.com/devcenter/docs/iOS-SDK-v1.0-beta) for more information.
