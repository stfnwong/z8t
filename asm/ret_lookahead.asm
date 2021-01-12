; Test the lookahead feature, which is currently only used
; with the RET instruction

ret nz
ret nc
ret         ; this has trailing spaces (before this comment)
ret m
; the ret below this comment has no trailing spaces
ret
