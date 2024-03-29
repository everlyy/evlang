#! /usr/bin/env bash

PROJECTS="evlangc"

for project in ${PROJECTS[@]};
do
    make -C $project clean all
done
