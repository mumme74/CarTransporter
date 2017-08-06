.pragma library
var _initObj = {
    globalApp: null,
    mainView: null,
    diagHeader: null,
    forwardBtn: null
};

function init(globalApp, mainView, forwardBtn) {
    _initObj.globalApp = globalApp;
    _initObj.mainView = mainView;
    _initObj.forwardBtn = forwardBtn;
}


function mainView() { return _initObj.mainView; }
function globalApp() { return _initObj.globalApp; }

function switchToDiagHeader(page) {
    if (!page.visible)
        return;
    if (!globalApp().header || !globalApp().header.name !== "DiagTabs") {
        if (!_initObj.diagHeader) {
            var comp = Qt.createComponent("DiagHeader.qml");
            _initObj.diagHeader = comp.createObject(globalApp());
        }
        globalApp().header = _initObj.diagHeader;
        _initObj.diagHeader.visible = true;
        mainView().anchors.top = globalApp().header.bottom;
        _initObj.diagHeader.anchors.right = globalApp.right;
        _initObj.diagHeader.anchors.left = globalApp.left;
        _initObj.diagHeader.anchors.top = globalApp.top;
    }
}

function hideDiagHeader() {
    _initObj.diagHeader.visible = false;
    globalApp().header = null;
}

function pushPage(strPageObj) {
    var comp = Qt.createComponent(strPageObj + ".qml");
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
        if (_lastPopped && mainView().currntItem !== _lastPopped)
            mainView().push(_lastPopped);
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
