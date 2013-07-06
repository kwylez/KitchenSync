var ff = require('ffef/FatFractal');

exports.cleanup = function() {
    var count = 0;
    var foos = ff.getArrayFromUri("/Foo");
    if (foos == null) return;

    for (var i = 0; i < foos.length; i++) {
        ff.deleteObj(foos[i]);
        count++;
    }
    var bars = ff.getArrayFromUri("/Bar");
    if (bars == null) return;

    for (var i = 0; i < bars.length; i++) {
        ff.deleteObj(bars[i]);
        count++;
    }
    var ffdls = "";
    var meta = ff.getAppMetaData();
    var keeps = ["/FFUser","/FFUserGroup","/FFNotificationID","/Foo","/Bar","/Baz"];
    print ("retrieved metadata " + JSON.stringify(meta));
    var colls = meta.collectionResources;
    print("colls is: " + JSON.stringify(colls));
    for(var i = 0; i < colls.length; i++) {
        print ("checking collections " +colls[i].collectionName);
        var a = keeps.indexOf(colls[i].collectionName);
        print("keeps.indexOf(colls[i].collectionName) is: "+a);
        if(keeps.indexOf(colls[i].collectionName) <0) {
            var ffdl = "DROP COLLECTION " + colls[i].collectionName + "\n"
            ffdls += ffdl;
            print ("added new FFDL command " +ffdl);
        }
    }
    var clazzs = meta.objectTypes; 
    var keepClazzs = ["FFUser","FFUserGroup","FFNotificationID", "*"];
    print("clazzs is: " + JSON.stringify(clazzs));
    for(var i = 0; i < clazzs.length; i++) {
        print ("checking collections " +clazzs[i].objectTypeName);
        var a = keepClazzs.indexOf(clazzs[i].objectTypeName);
        print("keepClazzs.indexOf(clazzs[i].objectTypeName) is: "+a);
        if(keepClazzs.indexOf(clazzs[i].objectTypeName) <0) {
            var ffdl = "DROP OBJECTTYPE " + clazzs[i].objectTypeName + "\n"
            ffdls += ffdl;
            print ("added new FFDL command " +ffdl);
        }
    }
    print ("FFDL is: " +ffdls);
    var foobar = "CREATE OBJECTTYPE Foo (fooParameter STRING)\n" +
                 "CREATE OBJECTTYPE Bar (barParameter STRING)\n";
    ffdls += foobar;
    ff.executeFFDL(ffdls);
    var returnMsg = "cleanup deleted " + count + " objects from your backend\n" +
                    "cleanup ran " + ffdls + " ffdl commands.";
    print(returnMsg);
    var r = ff.response();
    r.result = "success";
    r.responseCode="200";
    r.statusMessage = returnMsg;
    r.mimeType = "application/json";
}


