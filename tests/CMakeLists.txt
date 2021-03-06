# use assertions in tests
string(REGEX REPLACE "-DNDEBUG" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

include_directories(${CMAKE_SOURCE_DIR}/include)

# add check target
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

# --------------------------------------------------
# dg-test
# --------------------------------------------------
add_executable(dg-test dg-test.cpp)
add_test(dg-test dg-test)
add_dependencies(check dg-test)

# --------------------------------------------------
# data-flow-test
# --------------------------------------------------
add_executable(data-flow-test data-flow-test.cpp)
add_test(data-flow-test data-flow-test)
add_dependencies(check data-flow-test)

# --------------------------------------------------
# points-to-test
# --------------------------------------------------
add_executable(points-to-test points-to-test.cpp)
add_test(points-to-test points-to-test)
add_dependencies(check points-to-test)
target_link_libraries(points-to-test PRIVATE PTA)

# --------------------------------------------------
# rdmap-test
# --------------------------------------------------
add_executable(rdmap-test rdmap-test.cpp)
target_link_libraries(rdmap-test PRIVATE DGAnalysis)
add_test(rdmap-test rdmap-test)
add_dependencies(check rdmap-test)
target_link_libraries(rdmap-test PRIVATE RD)
target_include_directories(rdmap-test PRIVATE ${CMAKE_SOURCE_DIR}/lib)

# --------------------------------------------------
# reaching-definitions-test
# --------------------------------------------------
add_executable(reaching-definitions-test reaching-definitions-test.cpp)
add_test(reaching-definitions-test reaching-definitions-test)
add_dependencies(check reaching-definitions-test)
target_link_libraries(reaching-definitions-test PRIVATE RD)

# --------------------------------------------------
# adt-test
# --------------------------------------------------
add_executable(adt-test adt-test.cpp)
target_link_libraries(adt-test PRIVATE DGAnalysis)
add_test(adt-test adt-test)
add_dependencies(check adt-test)

# --------------------------------------------------
# bitvector-test
# --------------------------------------------------
add_executable(bitvector-test bitvector-test.cpp)
add_test(bitvector-test bitvector-test)
add_dependencies(check bitvector-test)

# --------------------------------------------------
# numbers-set-test
# --------------------------------------------------
add_executable(numbers-set-test numbers-set-test.cpp)
add_test(numbers-set-test numbers-set-test)
add_dependencies(check numbers-set-test)

# --------------------------------------------------
# points-to-set-test
# --------------------------------------------------
add_executable(points-to-set-test points-to-set-test.cpp)
target_link_libraries(points-to-set-test PRIVATE DGAnalysis PTA)
add_test(points-to-set-test points-to-set-test)
add_dependencies(check points-to-set-test)

# --------------------------------------------------
# disjunctive-intervals-map-test
# --------------------------------------------------
add_executable(disjunctive-intervals-map-test disjunctive-intervals-map-test.cpp)
target_link_libraries(disjunctive-intervals-map-test PRIVATE DGAnalysis)
add_test(disjunctive-intervals-map-test disjunctive-intervals-map-test)
add_dependencies(check disjunctive-intervals-map-test)
target_include_directories(disjunctive-intervals-map-test PRIVATE ${CMAKE_SOURCE_DIR}/lib)

# --------------------------------------------------
# nodes-walk-test
# --------------------------------------------------
add_executable(nodes-walk-test nodes-walk-test.cpp)
add_test(nodes-walk-test nodes-walk-test)
add_dependencies(check nodes-walk-test)

# --------------------------------------------------
# fuzzing tests
# --------------------------------------------------
if (TESTING_ENABLE_FUZZING)
	add_subdirectory(fuzzing)
endif(TESTING_ENABLE_FUZZING)

# --------------------------------------------------
# fuzzing regressions
# --------------------------------------------------
add_executable(numbers-set1-regressions
	       numbers-set1-regressions.cpp fuzzing/numbers-set1.cpp)
add_dependencies(check numbers-set1-regressions)
add_test(numbers-set1-regressions numbers-set1-regressions)

add_executable(disjunctive-interval-map-regressions
	       disjunctive-interval-map-regressions.cpp fuzzing/disjunctive-map1.cpp)
target_compile_definitions(disjunctive-interval-map-regressions PRIVATE "-DDUMP")
add_dependencies(check disjunctive-interval-map-regressions)
add_test(disjunctive-interval-map-regressions disjunctive-interval-map-regressions)

# --------------------------------------------------
# ThreadRegions test
# --------------------------------------------------

add_custom_command(OUTPUT simple.ll pthread_exit.ll
                   COMMAND clang -S -emit-llvm ${CMAKE_CURRENT_LIST_DIR}/thread-regions-test-files/simple.c
                   COMMAND clang -S -emit-llvm ${CMAKE_CURRENT_LIST_DIR}/thread-regions-test-files/pthread_exit.c
                   DEPENDS ${CMAKE_CURRENT_LIST_DIR}/thread-regions-test-files/simple.c ${CMAKE_CURRENT_LIST_DIR}/thread-regions-test-files/pthread_exit.c )

add_custom_target(thread-regions-test-file DEPENDS simple.ll)

add_executable(thread-regions-test ${CMAKE_CURRENT_LIST_DIR}/catch-main.cpp
                                  ${CMAKE_CURRENT_LIST_DIR}/thread-regions-test.cpp)
add_dependencies(thread-regions-test thread-regions-test-file)

target_compile_definitions(thread-regions-test
    PRIVATE
        SIMPLE_FILE="${CMAKE_CURRENT_BINARY_DIR}/simple.ll"
        PTHREAD_EXIT_FILE="${CMAKE_CURRENT_BINARY_DIR}/pthread_exit.ll")

target_link_libraries(thread-regions-test PRIVATE ThreadRegions
                                          PRIVATE ${llvm_core}
                                          PRIVATE ${llvm_irreader}
                                          PRIVATE ${llvm_support})
add_test(thread-regions-test thread-regions-test)
add_dependencies(check thread-regions-test)

# --------------------------------------------------
# llvm-dg-test
# --------------------------------------------------
if (LLVM_DG)
	set(LLVM_LINK_COMPONENTS core engine asmparser bitreader)
	add_executable(llvm-dg-test llvm-dg-test.cpp)
	target_link_libraries(llvm-dg-test
				PRIVATE LLVMdg
				PRIVATE ${llvm_irreader}
				PRIVATE ${llvm_analysis})

	add_test(llvm-dg-test llvm-dg-test)
	add_dependencies(check llvm-dg-test)

	add_test(slicing-test1 slicing-test1.sh)
	add_test(slicing-test2 slicing-test2.sh)
	add_test(slicing-test3 slicing-test3.sh)
	add_test(slicing-test4 slicing-test4.sh)
	add_test(slicing-test5 slicing-test5.sh)
	add_test(slicing-test6 slicing-test6.sh)
	add_test(slicing-test7 slicing-test7.sh)
	add_test(slicing-test8 slicing-test8.sh)
	add_test(slicing-test22 slicing-test22.sh)
	add_test(slicing-test222 slicing-test222.sh)
	add_test(slicing-recursive1 slicing-recursive1.sh)
	add_test(slicing-recursive2 slicing-recursive2.sh)
	add_test(slicing-recursive3 slicing-recursive3.sh)
	add_test(slicing-recursive4 slicing-recursive4.sh)
	add_test(slicing-interprocedural1 slicing-interprocedural1.sh)
	add_test(slicing-interprocedural2 slicing-interprocedural2.sh)
	add_test(slicing-interprocedural3 slicing-interprocedural3.sh)
	add_test(slicing-interprocedural4 slicing-interprocedural4.sh)
	add_test(slicing-interprocedural5 slicing-interprocedural5.sh)
	add_test(slicing-interprocedural6 slicing-interprocedural6.sh)
	add_test(slicing-interprocedural7 slicing-interprocedural7.sh)
	add_test(slicing-interprocedural8 slicing-interprocedural8.sh)
	add_test(slicing-interprocedural9 slicing-interprocedural9.sh)
	add_test(slicing-funcptr1 slicing-funcptr1.sh)
	add_test(slicing-funcptr2 slicing-funcptr2.sh)
	add_test(slicing-funcptr3 slicing-funcptr3.sh)
	add_test(slicing-funcptr4 slicing-funcptr4.sh)
	add_test(slicing-funcptr5 slicing-funcptr5.sh)
	add_test(slicing-funcptr6 slicing-funcptr6.sh)
	add_test(slicing-funcptr7 slicing-funcptr7.sh)
	add_test(slicing-funcptr8 slicing-funcptr8.sh)
	add_test(slicing-funcptr9 slicing-funcptr9.sh)
	add_test(slicing-funcptr10 slicing-funcptr10.sh)
	add_test(slicing-funcptr11 slicing-funcptr11.sh)
	add_test(slicing-funcptr12 slicing-funcptr12.sh)
	add_test(slicing-funcptr13 slicing-funcptr13.sh)
	add_test(slicing-funcptr-regression1 slicing-funcptr-regression1.sh)
	add_test(slicing-funcarray1 slicing-funcarray1.sh)
	add_test(slicing-funcarray2 slicing-funcarray2.sh)
	add_test(slicing-funcarray3 slicing-funcarray3.sh)
	add_test(slicing-unknownptr1 slicing-unknownptr1.sh)
	add_test(slicing-unknownptr2 slicing-unknownptr2.sh)
	add_test(slicing-unknownptr3 slicing-unknownptr3.sh)
	add_test(slicing-unknownptr4 slicing-unknownptr4.sh)
	add_test(slicing-unknownptr5 slicing-unknownptr5.sh)
	add_test(slicing-pointers1 slicing-pointers1.sh)
	add_test(slicing-pointers2 slicing-pointers2.sh)
	add_test(slicing-pointers3 slicing-pointers3.sh)
	add_test(slicing-pointers4 slicing-pointers4.sh)
	add_test(slicing-pointers5 slicing-pointers5.sh)
	add_test(slicing-pointers6 slicing-pointers6.sh)
	add_test(slicing-pointers7 slicing-pointers7.sh)
	add_test(slicing-ptrarray1 slicing-ptrarray1.sh)
	add_test(slicing-ptrarray2 slicing-ptrarray2.sh)
	add_test(slicing-phi1 slicing-phi1.sh)
	add_test(slicing-phi2 slicing-phi2.sh)
	add_test(slicing-phi3 slicing-phi3.sh)
	add_test(slicing-phi4 slicing-phi4.sh)
	add_test(slicing-global1 slicing-global1.sh)
	add_test(slicing-global2 slicing-global2.sh)
	add_test(slicing-global3 slicing-global3.sh)
	add_test(slicing-global4 slicing-global4.sh)
	add_test(slicing-global5 slicing-global5.sh)
	add_test(slicing-global6 slicing-global6.sh)
	add_test(slicing-global7 slicing-global7.sh)
	add_test(slicing-global8 slicing-global8.sh)
	add_test(slicing-global9 slicing-global9.sh)
	add_test(slicing-global10 slicing-global10.sh)
	add_test(slicing-ptrtoint1 slicing-ptrtoint1.sh)
	add_test(slicing-ptrtoint2 slicing-ptrtoint2.sh)
	add_test(slicing-ptrtoint3 slicing-ptrtoint3.sh)
	add_test(slicing-ptrtoint4 slicing-ptrtoint4.sh)
	add_test(slicing-ptrtoint5 slicing-ptrtoint5.sh)
	add_test(slicing-ptrtoint6 slicing-ptrtoint6.sh)
	add_test(slicing-ptrtoint7 slicing-ptrtoint7.sh)
	add_test(slicing-llvmmemcpy slicing-llvmmemcpy.sh)
	add_test(slicing-llvmmemcpy2 slicing-llvmmemcpy2.sh)
	add_test(slicing-memset1 slicing-memset1.sh)
	add_test(slicing-memcpy1 slicing-memcpy1.sh)
	add_test(slicing-memcpy2 slicing-memcpy2.sh)
	add_test(slicing-memcpy3 slicing-memcpy3.sh)
	add_test(slicing-memcpy4 slicing-memcpy4.sh)
	add_test(slicing-memcpy5 slicing-memcpy5.sh)
	add_test(slicing-memcpy6 slicing-memcpy6.sh)
	add_test(slicing-bitcast1 slicing-bitcast1.sh)
	add_test(slicing-bitcast2 slicing-bitcast2.sh)
	add_test(slicing-bitcast3 slicing-bitcast3.sh)
	add_test(slicing-bitcast4 slicing-bitcast4.sh)
	add_test(slicing-bitcast5 slicing-bitcast5.sh)
	add_test(slicing-loop1 slicing-loop1.sh)
	add_test(slicing-loop2 slicing-loop2.sh)
	add_test(slicing-loop3 slicing-loop3.sh)
	add_test(slicing-loop4 slicing-loop4.sh)
	add_test(slicing-loop5 slicing-loop5.sh)
	add_test(slicing-list1 slicing-list1.sh)
	add_test(slicing-list2 slicing-list2.sh)
	add_test(slicing-list3 slicing-list3.sh)
	add_test(slicing-list4 slicing-list4.sh)
	add_test(slicing-list5 slicing-list5.sh)
	add_test(slicing-list6 slicing-list6.sh)
	add_test(slicing-list7 slicing-list7.sh)
	add_test(slicing-list8 slicing-list8.sh)
	add_test(slicing-list9 slicing-list9.sh)
	add_test(slicing-dynalloc1 slicing-dynalloc1.sh)
	add_test(slicing-dynalloc2 slicing-dynalloc2.sh)
	add_test(slicing-dynalloc3 slicing-dynalloc3.sh)
	add_test(slicing-dynalloc4 slicing-dynalloc4.sh)
	add_test(slicing-dynalloc5 slicing-dynalloc5.sh)
	add_test(slicing-dynalloc6 slicing-dynalloc6.sh)
	add_test(slicing-dynalloc7 slicing-dynalloc7.sh)
	add_test(slicing-realloc1 slicing-realloc1.sh)
	add_test(slicing-realloc2 slicing-realloc2.sh)
	add_test(slicing-switch1 slicing-switch1.sh)
	add_test(slicing-switch2 slicing-switch2.sh)
	add_test(slicing-vararg1 slicing-vararg1.sh)
	add_test(slicing-vararg2 slicing-vararg2.sh)
	add_test(slicing-vararg3 slicing-vararg3.sh)
	add_test(slicing-vararg4 slicing-vararg4.sh)
	add_test(slicing-negoffset1 slicing-negoffset1.sh)
	add_test(slicing-negoffset2 slicing-negoffset2.sh)
	add_test(slicing-negoffset3 slicing-negoffset3.sh)
	add_test(slicing-sum1 slicing-sum1.sh)
	add_test(slicing-sum2 slicing-sum2.sh)
	add_test(slicing-sum3 slicing-sum3.sh)
	add_test(control-regression1 slicing-control-regression1.sh)
	add_test(pta_fs_regression1 slicing-pta_fs_regression1.sh)
	add_test(pta_regression2 slicing-pta_regression2.sh)
	add_test(alias_of_return slicing-alias_of_return.sh)
	add_test(regression1 slicing-regression1.sh)
	add_test(fptoui slicing-fptoui1.sh)
	add_test(malloc-redef slicing-malloc-redef.sh)
	add_test(globalptr1 slicing-globalptr1.sh)
	add_test(globalptr2 slicing-globalptr2.sh)
	add_test(globalptr3 slicing-globalptr3.sh)
	add_test(globalptr4 slicing-globalptr4.sh)
	add_test(pta-inv-infinite-loop pta-inv-infinite-loop.sh)

endif (LLVM_DG)

# --------------------------------------------------
# benchmarking
# --------------------------------------------------
add_executable(rdmap-benchmark rdmap-benchmark.cpp)
target_link_libraries(rdmap-benchmark RD)

add_executable(ptset-benchmark ptset-benchmark.cpp)
target_link_libraries(ptset-benchmark PRIVATE DGAnalysis)

