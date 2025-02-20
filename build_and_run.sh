#!/bin/bash

make -k && ./build/main black.txt -c && cat compressed.rep 

