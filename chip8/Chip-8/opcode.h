#ifndef _OPCODE_H_
#define _OPCODE_H_

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"

void QuitEmulator(void);							// 00FD
void RetSRAtConst(unsigned short int opCode, unsigned short int *stack,
		   unsigned short int *cReg, unsigned char *sCount);		// 00EE
void JumpToConstAddr(unsigned short int opCode, unsigned short int *cReg);	// 1NNN

void CallSRAtConst(unsigned short int opCode, unsigned short int *stack,
		   unsigned short int *cReg, unsigned char *sCount);		// 2NNN

void SkipIfXEquConst(unsigned short int opCode, unsigned char *gReg,
		    unsigned short int *cReg);					// 3XKK

void SkipIfXDEquConst(unsigned short int opCode, unsigned char *gReg,
		    unsigned short int *cReg);					// 4XKK

void SkipIfYEqualsX(unsigned short int opCode, unsigned char *gReg,
		    unsigned short int *cReg);					// 5XY0

void MovConstToReg(unsigned short int opCode, unsigned char *gReg);		// 6XKK

void AddConstToReg(unsigned short int opCode, unsigned char *gReg);		// 7XKK

void MovRegYToRegX(unsigned short int opCode, unsigned char *gReg);		// 8XY0
void ORRegXWithRegY(unsigned short int opCode, unsigned char *gReg);		// 8XY1
void ANDRegXWithRegY(unsigned short int opCode, unsigned char *gReg);		// 8XY2
void XORRegXWithRegY(unsigned short int opCode, unsigned char *gReg);		// 8XY3
void ADDRegYToRegXC(unsigned short int opCode, unsigned char *gReg);		// 8XY4
void SUBRegYFromRegXC(unsigned short int opCode, unsigned char *gReg);		// 8XY5
void ShiftRightRegXC(unsigned short int opCode, unsigned char *gReg);		// 8XY6
void SUBRegXFromRegYC(unsigned short int opCode, unsigned char *gReg);		// 8XY7
void ShiftLeftRegXC(unsigned short int opCode, unsigned char *gReg);		// 8XYE

void SkipIfYDEqualsX(unsigned short int opCode, unsigned char *gReg,
		    unsigned short int *cReg);					// 9XY0

void SetaRegToConst(unsigned short int opCode, unsigned short int *aReg);	// ANNN

void JumpToConARegAddr(unsigned short int opCode, unsigned char *gReg,
		       unsigned short int *cReg);				// BNNN

void GenRanNumANDConst(unsigned short int opCode, unsigned char *gReg);		// CXKK

void MovdTimerToRegX(unsigned short int opCode, unsigned char *gReg,
		     unsigned char *dTimer);					 // FX07
void SetDelTimToRegX(unsigned short int opCode, unsigned char *gReg,
		     unsigned char *dTimer);					// FX15
void SetSouTimToRegX(unsigned short int opCode, unsigned char *gReg,
		     unsigned char *sTimer);					// FX18
void AddAddRegToRegX(unsigned short int opCode, unsigned char *gReg,
		     unsigned short int *aReg);					// FX1E
void SavgRegToMemAtaReg(unsigned char *gReg, unsigned short int *aReg,
			unsigned char *memory);					// FX55
void LoadgRegToMemAtaReg(unsigned char *gReg, unsigned short int *aReg,
			 unsigned char *memory);				// FX65

#endif
