import QtQuick 2.0
import com.syberos.basewidgets 2.0
import com.syberos.multimedia.photos 2.0

CPageStackWindow {
    initialPage:CPage{
        id: mainPage
        width:parent.width
        height:parent.height

        property string currentPath: ""
        property string currentSuffix: ""

        contentAreaItem : Item {
            id : main

            function formatSize(fileSizeInBytes) {
                var i = -1;
                var byteUnits = [' kB', ' MB', ' GB', ' TB', 'PB', 'EB', 'ZB', 'YB'];
                do {
                    fileSizeInBytes = fileSizeInBytes / 1024;
                    i++;
                } while (fileSizeInBytes > 1024);

                return Math.max(fileSizeInBytes, 0.1).toFixed(1) + byteUnits[i];
            }

            Rectangle {
                id: titleBar
                color: "#3a3535"
                height: 60
                anchors.top: parent.top
//                anchors.topMargin: 80
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                Text {
                    id: txt
                    text: "元心文档"
                    color: "#fff"
                    font.pixelSize: 35
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            ListView {
                id: view
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: titleBar.bottom
                anchors.bottom: parent.bottom

                model: listmodel
                clip: true

                signal selectionStateChanged

                delegate: Rectangle {
                    id: _delegate
                    width: view.width
                    height: 120

                    Rectangle{
                        id: decorateLine
                        color: gUiConst.getValue("CL1")
                        height: 2
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.leftMargin: 40
                        anchors.right: parent.right
                        anchors.rightMargin: 40
                    }

                    Row {
                        width: parent.width
                        height: parent.height - 2 * 30
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 40
                        anchors.right: parent.right
                        anchors.rightMargin: 40
                        spacing: 5

                        Image {
                            id: img
                            source: (suffix == "docx"||suffix == "doc")?"qrc:/images/word.png"
                                                    :((suffix == "xlsx"||suffix == "xls")?"qrc:/images/excel.png"
                                                    :suffix == "pdf"?"qrc:/images/pdf.png"
                                                                      :"qrc:/res/pic.png")
                            width: 80
                            height: 80
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Column {
                            height: parent.height
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: img.right
                            anchors.leftMargin: 40
                            anchors.right: parent.right
                            spacing: 5

                            Text {
                                id: fileTitle
                                height: parent.height / 2
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.rightMargin: view.editing ? 80 : 0
                                text:  filename
                                color: "#383535"
                                font.pixelSize:   28
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                            }

                            Text {
                                id: fileSize
                                height: parent.height / 2
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.rightMargin: view.editing ? 80 : 0
                                text:  main.formatSize(size)
                                color: "#383535"
                                font.pixelSize:   28
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            listmodel.currentIndex = index;
                            mainPage.currentPath = listmodel.getcurrentPath()
                            mainPage.currentSuffix = listmodel.getcurrentSuffix()
                            console.log("mainPage.currentPath :"+mainPage.currentPath)
                            console.log("mainPage.currentSuffix :"+mainPage.currentSuffix)
                            if(mainPage.currentSuffix == "jpg"||mainPage.currentSuffix == "png") {
                                main.__openImageFile(mainPage.currentPath)
                            } else {
                                var ret = mDialog.open(mainPage.currentPath, CMIMEDialogTool.Open,"")
                                if(!ret) {
                                    gToast.requestToast("无可打开工具！可先下载金软Office！")
                                }
                            }
                        }
                    }

                }
            }

            function __openImageFile(filepath) {
                pageStack.push("qrc:/qml/PhotoViewer.qml", { filepath: filepath })
                console.debug("---- open image file:", filepath);
            }


            CMIMEDialog {
                id: mDialog
            }
        }
    }
}
