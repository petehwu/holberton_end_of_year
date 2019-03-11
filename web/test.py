from flask import Flask
app = Flask(__name__)

@app.route('/',strict_slashes=False)
def hello():
    return "<h1 style='color:blue'>Boo YAH hi two</h1>"

if __name__ == "__main__":
    app.run(host='0.0.0.0')
