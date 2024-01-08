function loadGetStarted() {
    clearContent();
    CONTAINER.appendChild(createTitle("Install FullBowody's binaries"));
    CONTAINER.appendChild(
        createElement("div", null, "align-x obj-container", null, null, 
            createElement("div", null, "align-y text-chunk", null, null,
                createText("1", null, "number-counter"),
                createText(
                    "Download FullBowody's binaries from this website\
                    and extract the .zip file where you want the application to be installed",
                    null, "text-chunk"
                )
            ),
            createElement("div", null, "align-y text-chunk", null, null,
                createText("2", null, "number-counter"),
                createText(
                    "If you want VR full body tracking, edit the <q>openvrpaths.vrpath</q> file at <br>\
                    <code>C:/Users/[User]/AppData/Local/openvr</code><br> and add the path to the FullBowody's\
                    openvr addon folder. <br><br> For example, if FullBowody's root folder is <br>\
                    <code>D:/fullbowody/</code><br> when you will have to add <br><code>D:/fullbowody/addons/openvr/plugin</code><br>\
                    to the <q>external_drivers</q> section of the file.",
                    null, "text-chunk"
                )
            ),
            createElement("div", null, "align-y text-chunk", null, null,
                createText("3", null, "number-counter"),
                createText(
                    "Launch the FullBowody's binary file to run the app<br><br>\
                    Please make sure FullBowody's application is running <span class='highlight'>before</span> you start SteamVR,\
                    otherwise the OpenVR plugin will struggle connecting to the app",
                    null, "text-chunk"
                )
            )
        )
    );
    CONTAINER.appendChild(createSpacer(50));
    CONTAINER.appendChild(createTitle("How to use FullBowody's addons ?"));
    CONTAINER.appendChild(
        createElement("div", null, "restrict-x", null, null,
            createText("Every FullBowody's addon has one class: FBConnection", null, "text-center text-chunk"),
            createText(
                "To connect to the FullBowody's app from your script, simply create a FBConnection instance,\
                and call the <code>.start()</code> method on it to start the connection", null, "text-left text-chunk"
            ),
            createText(
                "Now you are connected to the FullBowody's application. You can get the bodyPosition at any \
                time using the <code>.getBodyPosition()</code> method, this will return a list of points \
                corresponding to the position of each body joints.", null, "text-left text-chunk"
            ),
            createText(
                "When your program is finished, don't forget to call the <code>.stop()</code> method to\
                disconnect your program from FullBowody's app.", null, "text-left text-chunk"
            )
        )
    );
    CONTAINER.appendChild(
        createElement("div", null, "align-x obj-container", null, null,
            createElement("div", null, "align-y", null, null,
                createText("Python", null, "cat-name"),
                createCode(CODE.PYTHON, pythonDemo)
            ),
            createElement("div", null, "align-y", null, null,
                createText("C#", null, "cat-name"),
                createCode(CODE.CSHARP, csharpDemo)
            ),
            createElement("div", null, "align-y", null, null,
                createText("C++", null, "cat-name"),
                createCode(CODE.CSHARP, cppDemo)
            )
        )
    );
}

let pythonDemo = `
# First, create the connection
conn = FBConnection("My addon")

# [OPTIONAL] You can define a callback function that will be called when a new body position is recieved
def onNewPosition():
    print("New body position !")
conn.setCallback(onNewPosition)

# Next, start the connection
conn.start()

# Now, you can get the current body position at anytime
print(
    "Current body position: ",
    conn.getBodyPosition()
)

# At the end of the program, don't forget to stop the connection
conn.stop()
`;

let csharpDemo = `
// First, create the connection
FBConnection conn = new FBConnection("My addon");

// [OPTIONAL] You can define a callback function that will be called when a new body position is recieved
void onNewPosition() {
    Console.Write("New body position !");
}
conn.setCallback(onNewPosition);

// Next, start the connection
conn.start();

// Now, you can get the current body position at anytime
Console.Write(
    "Current body position: "+
    conn.getBodyPosition()
);

// At the end of the program, don't forget to stop the connection
conn.stop();
`;

let cppDemo = `
// First, create the connection
FBConnection conn("My addon");

// [OPTIONAL] You can define a callback function that will be called when a new body position is recieved
void onNewPosition()
{
    std::cout << "New body position !"
              << std::endl;;
}
conn.setCallback(onNewPosition);

// Next, start the connection
conn.start();

// Now, you can get the current body position at anytime
std::cout << "Current body position: "
          << conn.getBodyPosition()
          << std::endl;

// At the end of the program, don't forget to stop the connection
conn.stop();
`;