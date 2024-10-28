#!/usr/bin/env python3

from pwn import *

io = process("./exercise", level="debug")

io.recvuntil(b"Hello! What is your name? ")
# TODO: Construct the exploit payload here
io.sendline(payload)

io.recvline_contains(
b"Congratulations! You triggered a buffer overflow. Have a shell."
)
io.interactive()
