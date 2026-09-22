#!/bin/bash
# setup.sh - Clone NuttX and integrate Ame NuttX apps
#
# Usage:
#   ./scripts/setup.sh              # default workspace: ~/nuttx-workspace
#   NUTTX_WS=/path ./scripts/setup.sh

set -eu

NUTTX_WS="${NUTTX_WS:-$HOME/nuttx-workspace}"
AME_DIR="$(cd "$(dirname "$0")/.." && pwd)"

echo "=== Ame NuttX setup ==="
echo "Workspace: $NUTTX_WS"
echo "Source:    $AME_DIR"

mkdir -p "$NUTTX_WS"
cd "$NUTTX_WS"

# 1. Clone NuttX + apps
if [ ! -d nuttx ]; then
    echo "[1/5] Cloning apache/nuttx..."
    git clone --depth=1 https://github.com/apache/nuttx.git
fi

if [ ! -d apps ]; then
    echo "[1/5] Cloning apache/nuttx-apps..."
    git clone --depth=1 https://github.com/apache/nuttx-apps.git apps
fi

# 2. Copy ame apps
echo "[2/5] Copying ame apps..."
mkdir -p apps/ame
cp -r "$AME_DIR/apps/ame/"* apps/ame/

# 3. Add to apps/Kconfig
if ! grep -q 'ame/Kconfig' apps/Kconfig; then
    echo "[3/5] Adding ame to apps/Kconfig..."
    echo "source \"$NUTTX_WS/apps/ame/Kconfig\"" >> apps/Kconfig
else
    echo "[3/5] ame/Kconfig already registered"
fi

# 4. Configure board
echo "[4/5] Configuring esp32-wrover-kit:nsh..."
cd nuttx
make distclean 2>/dev/null || true
./tools/configure.sh esp32-wrover-kit:nsh

# 5. Overlay config + rcS
echo "[5/5] Applying ame config + rcS..."
cp "$AME_DIR/configs/esp32-wrover-kit-ame.config" .config
cp "$AME_DIR/boards/esp32-wrover-kit/rcS" \
   boards/xtensa/esp32/common/src/etc/init.d/rcS
chmod +x boards/xtensa/esp32/common/src/etc/init.d/rcS

echo ""
echo "=== Setup complete ==="
echo ""
echo "Next steps:"
echo "  cd $NUTTX_WS/nuttx"
echo "  make -j\$(nproc)"
echo "  esptool --chip esp32 --port /dev/ttyUSB0 erase-flash"
echo "  make flash ESPTOOL_PORT=/dev/ttyUSB0 ESPTOOL_BAUD=921600"
