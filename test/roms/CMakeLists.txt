
set(NES_CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/nes.cfg)

macro(add_roms TARGET ROMS)
    # copy source files
    file(COPY ${ROMS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    foreach(filepath ${ROMS})
        get_filename_component(ROM_NAME ${filepath} NAME_WE)

        # compile to .nes rom
        add_custom_command(OUTPUT ROM_${ROM_NAME}
            COMMAND ca65 ${filepath} -o ${ROM_NAME}.o 
            COMMAND ld65 -C ${NES_CONFIG} ${ROM_NAME}.o -o ${ROM_NAME}.nes)

        list(APPEND ROM_NAMES ROM_${ROM_NAME})
    endforeach()

    add_custom_target(${TARGET} ALL
        DEPENDS ${ROM_NAMES})
endmacro(add_roms)

configure_file(roms.h.in ${CMAKE_CURRENT_BINARY_DIR}/roms.h)

# Roms for CPU
file(GLOB CPU_ROM_SRC cpu/*.asm)
add_roms(CpuRoms "${CPU_ROM_SRC}")

