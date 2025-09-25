const http = require('http');
const fs = require('fs');
const path = require('path');

const port = 5000;

// MIME types for different file extensions
const mimeTypes = {
  '.html': 'text/html',
  '.css': 'text/css',
  '.js': 'application/javascript',
  '.json': 'application/json',
  '.png': 'image/png',
  '.jpg': 'image/jpeg',
  '.gif': 'image/gif',
  '.ico': 'image/x-icon'
};

const server = http.createServer((req, res) => {
  // Parse the URL
  let filePath = req.url;
  
  // Default to index.html for root requests
  if (filePath === '/') {
    filePath = '/index.html';
  }
  
  // Get the full file path
  const fullPath = path.join(__dirname, filePath);
  
  // Get file extension for MIME type
  const ext = path.extname(filePath);
  const contentType = mimeTypes[ext] || 'text/plain';
  
  // Set headers to disable caching for development
  res.setHeader('Cache-Control', 'no-cache, no-store, must-revalidate');
  res.setHeader('Pragma', 'no-cache');
  res.setHeader('Expires', '0');
  
  // Read and serve the file
  fs.readFile(fullPath, (err, data) => {
    if (err) {
      // File not found
      res.writeHead(404, { 'Content-Type': 'text/html' });
      res.end('<h1>404 - File Not Found</h1>');
      return;
    }
    
    // File found, serve it
    res.writeHead(200, { 'Content-Type': contentType });
    res.end(data);
  });
});

server.listen(port, '0.0.0.0', () => {
  console.log(`🚀 Mohavim web interface running on http://0.0.0.0:${port}`);
  console.log(`📝 Serving files from: ${__dirname}`);
  console.log(`💻 Terminal editor available at: ./src/mohavim`);
});