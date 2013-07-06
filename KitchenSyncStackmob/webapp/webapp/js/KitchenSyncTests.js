//
//  KitchenSyncTests
//

QUnit.config.autostart = false;

/*!
 * A bool value that controls whether certain log messages are printed out or not during these tests.
 */
var showLogs = true;

function setUpKitchenSyncTests() {
    var backendEl = document.getElementById('backend');
    var be = sessionStorage.getItem("backend");
    if (be && be == "secure") {
        stackmobSetSecure = true;
        backendEl.innerHTML = "Currently using secure backend, click to switch to open";
    } else if (be && be == "open") {
        stackmobSetSecure = false;
        backendEl.innerHTML = "Currently using open backend, click to switch to secure";
    }
    QUnit.start();
    // This does not work - please see the index.html page for init code
    //configureBackend();
}

function toggleBackend() {
    var backendEl = document.getElementById('backend');
    var be = sessionStorage.getItem("backend");
    if (be && be == "secure") {
    	sessionStorage.setItem("backend", "open");
        stackmobSetSecure = false;
        backendEl.innerHTML = "Currently using open backend, click to switch to secure";
    } else if (be && be == "open") {
    	sessionStorage.setItem("backend", "secure");
        stackmobSetSecure = true;
        backendEl.innerHTML = "Currently using secure backend, click to switch to open";
    } else {
        console.error("could not toggle backend mode");
    	sessionStorage.setItem("backend", "secure");
        stackmobSetSecure = true;
        backendEl.innerHTML = "Using secure backend";
    }
    console.log("backend is now " + sessionStorage.getItem("backend"));
    location.reload();
}

/*!
 * Stackmob uses client "collections" that are mapped to classes.
 *  This creates a Foos collection for the Foo model.
 */
var Foos = StackMob.Collection.extend({
    model: Foo 
});
/*!
 * Stackmob uses client "collections" that are mapped to classes. 
 * This creates a Bars collection for the Bar model.
 */
var Bars = StackMob.Collection.extend({
    model: Bar 
});

/*!
 * Setting up the backend and configuration.
 * Since we are not able to talk to multiple backends, we will initalize one of two 
 * backends based on the value of manually set a bool value stackmobSetSecure that 
 * will control which tests will be run.depending if the require the "open" 
 * configuration options or "secured". Toggle this and run again for a complete 
 * set of tests
 */
function configureBackend() {
    // This does not work - please see the index.html page for init code
    if(stackmobSetSecure) {
        StackMob.init({
            publicKey:'64936eb2-a6e8-4139-93f6-045ca78c8d8b',
            apiVersion:0
        });
    } else {
        Stackmob.init({
            publicKey:'b993d420-e60b-4f81-b551-b1c116a0b93a',
            apiVersion:0
        });
    }
    QUnit.start();
}

/*!
 * Tests the ability to access data without API authentication if allowed.
 */
asyncTest("test_100_Public_Access", function() {
    if(!stackmobSetSecure) {
        ok(!stackmobSetSecure, "backend is set to not secured");
        // create a new Foo() object
        var f1 = new Foo();
        f1.set("fooParameter", "Testing public access");
        if(showLogs) 
            console.log("\r\n\t test_100_Public_Access f1 before save = " + 
                JSON.stringify(f1));
        // create an object to make sure backend is really working
        f1.create({
            success: function(f2) {
                ok(f2 !== null, "f2 is " + JSON.stringify(f2));
                if(showLogs) 
                    console.log(
                        "\r\n\t test_100_Public_Access f2 after save = " + 
                        JSON.stringify(f2));
                try {
                    Stackmob.init({
                        publicKey:null,
                        apiVersion:0
                    });
                f2.fetch({
                    success: function(f3) {
                        ok(f3 !== null, "f3 is " + JSON.stringify(f3));
                        if(showLogs) 
                            console.log(
                                "\r\n\t test_100_Public_Access f3 after save = " + 
                                JSON.stringify(f3));
                        start(); 
                    },
                    error: function(e) {
                        ok(false, "Error reading object: " + e);
                        start(); 
                    }
                });
                } catch(error) {
                    ok(error == null, "error is " + error);
                    start();
                }  	    
            },
            error: function(e) {
                ok(false, "Error saving object: " + e);
                start(); 
            }
        });
    } else {
        ok(false, "backend is not set to secured, skipping test");
        start();
    }
});

/*!
 * Tests the ability to require secure access to access data.
 */
asyncTest("test_101_Secure_Access", function() {
    if(stackmobSetSecure) {
        ok(stackmobSetSecure, "backend is set to secured");
        // create a new Foo() object
        var f1 = new Foo();
        f1.set("fooParameter", "Testing secure access");
        if(showLogs) 
            console.log("\r\n\t test_101_Secure_Access f1 before save = " + 
                JSON.stringify(f1));
        // create an object to make sure backend is really working
        f1.create({
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
                    error: function(e) {
                        ok(false, "Error reading object: " + e);
                        start(); 
                    }
                });
            },
            error: function(e) {
                ok(false, "Error saving object: " + e);
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
    if(!stackmobSetSecure) {
        ok(!stackmobSetSecure, "backend is set to not secured");
        var clazz = randomString(5);
        var coll = clazz + "s";
        var cs = "test_102_New_Collection_Allowed clazz " + clazz + " collection " + coll;
        // Note: You can only defince an object "class" and collection together.
        var What = StackMob.Model.extend({
            schemaName: coll //this maps a schema at https://dashboard.stackmob.com/schemas/
        });
        var obj1 = new What({
            someParameter: cs
        });
        if(showLogs) 
            console.log(
               "\r\n\t test_102_New_Collection_Allowed obj1  = " + JSON.stringify(obj1));
        obj1.create({
            success: function(obj2) {
                if(showLogs) { 
                    console.log(
                        "\r\n\t test_102_New_Collection_Allowed obj2  = " + JSON.stringify(obj2));
                    // Note - the only value returned from the backend is all lower case
                    // if for the object that *sort of* represents the collection
            	    var mem = coll.toLowerCase() + "_id";
            	    console.log(mem);
            	    console.log(obj2.someParameter);
            	    console.log(obj2.hasOwnProperty(mem));
            	    console.log(mem in obj2);
            	    console.log(obj2.get(mem));
            	    console.log(obj2.get("schemaName"));
            	    console.log(obj2.constructor);
            	}
            	// the only way to check the collection is to look at the id
                ok(obj2.get(coll.toLowerCase() + "_id"), "obj2 schemaName is " + coll);
                obj2.fetch({
                    success: function(obj3) {
                        ok(obj2.get(coll.toLowerCase() + "_id"), "obj3 schemaName is " + coll);
                        if(showLogs) 
                            console.log(
                                "\r\n\t test_102_New_Collection_Allowed obj3 after save = " + 
                                JSON.stringify(obj3));
                        start(); 
                    },
                    error: function(e) {
                        ok(false, "Error reading object: " + e);
                        start(); 
                    }
                });
            },
            error: function(e) {
                ok(false, "Error saving object: " + e);
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
    if(stackmobSetSecure) {
        ok(stackmobSetSecure, "backend is set to not secured");
        var clazz = randomString(5);
        var coll = clazz + "s";
        var cs = "test_103_New_Collection_Disallowed clazz "+clazz+" collection "+coll;
        // There seems to be no other way to define a collection than to definde a model.
        var What = StackMob.Model.extend({
            schemaName: coll 
            //this maps a schema at https://dashboard.stackmob.com/schemas/
        });
        var obj1 = new What({
            someParameter: cs
        });
        if(showLogs) 
            console.log(
               "\r\n\t test_103_New_Collection_Disallowed obj1  = " + JSON.stringify(obj1));
        obj1.create({
            success: function(obj2) {
                if(showLogs) 
                    console.log(
                        "\r\n\t test_103_New_Collection_Disallowed obj2  = " + JSON.stringify(obj2));
                ok(false, "Should not have allowed this : " + JSON.stringify(obj2));
                start(); 
            },
            error: function(e) {
                ok(true, "Backend prevented adding : " + e);
                start(); 
            }
        });            
    } else {
        ok(false, "backend is set to secured, skipping test");
        start();
    }
});

/*!
 * Tests the ability for a schemaless backend to learn a new Class automatically.
 */
asyncTest("test_104_New_Class_Allowed", function() {
    ok(false, "Stackmob does not provide any information about the class of the object");
    start();
});

/*!
 * Tests that a backend with a locked schema will not allow a new Class definition
 * to be added by the client if disallowed by configuration.
 */
asyncTest("test_105_New_Class_Disallowed", function() {
    ok(false, "Stackmob does not provide any information about the class of the object");
    start();
});

/*!
 * Tests the ability to deploy and access a local development backend for from the client.
 */
asyncTest("test_106_Local_Backend", function() {
    ok(false, "Stackmob does not support local development");
    start();
});

/*!
 * Tests the ability to access multiple backends from the client SDK.
 */
asyncTest("test_107_Multiple_Backends", function() {
    ok(false, "Stackmob does not support access to multiple backends");
    start();
});

/*!
 * Tests the ability to access a metadata description of your backend.
 */
asyncTest("test_108_API_Metadata", function() {
    ok(false, "Stackmob does not provide API level metadata");
    start();
});

/*!
 * Tests the ability of the provider to host this web page (doh!).
 */
asyncTest("test_109_Web_Assets", function() {
    var host = window.location.hostname;
    var own = host.indexOf("stackmob.com") >=0;
    ok(own, "web app is hosted on " + host + ", should be stackmob.com");
    start();
});

/*!
 * Tests the ability to create an object from a defined object model without having to create a client schema.
 */
asyncTest("test_200_Foo_JSObjClass", function() {
    // create a new Foo() object
    var f1 = new Foo();
    f1.set("fooParameter", "Testing create a Foo from Foo model test");
    if(showLogs) 
        console.log("\r\n\t test_200_Foo_JSObjClass f1 before save = " + 
            JSON.stringify(f1));
    // create an object to make sure backend is really working
    f1.create({
        success: function(f2) {
            console.log(f2.get("foos_id")); 
            if(showLogs) 
                console.log("\r\n\t test_200_Foo_JSObjClass f2 after save = " + 
                    JSON.stringify(f2));
            ok(f2 !== null, "f2 is " + JSON.stringify(f2));
            f2.fetch({
                success: function(f3) {
                    console.log(f3.length);
                    console.log(f3.size);
                    console.log(f3.get("foos_id"));
                    console.log(JSON.stringify(f3));
                    console.log(f3.get("fooParameter"));
                    ok(f3.get("createddate") == f2.get("createddate"), "returned the correct item");
                    ok(f3.get("foos_id") == f2.get("foos_id"), "returned the correct item");
                    start();
                },
                error: function(e) {
                    ok(false, "Error reading object: " + e);
                    start(); 
                }
            });
        },
        error: function(e) {
            ok(false, "Error saving object: " + e);
            start(); 
        }
    });
});

/*!
 * Tests the ability to create an object without having to define an object model or create a client schema.
 */
asyncTest("test_201_Foo_Schemaless", function() {
    ok(false, "Stackmob does not support objects without defined models.");
    start();
});

/*!
 * Tests that a backend will add a new member propertry of a Class definition
 * to be added by the client if allowed by configuration.
 */
asyncTest("test_202_New_Member_Allowed", function() {
    if(!stackmobSetSecure) {
        ok(!stackmobSetSecure, "backend is set to not secured");
        var memb = randomString(5);
        var f1 = new Foo();
        f1.set("fooParameter", "Testing adding new member");
        f1.set(memb, "Testing adding new member " + memb);
        if(showLogs) 
            console.log(
                "\r\n\t test_202_New_Member_Allowed f1 before save = " +
                JSON.stringify(f1));
        f1.create({
            success: function(f2) {
                ok(f2 !== null, "f2 is " + JSON.stringify(f2));
                ok((f2.get(memb) !== null), "f2 new " + memb + " value is " + f2.get(memb));
                var id = f2.get("foos_id");
                console.log(id);
                f2.fetch({
                    success: function(f3) {
                        ok((f3.get(memb) !== null), "f3 new " + memb + " value is " + f3.get(memb));
                        if(showLogs) 
                            console.log(
                                "\r\n\t test_202_New_Member_Allowed f4 after save = " + 
                                JSON.stringify(f3));
                        start(); 
                    },
                    error: function(e) {
                        ok(false, "Error reading object: " + e);
                        start(); 
                    }
                });
            },
            error: function(e) {
                ok(false, "Error saving object: " + e);
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
    if(stackmobSetSecure) {
        ok(stackmobSetSecure, "backend is set to secured");
        var memb = randomString(5);
        var f1 = new Foo();
        f1.set("fooParameter", "Testing adding new member");
        f1.set(memb, memb);
        if(showLogs) 
            console.log(
                "\r\n\t test_203_New_Member_Disallowed f1 before save = " +
                JSON.stringify(f1));
        f1.create({
            success: function(f2) {
                if(showLogs) 
                     console.log(
                         "\r\n\t test_203_New_Member_Disallowed f2 after save = " +
                         JSON.stringify(f2));
                ok(false, "Should not have allowed this " + JSON.stringify(f2));
                f2.fetch({
                    success: function(f3) {
                        ok((f3.get(memb) !== "undefined"), "f3 new " + memb + " value is " + f3.get(memb));
                        if(showLogs) 
                            console.log(
                                "\r\n\t test_203_New_Member_Disallowed f3 after save = " + 
                                JSON.stringify(f3));
                        start(); 
                    },
                    error: function(e) {
                        ok(true, "Not allowed to add member: " + e);
                        start(); 
                    }
                });
            },
            error: function(e) {
                ok(false, "Error saving object: " + e);
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
    f1.create({
        success: function(f2) {
            var meta = {
               foos_id:f2.get("foos_id"),
               lastmoddate:f2.get("lastmoddate"),
               createddate:f2.get("createddate")
            }
            ok(f2 !== null, "f2 is " + JSON.stringify(f2));
            ok(meta !== null, "; metadata is " + JSON.stringify(meta));
            if(showLogs) 
                console.log("\r\n\t test_204_Metadata_Provided f2 metadata = " + 
                    JSON.stringify(meta));
            start(); 
        },
        error: function(e) {
            ok(false, "Error saving object: " + e);
            start(); 
        }
    });
});

/*!
 Generates a random string of up to 1000 characters in length. Generates a random length up to 1000 if numCharacters is set to 0.
 */
function randomString(numCharacters) {
    var possibleChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    var len;
    if(numCharacters > 1000 || numCharacters == 0) len = Math.random() * (numCharacters);
    else len = numCharacters;
    var randomString = "";
    for(var i=0; i < len; ++i ) {
    	var randomPoz = Math.floor(Math.random() * possibleChars.length);
    	randomString += possibleChars.substring(randomPoz,randomPoz+1);
    }
    return randomString;
}

