const win = require('./build/Release/windows.node');
const child_process = require('child_process');

var child = child_process.spawn('notepad.exe');
setTimeout(function() {
    var hwnd = win.getMainHWndByPID(child.pid);
    setTimeout(function() {
        win.moveWindow(hwnd,0,0,500,400);
    }, 1000);
}, 1000);
