#!/bin/bash
# Build script
echo "Compiling RTEMS code..."
arm-rtems6-gcc src/tracker.c -o tracker.exe $(pkg-config --cflags --libs arm-rtems6-realview_pbx_a9_qemu) -lm
echo "Build complete."
