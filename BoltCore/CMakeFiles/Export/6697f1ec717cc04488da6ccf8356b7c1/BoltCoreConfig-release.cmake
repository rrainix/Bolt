#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "BoltCore::BoltCore" for configuration "Release"
set_property(TARGET BoltCore::BoltCore APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(BoltCore::BoltCore PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/BoltCore.lib"
  )

list(APPEND _cmake_import_check_targets BoltCore::BoltCore )
list(APPEND _cmake_import_check_files_for_BoltCore::BoltCore "${_IMPORT_PREFIX}/lib/BoltCore.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
