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
var kinveySetSecure = false;

function setUpKitchenSyncTests() {
    var be = sessionStorage.getItem("backend");
    if (be && be == "secure") {
        kinveySetSecure = true;
        backend.innerHTML = "Currently using secure backend, click to switch to open";
    } else if (be && be == "open") {
        kinveySetSecure = false;
        backend.innerHTML = "Currently using open backend, click to switch to secure";
    }
    configureBackend();
    Kinvey.ping({
        success: function(response) {
            console.log('Kinvey Ping Success. Kinvey Service is alive, version: ' + response.version + ', response: ' + response.kinvey);
            QUnit.start();
        },
        error: function(error) {
            console.log('Kinvey Ping Failed. Response: ' + error.description);
        }
    });
}

function toggleBackend() {
    var backendEl = document.getElementById('backend');
    var be = sessionStorage.getItem("backend");
    if (be && be == "secure") {
    	sessionStorage.setItem("backend", "open");
        kinveySetSecure = false;
        backendEl.innerHTML = "Currently using open backend, click to switch to secure";
    } else if (be && be == "open") {
    	sessionStorage.setItem("backend", "secure");
        kinveySetSecure = true;
        backendEl.innerHTML = "Currently using secure backend, click to switch to open";
    } else {
        console.error("could not toggle backend mode");
    	sessionStorage.setItem("backend", "secure");
        kinveySetSecure = true;
        backendEl.innerHTML = "Using secure backend";
    }
    console.log("backend is now " + sessionStorage.getItem("backend"));
    location.reload();
}

/*!
 * Kinvey uses client "collections" that are mapped to classes. We need to create
 * a Kinvey.Collection for each model object we use. This creates a Foos collection
 * for the Foo model.
 */
var Foos = new Kinvey.Collection("Foo");

/*!
 * Kinvey uses client "collections" that are mapped to classes. We need to create
 * a Kinvey.Collection for each model object we use. This creates a Bars collection
 * for the Bar model.
 */
var Bars = new Kinvey.Collection("Bar");

/*!
 * Setting up the backend and configuration.
 * Since we are not able to talk to multiple backends, we will initalize one of two 
 * backends based on the value of manually set a bool value kinveySetSecure that 
 * will control which tests will be run.depending if the require the "open" 
 * configuration options or "secured". Toggle this and run again for a complete 
 * set of tests
 */
function configureBackend() {
    if(kinveySetSecure) {
        Kinvey.init({
            'appKey': 'kid_VPArUQbBtf',
            'appSecret': 'e4999495a5e346f29dfa75fb723fcfbb'
        });
    } else {
        Kinvey.init({
            'appKey': 'kid_TT-0IZbKn5',
            'appSecret': '749e1b6c9ec743d19270bfdb6631a290'
        });
    }
}

/*!
 * Tests the ability to access data without API authentication if allowed.
 */
asyncTest("test_100_Public_Access", function() {
    if(!kinveySetSecure) {
        ok(!kinveySetSecure, "backend is set to not secured");
        // create a new Foo() object
        var f1 = new Foo({
            fooParameter: 'Testing public access'
        });
        if(showLogs) 
            console.log("\r\n\t test_100_Public_Access f1 before save = " + 
                JSON.stringify(f1) +
                ", fooParameter: " + f1.get("fooParameter")
            );
        // create an object to make sure backend is really working
        f1.save({
            success: function(f2) {
                ok(f2 !== null, "f2 is " + JSON.stringify(f2) +
                ", fooParameter: " + f2.get("fooParameter")
                );
                try {
                    Kinvey.init({
                        'appKey': 'kid_VPArUQbBtf',
                        'appSecret': null
                    });
                    f2.load(f2.getId(), {
                        success: function(f3) {
                            ok(f3 !== null, "f3 is " + JSON.stringify(f3) +
                                ", fooParameter: " + f3.getFooParameter()
                            );
                            if(showLogs) 
                                console.log(
                                    "\r\n\t test_100_Public_Access f3 after save = " + 
                                    JSON.stringify(f3) + 
                                    ", fooParameter: " + f3.getFooParameter() +
                                     ", \r\n\tMetadata: " + 
                                     JSON.stringify(f3.getMetadata()));
                            start(); 
                        },
                        error: function(e) {
                            ok(false, "Error reading object: " + JSON.stringify(e));
                            start(); 
                        }
                    });
                } catch(err) {
                    ok(false, "Error with no key: " + err);
                    configureBackend();
                    start();
                }
            },
            error: function(e) {
                ok(false, "Error saving object, error message" + JSON.stringify(e));
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
    if(kinveySetSecure) {
        configureBackend();
        ok(kinveySetSecure, "backend is set to secured");
        // create a new Foo() object
        var f1 = new Foo({
            fooParameter: 'Testing secure access'
        });
        if(showLogs) 
            console.log("\r\n\t test_101_Secure_Access f1 before save = " + 
                JSON.stringify(f1) +
                ", fooParameter: " + f1.getFooParameter()
            );
        // create an object to make sure backend is really working
        f1.save({
            success: function(f2) {
                if(showLogs) 
                    console.log(
                        "\r\n\t test_101_Secure_Access f1 after save = " +
                        JSON.stringify(f1) +
                        ", fooParameter: " + f1.getFooParameter()
                    );
                ok(f2 !== null, "f2 is " + JSON.stringify(f2) +
                    ", fooParameter: " + f2.getFooParameter());
                if(showLogs) 
                    console.log(
                        "\r\n\t test_101_Secure_Access f2 after save = " +
                        JSON.stringify(f2) + 
                        ", fooParameter: " + f2.getFooParameter() + 
                        ", \r\n\tMetadata: " + 
                        JSON.stringify(f2.getMetadata()));
                f2.load(f1.getId(), {
                    success: function(f3) {
                        ok(f3 !== null, "f3 is " + JSON.stringify(f3) +
                            ", fooParameter: " + f3.getFooParameter());
                        if(showLogs) 
                            console.log(
                                "\r\n\t test_101_Secure_Access f3 after save = " + 
                                JSON.stringify(f3) +
                                ", fooParameter: " + f3.getFooParameter() + 
                                ", \r\n\tMetadata: " + 
                                JSON.stringify(f3.getMetadata()));
                        start(); 
                    },
                    error: function(e) {
                        ok(false, "Error reading object: " + JSON.stringify(e));
                        start(); 
                    }
                });
            },
            error: function(e) {
                ok(false, "Error saving object, error message" + JSON.stringify(e));
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
    if(!kinveySetSecure) {
        configureBackend();
        ok(!kinveySetSecure, "backend is set to not secured");
        var clazz = randomString(5);
        var coll = clazz + "s";
        var cs = "test_102_New_Collection_Allowed clazz " + clazz + " collection " + coll;
        var obj1 = new Kinvey.Entity({
            fooParameter:"test_102_New_Collection_Allowed",
        }, coll);
        if(showLogs) 
            console.log("\r\n\t test_102_New_Collection_Allowed obj1 before save = " + 
                JSON.stringify(obj1) +
                ", collection: " + obj1.collection +
                ", fooParameter: " + obj1.get("fooParameter")
            );
        obj1.save({
            success: function(obj2) {
                ok(obj2.collection == coll, "obj2 collection should be " + coll +
                    ", but is " + obj2.collection);
                obj2.load(obj2.getId(), {
                    success: function(obj3) {
                        ok(obj3.collection == coll, "obj3 collection should be " + coll + 
                            ", but is " + obj3.collection);
                        if(showLogs) 
                            console.log(
                                "\r\n\t test_102_New_Collection_Allowed obj3 after load = " + 
                                JSON.stringify(obj3) + 
                                ", fooParameter: " + obj3.get("fooParameter") +
                                ", \r\n\tMetadata: " + 
                                JSON.stringify(obj3.getMetadata()));
                        start(); 
                    },
                    error: function(e) {
                        ok(false, "Error reading object: " + JSON.stringify(e));
                        start(); 
                    }
                });
            },
            error: function(e) {
                ok(false, "Error saving object, error message" + JSON.stringify(e));
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
    if(kinveySetSecure) {
        configureBackend();
        ok(kinveySetSecure, "backend is set to secured");
        var clazz = randomString(5);
        var coll = clazz + "s";
        var cs = "test_103_New_Collection_Disallowed clazz "+clazz+" collection "+coll;
        var obj1 = new Kinvey.Entity({
            fooParameter:"test_102_New_Collection_Allowed",
        }, coll);
        if(showLogs) 
            console.log("\r\n\t test_103_New_Collection_Disallowed obj1 before save = " + 
                JSON.stringify(obj1) +
                ", fooParameter: " + obj1.get("fooParameter")
            );
        obj1.save({
            success: function(obj2) {
                if(showLogs) 
                    console.log(
                        "\r\n\t test_103_New_Collection_Disallowed obj2  = " + 
                        JSON.stringify(obj2) + 
                        ", \r\n\tMetadata: " + 
                        JSON.stringify(obj2.getMetadata()));
                ok(false, "Should not have allowed this " + JSON.stringify(obj2));
                obj2.load(obj2.getId(), {
                    success: function(obj3) {
                    	//TODO - how to get the collection from the object
                        ok(false,
                           "Should not have allowed this " + JSON.stringify(obj3));
                        if(showLogs) 
                            console.log(
                                "\r\n\t test_103_New_Collection_Disallowed obj3 after save = " + 
                                JSON.stringify(obj3) + 
                                ", \r\n\tMetadata: " + 
                                JSON.stringify(obj3.getMetadata()));
                        start(); 
                    },
                    error: function(e) {
                        ok(true, "Not allowed to add collection: " + JSON.stringify(e));
                        start(); 
                    }
                });
            },
            error: function(e) {
                ok(false, "Error saving object, error message" + JSON.stringify(e));
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
    ok(false, "Kinvey does not provide any information about the class of the object");
    start();
});

/*!
 * Tests that a backend with a locked schema will not allow a new Class definition
 * to be added by the client if disallowed by configuration.
 */
asyncTest("test_105_New_Class_Disallowed", function() {
    ok(false, "Kinvey does not provide any information about the class of the object");
    start();
});

/*!
 * Tests the ability to deploy and access a local development backend for from the client.
 */
asyncTest("test_106_Local_Backend", function() {
    ok(false, "Kinvey does not support local development");
    start();
});

/*!
 * Tests the ability to access multiple backends from the client SDK.
 */
asyncTest("test_107_Multiple_Backends", function() {
    ok(false, "Kinvey does not support access to multiple backends");
    start();
});

/*!
 * Tests the ability to access a metadata description of your backend.
 */
asyncTest("test_108_API_Metadata", function() {
    ok(false, "Kinvey does not provide API level metadata");
    start();
});

/*!
 * Tests the ability of the provider to host this web page (doh!).
 */
asyncTest("test_109_Web_Assets", function() {
    var host = window.location.hostname;
    var own = host.indexOf("kinvey.com") >=0;
    ok(own, "web app is hosted on " + host + ", should be kinvey.com");
    start();
});

/*!
 * Tests the ability to create an object from a defined object class without having to create a client schema.
 */
asyncTest("test_200_Foo_JSObjClass", function() {
    // create a new Foo() object
    configureBackend();
    var f1 = new Foo({
        fooParameter: 'test_200_Foo_JSObjClass'
    });
    if(showLogs) 
        console.log("\r\n\t test_200_Foo_JSObjClass f1 before save = " + 
            JSON.stringify(f1));
    // create an object to make sure backend is really working
    f1.save({
        success: function(f2) {
            ok(f2 !== null, "f2 is " + JSON.stringify(f2) +
                  "fooParameter is "+ f2.get("fooParameter"));
            f2.load(f2.getId(), {
                success: function(f3) {
                    ok(f3 !== null, "f3 is " + JSON.stringify(f3) +
                      "fooParameter is "+ f3.get("fooParameter"));
                    if(showLogs) 
                        console.log(
                            "\r\n\t test_200_Foo_JSObjClass f3 after save = " + 
                            JSON.stringify(f3) + 
                            ", \r\n\tMetadata: " + 
                            JSON.stringify(f3.getMetadata()));
                    start(); 
                },
                error: function(e) {
                    ok(false, "Error reading object: " + JSON.stringify(e));
                    start(); 
                }
            });
        },
        error: function(e) {
            ok(false, "Error saving object, error message" + JSON.stringify(e));
            start(); 
        }
    });
});

/*!
 * Tests the ability to create an object without having to define an object model or create a client schema.
 */
asyncTest("test_201_Foo_Schemaless", function() {
    configureBackend();
    // create a new Foo() object
    var f1 = new Kinvey.Entity({
        fooParameter:"test_201_Foo_Schemaless",
    }, "Foos");
    if(showLogs) 
        console.log("\r\n\t test_201_Foo_Schemaless f1 before save = " + 
            JSON.stringify(f1) +
            " fooParameter: " + f1.get("fooParameter")
        );
    f1.save({
        success: function(f2) {
            if(showLogs) 
                console.log(
                    "\r\n\t test_201_Foo_Schemaless f2 after save = " + 
                    JSON.stringify(f2) + 
                    ", fooParameter: " + f2.get("fooParameter") +
                    ", \r\n\tMetadata: " + 
                    JSON.stringify(f2.getMetadata()));
            ok(f2 !== null, "f2 is " + JSON.stringify(f2) +
                  "fooParameter is "+ f2.get("fooParameter"));
            f2.load(f2.getId(), {
                success: function(f3) {
                    ok(f3 !== null, "f3 is " + JSON.stringify(f3) +
                      "fooParameter is "+ f3.get("fooParameter"));
                    if(showLogs) 
                        console.log(
                            "\r\n\t test_201_Foo_Schemaless f3 after save = " + 
                            JSON.stringify(f3) + 
                            ", \r\n\tMetadata: " + 
                            JSON.stringify(f3.getMetadata()));
                    start(); 
                },
                error: function(e) {
                    ok(false, "Error reading object: " + JSON.stringify(e));
                    start(); 
                }
            });
        },
        error: function(e) {
            ok(false, "Error saving object, error message" + JSON.stringify(e));
            start(); 
        }
    });
});

/*!
 * Tests that a backend will add a new member propertry of a Class definition
 * to be added by the client if allowed by configuration.
 */
asyncTest("test_202_New_Member_Allowed", function() {
    if(!kinveySetSecure) {
        configureBackend();
        ok(!kinveySetSecure, "backend is set to not secured");
        var memb = randomString(5);
        var f1 = new Foo({
            fooParameter: 'test_202_New_Member_Allowed',
        });
        f1.set(memb,"test_202_New_Member_Allowed new member " + memb);
        if(showLogs) 
            console.log(
                "\r\n\t test_202_New_Member_Allowed f1 before save = " +
                JSON.stringify(f1) + ", " +
                memb + " is "+ f1.get(memb));
        f1.save({
            success: function(f2) {
                ok(f2 !== null, "f2 is " + JSON.stringify(f2) + ", " +
                    ", new member " + memb + " is: "+ f2.get(memb));
                f2.load(f2.getId(), {
                    success: function(f3) {
                        ok(f3 !== null, "f3 is " + JSON.stringify(f3) + 
                            ", new member " + memb + " is: "+ f3.get(memb));
                        if(showLogs) 
                            console.log(
                                "\r\n\t test_202_New_Member_Allowed f3 after save = " + 
                                JSON.stringify(f3) + 
                                ", \r\n\tMetadata: " + 
                                JSON.stringify(f3.getMetadata()));
                        start(); 
                    },
                    error: function(e) {
                        ok(false, "Error reading object: " + JSON.stringify(e));
                        start(); 
                    }
                });
            },
            error: function(e) {
                ok(false, "Error saving object, error message" + JSON.stringify(e));
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
    if(kinveySetSecure) {
        configureBackend();
        ok(kinveySetSecure, "backend is set to secured");
        var memb = randomString(5);
        var f1 = new Foo({
            fooParameter: 'test_202_New_Member_Allowed',
        });
        f1.set(memb,"test_202_New_Member_Allowed new member " + memb);
        if(showLogs) 
            console.log(
                "\r\n\t test_203_New_Member_Disallowed f1 before save = " +
                JSON.stringify(f1));
        f1.save({
            success: function(f2) {
                if(showLogs) 
                    console.log(
                        "\r\n\t test_103_New_Collection_Disallowed f2  = " + 
                        JSON.stringify(f2) + 
                        ", \r\n\tMetadata: " + 
                        JSON.stringify(f2.getMetadata()));
                ok(false, "Should not have allowed this " + 
                    JSON.stringify(f2) + ", " +
                    memb + " is "+ f2.get(memb));
                f2.load(f2.getId(), {
                    success: function(f3) {
                        ok(false, "Should not have allowed this " +
                            JSON.stringify(f3) + 
                            ", new member " + memb + " is "+ f3.get(memb));
                        if(showLogs) 
                            console.log(
                                "\r\n\t test_203_New_Member_Disallowed f3 after save = " + 
                                JSON.stringify(f3) + 
                                ", \r\n\tMetadata: " + 
                                JSON.stringify(f3.getMetadata()));
                        start(); 
                    },
                    error: function(e) {
                        ok(true, "Not allowed to add member: " + JSON.stringify(e));
                        start(); 
                    }
                });
            },
            error: function(e) {
                ok(false, "Error saving object, error message" + JSON.stringify(e));
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
    configureBackend();
    var f1 = new Foo();
    f1.set("fooParameter", "Testing metadata for Foo");
    if(showLogs) 
        console.log("\r\n\t test_204_Metadata_Provided f1 before save = " + 
            JSON.stringify(f1));
    f1.save({
        success: function(f2) {
            ok(f2 !== null, "f2 is " + JSON.stringify(f2) +
                  "; fooParameter is "+ f2.get("fooParameter"));
            var meta = f2.getMetadata();
            ok(meta !== null, "metadata is " + JSON.stringify(meta));
            if(showLogs) 
                console.log("\r\n\t test_204_Metadata_Provided f2 metadata = " + 
                    JSON.stringify(meta));
            start(); 
        },
        error: function(e) {
            ok(false, "Error saving object: " + JSON.stringify(e));
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
    if(numCharacters > 1000 || numCharacters == 0) len = Math.random() * (numCharacters);
    else len = numCharacters;
    var randomString = "";
    for(var i=0; i < len; ++i ) {
    	var randomPoz = Math.floor(Math.random() * possibleChars.length);
    	randomString += possibleChars.substring(randomPoz,randomPoz+1);
    }
    return randomString;
}

