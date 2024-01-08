function loadDownload() {
    clearContent();
    CONTAINER.appendChild(createTitle("Download"));
    CONTAINER.appendChild(createText("FullBowody's application binaries", null, "subtitle"));
    CONTAINER.appendChild(
        createElement("div", null, "align-x obj-container", null, null, 
            createElement("div", "dl_linux", "align-y item-container", null, dl_linux,
                createIcon("./resources/linux.png", null, "item-icon"),
                createText("Linux", null, "item-text")
            ),
            createElement("div", "dl_windows", "align-y item-container", null, dl_windows,
                createIcon("./resources/windows.png", null, "item-icon"),
                createText("Windows", null, "item-text")
            )
        )
    );
    CONTAINER.appendChild(createTitle("Addons"));
    CONTAINER.appendChild(createText("FullBowody's addons scripts", null, "subtitle"));
    CONTAINER.appendChild(
        createElement("div", null, "align-x obj-container", null, null, 
            createElement("div", "dl_python", "align-y item-container", null, dl_python,
                createIcon("./resources/Python.png", null, "item-icon"),
                createText("Python", null, "item-text")
            ),
            createElement("div", "dl_csharp", "align-y item-container", null, dl_csharp,
                createIcon("./resources/Csharp.png", null, "item-icon"),
                createText("C#", null, "item-text")
            ),
            createElement("div", "dl_cpp", "align-y item-container", null, dl_cpp,
                createIcon("./resources/Cpp.png", null, "item-icon"),
                createText("C++", null, "item-text")
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
    createInfo("Sorry, this binary is not available yet.");
    // downloadFile("./downloads/FullBowody_win64.zip");
}

function dl_linux()
{
    createInfo("Sorry, this binary is not available yet.");
    // downloadFile("./downloads/FullBowody_linux64.zip");
}

function dl_python()
{
    downloadFile("./downloads/FullBowody.py");
}

function dl_csharp()
{
    createInfo("Sorry, this addon is not available yet.");
    // downloadFile("./downloads/FullBowody.cs");
}

function dl_cpp()
{
    createInfo("Sorry, this addon is not available yet.");
    // downloadFile("./downloads/FullBowody.hpp");
}