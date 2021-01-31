/*
 * Copyright (c) 2021, Adam Boardman
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <catch2/catch_test_macros.hpp>
#include <cstdarg>
#include <vector>

#include "mcp23017.h"
#include "mcp23017_private.h"

static const int MCP_ALL_PINS_INPUT = 0xffff;
static const int MCP_ALL_PINS_OUTPUT = 0x0000;
static const int MCP_ALL_PINS_PULL_UP = 0xffff;
static const int MCP_ALL_PINS_COMPARE_TO_LAST = 0x0000;
static const int MCP_ALL_PINS_INTERRUPT_ENABLED = 0xffff;
static const int MCP_ALTERNATE_PINS_ON = 0xaaaa;

i2c_inst_t *i2c0 = nullptr;
Mcp23017 mcp(i2c0, 0x20); //NOLINT

TEST_CASE("Bit Write", "[mcp23017]") {
	int value=0;
	set_bit(value, 0, true);
	REQUIRE(value == 0b0000000000000001);
	set_bit(value, 1, true);
	REQUIRE(value == 0b0000000000000011);
	set_bit(value, 6, true);
	REQUIRE(value == 0b0000000001000011);
	set_bit(value, 7, true);
	REQUIRE(value == 0b0000000011000011);
	set_bit(value, 15, true);
	REQUIRE(value == 0b1000000011000011);
}

TEST_CASE("Bit Read", "[mcp23017]") {
	REQUIRE(is_bit_set(0b001, 0) == true);
	REQUIRE(is_bit_set(0b001, 1) == false);
	REQUIRE(is_bit_set(0b001, 2) == false);
	REQUIRE(is_bit_set(0b1000000000000, 2) == false);
	REQUIRE(is_bit_set(0b1000000000000, 12) == true);
}

TEST_CASE("Set Direction - Input", "[mcp23017]") {
	reset_for_test(i2c0);
	auto ret = mcp.set_io_direction(MCP_ALL_PINS_INPUT);
	REQUIRE(ret == PICO_ERROR_NONE);
	REQUIRE(lastAddress == 0x20);
	REQUIRE(mock_data_read ==  0);
	REQUIRE(last_length_written == 3);
	REQUIRE(mock_write_data.size() == 3);
	REQUIRE(mock_write_data[0] == MCP23017_IODIRA);
	REQUIRE(mock_write_data[1] == 0xff);
	REQUIRE(mock_write_data[2] == 0xff);
}

TEST_CASE("Set Direction - Output", "[mcp23017]") {
	reset_for_test(i2c0);
	auto ret = mcp.set_io_direction(MCP_ALL_PINS_OUTPUT);
	REQUIRE(ret == PICO_ERROR_NONE);
	REQUIRE(lastAddress == 0x20);
	REQUIRE(mock_data_read ==  0);
	REQUIRE(last_length_written == 3);
	REQUIRE(mock_write_data.size() == 3);
	REQUIRE(mock_write_data[0] == MCP23017_IODIRA);
	REQUIRE(mock_write_data[1] == 0x00);
	REQUIRE(mock_write_data[2] == 0x00);
}

TEST_CASE("Set Pull Up - On", "[mcp23017]") {
	reset_for_test(i2c0);
	auto ret = mcp.set_pullup(MCP_ALL_PINS_PULL_UP);
	REQUIRE(ret == PICO_ERROR_NONE);
	REQUIRE(lastAddress == 0x20);
	REQUIRE(mock_data_read ==  0);
	REQUIRE(last_length_written == 3);
	REQUIRE(mock_write_data.size() == 3);
	REQUIRE(mock_write_data[0] == MCP23017_GPPUA);
	REQUIRE(mock_write_data[1] == 0xff);
	REQUIRE(mock_write_data[2] == 0xff);
}

TEST_CASE("Set Interrupt Compare Type - Change From Last", "[mcp23017]") {
	reset_for_test(i2c0);
	auto ret = mcp.set_interrupt_type(MCP_ALL_PINS_COMPARE_TO_LAST);
	REQUIRE(ret == PICO_ERROR_NONE);
	REQUIRE(lastAddress == 0x20);
	REQUIRE(mock_data_read ==  0);
	REQUIRE(last_length_written == 3);
	REQUIRE(mock_write_data.size() == 3);
	REQUIRE(mock_write_data[0] == MCP23017_INTCONA);
	REQUIRE(mock_write_data[1] == 0x00);
	REQUIRE(mock_write_data[2] == 0x00);
}

TEST_CASE("Enable Interrupt", "[mcp23017]") {
	reset_for_test(i2c0);
	auto ret = mcp.enable_interrupt(MCP_ALL_PINS_INTERRUPT_ENABLED);
	REQUIRE(ret == PICO_ERROR_NONE);
	REQUIRE(lastAddress == 0x20);
	REQUIRE(mock_data_read ==  0);
	REQUIRE(last_length_written == 3);
	REQUIRE(mock_write_data.size() == 3);
	REQUIRE(mock_write_data[0] == MCP23017_GPINTENA);
	REQUIRE(mock_write_data[1] == 0xff);
	REQUIRE(mock_write_data[2] == 0xff);
}

TEST_CASE("Get Input Pin Values GPIOA", "[mcp23017]") {
	reset_for_test(i2c0);
	std::vector<uint8_t> data = {0b00000010, 0b00000000};
	set_read_data(data, 1);
	auto ret = mcp.update_input_values();
	REQUIRE(ret == PICO_ERROR_NONE);
	REQUIRE(lastAddress == 0x20);
	REQUIRE(mock_data_read ==  2);
	REQUIRE(last_length_written == 1);
	REQUIRE(mock_write_data.size() == 1);
	REQUIRE(mock_write_data[0] == MCP23017_GPIOA);
	REQUIRE(mcp.get_input_pin_value(1) == true);
	REQUIRE(mcp.get_input_pin_value(0) == false);
}

TEST_CASE("Get Input Pin Values GPIOB", "[mcp23017]") {
	reset_for_test(i2c0);
	std::vector<uint8_t> data = {0b00000000, 0b0001000};
	set_read_data(data, 1);
	auto ret = mcp.update_input_values();
	REQUIRE(ret == PICO_ERROR_NONE);
	REQUIRE(lastAddress == 0x20);
	REQUIRE(mock_data_read ==  2);
	REQUIRE(last_length_written == 1);
	REQUIRE(mock_write_data.size() == 1);
	REQUIRE(mock_write_data[0] == MCP23017_GPIOA);
	REQUIRE(mcp.get_input_pin_value(11) == true);
	REQUIRE(mcp.get_input_pin_value(0) == false);
}

TEST_CASE("Set Output Pin Values", "[mcp23017]") {
	reset_for_test(i2c0);
	auto ret = mcp.set_all_output_bits(MCP_ALTERNATE_PINS_ON);
	REQUIRE(ret == PICO_ERROR_NONE);
	REQUIRE(lastAddress == 0x20);
	REQUIRE(mock_data_read ==  0);
	REQUIRE(last_length_written == 3);
	REQUIRE(mock_write_data.size() == 3);
	REQUIRE(mock_write_data[0] == MCP23017_GPIOA);
	REQUIRE(mock_write_data[1] == 0xaa);
	REQUIRE(mock_write_data[2] == 0xaa);
}

TEST_CASE("Set Output Pin 5", "[mcp23017]") {
	reset_for_test(i2c0);
	mcp.set_output_bit_for_pin(5, true);
	auto ret = mcp.flush_output();
	REQUIRE(ret == PICO_ERROR_NONE);
	REQUIRE(lastAddress == 0x20);
	REQUIRE(mock_data_read ==  0);
	REQUIRE(last_length_written == 3);
	REQUIRE(mock_write_data.size() == 3);
	REQUIRE(mock_write_data[0] == MCP23017_GPIOA);
	REQUIRE(mock_write_data[1] == 0b100000);
	REQUIRE(mock_write_data[2] == 0);
}

TEST_CASE("Setup", "[mcp23017]") {
	reset_for_test(i2c0);

	int ret = mcp.setup(true, false);

	REQUIRE(ret == PICO_ERROR_NONE);
	REQUIRE(last_length_read ==  0);
	REQUIRE(last_length_written ==  2);
	REQUIRE(mock_data_read ==  0);
	REQUIRE(mock_write_data.size() ==  4);
	REQUIRE((int)mock_write_data[0] == MCP23017_IOCONA); //Write
	REQUIRE((int)mock_write_data[1] == 64); //Value written MCP23017_IOCON_MIRROR_BIT
	REQUIRE((int)mock_write_data[2] == MCP23017_IOCONB); //Write
	REQUIRE((int)mock_write_data[3] == 64); //Value written MCP23017_IOCON_MIRROR_BIT
}

TEST_CASE("Get Last Interrupt Pin None", "[mcp23017]") {
	reset_for_test(i2c0);
	std::vector<uint8_t> data = {0b000, 0b00};
	set_read_data(data, 1);

	int pin = mcp.get_last_interrupt_pin();

	REQUIRE(pin == PICO_ERROR_GENERIC);
	REQUIRE(lastAddress ==  0x20);
	REQUIRE(mock_data_read ==  2);
	REQUIRE(last_length_read ==  2);
	REQUIRE(last_length_written ==  1);
	REQUIRE(mock_write_data.size() == 1);
	REQUIRE(mock_write_data[0] == MCP23017_INTFA); //Read
}

TEST_CASE("Get Last Interrupt Pin 4(A-4)", "[mcp23017]") {
	reset_for_test(i2c0);
	std::vector<uint8_t> data = {0b00010000, 0b00000000};
	set_read_data(data, 2);

	int pin = mcp.get_last_interrupt_pin();

	REQUIRE(pin == 4);
	REQUIRE(lastAddress ==  0x20);
	REQUIRE(mock_data_read ==  2);
	REQUIRE(last_length_read ==  2);
	REQUIRE(last_length_written ==  1);
	REQUIRE(mock_write_data.size() == 1);
	REQUIRE((int)mock_write_data[0] == MCP23017_INTFA); //Read
}

TEST_CASE("Get Last Interrupt Pin 8(B-0)", "[mcp23017]") {
	reset_for_test(i2c0);
	std::vector<uint8_t> data = {0b0, 0b1};
	set_read_data(data, 2);

	int pin = mcp.get_last_interrupt_pin();

	REQUIRE(pin == 8);
	REQUIRE(lastAddress ==  0x20);
	REQUIRE(mock_data_read ==  2);
	REQUIRE(last_length_read ==  2);
	REQUIRE(last_length_written ==  1);
	REQUIRE(mock_write_data.size() ==  1);
	REQUIRE(mock_write_data[0] == MCP23017_INTFA); //Read
}

TEST_CASE("Get Last Interrupt Pin 8(B-0) Value 1", "[mcp23017]") {
	reset_for_test(i2c0);
	std::vector<uint8_t> data = {0b00000000, 0b00000001, 0b00000001, 0b00000000};
	set_read_data(data, 2);

	int pin = mcp.get_last_interrupt_pin();
	int value = mcp.get_interrupt_values();

	REQUIRE(pin == 8);
	REQUIRE(value == 1);
	REQUIRE(lastAddress ==  0x20);
	REQUIRE(mock_data_read ==  4);
	REQUIRE(last_length_read ==  2);
	REQUIRE(last_length_written ==  1);
	REQUIRE(mock_write_data.size() ==  2);
	REQUIRE(mock_write_data[0] == MCP23017_INTFA); //Read
	REQUIRE(mock_write_data[1] == MCP23017_INTCAPA); //Read
}
