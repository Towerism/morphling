Morphling web server

- Environment: Python 2.7, Chrome 58.0

- Requirements: the game server and the Firebase are all already setup.

- Run the web server:
```
make
```

- Terminate the server: `Ctrl+C` or enter
```
ps -a | grep 'morphling.py'| grep -v 'grep' | awk '{ print $1 }' | xargs kill
```
in another terminal (or tmux).

- In browser, enter http://127.0.0.1:5000/ and the Ongoing game will show up.

- Workflow of creating a game:
  - Go to http://127.0.0.1:5000/Settings.html and add players, whose names are separated by new line.
  - In the text area of "1 vs 1", put players' names and click "New Game".
  - Go to http://127.0.0.1:5000/ and the newly created game with given players is there.
  - Click "Watch".
  - Players will get the token from the game page to connect to the server.

- Credits:
http://startbootstrap.com/
http://stackoverflow.com/
