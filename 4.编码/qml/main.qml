import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.14
import QtQuick.Controls 1.4
import "httpHandler.js" as HttpHandler
//https://www.zjyxxn.cn/contact/contactDB
ApplicationWindow  {
    visible: true
    width: 640
    height: 480
    title: qsTr("数据库显示")

    menuBar: MenuBar {
           Menu {
               title: qsTr("菜单")
               MenuItem {
                   text: qsTr("刷新")
                   onTriggered: contactTable.updateMsg();
               }
           }
    }


    TableView{
        id: contactTable
        anchors.fill: parent
        function updateMsg(){
            msgModel.clear()
            var url="https://www.zjyxxn.cn/contact/contactDB"
            HttpHandler.get(url,
                            function(result){
                                var _ListVal = HttpHandler.parseResult(result)
                                for(var i=0;i<_ListVal.length;i++){
                                    console.debug(_ListVal[i][3])
                                    msgModel.append({"_id": _ListVal[i][0], "_name":_ListVal[i][1],"_email":_ListVal[i][2],"_text":_ListVal[i][3]})
                                }
                            },function (){              //失败后的回调函数
                                console.log("error")
                            }
                         );


                console.debug("update")
            }

            //TableViewColumn 描述表格的每一列
            TableViewColumn{role: "_id"; title: "Id"; width: 30; elideMode: Text.ElideRight;}
            TableViewColumn{role: "_name"; title: "Name"; width: 100;}
            TableViewColumn{role: "_email"; title: "Email"; width: 140;}
            TableViewColumn{role: "_text"; title: "Text"; width: 100;}

            rowDelegate: Rectangle{
                height: 20
                color: styleData.selected?"blue":"white"
            }

            itemDelegate:Text{//设置每个单元格的字体样式
                text: styleData.value
                height: 25
                color: styleData.selected? "red" : styleData.textColor
                elide: styleData.elideMode
                font.pixelSize: 20
            }

            headerDelegate :Rectangle{//设置表头的样式
                implicitWidth: 20
                implicitHeight: 48
                gradient: styleData.pressed ? contactTable.pressG :
                       (styleData.containsMouse ? contactTable.hoverG : contactTable.nomalG)
                border.width: 1
                border.color: "gray"
                Text{
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 4
                    anchors.right: parent.right
                    anchors.rightMargin: 4
                    text: styleData.value
                    color: styleData.pressed ? "red" : "blue"
                    font.bold: true
                }
            }
            model:ListModel{
                id: msgModel

            }
//            model: ListModel{
//                id: msgModel
//                ListElement{
//                    _id: "1"
//                    _name: "Mi5"
//                    _email: "3200"
//                    _text: "xiaomi"
//                }
//            }//model is end

            focus: true
        }
}
