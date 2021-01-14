set(RUB_DSP_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/biquad.cpp"
)

set(RUB_DSP_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_library(rubdsp ${RUB_DSP_SRCS})
target_include_directories(rubdsp PUBLIC ${RUB_DSP_INCLUDES})