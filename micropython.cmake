# Create an INTERFACE library for our C module.
add_library(led_strip_main INTERFACE)

# Add our source files to the lib
target_sources(led_strip_main INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/main/led_strip_main.c
    ${CMAKE_CURRENT_LIST_DIR}/main/led_strip.h
    ${CMAKE_CURRENT_LIST_DIR}/main/led_strip_rmt_ws2812.c
  
)

# Add the current directory as an include directory.
target_include_directories(led_strip_main INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE led_strip_main)