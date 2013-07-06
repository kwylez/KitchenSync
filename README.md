## KitchenSync

### Overview

This is a review of some of the various BaaS providers in the market today. What is different about this review is that it is an open, living and collaborative effort where anyone is invited to contribute. More importantly, it is based on a published set of functionality tests that objectively  compares them using real test code to provide verifiable results. I named the project KitchenSync because I threw in just about everything that I can think of to test the providers SDK and backend capabilities.

The methodology that I have chosen to use is to create a superset of tests that exercises all the functionality provided by all of the providers. As a result, where possible, the tests will show workaround coding where possible with a footnote explaining and shown as failing for those providers that I was not able to come up with a workaround for (corrections/ suggestions more than welcome here. Just issue pull request).

### Setup

You *should* be able to run these tests OOTB.

**NOTE**

Each project, exception for the FatFractal project, requires some form of *application / master* key. For those projects I've included **mine**. You can replace the keys with your own if you would like. Just edit the *ProjectName*`Configuration.plist.sample` with your credentials. I'm providing mine so that it makes it as easy as possible run the tests. *I can't stand a bunch of setup*. Though the FatFractal project doesn't require keys if you haven't install their local runtime then those tests **will** fail. For instructions on setting the local runtime up please visit their [getting started tutorial](http://fatfractal.com/prod/docs/getting-started/).

### Reports

* [Overview](http://cwil.es/kitchenSyncOverview)
* [Setup and Configuration - iOS](http://cwil.es/kitchenSyncSetupConfigIOS)
* [Object Construction - iOS](http://cwil.es/kitchenSyncObjConstructIOS)
* [Setup and Configuration - JS](http://cwil.es/kitchenSyncSetupConfigJS)

### TODO

* Add tests for Android
* Complete *coming soon* tests listed in the reports
* Make provider configurations easier
* Detect if user has local no-server engine setup. If they do then check at compile time if it is running or not. If they don't have it then issue notice.
