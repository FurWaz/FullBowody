let http = require("http");
let fs = require("fs")
let server = http.createServer({}, (req, res) => {
    if (req.url == "/")
        req.url = "/index.html";
    try {res.write(fs.readFileSync(__dirname+req.url, {encoding: "utf-8"}));}
    catch {}
    res.end();
});

server.listen(8080);