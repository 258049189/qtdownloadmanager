#!/bin/sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(dirname $0)/libs
$(dirname $0)/QtDownloadManager $1
