/*      ____ ______ ______  ____
       /  _//\  __//\  __ \/\_, \
 ____ /\  __ \\___  \\ \/\ \//  /__ ___________________________________________
|     \ \_____\\____/ \_____\\_____\                                           |
|  MOS \/_____//___/ \/_____//_____/ CPU Emulator                              |
|  Copyright (C) 1999-2020 Manuel Sainz de Baranda y Goñi.                     |
|                                                                              |
|  This emulator is free software: you can redistribute it and/or modify it    |
|  under the terms of the GNU Lesser General Public License as published by    |
|  the Free Software Foundation, either version 3 of the License, or (at your  |
|  option) any later version.                                                  |
|                                                                              |
|  This emulator is distributed in the hope that it will be useful, but        |
|  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  |
|  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public      |
|  License for more details.                                                   |
|                                                                              |
|  You should have received a copy of the GNU Lesser General Public License    |
|  along with this emulator. If not, see <http://www.gnu.org/licenses/>.       |
|                                                                              |
'=============================================================================*/

#ifndef emulation_CPU_6502_H
#define emulation_CPU_6502_H

#ifdef M6502_CPU_DEPENDENCIES_HEADER
#	include M6502_CPU_DEPENDENCIES_HEADER
#else
#	include <Z/macros/language.h>
#	include <Z/types/integral.h>
#endif

/**
  */

typedef zuint8 (* M6502Read)(void *context, zuint16 address);

/**
  */

typedef void (* M6502Write)(void *context, zuint16 address, zuint8 value);

/** 6502 emulator instance.
  * @details This structure contains the state of the emulated CPU and callback
  * pointers necessary to interconnect the emulator with external logic. There
  * is no constructor function, so, before using an object of this type, some
  * of its members must be initialized, in particular the following:
  * @c context, @c read and @c write. */

typedef struct {

	/** @brief The number of clock cycles executed by @c m6502_run.
	  * @details @c m6502_run sets this member variable to @c 0 before
	  * starting to execute instructions and its value persists after
	  * returning. The callbacks can use this to know during what cycle
	  * they are being called. *//* OK */

	zusize cycles;

	/** @brief The value of the @c context argument passed to the callbacks.
	  * @details This member variable can be used to reference the context
	  * of the machine being emulated. *//* OK */

	void *context;

	/** @brief Callback invoked when the CPU reads from memory.
	  * @param context The value of the @c context member variable.
	  * @param address The memory address to read from.
	  * @return An 8-bit value read from memory. *//* OK */

	zuint8 (* read)(void *context, zuint16 address);

	/** @brief Callback invoked when the CPU writes to memory.
	  * @param context The value of the @c context member variable.
	  * @param address The memory address to write to.
	  * @param value The 8-bit value to write. *//* OK */

	void (* write)(void *context, zuint16 address, zuint8 value);

	zuint16 pc; /**< @brief PC register. */
	zuint8	s;  /**< @brief S register.  */
	zuint8	p;  /**< @brief P register.  */
	zuint8	a;  /**< @brief A register.  */
	zuint8	x;  /**< @brief X register.  */
	zuint8	y;  /**< @brief Y register.  */

	/** @brief State of the @c IRQ line.
	  **/
	zuint8 irq;

	/** @brief State of the @c NMI line.
	  **/

	zuint8 nmi;

	/** @brief Temporary storage for memory address resolution.
	  * @details This is an internal private variable. */

	zuint8 opcode;

	/** @brief Temporary storage for the number of cycles consumed by
	  * instructions requiring memory address resolution.
	  * @details This is an internal private variable. */

	zuint8 ea_cycles;

	/** @brief Temporary storage for the resolved memory address.
	  * @details This is an internal private variable. */

	zuint16 ea;
} M6502;

#ifndef M6502_CPU_API
#	ifdef M6502_CPU_STATIC
#		define M6502_CPU_API
#	else
#		define M6502_CPU_API Z_API_IMPORT
#	endif
#endif

Z_EXTERN_C_BEGIN

/** Changes the CPU power status.
  * @param self A pointer to a 6502 emulator instance.
  * @param state @c TRUE = power ON; @c FALSE = power OFF. */

M6502_CPU_API void m6502_power(M6502 *self, zboolean state);

/** Resets the CPU.
  * @details This is equivalent to a pulse on the RESET line of a real 6502.
  * @param self A pointer to a 6502 emulator instance. */

M6502_CPU_API void m6502_reset(M6502 *self);

/** Runs the CPU for a given number of @p cycles.
  * @param self A pointer to a 6502 emulator instance.
  * @param cycles The number of cycles to be executed.
  * @return The number of cycles executed.
  * @note Given the fact that one 6502 instruction needs between 2 and 7 cycles
  * to be executed, it's not always possible to run the CPU the exact number of
  * @p cycles specfified. */

M6502_CPU_API zusize m6502_run(M6502 *self, zusize cycles);

/** Performs a non-maskable interrupt (NMI).
  * @details This is equivalent to a pulse on the NMI line of a real 6502.
  * @param self A pointer to a 6502 emulator instance. */

M6502_CPU_API void m6502_nmi(M6502 *self);

/** Changes the state of the maskable interrupt (IRQ).
  * @details This is equivalent to a change on the IRQ line of a real 6502.
  * @param self A pointer to a 6502 emulator instance.
  * @param state @c TRUE = line high; @c FALSE = line low. */

M6502_CPU_API void m6502_irq(M6502 *self, zboolean state);

Z_EXTERN_C_END

#endif /* emulation_CPU_6502_H */
