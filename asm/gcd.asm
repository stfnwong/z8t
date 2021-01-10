; Inputs: a, b
; Outputs: a = gcd(a, b)
; Destroys: c
; Assumes: a and b are positive one-byte integers
; 
; Taken from https://rosettacode.org/wiki/Greatest_common_divisor#Z80_Assembly

gcd:
    cp b
    ret z         ; while a != b
    jr c, else    ; if a > b
    sub b         ; a = a - b
    jr gcd
 
else:
    ld c, a       ; Save a
    ld a, b       ; Swap b into a so we can do the subtraction
    sub c         ; b = b - a
    ld b, a       ; Put a and b back where they belong
    ld a, c
 
    jr gcd
    ; some extra instrs for testing one or two parse
    ; TODO : move these to a new file for testing lexing pass only
    ;jr a, b 
    ;jr 45
    ;jr b
    ;jr c, 22

