function Foo(obj) {
   this.fooParameter = null;
   if(obj) {
       this.fooParameter = obj.fooParameter;
   }
   return this;
}
		
function Bar(obj) {
   this.barParameter = null;
   if(obj) {
       this.barParameter = obj.barParameter;
   }
   return this;
}

