#!/bin/bash
# build.sh - Build + flash Ame NuttX to ESP32
#
# Usage:
#   ./scripts/build.sh [/dev/ttyUSB0]

set -eu

PORT="${1:-/dev/ttyUSB0}"
NUTTX_WS="${NUTTX_WS:-$HOME/nuttx-workspace}"

cd "$NUTTX_WS/nuttx"

echo "=== Building Ame NuttX ==="
make -j"$(nproc)"

echo ""
echo "=== Erasing flash ==="
esptool --chip esp32 --port "$PORT" erase-flash

echo ""
echo "=== Flashing ==="
make flash ESPTOOL_PORT="$PORT" ESPTOOL_BAUD=921600

echo ""
echo "=== Done ==="
echo "Connect with: picocom -b 115200 $PORT"
