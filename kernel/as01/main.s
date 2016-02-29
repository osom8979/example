.data

message:
    .string "Hello world\n"
    length  = . - message

.text
    .global _start

_start:
#system call 04: sys_write
    movl    $length, %edx
    movl    $message, %ecx
    movl    $1, %ebx
    movl    $4, %eax
    int     $0x80

#system call 01: sys_exit
    movl    $0, %ebx
    movl    $1, %eax
    int     $0x80

