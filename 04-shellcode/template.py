#!/usr/bin/env python3

from pwn import *

stack_address = ??? # TODO: Figure out what this needs to be
stack_offset = ??? # TODO: Figure out what this needs to be

context(os="linux", arch="amd64")
elf = ELF("./exercise")
log.info("Stack address: %#x", stack_address)
log.info("Stack offset: %#x", stack_offset)
io = elf.process(level="debug")

io.recvuntil(b"Hello! What is your name? ")
# TODO: Construct the payload here
io.sendline(payload)

io.interactive()
