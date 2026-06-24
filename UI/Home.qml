import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Item {
    id: window
    visible: true
    anchors.fill: parent

    // Modern Medical-Themed Palette Constants
    readonly property color colorPrimary: "#2c3e50"       // Slate blue for main text/headers
    readonly property color colorAccent: "#16a085"        // Teal for highlights/buttons
    readonly property color colorBgLight: "#f4f6f7"       // Off-white canvas background
    readonly property color colorCardBg: "#ffffff"        // Pure white for containers
    readonly property color colorBorder: "#d5dbdb"        // Clean, subtle border color
    readonly property color colorTextMuted: "#7f8c8d"     // Grey for placeholders/small labels

    property int currentPage: 0

    // Canvas Background
    Rectangle {
        anchors.fill: parent
        color: window.colorBgLight
        z: -1
    }

    DatabaseManager {
        id: dbManager
        onDatabasePathChanged: {
            console.log("Active Application DB File path:", databasePath);
            // Optional: Trigger your TableModel to refresh its dataset records here
            myPatientModel.loadData(0, 10, "");
        }
    }

    FileDialog {
        id: dbFileDialog
        title: "الرجاء اختيار ملف قاعدة البيانات"
        currentFolder: StandardPaths.writableLocation(StandardPaths.DownloadLocation)
        nameFilters: ["Database files (*.db *.sqlite)", "All files (*)"]

        onAccepted: {
            // Send selected path payload over to the C++ parser method
            var success = dbManager.loadExternalDatabase(dbFileDialog.selectedFile);
            if (!success) {
                console.log("Error binding chosen data storage file target structure.");
            }
        }
    }

    Component.onCompleted: {
        myPatientModel.loadData(window.currentPage);
    }

    TableModel {
        id: myPatientModel

        onCurrentPatientRecordChanged: {
            var record = myPatientModel.currentPatientRecord;
            currentPatientID.text = record ? record.id : "";

            if (record) {
                bloodPressureField.text = record.bloodPressure ?? "";
                pulseRateField.text = record.pulseRate ?? "";
                bodyTemperatureField.text = record.bodyTemperature ?? "";
                symptomDurationField.text = record.symptomDuration ?? "";
                labResultsRawField.text = record.labResultsRaw ?? "";
                differentialDiagnosisField.text = record.differentialDiagnosis ?? "";
                chiefComplaintField.text = record.chiefComplaint ?? "";
                detailedHistoryField.text = record.detailedHistory ?? "";
                treatmentProcedureField.text = record.treatmentProcedure ?? "";
            } else {
                bloodPressureField.text = "";
                pulseRateField.text = "";
                bodyTemperatureField.text = "";
                symptomDurationField.text = "";
                labResultsRawField.text = "";
                differentialDiagnosisField.text = "";
                chiefComplaintField.text = "";
                detailedHistoryField.text = "";
                treatmentProcedureField.text = "";
            }
        }
    }

    ScrollView {
        clip: true
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            width: parent.width
            layoutDirection: Qt.RightToLeft
            spacing: 20
            anchors.margins: 15

            RowLayout {
                Layout.fillWidth: true
                layoutDirection: Qt.RightToLeft
                spacing: 10

                Button {
                    text: "اختر ملف قاعدة البيانات (Pick DB)"
                    onClicked: dbFileDialog.open()

                    background: Rectangle {
                        color: parent.hovered ? "#eaeded" : "#f2f4f4"
                        border.color: window.colorBorder
                        radius: 4
                    }
                }

                Label {
                    text: "المسار الحالي: " + (dbManager.databasePath ? dbManager.databasePath : "لم يتم التحميل بعد")
                    color: window.colorTextMuted
                    font.pixelSize: 11
                    Layout.fillWidth: true
                    elide: Text.ElideLeft // Shrinks path securely if overly long string lengths occur
                }
            }

            // ==========================================
            // TOP PART: INPUT FORM
            // ==========================================
            GroupBox {
                title: "اضافة مريض جديد"
                LayoutMirroring.enabled: true
                LayoutMirroring.childrenInherit: true
                Layout.fillWidth: true

                background: Rectangle {
                    color: window.colorCardBg
                    border.color: window.colorBorder
                    border.width: 1
                    radius: 6
                }

                RowLayout {
                    anchors.fill: parent
                    spacing: 12

                    // Labeled TextField component
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Label {
                            text: "الاسم"
                            font.pixelSize: 11
                            font.bold: true
                            color: window.colorTextMuted
                        }
                        TextField {
                            id: nameInput
                            placeholderText: "الاسم"
                            Layout.fillWidth: true
                        }
                    }

                    ColumnLayout {
                        Layout.preferredWidth: 90
                        spacing: 4

                        Label {
                            text: "العمر"
                            font.pixelSize: 11
                            font.bold: true
                            color: window.colorTextMuted
                        }
                        TextField {
                            id: ageInput
                            placeholderText: "العمر"
                            Layout.fillWidth: true
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Label {
                            text: "المهنة"
                            font.pixelSize: 11
                            font.bold: true
                            color: window.colorTextMuted
                        }
                        TextField {
                            id: jobInput
                            placeholderText: "المهنة"
                            Layout.fillWidth: true
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Label {
                            text: "الموقع"
                            font.pixelSize: 11
                            font.bold: true
                            color: window.colorTextMuted
                        }
                        TextField {
                            id: addressInput
                            placeholderText: "الموقع"
                            Layout.fillWidth: true
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Label {
                            text: "الرقم"
                            font.pixelSize: 11
                            font.bold: true
                            color: window.colorTextMuted
                        }
                        TextField {
                            id: phoneInput
                            placeholderText: "الرقم"
                            Layout.fillWidth: true
                        }
                    }

                    Button {
                        text: "اضافة المريض"
                        highlighted: true
                        Layout.alignment: Qt.AlignBottom
                        Layout.preferredHeight: 40

                        background: Rectangle {
                            color: parent.down ? Qt.darker(window.colorAccent, 1.2) : (parent.hovered ? Qt.lighter(window.colorAccent, 1.1) : window.colorAccent)
                            radius: 4
                        }
                        contentItem: Text {
                            text: parent.text
                            color: "white"
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {
                            if (nameInput.text !== "" && ageInput.text !== "") {
                                myPatientModel.addPatient(nameInput.text, ageInput.text, jobInput.text, addressInput.text, phoneInput.text);
                                nameInput.clear();
                                ageInput.clear();
                                jobInput.clear();
                                phoneInput.clear();
                                addressInput.clear();
                            }
                        }
                    }
                }
            }

            // Search Bar Row
            RowLayout {
                Layout.fillWidth: true
                layoutDirection: Qt.RightToLeft
                spacing: 12

                Label {
                    text: "بحث شامل:"
                    font.bold: true
                    font.pixelSize: 14
                    color: window.colorPrimary
                    Layout.alignment: Qt.AlignVCenter
                }

                Timer {
                    id: debounceTimer
                    interval: 300
                    repeat: false
                    onTriggered: {
                        window.currentPage = 0;
                        myPatientModel.loadData(window.currentPage, 10, searchInput.text);
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Label {
                        text: "البحث"
                        font.pixelSize: 11
                        font.bold: true
                        color: window.colorTextMuted
                        Layout.alignment: Qt.AlignRight
                    }
                    TextField {
                        id: searchInput
                        placeholderText: "ابحث في جميع الحقول المطابقة هنا..."
                        Layout.fillWidth: true
                        horizontalAlignment: TextInput.AlignRight
                        onTextChanged: debounceTimer.restart()
                    }
                }

                Button {
                    text: "مسح"
                    Layout.alignment: Qt.AlignBottom
                    Layout.preferredHeight: 40
                    onClicked: searchInput.clear()

                    background: Rectangle {
                        color: parent.down ? "#d5dbdb" : (parent.hovered ? "#eaeded" : "#f2f4f4")
                        border.color: window.colorBorder
                        radius: 4
                    }
                    contentItem: Text {
                        text: parent.text
                        color: window.colorPrimary
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            // ==========================================
            // MIDDLE PART: FIXED TABLE VIEW
            // ==========================================
            ColumnLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 300
                Layout.minimumHeight: 300
                spacing: 0

                HorizontalHeaderView {
                    id: horizontalHeader
                    syncView: tableView
                    Layout.fillWidth: true

                    delegate: Rectangle {
                        implicitHeight: 40
                        color: window.colorPrimary
                        border.color: Qt.darker(window.colorPrimary, 1.1)

                        Text {
                            text: display
                            anchors.centerIn: parent
                            font.bold: true
                            font.pixelSize: 12
                            color: "white"
                        }
                    }
                }

                TableView {
                    id: tableView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: myPatientModel

                    signal rowClicked(int rowIndex)

                    onRowClicked: function (rowIndex) {
                        console.log("Row clicked in DB model:", rowIndex);
                        var codeA = myPatientModel.getCodeA(rowIndex);
                        myPatientModel.fetchPatientRecored(codeA);
                    }

                    property var columnProportions: [0.10, 0.15, 0.15, 0.12, 0.18, 0.15, 0.15]

                    columnWidthProvider: function (column) {
                        var baseWidth = tableView.width > 50 ? tableView.width : 800;
                        var lastColumnIndex = columnProportions.length - 1;

                        if (column > lastColumnIndex)
                            return 80;

                        if (column === lastColumnIndex) {
                            var takenWidth = 0;
                            for (var i = 0; i < lastColumnIndex; ++i) {
                                takenWidth += Math.floor(baseWidth * columnProportions[i]);
                            }
                            return Math.max(80, baseWidth - takenWidth);
                        }
                        return Math.max(50, Math.floor(baseWidth * columnProportions[column]));
                    }

                    delegate: Rectangle {
                        implicitHeight: 40
                        color: (row % 2 === 0) ? window.colorCardBg : "#f9fbfb"
                        border.color: "#e5e8e8"

                        TextField {
                            id: editField
                            anchors.fill: parent
                            anchors.margins: 2
                            text: display
                            verticalAlignment: TextInput.AlignVCenter
                            readOnly: column === 0
                            selectByMouse: true

                            background: Rectangle {
                                color: "transparent"
                                border.width: parent.activeFocus ? 1 : 0
                                border.color: window.colorAccent
                            }

                            onActiveFocusChanged: {
                                if (activeFocus) {
                                    tableView.rowClicked(row);
                                }
                            }

                            onEditingFinished: {
                                if (text !== display) {
                                    display = text;
                                }
                            }
                        }
                    }
                }

                // ==========================================
                // BOTTOM PART: PAGINATION CONTROLS
                // ==========================================
                RowLayout {
                    Layout.fillWidth: true
                    Layout.topMargin: 10
                    spacing: 15

                    Item {
                        Layout.fillWidth: true
                    }

                    Button {
                        text: "◀ Previous"
                        enabled: window.currentPage > 0
                        onClicked: {
                            window.currentPage--;
                            myPatientModel.loadData(window.currentPage);
                        }
                    }

                    Rectangle {
                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 35
                        color: window.colorCardBg
                        border.color: window.colorBorder
                        radius: 4
                        Text {
                            anchors.centerIn: parent
                            text: "Page " + (window.currentPage + 1)
                            font.pixelSize: 13
                            font.bold: true
                            color: window.colorPrimary
                        }
                    }

                    Button {
                        text: "Next ▶"
                        onClicked: {
                            window.currentPage++;
                            myPatientModel.loadData(window.currentPage);
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                    }
                }
            }

            Text {
                id: currentPatientID
                text: "No Patient Selected"
                visible: false
            }

            // ==========================================
            // PATIENT DETAILS WITH PERSISTENT LABELS
            // ==========================================
            GroupBox {
                title: "تفاصيل الملف الطبي الحالي"
                Layout.fillWidth: true

                background: Rectangle {
                    color: window.colorCardBg
                    border.color: window.colorBorder
                    border.width: 1
                    radius: 6
                }

                GridLayout {
                    anchors.fill: parent
                    columns: 2
                    columnSpacing: 15
                    rowSpacing: 15

                    // Row 1
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Label {
                            text: "رقم تعريف المريض (CODEA)"
                            font.pixelSize: 11
                            font.bold: true
                            color: window.colorTextMuted
                            Layout.alignment: Qt.AlignRight
                        }
                        TextField {
                            id: idField
                            Layout.fillWidth: true
                            placeholderText: "رقم تعريف المريض"
                            horizontalAlignment: TextField.AlignRight
                            readOnly: true
                            text: myPatientModel.currentPatientRecord.id ?? ""

                            background: Rectangle {
                                color: "#f2f4f4"
                                border.color: window.colorBorder
                                radius: 4
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Label {
                            text: "تاريخ الدخول (DATEA)"
                            font.pixelSize: 11
                            font.bold: true
                            color: window.colorTextMuted
                            Layout.alignment: Qt.AlignRight
                        }
                        TextField {
                            id: admissionDateField
                            Layout.fillWidth: true
                            placeholderText: "تاريخ الدخول"
                            horizontalAlignment: TextField.AlignRight
                            text: myPatientModel.currentPatientRecord.admissionDate ?? ""
                            onEditingFinished: {
                                myPatientModel.currentPatientRecord.admissionDate = text;
                                myPatientModel.updatePatientRecored();
                            }
                        }
                    }

                    // Row 2
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Label {
                            text: "ضغط الدم (مثال: 70/110)"
                            font.pixelSize: 11
                            font.bold: true
                            color: window.colorTextMuted
                            Layout.alignment: Qt.AlignRight
                        }
                        TextField {
                            id: bloodPressureField
                            Layout.fillWidth: true
                            placeholderText: "ضغط الدم"
                            horizontalAlignment: TextField.AlignRight
                            text: myPatientModel.currentPatientRecord.bloodPressure ?? ""
                            onEditingFinished: {
                                myPatientModel.currentPatientRecord.bloodPressure = text;
                                myPatientModel.updatePatientRecored();
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Label {
                            text: "النبض (مثال: 80/د)"
                            font.pixelSize: 11
                            font.bold: true
                            color: window.colorTextMuted
                            Layout.alignment: Qt.AlignRight
                        }
                        TextField {
                            id: pulseRateField
                            Layout.fillWidth: true
                            placeholderText: "النبض"
                            horizontalAlignment: TextField.AlignRight
                            text: myPatientModel.currentPatientRecord.pulseRate ?? ""
                            onEditingFinished: {
                                myPatientModel.currentPatientRecord.pulseRate = text;
                                myPatientModel.updatePatientRecored();
                            }
                        }
                    }

                    // Row 3
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Label {
                            text: "درجة الحرارة (°C)"
                            font.pixelSize: 11
                            font.bold: true
                            color: window.colorTextMuted
                            Layout.alignment: Qt.AlignRight
                        }
                        TextField {
                            id: bodyTemperatureField
                            Layout.fillWidth: true
                            placeholderText: "درجة الحرارة"
                            horizontalAlignment: TextField.AlignRight
                            text: myPatientModel.currentPatientRecord.bodyTemperature ?? ""
                            onEditingFinished: {
                                myPatientModel.currentPatientRecord.bodyTemperature = text;
                                myPatientModel.updatePatientRecored();
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Label {
                            text: "مدة الأعراض (مثال: منذ حوالي سنتين)"
                            font.pixelSize: 11
                            font.bold: true
                            color: window.colorTextMuted
                            Layout.alignment: Qt.AlignRight
                        }
                        TextField {
                            id: symptomDurationField
                            Layout.fillWidth: true
                            placeholderText: "مدة الأعراض"
                            horizontalAlignment: TextField.AlignRight
                            text: myPatientModel.currentPatientRecord.symptomDuration ?? ""
                            onEditingFinished: {
                                myPatientModel.currentPatientRecord.symptomDuration = text;
                                myPatientModel.updatePatientRecored();
                            }
                        }
                    }

                    // Row 4
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Label {
                            text: "ملخص التحاليل (Hb, Hct, WBC...)"
                            font.pixelSize: 11
                            font.bold: true
                            color: window.colorTextMuted
                            Layout.alignment: Qt.AlignRight
                        }
                        TextField {
                            id: labResultsRawField
                            Layout.fillWidth: true
                            placeholderText: "ملخص التحاليل المختبرية"
                            horizontalAlignment: TextField.AlignRight
                            text: myPatientModel.currentPatientRecord.labResultsRaw ?? ""
                            onEditingFinished: {
                                myPatientModel.currentPatientRecord.labResultsRaw = text;
                                myPatientModel.updatePatientRecored();
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Label {
                            text: "التشخيص التفريقي / المبدئي"
                            font.pixelSize: 11
                            font.bold: true
                            color: window.colorTextMuted
                            Layout.alignment: Qt.AlignRight
                        }
                        TextField {
                            id: differentialDiagnosisField
                            Layout.fillWidth: true
                            placeholderText: "التشخيص التفريقي المبدئي"
                            horizontalAlignment: TextField.AlignRight
                            text: myPatientModel.currentPatientRecord.differentialDiagnosis ?? ""
                            onEditingFinished: {
                                myPatientModel.currentPatientRecord.differentialDiagnosis = text;
                                myPatientModel.updatePatientRecored();
                            }
                        }
                    }
                }
            }

            // ==========================================
            // MULTI-LINE TEXT FIELDS & EXPLANATIONS
            // ==========================================

            // حقل الشكوى الرئيسية (DZ4)
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4

                Label {
                    text: "الشكوى الرئيسية (Chief Complaint):"
                    font.bold: true
                    font.pixelSize: 13
                    color: window.colorPrimary
                    Layout.alignment: Qt.AlignRight
                }
                TextField {
                    id: chiefComplaintField
                    Layout.fillWidth: true
                    placeholderText: "اكتب الشكوى الأساسية للمريض هنا..."
                    horizontalAlignment: TextField.AlignRight
                    text: myPatientModel.currentPatientRecord.chiefComplaint ?? ""
                    onEditingFinished: {
                        myPatientModel.currentPatientRecord.chiefComplaint = text;
                        myPatientModel.updatePatientRecored();
                    }
                }
            }

            // حقل التاريخ المرضي المفصل (DZ6)
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4

                Label {
                    text: "التاريخ المرضي المفصل (Detailed History):"
                    font.bold: true
                    font.pixelSize: 13
                    color: window.colorPrimary
                    Layout.alignment: Qt.AlignRight
                }
                ScrollView {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 120
                    clip: true

                    background: Rectangle {
                        color: window.colorCardBg
                        border.color: window.colorBorder
                        radius: 4
                    }

                    TextArea {
                        id: detailedHistoryField
                        width: parent.width // Ensures text wrapping triggers instead of horizontal layout clipping
                        placeholderText: "تفاصيل السيرة المرضية والفحص السريري..."
                        horizontalAlignment: TextEdit.AlignRight
                        wrapMode: TextEdit.Wrap
                        text: myPatientModel.currentPatientRecord.detailedHistory ?? ""
                        padding: 8

                        background: null // Handled by scrollview parent context wrapper

                        onEditingFinished: {
                            myPatientModel.currentPatientRecord.detailedHistory = text;
                            myPatientModel.updatePatientRecored();
                        }
                    }
                }
            }

            // حقل الخطة العلاجية أو الإجراء الجراحي (DZ20)
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4

                Label {
                    text: "الإجراء العلاجي / الخطة الجراحية (Treatment Procedure):"
                    font.bold: true
                    font.pixelSize: 13
                    color: window.colorPrimary
                    Layout.alignment: Qt.AlignRight
                }
                ScrollView {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 120
                    clip: true

                    background: Rectangle {
                        color: window.colorCardBg
                        border.color: window.colorBorder
                        radius: 4
                    }

                    TextArea {
                        id: treatmentProcedureField
                        width: parent.width // Ensures text wrapping triggers instead of horizontal layout clipping
                        placeholderText: "تفاصيل التدخل الطبي، الأدوية أو العمل الجراحي..."
                        horizontalAlignment: TextEdit.AlignRight
                        wrapMode: TextEdit.Wrap
                        text: myPatientModel.currentPatientRecord.treatmentProcedure ?? ""
                        padding: 8

                        background: null // Handled by scrollview parent context wrapper

                        onEditingFinished: {
                            myPatientModel.currentPatientRecord.treatmentProcedure = text;
                            myPatientModel.updatePatientRecored();
                        }
                    }
                }
            }

            // Space placeholder to push pagination down if content is short
            Item {
                Layout.fillHeight: true
            }
        }
    }
}
