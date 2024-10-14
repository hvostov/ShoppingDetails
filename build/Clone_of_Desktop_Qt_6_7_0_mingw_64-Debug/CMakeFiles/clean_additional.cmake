# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\ShoppingDetails_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ShoppingDetails_autogen.dir\\ParseCache.txt"
  "ShoppingDetails_autogen"
  )
endif()
