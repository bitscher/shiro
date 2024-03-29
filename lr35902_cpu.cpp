#include "config.h"
#include "lr35902_cpu.h"
#include "utils.h"

#include <iostream>
#define SWITCHDEFAULTERR(func, opcode) default: fprintf(stderr,  "Error in %s(): unknown opcode: 0x%02X\n", func, opcode);
#define OPCODEERROR(func, opcode) fprintf(stderr,  "Error in %s(): unknown opcode: 0x%02X\n", func, opcode);
#define NOTIMPLEMENTED(func) std::cerr << "Error: " << func << "() not implemented" << std::endl;

#ifdef _DEBUG_DISPLAY
	#include <cstdio>
	#define OUTPUT_DEBUG(format,...) {printf(format, __VA_ARGS__);}
#else
	#define OUTPUT_DEBUG(format,...)
#endif


lr35902_cpu::instructionHandler lr35902_cpu::handlers[] = {
	/*         0                       1                       2                         3                        4                        5                       6                        7                      8                         9                       A                         B                       C                        D                        E                       F */
	/* 0 */    &lr35902_cpu::nop,      &lr35902_cpu::ld_d16,   &lr35902_cpu::ld_a,       &lr35902_cpu::inc_16,    &lr35902_cpu::inc_8,     &lr35902_cpu::dec_8,    &lr35902_cpu::ld_d8,    &lr35902_cpu::rlca,     &lr35902_cpu::ld_a16_sp,  &lr35902_cpu::add_hl,   &lr35902_cpu::ld_a,       &lr35902_cpu::dec_16,   &lr35902_cpu::inc_8,     &lr35902_cpu::dec_8,     &lr35902_cpu::ld_d8,    &lr35902_cpu::rrca,
	/* 1 */    &lr35902_cpu::stop,     &lr35902_cpu::ld_d16,   &lr35902_cpu::ld_a,       &lr35902_cpu::inc_16,    &lr35902_cpu::inc_8,     &lr35902_cpu::dec_8,    &lr35902_cpu::ld_d8,    &lr35902_cpu::rla,      &lr35902_cpu::jr_r8,      &lr35902_cpu::add_hl,   &lr35902_cpu::ld_a,       &lr35902_cpu::dec_16,   &lr35902_cpu::inc_8,     &lr35902_cpu::dec_8,     &lr35902_cpu::ld_d8,    &lr35902_cpu::rra,
	/* 2 */    &lr35902_cpu::jr_r8,    &lr35902_cpu::ld_d16,   &lr35902_cpu::ld_a,       &lr35902_cpu::inc_16,    &lr35902_cpu::inc_8,     &lr35902_cpu::dec_8,    &lr35902_cpu::ld_d8,    &lr35902_cpu::daa,      &lr35902_cpu::jr_r8,      &lr35902_cpu::add_hl,   &lr35902_cpu::ld_a,       &lr35902_cpu::dec_16,   &lr35902_cpu::inc_8,     &lr35902_cpu::dec_8,     &lr35902_cpu::ld_d8,    &lr35902_cpu::cpl,
	/* 3 */    &lr35902_cpu::jr_r8,    &lr35902_cpu::ld_d16,   &lr35902_cpu::ld_a,       &lr35902_cpu::inc_16,    &lr35902_cpu::inc_hl,    &lr35902_cpu::dec_hl,   &lr35902_cpu::ld_d8,    &lr35902_cpu::scf,      &lr35902_cpu::jr_r8,      &lr35902_cpu::add_hl,   &lr35902_cpu::ld_a,       &lr35902_cpu::dec_16,   &lr35902_cpu::inc_8,     &lr35902_cpu::dec_8,     &lr35902_cpu::ld_d8,    &lr35902_cpu::ccf,
	/* 4 */    &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,    &lr35902_cpu::ld_reg8,   &lr35902_cpu::ld_reg8,   &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,    &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,    &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,   &lr35902_cpu::ld_reg8,   &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,
	/* 5 */    &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,    &lr35902_cpu::ld_reg8,   &lr35902_cpu::ld_reg8,   &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,    &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,    &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,   &lr35902_cpu::ld_reg8,   &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,
	/* 6 */    &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,    &lr35902_cpu::ld_reg8,   &lr35902_cpu::ld_reg8,   &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,    &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,    &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,   &lr35902_cpu::ld_reg8,   &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,
	/* 7 */    &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,    &lr35902_cpu::ld_reg8,   &lr35902_cpu::ld_reg8,   &lr35902_cpu::ld_reg8,  &lr35902_cpu::halt,     &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,    &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,    &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,   &lr35902_cpu::ld_reg8,   &lr35902_cpu::ld_reg8,  &lr35902_cpu::ld_reg8,
	/* 8 */    &lr35902_cpu::add_a,    &lr35902_cpu::add_a,    &lr35902_cpu::add_a,      &lr35902_cpu::add_a,     &lr35902_cpu::add_a,     &lr35902_cpu::add_a,    &lr35902_cpu::add_a,    &lr35902_cpu::add_a,    &lr35902_cpu::adc,        &lr35902_cpu::adc,      &lr35902_cpu::adc,        &lr35902_cpu::adc,      &lr35902_cpu::adc,       &lr35902_cpu::adc,       &lr35902_cpu::adc,      &lr35902_cpu::adc,
	/* 9 */    &lr35902_cpu::sub,      &lr35902_cpu::sub,      &lr35902_cpu::sub,        &lr35902_cpu::sub,       &lr35902_cpu::sub,       &lr35902_cpu::sub,      &lr35902_cpu::sub,      &lr35902_cpu::sub,      &lr35902_cpu::sbc,        &lr35902_cpu::sbc,      &lr35902_cpu::sbc,        &lr35902_cpu::sbc,      &lr35902_cpu::sbc,       &lr35902_cpu::sbc,       &lr35902_cpu::sbc,      &lr35902_cpu::sbc,
	/* A */    &lr35902_cpu::and_a,    &lr35902_cpu::and_a,    &lr35902_cpu::and_a,      &lr35902_cpu::and_a,     &lr35902_cpu::and_a,     &lr35902_cpu::and_a,    &lr35902_cpu::and_a,    &lr35902_cpu::and_a,    &lr35902_cpu::xor_a,      &lr35902_cpu::xor_a,    &lr35902_cpu::xor_a,      &lr35902_cpu::xor_a,    &lr35902_cpu::xor_a,     &lr35902_cpu::xor_a,     &lr35902_cpu::xor_a,    &lr35902_cpu::xor_a,
	/* B */    &lr35902_cpu::or_a,     &lr35902_cpu::or_a,     &lr35902_cpu::or_a,       &lr35902_cpu::or_a,      &lr35902_cpu::or_a,      &lr35902_cpu::or_a,     &lr35902_cpu::or_a,     &lr35902_cpu::or_a,     &lr35902_cpu::cp,         &lr35902_cpu::cp,       &lr35902_cpu::cp,         &lr35902_cpu::cp,       &lr35902_cpu::cp,        &lr35902_cpu::cp,        &lr35902_cpu::cp,       &lr35902_cpu::cp,
	/* C */    &lr35902_cpu::ret_cc,   &lr35902_cpu::pop,      &lr35902_cpu::jp_cc_a16,  &lr35902_cpu::jp_a16,    &lr35902_cpu::call_a16,  &lr35902_cpu::push,     &lr35902_cpu::add_a,    &lr35902_cpu::rst,      &lr35902_cpu::ret_cc,     &lr35902_cpu::ret,      &lr35902_cpu::jp_cc_a16,  &lr35902_cpu::cb,       &lr35902_cpu::call_a16,  &lr35902_cpu::call_a16,  &lr35902_cpu::adc,      &lr35902_cpu::rst,
	/* D */    &lr35902_cpu::ret_cc,   &lr35902_cpu::pop,      &lr35902_cpu::jp_cc_a16,  &lr35902_cpu::invalid,   &lr35902_cpu::call_a16,  &lr35902_cpu::push,     &lr35902_cpu::sub,      &lr35902_cpu::rst,      &lr35902_cpu::ret_cc,     &lr35902_cpu::reti,     &lr35902_cpu::jp_cc_a16,  &lr35902_cpu::invalid,  &lr35902_cpu::call_a16,  &lr35902_cpu::invalid,   &lr35902_cpu::sbc,      &lr35902_cpu::rst,
	/* E */    &lr35902_cpu::ldh,      &lr35902_cpu::pop,      &lr35902_cpu::ld_a,       &lr35902_cpu::invalid,   &lr35902_cpu::invalid,   &lr35902_cpu::push,     &lr35902_cpu::and_a,    &lr35902_cpu::rst,      &lr35902_cpu::add_sp_r8,  &lr35902_cpu::jp_hl,    &lr35902_cpu::ld_a_a16,   &lr35902_cpu::invalid,  &lr35902_cpu::invalid,   &lr35902_cpu::invalid,   &lr35902_cpu::xor_a,    &lr35902_cpu::rst,
	/* F */    &lr35902_cpu::ldh,      &lr35902_cpu::pop,      &lr35902_cpu::ld_a,       &lr35902_cpu::di,        &lr35902_cpu::invalid,   &lr35902_cpu::push,     &lr35902_cpu::or_a,     &lr35902_cpu::rst,      &lr35902_cpu::ldhl_spr8,  &lr35902_cpu::ld_sp_hl, &lr35902_cpu::ld_a_a16,   &lr35902_cpu::ei,       &lr35902_cpu::invalid,   &lr35902_cpu::invalid,   &lr35902_cpu::cp,       &lr35902_cpu::rst
};

lr35902_cpu::instructionHandler lr35902_cpu::cb_handlers[] = {
	/*         0                       1                       2                        3                        4                         5                       6                       7                       8                         9                       A                         B                       C                        D                        E                       F */
	/* 0 */    &lr35902_cpu::rlc,      &lr35902_cpu::rlc,      &lr35902_cpu::rlc,       &lr35902_cpu::rlc,       &lr35902_cpu::rlc,        &lr35902_cpu::rlc,      &lr35902_cpu::rlcHL,    &lr35902_cpu::rlc,      &lr35902_cpu::rrc,        &lr35902_cpu::rrc,      &lr35902_cpu::rrc,        &lr35902_cpu::rrc,      &lr35902_cpu::rrc,       &lr35902_cpu::rrc,       &lr35902_cpu::rrcHL,    &lr35902_cpu::rrc,
	/* 1 */    &lr35902_cpu::rl,       &lr35902_cpu::rl,       &lr35902_cpu::rl,        &lr35902_cpu::rl,        &lr35902_cpu::rl,         &lr35902_cpu::rl,       &lr35902_cpu::rlHL,     &lr35902_cpu::rl,       &lr35902_cpu::rr,         &lr35902_cpu::rr,       &lr35902_cpu::rr,         &lr35902_cpu::rr,       &lr35902_cpu::rr,        &lr35902_cpu::rr,        &lr35902_cpu::rrHL,     &lr35902_cpu::rr,
	/* 2 */    &lr35902_cpu::sla,      &lr35902_cpu::sla,      &lr35902_cpu::sla,       &lr35902_cpu::sla,       &lr35902_cpu::sla,        &lr35902_cpu::sla,      &lr35902_cpu::slaHL,    &lr35902_cpu::sla,      &lr35902_cpu::sra,        &lr35902_cpu::sra,      &lr35902_cpu::sra,        &lr35902_cpu::sra,      &lr35902_cpu::sra,       &lr35902_cpu::sra,       &lr35902_cpu::sraHL,    &lr35902_cpu::sra,
	/* 3 */    &lr35902_cpu::swap,     &lr35902_cpu::swap,     &lr35902_cpu::swap,      &lr35902_cpu::swap,      &lr35902_cpu::swap,       &lr35902_cpu::swap,     &lr35902_cpu::swapHL,   &lr35902_cpu::swap,     &lr35902_cpu::srl,        &lr35902_cpu::srl,      &lr35902_cpu::srl,        &lr35902_cpu::srl,      &lr35902_cpu::srl,       &lr35902_cpu::srl,       &lr35902_cpu::srlHL,    &lr35902_cpu::srl,
	/* 4 */    &lr35902_cpu::bit,      &lr35902_cpu::bit,      &lr35902_cpu::bit,       &lr35902_cpu::bit,       &lr35902_cpu::bit,        &lr35902_cpu::bit,      &lr35902_cpu::bitHL,    &lr35902_cpu::bit,      &lr35902_cpu::bit,        &lr35902_cpu::bit,      &lr35902_cpu::bit,        &lr35902_cpu::bit,      &lr35902_cpu::bit,       &lr35902_cpu::bit,       &lr35902_cpu::bitHL,    &lr35902_cpu::bit,
	/* 5 */    &lr35902_cpu::bit,      &lr35902_cpu::bit,      &lr35902_cpu::bit,       &lr35902_cpu::bit,       &lr35902_cpu::bit,        &lr35902_cpu::bit,      &lr35902_cpu::bitHL,    &lr35902_cpu::bit,      &lr35902_cpu::bit,        &lr35902_cpu::bit,      &lr35902_cpu::bit,        &lr35902_cpu::bit,      &lr35902_cpu::bit,       &lr35902_cpu::bit,       &lr35902_cpu::bitHL,    &lr35902_cpu::bit,
	/* 6 */    &lr35902_cpu::bit,      &lr35902_cpu::bit,      &lr35902_cpu::bit,       &lr35902_cpu::bit,       &lr35902_cpu::bit,        &lr35902_cpu::bit,      &lr35902_cpu::bitHL,    &lr35902_cpu::bit,      &lr35902_cpu::bit,        &lr35902_cpu::bit,      &lr35902_cpu::bit,        &lr35902_cpu::bit,      &lr35902_cpu::bit,       &lr35902_cpu::bit,       &lr35902_cpu::bitHL,    &lr35902_cpu::bit,
	/* 7 */    &lr35902_cpu::bit,      &lr35902_cpu::bit,      &lr35902_cpu::bit,       &lr35902_cpu::bit,       &lr35902_cpu::bit,        &lr35902_cpu::bit,      &lr35902_cpu::bitHL,    &lr35902_cpu::bit,      &lr35902_cpu::bit,        &lr35902_cpu::bit,      &lr35902_cpu::bit,        &lr35902_cpu::bit,      &lr35902_cpu::bit,       &lr35902_cpu::bit,       &lr35902_cpu::bitHL,    &lr35902_cpu::bit,
	/* 8 */    &lr35902_cpu::res,      &lr35902_cpu::res,      &lr35902_cpu::res,       &lr35902_cpu::res,       &lr35902_cpu::res,        &lr35902_cpu::res,      &lr35902_cpu::resHL,    &lr35902_cpu::res,      &lr35902_cpu::res,        &lr35902_cpu::res,      &lr35902_cpu::res,        &lr35902_cpu::res,      &lr35902_cpu::res,       &lr35902_cpu::res,       &lr35902_cpu::resHL,    &lr35902_cpu::res,
	/* 9 */    &lr35902_cpu::res,      &lr35902_cpu::res,      &lr35902_cpu::res,       &lr35902_cpu::res,       &lr35902_cpu::res,        &lr35902_cpu::res,      &lr35902_cpu::resHL,    &lr35902_cpu::res,      &lr35902_cpu::res,        &lr35902_cpu::res,      &lr35902_cpu::res,        &lr35902_cpu::res,      &lr35902_cpu::res,       &lr35902_cpu::res,       &lr35902_cpu::resHL,    &lr35902_cpu::res,
	/* A */    &lr35902_cpu::res,      &lr35902_cpu::res,      &lr35902_cpu::res,       &lr35902_cpu::res,       &lr35902_cpu::res,        &lr35902_cpu::res,      &lr35902_cpu::resHL,    &lr35902_cpu::res,      &lr35902_cpu::res,        &lr35902_cpu::res,      &lr35902_cpu::res,        &lr35902_cpu::res,      &lr35902_cpu::res,       &lr35902_cpu::res,       &lr35902_cpu::resHL,    &lr35902_cpu::res,
	/* B */    &lr35902_cpu::res,      &lr35902_cpu::res,      &lr35902_cpu::res,       &lr35902_cpu::res,       &lr35902_cpu::res,        &lr35902_cpu::res,      &lr35902_cpu::resHL,    &lr35902_cpu::res,      &lr35902_cpu::res,        &lr35902_cpu::res,      &lr35902_cpu::res,        &lr35902_cpu::res,      &lr35902_cpu::res,       &lr35902_cpu::res,       &lr35902_cpu::resHL,    &lr35902_cpu::res,
	/* C */    &lr35902_cpu::set,      &lr35902_cpu::set,      &lr35902_cpu::set,       &lr35902_cpu::set,       &lr35902_cpu::set,        &lr35902_cpu::set,      &lr35902_cpu::setHL,    &lr35902_cpu::set,      &lr35902_cpu::set,        &lr35902_cpu::set,      &lr35902_cpu::set,        &lr35902_cpu::set,      &lr35902_cpu::set,       &lr35902_cpu::set,       &lr35902_cpu::setHL,    &lr35902_cpu::set,
	/* D */    &lr35902_cpu::set,      &lr35902_cpu::set,      &lr35902_cpu::set,       &lr35902_cpu::set,       &lr35902_cpu::set,        &lr35902_cpu::set,      &lr35902_cpu::setHL,    &lr35902_cpu::set,      &lr35902_cpu::set,        &lr35902_cpu::set,      &lr35902_cpu::set,        &lr35902_cpu::set,      &lr35902_cpu::set,       &lr35902_cpu::set,       &lr35902_cpu::setHL,    &lr35902_cpu::set,
	/* E */    &lr35902_cpu::set,      &lr35902_cpu::set,      &lr35902_cpu::set,       &lr35902_cpu::set,       &lr35902_cpu::set,        &lr35902_cpu::set,      &lr35902_cpu::setHL,    &lr35902_cpu::set,      &lr35902_cpu::set,        &lr35902_cpu::set,      &lr35902_cpu::set,        &lr35902_cpu::set,      &lr35902_cpu::set,       &lr35902_cpu::set,       &lr35902_cpu::setHL,    &lr35902_cpu::set,
	/* F */    &lr35902_cpu::set,      &lr35902_cpu::set,      &lr35902_cpu::set,       &lr35902_cpu::set,       &lr35902_cpu::set,        &lr35902_cpu::set,      &lr35902_cpu::setHL,    &lr35902_cpu::set,      &lr35902_cpu::set,        &lr35902_cpu::set,      &lr35902_cpu::set,        &lr35902_cpu::set,      &lr35902_cpu::set,       &lr35902_cpu::set,       &lr35902_cpu::setHL,    &lr35902_cpu::set
};

lr35902_cpu::lr35902_cpu(Memory& memory) : m_memory(memory)
{
	if (memory.isBootROMEnabled())
		m_registers.PC = 0x0000;
	else
		m_registers.PC = 0x0100;

	m_registers.SP.w = 0xFFFE;
	m_registers.A = 0x01;
	m_registers.BC.w = 0x0013;
	m_registers.DE.w = 0x00D8;
	m_registers.HL.w = 0x014D;
	m_registers.ZF = true;
	m_registers.CY = true;
	m_registers.H = true;
	m_registers.N = false;
	m_registers.divider = 0;
}

void lr35902_cpu::run(uint32_t cycles)
{
	for (uint32_t executed = 0; executed < cycles; executed += update());
}

uint8_t lr35902_cpu::update()
{
	uint8_t executedCycles = 4;

	if (!m_halted && !m_stoped)
	{
		if (!m_maskedInterrupts)
		{
			uint8_t ircycles = handleInterrupts();
			if (ircycles > 0)
			{
				m_halted = m_stoped = false;
				return ircycles;
			}
		}

		fetchIR();

		executedCycles = (*this.*handlers[m_registers.IR])(m_registers.IR);
		if (m_maskInterrupts) {
			m_maskInterrupts = false;
			m_maskedInterrupts = true;
		} else if (m_unMaskInterrupts) {
			m_unMaskInterrupts = false;
			m_maskedInterrupts = false;
		}
	}
	else if ((m_memory.read(IF_OFT) & m_memory.read(IE_OFT)) != 0)
	{
		m_halted = m_stoped = false;
	}

	processTimer(executedCycles);

	return executedCycles;
}

uint8_t lr35902_cpu::fetchIR()
{
	return m_registers.IR = read(m_registers.PC++);
}

uint8_t lr35902_cpu::handleInterrupts()
{
	uint8_t requestedInterrupts = m_memory.read(IE_OFT) & m_memory.read(IF_OFT);
	static const uint16_t itVector[] = {0x0040, 0x0048, 0x0050, 0x0058, 0x0060};

	if (requestedInterrupts == 0)
		return 0;

	for (uint8_t mask = 1, i = 0; i < 5; ++i, mask <<= 1)
	{
		if (requestedInterrupts & mask)
		{
			write(IF_OFT, read(IF_OFT) & ~mask);

			// Stack PC
			write(--m_registers.SP.w, static_cast<uint8_t>(m_registers.PC >> 8));
			write(--m_registers.SP.w, static_cast<uint8_t>(m_registers.PC & 0xFF));

			// Jump to itVector[i]
			m_registers.PC = itVector[i];

			// Mask interrupts
			m_maskedInterrupts = true;

			// Return cycles for stacking + jump
			return 16;
		}
	}
	return 0;
}

void lr35902_cpu::processTimer(uint8_t executedCycles)
{

	m_divRegisterCounter += executedCycles;
	if (m_divRegisterCounter >= CPU_FREQ / 16384)
	{
		++m_registers.divider;
		m_divRegisterCounter %= (CPU_FREQ / 16384);
	}

	if (!m_timerEnabled)
		return;

	m_clkCounter += executedCycles;
	if (m_clkCounter >= m_timerDivider)
	{
		uint8_t tima = read(TIMA_OFT);
		if (tima == 0xFF)
		{
			tima = read(TMA_OFT);
			m_memory.requestInterrupt(InteruptType::TIMER);
		}
		else
			++tima;

		m_clkCounter %= m_timerDivider;
		write(TIMA_OFT, tima);
	}
}

void lr35902_cpu::write(uint16_t address, uint8_t data)
{
	switch(address)
	{
		case TAC_OFT:
		{
			uint8_t mode = data & 0x3;

			m_timerEnabled = (data & 0x4) != 0;
			switch (mode)
			{
				case 0: m_timerDivider = CPU_FREQ / 4096; break;
				case 1: m_timerDivider = CPU_FREQ / 262144; break;
				case 2: m_timerDivider = CPU_FREQ / 65536; break;
				case 3: m_timerDivider = CPU_FREQ / 16384; break;
			}
			m_memory.write(address, data);
			break;
		}
		case DIV_OFT:
			m_registers.divider = 0;
			break;
		default:
			m_memory.write(address, data);
	}
}

uint8_t lr35902_cpu::read(uint16_t address)
{
	if (address == DIV_OFT)
		return m_registers.divider;

	return m_memory.read(address);
}

uint8_t lr35902_cpu::getValForReg3b(uint8_t regnum)
{
	switch(regnum) {
		case 0: return m_registers.BC.b[1]; //B
		case 1: return m_registers.BC.b[0]; //C
		case 2: return m_registers.DE.b[1]; //D
		case 3: return m_registers.DE.b[0]; //E
		case 4: return m_registers.HL.b[1]; //H
		case 5: return m_registers.HL.b[0]; //L
		case 6: return read(m_registers.HL.w); //(HL)
		case 7: return m_registers.A; //A
	}
	return 0;
}

uint16_t lr35902_cpu::getValForReg2b(uint8_t regnum)
{
	switch(regnum) {
		case 0: return m_registers.BC.w;
		case 1: return m_registers.DE.w;
		case 2: return m_registers.HL.w;
		case 3: return (m_registers.A << 8) | getFlags8b(); //AF
	}

	return 0;
}

uint8_t lr35902_cpu::getFlags8b()
{
	uint8_t flags = 0;
	if (m_registers.ZF)
		flags |= 0x80;
	if (m_registers.N)
		flags |= 0x40;
	if (m_registers.H)
		flags |= 0x20;
	if (m_registers.CY)
		flags |= 0x10;
	return flags;
}

uint8_t lr35902_cpu::nop(uint8_t)
{
	return 4;
}
uint8_t lr35902_cpu::stop(uint8_t)
{
	m_stoped = true;
	return 4;
}
uint8_t lr35902_cpu::jr_r8(uint8_t opcode)
{
	int8_t r8 = static_cast<int8_t>(fetchIR());
	switch(opcode) {
		case 0x18: //JR r8
			m_registers.PC += r8;
			break;
		case 0x20: //JR NZ,r8
			if(!m_registers.ZF)
				m_registers.PC += r8;
			break;
		case 0x30: //JR NC,r8
			if(!m_registers.CY)
				m_registers.PC += r8;
			break;
		case 0x28: //JR Z,r8
			if(m_registers.ZF)
				m_registers.PC += r8;
			break;
		case 0x38: //JR C,r8
			if(m_registers.CY)
				m_registers.PC += r8;
			break;
		SWITCHDEFAULTERR("jr_r8",opcode);
	}
	return 8;
}

uint8_t lr35902_cpu::ld_d16(uint8_t opcode)
{
	uint16_t d16 = fetchIR();
	d16 |= fetchIR() << 8;
	switch(opcode) {
		case 0x01: //LD BC,d16
			m_registers.BC.w = d16;
			break;
		case 0x11: //LD DE,d16
			m_registers.DE.w = d16;
			break;
		case 0x21: //LD HL,d16
			m_registers.HL.w = d16;
			break;
		case 0x31: //LD SP,d16
			m_registers.SP.w = d16;
			break;
		SWITCHDEFAULTERR("ld_d16",opcode);
	}
	return 16;
}

uint8_t lr35902_cpu::ld_a(uint8_t opcode)
{
	switch(opcode) {
		case 0x02: //LD (BC),A
			write(m_registers.BC.w, m_registers.A);
			break;
		case 0x12: //LD (DE),A
			write(m_registers.DE.w, m_registers.A);
			break;
		case 0x22: //LD (HL+),A
			write(m_registers.HL.w++, m_registers.A);
			break;
		case 0x32: //LD (HL-),A
			write(m_registers.HL.w--, m_registers.A);
			break;
		case 0x0A: //LD A,(BC)
			m_registers.A = read(m_registers.BC.w);
			break;
		case 0x1A: //LD A,(DE)
			m_registers.A = read(m_registers.DE.w);
			break;
		case 0x2A: //LD A,(HL+)
			m_registers.A = read(m_registers.HL.w++);
			break;
		case 0x3A: //LD A,(HL-)
			m_registers.A = read(m_registers.HL.w--);
			break;
		case 0xE2: //LD (C),A
			write(0xFF00 | m_registers.BC.b[0], m_registers.A);
			break;
		case 0xF2: //LD A, (C)
			m_registers.A = read(0xFF00 | m_registers.BC.b[0]);
			break;
		SWITCHDEFAULTERR("ld_a",opcode);
	}
	return 8;
}

uint8_t lr35902_cpu::inc_16(uint8_t opcode)
{
	switch(opcode) {
		case 0x03: //INC BC
			m_registers.BC.w++;
			break;
		case 0x13: //INC DE
			m_registers.DE.w++;
			break;
		case 0x23: //INC HL
			m_registers.HL.w++;
			break;
		case 0x33: //INC SP
			m_registers.SP.w++;
			break;
		SWITCHDEFAULTERR("inc_16",opcode);
	}
	return 8;
}

uint8_t lr35902_cpu::dec_16(uint8_t opcode)
{
	switch(opcode) {
		case 0x0B: //DEC BC
			m_registers.BC.w--;
			break;
		case 0x1B: //DEC DE
			m_registers.DE.w--;
			break;
		case 0x2B: //DEC HL
			m_registers.HL.w--;
			break;
		case 0x3B: //DEC SP
			m_registers.SP.w--;
			break;
		SWITCHDEFAULTERR("dec_16",opcode);
	}
	return 8;
}

uint8_t lr35902_cpu::inc_8(uint8_t opcode)
{
	switch(opcode) {
		case 0x04: //INC B
			m_registers.BC.b[1]++;
			m_registers.ZF = (m_registers.BC.b[1] == 0);
			m_registers.H = ((m_registers.BC.b[1]&0x0F) == 0x00);
			break;
		case 0x14: //INC D
			m_registers.DE.b[1]++;
			m_registers.ZF = (m_registers.DE.b[1] == 0);
			m_registers.H = ((m_registers.DE.b[1]&0x0F) == 0x00);
			break;
		case 0x24: //INC H
			m_registers.HL.b[1]++;
			m_registers.ZF = (m_registers.HL.b[1] == 0);
			m_registers.H = ((m_registers.HL.b[1]&0x0F) == 0x00);
			break;
		case 0x0C: //INC C
			m_registers.BC.b[0]++;
			m_registers.ZF = (m_registers.BC.b[0] == 0);
			m_registers.H = ((m_registers.BC.b[0]&0x0F) == 0x00);
			break;
		case 0x1C: //INC E
			m_registers.DE.b[0]++;
			m_registers.ZF = (m_registers.DE.b[0] == 0);
			m_registers.H = ((m_registers.DE.b[0]&0x0F) == 0x00);
			break;
		case 0x2C: //INC L
			m_registers.HL.b[0]++;
			m_registers.ZF = (m_registers.HL.b[0] == 0);
			m_registers.H = ((m_registers.HL.b[0]&0x0F) == 0x00);
			break;
		case 0x3C: //INC A
			m_registers.A++;
			m_registers.ZF = (m_registers.A == 0);
			m_registers.H = ((m_registers.A&0x0F) == 0x00);
			break;
		SWITCHDEFAULTERR("inc_8",opcode);
	}
	m_registers.N = false;
	return 4;
}

uint8_t lr35902_cpu::inc_hl(uint8_t)
{
	uint8_t val = read(m_registers.HL.w);
	uint8_t res = val + 1;
	write(m_registers.HL.w, res);

	m_registers.ZF = (res == 0);
	m_registers.H = (((val & 0x0F) + 1) & 0xF0)!= 0;
	m_registers.N = false;

	return 12;
}


uint8_t lr35902_cpu::dec_8(uint8_t opcode)
{
	switch(opcode) {
		case 0x05: //DEC B
			m_registers.BC.b[1]--;
			m_registers.ZF = (m_registers.BC.b[1] == 0);
			m_registers.H = ((m_registers.BC.b[1]&0x0F) == 0x0F);
			break;
		case 0x15: //DEC D
			m_registers.DE.b[1]--;
			m_registers.ZF = (m_registers.DE.b[1] == 0);
			m_registers.H = ((m_registers.DE.b[1]&0x0F) == 0x0F);
			break;
		case 0x25: //DEC H
			m_registers.HL.b[1]--;
			m_registers.ZF = (m_registers.HL.b[1] == 0);
			m_registers.H = ((m_registers.HL.b[1]&0x0F) == 0x0F);
			break;
		case 0x0D: //DEC C
			m_registers.BC.b[0]--;
			m_registers.ZF = (m_registers.BC.b[0] == 0);
			m_registers.H = ((m_registers.BC.b[0]&0x0F) == 0x0F);
			break;
		case 0x1D: //DEC E
			m_registers.DE.b[0]--;
			m_registers.ZF = (m_registers.DE.b[0] == 0);
			m_registers.H = ((m_registers.DE.b[0]&0x0F) == 0x0F);
			break;
		case 0x2D: //DEC L
			m_registers.HL.b[0]--;
			m_registers.ZF = (m_registers.HL.b[0] == 0);
			m_registers.H = ((m_registers.HL.b[0]&0x0F) == 0x0F);
			break;
		case 0x3D: //DEC A
			m_registers.A--;
			m_registers.ZF = (m_registers.A == 0);
			m_registers.H = ((m_registers.A&0x0F) == 0x0F);
			break;
		SWITCHDEFAULTERR("dec_8",opcode);
	}
	m_registers.N = true;
	return 4;
}

uint8_t lr35902_cpu::dec_hl(uint8_t)
{
	uint8_t val = read(m_registers.HL.w);
	uint8_t res = val - 1;
	write(m_registers.HL.w, res);

	m_registers.ZF = (res == 0);
	m_registers.H = ((res & 0x0F) == 0x0F);
	m_registers.N = true;

	return 12;
}

uint8_t lr35902_cpu::ld_d8(uint8_t opcode)
{
	uint8_t d8 = fetchIR();
	switch(opcode) {
		case 0x06: //LD B,d8
			m_registers.BC.b[1] = d8;
			break;
		case 0x16: //LD D,d8
			m_registers.DE.b[1] = d8;
			break;
		case 0x26: //LD H,d8
			m_registers.HL.b[1] = d8;
			break;
		case 0x0E: //LD C,d8
			m_registers.BC.b[0] = d8;
			break;
		case 0x1E: //LD E,d8
			m_registers.DE.b[0] = d8;
			break;
		case 0x2E: //LD L,d8
			m_registers.HL.b[0] = d8;
			break;
		case 0x3E: //LD A,d8
			m_registers.A = d8;
			break;
		case 0x36: //LD (HL),d8
			write(m_registers.HL.w, d8);
			return 12;
		SWITCHDEFAULTERR("ld_d8",opcode);
	}
	return 8;
}

uint8_t lr35902_cpu::rlca(uint8_t)
{
	if (m_registers.A & 0x80) {
		m_registers.CY = true;
		m_registers.A = (m_registers.A << 1) | 0x01;
	} else {
		m_registers.CY = false;
		m_registers.A = m_registers.A << 1;
	}
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = false;
	return 4;
}

uint8_t lr35902_cpu::rla(uint8_t)
{
	uint8_t orByte = (m_registers.CY) ? 0x01 : 0x00;
	m_registers.CY = (m_registers.A & 0x80) != 0;
	m_registers.A = (m_registers.A << 1) | orByte;
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = false;
	return 4;
}

uint8_t lr35902_cpu::rrca(uint8_t)
{
	if (m_registers.A & 0x01) {
		m_registers.CY = true;
		m_registers.A = (m_registers.A >> 1) | 0x80;
	} else {
		m_registers.CY = false;
		m_registers.A = m_registers.A >> 1;
	}
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = false;
	return 4;
}

uint8_t lr35902_cpu::rra(uint8_t)
{
	uint8_t orbyte = (m_registers.CY) ? 0x80 : 0x00;
	m_registers.CY = (m_registers.A & 0x01);
	m_registers.A = (m_registers.A >> 1) | orbyte;
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = false;
	return 4;
}

uint8_t lr35902_cpu::daa(uint8_t)
{
	uint16_t correction = (m_registers.CY) ? 0x60 : 0x0;
	int16_t res;

	if (m_registers.H)
		correction |= 0x06;

	if (m_registers.N)
		res = m_registers.A - correction;
	else
	{
		if (m_registers.A > 0x99)
			correction |= 0x60;
		if ((m_registers.A & 0x0F) > 0x09)
			correction |= 0x06;

		res = m_registers.A + correction;
	}

	m_registers.CY = (((correction << 2) & 0x100) != 0);
	m_registers.H = false;

	m_registers.A = static_cast<uint8_t>(res);

	m_registers.ZF = (m_registers.A == 0);

	return 4;
}

uint8_t lr35902_cpu::scf(uint8_t)
{
	m_registers.N = false;
	m_registers.H = false;
	m_registers.CY = true;
	return 4;
}

uint8_t lr35902_cpu::cpl(uint8_t)
{
	m_registers.A = ~m_registers.A;
	m_registers.N = true;
	m_registers.H = true;
	return 4;
}

uint8_t lr35902_cpu::ccf(uint8_t)
{
	m_registers.N = false;
	m_registers.H = false;
	m_registers.CY = !m_registers.CY;
	return 4;
}

uint8_t lr35902_cpu::ld_a16_sp(uint8_t)
{
	uint16_t a16 = fetchIR();
	a16 |= fetchIR() << 8;
	write(a16++, m_registers.SP.b[0]);
	write(a16, m_registers.SP.b[1]);
	return 20;
}

uint8_t lr35902_cpu::add_hl(uint8_t opcode)
{
	uint16_t sum;
	uint16_t HL = m_registers.HL.w;
	switch(opcode) {
		case 0x09: //ADD HL,BC
			sum = m_registers.BC.w;
			break;
		case 0x19: //ADD HL,DE
			sum = m_registers.DE.w;
			break;
		case 0x29: //ADD HL,HL
			sum = m_registers.HL.w;
			break;
		case 0x39: //ADD HL,SP
			sum = m_registers.SP.w;
			break;
		SWITCHDEFAULTERR("add_hl",opcode);
			sum = 0;
	}
	m_registers.HL.w += sum;
	m_registers.CY = (m_registers.HL.w < HL);
	m_registers.H = ((HL^sum^m_registers.HL.w) & 0x1000) != 0;
	m_registers.N = false;
	return 8;
}

uint8_t lr35902_cpu::ld_reg8(uint8_t opcode)
{
	uint8_t r1 = (opcode >> 3) & 0x07;
	uint8_t r2 = opcode & 0x07;
	uint8_t r2val;
	uint8_t clkCycles = (r2 != 6) ? 4 : 8;

	r2val = getValForReg3b(r2);


	switch(r1) {
		case 0: m_registers.BC.b[1] = r2val; break; //B
		case 1: m_registers.BC.b[0] = r2val; break; //C
		case 2: m_registers.DE.b[1] = r2val; break; //D
		case 3: m_registers.DE.b[0] = r2val; break; //E
		case 4: m_registers.HL.b[1] = r2val; break; //H
		case 5: m_registers.HL.b[0] = r2val; break; //L
		case 6: write(m_registers.HL.w, r2val); //(HL)
				clkCycles += 4;
				break;
		case 7: m_registers.A = r2val; break; //A
	}
	return clkCycles;
}

uint8_t lr35902_cpu::halt(uint8_t)
{
	//FIXME Decrease PC and execute again for each pending interrup before suspending...
	m_halted = true;
	return 4;
}

uint8_t lr35902_cpu::add_a(uint8_t opcode)
{
	uint8_t A = m_registers.A;
	uint8_t r = opcode & 0x07;
	uint8_t rval;

	if (opcode == 0xC6)
		rval = fetchIR();
	else
		rval = getValForReg3b(r);

	m_registers.A += rval;

	m_registers.ZF = (m_registers.A == 0);
	m_registers.N = false;
	m_registers.H = ((A^rval^m_registers.A) & 0x10) != 0;
	m_registers.CY = (m_registers.A < A);

	return (opcode != 0xC6 && r != 6) ? 4 : 8;
}

uint8_t lr35902_cpu::sub(uint8_t opcode)
{
	uint8_t A = m_registers.A;
	uint8_t r = opcode & 0x07;
	uint8_t rval;

	if (opcode == 0xD6)
		rval = fetchIR();
	else
		rval = getValForReg3b(r);

	m_registers.A -= rval;

	m_registers.ZF = (m_registers.A == 0);
	m_registers.N = true;
	m_registers.H = ((A^rval^m_registers.A) & 0x10) != 0;
	m_registers.CY = (m_registers.A > A);

	return (opcode != 0xD6 && r != 6) ? 4 : 8;
}

uint8_t lr35902_cpu::adc(uint8_t opcode)
{
	uint16_t res;
	uint8_t r = opcode & 0x07;
	uint8_t rval;
	uint8_t carry = (m_registers.CY) ? 1 : 0;

	if(opcode == 0xCE) {
		rval = fetchIR();
	} else {
		rval = getValForReg3b(r);
	}

	res = m_registers.A + rval + carry;

	m_registers.ZF = (static_cast<uint8_t>(res) == 0);
	m_registers.N = false;
	m_registers.H = ((m_registers.A & 0xF) + (rval & 0xF) + carry) > 0x0F;
	m_registers.CY = (res > 0xFF);

	m_registers.A = static_cast<uint8_t>(res);

	return (opcode != 0xCE && opcode != 0x8E) ? 4 : 8;
}

uint8_t lr35902_cpu::sbc(uint8_t opcode)
{
	int16_t res;
	uint8_t r = opcode & 0x07;
	uint8_t rval;
	uint8_t carry = (m_registers.CY) ? 1 : 0;

	if(opcode == 0xDE) {
		rval = fetchIR();
	} else {
		rval = getValForReg3b(r);
	}

	res = m_registers.A - rval - carry;

	m_registers.ZF = (static_cast<uint8_t>(res) == 0);
	m_registers.N = true;
	m_registers.H = ((m_registers.A & 0xF) - (rval & 0xF) - carry) < 0;
	m_registers.CY = (res < 0);

	m_registers.A = static_cast<uint8_t>(res);

	return (opcode != 0xDE && opcode != 0x9E) ? 4 : 8;
}

uint8_t lr35902_cpu::and_a(uint8_t opcode)
{
	uint8_t r = opcode & 0x7;
	uint8_t rval;

	if (opcode == 0xE6)
		rval = fetchIR();
	else
		rval = getValForReg3b(r);

	m_registers.A &= rval;

	m_registers.ZF = (m_registers.A == 0);
	m_registers.N = false;
	m_registers.H = true;
	m_registers.CY = false;

	return (opcode != 0xE6 && r == 6) ? 8 : 4;
}

uint8_t lr35902_cpu::xor_a(uint8_t opcode)
{
	uint8_t r = opcode & 0x7;
	uint8_t rval;

	if (opcode == 0xEE)
		rval = fetchIR();
	else
		rval = getValForReg3b(r);

	m_registers.A ^= rval;

	m_registers.ZF = (m_registers.A == 0);
	m_registers.N = false;
	m_registers.H = false;
	m_registers.CY = false;

	return (opcode != 0xEE && r != 6) ? 4 : 8;
}

uint8_t lr35902_cpu::or_a(uint8_t opcode)
{
	uint8_t r = opcode & 0x7;
	uint8_t rval;

	if (opcode == 0xF6)
		rval = fetchIR();
	else
		rval = getValForReg3b(r);

	m_registers.A |= rval;

	m_registers.ZF = (m_registers.A == 0);
	m_registers.N = false;
	m_registers.H = false;
	m_registers.CY = false;

	return (opcode != 0xF6 && r != 6) ? 4 : 8;
}

uint8_t lr35902_cpu::cp(uint8_t opcode)
{
	uint8_t A = m_registers.A;
	uint8_t r = opcode & 0x07;
	uint8_t rval;

	if(opcode == 0xFE) {
		rval = fetchIR();
	} else {
		rval = getValForReg3b(r);
	}

	A -= rval;

	m_registers.ZF = (A == 0);
	m_registers.N = true;
	m_registers.H = ((A^rval^m_registers.A) & 0x10) != 0;
	m_registers.CY = (m_registers.A < A);

	return (opcode != 0xFE && r != 6) ? 4 : 8;
}

uint8_t lr35902_cpu::pop(uint8_t opcode)
{

	uint8_t r = (opcode >> 4)&0x03;

	switch(r) {
		case 0:
			m_registers.BC.b[0] = read(m_registers.SP.w++);
			m_registers.BC.b[1] = read(m_registers.SP.w++);
			break;
		case 1:
			m_registers.DE.b[0] = read(m_registers.SP.w++);
			m_registers.DE.b[1] = read(m_registers.SP.w++);
			break;
		case 2:
			m_registers.HL.b[0] = read(m_registers.SP.w++);
			m_registers.HL.b[1] = read(m_registers.SP.w++);
			break;
		case 3:
			uint8_t flags = read(m_registers.SP.w++);
			m_registers.ZF = (flags & 0x80) != 0;
			m_registers.N = (flags & 0x40) != 0;
			m_registers.H = (flags & 0x20) != 0;
			m_registers.CY = (flags & 0x10) != 0;
			m_registers.A = read(m_registers.SP.w++);

			break;
	}

	return 12;
}

uint8_t lr35902_cpu::push(uint8_t opcode)
{
	uint8_t r = (opcode >> 4)&0x03;
	uint16_t val = getValForReg2b(r);

	write(--m_registers.SP.w, static_cast<uint8_t>(val >> 8));
	write(--m_registers.SP.w, static_cast<uint8_t>(val & 0xFF));

	return 12;
}

uint8_t lr35902_cpu::ret_cc(uint8_t opcode)
{
	bool retOK = false;
	if (opcode == 0xC0)
		retOK = !m_registers.ZF;
	else if (opcode == 0xC8)
		retOK = m_registers.ZF;
	else if (opcode == 0xD0)
		retOK = !m_registers.CY;
	else if (opcode == 0xD8)
		retOK = m_registers.CY;

	if (retOK) {
		m_registers.PC = read(m_registers.SP.w++);
		m_registers.PC |= read(m_registers.SP.w++) << 8;
		return 20;
	}
	else
		return 8;
}

uint8_t lr35902_cpu::invalid(uint8_t opcode)
{
	m_halted = true;
	OPCODEERROR("invalid", opcode);
	return 4;
}

uint8_t lr35902_cpu::ldh(uint8_t opcode)
{
	uint16_t val = 0xFF00 + fetchIR();
	if(opcode == 0xE0) { //LDH (a8), A
		write(val, m_registers.A);
	} else { //LDH A, (a8)
		m_registers.A = read(val);
	}

	return 12;
}

uint8_t lr35902_cpu::jp_cc_a16(uint8_t opcode)
{
	uint16_t a16 = fetchIR();
	a16 |= fetchIR() << 8;

	bool jpOk = false;

	if (opcode == 0xC2)
		jpOk = !m_registers.ZF;
	else if (opcode == 0xCA)
		jpOk = m_registers.ZF;
	else if (opcode == 0xD2)
		jpOk = !m_registers.CY;
	else if (opcode == 0xDA)
		jpOk = m_registers.CY;

	if (jpOk) {
		m_registers.PC = a16;
		return 16;
	} else
		return 12;

}
uint8_t lr35902_cpu::call_a16(uint8_t opcode)
{
	uint16_t a16 = fetchIR();
	a16 |= fetchIR() << 8;
	bool jpOk = false;

	if (opcode == 0xCD)
		jpOk = true;
	else if (opcode == 0xC4)
		jpOk = !m_registers.ZF;
	else if (opcode == 0xCC)
		jpOk = m_registers.ZF;
	else if (opcode == 0xD4)
		jpOk = !m_registers.CY;
	else if (opcode == 0xDC)
		jpOk = m_registers.CY;

	if (jpOk) {
		write(--m_registers.SP.w, static_cast<uint8_t>(m_registers.PC >> 8));
		write(--m_registers.SP.w, static_cast<uint8_t>(m_registers.PC & 0xFF));
		m_registers.PC = a16;
		return 24;
	} else
		return 12;
}

uint8_t lr35902_cpu::rst(uint8_t opcode)
{
	static const uint8_t address[8] {
		0x00, 0x08, 0x10, 0x18,
		0x20, 0x28, 0x30, 0x38
	};
	uint8_t p = (opcode >> 3)&0x07;

	write(--m_registers.SP.w, static_cast<uint8_t>(m_registers.PC >> 8));
	write(--m_registers.SP.w, static_cast<uint8_t>(m_registers.PC & 0xFF));
	m_registers.PC = address[p];

	return 16;
}

uint8_t lr35902_cpu::di(uint8_t)
{
	m_maskInterrupts = true;
	return 4;
}

uint8_t lr35902_cpu::ei(uint8_t)
{
	m_unMaskInterrupts = true;
	return 4;
}

uint8_t lr35902_cpu::add_sp_r8(uint8_t)
{
	int8_t offset = static_cast<int8_t>(fetchIR());
	uint16_t SP = m_registers.SP.w;
	m_registers.SP.w += offset;

	m_registers.ZF = false;
	m_registers.N = false;
	m_registers.H = (((SP^offset^m_registers.SP.w) & 0x10) == 0x10);
	m_registers.CY = (((SP^offset^m_registers.SP.w) & 0x100) == 0x100);
	return 16;
}

uint8_t lr35902_cpu::jp_hl(uint8_t)
{
	m_registers.PC = m_registers.HL.w;
	return 4;
}

uint8_t lr35902_cpu::ldhl_spr8(uint8_t)
{
	int8_t r8 = static_cast<int8_t>(fetchIR());
	m_registers.HL.w = m_registers.SP.w + r8;

	m_registers.ZF = false;
	m_registers.N = false;
	m_registers.H = (((m_registers.SP.w^r8^m_registers.HL.w) & 0x10) == 0x10);
	m_registers.CY = (((m_registers.SP.w^r8^m_registers.HL.w) & 0x100) == 0x100);

	return 12;
}

uint8_t lr35902_cpu::ld_sp_hl(uint8_t)
{
	m_registers.SP.w = m_registers.HL.w;
	return 8;
}

uint8_t lr35902_cpu::ld_a_a16(uint8_t opcode)
{
	uint16_t a16 = fetchIR();
	a16 |= fetchIR() << 8;

	if (opcode == 0xEA)
		write(a16, m_registers.A);
	else
		m_registers.A = read(a16);
	return 16;
}

uint8_t lr35902_cpu::jp_a16(uint8_t)
{
	uint16_t a16 = fetchIR();
	a16 |= fetchIR() << 8;
	m_registers.PC = a16;

	return 16;
}

uint8_t lr35902_cpu::ret(uint8_t)
{
	uint16_t PC = read(m_registers.SP.w++);
	PC |= read(m_registers.SP.w++) << 8;
	m_registers.PC = PC;
	return 16;
}

uint8_t lr35902_cpu::reti(uint8_t)
{
	ret(0);
	m_maskedInterrupts = false;
	return 16;
}

/***********************************************
 ***         START OF CB INSTRUCTIONS       ****
 ***********************************************/
uint8_t lr35902_cpu::cb(uint8_t)
{
	uint8_t cbinst = fetchIR();
	return (*this.*cb_handlers[cbinst])(cbinst);
}

uint8_t lr35902_cpu::rlc(uint8_t opcode)
{
	uint8_t reg = opcode & 0x7;
	uint8_t data = m_8b_reg_map[reg];

	m_8b_reg_map[reg].get() = (data << 1) | ((data & 0x80) ? 1 : 0);

	m_registers.CY = (data & 0x80) != 0;
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = (m_8b_reg_map[reg] == 0);

	return 8;
}

uint8_t lr35902_cpu::rlcHL(uint8_t)
{
	uint8_t shifted;
	uint8_t data = read(m_registers.HL.w);

	shifted = (data << 1) | ((data & 0x80) ? 1 : 0);
	write(m_registers.HL.w, shifted);

	m_registers.CY = (data & 0x80) != 0;
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = (shifted == 0);

	return 16;
}

uint8_t lr35902_cpu::rrc(uint8_t opcode)
{
	uint8_t reg = opcode & 0x7;
	uint8_t data = m_8b_reg_map[reg];

	m_8b_reg_map[reg].get() = (data >> 1) | ((data & 0x01) ? 0x80 : 0);

	m_registers.CY = (data & 0x01);
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = (m_8b_reg_map[reg] == 0);

	return 8;
}

uint8_t lr35902_cpu::rrcHL(uint8_t)
{
	uint8_t shifted;
	uint8_t data = read(m_registers.HL.w);

	shifted = (data >> 1) | ((data & 0x01) ? 0x80 : 0);
	write(m_registers.HL.w, shifted);

	m_registers.CY = (data & 0x01);
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = (shifted == 0);

	return 16;
}

uint8_t lr35902_cpu::rl(uint8_t opcode)
{
	uint8_t reg = opcode & 0x7;
	uint8_t data = m_8b_reg_map[reg];

	m_8b_reg_map[reg].get() = (data << 1) | ((m_registers.CY) ? 1 : 0);

	m_registers.CY = (data & 0x80) != 0;
	m_registers.N = false;
	m_registers.H = false;

	return 8;
}


uint8_t lr35902_cpu::rlHL(uint8_t)
{
	uint8_t shifted;
	uint8_t data = read(m_registers.HL.w);

	shifted = (data << 1) | ((m_registers.CY) ? 1 : 0);
	write(m_registers.HL.w, shifted);

	m_registers.CY = (data & 0x80) != 0;
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = (shifted == 0);

	return 16;
}

uint8_t lr35902_cpu::rr(uint8_t opcode)
{
	uint8_t reg = opcode & 0x7;
	uint8_t data = m_8b_reg_map[reg];

	m_8b_reg_map[reg].get() = (data >> 1) | ((m_registers.CY) ? 0x80 : 0);

	m_registers.CY = (data & 0x01);
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = (m_8b_reg_map[reg] == 0);

	return 8;
}

uint8_t lr35902_cpu::rrHL(uint8_t)
{
	uint8_t shifted;
	uint8_t data = read(m_registers.HL.w);

	shifted = (data >> 1) | ((m_registers.CY) ? 0x80 : 0);
	write(m_registers.HL.w, shifted);

	m_registers.CY = (data & 0x01);
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = (shifted == 0);

	return 16;
}

uint8_t lr35902_cpu::sla(uint8_t opcode)
{
	uint8_t reg = opcode & 0x7;
	uint8_t data = m_8b_reg_map[reg];

	m_8b_reg_map[reg].get() = (data << 1);

	m_registers.CY = (data & 0x80) != 0;
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = (m_8b_reg_map[reg] == 0);

	return 8;
}

uint8_t lr35902_cpu::slaHL(uint8_t)
{
	uint8_t shifted;
	uint8_t data = read(m_registers.HL.w);

	shifted = (data << 1);
	write(m_registers.HL.w, shifted);

	m_registers.CY = (data & 0x80) != 0;
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = (shifted == 0);

	return 16;
}

uint8_t lr35902_cpu::sra(uint8_t opcode)
{
	uint8_t reg = opcode & 0x7;
	uint8_t data = m_8b_reg_map[reg];

	m_8b_reg_map[reg].get() = (data >> 1) | ((data & 0x80) ? 0x80 : 0);

	m_registers.CY = (data & 0x01);
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = (m_8b_reg_map[reg] == 0);

	return 8;
}

uint8_t lr35902_cpu::sraHL(uint8_t)
{
	uint8_t shifted;
	uint8_t data = read(m_registers.HL.w);

	shifted = (data >> 1) | ((data & 0x80) ? 0x80 : 0);
	write(m_registers.HL.w, shifted);

	m_registers.CY = (data & 0x01);
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = (shifted == 0);
	return 16;
}

uint8_t lr35902_cpu::swap(uint8_t opcode)
{
	uint8_t reg = opcode & 0x7;

	m_8b_reg_map[reg].get() = (m_8b_reg_map[reg] >> 4) | (m_8b_reg_map[reg] << 4);

	m_registers.CY = false;
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = (m_8b_reg_map[reg] == 0);

	return 8;
}

uint8_t lr35902_cpu::swapHL(uint8_t)
{
	uint8_t swapped;
	uint8_t data = read(m_registers.HL.w);

	swapped = (data >> 4) | (data << 4);
	write(m_registers.HL.w, swapped);

	m_registers.CY = false;
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = (swapped == 0);

	return 16;
}

uint8_t lr35902_cpu::srl(uint8_t opcode)
{
	uint8_t reg = opcode & 0x7;
	uint8_t data = m_8b_reg_map[reg];

	m_8b_reg_map[reg].get() = (data >> 1);

	m_registers.CY = (data & 0x01);
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = (m_8b_reg_map[reg] == 0);
	return 8;
}

uint8_t lr35902_cpu::srlHL(uint8_t)
{
	uint8_t data = read(m_registers.HL.w);
	uint8_t shifted = (data >> 1);

	write(m_registers.HL.w, shifted);

	m_registers.CY = (data & 0x01);
	m_registers.N = false;
	m_registers.H = false;
	m_registers.ZF = (shifted == 0);
	return 16;
}

uint8_t lr35902_cpu::bit(uint8_t opcode)
{
	uint8_t reg = opcode & 0x7;
	uint8_t bit = (opcode>>3) & 0x7;

	m_registers.ZF = !(m_8b_reg_map[reg] & (1 << bit));
	return 8;
}


uint8_t lr35902_cpu::bitHL(uint8_t opcode)
{
	uint8_t bit = (opcode>>3) & 0x7;

	m_registers.ZF = !(read(m_registers.HL.w) & (1 << bit));
	return 16;
}

uint8_t lr35902_cpu::res(uint8_t opcode)
{
	uint8_t reg = opcode & 0x7;
	uint8_t bit = (opcode>>3) & 0x7;

	m_8b_reg_map[reg] &= ~(1 << bit);
	return 8;
}

uint8_t lr35902_cpu::resHL(uint8_t opcode)
{
	uint8_t bit = (opcode>>3) & 0x7;

	write(m_registers.HL.w, read(m_registers.HL.w) & ~(1 << bit));
	return 16;
}

uint8_t lr35902_cpu::set(uint8_t opcode)
{
	uint8_t reg = opcode & 0x7;
	uint8_t bit = (opcode>>3) & 0x7;

	m_8b_reg_map[reg] |= (1 << bit);
	return 8;
}

uint8_t lr35902_cpu::setHL(uint8_t opcode)
{
	uint8_t bit = (opcode>>3) & 0x7;

	write(m_registers.HL.w, read(m_registers.HL.w) | (1 << bit));
	return 16;
}
