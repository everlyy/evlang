#! /usr/bin/env bash

PROJECTS="evlang evlc evlvm"

for project in ${PROJECTS[@]};
do
    make -C $project $@
done
