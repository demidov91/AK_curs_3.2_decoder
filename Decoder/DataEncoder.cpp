#include "DataEncoder.h"
#include <stdio.h>



DataEncoder ::DataEncoder(FILE* file)
{
	__int8 keyPart[16];
	fread(keyPart, 1, 16, file);
	__asm
	{
		movupd xmm0, keyPart
	}
	fread(keyPart, 1, 16, file);
	__asm
	{
		movupd xmm1, keyPart
	}
	fread(keyPart, 1, 16, file);
	__asm
	{
		movupd xmm2, keyPart
	}
	fread(keyPart, 1, 16, file);
	__asm
	{
		movupd xmm3, keyPart
	}
	fread(keyPart, 1, 16, file);
	__asm
	{
		movupd xmm4, keyPart
	}
	fread(keyPart, 1, 16, file);
	__asm
	{
		movupd xmm5, keyPart
	}
	fread(keyPart, 1, 16, file);
	__asm
	{
		movupd xmm6, keyPart
	}
	step = 0;
}


void DataEncoder ::encodeBlock(char* buffer)
{
	__int16 step = this ->step;
	__asm{
			movups xmm7, buffer
			mov CX, step
isIt0:
			cmp CX, 0
			JNZ isIt1
			xorps xmm7, xmm0 
			jmp writeOut
isIt1:
			cmp CX, 1
			JNZ isIt2
			xorps xmm7, xmm1 
			jmp writeOut
isIt2:
			cmp CX, 2
			JNZ isIt3
			xorps xmm7, xmm2
			jmp writeOut
isIt3:
			cmp CX, 3
			JNZ isIt4
			xorps xmm7, xmm3
			jmp writeOut
isIt4:
			cmp CX, 4
			JNZ isIt5
			xorps xmm7, xmm4 
			jmp writeOut
isIt5:
			cmp CX, 5
			JNZ isIt6
			xorps xmm7, xmm5 
			jmp writeOut
isIt6:
			xorps xmm7, xmm6 
			jmp writeOut
writeOut:
			movups buffer, xmm7

			inc CX
			cmp CX, 7
			JNZ saveStep
			mov CX, 0			
saveStep:
			mov step, CX
		}
	this ->step  = step;

}
