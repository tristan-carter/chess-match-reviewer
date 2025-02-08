cmake_minimum_required(VERSION 3.16)

project(ALevelNEAChessReviewer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appALevelNEAChessReviewer
    main.cpp
)

qt_add_qml_module(appALevelNEAChessReviewer
    URI ALevelNEAChessReviewer
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
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appALevelNEAChessReviewer PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appALevelNEAChessReviewer
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appALevelNEAChessReviewer
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appALevelNEAChessReviewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
