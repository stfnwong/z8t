; INC instruction
; For testing lexer and assembler 
; Taken from http://www.z80.info/lesson1.htm

    ld a,5    ;a=5
    inc a     ;a=a+1, a=6
    ld b,a    ;b=a, b=6
    dec b     ;b=b-1, b=5

    ld bc,568 ;bc=568
    inc bc    ;bc=bc+1, bc=569
    inc bc    ;bc=bc+1, bc=570
