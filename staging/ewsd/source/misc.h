#ifndef MISC_INC
#define MISC_INC

// value not test
#define DELAY_465K		1	// 0 PASS

#define DELAY_20M		0 // 0 is possible


extern u32 gReadWriteDelay;


char *strupr (char *s);
void rwDelay(u32 t);


void TFC_Command19(BYTE a1, BYTE a2, BYTE a3, BYTE a4, BYTE a5);
WORD SDT_Command1ARead(LONG addr);
void TFC_Command19_A(LONG addr, WORD d);


void SDT_SetFlashCSConfig(BYTE mode);

#endif //MISC_INC

