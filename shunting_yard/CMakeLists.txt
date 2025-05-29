cmake_minimum_required(VERSION 4.0)
project(ShuntingYard)

add_executable(${PROJECT_NAME}
  ./main.c
  ./shunting_yard.c
)

target_link_libraries(${PROJECT_NAME}
  m
)
