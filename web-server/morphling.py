from flask import Flask, render_template, request, session, url_for, redirect, flash
from firebase import firebase
import json

app = Flask(__name__)
app.config.from_pyfile('config.cfg')
firebase = firebase.FirebaseApplication(app.config['FIREBASE_URL'], None)
boardWidth = xrange(int(firebase.get('/settings/board/height', None)))
boardHeight = xrange(int(firebase.get('/settings/board/width', None)))

@app.route('/testing')
def testing():
    data = {'name': 'a', 'id': '0'}
    result = firebase.post('/users', data)
    return '<h3>' + str(result) + '<h3>'

@app.route('/bootstrap')
@app.route('/')
def bootstrap():
    player1 = []
    player2 = []
    gameKeys = []

    if (firebase.get('/games', None) is None):
        return render_template('index.html',gameInfo=zip(player1,player2,keys))

    gameKeys = firebase.get('/games', None).keys()
    player1 = []
    player2 = []
    keys = []

    for k in gameKeys:
        score = firebase.get('/games/'+k+'/score', None)
        if (score is None):
            keys.append(k)
            players = firebase.get('/games/'+k, None)
            player1.append(str(firebase.get('/players/'+players['player1']+'/name', None)))
            player2.append(str(firebase.get('/players/'+players['player2']+'/name', None)))

    return render_template('index.html',gameInfo=zip(player1,player2,keys))

@app.route('/Settings.html')
def settings():
    return render_template('Settings.html')

@app.route('/Ranking.html')
def ranking():
    return render_template('Ranking.html')

@app.route('/Bracket.html')
def bracket():
    return render_template('Bracket.html')

@app.route('/game')
@app.route('/Game')
@app.route('/Game.html')
@app.route('/game/<token>')
def show_game(token=None):
    players = firebase.get('/games/'+token, None)
    player1 = firebase.get('/players/'+players['player1'], None)
    player2 = firebase.get('/players/'+players['player2'], None)
    player1['id'] = players['player1']
    player2['id'] = players['player2']

    if (firebase.get('/states', None) is None):
        return render_template('Game.html', token=token, session=session, player1=player1, player2=player2, boardWidth=boardWidth, boardHeight=boardHeight)

    tokens = json.dumps(firebase.get('/states', None).keys())
    pieces = firebase.get('/settings/tokens', None).keys()
    urls = []
    pieceUrls = {}
    backgroundUrl = firebase.get('/settings/board/image', None)

    for p in pieces:
        url = str(firebase.get('/settings/tokens/'+p+'/image', None))
        urls.append(url)
        pieceUrls[p] = url

    if (token in tokens):
        session[token] = True
    if token:
        if (boardWidth is None):
            global boardWidth
            boardWidth = xrange(3)
        if (boardHeight is None):
            global boardHeight
            boardHeight = xrange(3)

        return render_template('Game.html', token=token, session=session, player1=player1, player2=player2, boardWidth=boardWidth, boardHeight=boardHeight, pieceUrls=pieceUrls, backgroundUrl=backgroundUrl)
    else:
        return redirect(url_for('login'))

if __name__ == '__main__':
    app.run(debug=True)
