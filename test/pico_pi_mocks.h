/*
 * Copyright (c) 2021, Adam Boardman
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef PICO_PI_MOCKS_H
#define PICO_PI_MOCKS_H

#include <cstdint>
#include <vector>

typedef struct {
} i2c_hw_t;

struct i2c_inst {
	i2c_hw_t *hw;
	bool restart_on_next;
};

typedef struct i2c_inst i2c_inst_t;

typedef	unsigned int uint;

enum {
	PICO_OK = 0,
	PICO_ERROR_NONE = 0,
	PICO_ERROR_TIMEOUT = -1,
	PICO_ERROR_GENERIC = -2,
	PICO_ERROR_NO_DATA = -3,
};

extern int lastAddress;
extern int last_length_read;
extern int last_length_written;
extern long mock_data_read;
extern std::vector<uint8_t> mock_write_data;
extern std::vector<uint8_t> mock_read_data;

void reset_for_test(const i2c_inst_t *i2c);

void set_read_data(std::vector<uint8_t> &data, int length);

int i2c_read_blocking(i2c_inst_t *i2c, uint8_t addr, uint8_t *dst, size_t len, bool nostop);

int i2c_write_blocking(i2c_inst_t *i2c, uint8_t addr, const uint8_t *src, size_t len, bool nostop);

#endif // PICO_PI_MOCKS_H
