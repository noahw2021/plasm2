//
//  cpu_handlers.c
//  plasm2_emu
//
//  Created by Noah Wooten on 4/21/23.
//
#include "cpu.h"

DeclInstruction(MOV);
DeclInstruction(LDI);
DeclInstruction(JMP);
DeclInstruction(NXC);
DeclInstruction(NXE);
DeclInstruction(NXZ);
DeclInstruction(NXG);
DeclInstruction(NXL);
DeclInstruction(CLL);
DeclInstruction(RET);
DeclInstruction(IMR);
DeclInstruction(ADD);
DeclInstruction(SUB);
DeclInstruction(MUL);
DeclInstruction(DIV);
DeclInstruction(MOD);
DeclInstruction(AND);
DeclInstruction(BOR);
DeclInstruction(XOR);
DeclInstruction(LDW);
DeclInstruction(LDB);
DeclInstruction(STW);
DeclInstruction(STB);
DeclInstruction(PSH);
DeclInstruction(POP);
DeclInstruction(PSR);
DeclInstruction(POR);
DeclInstruction(VME);
DeclInstruction(VMD);
DeclInstruction(VPC);
DeclInstruction(VPD);
DeclInstruction(VSI);
DeclInstruction(VSD);
DeclInstruction(SPS);
DeclInstruction(SPG);
DeclInstruction(DSQ);
DeclInstruction(DSC);
DeclInstruction(DSD);
DeclInstruction(DGD);
DeclInstruction(DRS);
DeclInstruction(DPE);
DeclInstruction(DPD);
DeclInstruction(DGC);
DeclInstruction(INT);
DeclInstruction(HND);
DeclInstruction(IRT);
DeclInstruction(ENI);
DeclInstruction(DSI);
DeclInstruction(SHF);
DeclInstruction(SMH);
DeclInstruction(SIT);
DeclInstruction(CMP);
DeclInstruction(INC);
DeclInstruction(DEC);
DeclInstruction(BSL);
DeclInstruction(BSR);
DeclInstruction(NOT);
DeclInstruction(JMI);
DeclInstruction(CLI);
DeclInstruction(CMI);
DeclInstruction(ANI);
DeclInstruction(XOI);
DeclInstruction(ORI);
DeclInstruction(ADI);
DeclInstruction(SBI);
DeclInstruction(MLI);
DeclInstruction(DVI);
DeclInstruction(MDI);
DeclInstruction(BLI);
DeclInstruction(BRI);
DeclInstruction(LWS);
DeclInstruction(SWS);
DeclInstruction(LDH);
DeclInstruction(STH);
DeclInstruction(PSI);
DeclInstruction(CLR);
DeclInstruction(PPW);

void(*Instructions[256])(void) = {
	ListInstruction(MOV),
	ListInstruction(LDI),
	ListInstruction(JMP),
	ListInstruction(NXC),
	ListInstruction(NXE),
	ListInstruction(NXZ),
	ListInstruction(NXG),
	ListInstruction(NXL),
	ListInstruction(CLL),
	ListInstruction(RET),
	ListInstruction(IMR),
	ListInstruction(ADD),
	ListInstruction(SUB),
	ListInstruction(MUL),
	ListInstruction(DIV),
	ListInstruction(MOD),
	ListInstruction(AND),
	ListInstruction(BOR),
	ListInstruction(XOR),
	ListInstruction(LDW),
	ListInstruction(LDB),
	ListInstruction(STW),
	ListInstruction(STB),
	ListInstruction(PSH),
	ListInstruction(POP),
	ListInstruction(PSR),
	ListInstruction(POR),
	ListInstruction(VME),
	ListInstruction(VMD),
	ListInstruction(VPC),
	ListInstruction(VPD),
	ListInstruction(VSI),
	ListInstruction(VSD),
	ListInstruction(SPS),
	ListInstruction(SPG),
	ListInstruction(DSQ),
	ListInstruction(DSC),
	ListInstruction(DSD),
	ListInstruction(DGD),
	ListInstruction(DRS),
	ListInstruction(DPE),
	ListInstruction(DPD),
	ListInstruction(DGC),
	ListInstruction(INT),
	ListInstruction(HND),
	ListInstruction(IRT),
	ListInstruction(ENI),
	ListInstruction(DSI),
	ListInstruction(SHF),
	ListInstruction(SMH),
	ListInstruction(SIT),
	ListInstruction(CMP),
	ListInstruction(BSL),
	ListInstruction(BSR),
	ListInstruction(INC),
	ListInstruction(DEC),
	ListInstruction(NOT),
	ListInstruction(JMI),
	ListInstruction(CLI),
	ListInstruction(CMI),
	ListInstruction(ANI),
	ListInstruction(XOI),
	ListInstruction(ORI),
	ListInstruction(ADI),
	ListInstruction(SBI),
	ListInstruction(MLI),
	ListInstruction(DVI),
	ListInstruction(MDI),
	ListInstruction(BLI),
	ListInstruction(BRI),
	ListInstruction(LWS),
	ListInstruction(SWS),
	ListInstruction(LDH),
	ListInstruction(STH),
    ListInstruction(PSI),
    ListInstruction(CLR),
    ListInstruction(PPW),
};
