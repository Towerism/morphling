Experiment with Flask and Firebase

- Install Flask 
```
sudo pip install virtualenv
virtualenv venv
. venv/bin/activate
sudo pip install Flask
```

- Install Firebase
```
sudo pip install requests
sudo pip install python-firebase
```

- To run the app
```
export FLASK_APP=hello.py
flask run
```

- Firebase PUT test: open the browser, enter `http://127.0.0.1:5000/testing`. The webpage will show the reponse of PUT request.

- Firebase GET test: open the browser, enter `http://127.0.0.1:5000/`.  The webpage will show the reponse of GET request of a nested JSON file. 

- TODOs 
    - Integrate Flask wtf form to custom PUT requests and GET requests.
    - Write tests for different requests.
