.text

.global wavesASM

wavesASM:
			STMFD	SP!, {r0,r2,r4, r5, r6, LR}
			LDR 	r5, =table
			MOV		r6, #0
	
			
wavesEntry:
			LDRB 	r4,[r5,r6]
			MOV		r0, r4
			BL		disp_binary
			ADD		r6,r6,#1
			CMP		r6,#7
			BNE		wavesEntry
			
			MOV		r4, #0xFF
			MOV		r6, #0
wavesLeft:
			MOV		r0, r4
			BL		disp_binary
			MOV		r4, r4, LSR #1
			CMP		r6,#7
			ADD		r6,r6,#1
			BNE		wavesLeft
			BEQ		end
			
end:
			LDMFD	SP!, {r0,r2,r4, r5, r6, LR}
			MOV		PC, LR
.data 
table:
	.byte 0x01
	.byte 0x03
	.byte 0x07
	.byte 0x0F
	.byte 0x1F
	.byte 0x3F
	.byte 0x7F
	.byte 0xFF
