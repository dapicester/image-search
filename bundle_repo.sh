#!/bin/sh
file="image-search-`date +%Y%m%d`.git"
git bundle create $file --all --tags --remotes
echo "Created bundle $file"

