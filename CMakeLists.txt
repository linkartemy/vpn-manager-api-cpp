cmake_minimum_required(VERSION 3.14)
project(vpn_manager CXX)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "" FORCE)
# set(USERVER_FEATURE_CRYPTOPP_BLAKE2 OFF CACHE BOOL "" FORCE)
# set(USERVER_FEATURE_GRPC_CHANNELZ OFF CACHE BOOL "" FORCE)
# set(USERVER_FEATURE_REDIS_HI_MALLOC ON CACHE BOOL "" FORCE)

# # Добавляем userver как поддиректорию
# if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/userver")
#     message(STATUS "Using userver framework from third_party/userver")
#     add_subdirectory(third_party/userver)
# else()
#     message(FATAL_ERROR "userver submodule is missing. Make sure to initialize it with `git submodule update --init --recursive`.")
# endif()

# userver_setup_environment()

# add_library(${PROJECT_NAME}_objs OBJECT
#     src/constants.hpp

#     src/models/user_dto.hpp
#     src/models/exception.hpp

#     src/repositories/user_repository/user_repository.hpp
#     src/repositories/user_repository/user_repository.cpp

#     src/handlers/v1/user/get-by-username/view.hpp
#     src/handlers/v1/user/get-by-username/view.cpp
# )

# target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver-core userver-postgresql)

# # Основной исполняемый файл
# add_executable(${PROJECT_NAME} src/main.cpp)
# target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)

# # Поддержка тестов (если требуется)
# userver_testsuite_add_simple()

# # Тестовый исполняемый файл
# add_executable(${PROJECT_NAME}_unittest)
# target_link_libraries(${PROJECT_NAME}_unittest PRIVATE ${PROJECT_NAME}_objs userver::utest)
# add_google_tests(${PROJECT_NAME}_unittest)

# # Установка
# include(GNUInstallDirs)

# if(DEFINED ENV{PREFIX})
#     message(STATUS "Set install prefix: $ENV{PREFIX}")
#     file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
#     set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
# endif()

# # Установить конфигурационные файлы
# file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)
# install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
# install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})




# OLD

# Adding userver dependency
find_package(userver COMPONENTS core postgresql REQUIRED)
if(NOT userver_FOUND)  # Fallback to subdirectory usage
    # Enable userver libraries that are needed in this project
    set(USERVER_FEATURE_POSTGRESQL ON CACHE BOOL "" FORCE)

    # Compatibility mode: some systems don't support these features
    set(USERVER_FEATURE_CRYPTOPP_BLAKE2 OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_GRPC_CHANNELZ OFF CACHE BOOL "" FORCE)
    set(USERVER_FEATURE_REDIS_HI_MALLOC ON CACHE BOOL "" FORCE)

    if (EXISTS third_party/userver)
        message(STATUS "Using userver framework from third_party/userver")
        add_subdirectory(third_party/userver)
    else()
        message(FATAL_ERROR "Either install the userver or provide a path to it")
    endif()
endif()

userver_setup_environment()

# Common sources
add_library(${PROJECT_NAME}_objs OBJECT
        src/constants.hpp

        src/models/user_dto.hpp
        src/models/exception.hpp

        src/repositories/user_repository/user_repository.hpp
        src/repositories/user_repository/user_repository.cpp

        src/handlers/v1/user/get-by-username/view.hpp
        src/handlers/v1/user/get-by-username/view.cpp
)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver::postgresql)


# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Unit Tests
add_executable(${PROJECT_NAME}_unittest)
target_link_libraries(${PROJECT_NAME}_unittest PRIVATE ${PROJECT_NAME}_objs userver::utest)
add_google_tests(${PROJECT_NAME}_unittest)


# Benchmarks
add_executable(${PROJECT_NAME}_benchmark)
target_link_libraries(${PROJECT_NAME}_benchmark PRIVATE ${PROJECT_NAME}_objs userver::ubench)
add_google_benchmark_tests(${PROJECT_NAME}_benchmark)


# Functional Tests
userver_testsuite_add_simple()


# Install
include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
  message(STATUS "Set install prefix: $ENV{PREFIX}")
  file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
  set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
endif()

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
