# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(RaindropTests LANGUAGES CXX)
enable_testing()

# ---- Dependencies ----

find_package(GTest CONFIG REQUIRED)

add_subdirectory(UnitTests)

add_folders(Test)