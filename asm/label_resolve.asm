; Test label resolution 
; This file tests the literal indirection resolution where the literal
; is referenced by a label

x: .defw 30
y: .defw 20
z: .defw -10 * (2 + x) - (3 * y)    ; -380

ld hl (z) 
