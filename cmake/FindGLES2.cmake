#SET(CMAKE_SHARED_LIBRARY_PREFIX "lib")
#SET(CMAKE_SHARED_LIBRARY_SUFFIX "dll")

#find_path(GLES2_INCLUDE_DIR
#    NAMES
#        GLES2/gl2.h
#    PATHS
#        ${GLES2ROOT}/include
#        /usr/include
#        /usr/include/x86_64-linux-gnu
#        /usr/local/include
#        /local/include
#        /mingw/include
#        /opt/local/include
#        /opt/include
#        /sw/include
#)


find_library(GLES2_LIBRARY
    NAMES
        GLESv2
        libGLESv2.dll
    PATHS
        ${GLES2_ROOT}/lib
#        /usr/lib
#        /usr/local/lib
#        /opt/local/lib
#        /sw/lib
#        /local/lib
#        /mingw/lib
#        /local/bin
#        /mingw/bin
#        /usr/local/lib
)

IF(GLES2_LIBRARY)
   SET(GLES2_FOUND TRUE)
ENDIF(GLES2_LIBRARY)
MESSAGE(STATUS "Found GLES2:  CMAKE_SHARED_LIBRARY_PREFIX=${CMAKE_SHARED_LIBRARY_PREFIX} CMAKE_SHARED_LIBRARY_SUFFIX=${CMAKE_SHARED_LIBRARY_SUFFIX} GLES2_LIBRARY=${GLES2_LIBRARY} GLES2_INCLUDE_DIR=${GLES2_INCLUDE_DIR} GLES2_ROOT=${GLES2_ROOT}")
IF(GLES2_FOUND)
  IF(NOT GLES2_FIND_QUIETLY)
    MESSAGE(STATUS "Found GLES2: ${GLES2_LIBRARY} ${GLES2_INCLUDE_DIR}")
  ENDIF(NOT GLES2_FIND_QUIETLY)
ELSE(GLES2_FOUND)
  IF(GLES2_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find GLES2")
  ENDIF(GLES2_FIND_REQUIRED)
ENDIF(GLES2_FOUND)

