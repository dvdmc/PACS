ADD_PACS_EXECUTABLE(TARGET smallpt_thread_pool SOURCES smallpt_thread_pool.cpp)
target_include_directories(smallpt_thread_pool
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
ADD_PACS_EXECUTABLE(TARGET smallpt_thread_pool_lock_free SOURCES smallpt_thread_pool_lock_free.cpp)
target_include_directories(smallpt_thread_pool_lock_free
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
ADD_PACS_EXECUTABLE(TARGET test_with_queue SOURCES test_with_queue.cpp)
target_include_directories(test_with_queue
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")