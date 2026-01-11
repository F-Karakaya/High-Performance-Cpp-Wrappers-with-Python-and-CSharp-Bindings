
add_library(NativeLib SHARED
    CHelpers.cpp
)

target_link_libraries(NativeLib PRIVATE cpp_core)

# We need to make sure the DLL is output to a known location for the C# app to find it.
# For simplicity, we put it in binary dir/bin
set_target_properties(NativeLib PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
