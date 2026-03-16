# serial version
set(SERIAL_SOURCES SerialApproach.cpp circle_collider.cpp)
if(GRAPHICS)
list(APPEND SERIAL_SOURCES graphics_utils.cpp) 
endif()
add_executable(serialCollider ${SERIAL_SOURCES})
target_include_directories(serialCollider PUBLIC ${CMAKE_SOURCE_DIR}/include)

# distributed version
set(DISTRIBUTED_SOURCES SpacePartitioned.cpp circle_collider.cpp)
if(GRAPHICS)
list(APPEND DISTRIBUTED_SOURCES graphics_utils.cpp) 
endif()
add_executable(distributedCollider ${DISTRIBUTED_SOURCES})
target_include_directories(distributedCollider PUBLIC ${CMAKE_SOURCE_DIR}/include)


# Link to the actual SDL3 library.
target_link_libraries(serialCollider PUBLIC MPI::MPI_CXX)
target_link_libraries(distributedCollider PUBLIC MPI::MPI_CXX)
if(GRAPHICS)
    target_link_libraries(serialCollider PUBLIC SDL3::SDL3)
    target_link_libraries(distributedCollider PUBLIC SDL3::SDL3)
endif()