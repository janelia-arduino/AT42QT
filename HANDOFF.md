# AT42QT Handoff

## Summary

This repo was updated to mirror the newer `TCA6408` project structure and to
address several library bugs and coverage gaps.

The largest functional changes are:

- Added Pixi / PlatformIO / formatting / release-check scaffolding.
- Reworked the common `AT42QT` base class around explicit `TwoWire` setup and
  typed I2C error tracking.
- Fixed multiple chip-specific issues, especially for `AT42QT1060`,
  `AT42QT1070`, `AT42QT2120`, and `AT42QT2160`.
- Added native unit tests.
- Rewrote the README and updated examples to the new setup pattern.

## Files Added

- `.clang-format`
- `pixi.toml`
- `platformio.ini`
- `Makefile`
- `tools/clang_format.py`
- `tools/clang_format_all.py`
- `tools/pio_task.py`
- `tools/version_sync.py`
- `tools/release_check.py`
- `test/native/include/Arduino.h`
- `test/native/include/Wire.h`
- `test/native/test_at42qt.cpp`

## Core Library Changes

### Common Base Class

Files:

- `src/AT42QT.h`
- `src/AT42QT/AT42QTDefinitions.h`

Changes:

- Added `at42qt::I2cError` and `at42qt::Result<T>`.
- Added explicit `setup(TwoWire &wire)` API.
- Kept `begin()` as a legacy path.
- Added `initialized()` and `getLastI2cError()`.
- Added typed register read/write helpers under the hood.
- Invalid keys now return/set `InvalidKey` instead of silently mapping to the
  last valid key.
- I2C failures are now tracked instead of being silently ignored.

### Register Definitions

File:

- `src/AT42QT/RegisterAddresses.h`

Changes:

- Fixed typo:
  - `ADJACENT_KEY_SUPRESSION_MASK` -> `ADJACENT_KEY_SUPPRESSION_MASK`

## Chip-Specific Changes

### AT42QT1060

Files:

- `src/AT42QT1060.h`
- `src/AT42QT/AT42QT1060.cpp`

Changes:

- Fixed `getAdjacentKeySuppressionMask()` to read the correct register.
- Added helpers for:
  - `ACTIVE_LEVEL_MASK`
  - `USER_OUTPUT_BUFFER`
  - `DETECTION_INTEGRATOR`
  - `PWM_LEVEL`
  - `KEY_SIGNAL`
  - `KEY_REFERENCE`

### AT42QT1070

Files:

- `src/AT42QT1070.h`
- `src/AT42QT/AT42QT1070.cpp`

Changes:

- Added helpers for:
  - signal/reference reads
  - threshold reads/writes
  - AKS level reads/writes
  - detect integrator reads/writes
  - fast-out/max-cal/guard register
  - low-power interval
  - max-on duration

### AT42QT2120

Files:

- `src/AT42QT2120.h`
- `src/AT42QT/AT42QT2120.cpp`

Changes:

- Replaced compiler-dependent `Status` bitfield unpacking with explicit decode
  from the raw 4 status bytes.
- This specifically targets the bug report where touching key `7` caused keys
  `8-11` to appear in `status.keys`.
- Replaced ambiguous `KeyControl` bitfield usage with explicit helper methods:
  - `touchEnabled()`
  - `setTouchEnabled(bool)`
  - `outputHigh()`
  - `setOutputHigh(bool)`
  - `adjacentKeySuppressionGroup()`
  - `setAdjacentKeySuppressionGroup(uint8_t)`
  - `guardEnabled()`
  - `setGuardEnabled(bool)`
- Example updated to use the new `KeyControl` helpers.

Important note:

- The second issue report about key `0` interfering with other keys was not
  hardware-validated here.
- My current assessment is that it is likely configuration-related:
  - `AKS` grouping
  - `GUARD`
  - `EN` semantics on disabled channels
  - missing recalibration after changing key control
- The library now exposes cleaner helpers for this, but the behavior still
  needs bench validation on real hardware.

### AT42QT2160

Files:

- `src/AT42QT2160.h`
- `src/AT42QT/AT42QT2160.cpp`

Changes:

- Fixed `getStatus()` to read the documented 5-byte status block instead of
  reading 8 bytes into a `uint64_t` blindly.
- Added helpers for:
  - measurement interval
  - burst repetition
  - drift compensation timing
  - detection integrator
  - recalibration delay
  - drift hold duration
  - key threshold
  - key burst length
  - GPIO wake
  - common-change keys
  - signal/reference reads

## Example Changes

Files:

- `examples/PollingAT42QT1060/PollingAT42QT1060.ino`
- `examples/PollingAT42QT1070/PollingAT42QT1070.ino`
- `examples/PollingAT42QT2120/PollingAT42QT2120.ino`
- `examples/PollingAT42QT2160/PollingAT42QT2160.ino`
- `examples/InterruptAT42QT2120/InterruptAT42QT2120.ino`
- `examples/MultipleDevices/MultipleDevices.ino`

Changes:

- Updated examples to prefer:
  - `Wire.begin();`
  - `touch_sensor.setup(Wire);`
- This matches the newer explicit-I2C ownership pattern used in `TCA6408`.

## Metadata / Docs Changes

Files:

- `library.properties`
- `README.org`
- `.gitignore`

Changes:

- Version bumped to `0.7.0`.
- Improved `library.properties` metadata.
- Rewrote README to document:
  - supported chips
  - new setup style
  - typed I2C errors
  - chip coverage
  - known bug fixes
  - Pixi / direct build workflows

## Verification Completed

These passed locally:

- `python3 tools/version_sync.py check`
- `python3 tools/clang_format_all.py --check`
- `make native-test`

## Verification Not Completed

This did **not** fully complete in the current environment:

- `python3 tools/release_check.py`

Reason:

- It gets through version check, format check, and native tests.
- It then stops on the first PlatformIO build because `pio` is not installed in
  the current environment.
- The intended path is likely:
  - `pixi install`
  - then rerun `pixi run release-check`

## Remaining Work

### High Priority

- Run real hardware validation on `AT42QT2120` status decoding, especially keys
  `8-11`.
- Reproduce the reported `key 0` interaction issue on hardware and determine if
  the cause is:
  - bad library decode
  - AKS / guard configuration
  - required recalibration after `KeyControl` writes
  - actual silicon / board behavior
- Run PlatformIO example builds once `pio` is available.

### Recommended Next Changes

- Add a dedicated `AT42QT2120` diagnostic example that prints:
  - raw status bytes
  - decoded `status.keys`
  - each key's `KeyControl`
  - signal/reference values
- Consider replacing more remaining bitfield-based public structs with explicit
  masks/helpers for portability.
- Consider adding native tests for:
  - 1070 key helper register addressing
  - 2160 per-key addressing
  - `KeyControl` helper encoding/decoding
- Review whether `begin()` should remain long-term or whether the library should
  standardize fully on `setup(wire)`.

### Possible Follow-Up on README

- If hardware validation confirms the 2120 fix, document that specifically in
  the README changelog/history.
- If 2120 config guidance becomes clearer, add a short section on:
  - `AKS`
  - `GUARD`
  - disabling unused channels
  - recalibrating after key-control changes

## Notes About the Working Tree

- `datasheets/` is currently untracked.
- I did not remove or modify those PDF files.
- The repo has many modified files and newly added tooling/test files, so check
  `git status` before continuing.

## Suggested Resume Steps

1. Run `git status`.
2. Install repo tooling with `pixi install` if needed.
3. Run:
   - `pixi run format-check`
   - `pixi run test`
4. Build examples with:
   - `pixi run build examples/PollingAT42QT2120 pico`
   - `pixi run build examples/PollingAT42QT2160 pico`
5. Bench-test the `AT42QT2120` reports on real hardware.
6. If needed, add a focused diagnostic sketch for raw status byte dumps.
