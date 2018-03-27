/**
* @file include/retdec/bin2llvmir/optimizations/decoder/jump_targets.h
* @brief Jump targets representation.
* @copyright (c) 2017 Avast Software, licensed under the MIT license
*/

#ifndef RETDEC_BIN2LLVMIR_OPTIMIZATIONS_DECODER_JUMP_TARGETS_H
#define RETDEC_BIN2LLVMIR_OPTIMIZATIONS_DECODER_JUMP_TARGETS_H

#include <set>

#include "retdec/capstone2llvmir/capstone2llvmir.h"
#include "retdec/capstone2llvmir/x86/x86.h"
#include "retdec/utils/address.h"

namespace retdec {
namespace bin2llvmir {

/**
 * Representation of an address that will be tried to be decoded.
 */
class JumpTarget
{
	public:
		/**
		 * Jump target type and its priority.
		 * Lower number -> higher priority.
		 */
		enum class eType
		{
			// Jump targets discovered in control flow changing instructions.
			CONTROL_FLOW_BR_FALSE = 0,
			CONTROL_FLOW_BR_TRUE,
			CONTROL_FLOW_SWITCH_CASE,
			CONTROL_FLOW_CALL_TARGET,
			CONTROL_FLOW_RETURN_TARGET,
			// Jump targets from various other sources.
			ENTRY_POINT,
			LEFTOVER,
			// Default jump target.
			UNKNOWN,
		};

	public:
		JumpTarget();
		JumpTarget(
				retdec::utils::Address a,
				eType t,
				cs_mode m,
				retdec::utils::Address f);
		JumpTarget(
				retdec::utils::Address a,
				eType t,
				cs_mode m,
				llvm::Instruction* f);

		bool operator<(const JumpTarget& o) const;

		retdec::utils::Address getAddress() const;
		eType getType() const;
		llvm::Instruction* getFromInstruction() const;
		retdec::utils::Address getFromAddress() const;

	friend std::ostream& operator<<(std::ostream &out, const JumpTarget& jt);

	private:
		// This address will be tried to be decoded.
		retdec::utils::Address _address;
		// The type of jump target - determined by its source.
		eType _type = eType::UNKNOWN;
		/// Address from which this jump target was created.
		retdec::utils::Address _fromAddress;
		/// Instruction from which this jump target was created.
		/// This can be used to get from address.
		llvm::Instruction* _fromInst = nullptr;
		/// Disassembler mode that should be used for this jump target.
		cs_mode _mode = CS_MODE_BIG_ENDIAN;

		llvm::BasicBlock* _bb = nullptr;
};

/**
 * Jump target container.
 */
class JumpTargets
{
	public:
		auto begin();
		auto end();

		bool empty();
		std::size_t size() const;
		void clear();
		const JumpTarget& top();
		void pop();

		void push(
				retdec::utils::Address a,
				JumpTarget::eType t,
				cs_mode m,
				retdec::utils::Address f);
		void push(
				retdec::utils::Address a,
				JumpTarget::eType t,
				cs_mode m,
				llvm::Instruction* f);

	friend std::ostream& operator<<(std::ostream &out, const JumpTargets& jts);

	public:
		std::set<JumpTarget> _data;
};

} // namespace bin2llvmir
} // namespace retdec

#endif