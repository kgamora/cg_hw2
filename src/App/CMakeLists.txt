set(SRCS
    main.cpp
    mainwindow.cpp
    mainwindow.h
    Window.cpp
    Window.h
    camera.cpp
    camera.h
    tiny_gltf.h
    glm

    Shaders/diffuse.fs
    Shaders/diffuse.vs
    Textures/voronoi.png

    resources.qrc
)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(demo-app ${SRCS})

target_link_libraries(demo-app
    PRIVATE
        Qt5::Widgets
        FGL::Base
        thirdparty::tinygltf
        thirdparty::glm
)
