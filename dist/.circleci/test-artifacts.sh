#!/usr/bin/env bash
##
# Copy artifacts from the app container to the build host for storage.
#
set -e

docker cp $(docker-compose ps -q cli):/app/screenshots /tmp/artifacts/behat
