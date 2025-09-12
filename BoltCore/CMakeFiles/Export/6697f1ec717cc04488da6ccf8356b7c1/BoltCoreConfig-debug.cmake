#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "BoltCore::BoltCore" for configuration "Debug"
set_property(TARGET BoltCore::BoltCore APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(BoltCore::BoltCore PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/BoltCore.lib"
  )

list(APPEND _cmake_import_check_targets BoltCore::BoltCore )
list(APPEND _cmake_import_check_files_for_BoltCore::BoltCore "${_IMPORT_PREFIX}/lib/BoltCore.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
