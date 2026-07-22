const http = require('http');
const fs = require('fs');
const path = require('path');

const PORT = process.env.PORT || 3000;
const rootDir = path.resolve(__dirname);
const webDir = path.join(rootDir, 'web');

const mimeTypes = {
  '.html': 'text/html',
  '.css': 'text/css',
  '.js': 'application/javascript',
  '.json': 'application/json',
  '.txt': 'text/plain',
  '.png': 'image/png',
  '.jpg': 'image/jpeg',
  '.jpeg': 'image/jpeg',
  '.svg': 'image/svg+xml',
};

const sendResponse = (res, status, content, contentType) => {
  res.writeHead(status, {
    'Content-Type': contentType,
    'Access-Control-Allow-Origin': '*',
  });
  res.end(content);
};

const serveStatic = (filePath, res) => {
  fs.readFile(filePath, (err, content) => {
    if (err) {
      sendResponse(res, 404, 'File not found', 'text/plain');
      return;
    }

    const ext = path.extname(filePath).toLowerCase();
    const contentType = mimeTypes[ext] || 'application/octet-stream';
    sendResponse(res, 200, content, contentType);
  });
};

const server = http.createServer((req, res) => {
  const requestUrl = decodeURIComponent(req.url.split('?')[0] || '/');

  if (requestUrl === '/save-data' && req.method === 'POST') {
    let body = '';
    req.on('data', (chunk) => {
      body += chunk;
    });
    req.on('end', () => {
      const filePath = path.join(rootDir, 'data.txt');
      fs.writeFile(filePath, body, 'utf8', (err) => {
        if (err) {
          sendResponse(res, 500, 'Failed to save data.txt', 'text/plain');
          return;
        }
        sendResponse(res, 200, 'Saved data.txt', 'text/plain');
      });
    });
    return;
  }

  if (requestUrl === '/data.txt') {
    const dataFilePath = path.join(rootDir, 'data.txt');
    if (!fs.existsSync(dataFilePath)) {
      sendResponse(res, 404, 'data.txt not found', 'text/plain');
      return;
    }
    serveStatic(dataFilePath, res);
    return;
  }

  let filePath = requestUrl;
  if (filePath === '/') {
    filePath = '/index.html';
  }
  if (filePath.startsWith('/web/')) {
    filePath = filePath.slice(4);
  }

  const staticFilePath = path.join(webDir, filePath);
  serveStatic(staticFilePath, res);
});

server.listen(PORT, () => {
  console.log(`Server running at http://localhost:${PORT}/`);
  console.log('Serving web files from', webDir);
});
