#!/bin/bash -x

export VERS=V1.4

docker build --tag evansjr/dirsort:${VERS} -f Dockerfile2 .

docker push  evansjr/dirsort:${VERS}


