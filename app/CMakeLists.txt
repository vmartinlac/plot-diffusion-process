add_executable(plotdiffusionprocess main.cpp Window.cpp)
target_link_libraries(plotdiffusionprocess PUBLIC qcustomplot Qt5::Widgets MuParser)
