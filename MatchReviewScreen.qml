// MatchReviewScreen.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    visible: true
    width: 1000
    height: 660
    title: "Blunder Review"
    color: "#463F3A"

    // Store blunders and track the current index
    property int currentBlunderIndex: 0
    property var matchBlunders: []

    function updateBlunderDisplay() {
        if (matchBlunders.length > 0) {
            var currentBlunder = blunders[currentBlunderIndex];
            guiChessBoard.guiChessBoardOutputGrid = currentBlunder["boardBeforeBlunder"];
            severityText.text = "Blunder Severity: " + currentBlunder["severity"];

            // Highlight blundered move
            blunderSquare.x = currentBlunder["blunder_to_x"];
            blunderSquare.y = currentBlunder["blunder_to_y"];

            // Highlight best move
            bestMoveFrom.x = currentBlunder["best_from_x"];
            bestMoveFrom.y = currentBlunder["best_from_y"];
            bestMoveTo.x = currentBlunder["best_to_x"];
            bestMoveTo.y = currentBlunder["best_to_y"];
        }
    }

    RowLayout {
        anchors.centerIn: parent
        spacing: 20
        Layout.alignment: Qt.AlignVCenter

        // Left Panel - Blunder Navigation
        Column {
            spacing: 10
            Button {
                text: "Previous Blunder"
                enabled: currentBlunderIndex > 0
                onClicked: {
                    currentBlunderIndex--;
                    updateBlunderDisplay();
                }
            }
            Button {
                text: "Next Blunder"
                enabled: currentBlunderIndex < matchBlunders.length - 1
                onClicked: {
                    currentBlunderIndex++;
                    updateBlunderDisplay();
                }
            }
        }

        // Chess Board with Highlights
        Grid {
            id: guiChessBoard
            rows: 8
            columns: 8
            spacing: 0
            property var guiChessBoardOutputGrid: []

            Repeater {
                model: 64
                Rectangle {
                    width: 70
                    height: 70
                    color: (index + Math.floor(index / 8)) % 2 === 0 ? "#EEEED2" : "#769656"
                    border.color: "#2A2623"
                    border.width: 0.5

                    // Piece Text
                    Text {
                        anchors.centerIn: parent
                        text: guiChessBoard.guiChessBoardOutputGrid[Math.floor(index / 8)][index % 8]
                        font.pixelSize: 58
                    }

                    // Highlight blunder move in red
                    Rectangle {
                        id: blunderSquare
                        width: parent.width
                        height: parent.height
                        color: "red"
                        opacity: 0.5
                        visible: (Math.floor(index / 8) === blunderSquare.y) && ((index % 8) === blunderSquare.x)
                    }

                    // Highlight best move from in blue
                    Rectangle {
                        id: bestMoveFrom
                        width: parent.width
                        height: parent.height
                        color: "blue"
                        opacity: 0.5
                        visible: (Math.floor(index / 8) === bestMoveFrom.y) && ((index % 8) === bestMoveFrom.x)
                    }

                    // Highlight best move to in green
                    Rectangle {
                        id: bestMoveTo
                        width: parent.width
                        height: parent.height
                        color: "green"
                        opacity: 0.5
                        visible: (Math.floor(index / 8) === bestMoveTo.y) && ((index % 8) === bestMoveTo.x)
                    }
                }
            }
        }

        // Right Panel - Blunder Severity
        Rectangle {
            width: 200
            height: 300
            color: "#E0AFA0"
            radius: 8
            Column {
                anchors.centerIn: parent
                spacing: 10
                Text {
                    id: severityText
                    text: "Blunder Severity: N/A"
                    font.pixelSize: 16
                    color: "#1A1A1A"
                }
            }
        }
    }
}
