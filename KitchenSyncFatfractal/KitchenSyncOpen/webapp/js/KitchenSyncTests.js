//
//  KitchenSyncTests
//

QUnit.config.autostart = false;

function setUpKitchenSyncTests() {
    configureBackend();
    //deleteAllTestData();
}

/*!
 * A bool value that controls whether certain log messages are printed out or not during these tests.
 */
var showLogs = true;

var ff_open = null;
var ff_secure = null;
var ff_local = null;

/*!
 * Setting up the backend and configuration.
 */
function configureBackend() {
    var ready =0;
    if (! ff_open) {
        ff_open = new FatFractal();
        ff_open.setBaseUrl("https://kitchensync.fatfractal.com/open");
        ff_open.setDebug(false);
        ff_open.setSimulateCookies(true);
        ff_open.logout(function(r) {
            console.log("found ff_open installed");
            console.log("logout result " + JSON.stringify(r));
            readySetGo();
        }, function(code, msg) {
            console.log("it appears that ff_open is not deployed "+code+": "+msg);
            ff_open = false;
            readySetGo();
        });
    }
    if (! ff_secure) {
        ff_secure = new FatFractal();
        ff_secure.setBaseUrl("https://kitchensync.fatfractal.com/secure");
        ff_secure.setDebug(false);
        ff_secure.setSimulateCookies(true);
        ff_secure.logout(function(r) {
            console.log("found ff_secure installed");
            console.log("logout result " + JSON.stringify(r));
            readySetGo();
        }, function(code, msg) {
            console.log("it appears that ff_secure is not deployed "+code+": "+msg);
            ff_secure = false;
            readySetGo();
        });
    }
    if (! ff_local) {
        ff_local = new FatFractal();
        ff_local.setDebug(false);
        ff_local.setSimulateCookies(true);
        // check for localhost open
        ff_local.setBaseUrl("https://localhost:8443/open");
        ff_local.logout(function(r1) {
            console.log("found localhost open installed");
            console.log("logout result " + JSON.stringify(r1));
            readySetGo();
        }, function(code1, msg1) {
            console.log("it appears that ff_local open is not deployed "+code1+": "+msg1);
            // check for localhost secure
            ff_local.setBaseUrl("https://localhost:8443/secure");
            ff_local.logout(function(r2) {
                console.log("found localhost open installed");
                console.log("logout result " + JSON.stringify(r2));
                readySetGo();
            }, function(code2, msg2) {
                console.log("it appears that ff_local secure is not deployed "+code2+": "+msg2);
                ff_local = null;
                readySetGo();
            });
        });
    }
    function readySetGo() {
    	ready ++;
        console.log("ready " + ready);
    	if(ready == 3) QUnit.start();
    }
}

/*!
 * Tests the ability to access data without authentication if allowed.
 */
asyncTest("test_100_Public_Access", function() {
    if(ff_open) {	
        ok(ff_open, "have an ff_open backend");
        ff_open.login("test_user", "test_user", function(result) {
            // create a new Foo() object
	    console.log("ff_open.loggedIn is: " + ff_open.loggedIn());
            ok(ff_open.loggedIn(), "ff_open.loggedIn() is " + ff_open.loggedIn());
            var f1 = new Foo();
            f1.fooParameter = "Testing public access";
            if(showLogs) 
               console.log("\r\n\t test_100_Public_Access f1 before save = " + 
                   JSON.stringify(f1));
            // create an object to make sure backend is really working
            ff_open.createObjAtUri(f1, "/Foo", function(f2) {
                ok(f2 !== null, "f2 is " + JSON.stringify(f2));
                // verify that the object can be accessed without authentication
                var meta = new FFMetaData(f2);
                ff_open.logout(function() {
                    ff_open.getObjFromUri(meta.ffUrl, function(f3) {
                        ok(f3 !== null, "f3 is " + JSON.stringify(new Foo(f3)));
                        if(showLogs) 
                            console.log(
                                "\r\n\t test_100_Public_Access f3 after save = " + 
                                JSON.stringify(new Foo(f3)) + 
                                ", \r\n\tMetadata: " + 
                                JSON.stringify(new FFMetaData(f3)));
                        start(); 
                    }, function(code, msg) {
                        ok(false, "Error reading object: " + code + ", error message" + msg);
                        start(); 
                    });
                }, function(code, msg) {
                    ok(false, "Error logging out: " + code + ", error message" + msg);
                    start(); 
                });
            }, function(code, msg) {
                ok(false, "Error saving object: " + code + ", error message" + msg);
                start(); 
            });
        }, function(code, msg) {
            ok(false, "Error logging in: " + code + ", error message" + msg);
            start(); 
        });
    } else {
        ok(false, "no ff_open backend, skipping test");
        start();
    }
});

/*!
 * Tests the ability to require secure access to access data.
 */
asyncTest("test_101_Secure_Access", function() {
    if(ff_secure) {
        ok(ff_secure, "have an ff_secure backend");
        ff_secure.login("test_user", "test_user", function() {
            // create a new Foo() object
            console.log("ff_secure.loggedIn is: " + ff_secure.loggedIn());
            ok(ff_secure.loggedIn(), "ff_secure.loggedIn() is " + ff_secure.loggedIn());
            var f1 = new Foo();
            f1.fooParameter = "Testing secure access";
            if(showLogs)
                console.log("\r\n\t test_101_Secure_Access f1 before save = " + 
                    JSON.stringify(f1));
            // create an object to make sure backend is really working
            ff_secure.createObjAtUri(f1, "/Foo", function(f2) {
                ok(f2 !== null, "f2 is " + JSON.stringify(new Foo(f2)));
                var meta = new FFMetaData(f2);
                ff_secure.logout();
                // verify that the object cannot be accessed without authentication
                ff_secure.getObjFromUri(meta.ffUrl, function(f3) {
                    ok(f3 !== null, "f3 is " + JSON.stringify(new Foo(f3)));
                    if(showLogs) 
                        console.log(
                            "\r\n\t test_101_Secure_Access f3 after save = " + 
                            JSON.stringify(new Foo(f3)) + 
                            ", \r\n\tMetadata: " + 
                            JSON.stringify(new FFMetaData(f3)));
                    start(); 
                }, function(code, msg) {
                    ok(false, "Error reading object: " + code + " error message" + msg);
                    start(); 
                });
            }, function(code, msg) {
                ok(false, "Error saving object: " + code + " error message" + msg);
                start(); 
            });
        }, function(code, msg) {
            ok(false, "Error logging in: " + code + " error message" + msg);
            start(); 
        });
    } else {
        ok(false, "no ff_secure backend, skipping test");
        start();
    }
});

/*!
 * Tests the ability for a schemaless backend to learn a new Collection automatically.
 */
asyncTest("test_102_New_Collection_Allowed", function() {
    if(ff_open) {
        ok(ff_open, "have an ff_open backend");
        ff_open.login("test_user", "test_user", function() {
            ok(ff_open.loggedIn(), "ff_open.loggedIn() is " + ff_open.loggedIn());
            var coll = randomString(5);
            var cs = "JS Object in coll " + coll;
            var f1 = new Foo();
            f1.fooParameter = cs;
            if(showLogs) 
                console.log(
                    "\r\n\t test_102_New_Collection_Allowed f1 before save = " +
                    "fooParameter" + f1.fooParameter);
            ff_open.createObjAtUri(f1, "/"+coll, function(f2) {
                ok(f2 !== null, "f2 is " + JSON.stringify(new Foo(f2)));
                var meta = new FFMetaData(f2);
                ff_open.getObjFromUri(meta.ffUrl, function(f3) {
                    ok(f3.ffRL == "/"+coll, "f3 collection is " + f3.ffRL);
                    if(showLogs) 
                        console.log(
                            "\r\n\t test_102_New_Collection_Allowed f3 after save = " + 
                            JSON.stringify(new Foo(f3)) + 
                            ", \r\n\tMetadata: " + 
                            JSON.stringify(new FFMetaData(f3)));
                    start(); 
                }, function(code, msg) {
                    ok(false, "Error reading object: " + code + " error message" + msg);
                    start(); 
                });
            }, function(code, msg) {
                ok(false, "Error saving object: " + code + " error message" + msg);
                start(); 
            });
        }, function(code, msg) {
            ok(false, "Error logging in: " + code + " error message" + msg);
            start(); 
        });
    } else {
        ok(false, "no ff_open backend, skipping test");
        start();
    }
});

/*!
 * Tests that a backend with a locked schema will not allow a new Collection
 * to be added by the client if disallowed by configuration.
 */
asyncTest("test_103_New_Collection_Disallowed", function() {
    if(ff_secure) {
        ok(ff_secure, "have an ff_secure backend");
        ff_secure.login("test_user", "test_user", function() {
            ok(ff_secure.loggedIn(), "ff_secure.loggedIn() is " + ff_secure.loggedIn());
            var coll = randomString(5);
            var cs = "JS Object in coll " + coll;
            var f1 = new Foo();
            f1.fooParameter = cs;
                console.log(
                    "\r\n\t test_103_New_Collection_Disallowed foo before save = " +
                    "fooParameter" + f1.fooParameter);
            ff_secure.createObjAtUri(f1, "/"+coll, function(f2) {
                ok(f2 == null, "f2 is " + JSON.stringify(f2));
                start(); 
            }, function(code, msg) {
                ok(true, "Not allowed to add collection: " + code + " error message" + msg);
                start(); 
            });
        }, function(code, msg) {
            ok(false, "Error logging in: " + code + " error message" + msg);
            start(); 
        });
    } else {
        ok(false, "no ff_secure backend, skipping test");
        start();
    }
});

/*!
 * Tests the ability for a schemaless backend to learn a new Class automatically.
 */
asyncTest("test_104_New_Class_Allowed", function() {
    if(ff_open) {
        ok(ff_open, "have an ff_open backend");
        ff_open.login("test_user", "test_user", function() {
            ok(ff_open.loggedIn(), "ff_open.loggedIn() is " + ff_open.loggedIn());
            var clazz = randomString(5);
            var coll = "/" + clazz;
            var cs = "Object with clazz " + clazz;
            var obj1 = {
                clazz:clazz,
                someParameter:cs
            };
            if(showLogs) console.log(
               "\r\n\t test_104_New_Class_Allowed obj1  = " + JSON.stringify(obj1));
            ff_open.createObjAtUri(obj1, coll, function(obj2) {
                ok(obj2.clazz == clazz, "obj2 clazz is " + clazz);
                var meta = new FFMetaData(obj2);
                ff_open.getObjFromUri(meta.ffUrl, function(obj3) {
                    ok(obj3.clazz == clazz, "obj3 clazz is " + obj3.clazz);
                    if(showLogs) 
                        console.log(
                            "\r\n\t test_104_New_Class_Allowed obj3 after save = " + 
                            JSON.stringify(obj3) + 
                            ", \r\n\tMetadata: " + 
                            JSON.stringify(new FFMetaData(obj3)));
                    start(); 
                }, function(code, msg) {
                    ok(false, "Error reading object: " + code + " error message" + msg);
                    start(); 
                });
            }, function(code, msg) {
                ok(false, "Error saving object: " + code + " error message" + msg);
                start(); 
            });
        }, function(code, msg) {
            ok(false, "Error logging in: " + code + " error message" + msg);
            start(); 
        });
    } else {
        ok(false, "no ff_secure backend, skipping test");
        start();
    }
});

/*!
 * Tests that a backend with a locked schema will not allow a new Class definition
 * to be added by the client if disallowed by configuration.
 */
asyncTest("test_105_New_Class_Disallowed", function() {
    if(ff_secure) {
        ok(ff_secure, "have an ff_secure backend");
        ff_secure.login("test_user", "test_user", function() {
            ok(ff_secure.loggedIn(), "ff_secure.loggedIn() is " + ff_secure.loggedIn());
            var clazz = randomString(5);
            var coll = "/" + clazz;
            var cs = "Object with clazz " + clazz;
            var obj1 = {
                clazz:clazz,
                someParameter:cs
            };
            if(showLogs) console.log(
               "\r\n\t test_105_New_Class_Disallowed obj1  = " + JSON.stringify(obj1));
            ff_secure.createObjAtUri(obj1, coll, function(obj2) {
                ok(obj2 == null, "obj2 is " + JSON.stringify(obj2));
                start(); 
            }, function(code, msg) {
                ok(true, "Not allowed to add class: " + code + " error message" + msg);
                start(); 
            });
        }, function(code, msg) {
            ok(false, "Error logging in: " + code + " error message" + msg);
            start(); 
        });
    } else {
        ok(false, "no ff_secure backend, skipping test");
        start();
    }
});

/*!
 * Tests the ability to deploy and access a local development backend for from the client.
 */
asyncTest("test_106_Local_Backend", function() {
    if(ff_local) {
        ok(ff_local, "have an ff_local backend");
        ff_local.login("test_user", "test_user", function() {
            // create a new Foo() object
            console.log("ff_local.loggedIn is: " + ff_local.loggedIn());
            ok(ff_local.loggedIn(), "ff_local.loggedIn() is " + ff_local.loggedIn());
            var f1 = new Foo();
            f1.fooParameter = "Testing local access";
            if(showLogs)
                console.log("\r\n\t test_106_Local_Backend f1 before save = " + 
                    JSON.stringify(f1));
            // create an object to make sure backend is really working
            ff_local.createObjAtUri(f1, "/Foo", function(f2) {
                ok(f2 !== null, "f2 is " + JSON.stringify(new Foo(f2)));
                var meta = new FFMetaData(f2);
                ff_local.getObjFromUri(meta.ffUrl, function(f3) {
                    ok(f3 !== null, "f3 is " + JSON.stringify(new Foo(f3)));
                    if(showLogs) 
                        console.log(
                            "\r\n\t test_106_Local_Backend f3 after save = " + 
                            JSON.stringify(new Foo(f3)) + 
                            ", \r\n\tMetadata: " + 
                            JSON.stringify(new FFMetaData(f3)));
                    start(); 
                }, function(code, msg) {
                    ok(false, "Error reading object: " + code + " error message" + msg);
                    start(); 
                });
            }, function(code, msg) {
                ok(false, "Error saving object: " + code + " error message" + msg);
                start(); 
            });
        }, function(code, msg) {
            ok(false, "Error logging in: " + code + " error message" + msg);
            start(); 
        });
    } else {
        ok(false, "it appears that there is no local backend installed, skipping test");
        start();
    }
});

/*!
 * Tests the ability to access multiple backends from the client SDK.
 */
asyncTest("test_107_Multiple_Backends", function() {
    //stop(2);
    if(ff_open) {
        ok(ff_open, "have an ff_open backend");
        ff_open.login("test_user", "test_user", function(result) {
            // create a new Foo() object
            console.log("ff_open.loggedIn is: " + ff_open.loggedIn());
            ok(ff_open.loggedIn(), "ff_open.loggedIn() is " + ff_open.loggedIn());
            var f1 = new Foo();
            f1.fooParameter = "Testing multiple backends - ff_open";
            if(showLogs)
                console.log("\r\n\t  f1 before save = " +
                    JSON.stringify(f1));
            // create an object to make sure backend is really working
            ff_open.createObjAtUri(f1, "/Foo", function(f2) {
                ok(f2 !== null, "f2 is " + JSON.stringify(new Foo(f2)));
                var meta = new FFMetaData(f2);
                ff_open.getObjFromUri(meta.ffUrl, function(f3) {
                    ok(f3 !== null, "f3 is " + JSON.stringify(new Foo(f3)));
                    if(showLogs)
                        console.log(
                            "\r\n\t  f3 after save = " +
                                JSON.stringify(new Foo(f3)) +
                                ", \r\n\tMetadata: " +
                                JSON.stringify(new FFMetaData(f3)));
                    if(ff_secure) {
                        ok(ff_secure, "have an ff_secure backend");
                        ff_secure.login("test_user", "test_user", function() {
                            // create a new Foo() object
                            console.log("ff_secure.loggedIn is: " + ff_secure.loggedIn());
                            ok(ff_secure.loggedIn(), "ff_secure.loggedIn() is " + ff_secure.loggedIn());
                            var f4 = new Foo();
                            f4.fooParameter = "Testing multiple backends - ff_secure";
                            if(showLogs)
                                console.log("\r\n\t test_107_Multiple_Backends f4 before save = " +
                                    JSON.stringify(new Foo(f4)));
                            // create an object to make sure backend is really working
                            ff_secure.createObjAtUri(f4, "/Foo", function(f5) {
                                ok(f5 !== null, "f5 is " + JSON.stringify(new Foo(f5)));
                                var meta2 = new FFMetaData(f5);
                                ff_secure.getObjFromUri(meta2.ffUrl, function(f6) {
                                    ok(f6 !== null, "f6 is " + JSON.stringify(new Foo(f6)));
                                    if(showLogs)
                                        console.log(
                                            "\r\n\t test_107_Multiple_Backends f6 after save = " +
                                                JSON.stringify(new Foo(f6)) +
                                                ", \r\n\tMetadata: " +
                                                JSON.stringify(new FFMetaData(f6)));
                                    start();
                                }, function(code, msg) {
                                    ok(false, "Error reading object: " + code + " error message" + msg);
                                    start();
                                });
                            }, function(code, msg) {
                                ok(false, "Error saving object: " + code + " error message" + msg);
                                start();
                            });
                        }, function(code, msg) {
                            ok(false, "Error logging in: " + code + " error message" + msg);
                            start();
                        });
                    } else {
                        ok(false, "no ff_secure backend");
                        start();
                    }
                }, function(code, msg) {
                    ok(false, "Error reading object: " + code + ", error message" + msg);
                    start();
                });
            }, function(code, msg) {
                ok(false, "Error saving object: " + code + ", error message" + msg);
                start();
            });
        }, function(code, msg) {
            ok(false, "Error logging in: " + code + ", error message" + msg);
            start();
        });
    } else {
        ok(false, "no open backend");
        start();
    }
});

/*!
 * Tests the ability to access a metadata description of your backend.
 */
asyncTest("test_108_API_Metadata", function() {
    if(ff_open) {
        ok(ff_open, "have an ff_open backend");
        ff_open.getFFDL(function(md) {
            ok(md !== null, "md is " + JSON.stringify(md));
            if(showLogs) 
                console.log(
                    "\r\n\t test_108_API_Metadata backend metadata = " + 
                    JSON.stringify(md));
            start(); 
        }, function(code, msg) {
            ok(false, "Error reading object: " + code + " error message" + msg);
            start(); 
        });
    } else {
        ok(false, "no ff_open backend");
        start();
    }
});

/*!
 * Tests the ability of the provider to host this web page (doh!).
 */
asyncTest("test_109_Web_Assets", function() {
    var host = window.location.hostname;
    var own = (host.indexOf("fatfractal.com") >=0 || host.indexOf("localhost") >=0);
    ok(own, "web app is hosted on " + host + ", should be fatfractal.com");
    start();
});

/*!
 * Tests the ability to create an object from a defined object class without having to create a client schema.
 */
asyncTest("test_200_Foo_JSObjClass", function() {
    if(ff_open) {	
        ok(ff_open, "have an ff_open backend");
        ff_open.login("test_user", "test_user", function(result) {
            // create a new Foo() object
	    console.log("ff_open.loggedIn is: " + ff_open.loggedIn());
            ok(ff_open.loggedIn(), "ff_open.loggedIn() is " + ff_open.loggedIn());
            var f1 = new Foo();
            f1.fooParameter = "Testing create a Foo from Foo model";
            if(showLogs) 
               console.log("\r\n\t test_200_Foo_JSObjClass f1 before save = " + 
                   JSON.stringify(f1));
            ff_open.createObjAtUri(f1, "/Foo", function(f2) {
                ok(f2 !== null, "f2 is " + JSON.stringify(new Foo(f2)));
                if(showLogs) 
                    console.log(
                        "\r\n\t test_200_Foo_JSObjClass f3 after save = " + 
                        JSON.stringify(new Foo(new Foo(f2))) + 
                        ", \r\n\tMetadata: " + 
                        JSON.stringify(new FFMetaData(f2)));
                start(); 
            }, function(code, msg) {
                ok(false, "Error saving object: " + code + ", error message" + msg);
                start(); 
            });
        }, function(code, msg) {
            ok(false, "Error logging in: " + code + ", error message" + msg);
            start(); 
        });
    } else {
        ok(false, "no ff_open backend, skipping test");
        start();
    }
});

/*!
 * Tests the ability to create an object without having to define an object model or create a client schema.
 */
asyncTest("test_201_Foo_Schemaless", function() {
    if(ff_open) {	
        ok(ff_open, "have an ff_open backend");
        ff_open.login("test_user", "test_user", function(result) {
            // create a new object
	    console.log("ff_open.loggedIn is: " + ff_open.loggedIn());
            ok(ff_open.loggedIn(), "ff_open.loggedIn() is " + ff_open.loggedIn());
            var f1 = {
                fooParameter:"Testing create a JS Object with clazz Foo",
                clazz:"Foo"
            }
            if(showLogs) 
               console.log("\r\n\t test_201_Foo_Schemaless f1 before save = " + 
                   JSON.stringify(f1));
            ff_open.createObjAtUri(f1, "/Foos", function(f2) {
                ok(f2 !== null, "f2 is " + JSON.stringify(f2));
                var meta = new FFMetaData(f2);
                start(); 
            }, function(code, msg) {
                ok(false, "Error saving object: " + code + ", error message" + msg);
                start(); 
            });
        }, function(code, msg) {
            ok(false, "Error logging in: " + code + ", error message" + msg);
            start(); 
        });
    } else {
        ok(false, "no ff_open backend, skipping test");
        start();
    }
});

/*!
 * Tests that a backend will add a new member propertry of a Class definition
 * to be added by the client if allowed by configuration.
 */
asyncTest("test_202_New_Member_Allowed", function() {
    if(ff_open) {
        ok(ff_open, "have an ff_open backend");
        ff_open.login("test_user", "test_user", function() {
            ok(ff_open.loggedIn(), "ff_open.loggedIn() is " + ff_open.loggedIn());
            var memb = randomString(5);
            var f1 = new Foo();
            f1.fooParameter = "Testing adding new member";
            f1[memb] = memb;
            if(showLogs) 
                console.log(
                    "\r\n\t test_202_New_Member_Allowed f1 before save = " +
                    JSON.stringify(f1));
            ff_open.createObjAtUri(f1, "/Foo", function(f2) {
                ok(f2[memb] == memb, "f2 new member is " + memb);
                var meta = new FFMetaData(f2);
                start();
            }, function(code, msg) {
                ok(false, "Error saving object: " + code + " error message" + msg);
                start(); 
            });
        }, function(code, msg) {
            ok(false, "Error logging in: " + code + " error message" + msg);
            start(); 
        });
    } else {
        ok(false, "no ff_open backend, skipping test");
        start();
    }
});

/*!
 * Tests that a backend will not allow a new member propertry of a Class definition
 * to be added by the client if disallowed by configuration.
 */
asyncTest("test_203_New_Member_Disallowed", function() {
    if(ff_secure) {
        ok(ff_secure, "have an ff_secure backend");
        ff_secure.login("test_user", "test_user", function() {
            ok(ff_secure.loggedIn(), "ff_secure.loggedIn() is " + ff_secure.loggedIn());
            var memb = randomString(5);
            var f1 = new Foo();
            f1.fooParameter = "Testing adding new member";
            f1[memb] = memb;
            if(showLogs) 
                console.log(
                    "\r\n\t test_203_New_Member_Disallowed f1 before save = " +
                    JSON.stringify(f1));
            ff_secure.createObjAtUri(f1, "/Foo", function(f2) {
                ok(f2 == null, "f2 is " + JSON.stringify(f2));
                start(); 
            }, function(code, msg) {
                ok(true, "Error saving object: " + code + " error message" + msg);
                start(); 
            });
        }, function(code, msg) {
            ok(false, "Error logging in: " + code + " error message" + msg);
            start(); 
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
    if(ff_open) {	
        ok(ff_open, "have an ff_open backend");
        ff_open.login("test_user", "test_user", function(result) {
            // create a new Foo() object
	    console.log("ff_open.loggedIn is: " + ff_open.loggedIn());
            ok(ff_open.loggedIn(), "ff_open.loggedIn() is " + ff_open.loggedIn());
            var f1 = new Foo();
            f1.fooParameter = "Testing metadata for Foo";
            if(showLogs) 
               console.log("\r\n\t test_204_Metadata_Provided f1 before save = " + 
                   JSON.stringify(f1));
            ff_open.createObjAtUri(f1, "/Foo", function(f2) {
                var meta = new FFMetaData(f2);
                ok(meta !== null, "metadata is " + JSON.stringify(meta));
                start(); 
            }, function(code, msg) {
                ok(false, "Error saving object: " + code + ", error message" + msg);
                start(); 
            });
        }, function(code, msg) {
            ok(false, "Error logging in: " + code + ", error message" + msg);
            start(); 
        });
    } else {
        ok(false, "no ff_open backend, skipping test");
        start();
    }
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

