/*
 * Copyright (c) 2021, Adam Boardman
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <mcp23017_input.h>

Mcp23017_input::Mcp23017_input(Mcp23017 &mcp, int detect)
: _mcp_detect(mcp), _detect_pin(detect) {
}

bool Mcp23017_input::get_current_state() const {
	return _mcp_detect.get_last_input_pin_value(_detect_pin);
}
