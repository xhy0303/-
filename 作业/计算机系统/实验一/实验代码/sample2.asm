.ORIG x3000 
LD R6, ASCII 
LD R5,NEGASCII 
TRAP x23 ;���� 
ADD R1,R0,x0 
ADD R1,R1,R5
TRAP x23 
ADD R0,R0,R5
ADD R2,R0,R1 
ADD R2,R2,R6
LEA R0,MESG
TRAP x22
ADD R0,R2,x0 
TRAP x21
HALT 
ASCII .FILL x30 
NEGASCII .FILL xFFD0 
MESG .STRINGZ "The sum of those two numbers is " 
.END