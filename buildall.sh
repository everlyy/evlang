#! /usr/bin/env bash

PROJECTS="evlang evlangc"

for project in ${PROJECTS[@]};
do
    make -C $project $@
done
