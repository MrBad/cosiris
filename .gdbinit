set disassembly-flavor intel
set output-radix 16
set prompt \033[31mgdb$ \033[0m
target remote : 1234
#set debug remote 1
symbol-file kernel.bin
