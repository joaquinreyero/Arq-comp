.text
.extern intToBinario
.global knightRiderA

knightRiderA:
			STMFD	SP!, {r0,r4, r5, r6, r7, LR}
			MOV		r4, #0x80 
			MOV		r5, #8 
			
forRight:
			MOV		r0, r4
			BL		intToBinario
			MOV		r4, r4, LSR #1
			SUBS		r5, r5, #1
			BNE		forRight
			
			MOV		r4, #0x02
			MOV		r5, #7
			
forLeft:
			MOV		r0, r4
			BL		intToBinario
			MOV		r4, r4, LSL #1
			SUBS		r5, r5, #1
			BNE		forLeft
			BEQ		end
			
end:
			LDMFD	SP!, {r0,r4, r5, r6, r7, LR}
			MOV		PC, LR
