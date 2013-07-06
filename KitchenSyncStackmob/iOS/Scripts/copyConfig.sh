#!/bin/sh

echo "echo $SRCROOT and $PROJECT_DIR/KitchenSyncStackmobTests/"

PATH_TO_TESTS="$PROJECT_DIR/KitchenSyncStackmobTests"
STACKMOB_CONFIGURATION_EXAMPLE="$PATH_TO_TESTS/StackMobConfiguration.plist.sample"
STACKMOB_CONFIGURATION="$PATH_TO_TESTS/StackMobConfiguration.plist"

if [ -f $STACKMOB_CONFIGURATION_EXAMPLE ]; then
  cp $STACKMOB_CONFIGURATION_EXAMPLE $STACKMOB_CONFIGURATION
else
  echo "$STACKMOB_CONFIGURATION_EXAMPLE not found"
  exit 1
fi