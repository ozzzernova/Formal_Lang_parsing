include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(Library SHARED ${PROJECT_SOURCE_DIR}/src/Earley.cpp ${PROJECT_SOURCE_DIR}/src/Grammar.cpp)