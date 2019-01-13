include_directories(.)

add_library(catch_main catch_main.cpp)
set_target_properties(catch_main PROPERTIES FOLDER "tests")

SET( TESTS_LIST
  data_provider.cpp
  str_utils.cpp
  lexer.cpp
  simple_templates.cpp
  json.cpp
)


add_executable(all_tests ${TESTS_LIST})
target_link_libraries(all_tests catch_main Cpl_tmpl::Cpl_tmpl)

set_target_properties(all_tests PROPERTIES FOLDER "tests")
add_test(all_tests all_tests)

if(MSVC)
  target_compile_options(all_tests PRIVATE /W4 /WX)
else(MSVC)
  target_compile_options(all_tests PRIVATE -Wall -Wextra -pedantic -Werror)
endif(MSVC)