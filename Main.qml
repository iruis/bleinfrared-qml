import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import BleInfrared

Window {
    width: 720
    height: 480
    visible: true
    title: qsTr("infrered")
    color: 'black'

    required property list<string> devices;
    required property list<string> images
    property int offset: 0

    Settings {
        id: settings
    }

    Commander {
        id: commander

        device: device.currentText
    }

    Component.onCompleted: {
        device.model = devices
        device.currentIndex = device.find(settings.device)

        mode.currentIndex = mode.indexOfValue(settings.mode)
        fanSpeed.currentIndex = fanSpeed.indexOfValue(settings.fanSpeed)
        temperature.value = settings.temperature
        toggle.currentIndex = toggle.indexOfValue(settings.toggle)

        next()
    }

    function next() {
        image1.source = images[(offset + 0) % images.length]
        image2.source = images[(offset + 1) % images.length]

        animation.start()

        offset = (offset + 1) % images.length
    }

    function send(isOn) {
        settings.device = device.currentText
        settings.mode = mode.currentValue

        if (mode.currentValue === Commander.ModeFan) {
            settings.fanSpeed = fanSpeed.currentValue

            commander.fan(isOn, fanSpeed.currentValue)
        }
        if (mode.currentValue === Commander.ModeCool) {
            settings.fanSpeed = fanSpeed.currentValue
            settings.temperature = temperature.value

            commander.cool(isOn, temperature.value, fanSpeed.currentValue)
        }
        if (mode.currentValue === Commander.ModeDry) {
            settings.fanSpeed = fanSpeed.currentValue

            commander.dry(isOn, fanSpeed.currentValue)
        }
        if (mode.currentValue === Commander.ModeAi) {
            settings.ai = ai.currentValue

            commander.ai(isOn, ai.currentValue)
        }
        if (mode.currentValue === Commander.ModeToggle) {
            settings.toggle = toggle.currentValue

            commander.toggle(isOn, toggle.currentValue)
        }
    }

    Item {
        anchors.fill: parent

        Image {
            id: image1
            fillMode: Image.PreserveAspectCrop
            smooth: true

            anchors.fill: parent
        }
        Image {
            id: image2
            fillMode: Image.PreserveAspectCrop
            smooth: true

            anchors.fill: parent
        }

        SequentialAnimation {
            id: animation

            NumberAnimation {
                target: image2
                property: "opacity"
                from: 0
                to: 1
                duration: 5000
                easing.type: Easing.OutQuad
            }

            PauseAnimation {
                duration: 5000
            }

            onStopped: {
                next()
            }
        }

        ColumnLayout {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 5

            ComboBox {
                id: device

                Layout.fillWidth: true
            }

            ComboBox {
                id: mode

                textRole: 'text'
                valueRole: 'value'

                model: [
                    { value: Commander.ModeFan, text: '송풍' },
                    { value: Commander.ModeCool, text: '냉방' },
                    { value: Commander.ModeDry, text: '제습' },
                    { value: Commander.ModeAi, text: '인공지능' },
                    { value: Commander.ModeToggle, text: '기타' },
                ]

                Layout.fillWidth: true
            }

            ComboBox {
                id: fanSpeed

                textRole: 'text'
                valueRole: 'value'

                model: [
                    { value: Commander.FanSpeedLow, text: '약' },
                    { value: Commander.FanSpeedMedium, text: '중' },
                    { value: Commander.FanSpeedHigh, text: '강' },
                    { value: Commander.FanSpeedNature, text: '자연풍' },
                ]
                visible: mode.currentValue !== Commander.ModeAi && mode.currentValue !== Commander.ModeToggle

                Layout.fillWidth: true
            }

            ComboBox {
                id: ai

                textRole: 'text'
                valueRole: 'value'

                model: [
                    { value: Commander.AiN2, text: '더울때' },
                    { value: Commander.AiN1, text: '조금 더울때' },
                    { value: Commander.AiZero, text: '적당할때' },
                    { value: Commander.AiP1, text: '조금 추울때' },
                    { value: Commander.AiP2, text: '추울때' },
                ]
                visible: mode.currentValue === Commander.ModeAi

                Layout.fillWidth: true
            }

            ComboBox {
                id: toggle

                textRole: 'text'
                valueRole: 'value'

                model: [
                    { value: Commander.ToggleEnergySave, text: '에너지 절약' },
                    { value: Commander.ToggleSwingVertical, text: '상하 바람' },
                    { value: Commander.ToggleComfortAir, text: '간접 바람' },
                ]
                visible: mode.currentValue === Commander.ModeToggle

                Layout.fillWidth: true
            }

            Button {
                text: '▲'

                visible: mode.currentValue === Commander.ModeCool
                onClicked: temperature.value = Math.min(commander.temperatureMax, temperature.value + 1)

                Layout.fillWidth: true
            }
            SpinBox {
                id: temperature

                from: commander.temperatureMin
                to: commander.temperatureMax

                visible: mode.currentValue === Commander.ModeCool

                Layout.fillWidth: true
            }
            Button {
                text: '▼'

                visible: mode.currentValue === Commander.ModeCool
                onClicked: temperature.value = Math.max(commander.temperatureMin, temperature.value - 1)

                Layout.fillWidth: true
            }

            Button {
                text: '적용'

                visible: mode.currentValue !== Commander.ModeToggle
                onClicked: send(false)

                Layout.fillWidth: true
            }

            Button {
                text: '터보'

                visible: mode.currentValue !== Commander.ModeToggle
                onClicked: commander.turbo()

                Layout.fillWidth: true
            }

            Button {
                text: '켜기'

                visible: mode.currentValue !== Commander.ModeToggle
                onClicked: send(true)

                Layout.fillWidth: true
            }

            Button {
                text: '끄기'

                visible: mode.currentValue !== Commander.ModeToggle
                onClicked: commander.powerOff()

                Layout.fillWidth: true
            }

            Button {
                text: 'ON'

                visible: mode.currentValue === Commander.ModeToggle
                onClicked: send(true)

                Layout.fillWidth: true
            }

            Button {
                text: 'OFF'

                visible: mode.currentValue === Commander.ModeToggle
                onClicked: send(false)

                Layout.fillWidth: true
            }
        }
    }
}
