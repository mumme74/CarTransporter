.pragma library
.import QtQuick 2.2 as QtQuick2

var _initObj = {
    globalApp: null,
    mainView: null,
    diagHeader: null,
    settingsHeader: null,
    forwardBtn: null,
    notifierWgt: null
};

function init(globalApp, mainView, forwardBtn, notifierWgt) {
    _initObj.globalApp = globalApp;
    _initObj.mainView = mainView;
    _initObj.forwardBtn = forwardBtn;
    _initObj.notifierWgt = notifierWgt;
}

function getNotifier() { return _initObj.notifierWgt; }
function notifyCanIsOff() {
    getNotifier().setContent("qrc:/images/error.svg",
                                     qsTr("Network error"),
                                     qsTr("Can't send on CAN network"));
    getNotifier().showNotification();
}


function mainView() { return _initObj.mainView; }
function globalApp() { return _initObj.globalApp; }

function switchToDiagHeader(page) {
    switchToHeader("DiagHeader", page, "diagHeader");
}

function switchToSettingsHeader(page) {
    switchToHeader("SettingsHeader", page, "settingsHeader");
}

function switchToHeader(headerName, page, initObjMember) {
    if (!page || !page.visible)
        return;
    if (!globalApp().header || !globalApp().header.name !== headerName) {
        if (!_initObj[initObjMember]) {
            var comp = Qt.createComponent(headerName + ".qml");
            _initObj[initObjMember] = comp.createObject(globalApp());
        }
        globalApp().header = _initObj[initObjMember];
        _initObj[initObjMember].visible = true;
        mainView().anchors.top = globalApp().header.bottom;
        _initObj[initObjMember].anchors.right = globalApp.right;
        _initObj[initObjMember].anchors.left = globalApp.left;
        _initObj[initObjMember].anchors.top = globalApp.top;
    }
}

function hideDiagHeader() {
    if (_initObj.diagHeader)
        _initObj.diagHeader.visible = false;
    globalApp().header = null;
}

function pushPage(strPageObj) {
    var comp = Qt.createComponent(strPageObj + ".qml");
    if (comp.status === QtQuick2.Component.Error) {
        console.log("Error loading component " + strPageObj + " :", comp.errorString());
    }
    var page = comp.createObject(mainView());
    mainView().push(page);
}

var _forwardStack = [];
var _lastPopped = null;
function back() {
    if (mainView().depth > 1) {
        _lastPopped = mainView().pop();
        _forwardStack.push(_lastPopped);
        _initObj.forwardBtn.enabled = true;
    }
}

function forward() {
    if (_forwardStack.length) {
        _lastPopped = _forwardStack.pop();
        if (_lastPopped && mainView().currentItem !== _lastPopped) {
            mainView().push(_lastPopped);
            _lastPopped = _forwardStack[_forwardStack.length -1];
        }
    } else {
        _initObj.forwardBtn.enabled = false;
    }
}

function checkForward() {
    if (mainView() && _lastPopped !== mainView().currentItem) {
        _lastPopped = null;
        _forwardStack.length = 0;
        _initObj.forwardBtn.enabled = false;
    }
}

var carState = "NormalState";


