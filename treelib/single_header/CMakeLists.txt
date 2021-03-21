cmake_minimum_required(VERSION 3.1...3.19)

project(build_single_header)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# regular expression for include directives parsing
set(INCLUDE_REGEX "^[ \t]*#[ \t]*include[ \t]*[<\"]([^>\"]*)[>\"]")
set(LOCAL_INCLUDE_REGEX "^[ \t]*#[ \t]*include[ \t]*[\"]([^\"]*)[\"]")
set(LOCAL_INCLUDE_START)

# file paths
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
set(INCLUDE_DIR "${PARENT_DIR}/include")
set(ALL_HEADER "merge_order.hpp")

# auxiliary functions
function(create_file FILENAME)
    file(WRITE ${FILENAME} "")
endfunction()

function(append_header APPEND_TO_FILE HEADER)
    # read header's content and comment out include directives containing local headers
    file(READ ${HEADER} HEADER_CONTENT)
    string(REPLACE "#include \"" "// #include \"" HEADER_CONTENT "${HEADER_CONTENT}")
    # append the content to the file
    file(APPEND "${APPEND_TO_FILE}" "${HEADER_CONTENT}")
    file(APPEND "${APPEND_TO_FILE}" "\n")
endfunction()

# log building begin
message("building ${ALL_HEADER}")

# create the single header file
set(SINGLE_HEADER "single_header.hpp")
create_file("${SINGLE_HEADER}")

# read all.hpp header file
file(STRINGS "${ALL_HEADER}" LINES)
foreach(LINE ${LINES})
    if(LINE MATCHES "${INCLUDE_REGEX}")
        # get filename from the include directive
        set(FILENAME "")
        string(REGEX REPLACE "^[ \t]*#[ \t]*include[ \t]*[<\"]"
                "" FILENAME
                ${LINE}
        )
        string(REGEX REPLACE "[>\"]" "" FILENAME ${FILENAME})
        message("processing ${FILENAME}")

        # get full path to the file
        set(FULL_PATH ${INCLUDE_DIR})
        string(APPEND FULL_PATH "/${FILENAME}")

        # append the header to the single header file
        append_header("${SINGLE_HEADER}" "${FULL_PATH}")
        message("done ${FILENAME}")
    else()
        file(APPEND "${SINGLE_HEADER}" "${LINE}\n")
    endif()
endforeach()