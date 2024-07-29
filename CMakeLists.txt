cmake_minimum_required( VERSION 3.18.0 )
project( test_task )

set( CMAKE_CXX_STANDARD 20 CACHE STRING "" FORCE )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined")
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wextra")
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wfloat-equal")
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wshadow")
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wcast-align")
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wstrict-overflow=5")
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wwrite-strings")
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wcast-qual")
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wswitch-default")
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wswitch-enum")
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wconversion")
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wunreachable-code")
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wformat=2")
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_FORTIFY_SOURCE=2")
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fasynchronous-unwind-tables")
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pipe")
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Werror=format-security")

cmake_policy(SET CMP0004 OLD)
cmake_policy(SET CMP0004 OLD)

if (UNIX)
    execute_process(
        COMMAND bash -c "fltk-config --cxxflags"
        OUTPUT_VARIABLE cxx_flags_var
    )

    execute_process(
        COMMAND bash -c "fltk-config --ldflags"
        OUTPUT_VARIABLE ld_flags_var
    )


    add_executable( test_task )
    target_link_libraries(test_task ${ld_flags_var})
    add_compile_options(test_task "${${cxx_flags_var}}")
    add_subdirectory( "src" )
endif()

if (WIN32)
    FIND_PACKAGE(FLTK REQUIRED)
    FIND_PACKAGE(OpenGL REQUIRED)

    add_executable( test_task )

    target_link_libraries(test_task "-static")
    TARGET_LINK_LIBRARIES(test_task ${FLTK_LIBRARIES})
    TARGET_LINK_LIBRARIES(test_task ${OPENGL_LIBRARIES})
    add_subdirectory( "src" )
endif()
