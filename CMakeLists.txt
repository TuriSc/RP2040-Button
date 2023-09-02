if (NOT TARGET button)
    add_library(button INTERFACE)

    target_sources(button INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}/button.c
    )

    target_include_directories(button INTERFACE
            ${CMAKE_CURRENT_LIST_DIR}
    )

    target_link_libraries(button INTERFACE
        pico_stdlib
    )
endif()