// MatchReviewScreen.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    visible: true
    width: 1000
    height: 660
    title: "Chess Reviewer"
    color: "#463F3A"

    property int matchReviewCurrentBlunderIndex: 0
    property var matchBlunders: []

    // storage of coordinates about blunder and alternative
    // best move being shown so board squares can know if
    // they need to be red or green instead of their normal
    // colour
    property int blunderFromX: 0
    property int blunderFromY: 0
    property int blunderToX: 0
    property int blunderToY: 0

    property int bestMoveFromX: 0
    property int bestMoveFromY: 0
    property int bestMoveToX: 0
    property int bestMoveToY: 0

    // enables/d
    property bool next_blunder_available: false
    property bool previous_blunder_available: false

    // updated the board GUI to show a new blunder move, is called
    // whenever the "Next" or "Back" buttons are pressed to navigate
    // through the blunders they made during the match
    function updateGUIBoardBlunder() {
        var currentBlunder = matchBlunders[matchReviewCurrentBlunderIndex]
        var updated1dGUIChessBoard = currentBlunder["boardBeforeBlunder"]

        // converts received 1d board into a 2d board so it can be displayed by the GUI
        var updatedGUIChessBoard = [];
        for (var y = 0; y < 8; y++) {
            updatedGUIChessBoard.push(
                        updated1dGUIChessBoard.slice(y * 8, (y + 1) * 8) )
        }
        guiChessBoard.guiChessBoardOutputGrid = updatedGUIChessBoard

        // makes info about the current blunder being shown accessible to the board GUI
        // so the board GUI squares can work out what colour they need to be
        blunderFromX = currentBlunder["blunder_from_x"]
        blunderFromY = currentBlunder["blunder_from_y"]
        blunderToX = currentBlunder["blunder_to_x"]
        blunderToY = currentBlunder["blunder_to_y"]

        bestMoveFromX = currentBlunder["best_from_x"]
        bestMoveFromY = currentBlunder["best_from_y"]
        bestMoveToX = currentBlunder["best_to_x"]
        bestMoveToY = currentBlunder["best_to_y"]

        next_blunder_available = matchReviewCurrentBlunderIndex < matchBlunders.length - 1
        previous_blunder_available = matchReviewCurrentBlunderIndex > 0
    }

    // updates the board gui as soon as the screen has finished
    // loading in order to show the first blunder made by the user
    Component.onCompleted: {
        updateGUIBoardBlunder()
    }

    RowLayout {
        anchors.centerIn: parent
        spacing: 20
        Layout.alignment: Qt.AlignVCenter

        // Blunder Next/Back Nav Section
        Rectangle {
            Layout.preferredWidth: 185
            Layout.preferredHeight: 180
            color: "#E0AFA0"
            radius: 8
            ColumnLayout {
                anchors.centerIn: parent
                spacing: 10
                Layout.fillWidth : true
                Layout.fillHeight : true
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Worst moves you played"
                    font.pixelSize: 16
                    color: "#1A1A1A"
                }
                // Button to go to next blunder
                Button {
                    Layout.alignment: Qt.AlignHCenter
                    text: next_blunder_available ? "Next" : "No more blunders"
                    font.bold: true
                    Layout.preferredWidth: 170
                    Layout.preferredHeight: 55
                    font.pixelSize: next_blunder_available ? 25 : 15
                    background: Rectangle {
                        color: "#F4F3EE"
                        radius: 10
                    }
                    enabled: next_blunder_available
                    onClicked: {
                        matchReviewCurrentBlunderIndex += 1
                        updateGUIBoardBlunder()
                    }
                }
                // Button to go to previous blunder
                Button {
                    Layout.alignment: Qt.AlignHCenter
                    text: previous_blunder_available ? "Back" : "No previous blunders"
                    font.bold: true
                    Layout.preferredWidth: 170
                    Layout.preferredHeight: 55
                    font.pixelSize: previous_blunder_available ? 25 : 15
                    background: Rectangle {
                        color: "#F4F3EE"
                        radius: 10
                    }
                    enabled: previous_blunder_available
                    onClicked: {
                        matchReviewCurrentBlunderIndex -= 1
                        updateGUIBoardBlunder()
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
            property var guiChessBoardOutputGrid: []

            Repeater {
                model: 64
                Button {
                    property int row: Math.floor(index / 8)
                    property int col: index % 8

                    width: 70
                    height: 70
                    text: guiChessBoard.guiChessBoardOutputGrid[row][col]
                    font.pixelSize: 58
                    background: Rectangle {
                        color: {
                            if (row === blunderFromY && col === blunderFromX)
                                return "#FF0000"; // red for blunder start board square
                            else if (row === blunderToY && col === blunderToX)
                                return "#FF0000"; // red for blunder end board square
                            else if (row === bestMoveFromY && col === bestMoveFromX) {
                                return (blunderFromX === bestMoveFromX && blunderFromY === bestMoveFromY)
                                       ? "#FF0000" // red if same as blunder start board square
                                       : "#00FF00"; // bright green for best move start board square
                            }
                            else if (row === bestMoveToY && col === bestMoveToX)
                                return "#00FF00"; // bright green for best move end board square
                            else
                                return ((row + col) % 2 === 0) ? "#EEEED2" : "#769656"; // normal board square colours
                                // alternating between dark green and cream as is standard for chess boards
                        }
                        border.color: "#2A2623"
                        border.width: 0.5
                    }
                }
            }
        }

        // Blunder Severity Bar Section
        Column {
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Blunder Severity"
                font.pixelSize: 17
                color: "#F4F3EE"
            }

            Rectangle {
                width: 46
                height: 400
                color: "#FFFFFF"
                radius: 12
                border.color: "#35302C"
                border.width: 3
                anchors.horizontalCenter: parent.horizontalCenter
                Rectangle {
                    id: severityBar
                    width: 40
                    height: matchBlunders[matchReviewCurrentBlunderIndex]["severity"] / 100 * 394
                    color: "#E03535"
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    radius: 8
                    anchors.bottomMargin: 3
                }
            }
        }
    }
}
