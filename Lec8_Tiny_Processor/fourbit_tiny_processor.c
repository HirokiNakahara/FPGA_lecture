#include <stdio.h>
#include <stdlib.h>

// Program ROM
int ROM[16] = {
	/* 3min timer for noodle? */
	0x87, // OUT 0111
	0x41, // ADD A,0001
	0xB1, // JNC 0001
	0x41, // ADD A,001
	0xB3, // JNC 0011
	0x86, // OUT 0110
	0x41, // ADD A,0001
	0xB6, // JNC 0110
	0x41, // ADD A,0001
	0xB8, // JNC 1000
	0x80, // OUT 0000
	0x84, // OUT 0100
	0x41, // ADD 0001
	0xBA, // JNC 1010
	0x88, // OUT 1000
	0xAF, // JMP 1111
	/* led ctrl */
	/*
	0x81,
	0x82,
	0x84,
	0x88,
	0x84,
	0x82,
	0xA0,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	*/
	/* template */
	/*
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	*/
};

void fourbit_computer();

int main()
{
	int INPUT;
	int OUTPUT;

	INPUT = 0x0;
	OUTPUT = 0x0;

	fourbit_computer( INPUT, &OUTPUT);

	return 0;
}

void fourbit_computer( int INPUT, int *OUTPUT)
{
	int PC; // program counter
	int ROM_DATA; // rom data
	int INST; // instruction
	int IMM; // immediate value
	int REGA; // register A
	int REGB; // register B
	int CF; // carry flag

	int sel_idx; // selector index
	int sel_val; // selected signal
	int add_result; // 5bit (4+1) result of addition
	int wr_en; // 4bit write enable signal
	int jmp_en; // jump enable flag

	int step; // step counter for debug

	// initialization
	PC = 0;
	REGA = 0;
	REGB = 0;
	CF = 0;

	step = 0;

	// main loop
	while(1){
		// 1. Read ROM
		ROM_DATA = ROM[PC];
		INST = (ROM_DATA >> 4) & 0xF;
		IMM  = ROM_DATA & 0xF;

		printf("STEP%3d: PC=%X ROM=%X INST=%X IMM=%x\n", step, PC, ROM[PC], INST, IMM);

		// 2. Decode INST
		jmp_en = 0;

		switch( INST){
			case 0x0: // LOAD_A A<-IMM
				sel_idx = 0x8;
				wr_en   = 0x1;
			break;
			case 0x1: // LOAD_B B<-IMM
				sel_idx = 0x8;
				wr_en   = 0x2;
			break;
			case 0x2: // MOV_A A<-B
				sel_idx = 0x2;
				wr_en   = 0x1;				
			break;
			case 0x3: // MOV_B B<-A
				sel_idx = 0x1;
				wr_en   = 0x2;
			break;
			case 0x4: // ADD_A A<-A+IMM
				sel_idx = 0x1;
				wr_en   = 0x1;
			break;
			case 0x5: // ADD_B B<-A+IMM
				sel_idx = 0x2;
				wr_en   = 0x2;
			break;
			case 0x6: // IN_A A<-INPUT
				sel_idx = 0x4;
				wr_en   = 0x1;
			break;
			case 0x7: // IN_B B<-INPUT
				sel_idx = 0x4;
				wr_en   = 0x2;
			break;
			case 0x8: // OUT_IMM OUT<-IMM
				sel_idx = 0x8;
				wr_en   = 0x4;
			break;
			case 0x9: // OUT_B OUT<-B
				sel_idx = 0x2;
				wr_en   = 0x4;
			break;
			case 0xA: // JMP_IMM PC<-IMM
				sel_idx = 0x8;
				wr_en   = 0x8;
				jmp_en  = 1;
			break;
			case 0xB: // JNC IF CF==0 THEN PC<-IMM, ELSE PC<-PC+1
				sel_idx = 0x8;
				wr_en   = 0x8;
				if( CF == 0){
					jmp_en = 1;
				} else {
					jmp_en = 0;
				}
			break;
			case 0xC:
				// UNDEFINED
				sel_idx = 0x8;
				wr_en   = 0x0;				
			break;
			case 0xD:
				// UNDEFINED
				sel_idx = 0x8;
				wr_en   = 0x0;				
			break;
			case 0xE:
				// UNDEFINED
				sel_idx = 0x8;
				wr_en   = 0x0;				
			break;
			case 0xF:
				// NOP
				sel_idx = 0x8;
				wr_en   = 0x0;				
			break;
			default:
				fprintf( stderr, "ERROR: EXEC OF UNDEF INST\n");
				exit(-1);
			break;
		}

		printf(" sel_idx=%X wr_en=%X\n", sel_idx, wr_en);

		// 3. Select REG
		switch( sel_idx){
			case 0x1:
				sel_val = REGA;
				break;
			case 0x2:
				sel_val = REGB;
				break;
			case 0x4:
				sel_val = INPUT;
				break;
			case 0x8:
				sel_val = 0x0; // constant zero
				break;
			default:
				fprintf( stderr, "ERROR: UNKNOWN REG ACCESS\n");
				exit(-1);
				break;
		}

		// 4. Execute operation
		add_result = sel_val + IMM;

		// set carry flag (CF)
		if( (add_result & 0x10) != 0x0){
			CF = 1;
		} else {
			CF = 0;
		}

		// 5. Writeback REG
		// also, update PC
		switch( wr_en){
			case 0x1:
				REGA = add_result & 0xF;
				PC++;
			break;
			case 0x2:
				REGB = add_result & 0xF;
				PC++;
			break;
			case 0x4:
				*OUTPUT = add_result & 0xF;
				PC++;
			break;
			case 0x8:
				if( jmp_en == 1){
					PC = add_result & 0xF;
				} else {
					PC++;
				}
			break;
			default:
				fprintf( stderr, "WARNING: NON WRITEBACK (MAYBE NOP?)\n");
				exit(-1);
			break;
		}

		PC = (PC & 0xF);

		printf(" NextPC=%X, REGA=%X, REGB=%X, OUT=%X\n", PC, REGA, REGB, *OUTPUT);

		step++;
	}
}
