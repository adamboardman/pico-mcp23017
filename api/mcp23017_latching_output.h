/*
 * Copyright (c) 2021, Adam Boardman
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MCP23017_LATCHING_OUTPUT_H
#define MCP23017_LATCHING_OUTPUT_H

#include "mcp23017.h"
#include "output_switch.h"

class Mcp23017_latching_output : public Output_switch {
public:
	Mcp23017_latching_output(Mcp23017 &mcp, int off, int on);

	void set_output_state(bool desired_state) override;

	void desired_state_achieved() override;

private:
	Mcp23017 &_mcp_out;
	int _off_pin;
	int _on_pin;
};

#endif //MCP23017_LATCHING_OUTPUT_H
