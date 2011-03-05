#!/bin/bash

NEW_LD_DIR=$(readlink -f $(dirname $0))

export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$NEW_LD_DIR/:$NEW_LD_DIR/plugins/"

"$NEW_LD_DIR/alter"
