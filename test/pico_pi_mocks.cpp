/*
 * Copyright (c) 2021, Adam Boardman
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <cstring>

#include "pico_pi_mocks.h"

int lastAddress;
int last_length_read;
int last_length_written;
long mock_data_read = 0;
std::vector<uint8_t> mock_write_data;
std::vector<uint8_t> mock_read_data;

void reset_for_test(const i2c_inst_t *i2c) {
	lastAddress = 0;
	last_length_read = 0;
	last_length_written = 0;
	mock_data_read = 0;
	mock_read_data.clear();
	mock_write_data.clear();
}

void set_read_data(std::vector<uint8_t> &data, int length) {
	mock_read_data = data;
}

int i2c_read_blocking(i2c_inst_t *i2c, uint8_t addr, uint8_t *dst, size_t len, bool nostop) {
	lastAddress = addr;
	for (int i = 0; i < len; i++) {
		dst[i] = mock_read_data[mock_data_read];
		mock_data_read++;
	}
	last_length_read = len;
	return PICO_ERROR_NONE;
}

int i2c_write_blocking(i2c_inst_t *i2c, uint8_t addr, const uint8_t *src, size_t len, bool nostop) {
	lastAddress = addr;
	for (int i = 0; i < len; i++) {
		mock_write_data.push_back(src[i]);
	}
	last_length_written = len;
	return PICO_ERROR_NONE;
}

