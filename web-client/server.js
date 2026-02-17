const express = require('express');
const path = require('path');
const app = express();
const port = process.env.PORT || 3000;

app.use(express.static(__dirname));

app.get('/api/config', (req, res) => {
    res.json({
        wsUrl: process.env.WS_URL || 'ws://localhost:8080'
    });
});

app.get('*', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
});

app.listen(port, () => {
    console.log(`Frontend server running on port ${port}`);
});
