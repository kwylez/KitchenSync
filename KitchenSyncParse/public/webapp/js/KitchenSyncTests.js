//
//  KitchenSyncTests
//

QUnit.config.autostart = false;

/*!
 * A bool value that controls whether certain log messages are printed out or not during these tests.
 */
var showLogs = true;

/*!
 * Since we are not able to talk to multiple backends, we will manually set a bool
 * value that controls how tests check whether the backend in use is set up with
 * "open" configuration options or "secured". Toggle this and run again for a
 * complete set of tests
 */
var parseSetSecure = false;

function setUpKitchenSyncTests() {
    var backendEl = document.getElementById('backend');
    var be = sessionStorage.getItem("backend");
    if (be && be == "secure") {
        parseSetSecure = true;
        backendEl.innerHTML = "Currently using secure backend, click to switch to open";
    } else if (be && be == "open") {
        parseSetSecure = false;
        backendEl.innerHTML = "Currently using open backend, click to switch to secure";
    }
    configureBackend();
}

function toggleBackend() {
    var backendEl = document.getElementById('backend');
    var be = sessionStorage.getItem("backend");
    if (be && be == "secure") {
    	sessionStorage.setItem("backend", "open");
        parseSetSecure = false;
        backendEl.innerHTML = "Currently using open backend, click to switch to secure";
    } else if (be && be == "open") {
    	sessionStorage.setItem("backend", "secure");
        parseSetSecure = true;
        backendEl.innerHTML = "Currently using secure backend, click to switch to open";
    } else {
        console.error("could not toggle backend mode");
    	sessionStorage.setItem("backend", "secure");
        parseSetSecure = true;                             
        backendEl.innerHTML = "Using secure backend";
    }
    console.log("backend is now " + sessionStorage.getItem("backend"));
    location.reload();
}

/*!
 * Parse uses client "collections" that are mapped to classes. We need to create
 * a Parse.Collection for each model object we use. This creates a Foos collection
 * for the Foo model.
 */
var Foos = Parse.Collection.extend({
    // Reference to this collection's model.
    model: Foo,
});

/*!
 * Parse uses client "collections" that are mapped to classes. We need to create
 * a Parse.Collection for each model object we use. This creates a Bars collection
 * for the Bar model.
 */
var Bars = Parse.Collection.extend({
    // Reference to this collection's model.
    model: Bar,
});

/*!
 * Setting up the backend and configuration.
 * Since we are not able to talk to multiple backends, we will initalize one of two 
 * backends based on the value of manually set a bool value parseSetSecure that 
 * will control which tests will be run.depending if the require the "open" 
 * configuration options or "secured". Toggle this and run again for a complete 
 * set of tests
 */
function configureBackend() {
    if(parseSetSecure) {
        Parse.initialize(
            "fY9ovQib3b9MSdUQmowuONsS1UakYc1hAlfVJHq3",
            "S2UCUYcbmL4atq8EHrqB4ixxPEmfDu0XG52EPf9G"
    	);
    } else {
        Parse.initialize(
            "dZo6lFDxQgJBd4vz9msg3oxbude3k45ZlsT6Cx6z",
    	    "jqMQI8dlcEWrAz0368a57qPogqZ0L6JJyPRTlhAc"
    	);
    }
    QUnit.start();
}

/*!
 * Tests the ability to access data without API authentication if allowed.
 */
asyncTest("test_100_Public_Access", function() {
    ok(false, "Parse does not support access without client keys");
    start();
});

/*!
 * Tests the ability to require secure access to access data.
 */
asyncTest("test_101_Secure_Access", function() {
    if(parseSetSecure) {
        ok(parseSetSecure, "backend is set to secured");
        // create a new Foo() object
        var f1 = new Foo();
        f1.set("fooParameter", "test_101_Secure_Access");
        if(showLogs) 
            console.log("\r\n\t test_101_Secure_Access f1 before save = " + 
                JSON.stringify(f1));
        // create an object to make sure backend is really working
        f1.save(null, {
            success: function(f2) {
                ok(f2 !== null, "f2 is " + JSON.stringify(f2));
                f2.fetch({
                    success: function(f3) {
                        ok(f3 !== null, "f3 is " + JSON.stringify(f3));
                        if(showLogs) 
                            console.log(
                                "\r\n\t test_101_Secure_Access f3 after save = " + 
                                JSON.stringify(f3));
                        start(); 
                    },
                    error: function(obj, msg) {
                        ok(false, "Error reading object: " + 
                            JSON.stringify(obj) + ", error message: " + msg.message);
                        start(); 
                    }
                });
            },
            error: function(obj, msg) {
                ok(false, "Error saving object: " + 
                    JSON.stringify(obj) + ", error message: " + msg.message);
                start(); 
            }
        });
    } else {
        ok(false, "backend is not set to secured, skipping test");
        start();
    }
});

/*!
 * Tests the ability for a schemaless backend to learn a new Collection automatically.
 */
asyncTest("test_102_New_Collection_Allowed", function() {
    if(!parseSetSecure) {
        ok(!parseSetSecure, "backend is set to not secured");
        var clazz = randomString(5);
        var coll = clazz + "s";
        var cs = "test_102_New_Collection_Allowed clazz " + clazz + " collection " + coll;
        var obj1 = new Parse.Object(coll);
        obj1.set("someParameter",cs);
        if(showLogs) 
            console.log(
               "\r\n\t test_102_New_Collection_Allowed obj1  = " + JSON.stringify(obj1));
        obj1.save(null, {
            success: function(obj2) {
                if(showLogs) 
                    console.log(
                        "\r\n\t test_102_New_Collection_Allowed obj2  = " + JSON.stringify(obj2));
                obj2.fetch({
                    success: function(obj3) {
                        ok(obj3 !== null, "obj3 is " + JSON.stringify(obj3));
                        if(showLogs) 
                            console.log(
                                "\r\n\t test_102_New_Collection_Allowed obj3 after save = " + 
                                JSON.stringify(obj3));
                        start(); 
                    },
                    error: function(obj, msg) {
                        ok(false, "Error reading object: " + 
                            JSON.stringify(obj) + ", error message: " + JSON.stringify(msg));
                        start(); 
                    }
                });
            },
            error: function(obj, msg) {
                ok(false, "Error saving object: " + 
                    JSON.stringify(obj) + ", error message: " + JSON.stringify(msg));
                start(); 
            }
        });            
    } else {
        ok(false, "backend is set to secured, skipping test");
        start();
    }
});

/*!
 * Tests that a backend with a locked schema will not allow a new Collection
 * to be added by the client if disallowed by configuration.
 */
asyncTest("test_103_New_Collection_Disallowed", function() {
    if(parseSetSecure) {
        ok(parseSetSecure, "backend is set to secured");
        var clazz = randomString(5);
        var coll = clazz + "s";
        var cs = "test_103_New_Collection_Disallowed clazz " + clazz + " collection " + coll;
        var obj1 = new Parse.Object(coll);
        obj1.set("someParameter",cs);
        if(showLogs) 
            console.log(
               "\r\n\t test_103_New_Collection_Disallowed obj1  = " + JSON.stringify(obj1));
        obj1.save(null, {
            success: function(obj2) {
                if(showLogs) 
                    console.log(
                        "\r\n\t test_103_New_Collection_Disallowed obj2  = " + JSON.stringify(obj2));
                ok(false, "Should not have allowed this " + JSON.stringify(obj2));
                obj2.fetch({
                    success: function(obj3) {
                    	//TODO - how to get the collection from the object
                        ok(false, "Should not have allowed this " + JSON.stringify(obj3));
                        if(showLogs) 
                            console.log(
                                "\r\n\t test_103_New_Collection_Disallowed obj3 after save = " + 
                                JSON.stringify(obj3));
                        start(); 
                    },
                    error: function(obj, msg) {
                        ok(false, "Error reading object: " + 
                            JSON.stringify(obj) + ", error message: " + JSON.stringify(msg));
                        start(); 
                    }
                });
            },
            error: function(obj, msg) {
                ok(true, "Not allowed to add collection: " + 
                    JSON.stringify(obj) + ", error message: " + JSON.stringify(msg));
                start(); 
            }
        });            
    } else {
        ok(false, "backend is set to not secured, skipping test");
        start();
    }
});

/*!
 * Tests the ability for a schemaless backend to learn a new Class automatically.
 */
asyncTest("test_104_New_Class_Allowed", function() {
    ok(false, "Parse does not provide any information about the class of the object");
    start();
});

/*!
 * Tests that a backend with a locked schema will not allow a new Class definition
 * to be added by the client if disallowed by configuration.
 */
asyncTest("test_105_New_Class_Disallowed", function() {
    ok(false, "Parse does not provide any information about the class of the object");
    start();
});

/*!
 * Tests the ability to deploy and access a local development backend for from the client.
 */
asyncTest("test_106_Local_Backend", function() {
    ok(false, "Parse does not support local development");
    start();
});

/*!
 * Tests the ability to access multiple backends from the client SDK.
 */
asyncTest("test_107_Multiple_Backends", function() {
    ok(false, "Parse does not support access to multiple backends");
    start();
});

/*!
 * Tests the ability to access a metadata description of your backend.
 */
asyncTest("test_108_API_Metadata", function() {
    ok(false, "Parse does not provide API level metadata");
    start();
});

/*!
 * Tests the ability of the provider to host this web page (doh!).
 */
asyncTest("test_109_Web_Assets", function() {
    var host = window.location.hostname;
    var own = host.indexOf("parseapp.com") >=0;
    ok(own, "web app is hosted on " + host + ", should be parseapp.com");
    start();
});

/*!
 * Tests the ability to create an object from a defined object class without having to create a client schema.
 */
asyncTest("test_200_Foo_JSObjClass", function() {
    // create a new Foo() object
    var f1 = new Foo();
    f1.set("fooParameter", "test_200_Foo_JSObjClass");
    if(showLogs) 
        console.log("\r\n\t test_200_Foo_JSObjClass f1 before save = " + 
            JSON.stringify(f1));
    // create an object to make sure backend is really working
    f1.save(null, {
        success: function(f2) {
            ok(f2 !== null, "f2 is " + JSON.stringify(f2));
            f2.fetch({
                success: function(f3) {
                    ok(f3 !== null, "f3 is " + JSON.stringify(f3));
                    if(showLogs) 
                        console.log(
                            "\r\n\t test_200_Foo_JSObjClass f3 after save = " + 
                            JSON.stringify(f3));
                    start(); 
                },
                error: function(obj, msg) {
                    ok(false, "Error reading object: " + 
                        JSON.stringify(obj) + ", error message: " + msg.message);
                    start(); 
                }
            });
        },
        error: function(obj, msg) {
            ok(false, "Error saving object: " + 
                JSON.stringify(obj) + ", error message: " + msg.message);
            start(); 
        }
    });
});

/*!
 * Tests the ability to create an object without having to define an object model or create a client schema.
 */
asyncTest("test_201_Foo_Schemaless", function() {
    // create a new Foo() object
    var f1 = new Parse.Object("Foos");
    f1.set("fooParameter", "Testing create a Foo from Foo model");
    if(showLogs) 
        console.log("\r\n\t test_201_Foo_Schemaless f1 before save = " + 
            JSON.stringify(f1));
    // create an object to make sure backend is really working
    f1.save(null, {
        success: function(f2) {
            ok(f2 !== null, "f2 is " + JSON.stringify(f2));
            f2.fetch({
                success: function(f3) {
                    ok(f3 !== null, "f3 is " + JSON.stringify(f3));
                    if(showLogs) 
                        console.log(
                            "\r\n\t test_201_Foo_Schemaless f3 after save = " + 
                            JSON.stringify(f3));
                    start(); 
                },
                error: function(obj, msg) {
                    ok(false, "Error reading object: " + 
                        JSON.stringify(obj) + ", error message: " + msg.message);
                    start(); 
                }
            });
        },
        error: function(obj, msg) {
            ok(false, "Error saving object: " + 
                JSON.stringify(obj) + ", error message: " + msg.message);
            start(); 
        }
    });
});

/*!
 * Tests that a backend will add a new member propertry of a Class definition
 * to be added by the client if allowed by configuration.
 */
asyncTest("test_202_New_Member_Allowed", function() {
    if(!parseSetSecure) {
        ok(!parseSetSecure, "backend is set to not secured");
        var memb = randomString(5);
        var f1 = new Foo();
        f1.set("fooParameter", "Testing adding new member");
        f1.set(memb, "Testing adding new member " + memb);
        if(showLogs) 
            console.log(
                "\r\n\t test_202_New_Member_Allowed f1 before save = " +
                JSON.stringify(f1));
        f1.save(null, {
            success: function(f2) {
                ok(f2 !== null, "f2 is " + JSON.stringify(f2));
                ok((f2.get(memb) !== null), "f2 new " + memb + " value is " + f2.get(memb));
                f2.fetch({
                    success: function(f3) {
                        ok(f3 !== null, "f3 is " + JSON.stringify(f3));
                        if(showLogs) 
                            console.log(
                                "\r\n\t test_202_New_Member_Allowed f3 after save = " + 
                                JSON.stringify(f3));
                        start(); 
                    },
                    error: function(obj, msg) {
                        ok(false, "Error reading object: " + 
                            JSON.stringify(obj) + ", error message: " + msg.message);
                        start(); 
                    }
                });
            },
            error: function(obj, msg) {
                ok(false, "Error saving object: " + 
                    JSON.stringify(obj) + ", error message: " + msg.message);
                start(); 
            }
        });
    } else {
        ok(false, "backend is not set to secured, skipping test");
        start();
    }
});

/*!
 * Tests that a backend will not allow a new member propertry of a Class definition
 * to be added by the client if disallowed by configuration.
 */
asyncTest("test_203_New_Member_Disallowed", function() {
    if(parseSetSecure) {
        ok(parseSetSecure, "backend is set to secured");
        var memb = randomString(5);
        var f1 = new Foo();
        f1.set("fooParameter", "test_203_New_Member_Disallowed");
        f1.set(memb, "Testing adding new member" + memb);
        if(showLogs) 
            console.log(
                "\r\n\t test_203_New_Member_Disallowed f1 before save = " +
                JSON.stringify(f1));
        f1.save(null, {
            success: function(f2) {
                if(showLogs) 
                     console.log(
                         "\r\n\t test_203_New_Member_Disallowed f2 after save = " +
                         JSON.stringify(f2));
                ok(false, "Should not have allowed this " + 
                    JSON.stringify(f2) + ", " +
                    memb + " is "+ f2.get(memb));
                f2.fetch({
                    success: function(f3) {
                        ok((f3.get(memb) !== "undefined"), "f3 new " + memb + " value is " + f3.get(memb));
                        if(showLogs) 
                            console.log(
                                "\r\n\t test_203_New_Member_Disallowed f3 after save = " + 
                                JSON.stringify(f3));
                        start(); 
                    },
                    error: function(obj, msg) {
                        ok(true, "Not allowed to add member: " + 
                            JSON.stringify(obj) + ", error message: " + msg.message);
                        start(); 
                    }
                });
            },
            error: function(obj, msg) {
                ok(false, "Error saving object: " + 
                    JSON.stringify(obj) + ", error message: " + msg.message);
                start(); 
            }
        });
    } else {
        ok(false, "no ff_secure backend, skipping test");
        start();
    }
});

/*!
 * Tests if and what additional information about your object (metadata) is provided
 */
asyncTest("test_204_Metadata_Provided", function() {
    // create a new Foo() object
    var f1 = new Foo();
    f1.set("fooParameter", "test_204_Metadata_Provided metadata for Foo");
    if(showLogs) 
        console.log("\r\n\t test_204_Metadata_Provided f1 before save = " + 
            JSON.stringify(f1));
    f1.save(null, {
        success: function(f2) {
            var meta = {
               id:f2.id,
               updatedAt:f2.updatedAt,
               createdAt:f2.createdAt
            }
            ok(f2 !== null, "f2 is " + JSON.stringify(f2));
            ok(meta !== null, "metadata is " + JSON.stringify(meta));
            if(showLogs) 
                console.log("\r\n\t test_204_Metadata_Provided f2 metadata = " + 
                    JSON.stringify(meta));
            start(); 
        },
        error: function(obj, msg) {
            ok(false, "Error saving object: " + 
                JSON.stringify(obj) + ", error message: " + msg.message);
            start(); 
        }
   });
});

/*!
 Generates a random string of up to 1000 characters in length. Generates a random length up to 1000 if numCharacters is set to 0.
 */
function randomString(numCharacters) {
    //static char const possibleChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    var possibleChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    var len;
    if(numCharacters > 1000 || numCharacters == 0) len = Math.random() * (len);
    else len = numCharacters;
    var randomString = "";
    for(var i=0; i < len; ++i ) {
    	var randomPoz = Math.floor(Math.random() * possibleChars.length);
    	randomString += possibleChars.substring(randomPoz,randomPoz+1);
    }
    return randomString;
}

