import pytest

from testsuite.databases import pgsql


GET_USER_BY_USERNAME_PATH = '/v1/users/get-user-by-username'

# Start the tests via `make test-debug` or `make test-release`
# Tests return null ----------------------------------------------------


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_get_user_by_username_success(service_client):
    data = {
        'username': 'jdoe'
    }
    response = await service_client.post(
        GET_USER_BY_USERNAME_PATH,
        json=data
    )
    assert response.status == 200
    response_json = response.json()
    assert response_json['username'] == 'jdoe'
    assert 'id' in response_json
    assert 'first_name' in response_json
    assert 'last_name' in response_json
    assert 'email' in response_json
    assert 'phone_number' in response_json


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_get_user_by_username_not_found(service_client):
    data = {
        'username': 'notfound'
    }
    response = await service_client.post(
        GET_USER_BY_USERNAME_PATH,
        json=data
    )
    assert response.status == 404
