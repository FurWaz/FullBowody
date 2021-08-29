function loadDownload() {
    clearContent();
    CONTAINER.appendChild(createTitle("Download"));
    CONTAINER.appendChild(
        createElement("div", null, "align-x obj-container", null, null, 
            createElement("div", null, "align-y item-container", null, dl_linux,
                createIcon("./resources/linux.png", null, "item-icon"),
                createText("Linux", null, "item-text")
            ),
            createElement("div", null, "align-y item-container", null, dl_windows,
                createIcon("./resources/windows.png", null, "item-icon"),
                createText("Windows", null, "item-text")
            )
        )
    );
    CONTAINER.appendChild(createTitle("Addons"));
    CONTAINER.appendChild(
        createElement("div", null, "align-x obj-container", null, null, 
            createElement("div", null, "align-y item-container", null, dl_linux,
                createIcon("./resources/windows.png", null, "item-icon"),
                createText("Python", null, "item-text")
            ),
            createElement("div", null, "align-y item-container", null, dl_windows,
                createIcon("./resources/windows.png", null, "item-icon"),
                createText("C/C++", null, "item-text")
            )
        )
    );
    CONTAINER.appendChild(createTitle("Sources"));
    CONTAINER.appendChild(
        createElement("div", null, "align-x obj-container", null, null, 
            createElement("div", null, "align-y item-container", null, ()=>{window.location.href = "https://github.com/furWaz/fullbowody";},
                createIcon("./resources/windows.png", null, "item-icon"),
                createText("Github", null, "item-text")
            )
        )
    );
}

function dl_windows()
{
    alert("Sorry the windows executable is not available yet");
}

function dl_linux()
{
    alert("Sorry, the linux executable is not available yet");
}