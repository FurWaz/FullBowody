let express = require("express")();
express.get("/*", (req, res) => {
    if (req.url == "/") req.url = "/index.html";
    let path = (__dirname+req.url).split("?")[0];
    res.sendFile(path);
});
express.listen(80);