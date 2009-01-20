#include "chip8.h"

unsigned short int FetchOpCode(unsigned char *memory, unsigned short int cReg);

void ExecuteOpCode( unsigned short int opCode,
		    unsigned char      *gReg,
		    unsigned short int *aReg,
		    unsigned short int *cReg,
		    unsigned short int *stack,
		    unsigned char      *memory,
		    unsigned char      *dTimer,
		    unsigned char      *sTimer,
		    unsigned char      *sCount
		   );

bool OpenRom(char *fileName, FILE **rom);
bool RomToMemory(FILE *rom, unsigned char *memory);

bool InitHarware( unsigned char	     **gReg,
		  unsigned short int *aReg,
		  unsigned short int *cReg,
		  unsigned short int **stack,
		  unsigned char      **memory,
		  unsigned char      *dTimer,
		  unsigned char	     *sTimer,
		  unsigned char	     *sCount
		 );

int main(void) {
	// Chip-8 Harware
	unsigned char	   *gReg;	// Data Registers
	unsigned short int aReg;	// Adress Register
	unsigned short int cReg;	// Code Register
	unsigned short int *stack;	// Chip-8 Stack
	unsigned char	   *memory;	// Chip-8 Memory
	unsigned char	   dTimer;	// Delay Timer
	unsigned char	   sTimer;	// Sound Timer
	unsigned char	   sCount;	// Level in stack

	// Misc.
	unsigned short int opCode;

	// Program Tools
	FILE *rom;
	char fileName[MAXNAME];
	bool errcheck;

	// Timer Vaiables
	time_t bench, counter, wait;
	int loopCounter;

	// Find Rom and Open Rom
	do {
		do {
			printf("Open File: ");
			errcheck = scanf("%s", fileName);
			if(errcheck == false) printf("Invalid Input!\n");
		} while(errcheck != true);
		errcheck = OpenRom(fileName, &rom);
		if(errcheck == false) printf("%s does not exist or is not accessible!\n", fileName);
	} while(errcheck != true);

	// Initialise System Hardware
	errcheck = InitHarware(&gReg, &aReg, &cReg, &stack, &memory, &dTimer, &sTimer, &sCount);
	if(errcheck == false) {
		printf("ERROR! - FAILED TO INITIALISE HARDWARE - ERROR!\n");
		return -1;
	}

	// Load rom into memory
	errcheck = RomToMemory(rom, memory);
	if(errcheck == false) {
		printf("ERROR! - FAILED TO LOAD ROM INTO MEMORY - ERROR!\n");
		return -2;
	}

	// Close rom as it is no longer required
	fclose(rom);

	// Beging Emulation
//	BEGIN - LOOP COUNT MECHANISM
	while( 1 ) {
		loopCounter = 0;
		bench = time(NULL);
		do {
			counter = time(NULL);
			if(loopCounter < FRAMERATE) {
			loopCounter++;
//	BEGIN - LOOP CONTENTS


				if(cReg == MEMORYSIZE - 1) return 0;
				// Fetch
				opCode = FetchOpCode(memory, cReg);
				cReg += 2;
		
				// Execute
				ExecuteOpCode(opCode, gReg, &aReg, &cReg, stack, memory, &dTimer, &sTimer, &sCount);
				if(sTimer > 0) sTimer--;
				if(dTimer > 0) dTimer--;



//	BEGIN - LOOP COUNT MECHANISM
			}
		wait = (time_t)difftime(counter, bench);
		} while( !((loopCounter == FRAMERATE) && (wait >= TIMER)) );

		// Display Timer Resuls
		 printf("%d opcodes per second\n", loopCounter/((int)(wait)));
	}
//	END - LOOP COUNT MECHANISM

	return 0;
}

void ExecuteOpCode(unsigned short int opCode, unsigned char *gReg, unsigned short int *aReg, unsigned short int *cReg, unsigned short int *stack, unsigned char *memory, unsigned char *dTimer, unsigned char *sTimer, unsigned char *sCount) {
	switch(opCode & 0xF000) {
		case 0x0000:
			switch(opCode & 0x00F0) {
				case 0x00C0:
					break;
				case 0x00F0:
					switch(opCode & 0x000F) {
						case 0x000B:
							break;
						case 0x000C:
							break;
						case 0x000D: // 00FD - SUPER
							QuitEmulator();
							break;
						case 0x000E:
							break;
						case 0x000F:
							break;
					}
					break;
				case 0x00E0:
					switch(opCode & 0x000F) {
						case 0x0000:
							break;
						case 0x000E: // 00EE
							RetSRAtConst(opCode, stack, cReg, sCount);
							break;
					}
					break;
			}
			break;
		case 0x1000: // 1NNN
			JumpToConstAddr(opCode, cReg);
			break;
		case 0x2000: // 2NNN
			CallSRAtConst(opCode, stack, cReg, sCount);
			break;
		case 0x3000: // 3XKK
			SkipIfXEquConst(opCode, gReg, cReg);
			break;
		case 0x4000: // 4XKK
			SkipIfXDEquConst(opCode, gReg, cReg);
			break;
		case 0x5000: // 5XY0
			SkipIfYEqualsX(opCode, gReg, cReg);
			break;
		case 0x6000: // 6XKK
			MovConstToReg(opCode, gReg);
			break;
		case 0x7000: // 7XKK
			AddConstToReg(opCode, gReg);
			break;
		case 0x8000:
			switch(opCode & 0x000F) {
				case 0x0000: // 8XY0
					MovRegYToRegX(opCode, gReg);
					break;
				case 0x0001: // 8XY1
					ORRegXWithRegY(opCode, gReg);
					break;
				case 0x0002: // 8XY2
					ANDRegXWithRegY(opCode, gReg);
					break;
				case 0x0003: // 8XY3
					XORRegXWithRegY(opCode, gReg);
					break;
				case 0x0004: // 8XY4
					ADDRegYToRegXC(opCode, gReg);
					break;
				case 0x0005: // 8XY5
					SUBRegYFromRegXC(opCode, gReg);
					break;
				case 0x0006: // 8XY6
					ShiftRightRegXC(opCode, gReg);
					break;
				case 0x0007: // 8XY7
					SUBRegXFromRegYC(opCode, gReg);
					break;
				case 0x000E: // 8XYE
					ShiftLeftRegXC(opCode, gReg);
					break;
			}
			break;
		case 0x9000: // 9XY0
			SkipIfYDEqualsX(opCode, gReg, cReg);
			break;
		case 0xA000: // ANNN
			SetaRegToConst(opCode, aReg);
			break;
		case 0xB000: // BNNN
			JumpToConARegAddr(opCode, gReg, cReg);				
			break;
		case 0xC000: // CXKK
			GenRanNumANDConst(opCode, gReg);
			break;
		case 0xD000:
			break;
		case 0xE000:
			switch(opCode & 0x00FF) {
				case 0x009E:
					break;
				case 0x00A1:
					break;
			}
			break;
		case 0xF000:
			switch(opCode & 0x00F0) {
				case 0x0000:
					switch(opCode & 0x000F) {
						case 0x0007: // FX07
							MovdTimerToRegX(opCode, gReg, dTimer);
							break;
						case 0x000A:
							break;
					}
					break;
				case 0x0010:
					switch(opCode & 0x000F) {
						case 0x0005: // FX15
							SetDelTimToRegX(opCode, gReg, dTimer);
							break;
						case 0x0008: // FX18
							SetSouTimToRegX(opCode, gReg, sTimer);
							break;
						case 0x000E: // FX1E
							AddAddRegToRegX(opCode, gReg, aReg);
							break;
					}
					break;
				case 0x0020:
					break;
				case 0x0030:
					switch(opCode & 0x000F) {
						case 0x0000:
							break;
						case 0x0003:
							break;
					}
					break;
				case 0x0050: // FX55
					SavgRegToMemAtaReg(gReg, aReg, memory);
					break;
				case 0x0060: // FX65
					LoadgRegToMemAtaReg(gReg, aReg, memory);
					break;
				case 0x0070: //  - SUPER
					break;
				case 0x0080: //  - SUPER
					break;
			}
			break;
	}

	return;
}

bool RomToMemory(FILE *rom, unsigned char *memory) {
	bool errcheck = true;
	unsigned int size;
	unsigned int icount;

	fseek(rom, 0, SEEK_END);
	size = ftell(rom);
	
	fseek(rom, 0, SEEK_SET);
	icount = fread(memory, sizeof(char), size, rom);
	if(icount != size) errcheck = false;

	return errcheck;
}

bool InitHarware(unsigned char **gReg, unsigned short int *aReg, unsigned short int *cReg, unsigned short int **stack, unsigned char **memory, unsigned char *dTimer, unsigned char *sTimer, unsigned char *sCount) {
	int counter;
	unsigned char Chip8Font[80] = { 0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
				        0x20, 0x60, 0x20, 0x20, 0x70, // 1
				        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
				        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
				        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
				        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
				        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
				        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
				        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
				        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
				        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
				        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
				        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
				        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
				        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
				        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
				       };

	*memory = (unsigned char*)calloc(MEMORYSIZE, sizeof(char));	
	if(*memory == NULL) return false;
	for(counter = 0; counter < FONTCOUNT; counter++) {
		(*memory)[counter] = Chip8Font[counter];
	}
	for(counter = FONTCOUNT; counter < MEMORYSIZE; counter++) {
		(*memory)[counter] = 0;
	}

	*stack = (unsigned short int*)calloc(STACKSIZE, sizeof(short int));	
	if(*stack == NULL) return false;
	for(counter = 0; counter < STACKSIZE; counter++) {
		(*stack)[counter] = 0;
	}

	*gReg = (unsigned char*)calloc(REGISTERSIZE, sizeof(char));	
	if(*gReg == NULL) return false;
	for(counter = 0; counter < REGISTERSIZE; counter++) {
		(*gReg)[counter] = 0;
	}

	*aReg = 0;
	*cReg = ROMENTRY;
	*dTimer = 0;
	*sTimer = 0;
	*sCount = 0;

	return true;
}

unsigned short int FetchOpCode(unsigned char *memory, unsigned short int cReg) {
	unsigned short int opCode;

	opCode = (unsigned short int)memory[cReg];
	opCode <<= 8;
	opCode |= (unsigned short int)memory[cReg + 1];

	return opCode;
}

bool OpenRom(char *fileName, FILE **rom) {	
	bool errcheck = true;

	*rom = fopen(fileName, "rb");
	if(*rom == NULL) errcheck = false;

	return errcheck;
}
