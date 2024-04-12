#! /usr/bin/env bash

set -e

PROJECTS="evlang evlc evlvm evljit"

for project in ${PROJECTS[@]};
do
    make -C $project $@
done
