# cmake_minimum_required(VERSION 3.13)

# include(pico_sdk_import.cmake)

# project(tarefa_iot_botao C CXX ASM)

# set(PICO_BOARD pico_w)
# set(PICO_CYW43_ARCH_THREADSAFE_BACKGROUND ON)

# pico_sdk_init()

# include_directories(
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${PICO_SDK_PATH}/src/common/pico_base/include
#     ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_arch/include
#     FreeRTOS-Kernel/include
#     FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/include
# )

# add_library(freertos_config INTERFACE)
# target_include_directories(freertos_config SYSTEM
#   INTERFACE
#     ${CMAKE_CURRENT_SOURCE_DIR}  
# )

# #include(FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)
# add_subdirectory(FreeRTOS-Kernel)

# add_executable(tarefa_iot_botao
#     main.c
#     wifi.c
#     button.c
# )

# pico_enable_stdio_usb(tarefa_iot_botao 1)
# pico_enable_stdio_uart(tarefa_iot_botao 0)

# target_link_libraries(tarefa_iot_botao
#     pico_stdlib
#     pico_cyw43_arch_lwip_threadsafe_background
#     # FreeRTOS-Kernel-Objects
#     FreeRTOS::Kernel
# )

# pico_add_extra_outputs(tarefa_iot_botao)



# Generated Cmake Pico project file
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 1.5.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(tarefa_iot_botao C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(tarefa_iot_botao
    main.c
    wifi.c
    button.c
    joystick.c
)

pico_set_program_name(tarefa_iot_botao "tarefa_iot_botao")
pico_set_program_version(tarefa_iot_botao "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_usb(tarefa_iot_botao 1)
pico_enable_stdio_uart(tarefa_iot_botao 0)

# Add the standard include files to the build
# target_include_directories(tarefa_iot_botao PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(tarefa_iot_botao PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${PICO_SDK_PATH}/src/rp2_common/hardware_adc/include
    ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_arch/include
    ${PICO_SDK_PATH}/sdk/1.5.1/lib/lwip/src/include

)

# Add any user requested libraries
target_link_libraries(tarefa_iot_botao
    pico_stdlib 
    hardware_gpio
    pico_multicore 
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
    pico_cyw43_arch_lwip_threadsafe_background  
    lwip  
)

pico_add_extra_outputs(tarefa_iot_botao)