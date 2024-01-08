/* HEADER TEXT CENTERING */
let header_title = document.getElementById("header-title");
window.addEventListener("scroll", ev => {
    let shift = window.scrollY/2;
    header_title.style.transform = "translateY("+shift+"px)";
});

/* HISTORY MANAGEMENT */
function loadContent(id=PAGE.HOME, saveState=true) {
    // change header size
    let header = document.getElementById("header-container");
    if (id == PAGE.HOME)
        header.style.height = "var(--header-size-max)";
    else  header.style.height = "var(--header-size-min)";

    if (id < PAGE.LAST_PAGE_ID)
        moveContentRight();
    else
        moveContentLeft();

    setTimeout(() => {
        switch (id) {
            case PAGE.HOME:
                loadHome();
                break;
            case PAGE.GET_STARTED:
                loadGetStarted();
                break;
            case PAGE.DOWNLOAD:
                loadDownload();
                break;
            case PAGE.ABOUT:
                loadAbout();
                break;
        
            default:
                break;
        }
    }, ANIMATION_TIME/2);

    PAGE.LAST_PAGE_ID = id;
    CONTAINER.style.zIndex = 0;
    document.getElementById("menu-container").style.zIndex = "10";
    if (saveState)
        history.pushState({}, "FullBowody", "?page="+id2str(id));
}
function id2str(id=PAGE.HOME)
{
    switch (id) {
        case PAGE.HOME:
            return "home";
        case PAGE.GET_STARTED:
            return "getstarted";
        case PAGE.DOWNLOAD:
            return "download";
        case PAGE.ABOUT:
            return "about";
    }
    return "";
}
function str2id(str="home")
{
    switch (str) {
        case "home":
            return PAGE.HOME;
        case "getstarted":
            return PAGE.GET_STARTED;
        case "download":
            return PAGE.DOWNLOAD;
        case "about":
            return PAGE.ABOUT;
    }
    return PAGE.HOME;
}
window.onpopstate = ev => {
    let url = new URL(location.href);
    let strPage = url.searchParams.get("page");
    loadContent(str2id(strPage), false);
};

window.onpopstate();