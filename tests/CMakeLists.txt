cmake_minimum_required(VERSION 3.25)
project(rebind-tests LANGUAGES CXX)

# --------------------------------------------------------------------------------------------------------
# Create executable
# --------------------------------------------------------------------------------------------------------

add_executable(${PROJECT_NAME})
add_executable(rebind::tests ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23 CXX_EXTENSIONS OFF CXX_STANDARD_REQUIRED ON)

# --------------------------------------------------------------------------------------------------------
# Add Sources
# --------------------------------------------------------------------------------------------------------

file(GLOB src "*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${src})

# --------------------------------------------------------------------------------------------------------
# Link Dependencies 
# --------------------------------------------------------------------------------------------------------

include("../cmake/cpm.cmake")

CPMFindPackage(
  NAME           ut
  VERSION        2.3.1
  GIT_REPOSITORY "https://github.com/boost-ext/ut"
  OPTIONS        "BOOST_UT_DISABLE_MODULE ON"
)

target_link_libraries(${PROJECT_NAME} PRIVATE Boost::ut cr::rebind)

# --------------------------------------------------------------------------------------------------------
# Add Test-Target
# --------------------------------------------------------------------------------------------------------

include(CTest)

add_test(NAME rebind-tests COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
