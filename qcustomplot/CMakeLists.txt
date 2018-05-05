add_library(qcustomplot STATIC qcustomplot.cpp)
target_link_libraries(qcustomplot PUBLIC Qt5::Widgets Qt5::PrintSupport)
target_include_directories(qcustomplot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
