cmake_minimum_required( VERSION 3.18.0 )
project( test_task )

set( CMAKE_CXX_STANDARD 20 CACHE STRING "" FORCE )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE )
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


if (UNIX)
    set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined")
    cmake_policy(SET CMP0004 OLD)
    cmake_policy(SET CMP0004 OLD)
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

if (MINGW)
    add_executable( test_task )
    FIND_PACKAGE(FLTK REQUIRED)
    FIND_PACKAGE(OpenGL REQUIRED)
    set(BUILD_SHARED_LIBS OFF)
    set_target_properties(test_task PROPERTIES LINK_SEARCH_START_STATIC 1)
    set_target_properties(test_task PROPERTIES LINK_SEARCH_END_STATIC 1)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")


    target_include_directories(test_task PUBLIC ${FLTK_INCLUDE_DIRS})
    target_link_libraries(test_task ${FLTK_BASE_LIBRARY})
    target_link_libraries(test_task ${OPENGL_LIBRARIES})
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
    add_subdirectory( "src" )
    target_link_libraries(test_task comctl32)
endif()
