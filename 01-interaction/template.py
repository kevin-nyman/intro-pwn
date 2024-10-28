#!/usr/bin/env python3

from pwn import *

io = process("./exercise", level="debug")

io.recvuntil(b"The starting value is: ")
value_bytes = io.recvline()
# TODO: Process the information here
# TODO: Calculate answer
io.recvuntil(b"Please give me twice the value back: ")
io.sendline(f"{answer}".encode())


io.recvline_contains(b"Correct value! Congratulations")
io.interactive()
