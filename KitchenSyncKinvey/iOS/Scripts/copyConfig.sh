#!/bin/sh

echo "echo $SRCROOT and $PROJECT_DIR/KitchenSyncKinveyTests/"

PATH_TO_TESTS="$PROJECT_DIR/KitchenSyncKinveyTests"
KINVEY_CONFIGURATION_EXAMPLE="$PATH_TO_TESTS/KinveyConfiguration.plist.sample"
KINVEY_CONFIGURATION="$PATH_TO_TESTS/KinveyConfiguration.plist"

if [ -f $KINVEY_CONFIGURATION_EXAMPLE ]; then
  cp $KINVEY_CONFIGURATION_EXAMPLE $KINVEY_CONFIGURATION
else
  echo "$KINVEY_CONFIGURATION_EXAMPLE not found"
  exit 1
fi