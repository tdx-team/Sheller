 .code
    start:
puGetModule PROC
    mov rdi, rdi
	push rbp
	mov rbp, rsp
	sub rsp, 32h    ; 8byte * 4 = 32
	; init stack
	jmp __start

;--------------------------------------------------------
; __HashModulVA  __HashModulVAEND
;--------------------------------------------------------
__HashModulVA:
	; rcx
	; rdx
	push		rbp;
	mov			rbp, rsp;
	sub			rsp, 8h;
	mov			dword ptr[rbp - 4h], 00h
	push		rbx;
	push		rcx;
	push		rdx;
	push		rsi;
	push		rdi;

	push		rdx;			; push hash
	mov			rsi, rcx		; DLLName_String
	test		rsi, rsi		; if(!DLLName_String) 
	jz			__hashfailuers		;	return

	; loop cmp
	xor			rcx, rcx
	xor			rax, rax
__loophash:
	mov			al, [rsi + rcx]	; get 1byte
	test		al, al
	jz			__hashends
	mov			ebx, [rbp - 4h];
	shl			ebx, 19h;
	mov			edx, [rbp - 4h];
	shr         edx, 7h;
	or 			ebx, edx;
	add			ebx, eax;
	mov			dword ptr [rbp - 4h], ebx;
	inc			ecx;
	inc			ecx;
	jmp			__loophash;

__hashends:
	; get hash values
	pop			rbx			; get hash
	mov			edx, [rbp - 4h];
	xor			eax, eax	
	cmp			ebx, edx	; ebx = Findhash edx = string_hash
	jne			__hashfailuers;
	mov			rax, 1h;			; if(OK) return 1
	jmp			__HashModulVAEND
__hashfailuers:
	mov			rax, 00h	; else ret 0
	mov			edi, edi
	mov			rdi, rdi

__HashModulVAEND:
	pop			rdi
	pop			rsi
	pop			rdx
	pop			rcx
	pop			rbx
	mov			rsp, rbp
	pop			rbp
	ret


__GetMoudleAddress1:
    mov			rdi, rdi
	push		rbp
	push		rdi
	mov			rbp, rsp
	sub			rsp, 40h
	push		rdx
	push		rbx
	push		rsi
	mov			rbx, rcx					; save hash
	mov			eax, 0CCCCCCCCh
	mov			ecx, 16
	mov			rdi, rsp
	rep stos	dword ptr [rdi]

	mov			qword ptr [rbp - 38h], rdx	;ptr
	mov			qword ptr [rbp - 40h], rbx	;save hash
	
	; gs:[60h].PEB[18h].PEB_LDR[20h].[DLLName]
	mov			rax, 10h
	mov			rdi, 6
	mul			rdi
	mov			rbx, gs:[rax]
	mov			rax, 21h
	sub			rax, 9h
	mov			rbx, qword ptr [rbx + rax] ;_PEB_LDR_DATA.Ptr64
	mov			rax, 3
	mov			rdi, 5
	mul			rdi
	inc			rax								; 16 = 0x10
	mov			rbx, [rbx + rax]				; _PEB_LDR_DATA.InLoadOrderModuleList

__whiles:
	mov			qword ptr [rbp - 30h], rbx		; save
	mov			rax, 100h
	sub			rax, 0A8h
	add			rbx, rax						; rax = 58h[BaseDllName] _UNICODE_STRING
	mov			rbx, qword ptr [rbx + 8h]		; _UNICODE_STRING.buffer
	mov			rcx, rbx						; rcx = DLLName_String
	mov			rdx, qword ptr [rbp - 40h]		; rdx = Hash
	push		rax								; ret values
	call		__HashModulVA
	; int nStatus = HashModuleVA()
	; if(nStatus)
	;	Success
	;	*DllAddress = Address
	;	break	
	test		rax, rax
	jnz			_ModulSucess
	pop			rax								; get stack_rax
	; Ldr->data = ldr->next
	mov			rbx, qword ptr [rbp - 30h]
	mov			rbx, qword ptr [rbx]
	LOOP		__whiles

_ModulSucess:
	mov			rsi, qword ptr[rbp - 30h]
	mov			rax, 15h
	add			rax, 1Bh
	mov			rsi, qword ptr [rsi + rax]		; DllBase
	mov			rdi, qword ptr [rbp - 38h]		; get &ptr
	mov			qword ptr [rdi], rsi			; save dllbaseAddress
	pop			rax
	pop			rsi
	pop			rdi
	pop			rbx
	mov			rsp, rbp
	pop			rdx
	pop			rbp
	ret
	
__start:
    ; call GetModuleAddress1
	; rdx = ptrAddr
	; rcx = hash
	call		__GetMoudleAddress1
	mov			rsp, rbp
	pop			rbp
	ret

puGetModule ENDP

;
; rcx = DLL_Addr, rdx = Hash, ret FunctionAddr
;
puGetProcAddress PROC
    mov			rdi, rdi
	push		rbp
	push		rdi
	mov			rbp, rsp
	sub			rsp, 20h
	push		rbx
	push		rcx
	push		rdx
	jmp			__start

;--------------------------------------------------------
; __PeFindAddr  __PeFindAddrEnd
;--------------------------------------------------------
__PeFindAddr:
	mov			rdi, rdi
	push		rbp
	push		rdi
	push		rbx
	mov			rbx, rcx			; rbx = temp save DLL_Addr
	mov			rbp, rsp
	sub			rsp, 40h
	mov			ecx, 16
	mov			eax, 0cccccccch
	mov			rdi, rsp
	rep	stos	dword ptr [rdi]

	mov			qword ptr [rbp - 40h], rbx	; save DLL_addr
	mov			qword ptr [rbp - 38h], rdx	; save Hash_values
	mov			rax, [rbp - 40h]			; MZ_DOSHeader
	xor			rbx, rbx
	mov			ebx, dword ptr [rax + 3ch]	; Dos_e_lfanew
	lea			rbx, [rbx + rax]			; Nt_Headr

	xor			rdi, rdi
	mov			edi, dword ptr [rbx + 88h]	; x32-78byte,x64-88bit
	lea			rdi, [rdi + rax]			; DirectoryAddr(ExportTable)

	xor			rbx, rbx
	mov			ebx, dword ptr [rdi + 18h]
	mov			dword ptr[rbp - 30h], ebx	; save NameNumber

	xor			rbx, rbx
	mov			ebx, dword ptr [rdi + 20h]
	lea			rbx, [rbx + rax]
	mov			qword ptr [rbp - 28h], rbx	; save ENT

	mov			ebx, dword ptr [rdi + 24h]
	lea			rbx, [rbx + rax]
	mov			qword ptr [rbp - 20h], rbx	; save EOT

	mov			ebx, dword ptr [rdi + 1ch]	
	lea			rbx, [rbx + rax]
	mov			qword ptr [rbp - 18h], rbx	; save EAT
	;
	; rax = VA_Base
	; find function Name
	;
	xor			rcx, rcx
	mov			ecx, dword ptr[rbp - 30h]	; function number
	mov			rdi, [rbp - 40h]
	mov			r11, rax					; save dll.baseaddr
	xor			rsi, rsi
	mov			rbx, qword ptr [rbp - 28h];
	jmp			__whiles

;--------------------------------------------------------
; __Hashcmp  __HashcmpEnd
;--------------------------------------------------------
__Hashcpm:
	;
	; 1. Get FunctionName-Hash
	; 2. if( r8 == r9.GetHash() ) return 1 else 0
	;
	mov			edi, edi
	push		rbp
	push		rdi
	mov			rbp, rsp
	sub			rsp, 4h;
	push		rbx
	push		rcx
	push		rdi
	push		rsi
	push		rdx
	mov			dword ptr[rbp - 4h], 00h		; save string_hash

	mov			rsi, r9;
	xor			rcx, rcx;
	xor			rax, rax;

__addrloop:
	mov			al, [rsi + rcx]
	test		al, al				
	jz			__tag_end
	mov			ebx, [rbp - 4h];
	shl			ebx, 19h;
	mov			edx, [rbp - 4h];
	shr         edx, 7h;
	or			ebx, edx;
	add			ebx, eax;
	mov			dword ptr [ebp - 4h], ebx		; count of data 
	inc			ecx;
	jmp			__addrloop

__tag_end:
	mov			rbx, r8		; get hash
	mov			edx, [rbp - 4h]
	xor			rax, rax
	cmp			ebx, edx
	jne			__tagfailuer
	mov			rax, 1
	jmp			__HashcmpEnd
__tagfailuer:
	mov			rax, 0

__HashcmpEnd:
	pop			rdx
	pop			rsi
	pop			rdi
	pop			rcx
	pop			rbx
	mov			rsp, rbp
	pop			rdi
	pop			rbp
	ret	
;--------------------------------------------------------
; __Hashcmp  __HashcmpEnd
;--------------------------------------------------------

__whiles:
	;
	; call hash_cmp
	; r8 = hashvalues, r9 = functionNameVA
	;
	push		r8
	push		r9
	mov			r8, qword ptr [rbp - 38h] 
	xor			rdx, rdx
	mov			edx, dword ptr [rbx + rsi * 4]
	;;
	lea			rdx, [rdx + r11]
	mov			r9, rdx
	call		__Hashcpm
	pop			r9
	pop			r8
	; if (rax == 1)
	test		rax, rax
	jnz			__findaddrsuccess
	inc			rsi
	LOOP		__whiles
	jmp			__PeFindAddrEnd

__findaddrsuccess:
	; EOT --> EAT ret EAT.addr
	xor			rcx, rcx
	mov			rcx, rsi					; index
	mov			rbx, [rbp - 20h]			; EOT
	xor			rdx, rdx
	mov			dx, [rbx + rcx * 2]			; get word.indexnumber

	mov			rsi, [rbp - 18h]
	xor			rbx, rbx
	mov			ebx, dword ptr [rsi + rdx * 4]
	lea			rax, [rbx + r11]

__PeFindAddrEnd:
	mov			rsp, rbp
	pop			rbx
	pop			rdi
	pop			rbp
	ret
;--------------------------------------------------------
; __PeFindAddr  __PeFindAddrEnd
;--------------------------------------------------------

__start:
	; entry PE_Find_address
	call		__PeFindAddr
	test		rax, rax	; findaddr success ? 1 : 0
	jnz			__end
	xor			rax, rax	; find_addr_failuer
	; ret rax
__end:
	pop			rdx
	pop			rcx
	pop			rbx
	mov			rsp, rbp
	pop			rdi
	pop			rbp
	ret
puGetProcAddress ENDP

;
; rcx = g_stud.s_dwOepBase  
;
CodeExecEntry	PROC
	mov		rdi, rdi
	push	rbp
	push	rdi
	push	rsi
	push	rax
	mov		rsi, rcx			; get OepBase
	mov		rbp, rsp
	sub		rsp, 38h
	mov		eax, 00h
	mov		rcx, 7
	mov		rdi, rsp
	rep stos	qword ptr [rdi]



	xor		rax, rax			; OEP + ImageBase = code.exec.entry
	add		rax, 20000000h		; 0x140000000 
	add		rax, 20000000h		
	add		rax, 20000000h
	add		rax, 20000000h
	add		rax, 20000000h
	add		rax, 20000000h
	add		rax, 20000000h
	add		rax, 20000000h
	add		rax, 20000000h
	add		rax, 20000000h
	add		rsi, rax
	call	rsi


	mov		rsp, rbp
	pop		rax
	pop		rsi
	pop		rdi
	pop		rbp
	ret
CodeExecEntry	ENDP

end