#!/bin/bash
# Run script
echo "Starting RTEMS Simulation..."
qemu-system-arm -no-reboot -nographic -M realview-pbx-a9 -m 256M -kernel tracker.exe | tee data/tracking_data.csv
