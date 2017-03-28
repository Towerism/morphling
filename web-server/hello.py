from flask import Flask, render_template, request, session, url_for, redirect, flash
from firebase import firebase
import json
#from form import MyForm

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret key'
firebase = firebase.FirebaseApplication('https://flask-experiment-e7196.firebaseio.com', None)
new_user = 'abc abc'
tokens = ['abs','abss']

@app.route('/')
def index():
    #result = firebase.put('/users', new_user, {'print': 'pretty'}, {'X_FANCY_HEADER': 'VERY FANCY'})
    result = firebase.get('/rest', None)
    #return "<h1>Hello, world!</h1>"
    return '<h3>' + str(result) + '<h3>'

@app.route('/testing')
def testing():
    data = {'name': 'a', 'id': '0'}
    result = firebase.post('/users', data)
    #return "<h1>This is another testing page</h1>"
    return '<h3>' + str(result) + '<h3>'

@app.route('/bootstrap')
def bootstrap():
    #if not session.get('logged_in'):
    return render_template('index.html')

@app.route('/Settings.html')
def settings():
    return render_template('Settings.html')

@app.route('/Ranking.html')
def ranking():
    return render_template('Ranking.html')

@app.route('/Bracket.html')
def bracket():
    return render_template('Bracket.html')

@app.route('/login', methods=['GET','POST'])
def login():
    error = None
    if request.method == 'POST':
        if request.form['token'] in tokens:
            token = request.form['token']
            session['logged_in'] = True
            session[token] = True
            return redirect(url_for('show_game',token=token))
        else:
            error = 'Invalid Token'
            flash('Invalid Token')
    return render_template('Login.html',error=error)

@app.route('/game')
@app.route('/Game.html')
@app.route('/game/<token>')
def show_game(token=None):
    if token and session['logged_in']:
        #TODO get initial board data and player's data from firebase
        boardJSON = '{ "states": { "-hash": { "0": { "0": "_,_,_", "1": "_,x,_", "2": "_,_,_" } } } }'
        playersJSON = '{ "players": { "-hash1": { "name": "Demo Name", "score": 0 }, "-hash2": { "name": "Demo Name 2", "score": 0 } } }'

        boardData = json.loads(boardJSON)
        gameState = []
        rows = str(boardData["states"]["-hash"]["0"]).split(',')
        for r in rows:
            processRow(r,gameState)

        playersData = json.loads(playersJSON)
        player1 = playersData["players"]["-hash1"]
        player2 = playersData["players"]["-hash2"]
        return render_template('Game.html',token=token,session=session,gameState=gameState, player1=player1, player2=player2)
    else:
        return redirect(url_for('login'))

def processRow(row,gameState):
    for i in row:
        if i == '_':
            gameState.append("")
        elif i == 'x':
            gameState.append("X")
        elif i == 'o':
            gameState.append("O")

#count = 0
#
#@app.route('/api/put', methods=['GET', 'POST'])
#def tryput():
#    form = MyForm()
#    if form.validate_on_submit():
#        global count
#        count += 1
#        putdata = {'title':form.title.data, 'year':form.year.data, 'rating':form.rating.data}
#        firebase.put('/films', 'film' + str(count), putdata)
#        result = firebase.get('/films','film' + str(count))
#        #return '<h3>' + str(result) + str(count) + '<h3>'
#        return render_template('api-put-result.html', form=form, putData=putData)
#    return render_template('My-Form.html', form=form)

if __name__ == '__main__':
    app.run(debug=True)
