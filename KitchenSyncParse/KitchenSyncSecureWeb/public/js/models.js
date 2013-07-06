var Foo = Parse.Object.extend("Foos", {
    // Default attributes for Foo.
    defaults: {
      fooParameter: null
    },
    initialize: function() {
      if (!this.get("fooParameter")) {
        this.set({"fooParameter": this.defaults.fooParameter});
      }
    },
});

var Bar = Parse.Object.extend("Bars", {
    // Default attributes for Bar.
    defaults: {
      barParameter: null
    },
    initialize: function() {
      if (!this.get("barParameter")) {
        this.set({"barParameter": this.defaults.barParameter});
      }
    },
});

