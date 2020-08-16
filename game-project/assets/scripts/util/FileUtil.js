export function loadShaderSourceCodes(fileName) {
    let xhr = new XMLHttpRequest();
    let okStatus = document.location.protocol === "http:" ? 0 : 200;
    // console.log(okStatus);
    xhr.open('GET', fileName, false);
    xhr.overrideMimeType("text/plain;charset=utf-8");//默认为utf-8
    xhr.send();
    return xhr.status === okStatus ? xhr.responseText : null;
}