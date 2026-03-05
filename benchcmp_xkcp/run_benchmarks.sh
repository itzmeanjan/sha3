#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

BUILD_DIR="build"
RESULTS_DIR="results"
XKCP_DIR="xkcp"

BENCH_FLAGS=(
  --benchmark_min_warmup_time=.05
  --benchmark_enable_random_interleaving=true
  --benchmark_repetitions=10
  --benchmark_min_time=0.1s
  --benchmark_display_aggregates_only=true
  --benchmark_report_aggregates_only=true
  --benchmark_counters_tabular=true
)

# Clone XKCP if not present
if [ ! -d "$XKCP_DIR" ]; then
  echo "=== Cloning XKCP ==="
  git clone https://github.com/XKCP/XKCP.git "$XKCP_DIR"
  cd "$XKCP_DIR" && git submodule update --init --recursive && cd ..
fi

echo "=== Step 1: Build XKCP (generic64 target with -O3 -march=native -flto) ==="
make -C "$XKCP_DIR" generic64/libXKCP.a \
  EXTRA_CFLAGS="-O3 -march=native -flto" \
  EXTRA_LDFLAGS="-flto"

echo ""
echo "=== Step 2: Build benchmark executable ==="
cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD_DIR" -j

echo ""
echo "=== Step 3: Run benchmarks (XKCP-plain64 + portable-sha3, single binary) ==="
mkdir -p "$RESULTS_DIR"
"$BUILD_DIR/benchmarks" \
  "${BENCH_FLAGS[@]}" \
  --benchmark_out="$RESULTS_DIR/turboshake128.json" \
  --benchmark_out_format=json

echo ""
echo "=== Step 4: Compare (XKCP plain64 as baseline vs. portable-sha3) ==="
COMPARE_PY="$(find "$BUILD_DIR" -path "*tools/compare.py" -print -quit)"
REQUIREMENTS_TXT="$(find "$BUILD_DIR" -path "*tools/requirements.txt" -print -quit)"

if [ -z "$COMPARE_PY" ]; then
  echo "ERROR: compare.py not found in build directory."
  exit 1
fi

VENV_DIR="$BUILD_DIR/.venv"
if [ ! -d "$VENV_DIR" ]; then
  python3 -m venv "$VENV_DIR"
  "$VENV_DIR/bin/pip" install -r "$REQUIREMENTS_TXT"
fi

"$VENV_DIR/bin/python" "$COMPARE_PY" filters \
  "$RESULTS_DIR/turboshake128.json" "xkcp_turboshake128" "portable_turboshake128"
