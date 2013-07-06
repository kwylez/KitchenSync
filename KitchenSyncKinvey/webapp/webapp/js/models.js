var Foo = Kinvey.Entity.extend({
    // Overwrite the constructor to automatically link all instances to the foo collection.
    constructor: function(attributes) {
        Kinvey.Entity.prototype.constructor.call(this, attributes, 'Foos');
    },
    // Returns the fooParameter of the foo.
    getFooParameter: function() {
        return this.get('fooParameter');
    },
});

var Bar = Kinvey.Entity.extend({
    // Overwrite the constructor to automatically link all instances to the bar collection.
    constructor: function(attributes) {
        Kinvey.Entity.prototype.constructor.call(this, attributes, 'Bars');
    },
    // Returns the barParameter of the bar.
    getBarParameter: function() {
        return this.get('barParameter');
    },
});

