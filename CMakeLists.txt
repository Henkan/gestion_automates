cmake_minimum_required(VERSION 3.14)
project(Algorithmique)

set(CMAKE_CXX_STANDARD 17)

add_executable(lanceur main.cpp Automaton.cpp Automaton.h State.cpp State.h Transition.cpp Transition.h utils.h utils.cpp)