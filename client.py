from flask import Flask, request, redirect
import random
import string

app = Flask(__name__)

# Словарь для хранения сокращенных ссылок
url_mapping = {}

def generate_short_url():
    # Генерация случайной короткой строки
    return ''.join(random.choices(string.ascii_letters + string.digits, k=6))

@app.route('/', methods=['POST'])
def shorten_url():
    long_url = request.form.get('url')

    if not long_url:
        return "Error: No URL provided", 400

    # Генерация короткой ссылки
    short_url = generate_short_url()

    # Сохранение соответствия между короткой и длинной ссылкой
    url_mapping[short_url] = long_url

    return short_url

@app.route('/<short_url>', methods=['GET'])
def redirect_to_original(short_url):
    # Получение длинной ссылки по короткой
    long_url = url_mapping.get(short_url)

    if long_url:
        return redirect(long_url)
    else:
        return "Error: URL not found", 404

if __name__ == '__main__':
    app.run(debug=True)