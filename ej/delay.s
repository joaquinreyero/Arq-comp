.global delay

delay:
    CMP R0, #0       
    BEQ end 
for
    SUBS R0, R0, #1  
    BNE for         
end
    MOV PC, LR       
