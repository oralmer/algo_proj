cmake_minimum_required(VERSION 3.12)
project(cpp_src)

set(CMAKE_CXX_C)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=gnu++0x -g -lgmpxx")

include(ExternalProject)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)
find_package(OpenSSL REQUIRED)
ExternalProject_Add(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG master
        SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/googletest-src"
        BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/googletest-build"
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION} -Dgtest_build_tests=OFF
        UPDATE_DISCONNECTED false
        )

ExternalProject_Add(jspirit
        GIT_REPOSITORY https://github.com/nlohmann/json
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
        -DJSON_BuildTests=OFF
        UPDATE_DISCONNECTED false
        )

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
include_directories(${OPENSSL_INCLUDE_DIR})
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

add_library(PassGen SHARED
        password_generation/DictPassGen.cpp
        password_generation/DictPassGen.h
        password_generation/PassGenFactory.cpp
        password_generation/PassGenFactory.h
        password_generation/IPassGen.h
        password_generation/CharPassGen.cpp
        password_generation/CharPassGen.h
        password_generation/CartesianPassGen.cpp
        password_generation/CartesianPassGen.h
        password_generation/OrPassGen.cpp
        password_generation/OrPassGen.h
        password_generation/BigDictPassGen.cpp
        password_generation/BigDictPassGen.h
        password_generation/GenerationHelperFuncs.cpp
        password_generation/GenerationHelperFuncs.h
        HelperFuncs.cpp
        HelperFuncs.h
        test_resources/HasherIn/EChasher_in.h
        test_resources/PassGenIn/cartesian_in.h
        test_resources/PassGenIn/dict_in.h
        test_resources/PassGenIn/or_in.h
        test_resources/PassGenIn/sanity_in.h
        test_resources/PassGenOut/cartesian_out.h
        test_resources/PassGenOut/dict_out.h
        test_resources/PassGenOut/or_out.h
        test_resources/PassGenOut/sanity_out.h
        hashing/IHasher.h
        hashing/HasherFactory.cpp
        hashing/HasherFactory.h
        hashing/IDHasher.cpp
        hashing/IDHasher.h
        hashing/ModPHasher.cpp
        hashing/ModPHasher.h
        hashing/CountTest.cpp
        hashing/CountTest.h
        hashing/EllipticCurveHasher.cpp
        hashing/EllipticCurveHasher.h
        hashing/ThresholdHasher.inl
        hashing/ThresholdHasher.h
        hashing/HashHelperFuncs.cpp
        hashing/HashHelperFuncs.h
        hashing/WPA3KeyDeriver.cpp
        hashing/WPA3KeyDeriver.h)

add_dependencies(PassGen jspirit)
target_link_libraries(PassGen gmpxx gmp ${OPENSSL_LIBRARIES})

add_executable(FirstTest tests/PassGenTest.cpp tests/HasherTest.cpp tests/PreformanceTest.cpp test_resources/PerformanceIn/GenerationIn.h test_resources/PassGenIn/bigDict_in.h test_resources/PerformanceIn/BigDictIn.h test_resources/PassGenOut/bigDict_out.h test_resources/HasherIn/ModPHasher_in.h)
add_dependencies(FirstTest googletest jspirit PassGen)
target_link_libraries(FirstTest gtest gtest_main PassGen)

add_executable(PassBreaker main.cpp)
add_dependencies(PassBreaker jspirit PassGen)
target_link_libraries(PassBreaker PassGen)
#target_link_libraries(cpp_src boost::filesystem )