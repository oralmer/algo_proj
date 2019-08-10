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

#set( boost_URL "http://sourceforge.net/projects/boost/files/boost/1.63.0/boost_1_63_0.tar.bz2" )
#set( boost_SHA1 "9f1dd4fa364a3e3156a77dc17aa562ef06404ff6" )
#set( boost_INSTALL ${CMAKE_CURRENT_BINARY_DIR}/third_party/boost )
#set( boost_INCLUDE_DIR ${boost_INSTALL}/include )
#set( boost_LIB_DIR ${boost_INSTALL}/lib )

#ExternalProject_Add( external_boost
#        PREFIX boost
#        URL ${boost_URL}
#        URL_HASH SHA1=${boost_SHA1}
#        BUILD_IN_SOURCE 1
#        CONFIGURE_COMMAND ./bootstrap.sh
#        --with-libraries=filesystem
#        --with-libraries=system
#        --with-libraries=date_time
#        --with-libraries=program_options
#        --prefix=<INSTALL_DIR>
#        BUILD_COMMAND
#        ./b2 install link=static variant=release threading=multi runtime-link=static
#        INSTALL_COMMAND ""
#        INSTALL_DIR ${boost_INSTALL} )
#
#set( boost_LIBRARY_SUFFIX .a )
#
#add_library( boost::date_time STATIC IMPORTED )
#set_property( TARGET boost::date_time PROPERTY IMPORTED_LOCATION ${boost_LIB_DIR}/libboost_date_time${boost_LIBRARY_SUFFIX} )
#set_property( TARGET boost::date_time PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${boost_INCLUDE_DIR} )
#add_dependencies( boost::date_time external_boost )
#
#add_library( boost::system STATIC IMPORTED )
#set_property( TARGET boost::system PROPERTY IMPORTED_LOCATION ${boost_LIB_DIR}/libboost_system${boost_LIBRARY_SUFFIX} )
#set_property( TARGET boost::system PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${boost_INCLUDE_DIR} )
#add_dependencies( boost::system external_boost )
#
#add_library( boost::filesystem STATIC IMPORTED )
#set_property( TARGET boost::filesystem PROPERTY IMPORTED_LOCATION ${boost_LIB_DIR}/libboost_filesystem${boost_LIBRARY_SUFFIX} )
#set_property( TARGET boost::filesystem PROPERTY INTERFACE_LINK_LIBRARIES boost::system )
#set_property( TARGET boost::filesystem PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${boost_INCLUDE_DIR} )
#add_dependencies( boost::filesystem external_boost )

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
        hashing/HashHelperFuncs.h password_generation/GenerationHelperFuncs.cpp password_generation/GenerationHelperFuncs.h hashing/WPA3KeyDeriver.cpp hashing/WPA3KeyDeriver.h)

add_dependencies(PassGen jspirit)
target_link_libraries(PassGen gmpxx gmp ${OPENSSL_LIBRARIES})

add_executable(FirstTest tests/PassGenTest.cpp tests/HasherTest.cpp tests/PreformanceTest.cpp test_resources/PerformanceIn/GenerationIn.h test_resources/PassGenIn/bigDict_in.h test_resources/PerformanceIn/BigDictIn.h test_resources/PassGenOut/bigDict_out.h test_resources/HasherIn/ModPHasher_in.h)
add_dependencies(FirstTest googletest jspirit PassGen)
target_link_libraries(FirstTest gtest gtest_main PassGen)

add_executable(PassBreaker main.cpp)
add_dependencies(PassBreaker jspirit PassGen)
target_link_libraries(PassBreaker PassGen)
#target_link_libraries(cpp_src boost::filesystem )