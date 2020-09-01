from http.server import BaseHTTPRequestHandler, HTTPServer
import os

port = int(os.environ.get('PORT', 8080))

class LoggingServer(BaseHTTPRequestHandler):
    def _send_response(self):
        self.send_response(204)
        self.end_headers()
        self.wfile.write(''.encode('utf-8'))

    def do_GET(self):
        print("GET " + str(self.path))
        self._send_response()

    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length).decode('utf-8')
        headers = self.headers;
        print("---\nPOST " + str(self.path) + "\n" + str(headers) + post_data)
        self._send_response()

    def log_message(self, format, *args):
        return

httpd = HTTPServer(('', port), LoggingServer)
httpd.serve_forever()

