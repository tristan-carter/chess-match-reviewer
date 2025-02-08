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

    Popup {
        id: chooseUserSide
        modal: true
        focus: true
        anchors.centerIn: parent
        width: 210
        height: 140
        background: Rectangle {
            color: "#E0AFA0"
            radius: 8
        }
        Column {
            width: 195
            anchors.centerIn: parent
            spacing: 11
            Text {
                Layout.alignment: Qt.AlignHCenter
                text: "Select the side you played during the match you would like to review"
                wrapMode: Text.WordWrap
                width: 190
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#1A1A1A"
            }
            Row {
                spacing: 20
                anchors.horizontalCenter: parent.horizontalCenter
                Button {
                    Layout.alignment: Qt.AlignHCenter
                    text: "White"
                    width: 85
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
                    Layout.alignment: Qt.AlignHCenter
                    text: "Black"
                    width: 85
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

    RowLayout {
        anchors.centerIn: parent
        spacing: 20
        Layout.alignment: Qt.AlignVCenter

        // Start Review Section
        Rectangle {
            Layout.preferredWidth: 185
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
                    Layout.preferredWidth: 170
                    Layout.preferredHeight: 55
                    font.pixelSize: 25
                    background: Rectangle {
                        color: "#F4F3EE"
                        radius: 10
                    }
                    onClicked: console.log("Review Started!")
                }
            }
        }

        // Chess Board Section
        Grid {
            id: guiChessBoard
            rows: 8
            columns: 8
            spacing: 0

            property var boardState: [
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
            property int selectedRow: -1
            property int selectedCol: -1

            Repeater {
                model: 64
                Button {
                    width: 70
                    height: 70
                    text: guiChessBoard.boardState[Math.floor(index / 8)][index % 8]
                    font.pixelSize: 58
                    background: Rectangle {
                        color: (index + Math.floor(index / 8)) % 2 === 0 ? "#EEEED2" : "#769656"
                        border.color: "#2A2623"
                        border.width: 0.5
                    }
                    onClicked: {
                        let row = Math.floor(index / 8)
                        let col = index % 8

                        if (guiChessBoard.selectedPiece === null) {
                            if (text !== "") {
                                guiChessBoard.selectedPiece = text
                                guiChessBoard.selectedRow = row
                                guiChessBoard.selectedCol = col
                            }
                        } else {
                            let newBoardFlat = match_review.enter_move_from_match(col, 7-row, guiChessBoard.selectedCol, 7-guiChessBoard.selectedRow);

                            let newBoard = [];
                            for (let y = 0; y < 8; y++) {
                                newBoard.push(newBoardFlat.slice(y * 8, (y + 1) * 8));
                            }

                            guiChessBoard.boardState = newBoard;

                            guiChessBoard.selectedPiece = null
                            guiChessBoard.selectedRow = -1
                            guiChessBoard.selectedCol = -1

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
                    text: "Replay the match you would like to review on the chess board before pressing start review. Move pieces by clicking a piece then clicking the square to move it to."
                    font.pixelSize: 16
                    color: "#1A1A1A"
                }
            }
        }
    }
}
