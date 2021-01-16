; Inputs: a, b
; Outputs: a = gcd(a, b)
; Destroys: c
; Assumes: a and b are positive one-byte integers
; 
; This version loads values into a and b for testing on emulator

org 100h      ; this is just to make the unit test more convenient

a_val: defb 48
b_val: defb 18

; load a, b registers 
ld a, (b_val)
ld b, a
ld a, (a_val)

gcd:
    cp b
    ret z         ; while a != b
    jr c, gcd_else    ; if a > b
    sub b         ; a = a - b
    jr gcd
 
gcd_else:
    ld c, a       ; Save a
    ld a, b       ; Swap b into a so we can do the subtraction
    sub c         ; b = b - a
    ld b, a       ; Put a and b back where they belong
    ld a, c
 
    jr gcd

    nop     ;; halt here
