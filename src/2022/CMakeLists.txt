# Determine how many Advent days to build
execute_process(COMMAND ls
                COMMAND wc -l
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                OUTPUT_STRIP_TRAILING_WHITESPACE
                OUTPUT_VARIABLE NUM_FILES_RAW)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)

math(EXPR NUM_FILES "${NUM_FILES_RAW} - 1" OUTPUT_FORMAT DECIMAL) 
foreach(x RANGE 1 ${NUM_FILES})

# AoC Days Section

add_executable(day${x}_${PARENT_DIR} ${AOC_SOURCE_DIR}/src/main.cpp ${x}.cpp)
target_link_libraries(day${x}_${PARENT_DIR} PUBLIC fmt scn aocutil ctre)

# Test Section

add_test(NAME d${x}_${PARENT_DIR} COMMAND day${x}_${PARENT_DIR} ${AOC_SOURCE_DIR}/input/${PARENT_DIR}/${x}.txt)

endforeach()
