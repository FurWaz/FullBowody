function loadHome() {
    clearContent();
    CONTAINER.appendChild(
        createElement("div", null, "align-x", null, null, 
            createElement("h2", null, "text-left", "A free full body tracking app using HD cameras for VR, 3D animation, etc.")
        )
    );
    CONTAINER.appendChild(createSpacer(50));
    CONTAINER.appendChild(createTitle("Sorry, this website is still in construction"));
}