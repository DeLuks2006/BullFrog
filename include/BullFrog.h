#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

#define LOG(...) printf(##__VA_ARGS__ )

namespace Tadpole {

	enum class Regs {
		VM_IP = 0x0,
		VM_SP = 0x1,
		VM_R1 = 0x2,
		VM_R2 = 0x3,
		VM_R3 = 0x4,
		VM_R4 = 0x5,
		VM_R5 = 0x6,
		VM_R6 = 0x7,
		VM_R7 = 0x8,
		VM_R8 = 0x9
	};

	enum class Opcode
	{
		OP_PUSH = 0x0,
		OP_POP = 0x1,
		OP_ADD = 0x2,
		OP_SUB = 0x3,
		OP_MOV_IMM = 0x4,
		OP_MOV_MEM = 0x5,
		OP_XCHG = 0x6,
		OP_JMP = 0x7,
		OP_JMP_REG = 0x8,
		OP_JNZ = 0x9,
		OP_JE = 0xA,
		OP_AND = 0xB,
		OP_NOT = 0xC,
		OP_OR = 0xD,
		OP_XOR = 0xE,
		OP_CALL = 0xF,
		OP_RET = 0x10,
		OP_EXIT = 0x11,
		OP_FGETS = 0x12,
		OP_PRINT = 0x13,
	};

#pragma pack(push,1)
	union Instruction
	{
		struct { uint8_t IsReg; uint64_t Value; } PUSH;
		struct { uint8_t Reg; } POP;
		struct { uint8_t Dest; uint8_t IsMem; uint64_t Value; } ADD;
		struct { uint8_t Dest; uint8_t IsMem; uint64_t Value; } SUB;
		struct { uint8_t Dest; uint8_t IsReg; uint64_t Value; } MOV_IMM;
		struct { uint8_t RegToMem; uint64_t Src; uint8_t MemToReg; uint64_t Dest; } MOV_MEM;
		struct { uint8_t Reg1; uint8_t Reg2; } XCHG;
		struct { uint64_t Target; } JMP;
		struct { uint8_t Reg; } JMP_REG;
		struct { uint8_t CondReg; uint8_t IsReg; uint64_t Target; } JNZ;
		struct { uint8_t Dest; uint8_t IsReg; uint64_t Src; uint64_t Target; } JE;
		struct { uint8_t Dest; uint8_t IsMem; uint64_t Value; } AND;
		struct { uint8_t Reg; } NOT;
		struct { uint8_t Dest; uint8_t IsMem; uint64_t Value; } OR;
		struct { uint8_t Dest; uint8_t IsMem; uint64_t Value; } XOR;
		struct { uint8_t IsReg; uint64_t Target; } CALL;
		// struct { } RET;
		// struct { } EXIT;
		struct { uint8_t IsReg; uint64_t BufAddr; uint64_t MaxCount; } FGETS;
		struct { uint8_t IsReg; uint64_t SrcAddr; uint64_t Length; } PRINT;
	};
#pragma pack(pop)

#pragma pack(push, 1)
	typedef struct {
		__int64		Regs[10];
		__int8		Mem[8192];
		__int64		Stack[1024];
	} VM_CTX;
#pragma pack(pop)

	const char* GetRegister(__int64 i64Reg);

	size_t Decoder(const uint8_t opcode, const uint8_t* mem, uint64_t pc);

	bool Disassemble(std::string target);
};
