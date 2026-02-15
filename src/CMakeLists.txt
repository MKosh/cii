add_executable(test test.c)

add_library(cii stack.c arena.c atom.c assert.c except.c mem.c types.c)

target_compile_definitions(test PUBLIC
  "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_compile_definitions(cii PUBLIC
  "$<$<CONFIG:DEBUG>:DEBUG>"
)

target_compile_options(cii PRIVATE
  -fsanitize=address,leak
  -static-libasan
  -fno-omit-frame-pointer
)

target_link_options(cii PRIVATE
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
  cii
  m
)
