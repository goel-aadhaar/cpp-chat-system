const WebSocket = require("ws");
const net = require("net");

const wss = new WebSocket.Server({ port: 8080 });

wss.on("connection", (ws) => {
    console.log("Browser connected");

    // Connect to C++ TCP server
    const tcpClient = net.createConnection(
        { host: "127.0.0.1", port: 9099 },
        () => console.log("Connected to C++ server")
    );

    tcpClient.on("error", (err) => {
        console.error("TCP Client Error:", err.message);
        ws.close();
    });

    tcpClient.on("close", () => {
        console.log("TCP connection closed");
        ws.close();
    });

    // Browser → C++ server
    ws.on("message", (message) => {
        if (!tcpClient.destroyed) {
            tcpClient.write(message + "\n");
        }
    });

    // C++ server → Browser
    tcpClient.on("data", (data) => {
        if (ws.readyState === WebSocket.OPEN) {
            ws.send(data.toString());
        }
    });

    ws.on("close", () => {
        console.log("WebSocket client disconnected");
        if (!tcpClient.destroyed) {
            tcpClient.end();
        }
    });
});
