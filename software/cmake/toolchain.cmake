if(WIN32)
    set(TOOLCHAIN_SUFFIX ".exe")
endif()

set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

if(WIN32)
    set(TOOLCHAIN_PREFIX "C:/ST/STM32CubeCLT/GNU-tools-for-STM32/bin")
elseif(MSVC)
    set(TOOLCHAIN_PREFIX "")
else()
    set(TOOLCHAIN_PREFIX "")
endif()

set(TOOLCHAIN_DIRECTORY "C:/ST/STM32CubeCLT/GNU-tools-for-STM32/bin")

set(TOOLCHAIN_PREFIX                "arm-none-eabi-")
if(DEFINED TOOLCHAIN_DIRECTORY)
    set(TOOLCHAIN_PREFIX            "${TOOLCHAIN_DIRECTORY}/${TOOLCHAIN_PREFIX}")
endif()
set(FLAGS                           "-fdata-sections -ffunction-sections --specs=nano.specs --specs=nosys.specs -Wl,--gc-sections")
set(ASM_FLAGS                       "-x assembler-with-cpp")
set(CPP_FLAGS                       "-fno-rtti -fno-exceptions -fno-threadsafe-statics")

set(CMAKE_C_COMPILER                ${TOOLCHAIN_PREFIX}gcc${TOOLCHAIN_SUFFIX} ${FLAGS})
set(CMAKE_ASM_COMPILER              ${CMAKE_C_COMPILER} ${ASM_FLAGS})
set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PREFIX}g++${TOOLCHAIN_SUFFIX} ${FLAGS} ${CPP_FLAGS})
set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}objcopy${TOOLCHAIN_SUFFIX})
set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}size${TOOLCHAIN_SUFFIX})

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)