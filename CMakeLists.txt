cmake_minimum_required(VERSION 3.0.2)
project(bumpybot_teleop)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  rviz
  std_msgs
)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(QT_LIBRARIES Qt5::Widgets)

catkin_package(
        LIBRARIES ${PROJECT_NAME}
        CATKIN_DEPENDS roscpp rviz std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES bumpybot_teleop
#  CATKIN_DEPENDS roscpp rviz std_msgs
#  DEPENDS system_lib
)


include_directories(include
        ${catkin_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        )

link_directories(
        ${catkin_LIBRARY_DIRS}
        ${Boost_LIBRARY_DIRS}
)

add_definitions(-DQT_NO_KEYWORDS)

QT5_WRAP_CPP(MOC_FILES
        src/wheel_control_pad.h
        src/teleop_pad.h
        OPTIONS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED -DBOOST_LEXICAL_CAST_INCLUDED
        )

set(SOURCE_FILES
        src/wheel_control_pad.cpp
        src/teleop_pad.cpp
        ${MOC_FILES}
        )

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${catkin_LIBRARIES})

#install(TARGETS
#        ${PROJECT_NAME}
#        ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
#        )
#
#install(TARGETS
#        ${PROJECT_NAME}
#        ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
#        )
#
#install(FILES
#        wheel_teleop_plugin.xml
#        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_bumpybot_teleop.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
