#pragma once
/*
cg.h
plasm2
plasm2_asm
(c) Noah Wooten 2023, All Rights Reserved
*/

void cg_init(void);
void cg_shutdown(void);

void cg_parse(const char* Line);

void cge_error(int Line, const char* Reason);
int  cge_errorcnt(void);

void cgp_put1(byte Data);
void cgp_put8(u64 Data);

typedef struct _cgctx {
	int LineCount;
	int CurrentLine;

	int ErrorCount;
	struct {
		int Line;
		char* Reason;
	}*Errors;
}cgctx_t;
extern cgctx_t* cgctx;