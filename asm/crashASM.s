.text
.global crashASM
crashASM:
				STMFD	SP!, {r0,r4, r5, r6, LR} 
				LDR		r5, =table 
				MOV		r6, #0 
				
For:
				LDRB		r4, [r5, r6] 
				MOV		r0, r4
				BL		disp_binary
				ADD		r6, r6, #1  
				CMP		r6, #7     
				BNE		For	
				BL		last
				
				
last:
				LDMFD	SP!, {r0,r4, r5, r6, LR} 
				MOV	PC, LR
table:
	.byte 0x81
	.byte 0x42
	.byte 0x24
	.byte 0x18
	.byte 0x24
	.byte 0x42
	.byte 0x81
