/** @namespace */

/**
 @constructor
 @classdesc Register Requests have (at least) the following fields. You may of course add other fields and they will form part of the new FFUser object.
 <ul>
 <li><strong>userName</strong> - the userName which is subsequently used in login requests
 <li><strong>password</strong>
 <li><strong>firstName</strong> - the user's first name
 <li><strong>lastName</strong> - the user's last name
 <li><strong>email</strong> - the user's email address
 </ul>
 @see FatFractal#register
 */
function RegisterRequest() {
    this.userName  = null;
    this.password  = null;
    this.firstName = null;
    this.lastName  = null;
    this.email     = null;
    return this;
}

/**
 @constructor
 @classdesc FFUsers have (at least) the following fields:
 <ul>
 <li><strong>clazz</strong> - always == 'FFUser' - maps to the FatFractal OBJECTTYPE
 <li><strong>userName</strong>
 <li><strong>firstName</strong>
 <li><strong>lastName</strong>
 <li><strong>email</strong>
 <li><strong>active</strong> - whether this FFUser is currently active or not. (For example, users may be inactive while waiting for an email verification.)
 </ul>
 @see FatFractal#login
 @see FatFractal#register
 @see FatFractal#loggedInUser
 */
function FFUser() {
    this.clazz = "FFUser";
    this.userName   = null;
    this.firstName  = null;
    this.lastName   = null;
    this.email      = null;
    this.active     = null;
    return this;
}

/**
 @constructor
 @classdesc FFMetaData have the following fields:
 <ul>
 <li><strong>clazz</strong> - maps to the FatFractal OBJECTTYPE.
 <li><strong>ffUrl</strong> - a String that contains the url (relative to the application base url) of this object.
 <li><strong>guid</strong> - a String that contains the guid of this object. Guids must be unique within a given collection.
 <li><strong>objVersion</strong> - the object's version. Objects are created with version 1, version increments on every update.
 <li><strong>createdBy</strong> - a String that contains guid of the FFUser that created the object.
 <li><strong>createdAt</strong> - a Date with the date/time stamp when the object was created as set by your app's backend.
 <li><strong>updatedBy</strong> - a String that contains the guid of the FFUser that last updated the object.
 <li><strong>updatedAt</strong> - a Date with the date/time stamp when the object was last updated as set by your app's backend.
 <li><strong>ffRefs</strong> - an Array containing metadata about any blobs or referred objects that this object has.
 <li><strong>ffUserCanEdit</strong> - when objects are retrieved from the backend, this flag lets the client know whether this user has permission to modify it.
 </ul>
 @see FatFractal#login
 @see FatFractal#register
 @see FatFractal#loggedInUser
 */
function FFMetaData(obj) {
    this.clazz         = null;
    this.ffUrl         = null;
    this.guid          = null;
    this.ffRl          = null;
    this.objVersion    = null;
    this.createdBy     = null;
    this.createdAt     = null;
    this.updatedBy     = null;
    this.updatedAt     = null;
    this.ffRefs        = [];
    this.ffUserCanEdit = false;
    if(obj) {
        this.clazz         = obj.clazz;
        this.ffUrl         = obj.ffUrl;
        this.guid          = obj.guid;
        this.ffRl          = obj.ffRl;
        this.objVersion    = new Number(obj.objVersion);
        this.createdBy     = obj.createdBy;
        this.createdAt     = new Date(obj.createdAt);
        this.updatedBy     = obj.updatedBy;
        this.updatedAt     = new Date(obj.updatedAt);
        this.ffRefs        = [obj.ffRefs];
        this.ffUserCanEdit = obj.ffUserCanEdit;
    }
   this.description = "FFMetaData:" +
       ", clazz: "         + this.clazz + 
       ", ffUrl: "         + this.ffUrl + 
       ", guid: "          + this.guid + 
       ", ffRl: "          + this.ffRl + 
       ", objVersion: "    + this.objVersion + 
       ", createdBy: "     + this.createdBy + 
       ", createdAt: "     + this.createdAt + 
       ", updatedBy: "     + this.updatedBy + 
       ", updatedAt: "     + this.updatedAt + 
       ", ffRefs: "        + this.ffRefs + 
       ", ffUserCanEdit: " + this.ffUserCanEdit;
    return this;
}

/**
 * @classdesc OAuth token object
 * @param {String} token OAuth token
 * @param {String} secret OAuth secret (OAuth 1)
 * @constructor
 * @see FatFractal#setTokenForScriptAuthService
 * @see FatFractal#getTokenForScriptAuthService
 */
function Token(token, secret) {
    this.token = token;
    this.secret = secret;
}

/** Default constructor for this class
 @constructor
 @return {FatFractal} an instance of the FatFractal class
 */
function FatFractal() {
    /*
    This ensures that no errors are thrown with ie if the console is not enabled.
     */
    if (typeof console == "undefined")
        console = {};
    if (typeof console.log == "undefined")
        console.log = function (/**...*/ args) {};

    /*
    This local variable holds the instance of this function.
     */
    var m_ff = this;

    /*
    Prevent errors with Internet Explorer if window.console is not available.
     */
    if (! window.console)
        window.console = { log : function (/**...*/ args) {} };

    /*
    Prevent errors with Internet Explorer if Array.isArray is not available.
     */
    if(!Array.isArray) {
        Array.isArray = function (vArg) {
            return Object.prototype.toString.call(vArg) === "[object Array]";
        };
    }

    /*
    Prevent errors with Internet Explorer if Object.defineProperty is not available.
     */
    if (Object.prototype.__defineGetter__&&!Object.defineProperty) {
       Object.defineProperty=function(obj,prop,desc) {
           if ("get" in desc) obj.__defineGetter__(prop,desc.get);
           if ("set" in desc) obj.__defineSetter__(prop,desc.set);
       }
    }

    /*
    This local variable (String) will override the natural relative addressing with a specified baseUrl, defaults to null.
     */
    var m_baseUrl = null;

    /*
    This local variable (Boolean) contains the LoggedIn status for the FatFractal library, defaults to false.
     */
    var m_loggedIn = false;

    /*
    This local variable (FFUSer) holds the LoggedInUser object for the FatFractal library, defaults to null.
     */
    var m_loggedInUser = null;

    /*
    This local variable (Object) contains the latest server status message.
     */
    var m_serverStatusMessage = null;

    /*
    This local variable (Object) contains items that are cached in memory during the browser session.
     */
    var m_cache = {};

    /*
    This local variable contains items are to store separate from JSON structure stores (blobs).
     */
    var m_pendingBlobs = {};

    /*
    This local variable (Boolean) holds the setting of the Debug mode for the FatFractal library, defaults to false.
     */
    var m_debug = false;

    /*
    This local variable (String) holds the sessionId for the FatFractal library, defaults to null.
     */
    var m_sessionId = null;

    /*
    This local variable (Boolean) specifies whether to "simulate" cookies by adding request headers - for example if cookies are disabled by the user.
    Defaults to false.
     */
    var m_simulateCookies = false;

    /*
    This object stores callback URIs used by ScriptAuth OAuth services.
    URIs are keyed by ScriptAuth service name, e.g. "FACEBOOK"
     */
    var m_scriptAuthCallbackUris = {};

    /*
    ScriptAuth access tokens.
     */
    var m_scriptAuthTokens = {};

    /*
    ScriptAuth request tokens, used by OAuth 1.0a services such as Twitter.
     */
    var m_scriptAuthRequestTokens = {};

    /*
    When true, then when Object A is loaded, its references are also loaded asynchronously in a separate request.
    <br/> It's usually more desirable to have this property set to false, and use the ?depthRefs query parameter to load references
     */
    var m_autoLoadRefs = true;

    /*
    This local variable (String) contains the version of the FatFractal Javascript Client-Side SDK.
     */
    var m_version = "FF_JAVASCRIPT_SDK_BETA_5_R2221";

    /**
     * ScriptAuth service name for Facebook.
     * @type {string}
     * @see #authUriForScriptAuthService
     * @see #retrieveAccessTokenForScriptAuthService
     * @see #registerWithScriptAuthService
     * @see #loginWithScriptAuthService
     */
    this.SCRIPT_AUTH_SERVICE_FACEBOOK = "FACEBOOK";

    /**
     * ScriptAuth service name for Twitter.
     * @type {string}
     * @see #authUriForScriptAuthService
     * @see #retrieveAccessTokenForScriptAuthService
     * @see #registerWithScriptAuthService
     * @see #loginWithScriptAuthService
     */
    this.SCRIPT_AUTH_SERVICE_TWITTER = "TWITTER";

    /*
    This method will determine the File API support for the browser in use.
    @return {Boolean} returns true if the HTML5 File API is supported on this browser, else false.
     */
    var m_fileAPI = function() {
        return window.File && window.FileList && window.FileReader;
    };

    /**
    This method will determine if the XMLHttpRequest is version 2 or not.
    @return {Boolean} returns true if the HTML5 XMLHttpRequest is supported on this browser.
     */
    var m_http2 = function() {
        var xhr = new XMLHttpRequest();
        return xhr.upload ? true : false;
    };

    /**
    This method will set the Debug mode for the FatFractal library to true, which generates
    console.log messages for most operations or false which will generate error messages only.
    @param {Boolean} tf will set the debug mode to true or false.
     */
    this.setDebug = function(tf) {
       if(tf) {
          m_debug = true;
       }
       else m_debug = false;
       console.log("FatFractal().setDebug(boolean) set debug mode to: " + m_debug);
    };

    /**
    This method will set the AutoRefs mode for the FatFractal library
    @param {Boolean} tf true or false
     */
    this.setAutoLoadRefs = function(tf) {
       if(tf)
           m_autoLoadRefs = true;
       else
           m_autoLoadRefs = false;
    };

    /**
    This method will get the Debug mode for the FatFractal library.
    @return {Boolean} returns true if the debug mode to true, else returns false.
     */
    this.getDebug = function() {
       if(m_debug) console.log("FatFractal().getDebug() determined debug mode is: " + m_debug);
       if(m_debug) return true;
       else return false;
    };

    /**
    This method will returns the version of the FatFractal library.
    @return {String} returns the version of the FatFractal library.
     */
    this.version = function() {
       if(m_debug) console.log("FatFractal().version() determined version is: " + m_version);
       return m_version;
    };

    function getWindowURL() {
        var url = location.href;  // entire url including querystring - also: window.location.href;
        if(m_debug) console.log("FatFractal().getWindowURL(String) url is: " + url);
        var baseURL = url.substring(0, url.indexOf('/', 14));
        if(m_debug) console.log("FatFractal().getWindowURL(String) baseURL is: " + baseURL);
        var pathname, index1, index2;

        pathname = location.pathname;  // window.location.pathname;
        index1 = url.indexOf(pathname);
        index2 = url.indexOf("/", index1 + 1);
        var baseLocalUrl = url.substr(0, index2) + "/";
        if(m_debug) console.log("FatFractal().getWindowURL(String) returning : " + baseLocalUrl);
        return baseLocalUrl;
    }

    var m_validUrl = function(/**String*/ url, /**String=*/ type) {
        if(m_debug) console.log("FatFractal().m_validUrl received url: " + url);
        if(m_debug) console.log("FatFractal().m_validUrl received type: " + type);
    	var validUrl = null;
        if(type == "extension") {
            if(url.substring(0,1) == "/") {
                if(url.substring(0,8) == "/ff/ext/") validUrl = url.substring(1,url.length);
                else validUrl = "ff/ext" + url;
            }
            else if(url.substring(0,7) == "ff/ext/") validUrl = url;
            else validUrl = "ff/ext/" + url;
        } else if(type == "resources" || type == "resource") {
            if(url.substring(0,1) == "/") {
                if(url.substring(0,14) == "/ff/resources/") validUrl = url.substring(1,url.length);
                else validUrl = "ff/resources" + url;
            }
            else if(url.substring(0,13) == "ff/resources/") validUrl = url;
            else validUrl = "ff/resources/" + url;
        } else {
            if(url.substring(0,1) == "/") {
                if(url.substring(0,14) == "/ff/resources/") validUrl = url.substring(1,url.length);
                else if(url.substring(0,8) == "/ff/ext/") validUrl = url.substring(1,url.length);
                else validUrl = "ff/resources" + url;
            }
            else if(url.substring(0,13) == "ff/resources/") validUrl = url;
            else if(url.substring(0,7) == "ff/ext/") validUrl = url;
            else validUrl = "ff/resources/" + url;
        }
        if(m_debug) console.log("FatFractal().m_validUrl returned validUrl: " + validUrl);
        return validUrl;
    };

    /**
    This method will set the base url for the FatFractal library.
    @param {String} base the value the base href will be set to.
    @return {String} the value the base href has been set to.
     */
    this.setBaseUrl = function(base) {
        if(base) {
            if(base.charAt(base.length -1) == "/") {
                m_baseUrl = base;
            } else m_baseUrl = base + "/";
        }
        else m_baseUrl = null;
        if(m_debug) console.log("FatFractal().setBaseUrl(String) m_baseUrl to: " + m_baseUrl);
        return m_baseUrl;
    };

    /**
    This method will get the base url for the FatFractal library.
    @return {String} the value the base href has been set to.
     */
    this.getBaseUrl = function() {
        var url = null;
        if(m_baseUrl == null) {
            url = getWindowURL();
            if(m_debug) console.log("FatFractal().getBaseUrl() window location is: " + url);
        }
        else url = m_baseUrl;

        if(m_debug) console.log("FatFractal().getBaseUrl() determined base url is: " + url);
        return url;
    };

    /**
     * Obsolete
     * @deprecated
     */
    this.setSSLUrl = function() {
    };

    /**
     * Obsolete - now just returns getBaseUrl
     * @deprecated
     * @return {String}
     */
    this.getSSLUrl = function() {
        return this.getBaseUrl();
    };

    function pSetSimulateCookies (/**Boolean*/ b) {
        if (b === true)
            m_simulateCookies = true;
        else
            m_simulateCookies = false;
    }

    this.setSimulateCookies = pSetSimulateCookies;

    /**
    This method is used to get the LoggedIn state for the FatFractal library.
    @return {Boolean} returns true if user is logged in and a valid session exists, else returns false.
    @see #login
    @see #register
     */
    this.loggedIn = function() {
        m_sessionId = retrieveSessionID();
        if(m_debug) console.log("FatFractal().loggedIn() determined m_sessionId is: " + m_sessionId);
        if(retrieveFFUser().guid) m_loggedInUser = retrieveFFUser();
        if(m_debug) console.log("FatFractal().loggedIn() determined m_loggedInUser is: " + JSON.stringify(m_loggedInUser));
        if(m_sessionId && m_loggedInUser) m_loggedIn = true;
        else {
            m_sessionId = null;
            m_loggedIn = false;
            m_loggedInUser = null;
            clearSessionInfo();
        }
        if(m_debug) console.log("FatFractal().loggedIn() determined loggedIn is: " + m_loggedIn);
        return m_loggedIn;
    };

    /**
    This method will get the LoggedIn state for the FatFractal library.
    @return {FFUser} returns the FFUser that is logged in if a valid session exists, else returns null.
    @see #login
    @see #register
     */
    this.loggedInUser = function() {
        m_sessionId = retrieveSessionID();
        if(m_debug) console.log("FatFractal().loggedInUser() determined m_sessionId is: " + m_sessionId);
        if(retrieveFFUser().guid) m_loggedInUser = retrieveFFUser();
        if(m_debug) console.log("FatFractal().loggedInUser() determined m_loggedInUser is: " + JSON.stringify(m_loggedInUser));
        if(m_sessionId && m_loggedInUser) m_loggedIn = true;
        else {
            m_sessionId = null;
            m_loggedIn = false;
            m_loggedInUser = null;
            clearSessionInfo();
        }
        if(m_debug) console.log("FatFractal().loggedInUser() determined m_loggedInUser is: " + JSON.stringify(m_loggedInUser));
        return m_loggedInUser;
    };

    /**
    Get the sessionId . This can only be set with login or register methods.
    @return {String} Returns the sessionId if logged in, else null.
    @see #login
    @see #register
     */
    this.sessionId = function() {
        if(m_sessionId == null) {
            m_sessionId = retrieveSessionID();
            if(m_debug) console.log("FatFractal().sessionId() determined m_sessionId is: " + m_sessionId);
            if(retrieveFFUser().guid) m_loggedInUser = retrieveFFUser();
            if(m_debug) console.log("FatFractal().sessionId() determined m_loggedInUser is: " + JSON.stringify(m_loggedInUser));
            if(m_sessionId && m_loggedInUser) m_loggedIn = true;
            else {
                m_sessionId = null;
                m_loggedIn = false;
                m_loggedInUser = null;
                clearSessionInfo();
            }
        }
        if(m_debug) console.log("FatFractal().sessionId() determined sessionId is: " + m_sessionId);
        return m_sessionId;
    };

    /**
     * Get callback URI for the specified ScriptAuth service.
     * @param {String} scriptAuthService The ScriptAuth service
     * @return {String} Callback URI
     */
    this.getCallbackUriForScriptAuthService = function(scriptAuthService) {
        return m_scriptAuthCallbackUris[scriptAuthService];
    };

    /**
     * Set callback URI for the specified ScriptAuth service.
     * @param {String} scriptAuthService The ScriptAuth service
     * @param {String} callbackUri The callback URI
     */
    this.setCallbackUriForScriptAuthService = function(scriptAuthService, callbackUri) {
        m_scriptAuthCallbackUris[scriptAuthService] = callbackUri;
    };

    /**
     * Get access token for the specified ScriptAuth service.
     * @param {String} scriptAuthService The ScriptAuth service
     * @return {Token} Access token
     */
    this.getTokenForScriptAuthService = function(scriptAuthService) {
        return m_scriptAuthTokens[scriptAuthService];
    };

    /**
     * Set access token for the specified ScriptAuth service.
     * @param {String} scriptAuthService The ScriptAuth service
     * @param {Token} token The access token
     */
    this.setTokenForScriptAuthService = function(scriptAuthService, token) {
        m_scriptAuthTokens[scriptAuthService] = token;
    };

    /**
     * Clears the access token for the specified ScriptAuth service.
     * @param {String} scriptAuthService The ScriptAuth service
     */
    this.clearTokenForScriptAuthService = function(scriptAuthService) {
        delete m_scriptAuthTokens[scriptAuthService];
    };

    /**
     * Get request token for the specified ScriptAuth service.
     * @param {String} scriptAuthService The ScriptAuth service
     * @return {Token} Access token
     */
    this.getRequestTokenForScriptAuthService = function(scriptAuthService) {
        return m_scriptAuthRequestTokens[scriptAuthService];
    };

    /**
     * Set request token for the specified ScriptAuth service.
     * @param {String} scriptAuthService The ScriptAuth service
     * @param {Token} token The access token
     */
    this.setRequestTokenForScriptAuthService = function(scriptAuthService, token) {
        m_scriptAuthRequestTokens[scriptAuthService] = token;
    };

    /**
     * Clears the request token for the specified ScriptAuth service.
     * @param {String} scriptAuthService The ScriptAuth service
     */
    this.clearRequestTokenForScriptAuthService = function(scriptAuthService) {
        delete m_scriptAuthRequestTokens[scriptAuthService];
    };

    /**
    This method will get the last server status message from an HTTP request.
    @return {String} returns the status message from the last HTTP request.
     */
    this.serverStatusMessage = function() {return m_serverStatusMessage;};

    /**
    Default error handler that will report errors to the browser console. Used if a caller doesn't supply a callback.
     */
    this.defaultErrorCallback = function(status, message) {
       if(console.log) console.log(status + ": " + message);
    };

    var m_defaultErrorCallback = this.defaultErrorCallback;

    /**
    Default success handler that will report errors to the browser console. Used if a caller doesn't supply a callback.
     */
    this.defaultSuccessCallback = function(response) {
       if(console.log) console.log(response);
    };

    var m_defaultSuccessCallback = this.defaultSuccessCallback;

    /*
    This method will serialize an object to JSON.
     */
    this._toJSON = JSON.stringify;

    /*
    This method will deserialize an object to JSON.
     */
    this._fromJSON = JSON.parse;

    this.AjaxParams = function() {
        this.dataType = null;
        this.contentType = null;
        this.contentLength = null;
        this.contentName = null;
        this.fileName = null;
        this.fileType = null;
        this.fileSize = null;
    };

    function m_ajax (ajaxParams) {
        function oldIEAjax(type, url, data, success, error) {
            var xdr = new XDomainRequest();
            xdr.onload = function() {
                var response;
                try {
                    response = JSON.parse(xdr.responseText);
                } catch (e) {
                    if (error)
                        error(xdr.status,xdr.responseText);
                    else
                        m_defaultErrorCallback(xdr.status,xdr.responseText)
                }
                if (response) {
                    if (success)
                        success(response, xdr);
                }
            };
            xdr.onerror = xdr.ontimeout = function() {
                if(console.error) console.error("FatFractal.m_ajax " + xdr.status + ", " + xdr.responseText);
                if (error) error(xdr.status,xdr.responseText);
            };
            xdr.onprogress = function() {};
            xdr.open(type, url);
            xdr.send(data);
        }

        var xmlHTTP;
        try{
            xmlHTTP = new XMLHttpRequest();
        } catch (e){
            // Internet Explorer Browsers
            try{
                xmlHTTP = new ActiveXObject("Msxml2.XMLHTTP");
            } catch (e) {
                try{
                    xmlHTTP = new ActiveXObject("Microsoft.XMLHTTP");
                } catch (e){
                    try{
                        if (typeof XDomainRequest !== "undefined") {
                            oldIEAjax(ajaxParams.type, ajaxParams.url, ajaxParams.data, ajaxParams.success, ajaxParams.error);
                        }
                    } catch (e){
                        var exceptionMessage = "A browser that supports AJAX requests is required!";
                        alert(exceptionMessage);
                        throw exceptionMessage;
                    }
                }
            }
        }
        var handled = false;
        xmlHTTP.open(ajaxParams.type.toUpperCase(),  ajaxParams.url, true);
        if(!ajaxParams.dataType) xmlHTTP.setRequestHeader("Data-type","application/json");
        else if((ajaxParams.dataType.toLowerCase() == "application/json") || (ajaxParams.dataType.toLowerCase() == "json")) xmlHTTP.setRequestHeader("Data-type","application/json");
        else xmlHTTP.setRequestHeader("Data-type",ajaxParams.dataType);
        if(!ajaxParams.contentType) xmlHTTP.setRequestHeader("Content-type","application/json");
        else if((ajaxParams.contentType.toLowerCase() == "application/json") || (ajaxParams.contentType.toLowerCase() == "json")) xmlHTTP.setRequestHeader("Content-type","application/json");
        else xmlHTTP.setRequestHeader("Content-type",ajaxParams.contentType);
        // Blob headers
        if(ajaxParams.contentLength) xmlHTTP.setRequestHeader('Content-Length', ajaxParams.contentLength);
        if(ajaxParams.contentName) xmlHTTP.setRequestHeader('Content-Name', ajaxParams.contentName);
        if(ajaxParams.fileName) xmlHTTP.setRequestHeader('x-file-name', ajaxParams.fileName);
        if(ajaxParams.fileSize) xmlHTTP.setRequestHeader('x-file-size', ajaxParams.fileSize);
        if(ajaxParams.fileType) xmlHTTP.setRequestHeader('x-file-type', ajaxParams.fileType);
        //xmlHTTP.setRequestHeader("Connection", "close");
        if (m_simulateCookies && m_loggedIn) {
            xmlHTTP.setRequestHeader("X-Ff-Auth-User-Guid", m_loggedInUser.guid);
            xmlHTTP.setRequestHeader("X-Ff-Auth-Session-Id", m_sessionId);
        }
        xmlHTTP.send(ajaxParams.data);
        xmlHTTP.onreadystatechange = function() {
            if(xmlHTTP.readyState == 4) {
                if(m_debug) console.log("xmlHTTP.status: " + xmlHTTP.status + ", xmlHTTP.readyState: " + xmlHTTP.readyState + ", xmlHTTP.responseText: " + xmlHTTP.responseText);
                if (handled) {
                  return;
                }
                handled = true;
                if(xmlHTTP.status >= 200 && xmlHTTP.status < 300) {
                    var response = JSON.parse(xmlHTTP.responseText);
                    if (ajaxParams.success) {
                        if(m_debug) console.log(response);
                        ajaxParams.success(response);
                    }
                    else if(m_debug) console.log("no success callback");
                } else if (xmlHTTP.status == 304) {
                    // check if in cache
                }
                else {
                    if(console.error) console.error(xmlHTTP.status + ": " + xmlHTTP.statusText);
                    if (xmlHTTP.status == 401) {
                        if(console.error) console.error("Got a 401 - clearing all session info");
                        m_loggedIn = false;
                        m_loggedInUser = null;
                        m_sessionId = null;
                        clearSessionInfo();
                    }
                    if(ajaxParams.error) {
                        ajaxParams.error(xmlHTTP);
                        if(console.error) console.error("xmlHTTP : " + JSON.stringify(xmlHTTP));
                    }
                    else if(m_debug) console.log("no success callback");
                }
            }
            else if(m_debug) console.log("xmlHTTP.status: " + xmlHTTP.status + ", xmlHTTP.readyState: " + xmlHTTP.readyState + ", xmlHTTP.responseText: " + xmlHTTP.responseText);
        }
    }

    /**
    Set the default permissions for this object. If object has a specific ACL, it is removed so that the defaults
    apply (PERMIT commands or, if there are no PERMIT commands, the overall system default (public can read, only
    creator can write)
    @param obj - the object for which we are setting permissions
    @param {Function} successCallback
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     */
    this.setDefaultPermission = function(obj, successCallback, errorCallback) {
        var url = m_validUrl(obj.ffUrl) + "/ffACL";
        if(m_baseUrl) url = m_baseUrl + url;

        m_ajax({
            type: "PUT",
            url: url,
            dataType: 'json',
            contentType:'application/json',
            data: null,
            success: function(response) {
                if (successCallback) successCallback(response);
            },
            error: function(xmlHTTP) {
                if(console.error) console.error("FatFractal.setDefaultPermission " + xmlHTTP.status + ", " + xmlHTTP.responseText);
                m_serverStatusMessage = "HTTP request failed - response code was " + xmlHTTP.status + " responseText was " + xmlHTTP.responseText;
                if (errorCallback) errorCallback(xmlHTTP.status, xmlHTTP.responseText);
            }
        });
    };

    this.PermissionObject = function(_readUsers, _readGroups, _writeUsers, _writeGroups) {
        this.readUsers   = _readUsers   ? _readUsers   : null;
        this.readGroups  = _readGroups  ? _readGroups  : null;
        this.writeUsers  = _writeUsers  ? _writeUsers  : null;
        this.writeGroups = _writeGroups ? _writeGroups : null;
    };

    /**
    Using this method, permissions are granted to read or write an object by user and by group. Once permission is granted to
    read or write an object by user, that permission persists until the method is reissued. If users are added or taken out of
    groups, the corresponding permissions for that group obtain dynamically.
    @param obj - the object for which we are setting permissions
    @param readUsers   - must be an array of FFUser      each of which is to be given read  access to the object
    @param writeUsers  - must be an array of FFUser      each of which is to be given write access to the object
    @param readGroups  - must be an array of FFUserGroup each of which is to be given read  access to the object
    @param writeGroups - must be an array of FFUserGroup each of which is to be given write access to the object
    @param {Function} successCallback Function with one argument - the registered FFUser
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     */
    this.setPermission = function(obj, readUsers, readGroups, writeUsers, writeGroups, successCallback, errorCallback) {
        var request = new this.PermissionObject (readUsers, readGroups, writeUsers, writeGroups);

        var url = m_validUrl(obj.ffUrl) + "/ffACL";
        if(m_baseUrl) url = m_baseUrl + url;

        m_ajax({
            type: "PUT",
            url: url,
            dataType: 'json',
            contentType:'application/json',
            data: m_ff._toJSON(request),
            success: function(response) {
                if (successCallback) successCallback(response);
            },
            error: function(xmlHTTP) {
                if(console.error) console.error("FatFractal.setPermission " + xmlHTTP.status + ", " + xmlHTTP.responseText);
                m_serverStatusMessage = "HTTP request failed - response code was " + xmlHTTP.status + " responseText was " + xmlHTTP.responseText;
                if (errorCallback) errorCallback(xmlHTTP.status, xmlHTTP.responseText);
            }
        });
    };

    /**
    Get all items from the object's named grab bag
    <br>For example, grabBagGetAll(anOrder, "OrderLines")
    @param parentObj - The object from whose grab bag we are retrieving
    @param gbName - The grab bag name
    @param {Function} successCallback Function with one argument - the registered FFUser
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     */
    this.grabBagGetAll = function(parentObj, gbName, successCallback, errorCallback) {
        m_ff.grabBagGetAllForQuery(parentObj, gbName, null, successCallback, errorCallback);
    };

    /**
    Get items, via query, from the object's named grab bag
    <br>For example, grabBagGetAllForQuery(anOrder, "OrderLines", "(orderLineValue gt 1000)")
    @param parentObj - The object from whose grab bag we are retrieving
    @param gbName - The grab bag name
    @param query - The query string in FatFractal Query Language format
    @param {Function} successCallback Function with one argument - the registered FFUser
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     */
    this.grabBagGetAllForQuery = function(parentObj, gbName, query, successCallback, errorCallback) {
        var grabBagRelativeUrl = parentObj.ffUrl + "/" + gbName;

        if(query != null)
            grabBagRelativeUrl = grabBagRelativeUrl + "/(" + query + ")";

        m_ff.getArrayFromUri(grabBagRelativeUrl, successCallback, errorCallback);
    };

    /**
    Add an item to the object's named grab bag.
    <br>For example, grabBagAdd(anOrderLine, anOrder, "OrderLines")
    @param item - The item which is to be added
    @param parentObj - The object whose grab bag is going to be updated
    @param gbName - The grab bag name
    @param {Function} successCallback Function with one argument - the registered FFUser
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     */
    this.grabBagAdd = function(item, parentObj, gbName, successCallback, errorCallback) {
        m_ff.handleGrabBagRequest(item, parentObj, gbName, true, successCallback, errorCallback);
    };

    /**
    Remove an item from the object's named grab bag.
    <br>For example, grabBagRemove(anOrderLine, anOrder, "OrderLines")
    @param item - The item which is to be removed
    @param parentObj - The object whose grab bag is going to be updated
    @param gbName - The grab bag name
    @param {Function} successCallback Function with one argument - the registered FFUser
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     */
    this.grabBagRemove = function(item, parentObj, gbName, successCallback, errorCallback) {
        m_ff.handleGrabBagRequest(item, parentObj, gbName, false, successCallback, errorCallback);
    };

    this.GrabBagAddOrRemoveObj = function(_ffUrl, _addOrRemove) {
        this.ffUrl       = _ffUrl       ? _ffUrl        : null;
        this.AddOrRemove = _addOrRemove ? _addOrRemove  : null;
    };

    /*
    This method is responsible for interacting with the FatFractal backend with respect to GrabBags.
    <br>For example, grabBagRemove(anOrderLine, anOrder, "OrderLines")
    @param item - The item which is to be removed
    @param parentObj - The object whose grab bag is going to be updated
    @param gbName - The grab bag name
    @param gbAction - The action to be performed (ADD or REMOVE).
    @param {Function} successCallback
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     */
    this.handleGrabBagRequest = function(item, parentObj, gbName, add, successCallback, errorCallback) {
        if(! successCallback) throw new Error("FatFractal.handleGrabBagRequest: successCallback not supplied");
        if(! errorCallback) errorCallback = m_ff.defaultErrorCallback;
        if(! errorCallback) throw new Error("FatFractal.handleGrabBagRequest: errorCallback not supplied");

        var addOrRemove = (add ? "ADD" : "REMOVE");

        var grabBagRelativeUrl = parentObj.ffUrl + "/" + gbName;
        var itemUrl = item.ffUrl;

        var request = new this.GrabBagAddOrRemoveObj(itemUrl, addOrRemove);
        var url = m_validUrl(grabBagRelativeUrl);
        if(m_baseUrl) url = m_baseUrl + url;

        m_ajax({
            type: "POST",
            url: url,
            dataType: 'json',
            contentType:'application/json',
            data: m_ff._toJSON(request),
            success: function(response) {
                if (successCallback) successCallback(response);
            },
            error: function(xmlHTTP) {
                if(console.error) console.error("FatFractal.handleGrabBagRequest " + xmlHTTP.status + ", " + xmlHTTP.responseText);
                m_serverStatusMessage = "HTTP request failed - response code was " + xmlHTTP.status + " responseText was " + xmlHTTP.responseText;
                if (errorCallback) errorCallback(xmlHTTP.status, xmlHTTP.responseText);
            }
        });
    };

    /**
    Registers a new FFUSer
    Currently a successful registration will also log the user in.
    @param {RegisterRequest} registerRequest
    @param {Function} successCallback Function with one argument - the registered FFUser
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
    Additionally upon success, the values for some other properties that are accessible by your
    application are set:
     <br><b>String sessionId</b> is set to the SessionId returned by your app's backend
     <br><b>String loggedInUser</b> is set to the FFUser response from the backend
     <br><b>BOOL loggedIn</b> is set to true
    @see RegisterRequest
    @see FFUser
    @see #loggedIn
    @see #sessionId
    @see #loggedInUser
    */
    this.register = function(registerRequest, successCallback, errorCallback) {
        if(! successCallback) throw new Error("FatFractal.register: successCallback not supplied");
        if(! errorCallback) errorCallback = m_ff.defaultErrorCallback;
        if(! errorCallback) throw new Error("FatFractal.register: errorCallback not supplied");

        var url = this.getBaseUrl() + "ff/register";

        m_ajax({
            type: "POST",
            url: url,
            dataType: 'json',
            contentType:'application/json',
            data: m_ff._toJSON(registerRequest),
            success: function(response) {
                if(response.result != null && response.result.loggedInUser != null) {
                    m_loggedIn = true;
                    m_loggedInUser = response.result.loggedInUser;
                    m_sessionId = response.result.authResult.session.sessionId;
                    storeSessionInfo(m_loggedInUser, m_sessionId);
                    if(successCallback) successCallback(m_loggedInUser);
                }
                else
                    if(errorCallback) errorCallback(500, "No result.loggedInUser in response")
            },
            error: function(xmlHTTP) {
                if(console.error) console.error("FatFractal.register " + xmlHTTP.status + ", " + xmlHTTP.responseText);
                m_serverStatusMessage = "HTTP request failed - response code was " + xmlHTTP.status + " responseText was " + xmlHTTP.responseText;
                if (errorCallback) errorCallback(xmlHTTP.status, xmlHTTP.responseText);
            }
        });
    };

    /**
     * Register a new FFUser using ScriptAuth. Credential is constructed using a stored token, retrieved via {@link FatFractal#getTokenForScriptAuthService}.
     * @param {String} scriptAuthService The ScriptAuth service to use
     * @param {Function} successCallback Function with one argument - the registered FFUser
     * @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     * @see #retrieveAccessTokenForScriptAuthService
     * @see #setTokenForScriptAuthService
     */
    this.registerWithScriptAuthService = function(scriptAuthService, successCallback, errorCallback) {
        var rr = new RegisterRequest();
        rr.authDomain = "SCRIPT";
        rr.scriptAuthService = scriptAuthService;

        var token = this.getTokenForScriptAuthService(scriptAuthService);
        rr.token = token.token;
        rr.secret = token.secret;

        this.register(rr, successCallback, errorCallback);
    };

    /**
    Login using a username and password.
     <br>login will succeed if there exists a registered FFUser with this userName and password. If you have set
    AllowAutoRegistration to true in your application.ffdl configuration, then login will also succeed if this userName
    does not exist in your app's backend.
    @param {String} username - the user name - can be anything, doesn't have to be meaningful
    @param {String} password - the password
    @param {Function} successCallback Function with one argument - the registered FFUser
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
    Additionally upon success, the values for some other properties that are accessible by your
    application are set:
     <br><b>String sessionId</b> is set to the SessionId returned by your app's backend
     <br><b>String loggedInUser</b> is set to the FFUser response from the backend
     <br><b>BOOL this.loggedIn()</b> will return true.
    @see #register
    @see FFUser
    @see #loggedIn
    @see #sessionId
    @see #loggedInUser
     */
    this.login = function(username, password, successCallback, errorCallback) {
        var credential = {
            userName: username,
            password: password
        };
        this.loginUsingCredential(credential, successCallback, errorCallback);
    };

    /**
    Login using console credentials.
    <br>login will succeed if (a) console credentials are valid (b) console user has write permission to this backend and (c) system user is allowed to log in
    @param {String} username - the FF console user name
    @param {String} password - the password
    @param {Function} successCallback Function with one argument - the FFUser
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
    Additionally upon success, the values for some other properties that are accessible by your
    application are set:
     <br><b>String sessionId</b> is set to the SessionId returned by your app's backend
     <br><b>String loggedInUser</b> is set to the FFUser response from the backend
     <br><b>BOOL this.loggedIn()</b> will return true.
    @see #register
    @see #login
    @see FFUser
    @see #loggedIn
    @see #sessionId
    @see #loggedInUser
     */
    this.loginUsingConsoleCredentials = function(username, password, successCallback, errorCallback) {
        var credential = {
            authDomain: 'FFCONSOLE',
            userName: username,
            password: password
        };
        this.loginUsingCredential(credential, successCallback, errorCallback);
    };

    /**
     * Log in using ScriptAuth service. Credential is constructed using a stored token, retrieved via {@link FatFractal#getTokenForScriptAuthService}.
     * @param {String} scriptAuthService The ScriptAuth service
     * @param {Function} successCallback Function with one argument - the FFUser
     * @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     * @see #retrieveAccessTokenForScriptAuthService
     * @see #setTokenForScriptAuthService
     */
    this.loginWithScriptAuthService = function(scriptAuthService, successCallback, errorCallback) {
        var token = this.getTokenForScriptAuthService(scriptAuthService);
        var credential = {
            authDomain: "SCRIPT",
            scriptAuthService: scriptAuthService,
            token: token.token,
            secret: token.secret
        };
        this.loginUsingCredential(credential, successCallback, errorCallback);
    };

    /**
     * General log in function, allowing arbitrary credentials to be passed.
     * @param credential Credential map
     * @param {Function} successCallback Function with one argument - the FFUser
     * @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     */
    this.loginUsingCredential = function(credential, successCallback, errorCallback) {
        if(! successCallback) throw new Error("FatFractal.loginUsingConsoleCredentials: successCallback not supplied");
        if(! errorCallback) errorCallback = m_ff.defaultErrorCallback;
        if(! errorCallback) throw new Error("FatFractal.loginUsingConsoleCredentials: errorCallback not supplied");

        var req = {
            credential: credential
        };

        var url = this.getBaseUrl() + "ff/login";

        m_ajax({
                   type: "POST",
                   url: url,
                   dataType: 'json',
                   contentType: 'application/json',
                   data: m_ff._toJSON(req),
                   success: function (response) {
                       if (response.result != null && response.result.loggedInUser != null) {
                           m_loggedIn = true;
                           m_loggedInUser = response.result.loggedInUser;
                           m_sessionId = response.result.authResult.session.sessionId;
                           storeSessionInfo(m_loggedInUser, m_sessionId);
                           if (successCallback) successCallback(m_loggedInUser);
                       }
                       else if (errorCallback) errorCallback(500, "No result.loggedInUser in response")
                   },
                   error: function (xmlHTTP) {
                       if (console.error) console.error("FatFractal.login " + xmlHTTP.status + ", " + xmlHTTP.responseText);
                       m_serverStatusMessage = "HTTP request failed - response code was " + xmlHTTP.status + " responseText was " + xmlHTTP.responseText;
                       if (errorCallback) errorCallback(xmlHTTP.status, xmlHTTP.responseText);
                   }
               });
    };


    /**
    Explicit logout from your app's backend.
    @param {Function} successCallback Function with one argument - the response object
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
    <br>Sets #loggedInSessionId to null, #loggedInUserName to null, #loggedInUser to null, #loggedIn to false
    */
    this.logout = function(successCallback, errorCallback) {
        //if(! successCallback) throw new Error("FatFractal.logout: successCallback not supplied");
        if(! errorCallback) errorCallback = m_ff.defaultErrorCallback;
        if(! successCallback) successCallback = m_ff.defaultSuccessCallback;
        //if(! errorCallback) throw new Error("FatFractal.logout: errorCallback not supplied");

        var url = "ff/logout";
        if(m_baseUrl) url = m_baseUrl + "ff/logout";

        m_ajax({
            type: "POST",
            url: url,
            dataType: 'json',
            contentType:'application/json',
            success: function(response) {
                m_loggedIn = false;
                m_loggedInUser = null;
                m_sessionId = null;
                clearSessionInfo();
                if(successCallback) successCallback(response);
            },
            error: function(xmlHTTP) {
                if(console.error) console.error("FatFractal.logout " + xmlHTTP.status + ", " + xmlHTTP.responseText);
                m_serverStatusMessage = "HTTP request failed - response code was " + xmlHTTP.status + " responseText was " + xmlHTTP.responseText;
                if (errorCallback) errorCallback(xmlHTTP.status, xmlHTTP.responseText);
            }
        });
    };

    /**
     * Get the authorization URI for the specified ScriptAuth service.
     * The page at the resulting URI should be shown to the user.
     * A callback URI must be specified with {@link FatFractal#setCallbackUriForScriptAuthService} before calling.
     * When redirected to your callback URI, call {@link FatFractal#retrieveAccessTokenForScriptAuthService} to get the access token.
     *
     * <br><br><strong>Note:</strong> If using an OAuth 1.0a service such as Twitter, this function will generate a request token.
     * In your successCallback, you <strong>must</strong> store the request token, by e.g. setting a cookie.
     * The request token may be accessed using {@link FatFractal#getRequestTokenForScriptAuthService}.
     * When your callback URI is called, you must then set the request token using {@link FatFractal#setRequestTokenForScriptAuthService}.
     *
     * @param {String} scriptAuthService The ScriptAuth service
     * @param {Function} successCallback Function with one argument - the authorization URI
     * @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     * @see #retrieveAccessTokenForScriptAuthService
     * @see #getRequestTokenForScriptAuthService
     */
    this.authUriForScriptAuthService = function(scriptAuthService, successCallback, errorCallback) {
        var callbackUri = this.getCallbackUriForScriptAuthService(scriptAuthService);
        if (!callbackUri) throw new Error("FatFractal.authUriForScriptAuthService: no callback URI found for ScriptAuth service " + scriptAuthService);

        var url = this.getBaseUrl() + "/ff/auth?action=getAuthUri&scriptAuthService=" + scriptAuthService + "&callbackUri=" +  encodeURIComponent(callbackUri);

        var that = this;
        m_ajax({
            type: "GET",
            url: url,
            dataType: "JSON",
            data: null,
                   success: function(response) {
                       if (!response.result) {
                           throw new Error("FatFractal.authUriForScriptAuthService: response does not contain 'result'");
                       }

                       var authUri = response.result.authorizationUri;
                       if (!authUri) {
                           throw new Error("FatFractal.authUriForScriptAuthService: response does not contain 'authorizationUri'");
                       }

                       var token = response.result.token;
                       var secret = response.result.secret;
                       if (token) {
                           that.setRequestTokenForScriptAuthService(scriptAuthService, new Token(token, secret));
                       }

                       successCallback(authUri, response.statusMessage);
                   },
                   error: function(xmlHTTP) {
                       if(console.error) console.error("FatFractal.authUriForScriptAuthService " + xmlHTTP.status + ", " + xmlHTTP.responseText);
                       m_serverStatusMessage = "HTTP request failed - response code was " + xmlHTTP.status + " responseText was " + xmlHTTP.responseText;
                       if (errorCallback) errorCallback(xmlHTTP.status, xmlHTTP.responseText);
                   }
               });
    };

    /**
     * Retrieve an access token for the specified ScriptAuth service.
     * A callback URI must be specified with {@link FatFractal#setCallbackUriForScriptAuthService} before calling.
     * This function should be called after the user has been shown the authorization URI
     * returned by {@link FatFractal#authUriForScriptAuthService}
     * and the service has subsequently redirected to your callback URI.
     * <br><br>The access token is automatically stored via {@link FatFractal#setTokenForScriptAuthService}.
     * The token may be accessed by calling {@link FatFractal#getTokenForScriptAuthService}.
     * Calls to {@link FatFractal#registerWithScriptAuthService} and {@link FatFractal#loginWithScriptAuthService}
     * will automatically use such a stored access token.
     * <br><br><strong>Note:</strong> If using an OAuth 1.0a service such as Twitter,
     * you <strong>must</strong> set the request token before calling this function.
     * The request URI may be set using {@link FatFractal#setRequestTokenForScriptAuthService}.
     * See also discussion for {@link FatFractal#authUriForScriptAuthService}.
     * @param {String} scriptAuthService The ScriptAuth service
     * @param {String} callbackUriWithVerifier Full callback URI, which includes the OAuth verifier as a query parameter
     * @param {Function} successCallback Function with one argument - null
     * @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     * @see #setRequestTokenForScriptAuthService
     * @see #authUriForScriptAuthService
     * @see #registerWithScriptAuthService
     * @see #loginWithScriptAuthService
     */
    this.retrieveAccessTokenForScriptAuthService = function(scriptAuthService, callbackUriWithVerifier, successCallback, errorCallback) {
        var callbackUri = this.getCallbackUriForScriptAuthService(scriptAuthService);
        if (!callbackUri) throw new Error("FatFractal.authUriForScriptAuthService: no callback URI found for ScriptAuth service " + scriptAuthService);

        var query = callbackUriWithVerifier.substr(callbackUriWithVerifier.indexOf("?") + 1);

        var url = this.getBaseUrl() + "/ff/auth?action=getToken&scriptAuthService=" + scriptAuthService + "&callbackUri=" + encodeURIComponent(callbackUri) + "&codeQuery=" + encodeURIComponent(query);

        var token = m_scriptAuthRequestTokens[scriptAuthService];
        if (token) {
            url += "&token=" + token.token + "&secret=" + token.secret;
            delete m_scriptAuthRequestTokens[scriptAuthService];
        } else {
            console.log("Did not find token");
        }

        var that = this;
        m_ajax({
                   type: "GET",
                   url: url,
                   dataType: "JSON",
                   data: null,
                   success: function(response) {
                       if (!response.result) {
                           throw new Error("FatFractal.authUriForScriptAuthService: response does not contain 'result'");
                       }
                       if (!response.result.credential) {
                           throw new Error("FatFractal.authUriForScriptAuthService: response does not contain 'credential'");
                       }

                       var token = response.result.credential.token;
                       if (!token) {
                           throw new Error("FatFractal.authUriForScriptAuthService: response does not contain token");
                       }

                       var secret = response.result.credential.secret;
                       that.setTokenForScriptAuthService(scriptAuthService, new Token(token, secret));

                       successCallback(null, response.statusMessage);
                   },
                   error: function(xmlHTTP) {
                       if(console.error) console.error("FatFractal.retrieveAccessTokenForScriptAuthService " + xmlHTTP.status + ", " + xmlHTTP.responseText);
                       m_serverStatusMessage = "HTTP request failed - response code was " + xmlHTTP.status + " responseText was " + xmlHTTP.responseText;
                       if (errorCallback) errorCallback(xmlHTTP.status, xmlHTTP.responseText);
                   }
               });
    };

    var m_copyDataToObjFromResponse = function (obj, result) {
        for (var key in result) {
            obj[key] = result[key];
        }
    };

    //
    // Create and update in JS SDK now load references based on the response echo from the server when they have succeeded.
    // This is so that in the event that the backend is adding references (which looks like it's going to be a common theme) then the client "gets" them automagically.
    // What we are NOT doing here is binding all of the other data that's coming back from the server.
    // So for example if the client sent 'Male' as the value for a gender and the server converts it to 'MALE', then the client will still have a value of 'Male' ...
    //
    // Therefore ...
    // TODO We need to bind all of the other data also.
    //
    /**
    Create an object on your FatFractal application backend.
    @param {Object} obj The object to be created.
    @param {String} collectionUri The collection name or relative URI in which the object is to be created
    @param {Function} successCallback Function with two arguments - the actual data returned, and the accompanying statusMessage
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     */
    this.createObjAtUri = function(obj, collectionUri, successCallback, errorCallback) {
        if(m_debug) console.log("FatFractal.createObjAtUri was called.");
        if(! successCallback) throw new Error("FatFractal.createObjAtUri: successCallback not supplied");
        if(! errorCallback) errorCallback = m_ff.defaultErrorCallback;
        if(! errorCallback) throw new Error("FatFractal.createObjAtUri: errorCallback not supplied");

        function getObjectClass(obj) {
            if(obj && obj.constructor && obj.constructor.toString) {
                var arr = obj.constructor.toString().match(/function\s*(\w+)/);
                if(arr && arr.length == 2) {
                    return arr[1];
                }
            }
            return undefined;
        }

        var clazz = getObjectClass(obj);
        if(obj.clazz) console.log("object has clazz defined: " +  obj.clazz);
        else if(clazz) obj.clazz = clazz;
        else if(console.error) console.error("cannot resolve the class name for this object");

        if(m_debug) console.log("FatFractal.createObjAtUri thinks this class is: " + clazz + ".");

        var objAsJson = m_transformReferencesForPersistence(obj);
        var tempFfUrl = obj.ffUrl;

        var url = m_validUrl(collectionUri);
        if(m_baseUrl) url = m_baseUrl + url;

        m_ajax({
            type: "POST",
            url: url,
            dataType: 'json',
            contentType:'application/json',
            data: objAsJson,
            success: function(response) {
                if(m_debug) console.log("FatFractal.createObjAtUri: CREATE response is " + JSON.stringify(response));
                m_copyDataToObjFromResponse(obj, response.result);
                if(m_debug) console.log("FatFractal.createObjAtUri: Adding object to local cache");
                m_cache[response.result.ffUrl] = obj;
                var pendingBlobsForObj = m_pendingBlobs[tempFfUrl];
                if (pendingBlobsForObj) for (var key in pendingBlobsForObj) {
                    if(m_debug) console.log("FatFractal.createObjAtUri will save a blob called: " + key);
                    var blob = pendingBlobsForObj[key];
                    if(m_debug) console.log("FatFractal.createObjAtUri is saving a blob with byteLength : " + blob.byteLength + " bytes: ");
                    if(blob.byteLength > 0) {
                        //var data = new Uint8Array(blob);
                        var url = m_validUrl(response.result.ffUrl);
                        if(m_baseUrl) url = m_baseUrl + url;
                        m_ajax({
                            type: "PUT",
                            url: url + "/" + key,
                            dataType: "application/octet-stream",
                            contentType:"application/octet-stream",
                            mimeType: "application/octet-stream",
                            data: blob,
                            success: function(response) {
                                if(console.log) console.log("FatFractal.createObjAtUri " + response.status + ", " + response.responseText);
                            },
                            error: function(xmlHTTP) {
                                if(console.error) console.error("FatFractal.createObjAtUri " + xmlHTTP.status + ", " + xmlHTTP.responseText);
                            }
                        });
                    }
                    if(m_debug) console.log("FatFractal.createObjAtUri is removing blob from queue.");
                    delete pendingBlobsForObj[key];
                }
                m_loadAllReferences(obj);
                successCallback(obj, response.statusMessage);
            },
            error: function(xmlHTTP) {
                if(console.error) console.error("FatFractal.createObjAtUri " + xmlHTTP.status + ", " + xmlHTTP.responseText);
                m_serverStatusMessage = "HTTP request failed - response code was " + xmlHTTP.status + " responseText was " + xmlHTTP.responseText;
                if (errorCallback) errorCallback(xmlHTTP.status, xmlHTTP.responseText);
            }
        });
    };

    /**
    Update an object on your FatFractal application backend.
    @param {Object} obj The object to be created.
    @param {Function} successCallback Function with two arguments - the actual data returned, and the accompanying statusMessage
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     */
    this.updateObj = function(obj, successCallback, errorCallback) {
        if(! successCallback) throw new Error("FatFractal.updateObj: successCallback not supplied");
        if(! errorCallback) errorCallback = m_ff.defaultErrorCallback;
        if(! errorCallback) throw new Error("FatFractal.updateObj: errorCallback not supplied");

        if(! obj.ffUrl) throw new Error("Cannot update this object - doesn't have FatFractal metadata");

        var objAsJson = m_transformReferencesForPersistence(obj);

        var url = m_validUrl(obj.ffUrl);
        if(m_baseUrl) url = m_baseUrl + url;

        m_ajax({
            type: "PUT",
            url: url,
            dataType: 'json',
            contentType:'application/json',
            data: objAsJson,
            success: function(response) {
                if(m_debug) console.log("FatFractal.updateObj: UPDATE response is " + JSON.stringify(response));
                m_copyDataToObjFromResponse(obj, response.result);
                if(m_debug) console.log("FatFractal.updateObj: Updating local cache");
                m_cache[response.result.ffUrl] = obj;
                m_loadAllReferences(obj);
                successCallback(obj, response.statusMessage);
            },
            error: function(xmlHTTP) {
                if(console.error) console.error("FatFractal.updateObj " + xmlHTTP.status + ", " + xmlHTTP.responseText);
                m_serverStatusMessage = "HTTP request failed - response code was " + xmlHTTP.status + " responseText was " + xmlHTTP.responseText;
                if (errorCallback) errorCallback(xmlHTTP.status, xmlHTTP.responseText);
            }
        });
    };

    /**
     * Updates the specified BLOB member in an object
     * @param {Object} obj The object to update
     * @param {ArrayBuffer} blob The new BLOB
     * @param {String} memberName Member name of the BLOB in 'obj'
     * @param {Function} successCallback Function with two arguments - the actual data returned, and the accompanying statusMessage
     * @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     */
    this.updateBlobForObj = function(obj, blob, memberName, successCallback, errorCallback) {
        // TODO: take mime type? iOS SDK does but I don't see the point

        if(m_debug) console.log("FatFractal.updateBlobForObj was called.");
        if(! successCallback) throw new Error("FatFractal.updateBlobForObj: successCallback not supplied");
        if(! errorCallback) errorCallback = m_ff.defaultErrorCallback;
        if(! errorCallback) throw new Error("FatFractal.updateBlobForObj: errorCallback not supplied");

        if(blob.byteLength > 0) {
            var url = m_validUrl(obj.ffUrl);
            if(m_baseUrl) url = m_baseUrl + url;
            m_ajax({
                       type: "PUT",
                       url: url + "/" + memberName,
                       dataType: "application/octet-stream",
                       contentType:"application/octet-stream",
                       mimeType: "application/octet-stream",
                       data: blob,
                       success: function(response) {
                           if (console.log) console.log("FatFractal.updateBlobForObj " + response.status + ", " + response.responseText);
                           successCallback(obj, response.statusMessage);
                       },
                       error: function(xmlHTTP) {
                           if (console.error) console.error("FatFractal.updateBlobForObj " + xmlHTTP.status + ", " + xmlHTTP.responseText);
                           m_serverStatusMessage = "HTTP request failed - response code was " + xmlHTTP.status + " responseText was " + xmlHTTP.responseText;
                           if (errorCallback) errorCallback(xmlHTTP.status, xmlHTTP.responseText);
                       }
                   });
        }
    };

    /**
    Delete an object from your FatFractal application backend.
    @param {Object} obj The object to be deleted.
    @param {Function} successCallback Function with one argument - the statusMessage
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     */
    this.deleteObj = function(obj, successCallback, errorCallback) {
        if(! successCallback) throw new Error("FatFractal.deleteObj: successCallback not supplied");
        if(! errorCallback) errorCallback = m_ff.defaultErrorCallback;
        if(! errorCallback) throw new Error("FatFractal.deleteObj: errorCallback not supplied");

        if(! obj.ffUrl) throw new Error("Cannot delete this object - doesn't have FatFractal metadata");

        var url = m_validUrl(obj.ffUrl);
        if(m_baseUrl) url = m_baseUrl + url;

        m_ajax({
            type: "DELETE",
            url: url,
            dataType: 'json',
            data: null,
            success: function(response) {
                if(m_debug) console.log("FatFractal.deleteObj: Removing deleted object from local cache");
                delete m_cache[obj.ffUrl];
                //if(m_debug) console.log("Cache is now: " + m_ff._toJSON(m_cache));
                successCallback(response.statusMessage);
            },
            error: function(xmlHTTP) {
                if(console.error) console.error("FatFractal.deleteObj " + xmlHTTP.status + ", " + xmlHTTP.responseText);
                m_serverStatusMessage = "HTTP request failed - response code was " + xmlHTTP.status + " responseText was " + xmlHTTP.responseText;
                if (errorCallback) errorCallback(xmlHTTP.status, xmlHTTP.responseText);
            }
        });
    };

    /**
    POST some data to a server extension.
    @param {Object} obj - the object to be serialized and POSTed to your server extension
    @param {String} extensionUri - the name of the server extension
    @param {Function} successCallback Function with two arguments - the result object and the statusMessage
    @param {Function} errorCallback Function with two arguments - Number statusCode and String responseText
     */
    this.postObjToExtension = function (obj, extensionUri, successCallback, errorCallback) {
        var url = m_validUrl(extensionUri, "extension");
        this.createObjAtUri(obj, url,
            function(result, statusMessage) {
//                var retVal = null;
//                if (response.length > 0)
//                    retVal = response[0];
//                successCallback(retVal, response.statusMessage);
                successCallback(result, statusMessage);
            },
            function(statusCode, responseText) {
                if(console.error) console.error("FatFractal.postObjToExtension " + statusCode + ", " + responseText);
                m_serverStatusMessage = "HTTP request failed - response code was " + statusCode + " responseText was " + responseText;
                if (errorCallback) errorCallback(statusCode, responseText);
            }
        );
    };

    /**
    Convenience method: Get a single object from a server extension. If the extension returns more than one object, an exception is thrown.
    @param {String} extensionUri - the name of the server extension
    @param {Function} successCallback Function with one argument - the response object, or null if there was no response object
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
    */
    this.getObjFromExtension = function(extensionUri, successCallback, errorCallback) {
        var url = m_validUrl(extensionUri, "extension");
        this.getArrayFromUri(url,
            function(response) {
                var retVal = null;
                if (response.length > 1)
                    throw "getObjFromExtension received " + response.length + " objects from the server. Suggest use getArrayFromExtension() instead.";
                if (response.length > 0)
                    retVal = response[0];
                successCallback(retVal, response.statusMessage);
            },
            function(statusCode, responseText) {
                if(console.error) console.error("FatFractal.getObjFromExtension " + statusCode + ", " + responseText);
                m_serverStatusMessage = "HTTP request failed - response code was " + statusCode + " responseText was " + responseText;
                if (errorCallback) errorCallback(statusCode, responseText);
            }
        );
    };

    /**
    Get an Array of objects from a server extension. If the extension returns 0 or 1 objects, then you will get an array of length 0 or 1.
    @param {String} extensionUri - the name of the server extension
    @param {Function} successCallback Function with one argument - the response object
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
    */
    this.getArrayFromExtension = function(extensionUri, successCallback, errorCallback) {
        var url = m_validUrl(extensionUri, "extension");
        this.getArrayFromUri(url,
            function(response) {
                successCallback(response, response.statusMessage);
            },
            function(statusCode, responseText) {
                if(console.error) console.error("FatFractal.getArrayFromExtension " + statusCode + ", " + responseText);
                m_serverStatusMessage = "HTTP request failed - response code was " + statusCode + " responseText was " + responseText;
                if (errorCallback) errorCallback(statusCode, responseText);
            }
        );
    };

    /**
    This method returns a single object from a Collection URI,typically when presented with a guid or query that
    should return a single object.
    @param collectionUri
    @param {Function} successCallback Function with one argument - the response object
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     */
    this.getObjFromUri = function(collectionUri, successCallback, errorCallback) {
        if(! successCallback) throw new Error("FatFractal.getObjFromUri: successCallback not supplied");
        if(! errorCallback) errorCallback = m_ff.defaultErrorCallback;
        if(! errorCallback) throw new Error("FatFractal.getObjFromUri: errorCallback not supplied");
        var url = m_validUrl(collectionUri, "resources");
        this.getArrayFromUri(url,
            function(response) {
                var retVal = null;
                if (response.length > 0)
                    retVal = response[0];

                successCallback(retVal, response.statusMessage);
            },
            function(statusCode, responseText) {
                if(console.error) console.error("FatFractal.getObjFromUri.getArrayFromUri " + statusCode + ", " + responseText);
                m_serverStatusMessage = "HTTP request failed - response code was " + statusCode + " responseText was " + responseText;
                if (errorCallback) errorCallback(statusCode, responseText);
            }
        );
    };

    var m_processResultObject = function(retrieved) {
        //      Logic for retrieve
        //      Add the object to the cache (or update it if it's already there)
        //      Check each item in the ffRefs array
        //      If it exists in cache (lookup by ffUrl), then set it as a FIELD on the retrieved object
        //      If not, then load it
        var cached = m_cache[retrieved.ffUrl];
        if (cached == null) {
            if(m_debug) console.log("FatFractal.getArrayFromUri: Adding " + retrieved.ffUrl + " to cache");
            m_cache[retrieved.ffUrl] = retrieved;
        }
        else { // update existing in cache
            if(m_debug) console.log("FatFractal.getArrayFromUri: Updating existing " + retrieved.ffUrl + " in cache");
            for (key in cached) cached[key] = null;
            for (var key in retrieved) cached[key] = retrieved[key];
            }

        retrieved = m_cache[retrieved.ffUrl];

        m_loadAllReferences(retrieved);
    };

    /**
    GET an Array of objects from a URI
    @param {String} collectionUri
    @param {Function} successCallback Function with one argument - the response object
    @param {Function} errorCallback Function with two arguments - Number statusCode and String statusMessage
     */
    this.getArrayFromUri = function(collectionUri, successCallback, errorCallback) {
        if(! successCallback) throw new Error("FatFractal.getArrayFromUri: successCallback not supplied");
        if(! errorCallback) errorCallback = m_ff.defaultErrorCallback;
        if(! errorCallback) throw new Error("FatFractal.getArrayFromUri: errorCallback not supplied");

        var url = m_validUrl(collectionUri);
        if(m_baseUrl) url = m_baseUrl + url;

        m_ajax({
            type: "GET",
            url: url,
            dataType: 'json',
            data: null,
            success: function(response) {
                // If we've got a 'references' field in the response, then process it
                var responseReferences = response.references;
                if (responseReferences) {
                    for (var refIx = 0; refIx < responseReferences.length; refIx++) {
                        m_processResultObject(responseReferences[refIx]);
                    }
                }

                var tmpRetVal;
                // We always want to return an array
                if(! Array.isArray(response.result)) {
                    tmpRetVal = [response.result];
                }
                else {
                    tmpRetVal = response.result;
                }

                var realRetVal = [];
                for (var i = 0; i < tmpRetVal.length; i++) {
                    if (!tmpRetVal[i].ffUrl) { // not an object under management by FatFractal - probably an extension result
                        realRetVal.push(tmpRetVal[i]);
                    }
                    else { // object under management by FatFractal
                        m_processResultObject(tmpRetVal[i]);
                        realRetVal.push(m_cache[tmpRetVal[i].ffUrl]);
                    }
                }

                successCallback(realRetVal, response.statusMessage);
            },
            error: function(xmlHTTP) {
                if(console.error) console.error("FatFractal.getArrayFromUri " + xmlHTTP.status + ", " + xmlHTTP.responseText);
                m_serverStatusMessage = "HTTP request failed - response code was " + xmlHTTP.status + " responseText was " + xmlHTTP.responseText;
                if (errorCallback) errorCallback(xmlHTTP.status, xmlHTTP.responseText);
            }
        });
    };

    /**
    <br>Dynamically execute FFDL on the app's backend, if the user has permission to do so.
    @param {String} ffdl - a string containing one or more FFDL commands. If multiple FFDL commands are supplied, they must be separated by \n
    @param {Function} successCallback Will be invoked with one argument - the response from the backend
    @param {Function} errorCallback Will be invoked with one argument - the xmlHTTP object
     */
    this.executeFFDL = function(ffdl, successCallback, errorCallback) {
        if(! successCallback) throw new Error("FatFractal.executeFFDL: successCallback not supplied");
        if(! errorCallback) errorCallback = m_ff.defaultErrorCallback;
        if(! errorCallback) throw new Error("FatFractal.executeFFDL: errorCallback not supplied");

        var url = this.getBaseUrl() + "ff/metadata";

        m_ajax({
            type: "POST",
            url: url,
            dataType: 'json',
            contentType:'application/json',
            data: m_ff._toJSON({ffdl:ffdl}),
            success: function(response) {
                successCallback(response);
            },
            error: function(xmlHTTP) {
                if (errorCallback) errorCallback(xmlHTTP);
            }
        });
    };

    /**
    <br>Retrieve FFDL on the app's backend, if the permission to do so.
    @param {Function} successCallback Will be invoked with one argument - the response from the backend
    @param {Function} errorCallback Will be invoked with one argument - the xmlHTTP object
     */
    this.getFFDL = function(successCallback, errorCallback) {
        if(! successCallback) throw new Error("FatFractal.getFFDL: successCallback not supplied");
        if(! errorCallback) errorCallback = m_ff.defaultErrorCallback;
        if(! errorCallback) throw new Error("FatFractal.getFFDL: errorCallback not supplied");

        var url = this.getBaseUrl() + "ff/metadata";

        m_ajax({
            type: "GET",
            url: url,
            dataType: 'json',
            data: null,
            success: function(response) {
                successCallback(response.result);
            },
            error: function(xmlHTTP) {
                if (errorCallback) errorCallback(xmlHTTP);
            }
        });
    };

    /**
    Forget about this object - i.e. remove it from the FatFractal local cache.
    @param obj
     */
    this.forgetObj = function (obj) {
        if(obj == null || obj.ffUrl == null)
            throw ("forgetObj: An object with an ffUrl field must be supplied");

        delete m_cache[obj.ffUrl];
    };

    /**
    Get (from the local in-memory cache) the object with this ffUrl
    @param ffUrl
     */
    this.getFromInMemCache = function (ffUrl) {
        return m_cache[ffUrl];
    };

    var m_loadAllReferences = function(referringObject) {
        if (m_autoLoadRefs) {
            if (referringObject.ffRefs != null) {
                if (m_debug) console.log("m_loadAllReferences: Iterating over references: " + JSON.stringify(referringObject.ffRefs));
                for (var refItemIx = 0; refItemIx < referringObject.ffRefs.length; refItemIx++) {
                    var ffRefItem = referringObject.ffRefs[refItemIx];
                    var cachedRefItem = m_cache[ffRefItem.url];
                    if (cachedRefItem) {
                       referringObject[ffRefItem.name] = cachedRefItem;
                       if (m_debug) console.log("m_loadAllReferences: Found cached reference for : " + JSON.stringify(ffRefItem));
                    } else {
                        if (m_debug) console.log("m_loadAllReferences: Loading reference: " + JSON.stringify(ffRefItem));
                        m_loadReference(ffRefItem, referringObject);
                    }
                }
            }
        }
    };

    var m_loadReference = function(refItem, referringObj) {
        var cachedRef = m_cache[refItem.url];
        // If referred object is not already in cache
        if (cachedRef == null) {
            // then put an empty object in the cache
            cachedRef = {ffUrl:refItem.url};
            m_cache[refItem.url] = cachedRef;
            // and set the reference in the referring object
            referringObj[refItem.name] = cachedRef;
            // and load up the object
            if(m_debug) console.log("m_loadReference: refItem.url is - " + refItem.url);
            if(refItem.type == "FFO") {
                m_ff.getObjFromUri(refItem.url, function(response) {
                    if (! response) {
                        if(console.error) console.error("Reference for [" + refItem.url + "] was not returned to m_loadReference's success block");
                        return;
                    }
                    if(m_debug) console.log("Loaded reference: " + JSON.stringify(response));
                    // and copy the response fields into the empty object we just created
                    for (var key in response) cachedRef[key] = response[key];
                },
                function() {
                    if(console.error) console.error("Warning: Failed to load reference: " + JSON.stringify(refItem));
                });
            } else if(refItem.type == "FFB") {
                if(m_http2) {
                    // blob and browser can handle blobs
                    if(m_debug) console.log("Warning: Attempting to load a blob: " + JSON.stringify(refItem));
                    var url = m_validUrl(referringObj.ffUrl);
                    if(m_baseUrl) url = m_baseUrl + url;
                    var xhr = new XMLHttpRequest();
                    xhr.open("GET", url + "/" + refItem.name, true);
                    xhr.responseType = "arraybuffer";
                    xhr.onload = function (oEvent) {
                        var arrayBuffer = xhr.response; // Note: not xhr.responseText
                        if(m_debug) console.log("FatFractal.m_loadReference: arrayBuffer.byteLength " + arrayBuffer.byteLength);
                        if(arrayBuffer) {
                            var byteArray = new Uint8Array(arrayBuffer);
                            referringObj[refItem.name] = byteArray;
                            m_cache[refItem.url] = byteArray;
                        }
                    };
                    xhr.send(null);
                } else if(console.error) console.error("FatFractal.m_loadReference: browser does not support XMLHttpRequest version 2, cannot load blob as data.");
            } else if(console.error) console.error("FatFractal.m_loadReference: for " + refItem.url + " cannot determine the type of reference");
        } else {
            // if referred object is in cache, then we shouldn't have been called - must be a race condition - that's not healthy so we should log it
            if(console.error) console.error("FatFractal.m_loadReference: for " + refItem.url + " called but the object is already in cache");
            // just set the reference in the referring object
            referringObj[refItem.name] = cachedRef;
        }
    };

    var m_tempIdentifier = 1000000;
    var m_transformReferencesForPersistence = function(obj) {
        if (obj['ffRefs'])
            delete obj['ffRefs'];

        // check to see if any of the fields is in the cache
        var referenceKeys = {};
        var foundReferences = false;
        var foundBlobs = false;
        var pendingBlobsForObj = null;
        if (!obj.ffUrl)
            obj.ffUrl = ("" + m_tempIdentifier++);
        if (m_pendingBlobs[obj.ffUrl]) delete m_pendingBlobs[obj.ffUrl];
        for (var key in obj) {
            var theField = obj[key];

            if(m_debug) console.log("m_transformReferencesForPersistence: Checking if " + key + " (" + typeof theField + ") is a reference");
            if (theField != null && (typeof theField == 'object')) {
                if(m_debug) console.log("m_transformReferencesForPersistence: Found non-null : " + JSON.stringify(theField));
                if (theField.ffUrl != null && theField.ffUrl != "") {
                    if(m_debug) console.log("m_transformReferencesForPersistence: Found object field [" + key + "] : " + theField.ffUrl + " - treating as reference");
                    referenceKeys[key] = theField.ffUrl;
                    foundReferences = true;
                } else if(theField.byteLength) {
                    if(m_debug) console.log("m_transformReferencesForPersistence: Found BLOB for field : " + key);
                    // file or blob
                    pendingBlobsForObj = m_pendingBlobs[obj.ffUrl];
                    if (! pendingBlobsForObj) {
                        pendingBlobsForObj = {};
                        m_pendingBlobs[obj.ffUrl] = pendingBlobsForObj;
                    }
                    pendingBlobsForObj[key] = theField;
                    foundBlobs = true;
                }
            }
        }
        // Now clone it; remove the fields which are references or blobs; add in ffRefs
        var objClone = {};
        for (var objKey in obj) objClone[objKey] = obj[objKey];

        if (foundReferences) {
            objClone.ffRefs = [];
            for (var referenceKey in referenceKeys) {
                delete objClone[referenceKey];
                objClone.ffRefs.push({name:referenceKey,type:"FFO",url:referenceKeys[referenceKey]});
            }
        }

        if (foundBlobs && pendingBlobsForObj) {
            for (var blobKey in pendingBlobsForObj) {
                delete objClone[blobKey];
            }
        }
        return m_ff._toJSON(objClone);
    };

    function getCookie(c_name) {
        var i,x,y,ARRcookies=document.cookie.split(";");
        for (i=0;i<ARRcookies.length;i++) {
            x=ARRcookies[i].substr(0,ARRcookies[i].indexOf("="));
            y=ARRcookies[i].substr(ARRcookies[i].indexOf("=")+1);
            x=x.replace(/^\s+|\s+$/g,"");
            if(x==c_name) {
                return unescape(y);
            }
        }
        return null;
    }

    function getSessionInfoKey(thing) {
        return m_baseUrl + "::" + thing;
    }

    function clearSessionInfo() {
        sessionStorage.removeItem(getSessionInfoKey("user"));
        sessionStorage.removeItem(getSessionInfoKey("sessionID"));
    }

    function storeSessionInfo(user, sessionID) {
        if(m_debug) console.log("Storing session info for " + getSessionInfoKey("user"), user);
        sessionStorage.setItem(getSessionInfoKey("user"), JSON.stringify(user));
        if(m_debug) console.log("Storing session info for " + getSessionInfoKey("sessionID"), sessionID);
        sessionStorage.setItem(getSessionInfoKey("sessionID"), sessionID);
        m_cache[user.ffUrl] = user;
    }

    function retrieveSessionID() {
        var sid;
        if (m_simulateCookies === true)
            sid = sessionStorage.getItem(getSessionInfoKey("sessionID"));
        else
            sid = getCookie("sessionId");
        if (! sid)
            return null;
        else
            return sid;
    }

    function retrieveFFUser() {
        if(m_debug) console.log("Retrieving user info for " + getSessionInfoKey("user"));
    	var user = JSON.parse(sessionStorage.getItem(getSessionInfoKey("user")));
        if (user === null || user === undefined) {
            if(console.log) console.log("No existing user info found - creating new FFUser");
            user = new FFUser();
        }
        else {
            if(m_debug) console.log("Found existing FFUser session info: ", user)
        }
        m_cache[user.ffUrl] = user;
        return user;
    }

    return this;
}
