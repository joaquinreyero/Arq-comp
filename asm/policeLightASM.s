.text

.global policeLightASM

policeLightASM:
			STMFD	SP!, {r0,r4, r5, r6, r7, LR}
			MOV		r4, #0x11 
			MOV		r5, #3
			
RighttoLeft:
			MOV		r0, r4
			BL		disp_binary
			MOV		r4, r4, LSL #1
			SUBS		r5, r5, #1
			BNE		RighttoLeft
			
			
			MOV		r5, #3
			
LefttoRight:
			MOV		r0, r4
			BL		disp_binary
			MOV		r4, r4, LSR #1
			SUBS		r5, r5, #1
			BNE		LefttoRight
			BEQ		end
			
end:
			LDMFD	SP!, {r0,r4, r5, r6, r7, LR}
			MOV		PC, LR
