add_library(pico_mcp23017 INTERFACE)

target_sources(pico_mcp23017 INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/source/mcp23017.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/mcp23017_input.cpp
        ${CMAKE_CURRENT_LIST_DIR}/source/mcp23017_latching_output.cpp
        )

target_include_directories(pico_mcp23017 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/api)
target_link_libraries(pico_mcp23017 INTERFACE pico_interfaces pico_stdlib hardware_gpio hardware_i2c)
