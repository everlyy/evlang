#! /usr/bin/env bash

PROJECTS="common evlangc"

for project in ${PROJECTS[@]};
do
    make -C $project $@
done
