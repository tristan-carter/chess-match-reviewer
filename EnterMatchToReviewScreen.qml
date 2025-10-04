// EnterMatchToReviewScreen.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    visible: true
    width: 1000
    height: 660
    title: "Chess Reviewer"
    color: "#463F3A"

    // Pop up shown when chess reviewer application is opened, allows user to
    // select which side they played during the match they want to review
    Popup {
        id: chooseUserSide
        modal: true
        focus: true
        anchors.centerIn: parent
        width: 260
        height: 140
        background: Rectangle {
            color: "#E0AFA0"
            radius: 8
        }
        Column {
            width: 230
            anchors.centerIn: parent
            spacing: 11
            Text {
                Layout.alignment: Qt.AlignHCenter
                text: "Select the side you played during the match you would like to review"
                wrapMode: Text.WordWrap
                width: 230
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#1A1A1A"
            }
            Row {
                spacing: 20
                anchors.horizontalCenter: parent.horizontalCenter
                Button {
                    text: "White"
                    width: 110
                    height: 47
                    font.bold: true
                    font.pixelSize: 25
                    background: Rectangle {
                        color: "#F4F3EE"
                        radius: 8
                    }
                    onClicked: {
                        match_review.enter_user_side(0)
                        chooseUserSide.close()
                    }
                }

                Button {
                    text: "Black"
                    width: 110
                    height: 47
                    font.bold: true
                    font.pixelSize: 25
                    background: Rectangle {
                        color: "#F4F3EE"
                        radius: 8
                    }
                    onClicked: {
                        match_review.enter_user_side(1)
                        chooseUserSide.close()
                    }
                }

            }
        }
    }
    // opens pop up for user to select side played during match once GUI
    // screen for revieweing has loaded
    Component.onCompleted: chooseUserSide.open()

    // Pop up for selecting which type of piece to promote a pawn to once it reaches
    // the end of the board
    Popup {
        id: choosePromotedPieceType
        modal: true
        focus: true
        anchors.centerIn: parent
        width: 320
        height: 125
        background: Rectangle {
            color: "#E0AFA0"
            radius: 8
        }
        Column {
            anchors.centerIn: parent
            spacing: 11
            Text {
                text: "Select the piece to promote your pawn to"
                font.pixelSize: 16
                color: "#1A1A1A"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Row {
                spacing: 10
                anchors.horizontalCenter: parent.horizontalCenter
                // promote to bishop
                Button {
                    text: "♗"
                    font.pixelSize: 38
                    width: 50
                    height: 50
                    background: Rectangle {
                        color: "#F4F3EE"
                        radius: 10
                    }
                    onClicked: {
                        callEnterMoveFromMatch(2)
                        choosePromotedPieceType.close()
                    }
                }
                // promote to knight
                Button {
                    text: "♘"
                    font.pixelSize: 38
                    width: 50
                    height: 50
                    background: Rectangle {
                        color: "#F4F3EE"
                        radius: 10
                    }
                    onClicked: {
                        callEnterMoveFromMatch(3)
                        choosePromotedPieceType.close()
                    }
                }
                // promote to rook
                Button {
                    text: "♖"
                    font.pixelSize: 38
                    width: 50
                    height: 50
                    background: Rectangle {
                        color: "#F4F3EE"
                        radius: 10
                    }
                    onClicked: {
                        callEnterMoveFromMatch(4)
                        choosePromotedPieceType.close()
                    }
                }
                // promote to queen
                Button {
                    text: "♕"
                    font.pixelSize: 38
                    width: 50
                    height: 50
                    background: Rectangle {
                        color: "#F4F3EE"
                        radius: 10
                    }
                    onClicked: {
                        guiChessBoard.callEnterMoveFromMatch(5)
                        choosePromotedPieceType.close()
                    }
                }
            }
        }
    }

    // Popup shown if user presses start review but there are no blunders
    Popup {
        id: noBlundersInMatch
        modal: true
        focus: true
        anchors.centerIn: parent
        width: 210
        height: 145
        background: Rectangle {
            color: "#E0AFA0"
            radius: 8
        }
        Column {
            width: 205
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 11
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "None of your moves were blunders, well done!"
                wrapMode: Text.WordWrap
                width: 190
                font.pixelSize: 17
                horizontalAlignment: Text.AlignHCenter
                color: "#1A1A1A"
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Go back"
                width: 170
                height: 52
                font.bold: true
                font.pixelSize: 25
                background: Rectangle {
                    color: "#F4F3EE"
                    radius: 8
                }
                onClicked: {
                    noBlundersInMatch.close()
                }
            }
        }
    }

    // Horizontally lays out the 3 sections of the
    // screen (start button, board and explanation of the screen)
    RowLayout {
        anchors.centerIn: parent
        spacing: 20
        Layout.alignment: Qt.AlignVCenter

        // Start Review Section
        Rectangle {
            Layout.preferredWidth: 220
            Layout.preferredHeight: 103
            color: "#E0AFA0"
            radius: 8
            ColumnLayout {
                anchors.centerIn: parent
                spacing: 10
                Layout.fillWidth : true
                Layout.fillHeight : true
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Finished entering match?"
                    font.pixelSize: 16
                    color: "#1A1A1A"
                }
                Button {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Start Review"
                    font.bold: true
                    Layout.preferredWidth: 190
                    Layout.preferredHeight: 55
                    font.pixelSize: 25
                    background: Rectangle {
                        color: "#F4F3EE"
                        radius: 10
                    }
                    onClicked: {
                        var matchBlunders = match_review.find_blunders()

                        if (matchBlunders.length > 0) {
                            var component = Qt.createComponent("qrc:/MatchReviewScreen.qml")
                            if (component.status === Component.Ready) {
                                var matchReviewScreen = component.createObject(parent, {
                                    matchBlunders: matchBlunders
                                })

                                if (!matchReviewScreen) {
                                    console.error("ERROR - matchReviewScreen failed to create")
                                    return
                                }

                                matchReviewScreen.show()
                            } else {
                                console.error("ERROR - MatchReviewScreen component failed to load:", component.errorString())
                            }
                        } else {
                            noBlundersInMatch.open()
                        }
                    }

                }
            }
        }

        // Chess Board Section
        Grid {
            id: guiChessBoard
            rows: 8
            columns: 8
            spacing: 0
            property var guiChessBoardOutputGrid: [
                ["♜", "♞", "♝", "♛", "♚", "♝", "♞", "♜"],
                ["♟", "♟", "♟", "♟", "♟", "♟", "♟", "♟"],
                ["", "", "", "", "", "", "", ""],
                ["", "", "", "", "", "", "", ""],
                ["", "", "", "", "", "", "", ""],
                ["", "", "", "", "", "", "", ""],
                ["♙", "♙", "♙", "♙", "♙", "♙", "♙", "♙"],
                ["♖", "♘", "♗", "♕", "♔", "♗", "♘", "♖"],
            ]
            property var selectedPiece: null
            property int x_from: -1
            property int y_from: -1
            property int x_to: -1
            property int y_to: -1

            function callEnterMoveFromMatch(pieceToPromotePawnTo) {
                var updated1dGUIChessBoard = match_review
                .enter_move_from_match(guiChessBoard.y_to, 7-guiChessBoard.x_to,
                guiChessBoard.y_from, 7 - guiChessBoard.x_from, pieceToPromotePawnTo)

                // converts recieved 1d board into a 2d board so it can be displayed by the GUI
                var updatedGUIChessBoard = []
                for (var y = 0; y < 8; y++) {
                    updatedGUIChessBoard.push(updated1dGUIChessBoard.slice(y * 8, (y + 1) * 8))
                }
                guiChessBoard.guiChessBoardOutputGrid = updatedGUIChessBoard

                // resets selected square
                guiChessBoard.selectedPiece = null
                guiChessBoard.x_from = -1
                guiChessBoard.y_from = -1
            }


            Repeater {
                model: 64
                Button {
                    width: 70
                    height: 70
                    text: guiChessBoard.guiChessBoardOutputGrid[Math.floor(index / 8)][index % 8]
                    font.pixelSize: 58
                    background: Rectangle {
                        color: (index + Math.floor(index / 8)) % 2 === 0 ? "#EEEED2" : "#769656"
                        border.color: "#2A2623"
                        border.width: 0.5
                    }
                    onClicked: {
                        guiChessBoard.x_to = Math.floor(index / 8)
                        guiChessBoard.y_to = index % 8
                        if (guiChessBoard.selectedPiece === null) {
                            if (text !== "") {
                                guiChessBoard.selectedPiece = text
                                guiChessBoard.x_from = guiChessBoard.x_to
                                guiChessBoard.y_from = guiChessBoard.y_to
                            }
                        } else {
                            // checks if a either a white or black is in a
                            // position where it now
                            // needs to be promoted
                            var isPawnPromoting = (guiChessBoard.selectedPiece === "♙"
                                && guiChessBoard.x_to === 0)
                                || (guiChessBoard.selectedPiece === "♟"
                                && guiChessBoard.x_to === 7)

                            if (isPawnPromoting) {
                                choosePromotedPieceType.open()
                            } else {
                                guiChessBoard.callEnterMoveFromMatch(0)
                            }
                        }
                    }
                }
            }
        }

        // Explanation Of How To Enter A Match Section
        Rectangle {
            Layout.preferredWidth: 185
            Layout.preferredHeight: 195
            color: "#BCB8B1"
            radius: 12
            Rectangle {
                anchors.centerIn: parent
                color: "#F4F3EE"
                radius: 11
                width: parent.width - 10
                height: parent.height - 10
                Text {
                    anchors.centerIn: parent
                    width: parent.width - 20
                    wrapMode: Text.WordWrap
                    text: "Replay the match you would like to review on the chess board before "
                          + "pressing start review. Move pieces by clicking a piece then clicking "
                          + "the square to move it to." // separated into
                    // multiple lines so code is more readable
                    font.pixelSize: 16
                    color: "#1A1A1A"
                }
            }
        }
    }
}
