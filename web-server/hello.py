from flask import Flask, render_template
from firebase import firebase
#from form import MyForm

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret key'
firebase = firebase.FirebaseApplication('https://flask-experiment-e7196.firebaseio.com', None)
new_user = 'abc abc'

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


