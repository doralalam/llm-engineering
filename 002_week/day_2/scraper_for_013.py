from bs4 import BeautifulSoup
import requests

# Standard headers to fetch a website
headers = {
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/117.0.0.0 Safari/537.36"
}

# Without Useer-Agent, the server may think the request came from a script and reject it or return a minimal page
# A User-Agent header tells the website that the request is coming from a browser like client

def fetch_website_contents(url):
    '''
    Return the title and contents of the website at the given url;
    truncate to 2000 characters as a sensible limit
    '''
    response = requests.get(url, headers=headers)
    soup = BeautifulSoup(response.content, "html.parser")           # Parses the html content of the webpage and BeautifulSoup turns the raw HTML into a structure Python can work with
    title = soup.title.string if soup.title else "No title found"   # If the webpage has <title> tag, it takes its text, otherwise "No title found"
    if soup.body:                                                   # If the webpage has <body> tag, then the code insider runs
        for irrelevant in soup.body({'script', 'style', 'img', 'input'}):   # Identifies the undwanted elements like <script>, <style>, <img>, <input> etc.,
            irrelevant.decompose()                                  # Remove each unwanted element from the html tree
        text = soup.body.get_text(separator='\n', strip=True)       # Extract all visible text from the page body, separator='\n' inserts a newline between text blocks and strip removes the extra whitespaces at the beginning and end
    else:                                                           # If the page doesn't have the <body> tag, then empty string is considered as text
        text=""
    return (title + "\n\n" + text)[:2000]                           # Combines the title and body text, then cuts the result into first 2000 characters


def fetch_website_links(url):
    """
    Return the links on the website at the given url
    """
    response = requests.get(url, headers=headers)
    soup = BeautifulSoup(response.content, "html.parser")
    links = [link.get("href") for link in soup.find_all("a")]       # returns a list of all link elements with <a> anchor tag and for each link, extract the value of href attribute
    return [link for link in links if link]                         # return links that are not empty