#!/bin/sh

echo "echo $SRCROOT and $PROJECT_DIR/KitchenSyncParseTests/"

PATH_TO_TESTS="$PROJECT_DIR/KitchenSyncParseTests"
PARSE_CONFIGURATION_EXAMPLE="$PATH_TO_TESTS/ParseConfiguration.plist.sample"
PARSE_CONFIGURATION="$PATH_TO_TESTS/ParseConfiguration.plist"

if [ -f $PARSE_CONFIGURATION_EXAMPLE ]; then
  cp $PARSE_CONFIGURATION_EXAMPLE $PARSE_CONFIGURATION
else
  echo "$PARSE_CONFIGURATION_EXAMPLE not found"
  exit 1
fi