cmake_minimum_required(VERSION 3.16)
project(rebind-tests LANGUAGES CXX)

# --------------------------------------------------------------------------------------------------------
# Create executable
# --------------------------------------------------------------------------------------------------------

add_executable(${PROJECT_NAME})
add_executable(rebind::tests ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS OFF CXX_STANDARD_REQUIRED ON)

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
  # https://github.com/boost-ext/ut/pull/572 is required for alpine
  GIT_TAG        99f7c532466a2c559a5c82df0c495203c3bb3950
  GIT_REPOSITORY "https://github.com/R2RT/ut"
  OPTIONS        "BOOST_UT_DISABLE_MODULE ON"
)

target_link_libraries(${PROJECT_NAME} PRIVATE Boost::ut cr::rebind)

# --------------------------------------------------------------------------------------------------------
# Add Test-Target
# --------------------------------------------------------------------------------------------------------

include(CTest)

add_test(NAME rebind-tests COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
