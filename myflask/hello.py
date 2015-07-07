from flask import Flask
from flask.ext.script import Manager

app=Flask(__name__)

manager = Manager(app)

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
   manager.run()

