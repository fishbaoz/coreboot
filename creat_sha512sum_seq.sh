#!/bin/sh
for num in `seq 10`; do
    printf "#if (BOARD_NUM==$num)\n"
    echo boardnum$num | sha512sum | grep -o "[0-9a-f]*" | sed -e 's/\([0-9a-f]\{2\}\)/0x\1,/g' | sed -e 's/,$/};/g' | sed -e 's/^0x/u8 sha512_auth[OTP_LEN]={0x/g'
#    echo "\n"
    echo "#endif\n"
done
