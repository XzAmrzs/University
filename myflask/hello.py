from flask import Flask
app=Flask(__name__)
@app.route('/')
def index():
    return '<h1>Hello World!<h1>'

@app.route('/user/<name>')
def user(name):
    return '<h1>Hello %s!<h1>' %name
from flask import redirect
@app.route('/user')
def redir():
    return redirect('http://www.baidu.com')
if __name__ == '__main__':
   app.run(debug=True)

