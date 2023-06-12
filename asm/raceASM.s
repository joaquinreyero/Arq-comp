.text
.global raceASM
raceASM:
				STMFD	SP!, {r0,r4, r5, r6, LR} 
				LDR		r5, = table 
				MOV		r6, #0 
				
For:
				LDRB		r4, [r5, r6] 
				MOV		r0, r4
				BL		disp_binary
				ADD		r6, r6, #1  
				CMP		r6, #6     
				BNE		For	
				BL		last
				
				
last:
				LDMFD	SP!, {r0,r4, r5, r6, LR} 
				MOV	PC, LR
table:
	.byte 0xA0
	.byte 0x30
	.byte 0x08
	.byte 0x06
	.byte 0x03
	.byte 0x01
