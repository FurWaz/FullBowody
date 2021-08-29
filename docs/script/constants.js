/* WEBSITE CONSTANTS */
let PAGE = {
    HOME: 1,
    GET_STARTED: 2,
    DOWNLOAD: 3,
    ABOUT: 4,
    LAST_PAGE_ID: 1
}

let ANIMATION_TIME = 200;

let CONTAINER = document.getElementById("main-container")

/* BASIC CONTENT MANAGEMENT FUNCTIONS */
function clearContent(div=CONTAINER)
{
    while (div.firstChild)
        div.firstChild.remove();
}

function createElement(_type="div", _id=null, _class="", _content=null, _callback=null, ..._elements)
{
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

function createTitle(str="", _id=null, _class=null)
{
    let el = createElement("div", _id, "title-container "+_class, null, null,
        createElement("div", null, "title-bar"),
        createElement("p", null, "title-text", str)
    );
    return el;
}

function createIcon(src="./resources/icon.png", _id=null, _class=null)
{
    let el = createElement("img", _id, _class, null, null);
    el.src = src;
    return el;
}

function createText(str="", _id=null, _class=null)
{
    let el = createElement("p", _id, _class, str, null);
    return el;
}

function moveContentLeft()
{
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

function moveContentRight()
{
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