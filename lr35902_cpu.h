#pragma once

#include <cstdint>

#include "memory.h"
#define CPU_FREQ 4194304

class lr35902_cpu
{
public:
	lr35902_cpu(Memory& memory);

	uint8_t update();
	void run(uint32_t cycles);

private:
	uint8_t nop(uint8_t opcode);

	using instructionHandler = decltype(&lr35902_cpu::nop);

	static instructionHandler handlers[256];
	static instructionHandler cb_handlers[256];

	struct registers_t {
		uint8_t A;			// Accumulator
		union {
			uint16_t	w;
			uint8_t		b[2];
		} BC; 				// General purpose
		union {
			uint16_t	w;
			uint8_t		b[2];
		} DE;				// General purpose
		union {
			uint16_t	w;
			uint8_t		b[2];
		} HL;				// General purpose
		union {
			uint16_t	w;
			uint8_t		b[2];
		} SP;				// Stack pointer
		uint16_t PC;		// Program counter
		bool ZF;			// Zero flag
		bool N;				// Add/Sub flag
		bool H;				// Half carry flag
		bool CY;			// Carry flag

		uint8_t IR;
	} m_registers;

	bool m_halted = false;
	bool m_stoped = false;
	bool m_maskedInterrupts = false;
	bool m_maskInterrupts = false;
	bool m_unMaskInterrupts = false;

	uint64_t m_clkCounter = 0;
	uint32_t m_timerDivider = CPU_FREQ;
	bool m_timerEnabled = false;

	Memory& m_memory;

	uint8_t fetchIR();

	void processTimer(uint8_t executedCycles);
	uint8_t handleInterrupts();

	inline uint8_t getValForReg3b(uint8_t regnum);
	inline uint16_t getValForReg2b(uint8_t regnum);
	inline uint8_t getFlags8b();

	uint8_t stop(uint8_t opcode);
	uint8_t jr_r8(uint8_t opcode);
	uint8_t ld_d16(uint8_t opcode);
	uint8_t ld_a(uint8_t opcode);
	uint8_t inc_16(uint8_t opcode);
	uint8_t dec_16(uint8_t opcode);
	uint8_t inc_8(uint8_t opcode);
	uint8_t inc_hl(uint8_t opcode);
	uint8_t dec_8(uint8_t opcode);
	uint8_t dec_hl(uint8_t opcode);
	uint8_t ld_d8(uint8_t opcode);
	uint8_t rlca(uint8_t opcode);
	uint8_t rla(uint8_t opcode);
	uint8_t rrca(uint8_t opcode);
	uint8_t rra(uint8_t opcode);
	uint8_t daa(uint8_t opcode);
	uint8_t scf(uint8_t opcode);
	uint8_t cpl(uint8_t opcode);
	uint8_t ccf(uint8_t opcode);
	uint8_t ld_a16_sp(uint8_t opcode);
	uint8_t add_hl(uint8_t opcode);
	uint8_t ld_reg8(uint8_t opcode);
	uint8_t halt(uint8_t opcode);
	uint8_t add_a(uint8_t opcode);
	uint8_t sub(uint8_t opcode);
	uint8_t adc(uint8_t opcode);
	uint8_t sbc(uint8_t opcode);
	uint8_t and_a(uint8_t opcode);
	uint8_t xor_a(uint8_t opcode);
	uint8_t or_a(uint8_t opcode);
	uint8_t cp(uint8_t opcode);
	uint8_t invalid(uint8_t opcode);
	uint8_t ldh(uint8_t opcode);
	uint8_t jp_cc_a16(uint8_t opcode);
	uint8_t call_a16 (uint8_t opcode);
	uint8_t rst(uint8_t opcode);
	uint8_t di(uint8_t opcode);
	uint8_t ei(uint8_t opcode);
	uint8_t add_sp_r8(uint8_t opcode);
	uint8_t jp_hl(uint8_t opcode);
	uint8_t ldhl_spr8(uint8_t opcode);
	uint8_t ld_sp_hl(uint8_t opcode);
	uint8_t ld_a_a16(uint8_t opcode);
	uint8_t ret_cc(uint8_t opcode);
	uint8_t push(uint8_t opcode);
	uint8_t pop(uint8_t opcode);
	uint8_t reti(uint8_t opcode);
	uint8_t ret(uint8_t opcode);
	uint8_t jp_a16(uint8_t opcode);

	uint8_t cb(uint8_t opcode);
	uint8_t rlc(uint8_t opcode);
	uint8_t rrc(uint8_t opcode);
	uint8_t rl(uint8_t opcode);
	uint8_t rr(uint8_t opcode);
	uint8_t sla(uint8_t opcode);
	uint8_t sra(uint8_t opcode);
	uint8_t swap(uint8_t opcode);
	uint8_t srl(uint8_t opcode);
	uint8_t bit(uint8_t opcode);
	uint8_t res(uint8_t opcode);
	uint8_t set(uint8_t opcode);

	void write(uint16_t address, uint8_t data);

	void debugDisplay();
};
