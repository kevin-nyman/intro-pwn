#!/usr/bin/env python3

from pwn import *
stack_offset = ??? # TODO: Figure out what this needs to be

context(os="linux", arch="amd64")
elf = ELF("./exercise")
log.info("Stack offset: %#x", stack_offset)
io = elf.process(level="debug")

io.recvuntil(b"Through a bug in the program you happen to know that &name == ")
addr_name = int(io.recvline().decode().strip(), 16)
log.info("Addr name: %#x", addr_name)
addr_ret = addr_name + stack_offset
log.info("Addr retaddr: %#x", addr_ret)

io.recvuntil(b"Through a bug in the program you happen to know that &exercise == ")
addr_exercise = int(io.recvline().decode().strip(), 16)
log.info("Addr exercise(): %#x", addr_exercise)
addr_base = addr_exercise - elf.symbols["exercise"]
log.info("Addr base: %#x", addr_base)
elf.address = addr_base

io.recvuntil(
b"Through a bug in the program you happen to know that the stack cookie is "
)
stack_cookie = int(io.recvline().decode().strip(), 16)
log.info("Stack cookie: %#x", stack_cookie)

io.recvuntil(b"Hello! What is your name? ")
# TODO: Construct the payload here
io.sendline(payload)

io.interactive()
