add_executable(test test.c)

add_library(stack stack.c)

target_compile_definitions(test PUBLIC
  "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_compile_definitions(stack PUBLIC
  "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_compile_options(stack PRIVATE
  -fsanitize=address,leak
  -static-libasan
  -fno-omit-frame-pointer
)

target_link_options(stack PRIVATE
  -fsanitize=address,leak
  -static-libasan
  -fno-omit-frame-pointer
)

target_compile_options(test PRIVATE
  -fsanitize=address,leak
  -static-libasan
  -fno-omit-frame-pointer
)

target_link_options(test PRIVATE
  -fsanitize=address,leak
  -static-libasan
  -fno-omit-frame-pointer
)

target_link_libraries(test PUBLIC
  stack
  m
)
