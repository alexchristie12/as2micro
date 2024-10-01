cmake_minimum_required(VERSION 3.13)

# Detect if the active kit is an ARM cross-compiler
if(CMAKE_CXX_COMPILER MATCHES "arm-none-eabi")
    message(STATUS "Detected that the current kit is a cross-compiler.")
    set(CrossCompiling 1)
else()
    message(STATUS "Detected that the current kit is a host compiler. Building the test harness.")
endif()

# Detect if the active kit is an ARM cross-compiler
if(CrossCompiling)
    # Yes, build for the RP2040

    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 17)

    # Specify an increased oscillator startup delay to work around a potential hardware issue where the 
    # clock is slow to boot.
    add_compile_definitions(PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64)

    # Pull in Raspberry Pi Pico SDK (must be before project)
    include(pico_sdk_import.cmake)

    project(as2micro C CXX ASM)
    
    # Initialise the Raspberry Pi Pico SDK
    pico_sdk_init()


    add_executable(rmu)
    target_sources(rmu 
        PUBLIC
        src/main.c
        src/drivers/logging/logging.c
        src/drivers/WS2812/led.c
        src/drivers/io/io.c
        src/drivers/i2c/i2c.c
        src/sensors/CHT8305C/temp_and_humidity.c
        src/sensors/SEN13322/soil_moisture.c
        src/tasks/tasks.c
    )
    target_include_directories(rmu
        PUBLIC 
        src/
    )

    pico_set_program_name(rmu "as2micro")
    pico_set_program_version(rmu "0.2")

    # select UART for standard IO
    pico_enable_stdio_uart(rmu 1)
    pico_enable_stdio_usb(rmu 0)

    # compile the PIO file
    pico_generate_pio_header(rmu ${CMAKE_CURRENT_LIST_DIR}/src/drivers/WS2812/WS2812.pio)

    # Add the standard library to the build
    target_link_libraries(rmu
        pico_stdlib
        hardware_spi
        hardware_i2c
        hardware_adc
        hardware_dma
        hardware_pio
        hardware_interp
        hardware_timer
        hardware_watchdog
        hardware_clocks
        hardware_pwm
    )

    pico_add_extra_outputs(rmu)

else()

    # set(CMAKE_C_STANDARD 11)
    # set(CMAKE_CXX_STANDARD 20)

    # # We are building natively, so create the test harness instead
    # project(cc3501-labs CXX)

    # add_executable(labs)
    # target_sources(labs 
    #     PUBLIC
    #     src/main.cpp
    #     src/drivers/logging/logging.cpp
    #     src/drivers/WS2812/led.cpp
    #     src/drivers/WS2812/led_io.cpp
    #     tests/mocks/pico/stdlib.cpp
    #     tests/mocks/pico/time.cpp
    #     tests/mocks/hardware/gpio.cpp
    #     tests/mocks/hardware/pio.cpp
    #     tests/mocks/ws2812.cpp
    #     io/mock/io/io.cpp
    # )
    # target_include_directories(labs
    #     PUBLIC 
    #     src/
    #     tests/
    #     tests/mocks/
    #     io/mock/
    # )
    # target_compile_definitions(labs 
    #     PUBLIC
    #     TEST_HARNESS=1
    # )

endif()

target_compile_definitions(rmu 
    PUBLIC
    LOG_DRIVER_STYLE=${LogDriverImplementation}
)
