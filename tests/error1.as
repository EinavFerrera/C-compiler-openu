
; Double definition of label
DUPLABEL: clr r1
DUPLABEL: inc r1

; Missing instruction/action after label
EMPTYLABEL: 

; Entry on extern label
.extern EXTERNLABEL
.entry EXTERNLABEL

; Extern on entry label
.entry ENTRYLABEL
.extern ENTRYLABEL

; Missing operand
mov r1, 

; Unintended comma placements
B: .data 5,,6
C: .data ,7

; Spaces between '#' and IMM 
mov  r1, # -5

; Invalid characters in string
STR1: .string "abc$ef"
STR2: .string "ghi^jk"

; Too many operands
D: add r1, r2, r3

; Label starting with a number (invalid)
1INVALIDLABEL: hlt

; Operand without preceding instruction or action
#1234

; Floating label without following colon
FLOATLABEL mov r1, r2

; Nested instructions
E: .data 5, .string "hello", 6

; Two instructions/actions on one line
F: clr r1 , hlt

; Use undefined label as operand
G: mov r2, UNDEFINED

; Exceeding character limit (assuming 81 is the max)
LONG: .string "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopq"

; Label that is an instruction
.string: mov r2, r1

; Label that is an CODE
mov: add r1, r2

; Two colons after a label
H:: clr r3

; Undefined action
J: undefaction r1

; Multiple spaces and tabs between operands
K: mov       r1,        r2

; Using a decimal number
L: .data 5.5

; Invalid characters in labels
M@: clr @r1
N$: inc @r2

; Invalid register names
O: mov rX, r10