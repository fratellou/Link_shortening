from flask import Flask, request, redirect
import hashlib
import socket
DB_HOST = '127.0.0.1'
DB_PORT = 6379
app = Flask(__name__)

def save_to_database(short_key, long_url):
    # Connect to the database server
    db_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    db_socket.connect((DB_HOST, DB_PORT))

    command = f'HSET hash {short_key} {long_url}\0' # Prepare the command to save data in the database
    db_socket.sendall(command.encode()) # Send the command to the database server
    db_socket.close() # Close the connection

def get_from_database(short_key):
    # Connect to the database server
    db_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    db_socket.connect((DB_HOST, DB_PORT))

    
    command = f'HGET hash {short_key}\0' # Prepare the command to get data from the database
    db_socket.sendall(command.encode()) # Send the command to the database server and receive the response
    response = db_socket.recv(1024).decode()
    db_socket.close() # Close the connection

    return response.strip()

@app.route('/', methods=['POST'])
def shorten_url():
    long_url = request.form.get('url')
    if not long_url:
        return "Error: No URL provided", 400

    # Generate a unique short key using SHA-1 hash
    short_key = hashlib.sha1(long_url.encode()).hexdigest()[:6]
    
    # Store the mapping in-memory
    save_to_database(short_key, long_url)

    return short_key

@app.route('/<short_key>', methods=['GET'])
def redirect_to_original(short_key):
    # Retrieve the original URL from the mapping
    long_url = get_from_database(short_key)

    if long_url:
        return redirect(long_url, code=302)
    else:
        return "Error: URL not found", 404

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)