class SimpleHTMLParser:
    def __init__(self, html):
        self.html = html.strip() #Remove leading and trailing spaces
        self.tags = [] #open & close
        self.data = []
        self.current_tag = None #curr open tag

    def parse(self):
        i = 0
        while i < len(self.html):
            if self.html[i] == "<":
                tag_end = self.html.find(">", i)
                if tag_end == -1:
                    break
                tag = self.html[i+1:tag_end].strip() #extract tag between <>

                if tag[0] == "/":
                    self.tags.append(f"End tag: {tag[1:].strip()}")
                    self.current_tag = None
                else:
                    self.tags.append(f"Start tag: {tag}")
                    self.current_tag = tag

                i = tag_end
            else:
                data_start = i
                data_end = self.html.find("<", i)
                if data_end == -1:
                    data_end = len(self.html)
                data = self.html[data_start:data_end].strip()
                if data and self.current_tag:
                    self.data.append(f"{self.current_tag}: {data}")
                i = data_end - 1
            i += 1

        return {'tags': self.tags, 'data': self.data}

# html_string = "<html><head><title>My Title</title></head><body><h1>Hello, World!</h1></body></html>"
htmlString="""<html>
    <body>
        <h1>Welcome to My Page</h1>
        <p>This is a simple paragraph.</p>
    </body>
</html>"""

parser = SimpleHTMLParser(htmlString)
parsed_html = parser.parse()
print("Tags:")
for tag in parsed_html['tags']:
    print(tag)

print("\nData:")
for data in parsed_html['data']:
    print(data)
