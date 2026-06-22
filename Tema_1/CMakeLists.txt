cmake_minimum_required(VERSION 3.16)

# Creează executabilul Tema1Launcher cu TOT ce ai nevoie pentru Tema 1
add_executable(Tema1Launcher
    MainLauncher.cpp

    # sursele jocurilor
    src/EditorNava.cpp
    src/Componenta.cpp

    src/Breakout.cpp
    src/BreakoutBall.cpp
    src/Brick.cpp
    src/PaddleBreakout.cpp
    src/ParticleSystem.cpp

    src/ChickenInvaders.cpp
    src/CIPlayer.cpp
    src/Enemy.cpp
    src/Projectile.cpp

    src/Pong.cpp
    src/PongPlayer.cpp
    src/Ball.cpp
)

# Include-uri locale
target_include_directories(Tema1Launcher PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# C++17
target_compile_features(Tema1Launcher PRIVATE cxx_std_17)

# Platform specifics
if (WIN32)
    target_link_libraries(Tema1Launcher PRIVATE opengl32)
else()
    # Linux
    find_package(OpenGL REQUIRED)
    target_link_libraries(Tema1Launcher PRIVATE OpenGL::GL)
endif()

# GLEW și GLFW (presupunând că proiectul tău rădăcină deja le aduce sau sunt în system)
# Dacă ai target-uri deja create în proiectul mare, le poți linka aici, altfel:
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
target_link_libraries(Tema1Launcher PRIVATE GLEW::GLEW glfw)

# (Opțional) pe macOS ar mai trebui framework-uri, dar ignorăm pentru moment
