
cmake_minimum_required ( VERSION 3.11 )

project ( RTWeekendCpp)

# Set to c++11
set ( CMAKE_CXX_STANDARD 11 )

# Source

set ( COMMON_ALL
  common/rtweekend.h
  common/camera.h
  common/ray.h
  common/vec3.h
)



set ( SOURCE_NEXT_WEEK
  ${COMMON_ALL}
  common/aabb.h
  common/external/stb_image.h
  common/perlin.h
  common/rtw_stb_image.h
  common/texture.h
  TheNextWeek/aarect.h
  TheNextWeek/box.h
  TheNextWeek/bvh.h
  TheNextWeek/constant_medium.h
  TheNextWeek/hittable.h
  TheNextWeek/hittable_list.h
  TheNextWeek/material.h
  TheNextWeek/moving_sphere.h
  TheNextWeek/sphere.h
  TheNextWeek/main.cc
)



# Executables

add_executable(theNextWeekCpp  ${SOURCE_NEXT_WEEK})



target_include_directories(theNextWeekCpp  PRIVATE .)

