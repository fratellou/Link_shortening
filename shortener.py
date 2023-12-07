from flask import Flask, request, redirect
import socket
import hashlib

app = Flask(__name__)

def database(query):  # accessing the database
    host = '127.0.0.1'
    port = 6379
    # Creating a TCP socket and establishing a connection
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, port))
    sock.sendall(query.encode())  # sending a request
    data = sock.recv(1024)  # receiving a response
    sock.close()  # closing the socket
    return data.decode('utf-8')

@app.route('/', methods=['POST'])
def shorten_url():
    long_url = request.form.get('url')
    if not long_url:
        return "Error: No URL provided", 400

    # Generate a unique short key using SHA-1 hash
    short_key = hashlib.sha1(long_url.encode()).hexdigest()[:6]
    # Store the mapping in-memory (replace this with a database)
    req = f"HSET hash {short_key} {long_url}\0"
    database(req)

    return short_key

'''@app.route('/<short_key>', methods=['GET'])
def redirect_to_original(short_key):
    # Retrieve the original URL from the mapping
    req2 = f"HGET hash {short_key}\0"
    long_url = database(req2)
    print("\nLONG_URL GET: ", long_url)

    if long_url:
        return redirect(long_url, code=302)
    else:
        return "Error: URL not found", 404'''
        

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)