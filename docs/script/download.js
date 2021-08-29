function loadDownload() {
    clearContent();
    CONTAINER.appendChild(createTitle("Download"));
    CONTAINER.appendChild(createText("FullBowody's application binaries", null, "subtitle"));
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
    CONTAINER.appendChild(createText("FullBowody's addons scripts", null, "subtitle"));
    CONTAINER.appendChild(
        createElement("div", null, "align-x obj-container", null, null, 
            createElement("div", null, "align-y item-container", null, dl_python,
                createIcon("./resources/Python.png", null, "item-icon"),
                createText("Python", null, "item-text")
            ),
            createElement("div", null, "align-y item-container", null, dl_csharp,
                createIcon("./resources/Csharp.png", null, "item-icon"),
                createText("C#", null, "item-text")
            ),
            createElement("div", null, "align-y item-container", null, dl_cpp,
                createIcon("./resources/Cpp.png", null, "item-icon"),
                createText("C/C++", null, "item-text")
            )
        )
    );
    CONTAINER.appendChild(createTitle("Sources"));
    CONTAINER.appendChild(createText("FullBowody's source code", null, "subtitle"));
    CONTAINER.appendChild(
        createElement("div", null, "align-x obj-container", null, null, 
            createElement("div", null, "align-y item-container", null, ()=>{location.href = "https://github.com/furWaz/fullbowody";},
                createIcon("./resources/Github.png", null, "item-icon"),
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

function dl_python()
{
    alert("Sorry, the Python addon is not available yet");
}

function dl_csharp()
{
    alert("Sorry, the C# addon is not available yet");
}

function dl_cpp()
{
    alert("Sorry, the C/C++ addon is not available yet");
}