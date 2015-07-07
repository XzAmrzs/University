from flask import Flask
from flask.ext.script import Manager
from flask import render_template

app=Flask(__name__)

manager = Manager(app)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/user/<name>')
def user(name):
    return render_template('user.html',name = name)

from flask import redirect
@app.route('/user')
def redir():
    return redirect('http://www.baidu.com')

if __name__ == '__main__':
   manager.run()

