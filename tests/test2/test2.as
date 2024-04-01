; file ps.as
.entry LIST
.extern W
.define sz=2
.define qw = -1 

MAIN:	mov r3, LIST[sz]  
    	mov r0,r7
	mov #+1,   W[3]   
	mov     #-1111, LIST[sz]  
	mov r3, LIST[78]  
	mov r3, LIST[-100] 
	mov #456 , LIST[0]
LOOP:	jmp W
	mcr  m_mcr
		cmp K,#sz
		bne W
	endmcr 
		prn #-5
		prn r4
		mov STR[5],STR[2]
		sub r1,r4 
	m_mcr  
L1:     inc L3
.entry  LOOP
		bne LOOP
		mov #-1  ,  r4
		cmp  #qw		,   #45
		add #-100,r3
		sub #1,STR[qw]
		not r6
		clr  r0
		inc r3 
		dec r3 
		jmp END
		bne  LOOP
		red  r4
		prn  r0
		jsr r6
		rts 
		hlt
		
		mov  #12 ,W
		add  #qw ,STR[5]
		sub  #-1 ,r3
		mov  LIST,W
		add  LIST,STR[5]
		sub  W,r3
		mov  STR[3],W
		add  STR[5],STR[5]
		sub  STR[sz],r3
		mov   r1,W
		add   r2,STR[5]
		sub   r7,r3

		cmp  #12 ,W
		cmp  #qw ,STR[5]
		cmp  #-1 ,r3
		cmp  #-1 ,#sz
		cmp  LIST,W
		cmp  LIST,STR[5]
		cmp  W,r3
		cmp  W,#-345
		cmp  STR[3],W
		cmp  STR[5],STR[5]
		cmp  STR[sz],r3
		cmp  STR[sz],#1024
		cmp   r1,W
		cmp   r2,STR[5]
		cmp   r7,r3
		cmp   r7,#1111


		not  W
		clr  STR[3]
		inc  r5
		dec  r3

		lea  STR ,W
		lea  STR ,STR[5]
		lea  STR ,r3
		lea  STR[sz] ,W
		lea  STR[5] ,STR[5]
		lea  STR[5],r3

		jmp W
		bne r5

		prn   W
		prn   STR[5]
		prn   r3
		prn   #1111


		lea LOOP , r7
END:    hlt
.define  len = 4
STR:	.string "abcdef"
LIST:	.data 6,-9, len
LIST1:	.data len, len,len, sz
LIST2:	.data 6,-9, len
LIST3:	.data len ,-8, len,5,sz 
K:		.data 22
.extern L3
 
 