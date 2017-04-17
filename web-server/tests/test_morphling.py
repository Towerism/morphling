import pytest
import morphling

@pytest.fixture
def client(request):
    client = morphling.app.test_client()
    return client

def test_config():
    app = morphling.app
    assert app.config['SECRET_KEY'] == 'secret key'

def test_home(client):
    rv = client.get('/')
    assert rv.status_code == 200
