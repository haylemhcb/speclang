import http.server
import socketserver
import argparse
import threading

# Configurar el parser de argumentos
parser = argparse.ArgumentParser(description='Iniciar un servidor CGI en el puerto especificado.')
parser.add_argument('port', type=int, help='El puerto en el que el servidor escuchará')

args = parser.parse_args()
PORT = args.port

class Handler(http.server.SimpleHTTPRequestHandler):
    # Habilitar el manejo de CGI
    def do_GET(self):
        if self.path.startswith('/cgi-bin/'):
            self.cgi = True
            self.path = self.path[9:]  # Eliminar '/cgi-bin/' de la ruta
        return super().do_GET()

def run_server():
    # Configurar el directorio CGI
    handler = Handler
    handler.cgi_directories = ['/cgi-bin']

    with socketserver.TCPServer(("", PORT), handler) as httpd:
        print(f"Servidor CGI en ejecución en http://localhost:{PORT}")
        httpd.serve_forever()

# Iniciar el servidor en un hilo separado
server_thread = threading.Thread(target=run_server)
server_thread.start()

# Esperar a que se presione Enter para cerrar el servidor
input("Presiona Enter para cerrar el servidor...\n")

# Cerrar el servidor
print("Cerrando el servidor...")
server_thread.join(timeout=1)  # Esperar a que el hilo del servidor termine
