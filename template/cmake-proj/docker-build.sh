#!/bin/bash

WORKING=$PWD
NAME=main

FLAGS_UBUNTU_14_04=''
FLAGS_UBUNTU_14_04="$FLAGS_UBUNTU_14_04 --tag=$NAME-ubuntu:14.04"
FLAGS_UBUNTU_14_04="$FLAGS_UBUNTU_14_04 --file=docker/dockerfile-ubuntu-14.04"

docker build $FLAGS_UBUNTU_14_04 .

