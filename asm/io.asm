.text
main:
    ; Allocate buffer on the stack
    mov r0, sp       ; Save current stack pointer
    sub sp, 16       ; Allocate 16 bytes

    ; Read input from stdin (up to 16 bytes)
    mov r1, sp       ; Buffer address
    mov r2, 16       ; Buffer size
    in (r1), r2      ; Read from stdin

    ; Echo the input back to stdout
    out (r1), r2     ; Write to stdout

    ; Restore stack
    mov sp, r0
    halt
