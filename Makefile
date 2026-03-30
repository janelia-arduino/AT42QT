.PHONY: clean
clean:
	rm -rf .pio

.PHONY: native-test
native-test:
	g++ -std=c++17 -Wall -Wextra -Itest/native/include -Isrc test/native/test_at42qt.cpp src/AT42QT/AT42QT1060.cpp src/AT42QT/AT42QT1070.cpp src/AT42QT/AT42QT2120.cpp src/AT42QT/AT42QT2160.cpp -o /tmp/at42qt_native_test
	/tmp/at42qt_native_test

.PHONY: teensy-firmware
teensy-firmware: clean
	pio run -e teensy40

.PHONY: pico-firmware
pico-firmware: clean
	pio run -e pico

.PHONY: monitor
monitor:
	pio device monitor --echo --eol=LF
