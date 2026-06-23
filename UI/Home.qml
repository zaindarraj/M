import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: window
    visible: true
    anchors.fill: parent

    // Track the active page index state dynamically
    property int currentPage: 0

    Component.onCompleted: {
        myPatientModel.loadData(window.currentPage) // Initial fetch for Page 0
    }

    TableModel {
        id: myPatientModel
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 15
        anchors.margins: 10

        // ==========================================
        // TOP PART: INPUT FORM
        // ==========================================
        GroupBox {
            title: "اضافة مريض جديد"
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent
                spacing: 10

                TextField {
                    id: nameInput
                    placeholderText: "الاسم"
                    Layout.fillWidth: true
                }

                TextField {
                    id: ageInput
                    placeholderText: "العمر"
                    Layout.preferredWidth: 80
                }

                TextField {
                    id: jobInput
                    placeholderText: "المهنة"
                    Layout.fillWidth: true
                }

                TextField {
                    id: addressInput
                    placeholderText: "الموقع"
                    Layout.fillWidth: true
                }

                Button {
                    text: "Add Patient"
                    highlighted: true
                    onClicked: {
                        if (nameInput.text !== "" && ageInput.text !== "") {
                            myPatientModel.addPatient(nameInput.text, ageInput.text, jobInput.text, addressInput.text)
                            nameInput.clear()
                            ageInput.clear()
                            jobInput.clear()
                            dateInput.clear()
                        }
                    }
                }
            }
        }

        RowLayout {
                    Layout.fillWidth: true
                    Layout.margins: 5
                    layoutDirection: Qt.RightToLeft

                    Label {
                        text: "بحث شامل:"
                        font.bold: true
                        Layout.alignment: Qt.AlignVCenter
                    }

                    TextField {
                        id: searchInput
                        placeholderText: "ابحث في جميع الحقول المطابقة هنا..."
                        Layout.fillWidth: true

                        horizontalAlignment: TextInput.AlignRight

                        // Calls C++ loadData filtering automatically as you type
                        onTextChanged: {

                            window.currentPage = 0 // Reset pagination to index 0 on new filters

                            myPatientModel.loadData(window.currentPage, 10 ,searchInput.text)
                        }
                    }

                    Button {
                        text: "مسح"
                        onClicked: searchInput.clear()
                    }
                }

        // ==========================================
        // MIDDLE PART: TABLE VIEW WITH HEADERS
        // ==========================================
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            // 1. Table Header
            HorizontalHeaderView {
                id: horizontalHeader
                syncView: tableView
                Layout.fillWidth: true


                delegate: Rectangle {
                    implicitHeight: 35
                    color: "#e0e0e0"
                    border.color: "#cccccc"

                    Text {
                        text: display
                        anchors.centerIn: parent
                        font.bold: true
                        color: "#333333"
                    }
                }
            }

            // 2. Data Grid View
            TableView {
                id: tableView

                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true

                model: myPatientModel


               property var columnProportions: [0.10, 0.15, 0.30, 0.12, 0.18, 0.15]
                // Note: Make sure the numbers inside add up to exactly 1.0 (100%)!

                columnWidthProvider: function(column) {
                    // 1. Establish the maximum available display grid width dynamically
                    var baseWidth = tableView.width > 50 ? tableView.width : (parent.width > 50 ? parent.width : 800);

                    // 2. Identify what the absolute LAST column index is dynamically
                    var lastColumnIndex = columnProportions.length - 1;

                    // Edge Case Guard: If a column index falls outside our proportion matrix data boundaries
                    if (column > lastColumnIndex) {
                        return 80; // Safe default width fallback
                    }

                    // 3. Dynamic Calculation for the LAST Column (Absorbs any remaining fractional pixels)
                    if (column === lastColumnIndex) {
                        var takenWidth = 0;
                        // Loop through every single column EXCEPT the last one dynamically
                        for (var i = 0; i < lastColumnIndex; ++i) {
                            takenWidth += Math.floor(baseWidth * columnProportions[i]);
                        }
                        var remainingWidth = baseWidth - takenWidth;
                        return Math.max(80, remainingWidth);
                    }

                    // 4. Standard dynamic calculations for all preceding layout blocks
                    return Math.max(50, Math.floor(baseWidth * columnProportions[column]));
                }

                delegate: Rectangle {
                    implicitHeight: 40
                    border.color: "#eeeeee"

                    TextField {
                        id: editField
                        anchors.fill: parent
                        anchors.margins: 2
                        text: display
                        verticalAlignment: TextInput.AlignVCenter
                        readOnly: column === 0

                        onEditingFinished: {
                            if (text !== display) {
                                display = text
                            }
                        }
                    }
                }
            }
        }

        // ==========================================
        // BOTTOM PART: PAGINATION CONTROLS
        // ==========================================
        RowLayout {
            id: paginationBar
            Layout.fillWidth: true
            Layout.preferredHeight: 45
            spacing: 20

            // Push layout to center
            Item { Layout.fillWidth: true }

            Button {
                id: prevButton
                text: "◀ Previous"
                flat: false
                // Disable button when on Page 0 to prevent database negative out-of-bounds crashes
                enabled: window.currentPage > 0

                onClicked: {
                    window.currentPage--;
                    myPatientModel.loadData(window.currentPage);
                }
            }

            Rectangle {
                id: pageDisplayBox
                Layout.preferredWidth: 100
                Layout.preferredHeight: 35
                color: "#f5f5f5"
                border.color: "#dddddd"
                radius: 4

                Text {
                    anchors.centerIn: parent
                    // Convert internal indexing (0, 1, 2) to user-friendly page numbers (1, 2, 3)
                    text: "Page " + (window.currentPage + 1)
                    font.pixelSize: 14
                    font.bold: true
                    color: "#555555"
                }
            }

            Button {
                id: nextButton
                text: "Next ▶"
                flat: false

                onClicked: {
                    window.currentPage++;
                    myPatientModel.loadData(window.currentPage);

                    // Note: If you want to automatically disable this button on the final empty page,
                    // you can check your model's current row count:
                    // enabled: myPatientModel.rowCount > 0
                }
            }

            // Push layout to center
            Item { Layout.fillWidth: true }
        }
    }
}