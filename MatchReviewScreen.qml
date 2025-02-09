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

    function updateGUIBoardBlunder() {
        if (matchBlunders.length > 0 && matchBlunders[matchReviewCurrentBlunderIndex]) {
            var currentBlunder = matchBlunders[matchReviewCurrentBlunderIndex];
            var updated1dGUIChessBoard = currentBlunder["boardBeforeBlunder"];

            // converts recieved 1d board into a 2d board so it can be displayed by the GUI
            var updatedGUIChessBoard = []
            for (var y = 0; y < 8; y++) {
                updatedGUIChessBoard.push(updated1dGUIChessBoard.slice(y * 8, (y + 1) * 8))
            }
            guiChessBoard.guiChessBoardOutputGrid = updatedGUIChessBoard

            blunderSquare.x = currentBlunder["blunder_to_x"];
            blunderSquare.y = currentBlunder["blunder_to_y"];

            bestMoveFrom.x = currentBlunder["best_from_x"];
            bestMoveFrom.y = currentBlunder["best_from_y"];
            bestMoveTo.x = currentBlunder["best_to_x"];
            bestMoveTo.y = currentBlunder["best_to_y"];
        }
    }

    Component.onCompleted: {
        updateGUIBoardBlunder();
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
                    text: "Next"
                    font.bold: true
                    Layout.preferredWidth: 170
                    Layout.preferredHeight: 55
                    font.pixelSize: 25
                    background: Rectangle {
                        color: "#F4F3EE"
                        radius: 10
                    }
                    enabled: matchReviewCurrentBlunderIndex < matchBlunders.length - 1
                    onClicked: {
                        matchReviewCurrentBlunderIndex += 1
                        updateGUIBoardBlunder()
                    }
                }
                // Button to go to previous blunder
                Button {
                    Layout.alignment: Qt.AlignHCenter
                    text: "Back"
                    font.bold: true
                    Layout.preferredWidth: 170
                    Layout.preferredHeight: 55
                    font.pixelSize: 25
                    background: Rectangle {
                        color: "#F4F3EE"
                        radius: 10
                    }
                    enabled: matchReviewCurrentBlunderIndex > 0
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
                    width: 70
                    height: 70
                    text: guiChessBoard.guiChessBoardOutputGrid[Math.floor(index / 8)][index % 8]
                    font.pixelSize: 58
                    background: Rectangle {
                        color: (index + Math.floor(index / 8)) % 2 === 0 ? "#EEEED2" : "#769656"
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
                    anchors.bottomMargin: 3 // to adjust for border on bottom
                }
            }
        }
    }
}
