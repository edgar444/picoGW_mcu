
# Personal

## tmux templates

SEGGER Jlink setup for SWD Debugging:

```
tmux set-option -g history-limit 1000000 \; \
	new-session -s jlink \; \
	send-keys -l $'sleep 2s;\ntelnet 127.0.0.1 4444\n' \; \
	split-window -h -p 60 \; \
	send-keys -l "dmesg -kwTLalways"$'\n' \; \
	split-window -v -l 12 \; \
	send-keys -l "openocd -c 'interface jlink' -c 'transport select swd' -f target/stm32f4x.cfg"$'\n' \; \
	split-window -v -l 2 \; \
	send-keys -l "watch -tn.7 'lsusb -d 0483:'"$'\n'
```

Debugging via cgdb:

```
# Opinionated Share functions
function oswget { (
	md5sum=$1
	url=$2
	file=${url##*/}
	cd "$OCACHE_DL"
	<<<"$md5sum  $file" md5sum -c >/dev/null || { rm -rf "$file"; wget "$url";}
	test -d "$OCACHE_DL/gcc-arm-none-eabi-6-2017-q2-update-linux" || { mkdir "$OCACHE_DL/gcc-arm-none-eabi-6-2017-q2-update-linux"
	cd "$OCACHE_DL/gcc-arm-none-eabi-6-2017-q2-update-linux"
	tar xf "$OCACHE_DL/gcc-arm-none-eabi-6-2017-q2-update-linux.tar.bz2";}
) }

cd $OCACHE_GIT/github.com/edgar444/picoGW_mcu/bin/
oswget 13747255194398ee08b3ba42e40e9465 https://developer.arm.com/-/media/Files/downloads/gnu-rm/6-2017q2/gcc-arm-none-eabi-6-2017-q2-update-linux.tar.bz2
export PATH="$OCACHE_GIT/github.com/plietar/dfuse-tool:$OCACHE_DL/Atollic_TrueSTUDIO_for_STM32_linux_x86_64_v9.3.0_20190212-0734/Atollic_TrueSTUDIO_for_STM32_9.3.0_installer/ide:$OCACHE_DL/gcc-arm-none-eabi-6-2017-q2-update-linux/gcc-arm-none-eabi-6-2017-q2-update/bin:$PATH"

# Use GDB (with CGDB frontend)
cgdb -d arm-none-eabi-gdb -- -ex 'target remote :3333' -ex 'symbol-file lgw_fw_usb.elf' --ex 'set print pretty on'
```

Building and Flashing Setup (Won't flash):

```
IFS= read -rd '' toolchain <<'EOF' || true
# Opinionated Share functions
function oswget { (
	md5sum=$1
	url=$2
	file=${url##*/}
	cd "$OCACHE_DL"
	<<<"$md5sum  $file" md5sum -c >/dev/null || { rm -rf "$file"; wget "$url";}
	test -d "$OCACHE_DL/gcc-arm-none-eabi-6-2017-q2-update-linux" || { mkdir "$OCACHE_DL/gcc-arm-none-eabi-6-2017-q2-update-linux"
	cd "$OCACHE_DL/gcc-arm-none-eabi-6-2017-q2-update-linux"
	tar xf "$OCACHE_DL/gcc-arm-none-eabi-6-2017-q2-update-linux.tar.bz2";}
) }

oswget 13747255194398ee08b3ba42e40e9465 https://developer.arm.com/-/media/Files/downloads/gnu-rm/6-2017q2/gcc-arm-none-eabi-6-2017-q2-update-linux.tar.bz2
export PATH="$OCACHE_GIT/github.com/plietar/dfuse-tool:$OCACHE_DL/Atollic_TrueSTUDIO_for_STM32_linux_x86_64_v9.3.0_20190212-0734/Atollic_TrueSTUDIO_for_STM32_9.3.0_installer/ide:$OCACHE_DL/gcc-arm-none-eabi-6-2017-q2-update-linux/gcc-arm-none-eabi-6-2017-q2-update/bin:$PATH"

cd $OCACHE_GIT/github.com/edgar444/picoGW_mcu/
ninja
# dfu-util -a 0 -D bin/lgw_fw_usb.dfu
EOF

tmux set-option -g history-limit 1000000 \; \
	new-session -s flash \; \
	send-keys -l $'cd $OCACHE_GIT/github.com/Lora-net/picoGW_hal/;\n sudo timeout -s9 1 ./util_chip_id/util_chip_id; sudo timeout -s9 3 ./util_boot/util_boot' \; \
	split-window -v \; \
	send-keys -l "$toolchain"'' \; \
	split-window -v -l 2 \; \
	send-keys -l "watch -tn.7 'lsusb -d 0483:'"$'\n'
```

Interceptty:

```
tmux set-option -g history-limit 1000000 \; \
	new-session -s itty \; \
	set-option mouse on \; \
	send-keys -l "clear; interceptty -s 'ispeed 115200 ospeed 115200' /dev/ttyACM0 /dev/ttyV0 | tee /tmp/itty | grep '^>' --color=always --line-buffered | ts '%H:%M:%.S'"$'\n' \; \
	split-window -h \; \
	send-keys -l $'clear; tail -f /tmp/itty | ts "%H:%M:%.S"\n' \; \
	split-window -h \; \
	send-keys -l "clear; unbuffer bash -c \"cat /dev/ttyV0 | hexdump -v -e '/1 \\\"0x%02x\n\\\"'\" | ts \"%H:%M:%.S\""$'\n' \; \
	split-window -h \; \
	resize-pane -t 0 -x 32 \; \
	resize-pane -t 1 -x 32 \; \
	resize-pane -t 2 -x 20 \; \
	send-keys -l "clear"$'\n'"printf 'l\0\x4\0\x1\xa\0\x6' > /dev/ttyV0"



```
