cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0011 OLD)

PROJECT (libxbrRunner C CXX)

INCLUDE_DIRECTORIES(
  "${CMAKE_CURRENT_SOURCE_DIR}"
)

# Helpers dedicated to the samples
SET( files_SRCS
	"filters.h"
	"xbr.c"
	"hq2x.c"
    "hq3x.c"
	"hq4x.c"
	"internal_hqx_common.h"
	"lodepng.cpp"
	"lodepng.h"
	"main.cpp"
)

ADD_EXECUTABLE( libxbrRunner "main.cpp" ${files_SRCS} )
