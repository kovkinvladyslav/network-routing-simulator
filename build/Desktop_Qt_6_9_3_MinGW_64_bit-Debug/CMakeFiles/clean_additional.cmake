# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\kpsrc_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\kpsrc_autogen.dir\\ParseCache.txt"
  "kpsrc_autogen"
  )
endif()
