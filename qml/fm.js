function getDir(filePath) {
    return filePath.substr(0, filePath.lastIndexOf('/'));
}

function getFileName(filePath) {
    return filePath.substring(filePath.lastIndexOf("/")+1, filePath.length);
}

function getSizeStr(bytes) {
    var unit = ["B", "KB", "MB", "GB", "TB"]
    var precision;
    var i = 0
    var str = bytes + unit[0]

    while(bytes > 1024 && i < unit.length) {
        bytes /= 1024.0
        i++
//        str = Number(bytes).toPrecision(3) + unit[i];
    }
    if (i > 0 && i < unit.length) {
        if (bytes >= 1000)
            precision = 4;
        else
            precision = 3;
        str = Number(bytes).toPrecision(precision) + unit[i];
    }

    return str;
}

function getSuffix(filePath) {
    var p = filePath.lastIndexOf('.')
    if(p < 0)
        return ""
    return filePath.substr(p+1).toLowerCase() //??? toLowerCase ??
}
function isPicture(suffix) {
    if(suffix === "png" || suffix === "jpg"
            || suffix === "jpeg" || suffix === "bmp" || suffix === "gif" || suffix === "tiff" //not support??
            )
        return true
    else
        return false
}
function isVideo(suffix) {
    if( suffix === "mp4" )
        return true
    else
        return false
}
function isMusic(suffix) {
    if(suffix === "mp3" || suffix === "ape" || suffix === "wav" || suffix === "flac"
            || suffix === "wma" || suffix ==="aac" || suffix === "amr" || suffix === "m4a")
        return true
    else
        return false
}
function isDoc(suffix) {
    if(suffix === "txt" || suffix === "doc" || suffix === "pdf" || suffix === "sh" || suffix === "docx")
        return true
    else
        return false
}

function hasThumbnail(fileName) {
    var suffix = getSuffix(fileName)
    if(isPicture(suffix))
    //if(isPicture(suffix) || isVideo(suffix)) //not support video
        return true
    else
        return false
}
function date2str(x,y) {
    var z = {y:x.getFullYear(),M:x.getMonth()+1,d:x.getDate(),h:x.getHours(),m:x.getMinutes(),s:x.getSeconds()};
    return y.replace(/(y+|M+|d+|h+|m+|s+)/g,function(v) {return ((v.length>1?"0":"")+eval('z.'+v.slice(-1))).slice(-(v.length>2?v.length:2))});
}
function formatDate(date) {
    var datetime = new Date(date)
    var year = datetime.getFullYear()
    var month = datetime.getMonth() + 1
    var day = datetime.getDate()
    var hour = datetime.getHours()
    var minute = datetime.getMinutes().toString()
    if(minute.length == 1)
        minute = "0" + minute
    return year + "-" + month + "-" + day + " " + hour + ":" + minute
}

function openFile(filepath) {
    console.debug("open file mimetype:", filepath, fileUtils.getMimeType(filepath))
    var mimetype = fileUtils.getFileType(filepath);
    //var suffix = FM.getSuffix(fileName)
    if(mimetype == "video"){//(FM.isVideo(suffix)) {
        __openVideoFile(filepath);
    } else if(mimetype == "music"){//if(FM.isMusic(suffix)){
        __openAudioFile(filepath);
    } else if(mimetype == "image"){//if(FM.isPicture(suffix)) {
        __openImageFile(filepath, 0, "single");
    } else if(mimetype == "text"){//if(FM.getSuffix(filepath) === "txt"){
        __openTextFile(filepath);
    }
}

function __openAudioFile(filepath) {
    var comp = Qt.createComponent("qrc:/qml/MusicPlayer.qml")
    if (comp.status == Component.Ready) {
        var musicPlayer = comp.createObject(rootPage, { "filepath": filepath });
        //musicPlayer.exit.connect(function(){musicPlayer.destroy()})
    }
    console.debug("---- open audio file:", filepath);
}
function __openVideoFile(filepath) {
    var filepathUrl = "file://" + filepath;
    console.debug("---- open video file----------", filepath, filepathUrl);
    globalqApp.openUrl("videoplugin://play?fileuri=" + filepathUrl);
}
function __openImageFile(filepath) {
    pageStack.push("qrc:/qml/PhotoViewer.qml", { filepath: filepath })
    console.debug("---- open image file:", filepath);
}
function __openTextFile(filepath) {
    pageStack.push("qrc:/qml/TextViewer.qml", { fullfilepath: filepath })
    console.debug("---- open text file:", filepath);
}

function getIcon(filePath, isFile) {
    if(isFile) {
        var mimetype = fileUtils.getFileType(filePath);
        if(mimetype == "video" || mimetype == "image") {
            return "image://nemoThumbnail/" + filePath;
        } else if(mimetype == "music"){
            return "qrc:/img/file_music.png";
        } else if(mimetype == "text"){
            return "qrc:/img/file_doc.png";
        } else
            return "qrc:/img/file_unknown.png"
    } else {
        if(filePath == "/home/user")
            return "qrc:/img/icon_localhost.png"
        else if(filePath == "/mnt/sdcard")
            return "qrc:/img/icon_sd.png"
        return "qrc:/img/dic.png"
    }
}
