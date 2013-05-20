IFDEF _WIN64 ; --------------------------- x86-64

EXTRN	_fltused:DWORD

PUBLIC	cvt80to64
PUBLIC	cvt64to80

_TEXT	SEGMENT

var_8           = qword ptr -8

cvt80to64 PROC

	fld     tbyte ptr [rcx]
	fstp    [rsp+var_8]
	movsd   xmm0, [rsp+var_8]
	ret	0

cvt80to64 ENDP

var_8           = qword ptr -8

cvt64to80 PROC

	movsd   [rsp+var_8], xmm0
	fld     [rsp+var_8]
	fstp    tbyte ptr [rdx]
	ret	0

cvt64to80 ENDP

ELSE ; --------------------------- x86

	.686P
	.XMM

_TEXT	SEGMENT

EXTRN	__fltused:DWORD

PUBLIC	_cvt80to64
PUBLIC	_cvt64to80

_cvt80to64 PROC

	mov	eax, dword ptr [esp+4]
	fld	TBYTE PTR [eax]

	ret	0
_cvt80to64 ENDP


_cvt64to80 PROC
	mov	eax, DWORD PTR [esp+12]
	fld	QWORD PTR [esp+4]
	fstp	TBYTE PTR [eax]
	ret	0
_cvt64to80 ENDP

ENDIF

_TEXT	ENDS
	END
