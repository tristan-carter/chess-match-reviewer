cmake_minimum_required(VERSION 3.16)
project(chess-match-reviewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2 Qml Gui)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appchess-match-reviewer
    main.cpp
    Bishop.cpp
    BoardStructure.cpp
    King.cpp
    Knight.cpp
    MatchReview.cpp
    Pawn.cpp
    Piece.cpp
    PieceSquareTables.cpp
    Queen.cpp
    Rook.cpp
    resources.qrc
)

qt_add_qml_module(appchess-match-reviewer
    URI com.tristancarter.chessmatchreviewer
    VERSION 1.0
    QML_FILES
        EnterMatchToReviewScreen.qml
        MatchReviewScreen.qml
)

set_target_properties(appchess-match-reviewer PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appchess-match-reviewer
    PRIVATE
        Qt6::Quick
        Qt6::Qml
        Qt6::Gui
        Qt6::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS appchess-match-reviewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
