target_sources(quick_sort_seq_lib PRIVATE quick_sort_seq.cpp)
target_sources(quick_sort_seq PRIVATE main_seq.cpp)

target_sources(quick_sort_par PRIVATE
    main.cpp
    quick_sort_par.cpp
)
