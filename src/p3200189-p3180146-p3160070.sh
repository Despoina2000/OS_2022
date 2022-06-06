#!/bin/bash
gcc -pthread name.c -o name && ./name 100 1000
chmod +x name.sh