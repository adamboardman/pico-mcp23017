/*
 * Copyright (c) 2021, Adam Boardman
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MCP23017_INPUT_H
#define MCP23017_INPUT_H

#include "mcp23017.h"
#include "input_detect.h"

class Mcp23017_input : public Input_detect {
public:
	Mcp23017_input(Mcp23017 &mcp, int detect);

	[[nodiscard]] bool get_current_state() const override;

private:
	Mcp23017 &_mcp_detect;
	int _detect_pin;
};

#endif //MCP23017_INPUT_H
