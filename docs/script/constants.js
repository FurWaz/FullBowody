/* WEBSITE CONSTANTS */
let PAGE = {
    HOME: 1,
    GET_STARTED: 2,
    DOWNLOAD: 3,
    ABOUT: 4,
    LAST_PAGE_ID: 1
}
let CODE = {
    PYTHON: 1,
    CSHARP: 2,
    CPP: 3,
}

let ANIMATION_TIME = 200;
let MOUSE_POS = {X: 0, Y: 0};

let CONTAINER = document.getElementById("main-container")

window.onmousemove = ev => {MOUSE_POS = {X: ev.x, Y: ev.y};};

/* BASIC CONTENT MANAGEMENT FUNCTIONS */
function clearContent(div=CONTAINER) {
    while (div.firstChild)
        div.firstChild.remove();
}

function createElement(_type="div", _id=null, _class=null, _content=null, _callback=null, ..._elements) {
    let el = document.createElement(_type);
    if (_id != null)
        el.id = _id;
    if (_class != null)
        _class.split(" ").forEach(c => {
            el.classList.add(c);
        });
    if (_elements != null)
        _elements.forEach(e => {
            el.appendChild(e);
        });
    if (_callback != null)
        el.onclick = _callback;
    if (_content != null)
        el.innerHTML = _content;
    return el;
}

function createTitle(str="", _id=null, _class=null) {
    let el = createElement("div", _id, "title-container "+_class, null, null,
        createElement("div", null, "title-bar"),
        createElement("p", null, "title-text", str)
    );
    return el;
}

function createIcon(src="./resources/icon.png", _id=null, _class=null) {
    let el = createElement("img", _id, _class, null, null);
    el.src = src;
    return el;
}

function createText(str="", _id=null, _class=null) {
    let el = createElement("p", _id, _class, str, null);
    return el;
}

function createInfo(str="Info", x=MOUSE_POS.X, y=MOUSE_POS.Y) {
    let el = createElement("div", null, "info-popup-container", null, null,
        createText(str, null, "info-popup-text")
    );
    setTimeout(() => {
        el.classList.add("info-popup-disappear");
        setTimeout(() => {
            el.remove();
        }, 200);
    }, 2000);
    document.body.appendChild(el);
    el.style.left = (x-el.getBoundingClientRect().width/2)+"px";
    el.style.top = (y+window.pageYOffset-el.getBoundingClientRect().height-10)+"px";
    return el;
}

function createSpacer(size=10) {
    let el = createElement("div");
    el.style.width = size+"px";
    el.style.height = size+"px";
    el.style.backgroundColor = "transparent";
    return el;
}

function moveContentLeft() {
    CONTAINER.style.transform = "translateX(-20%)";
    CONTAINER.style.opacity = "0";
    setTimeout(() => {
        CONTAINER.style.transition = "none";
        CONTAINER.style.transform = "translateX(20%)";
        CONTAINER.style.opacity = "0";
        setTimeout(() => {
            CONTAINER.style.transform = "translateX(0%)";
            CONTAINER.style.opacity = "1";
            CONTAINER.style.transition = "transform 100ms ease, opacity 100ms ease";
        }, 10);
    }, ANIMATION_TIME/2-10);
}

function moveContentRight() {
    CONTAINER.style.transform = "translateX(20%)";
    CONTAINER.style.opacity = "0";
    setTimeout(() => {
        CONTAINER.style.transition = "none";
        CONTAINER.style.transform = "translateX(-20%)";
        CONTAINER.style.opacity = "0";
        setTimeout(() => {
            CONTAINER.style.transform = "translateX(0%)";
            CONTAINER.style.opacity = "1";
            CONTAINER.style.transition = "transform 100ms ease, opacity 100ms ease";
        }, 10);
    }, ANIMATION_TIME/2-10);
}

function downloadFile(src) {
    document.getElementById("file-downloader").src = src;
}

function isInList(sample="", list=[])
{
    for(let i = 0; i < list.length; i++)
        if (list[i] == sample)
            return true;
    return false;
}

function createCode(lang=CODE.PYTHON, code="") {
    let commentChar, keyWords;
    switch (lang) {
        case CODE.PYTHON:
            commentChar = "#";
            keyWords = ["if", "else", "def", "continue", "break", "for", "while"];
            break;
        case CODE.CSHARP:
            commentChar = "//";
            keyWords = ["if", "else", "void", "continue", "break", "for", "while"];
            break;
        case CODE.CPP:
            commentChar = "//";
            keyWords = ["if", "else", "void", "continue", "break", "for", "while"];
            break;
        default:
            break;
    }
    let program = [];
    let lines = code.split("\n");
    for(let i = 0; i < lines.length; i++) {
        let line = lines[i];
        if (line.length < 1) continue;
        let newLine = "";
        while (line.startsWith("  "))
        {
            newLine += "<span class='shift'></span>";
            line = line.substring(2, line.length);
        }
        let words = line.split(" ");
        if (line.startsWith(commentChar))
            newLine = "<span style='color: var(--color-comment);'>"+line+"</span>";
        else
            for (let j = 0; j < words.length; j++) {
                let word = words[j];
                let res = word.match(/^[a-z][A-Za-z]*\(/);
                if (res != null)
                {
                    word = "<span style='color: var(--color-function);'>"+
                           word.substr(res.index, res[0].length-1)+"</span>"+
                           word.substring(res.index+res[0].length-1, word.length);
                }
                res = word.match(/^[A-Z][A-Za-z]*/);
                if (res != null)
                {
                    word = "<span style='color: var(--color-object);'>"+
                           word.substr(res.index, res[0].length)+"</span>"+
                           word.substring(res.index+res[0].length, word.length);
                }
                res = word.match(/\.[A-Za-z]*\(/);
                if (res != null)
                {
                    word = word.substr(0, res.index)+".<span style='color: var(--color-function);'>"+
                           word.substr(res.index+1, res[0].length-2)+"</span>("+
                           word.substring(res.index+res[0].length, word.length);
                }
                if (isInList(word, keyWords))
                    word = "<span style='color: var(--color-keyword);'>"+word+"</span>";
                newLine += word+" ";
            }
        newLine = newLine.substr(0, newLine.length-1);
        let res = newLine.match(/"[A-Za-z !:;,]*"/);
        console.log("res: "+res+" on line: "+newLine)
        if (res != null)
        {
            newLine = newLine.substring(0, res.index)+
                    "<span style='color: var(--color-string);'>"+
                    newLine.substring(res.index, res.index+res[0].length)+"</span>"+
                    newLine.substring(res.index+res[0].length, newLine.length);
        }
        program.push(newLine);
    }
    
    let el = createElement("div", null, "codezone");
    program.forEach(l => {
        el.appendChild(createText(l));
    });
    return el;
}