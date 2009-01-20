#include "opcode.h"

void QuitEmulator(void) {
	// 00FD
	exit(-1);
}

void RetSRAtConst(unsigned short int opCode, unsigned short int *stack, unsigned short int *cReg, unsigned char *sCount) {
	// 00EE	
	*sCount -= 1;
	*cReg = stack[*sCount];
	return;
}

void JumpToConstAddr(unsigned short int opCode, unsigned short int *cReg) {
	// 1NNN
	*cReg = opCode & 0x0FFF;
	return;
}

void CallSRAtConst(unsigned short int opCode, unsigned short int *stack, unsigned short int *cReg, unsigned char *sCount) {
	// 2NNN
	stack[*sCount] = *cReg;
	*sCount += 1;
	*cReg = opCode & 0x0FFF;
	return;
}

void SkipIfXEquConst(unsigned short int opCode, unsigned char *gReg, unsigned short int *cReg) {
	// 3XKK
	*cReg += ( gReg[(opCode & 0x0F00) >> 8] == (opCode & 0x00FF) ) ? 2 : 0;
	return;
}

void SkipIfXDEquConst(unsigned short int opCode, unsigned char *gReg, unsigned short int *cReg) {
	// 4XKK
	*cReg += ( gReg[(opCode & 0x0F00) >> 8] != (opCode & 0x00FF) ) ? 2 : 0;
	return;
}

void SkipIfYEqualsX(unsigned short int opCode, unsigned char *gReg, unsigned short int *cReg) {
	// 5XY0
	*cReg += ( gReg[(opCode & 0x0F00) >> 8] == gReg[(opCode & 0x00F0) >> 4] ) ? 2 : 0;
	return;
}

void MovConstToReg(unsigned short int opCode, unsigned char *gReg) {
	// 6XKK
	gReg[(opCode & 0x0F00) >> 8] = opCode & 0x00FF;
	return;
}

void AddConstToReg(unsigned short int opCode, unsigned char *gReg) {
	// 7XKK
	gReg[(opCode & 0x0F00) >> 8] += opCode & 0x00FF;
	return;
}

void MovRegYToRegX(unsigned short int opCode, unsigned char *gReg) {
	// 8XY0
	gReg[(opCode & 0x0F00) >> 8] = gReg[(opCode & 0x00F0) >> 4];
	return;
}

void ORRegXWithRegY(unsigned short int opCode, unsigned char *gReg) {
	// 8XY1
	gReg[(opCode & 0x0F00) >> 8] |= gReg[(opCode & 0x00F0) >> 4];
	return;
}

void ANDRegXWithRegY(unsigned short int opCode, unsigned char *gReg) {
	// 8XY2
	gReg[(opCode & 0x0F00) >> 8] &= gReg[(opCode & 0x00F0) >> 4];
	return;
}

void XORRegXWithRegY(unsigned short int opCode, unsigned char *gReg) {
	// 8XY3
	gReg[(opCode & 0x0F00) >> 8] ^= gReg[(opCode & 0x00F0) >> 4];
	return;
}

void ADDRegYToRegXC(unsigned short int opCode, unsigned char *gReg) {
	// 8XY4
	gReg[(opCode & 0x0F00) >> 8] += gReg[(opCode & 0x00F0) >> 4];
	if( (gReg[(opCode & 0x00F0) >> 4]) > (0xFF - gReg[(opCode & 0x0F00) >> 8]) ) gReg[0xF] = 1;
	return;
}

void SUBRegYFromRegXC(unsigned short int opCode, unsigned char *gReg) {
	// 8XY5
	gReg[0xF] = (gReg[(opCode & 0x0F00) >> 8] >= gReg[(opCode & 0x00F0) >> 4]) ? 0 : 1;
	gReg[(opCode & 0x0F00) >> 8] -= gReg[(opCode & 0x00F0) >> 4];
	return;
}

void ShiftRightRegXC(unsigned short int opCode, unsigned char *gReg) {
	// 8XY6
	gReg[0xF] = (gReg[(opCode & 0x0F00) >> 8] & 0x01);
	gReg[(opCode & 0x0F00) >> 8] >>= 1;
	return;
}

void SUBRegXFromRegYC(unsigned short int opCode, unsigned char *gReg) {
	// 8XY7
	gReg[0xF] = (gReg[(opCode & 0x00F0) >> 4] >= gReg[(opCode & 0x0F00) >> 8]) ? 0 : 1;
	gReg[(opCode & 0x0F00) >> 8] = gReg[(opCode & 0x00F0) >> 4] - gReg[(opCode & 0x0F00) >> 8];
	return;
}

void ShiftLeftRegXC(unsigned short int opCode, unsigned char *gReg) {
	// 8XYE
	gReg[0xF] = (gReg[(opCode & 0x0F00) >> 8] & 0x80) >> 7;
	gReg[(opCode & 0x0F00) >> 8] <<= 1;
	return;
}

void SkipIfYDEqualsX(unsigned short int opCode, unsigned char *gReg, unsigned short int *cReg) {
	// 9XY0
	*cReg += ( gReg[(opCode & 0x0F00) >> 8] =! gReg[(opCode & 0x00F0) >> 4] ) ? 2 : 0;
	return;
}

void SetaRegToConst(unsigned short int opCode, unsigned short int *aReg) {
	// ANNN
	*aReg = opCode & 0x0FFF;
	return;
}

void JumpToConARegAddr(unsigned short int opCode, unsigned char *gReg, unsigned short int *cReg) {
	// BNNN
	*cReg = gReg[0x0] + (opCode & 0xFFF);
	return;
}

void GenRanNumANDConst(unsigned short int opCode, unsigned char *gReg) {
	// CXKK
	srand((int)time(NULL));
	gReg[(opCode & 0x0F00) >> 8] = rand() & (opCode & 0x00FF);
	return;
}

void MovdTimerToRegX(unsigned short int opCode, unsigned char *gReg, unsigned char *dTimer) {
	 // FX07
	gReg[(opCode & 0x0F00) >> 8] = *dTimer;
	return;
}

void SetDelTimToRegX(unsigned short int opCode, unsigned char *gReg, unsigned char *dTimer) {
	// FX15
	*dTimer = (opCode & 0x0F00) >> 8;
	return;
}

void SetSouTimToRegX(unsigned short int opCode, unsigned char *gReg, unsigned char *sTimer) {
	// FX18
	*sTimer = (opCode & 0x0F00) >> 8;
	return;
}

void AddAddRegToRegX(unsigned short int opCode, unsigned char *gReg, unsigned short int *aReg) {
	// FX1E
	*aReg += gReg[(opCode & 0x0F00) >> 8];
	return;
}

void SavgRegToMemAtaReg(unsigned char *gReg, unsigned short int *aReg, unsigned char *memory) {
	// FX55
	int icount;
	
	for(icount = 0; icount < REGISTERSIZE; icount++) {
		memory[icount + *aReg] = gReg[icount];
	}

	return;
}


void LoadgRegToMemAtaReg(unsigned char *gReg, unsigned short int *aReg, unsigned char *memory) {
	// FX65
	int icount;

	for(icount = 0; icount < REGISTERSIZE; icount++) {
		gReg[icount] = memory[icount + *aReg];
	}

	return;
}
