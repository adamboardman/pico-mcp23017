# Raspberry Pi Pico MCP23017 Library

This library allows you to talk to MCP23017 16bit I/O Expander

# Usage

## Include library

Check the module out as a submodule of your project to, say: modules/pico-mcp23017.
You will also need [pico-interfaces](https://github.com/adamboardman/pico-interfaces).

Then add the subdirectories and link the library in your CMakeLists.txt:

```
add_subdirectory(modules/pico-interfaces)
add_subdirectory(modules/pico-mcp23017)

target_link_libraries( ... pico_mcp23017)
```

## Input

```C++
#include "hardware/i2c.h"
#include "mcp23017.h"

static const bool MIRROR_INTERRUPTS = true; //save a gpio by mirroring interrupts across both banks
static const bool OPEN_DRAIN_INTERRUPT_ACTIVE = false;
static const bool POLARITY_INTERRUPT_ACTIVE_LOW = false;
static const int MCP_ALL_PINS_INPUT = 0xffff;
static const int MCP_ALL_PINS_PULL_UP = 0xffff;
static const int MCP_ALL_PINS_COMPARE_TO_LAST = 0x0000;
static const int MCP_ALL_PINS_INTERRUPT_ENABLED = 0xffff;

#define MCP_IRQ_GPIO_PIN 19
#define I2C_GPIO_PIN_SDA 20
#define I2C_GPIO_PIN_SLC 21

Mcp23017 mcp0(i2c0, 0x20); // MCP with A0,1,2 to GND
bool interrupt_on_mcp0 = false; //set to true if you want to read values on startup

void gpio_callback(uint gpio, uint32_t events) {
	printf("IRQ callback on GPIO %u, events: %lu\n", gpio, events);
	if (gpio == MCP_IRQ_GPIO_PIN && (events & GPIO_IRQ_EDGE_FALL)) {
		interrupt_on_mcp0 = true;
	}
}

void setup_input(Mcp23017 mcp, uint gpio_irq) {
	int result;

	result = mcp.setup(MIRROR_INTERRUPTS, OPEN_DRAIN_INTERRUPT_ACTIVE, POLARITY_INTERRUPT_ACTIVE_LOW);
	result = mcp.set_io_direction(MCP_ALL_PINS_INPUT);
	result = mcp.set_pullup(MCP_ALL_PINS_PULL_UP);
	result = mcp.set_interrupt_type(MCP_ALL_PINS_COMPARE_TO_LAST);
	result = mcp.enable_interrupt(MCP_ALL_PINS_INTERRUPT_ENABLED);

	gpio_set_irq_enabled_with_callback(gpio_irq, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

	//once we are listening for interrupts clear previous ones just incase
	int int_values = mcp.get_interrupt_values();
}

int main() {
	stdio_init_all();
	sleep_ms(2000);
	printf("Starting up\n");

	i2c_init(i2c0, 400000);
	gpio_set_function(I2C_GPIO_PIN_SDA, GPIO_FUNC_I2C);
	gpio_set_function(I2C_GPIO_PIN_SLC, GPIO_FUNC_I2C);
	gpio_pull_up(I2C_GPIO_PIN_SDA);
	gpio_pull_up(I2C_GPIO_PIN_SLC);

	setup_input(mcp0, MCP_IRQ_GPIO_PIN);

	while (true) {
		if (interrupt_on_mcp0) {
			interrupt_on_mcp0 = false;
			int pin = mcp0.get_last_interrupt_pin();
			int int_values = mcp0.get_interrupt_values();
			int input_values_ok = mcp0.update_and_get_input_values();

			printf("MCP(0x%2x), PIN: %d, Int:%d, InputOK:%d\n", mcp0.get_address(), pin, int_values, input_values_ok);
			printf("InputPin0: %d\n", mcp0.get_last_input_pin_value(0));
			printf("InputPin1: %d\n", mcp0.get_last_input_pin_value(1));
			printf("InputPin2: %d\n", mcp0.get_last_input_pin_value(2));
			printf("InputPin3: %d\n", mcp0.get_last_input_pin_value(3));
			light_switch.switch_to_state(mcp0.get_last_input_pin_value(1));
		}

		__wfe();
    }
    return 0;
}
```

## Output


```C++
#include "hardware/i2c.h"
#include "mcp23017.h"

static const int MCP_ALL_PINS_OFF = 0x0000;

#define I2C_GPIO_PIN_SDA 20
#define I2C_GPIO_PIN_SLC 21

Mcp23017 mcp1(i2c0, 0x21); // MCP with A0 to +3, A1,2 to GND

void setup_output(Mcp23017 mcp) {
	int result;

	result = mcp.setup(true, false, false);
	result = mcp.set_io_direction(MCP_ALL_PINS_OUTPUT);
}

int main() {
	stdio_init_all();
	sleep_ms(2000);
	printf("Starting up\n");

	i2c_init(i2c0, 400000);
	gpio_set_function(I2C_GPIO_PIN_SDA, GPIO_FUNC_I2C);
	gpio_set_function(I2C_GPIO_PIN_SLC, GPIO_FUNC_I2C);
	gpio_pull_up(I2C_GPIO_PIN_SDA);
	gpio_pull_up(I2C_GPIO_PIN_SLC);

	setup_output(mcp1);
	mcp1.set_all_output_bits(MCP_ALL_PINS_OFF);

	printf("Setting MCP(0x21) pin 4\n");
	mcp1.set_output_bit_for_pin(4, true);
	mcp1.flush_output();

    return 0;
}
```

# Running the test code on a desktop

If your just using the library you don't need to worry about the test code.
If you plan on fixing bugs or extending the library then you must.

Install Catch2 version 3, it might be available on your desktop linux distro,
though possibly only an older version. Or you can download, build and install it:

```sh
git clone  https://github.com/catchorg/Catch2.git
cd Catch2
cmake -Bbuild -H. -DBUILD_TESTING=OFF
cmake --build build/
sudo cmake --build build/ --target install
```

Build and run the test code:

```sh
cd test
mkdir build
cd build
cmake ..
make
./tests
```

You should then see 'All tests passed'
