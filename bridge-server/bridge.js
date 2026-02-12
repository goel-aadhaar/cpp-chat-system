const WebSocket = require("ws");
const net = require("net");

const wss = new WebSocket.Server({ port: 8080 });

wss.on("connection", (ws) => {
    console.log("Browser connected");

    // Connect to your C++ TCP server
    const tcpClient = net.createConnection(
        { host: "127.0.0.1", port: 9099 },
        () => console.log("Connected to C++ server")
    );

    // Browser → C++ server
    ws.on("message", (message) => {
        tcpClient.write(message + "\n");
    });

    // C++ server → Browser
    tcpClient.on("data", (data) => {
        ws.send(data.toString());
    });

    ws.on("close", () => {
        tcpClient.end();
    });
});
