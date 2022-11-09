/*
 * Copyright (c) 2021, Adam Boardman
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef PICO_MCP23017_H
#define PICO_MCP23017_H

#ifdef MOCK_PICO_PI

#include "../test/pico_pi_mocks.h"

#else

#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "pico/time.h"

#endif

//#define DEBUG_MCP23017
#ifdef  DEBUG_MCP23017
#define mcp_debug(fmt, arg...) \
	do {                       \
        printf(fmt, ##arg);    \
	} while (0)
#else
#define mcp_debug(...) do { } while (false)
#endif

/**
 * MCP23017 I/O Expander, 16bit
 *
 */
class Mcp23017 {

public:
	/**
	 * Create a MCP23017 controller on the specified i2c bus
	 *
	 * Note: The i2c bus is expected to be already initialised as it may be shared between multiple devices
	 *
	 * @param i2c selected bus
	 * @param _address address on the bus
	 */
	Mcp23017(i2c_inst_t *i2c,  uint8_t _address);

	/**
	 * Configure with a IOCON (I/O Expander configuration register)
	 *
	 * @param mirroring true if you want the INT pins to be internally connected, allows you to save IO lines needed for detecting interrupts
	 * @param polarity the polarity of the interrupt, true = active-high, false = active-low
	 * @return PICO_ERROR_NONE or PICO_ERROR_GENERIC
	 */
	int setup(bool mirroring, bool polarity) const;

	/**
	 * Gets the first pin that has changed values within the last interrupt, not 100% reliable
	 * @return pin 0-15 or PICO_ERROR_GENERIC
	 */
	int get_last_interrupt_pin() const;

	/**
	 * Gets the values of all interrupts for client code interrogation
	 * @return values or PICO_ERROR_GENERIC
	 */
	int get_interrupt_values() const;

	/**
	 * Stores and returns the last input state in the class for later interrogation with
	 * get_last_input_pin_value or get_last_input_pin_values
	 * @return the pin values or PICO_ERROR_NONE or PICO_ERROR_GENERIC
	 */
	int update_and_get_input_values();

	/**
	 * Checks the pin state from the last update_and_get_input_values
	 * @param pin the pin to query
	 * @return the pin's state
	 */
	bool get_last_input_pin_value(int pin) const;

	/**
	 * Returns all the pin states from the last update_and_get_input_values
	 * @return the pin values
	 */
	uint16_t get_last_input_pin_values() const;

	/**
	 * Gets the address we were constructed to talk to
	 * @return the address
	 */
	[[nodiscard]] int get_address() const;

	/**
	 * Sets the IO direction for each pin
	 * @param direction '1' bits input, '0' bits output
	 * @return PICO_ERROR_NONE or PICO_ERROR_GENERIC
	 */
	int set_io_direction(int direction) const;

	/**
	 * Sets the pull-up resistors for the pins (100K)
	 * @param direction '1' bits enable, '0' bits disable
	 * @return PICO_ERROR_NONE or PICO_ERROR_GENERIC
	 */
	int set_pullup(int direction) const;

	/**
	 * Sets the interrupt control register
	 * @param compare_to_reg '1' bits compare to default values, '0' bits compare to previous values
	 * @return PICO_ERROR_NONE or PICO_ERROR_GENERIC
	 */
	int set_interrupt_type(int compare_to_reg) const;

	/**
	 * Sets the interrupt enabled register
	 * @param enabled '1' bits enable, '0' bits disable
	 * @return PICO_ERROR_NONE or PICO_ERROR_GENERIC
	 */
	int enable_interrupt(int enabled) const;

	/**
	 * Sets all the output bits at once, also stores this as the internal state for later per pin manipulation with set_output_bit_for_pin
	 * @param all_bits '1' bits on, '0' bits off
	 * @return PICO_ERROR_NONE or PICO_ERROR_GENERIC
	 */
	int set_all_output_bits(int all_bits);

	/**
	 * Sets an individual pin's within the internal state, this must be flushed to take effect
	 * @param pin the pin 0-15
	 * @param set true = on, false = off
	 */
	void set_output_bit_for_pin(int pin, bool set);

	/**
	 * Check the state of an individual pin within the internal state
	 * @param pin the pin 0-15
	 * @return true = on, false = off
	 */
	bool get_output_bit_for_pin(int pin) const;

	/**
	 * Flushes the internal output state to the device
	 * @return PICO_ERROR_NONE or PICO_ERROR_GENERIC
	 */
	int flush_output() const;

private:
	int setup_bank_configuration(int reg, bool mirroring, bool polarity) const;

	int write_register(uint8_t reg, uint8_t value) const;

	int read_register(uint8_t reg) const;

	int read_dual_registers(uint8_t reg) const;

	int write_dual_registers(uint8_t reg, int value) const;

private:
	i2c_inst_t *i2c;
	const uint8_t address;
	int output{};
	int last_input{};
};

#endif // PICO_MCP23017_H
