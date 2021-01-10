; EXPR
; Test expression parsing

scale:  .equ 256
x: .defw  2 * scale / 4   ; 128
y: .defw -5 * scale / 4   ; -320

equation: .defw (2 * x) + y     ; -64

;yy defw -5 * scale / 4, -4 * scale / 2
