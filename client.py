import requests

def shorten_url(url):
    # URL of the shortening service
    service_url = 'http://127.0.0.1:5000/'

    # Send a POST request to shorten the URL
    response = requests.post(service_url, data={'url': url})

    if response.status_code == 200:
        # Print the shortened URL
        return response.text
    else:
        return "Error"

def redirect_to_original(short_key):
    # URL of the shortening service with the short key
    service_url = f'http://127.0.0.1:5000/{short_key}'

    # Send a GET request to redirect to the original URL
    response = requests.get(service_url, allow_redirects=False)

    if response.status_code == 302:
        # Extract the absolute URL from the 'Location' header
        redirect_url = response.headers.get('Location')
        print("Redirecting to the original URL:", redirect_url)
    else:
        print(response.text)

if __name__ == '__main__':
    original_url = input("Input the URL: ")
    shortened = shorten_url(original_url)
    if shortened != "Error":
        print("Shortened URL:", shortened)
        full_shortened = f"http://127.0.0.1:5000/{shortened}"
        print("Full shortened URL:", full_shortened)

        # Redirect to the original URL
        redirect_to_original(shortened)
    else:
        print("Error")