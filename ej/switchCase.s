.global switchcaseASM

switchcase ASM:

    CMP R0, #1       
    BEQ case_1       
    CMP R0, #2       
    BEQ case_2       
    CMP R0, #3       
    BEQ case_3       
    CMP R0, #4       
    BEQ case_4       
    B default

case_1
    B end_switch     
case_2
    B end_switch     

case_3
    B end_switch    

case_4
    B end_switch   
default

end
    
