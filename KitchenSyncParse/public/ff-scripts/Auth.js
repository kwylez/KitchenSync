var auth = require('ffef/ScriptAuth');

var TWITTER = "TWITTER";
auth.setScribeApiClassName(TWITTER, "org.scribe.builder.api.TwitterApi");
auth.setScribeApiKey(TWITTER, "twitter_api_key");
auth.setScribeApiSecret(TWITTER, "twitter_api_secret");

var FACEBOOK = "FACEBOOK";
auth.setScribeApiClassName(FACEBOOK, "org.scribe.builder.api.FacebookApi");
auth.setScribeApiKey(FACEBOOK, "facebook_app_id");
auth.setScribeApiSecret(FACEBOOK, "facebook_app_secret");

var AUTH_SERVICES = [TWITTER, FACEBOOK];

function validAuthServices() {
    return AUTH_SERVICES;
}

function getRequestToken() {
    var authService = auth.getAuthRequestData().scriptAuthService;
    var callbackUri = auth.getAuthRequestData().callbackUri;

    var token = null;
    if (authService == TWITTER) {
        var scribeToken = auth.getScribeRequestToken(authService, callbackUri, true);   // this returns a Scribe Token object
        token = new auth.Token(scribeToken.getToken(), scribeToken.getSecret());        // this creates the type of object we should return
    }

    return token;
}

function getAuthorizationUri() {
    var authService = auth.getAuthRequestData().scriptAuthService;
    var requestToken = auth.getAuthRequestData().requestToken;
    var callbackUri = auth.getAuthRequestData().callbackUri;

    var scribeRequestToken = null;
    if (authService == TWITTER) {
        // 'requestToken' is an auth.Token type, must create a Scribe Token from it
        scribeRequestToken = new Packages.org.scribe.model.Token(requestToken.token, requestToken.secret);
    }

    return auth.getScribeAuthorizationUri(authService, scribeRequestToken, callbackUri, true);
}

function getVerifierParameterName() {
    var authService = auth.getAuthRequestData().scriptAuthService;
    switch (authService) {
        case TWITTER:
            return "oauth_verifier";
        case FACEBOOK:
            return "code";
        default:
            return null;
    }
}

function getAccessToken() {
    var authService = auth.getAuthRequestData().scriptAuthService;
    var requestToken = auth.getAuthRequestData().requestToken;
    var verifier = auth.getAuthRequestData().verifier;
    var callbackUri = auth.getAuthRequestData().callbackUri;

    // 'requestToken' is an auth.Token type, must create a Scribe Token from it
    var scribeRequestToken = new Packages.org.scribe.model.Token(requestToken.token, requestToken.secret);

    var scribeAccessToken = auth.getScribeAccessToken(authService, scribeRequestToken, verifier, callbackUri, true);
    var accessToken = new auth.Token(scribeAccessToken.getToken(), scribeAccessToken.getSecret());
    return accessToken;
}

function validateRegisterRequest() {
    var rr = auth.getAuthRequestData().registerRequest;
    var authService = rr.scriptAuthService;

    switch (authService) {
        case TWITTER:
            if (rr.token && rr.secret) {
                return true;
            } else {
                print("ERROR: Script auth service " + authService + " requires credential fields 'token' and 'secret'");
                return false;
            }
        case FACEBOOK:
            if (rr.token) {
                return true;
            } else {
                print("ERROR: Script auth service " + authService + " requires credential field 'token'");
                return false;
            }
        default:
            print("ERROR: Unknown script auth service: " + authService);
            return false;
    }
}

/**
 * Takes a credential map and removes any sensitive information, such as passwords
 * @return {*} Sanitized credential map
 */
function sanitizeCredential() {
    var credential = auth.getAuthRequestData().credential;
    var authService = credential.scriptAuthService;

    var sanitizedCredential = {};
    for (var i in credential) {
        sanitizedCredential[i] = credential[i];
    }

    switch (authService) {
        case TWITTER:
            delete sanitizedCredential.token;
            delete sanitizedCredential.secret;
            break;
        case FACEBOOK:
            delete sanitizedCredential.token;
            break;
    }

    return sanitizedCredential;
}

/**
 * Verify provided credentials.
 * If successful, return an {@link FFUser} object containing AT LEAST a username, more information if applicable.
 * If unsuccessful, return null.
 * Note that any additional information returned in the FFUser object will be used to supplement a registration request.
 * @param credential map of credentials
 * @param createAccount boolean, whether or not to create a new account if the username is not taken
 * @return {*} FFUser object, or null if authentication fails.
 */
function verifyCredential() {
    var credential = auth.getAuthRequestData().credential;
    var createAccount = auth.getAuthRequestData().createAccount;
    var authService = credential.scriptAuthService;

    var scribeAccessToken = new Packages.org.scribe.model.Token(credential.token, credential.secret);
    var url = null;
    switch (authService) {
        case TWITTER:
            url = "https://api.twitter.com/1.1/account/verify_credentials.json";
            break;
        case FACEBOOK:
            url = "https://graph.facebook.com/me";
            break;
    }

    var rawUser = auth.scribeGet(authService, url, scribeAccessToken);

    // check for errors
    if ((authService == TWITTER && rawUser.errors) ||
        (authService == FACEBOOK && rawUser.error)) {
        print("Error retrieving user object from service " + authService);
        return null;
    }

    var userName, firstName, lastName, email;
    switch (authService) {
        case TWITTER:
            userName = rawUser.screen_name;
            email = null;  // not returned by Twitter API calls

            // figure out first and last names
            var fullName = rawUser.name;
            var names = fullName.split(" ");
            if (names.length > 1) {
                var tmp = names[0];
                for (var i = 1; i < names.length-1; i++) {
                    tmp += " " + names[i];
                }
                firstName = tmp.toString(); // necessary to turn ConsString into normal string (Rhino-specific)
                lastName = names[names.length-1];
            } else {
                firstName = fullName;
                lastName = "";
            }

            break;
        case FACEBOOK:
            userName = rawUser.username;
            firstName = rawUser.first_name;
            lastName = rawUser.last_name;
            email = rawUser.email;
            break;
    }

    var user = new auth.FFUser(userName, firstName, lastName, email);
    return user;
}

exports.validAuthServices = validAuthServices;
exports.getRequestToken = getRequestToken;
exports.getAuthorizationUri = getAuthorizationUri;
exports.getVerifierParameterName = getVerifierParameterName;
exports.getAccessToken = getAccessToken;

exports.validateRegisterRequest = validateRegisterRequest;
exports.sanitizeCredential = sanitizeCredential;
exports.verifyCredential = verifyCredential;
