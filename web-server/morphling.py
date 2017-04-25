from flask import Flask, render_template, request, session, url_for, redirect
from firebase import firebase
import json
import operator

app = Flask(__name__)
app.config.from_pyfile('config.cfg')
firebase = firebase.FirebaseApplication(app.config['FIREBASE_URL'], None)
boardHeight = xrange(int(firebase.get('/settings/board/height', None))) if firebase.get('/settings/board/height', None) else xrange(app.config['BOARD_HEIGHT'])
boardWidth = xrange(int(firebase.get('/settings/board/width', None))) if firebase.get('/settings/board/width', None) else xrange(app.config['BOARD_WIDTH'])

@app.route('/bootstrap')
@app.route('/')
def bootstrap():
    player1 = []
    player2 = []
    keys = []

    if (firebase.get('/games', None) is None):
        return render_template('index.html',gameInfo=zip(player1,player2,keys))

    gameKeys = firebase.get('/games', None).keys()

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
    delay = firebase.get('/settings/delay', None)
    timeout = firebase.get('/settings/timeout', None)

    return render_template('Settings.html',delay=delay,timeout=timeout)

@app.route('/Ranking.html')
def ranking():
    players = firebase.get('/players', None)
    playersSorted = []
    if (players is None):
        return render_template('Ranking.html',players=playersSorted)
    players = sorted(players.items(), key=lambda i:operator.itemgetter(1)(i)['score'], reverse=True)
    for item in players:
        player = {'name':operator.itemgetter(1)(item)['name'],'score':operator.itemgetter(1)(item)['score']}
        playersSorted.append(player)
    return render_template('Ranking.html',players=playersSorted)

@app.route('/Bracket.html')
def bracket():
    return render_template('Bracket.html')

@app.route('/game/<token>')
def show_game(token=None):
    if token is None or firebase.get('/settings/tokens', None) is None:
        return redirect(url_for('bootstrap'))
    players = firebase.get('/games/'+token, None)
    player1, player2 = {}, {}
    if players is not None:
        player1 = firebase.get('/players/'+players['player1'], None)
        player2 = firebase.get('/players/'+players['player2'], None)
        player1['id'] = players['player1']
        player2['id'] = players['player2']

    pieces = firebase.get('/settings/tokens', None).keys()
    pieceUrls = {}
    backgroundUrl = firebase.get('/settings/board/image', None)

    for p in pieces:
        url = str(firebase.get('/settings/tokens/'+p+'/image', None))
        pieceUrls[p] = url

    if (firebase.get('/states', None) is None):
        return render_template('Game.html', token=token, session=session, player1=player1, player2=player2, boardWidth=boardWidth, boardHeight=boardHeight, pieceUrls=pieceUrls, backgroundUrl=backgroundUrl)

    tokens = json.dumps(firebase.get('/states', None).keys())
    if (token in tokens):
        session[token] = True

    return render_template('Game.html', token=token, session=session, player1=player1, player2=player2, boardWidth=boardWidth, boardHeight=boardHeight, pieceUrls=pieceUrls, backgroundUrl=backgroundUrl)

if __name__ == '__main__':
    app.run(debug=True)
