# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\MinesweeperProject_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MinesweeperProject_autogen.dir\\ParseCache.txt"
  "MinesweeperProject_autogen"
  )
endif()
