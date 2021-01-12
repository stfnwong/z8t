; Test that lexer/assembler can pick up indirect registers, literals

ld (bc), a
ld (de), a
add a, (hl)
ld (hl), e 

ld a, (de)
ld a, (bc)
ld e, (hl)
; literal indirect
ld hl, ($20)
; TODO: test error handling
