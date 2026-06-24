include_guard()

set(HC32_SDK ${TOP}/hw/mcu/hdsc/sdk-csp-hc32f4)
set(HC32_CMSIS ${HC32_SDK}/libraries/cmsis)
set(HC32_DEVICE ${HC32_CMSIS}/Device/HDSC/hc32f4xx)
set(HC32_DDL ${HC32_SDK}/libraries/hc32f460_ddl/hc32_ll_driver)

# include board specific
include(${CMAKE_CURRENT_LIST_DIR}/boards/${BOARD}/board.cmake)

# toolchain set up
set(CMAKE_SYSTEM_CPU cortex-m4 CACHE INTERNAL "System Processor")
set(CMAKE_TOOLCHAIN_FILE ${TOP}/examples/build_system/cmake/toolchain/arm_${TOOLCHAIN}.cmake)

set(FAMILY_MCUS HC32F4 CACHE INTERNAL "")

#------------------------------------
# Startup & Linker script
#------------------------------------
if (NOT DEFINED LD_FILE_GNU)
  set(LD_FILE_GNU ${HC32_SDK}/linkscripts/HC32F460xE/link.ld)
endif ()
set(LD_FILE_Clang ${LD_FILE_GNU})

set(STARTUP_FILE_GNU ${HC32_DEVICE}/Source/GCC/startup_hc32f460.S)
set(STARTUP_FILE_Clang ${STARTUP_FILE_GNU})

#------------------------------------
# Board Target
#------------------------------------
function(family_add_board BOARD_TARGET)
  add_library(${BOARD_TARGET} STATIC
    ${HC32_DEVICE}/Source/system_hc32f460.c
    ${HC32_DDL}/src/hc32_ll.c
    ${HC32_DDL}/src/hc32_ll_clk.c
    ${HC32_DDL}/src/hc32_ll_efm.c
    ${HC32_DDL}/src/hc32_ll_fcg.c
    ${HC32_DDL}/src/hc32_ll_gpio.c
    ${HC32_DDL}/src/hc32_ll_interrupts.c
    ${HC32_DDL}/src/hc32_ll_pwc.c
    ${HC32_DDL}/src/hc32_ll_sram.c
    ${HC32_DDL}/src/hc32_ll_usart.c
    ${HC32_DDL}/src/hc32_ll_utility.c
    )

  target_include_directories(${BOARD_TARGET} PUBLIC
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/boards/${BOARD}
    ${HC32_CMSIS}/Include
    ${HC32_DEVICE}/Include
    ${HC32_DDL}/inc
    )

  target_compile_definitions(${BOARD_TARGET} PUBLIC
    HC32F460
    USE_DDL_DRIVER
    )

  target_compile_options(${BOARD_TARGET} PRIVATE
    -Wno-error=cast-align
    -Wno-error=conversion
    -Wno-error=sign-conversion
    -Wno-error=unused-parameter
    )

  update_board(${BOARD_TARGET})
endfunction()

#------------------------------------
# Functions
#------------------------------------
function(family_configure_example TARGET RTOS)
  family_configure_common(${TARGET} ${RTOS})
  family_add_tinyusb(${TARGET} OPT_MCU_HC32F4)

  target_sources(${TARGET} PUBLIC
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/family.c
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../board.c
    ${TOP}/src/portable/synopsys/dwc2/dcd_dwc2.c
    ${TOP}/src/portable/synopsys/dwc2/hcd_dwc2.c
    ${TOP}/src/portable/synopsys/dwc2/dwc2_common.c
    ${STARTUP_FILE_${CMAKE_C_COMPILER_ID}}
    )

  target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../../
    ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/boards/${BOARD}
    )

  if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_link_options(${TARGET} PUBLIC
      "LINKER:--script=${LD_FILE_GNU}"
      -nostartfiles
      --specs=nosys.specs
      --specs=nano.specs
      )
  elseif (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR "Clang is not supported")
  elseif (CMAKE_C_COMPILER_ID STREQUAL "IAR")
    target_link_options(${TARGET} PUBLIC
      "LINKER:--config=${LD_FILE_IAR}"
      )
  endif ()

  if (CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set_source_files_properties(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/family.c PROPERTIES
      COMPILE_FLAGS "-Wno-missing-prototypes -Wno-redundant-decls")
  endif ()

  set_source_files_properties(${STARTUP_FILE_${CMAKE_C_COMPILER_ID}} PROPERTIES
    SKIP_LINTING ON
    COMPILE_OPTIONS -w)

  # Flashing
  family_add_bin_hex(${TARGET})
  family_flash_pyocd(${TARGET})
endfunction()
