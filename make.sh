#!/bin/bash
rm -rf *.o *.symvers *.order ./*.cmd *.json *.ko *.mod.c *.mod.o *.o.d .tmp_versions
bear make
rm -rf *.o *.symvers *.order ./*.cmd *.mod.c
sed -i '/-mapcs/d' compile_commands.json
sed -i '/-mno-sched-prolog/d' compile_commands.json
sed -i '/-fno-ipa-sra/d' compile_commands.json
sed -i '/-mno-thumb-interwork/d' compile_commands.json
sed -i '/-fno-var-tracking-assignments/d' compile_commands.json
sed -i '/-fconserve-stack/d' compile_commands.json
sed -i 's/],/\t,"-I\/home\/book\/100ask_imx6ull-sdk\/ToolChain\/arm-buildroot-linux-gnueabihf_sdk-buildroot\/arm-buildroot-linux-gnueabihf\/sysroot\/usr\/include",\n\t\t\t"-I\/home\/book\/100ask_imx6ull-sdk\/ToolChain\/arm-buildroot-linux-gnueabihf_sdk-buildroot\/bin\/..\/lib\/gcc\/arm-buildroot-linux-gnueabihf\/7.5.0\/..\/..\/..\/..\/arm-buildroot-linux-gnueabihf\/include",\n\t\t\t"-I\/home\/book\/100ask_imx6ull-sdk\/ToolChain\/arm-buildroot-linux-gnueabihf_sdk-buildroot\/bin\/..\/lib\/gcc\/arm-buildroot-linux-gnueabihf\/7.5.0\/include-fixed",\n\t\t\t"-I\/home\/book\/100ask_imx6ull-sdk\/ToolChain\/arm-buildroot-linux-gnueabihf_sdk-buildroot\/bin\/..\/lib\/gcc\/arm-buildroot-linux-gnueabihf\/7.5.0\/include"\n\t\t&/' compile_commands.json