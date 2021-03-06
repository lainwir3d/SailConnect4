import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import "."

Item {
	signal configChanged()
	signal resetScores()

	property double minW: mainLayout.Layout.minimumWidth
	property double minH: mainLayout.Layout.minimumHeight
	property double implW: mainLayout.implicitWidth
	property double implH: mainLayout.implicitHeight

	anchors.fill: parent

	ColumnLayout {
		id: mainLayout
		anchors.fill: parent
		anchors.margins: margin
		spacing: 10

		ControlsPlayer {
			id: p1
			title: qsTr("Player %1").arg(1)
			name: Config.player1_name
			force: Config.player1_force
			type: Config.player1_type

			onNameEdited: Config.player1_name = new_name
			onSubmit: {
				Config.player1_force = force
				Config.player1_type = getType()
			}
		}

		ControlsPlayer {
			id: p2
			title: qsTr("Player %1").arg(2)
			name: Config.player2_name
			force: Config.player2_force
			type: Config.player2_type

			onNameEdited: Config.player2_name = new_name
			onSubmit: {
				Config.player2_force = force
				Config.player2_type = getType()
			}
		}

		Button {
			text: qsTr("Erase scores")
			Layout.alignment: Qt.AlignCenter

			onClicked: {
				Config.player1_points = 0
				Config.player2_points = 0
				resetScores()
			}
		}

		Button {
			text: qsTr("Submit")
			Layout.alignment: Qt.AlignCenter
			onClicked: {
				p1.submitAll()
				p2.submitAll()
				configChanged()
			}
		}
	}
}
