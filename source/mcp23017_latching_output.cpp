/*
 * Copyright (c) 2021, Adam Boardman
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <mcp23017_latching_output.h>

Mcp23017_latching_output::Mcp23017_latching_output(Mcp23017 &mcp, int off, int on)
: _mcp_out(mcp), _off_pin(off), _on_pin(on) {
}

void Mcp23017_latching_output::set_output_state(bool desired_state) {
	_mcp_out.set_output_bit_for_pin(_off_pin, !desired_state);
	_mcp_out.set_output_bit_for_pin(_on_pin, desired_state);
	_mcp_out.flush_output();
}

void Mcp23017_latching_output::desired_state_achieved() {
	_mcp_out.set_output_bit_for_pin(_off_pin, false);
	_mcp_out.set_output_bit_for_pin(_on_pin, false);
	_mcp_out.flush_output();
}
