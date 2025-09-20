#include "../include/BullFrog.h"

namespace Tadpole {
	bool Disassemble(std::string path) {
		std::ifstream f(path, std::ios::binary | std::ios::ate);
		if (!f.is_open())
			return false;

		std::streamsize size = f.tellg();
		if (size <= 0)
			return false;

		f.seekg(0, std::ios::beg);

		std::vector<uint8_t> buffer(size);
		if (!f.read((char*)buffer.data(), size))
			return false;

		f.close();

		uint64_t pc = 0;
		while (pc < (uint64_t)buffer.size()) {
			uint8_t opcode = buffer[pc];
			pc += Decoder(opcode, buffer.data(), pc);
		}

		return true;
	}

	const char* GetRegister(__int64 i64Reg) {
		constexpr const char* regs[] = { "IP", "SP", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8" };
		if (i64Reg >= 0 && i64Reg < 9) return regs[i64Reg];
		return "R?";
	}

	size_t Decoder(uint8_t opcode, const uint8_t* mem, uint64_t pc)
	{
		const Instruction* ins = reinterpret_cast<const Instruction*>(&mem[pc+1]);
		LOG("0x%04llx > ", pc);
		switch ((Opcode)opcode)
		{
		case Opcode::OP_PUSH:
			if (ins->PUSH.IsReg)
				LOG("PUSH %s\n", GetRegister((uint8_t)ins->PUSH.Value));
			else
				LOG("PUSH 0x%llx\n", (uint64_t)ins->PUSH.Value);
			return 10;

		case Opcode::OP_POP:
			LOG("POP %s\n", GetRegister(ins->POP.Reg));
			return 2;

		case Opcode::OP_ADD:
			if (ins->ADD.IsMem)
				LOG("ADD %s, %s\n", GetRegister(ins->ADD.Dest), GetRegister((uint8_t)ins->ADD.Value));
			else
				LOG("ADD %s, 0x%llx\n", GetRegister(ins->ADD.Dest), (uint64_t)ins->ADD.Value);
			return 11;

		case Opcode::OP_SUB:
			if (ins->SUB.IsMem)
				LOG("SUB %s, %s\n", GetRegister(ins->SUB.Dest), GetRegister((uint8_t)ins->SUB.Value));
			else
				LOG("SUB %s, 0x%llx\n", GetRegister(ins->SUB.Dest), (uint64_t)ins->SUB.Value);
			return 11;

		case Opcode::OP_MOV_IMM:
			if (ins->MOV_IMM.IsReg)
				LOG("MOV %s, %s\n", GetRegister(ins->MOV_IMM.Dest), GetRegister((uint8_t)ins->MOV_IMM.Value));
			else
				LOG("MOV %s, 0x%llx\n", GetRegister(ins->MOV_IMM.Dest), (uint64_t)ins->MOV_IMM.Value);
			return 11;

		case Opcode::OP_MOV_MEM:
			if (ins->MOV_MEM.RegToMem && !ins->MOV_MEM.MemToReg)
				LOG("MOV [0x%llx], %s\n",
					(uint64_t)ins->MOV_MEM.Dest,
					GetRegister((uint8_t)ins->MOV_MEM.Src));
			else if (!ins->MOV_MEM.RegToMem && ins->MOV_MEM.MemToReg)
				LOG("MOV %s, [0x%llx]\n",
					GetRegister((uint8_t)ins->MOV_MEM.Dest),
					(uint64_t)ins->MOV_MEM.Src);
			else
				LOG("MOV ???\n");
			return 19;

		case Opcode::OP_XCHG:
			LOG("XCHG %s, %s\n", GetRegister(ins->XCHG.Reg1), GetRegister(ins->XCHG.Reg2));
			return 3;

		case Opcode::OP_JMP:
			LOG("JMP 0x%llx\n", (uint64_t)ins->JMP.Target);
			return 9;

		case Opcode::OP_JMP_REG:
			LOG("JMP %s\n", GetRegister(ins->JMP_REG.Reg));
			return 2;

		case Opcode::OP_JNZ:
			if (ins->JNZ.IsReg)
				LOG("JNZ %s, %s\n", GetRegister(ins->JNZ.CondReg), GetRegister((uint8_t)ins->JNZ.Target));
			else
				LOG("JNZ %s, 0x%llx\n", GetRegister(ins->JNZ.CondReg), (uint64_t)ins->JNZ.Target);
			return 11;

		case Opcode::OP_JE:
			if (ins->JE.IsReg)
				LOG("JE %s, %s, 0x%llx\n",
					GetRegister(ins->JE.Dest),
					GetRegister((uint8_t)ins->JE.Src),
					(uint64_t)ins->JE.Target);
			else
				LOG("JE %s, 0x%llx, 0x%llx\n",
					GetRegister(ins->JE.Dest),
					(uint64_t)ins->JE.Src,
					(uint64_t)ins->JE.Target);
			return 18;

		case Opcode::OP_AND:
			if (ins->AND.IsMem)
				LOG("AND %s, %s\n", GetRegister(ins->AND.Dest), GetRegister((uint8_t)ins->AND.Value));
			else
				LOG("AND %s, 0x%llx\n", GetRegister(ins->AND.Dest), (uint64_t)ins->AND.Value);
			return 11;

		case Opcode::OP_NOT:
			LOG("NOT %s\n", GetRegister(ins->NOT.Reg));
			return 2;

		case Opcode::OP_OR:
			if (ins->OR.IsMem)
				LOG("OR %s, %s\n", GetRegister(ins->OR.Dest), GetRegister((uint8_t)ins->OR.Value));
			else
				LOG("OR %s, 0x%llx\n", GetRegister(ins->OR.Dest), (uint64_t)ins->OR.Value);
			return 11;

		case Opcode::OP_XOR:
			if (ins->XOR.IsMem)
				LOG("XOR %s, %s\n", GetRegister(ins->XOR.Dest), GetRegister((uint8_t)ins->XOR.Value));
			else
				LOG("XOR %s, 0x%llx\n", GetRegister(ins->XOR.Dest), (uint64_t)ins->XOR.Value);
			return 11;

		case Opcode::OP_CALL:
			if (ins->CALL.IsReg)
				LOG("CALL %s\n", GetRegister((uint8_t)ins->CALL.Target));
			else
				LOG("CALL 0x%llx\n", (uint64_t)ins->CALL.Target);
			return 10;

		case Opcode::OP_RET:
			LOG("RET\n");
			return 1;

		case Opcode::OP_EXIT:
			LOG("EXIT\n");
			return 1;

		case Opcode::OP_FGETS:
			if (ins->FGETS.IsReg)
				LOG("FGETS %s, 0x%llx\n", GetRegister((uint8_t)ins->FGETS.BufAddr), (uint64_t)ins->FGETS.MaxCount);
			else
				LOG("FGETS 0x%llx, 0x%llx\n", (uint64_t)ins->FGETS.BufAddr, (uint64_t)ins->FGETS.MaxCount);
			return 18;

		case Opcode::OP_PRINT:
			if (ins->PRINT.IsReg)
				LOG("PRINT %s, len=0x%llx\n", GetRegister((uint8_t)ins->PRINT.SrcAddr), (uint64_t)ins->PRINT.Length);
			else
				LOG("PRINT 0x%llx, len=0x%llx\n", (uint64_t)ins->PRINT.SrcAddr, (uint64_t)ins->PRINT.Length);
			return 18;

		default:
			LOG("DB 0x%02X\n", opcode);
			return 1;
		}
	}
};
