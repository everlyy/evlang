#! /usr/bin/env bash

PROJECTS="evlang evlc"

for project in ${PROJECTS[@]};
do
    make -C $project $@
done
