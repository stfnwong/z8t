; ADD / SUB
; Testing program for lexer/assembler 
; Taken from http://www.z80.info/lesson1.htm
;

    ld a,8      ;a=8
    add a,10    ;a=a+10  a=18
    ld hl,$FC00 ;hl = $FC00
    ld bc,$00BB ;bc = $00BB
    add hl,FCBB ;hl=hl+bc  hl = $fcbb
 
    
    ld b,8      ;b=8
    ld a,b      ;a=b
    add a,5     ;a='b+5'
    ld b,a      ;b='b+5'

    ;or

    ld bc,46    ;bc=46
    ld h,b      ;you can't do'ld hl,bc'
    ld l,c      ;
    ld bc,52    ;
    add hl,bc   ;hl = bc+52
    ld b,h      ;.
    ld c,l      ;bc =bc+52
