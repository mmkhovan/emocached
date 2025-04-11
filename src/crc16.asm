// x86_64 SysV ABI
// uint16_t crc16(const uint8_t* data, size_t len)
global crc16
section .text
crc16:
    xor     rax, rax
    mov     r8, 0x1021
.loop:
    cmp     rsi, 0
    je      .done
    movzx   rcx, byte [rdi]
    shl     rcx, 8
    xor     rax, rcx
    mov     rcx, 8
.bitloop:
    shl     rax, 1
    test    rax, 0x10000
    jz      .skip_xor
    xor     rax, r8
.skip_xor:
    dec     rcx
    jnz     .bitloop
    inc     rdi
    dec     rsi
    jmp     .loop
.done:
    and     rax, 0xFFFF
    ret