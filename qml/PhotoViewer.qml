/*
* This file is part of file-manager
*
* Copyright (C) 2014 Beijing Yuan Xin Technology Co.,Ltd. All rights reserved.
*
* Authors:
*       Zhenbin Cai <caizhenbin@syberos.com>
*       Pengcheng Zhang <zhangpengcheng@syberos.com>
*
* This software, including documentation, is protected by copyright controlled
* by Beijing Yuan Xin Technology Co.,Ltd. All rights are reserved.
*/

import QtQuick 2.0
import com.syberos.basewidgets 1.0
import com.syberos.multimedia.photos 2.0
import "fm.js" as FM

ManagerPreviewMain {
    id: photoViewer

    property string filepath: ""

    Component.onCompleted: {
//        viewHandle.hideStatusBar();
    }
    Component.onDestruction: {
//        viewHandle.showStatusBar();
    }

    onStatusChanged:{
       if (status === CPageStatus.WillShow) {
           gScreenInfo.setStatusBar(false);
       } else if (status === CPageStatus.WillHide) {
           gScreenInfo.setStatusBar(true);
           gScreenInfo.setStatusBarStyle("transblack");
       }
    }

    onFilepathChanged: {
        imageModel.clear();

        console.debug("photo viewer image path:", filepath);
        var dir = FM.getDir(filepath);
        //console.debug("photo viewer image dir:", dir);
        var fl;
        /*if(isRealSearchBarShow)
            fl = [filepath];
        else *//*if(currentTab == 0) // type mode
            fl = editModel.getFileList();
        else*/
            fl = fileUtils.getImgFilesInDir(dir);
        var i;
        for(i=0; i<fl.length; i++) {
            if(fl[i] == filepath)
                currentindex = i;
            imageModel.append({"url": "file://" + encodeURIComponent(fl[i])});
        }
        photoViewer.currentmodel = imageModel
    }

//    onStatusBarDisplay: {//when clicked the image receiving this signal
//        console.log("11Receive the statusBarDisplay siganl, we should show or hide the status bar, display is ", display);
//        if (display) {
//            viewHandle.showStatusBar();
//            viewHandle.setStatusBarStyle("transwhite");
//        } else {
//            viewHandle.hideStatusBar();
//        }
//    }

    onOperateFinished: {//after delete and crushing delete, receive the operateFinished signal
        console.log("Receive the operateFinished siganl, the index and path of deleted picture:", index, path)
        editModel.removeRowByFilePath(path);
    }


    ListModel {
      id: imageModel
    }
}
