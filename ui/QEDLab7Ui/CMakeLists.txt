cmake_minimum_required(VERSION 3.19)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()


qt_add_executable(QEDLab7Ui
WIN32 MACOSX_BUNDLE
main.cpp
qed.cpp
qed.h
qed.ui
task1qed.h task1qed.cpp task1qed.ui
task2qed.h task2qed.cpp task2qed.ui
task3qed.h task3qed.cpp task3qed.ui
dirty_wrapper.h
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
target_compile_definitions(QEDLab7Ui PRIVATE DEBUG)
endif(CMAKE_BUILD_TYPE STREQUAL Debug)

target_link_libraries(QEDLab7Ui
    PRIVATE
        Qt::Core
        Qt::Widgets
)

target_include_directories(QEDLab7Ui
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

include(GNUInstallDirs)

install(TARGETS QEDLab7Ui
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET QEDLab7Ui
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
