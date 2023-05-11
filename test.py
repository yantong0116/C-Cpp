from bs4 import BeautifulSoup
import requests

html = requests.get('https://github.com/yantong0116/C-Cpp-Learning/settings').content

soup = BeautifulSoup(html, 'html.parser')
print(soup.encode("utf-8"))

# quote = float(soup.find(id='clipboard-copy-content-win-create-the-runner-and-start-the-configuration-experience').get_text())
# quote = soup.find(id='js-global-screen-reader-notice').get_text()

# print(quote)

# print(soup)

