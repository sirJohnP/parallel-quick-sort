add_executable(quick_sort
    src/main.cpp
    src/quick_sort_par.cpp
    src/quick_sort_seq.cpp
)

target_compile_features(quick_sort PRIVATE cxx_std_17)
target_include_directories(quick_sort PRIVATE include)
target_link_libraries(quick_sort PRIVATE parlay)

if(BUILD_TESTING)
    add_executable(sort_tests
        tests/sort_tests.cpp
        src/quick_sort_par.cpp
        src/quick_sort_seq.cpp
    )
    target_compile_features(sort_tests PRIVATE cxx_std_17)
    target_include_directories(sort_tests PRIVATE include)
    target_link_libraries(sort_tests PRIVATE parlay)

    add_test(NAME sort_seq COMMAND sort_tests seq)
    foreach(threads IN ITEMS 1 4)
        add_test(NAME sort_par_${threads}_threads COMMAND sort_tests par)
        set_tests_properties(sort_par_${threads}_threads PROPERTIES
            ENVIRONMENT "PARLAY_NUM_THREADS=${threads}"
        )
    endforeach()
    set_tests_properties(sort_seq sort_par_1_threads sort_par_4_threads
        PROPERTIES TIMEOUT 30
    )
endif()
