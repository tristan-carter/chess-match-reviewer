cmake_minimum_required(VERSION 3.16)

project(chess-match-reviewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appchess-match-reviewer
    main.cpp
)

qt_add_qml_module(appchess-match-reviewer
    URI com.tristancarter.chessmatchreviewer
    VERSION 1.0
    QML_FILES
    EnterMatchToReviewScreen.qml
    QML_FILES
    SOURCES
    SOURCES BoardStructure.h
    SOURCES BoardStructure.cpp
    SOURCES BoardCell.h
    SOURCES Piece.h
    SOURCES Piece.cpp
    SOURCES Move.h
    SOURCES PastMove.h
    SOURCES Coord.h
    SOURCES PieceSquareTables.cpp
    SOURCES Pawn.h
    SOURCES Bishop.h
    SOURCES King.h
    SOURCES Knight.h
    SOURCES Queen.h
    SOURCES Rook.h
    SOURCES Bishop.cpp
    SOURCES King.cpp
    SOURCES Knight.cpp
    SOURCES Pawn.cpp
    SOURCES Queen.cpp
    SOURCES Rook.cpp
    SOURCES MatchReview.h MatchReview.cpp
    QML_FILES MatchReviewScreen.qml
    QML_FILES Theme.qml
    QML_FILES StyledPopup.qml
    QML_FILES PrimaryButton.qml
    RESOURCES qmldir
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appchess-match-reviewer PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appchess-match-reviewer
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
