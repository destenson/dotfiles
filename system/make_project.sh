#!/bin/env bash

# TODO: Finish this

# leave this blank to only create bare repositories
#WORKING_DIRECTORY_ROOT=
WORKING_DIRECTORY_ROOT=${HOME}/Source/Projects

# we need an argument
[ $# -ne 1 ] &&\
echo "To create a new bare repository in ./PROJECT_NAME.git/:" &&\
echo "" &&\
echo "$(basename $0) PROJECT_NAME" &&\
exit 1 

# secretly, PROJECT_NAME can be a relative path from the current directory and need not already exist

PROJ_NAME=$(basename "$1")
NEW_REPO_ROOT="$(pwd)$1.git"
WD_ROOT="${HOME}/Source/Projects"
NEW_WD_ROOT="${WD_ROOT}/$1"

mkdir -p ${NEW_REPO_ROOT} && \
cd ${NEW_REPO_ROOT} && \
git init --bare && \
mkdir -p ${NEW_WD_ROOT} && \
cd ${NEW_WD_ROOT} #&& \
git clone --local ${NEW_REPO_ROOT}

# TODO:





#EOF
