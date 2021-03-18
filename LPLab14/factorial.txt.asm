.586
.model flat,stdcall
includelib libucrt.lib
includelib kernel32.lib

includelib "MAVSTD.lib"
ExitProcess PROTO :DWORD
std_out_str PROTO: DWORD
std_out_int PROTO: SDWORD

.stack 4096

.const
	literal_1 SDWORD 1
	literal_2 SDWORD 0
	literal_3 DB "Введите число для вычисления факториала: " , 0
	literal_4 DB " " , 0
	literal_5 DB "Факториал равен: " , 0

.data
	factorialresult SDWORD 0
	factoriali SDWORD 0
	factorialf SDWORD 0
	mainx SDWORD 0

.code

strlength PROTO : DWORD
strtoint PROTO : DWORD
std_in_int PROTO : SDWORD
factorial PROC factorialx : SDWORD
	push literal_1
	pop factorialresult
	push literal_1
	pop factoriali
	push factorialx
	push literal_2
	pop eax 
	pop ebx 
	.IF eax == ebx
	push factorialresult
	pop eax
	ret
	.ENDIF
	push factorialx
	push literal_1
	pop eax 
	pop ebx 
	.IF eax == ebx
	push factorialresult
	pop eax
	ret
	.ENDIF
	push factorialx
	push factoriali
	pop eax 
	pop ebx 
	.WHILE eax <= ebx
	push factorialresult
	pop factorialf
	push factoriali
	push factorialf
	pop eax
	pop ebx
	mul ebx
	push eax
	pop factorialresult
	push factoriali
	push literal_1
	pop eax
	pop ebx
	add eax, ebx
	push eax
	pop factoriali
	push factoriali
	push factorialx
	pop ebx 
	pop eax 
	.ENDW
	push factorialresult
	pop eax
	ret
	pop eax
factorial ENDP

main PROC
	push offset literal_3
call std_out_str
	push literal_1
	call std_in_int
	push eax
	pop mainx
	push mainx
	call factorial
	push eax
	pop mainx
	push offset literal_4
call std_out_str
	push offset literal_5
call std_out_str
	push mainx
call std_out_int
	push 0
	call ExitProcess
main ENDP
end main