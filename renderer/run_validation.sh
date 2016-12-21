#!/bin/bash

make distclean; make USE_GCC=1 USE_LIST=1 USE_FILE=1
samples/renderer_sample_bin

make distclean; make USE_GCC=1 USE_LIST=1
samples/renderer_sample_bin

make distclean; make USE_GCC=1 USE_FILE=1
samples/renderer_sample_bin

make distclean; make USE_GCC=1
samples/renderer_sample_bin
