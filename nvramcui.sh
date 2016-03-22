cd payloads/libpayload
make menuconfig
make
make install

cd -
cd payloads/nvramcui
CC=/home/baozheng/x86/coreboot-gerrit/util/crossgcc/xgcc/bin/i386-elf-gcc PDCDEBUG=1 make clean
CC=/home/baozheng/x86/coreboot-gerrit/util/crossgcc/xgcc/bin/i386-elf-gcc PDCDEBUG=1 make

cd -
make
./build/cbfstool ./build/coreboot.rom add-payload -f payloads/nvramcui/nvramcui.elf -n img/nvramcui.elf -t payload

