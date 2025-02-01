import pytest


GET_KEY_BY_NAME_PATH = '/v1/keys/get-key-by-name'

# Start the tests via `make test-debug` or `make test-release`
# Tests return null ----------------------------------------------------


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_get_key_by_name_success(service_client):
    data = {
        'name': 'Home WiFi'
    }
    response = await service_client.post(
        GET_KEY_BY_NAME_PATH,
        json=data
    )
    assert response.status == 200
    response_data = response.json()["data"]
    assert response_data['name'] == 'Home WiFi'
    assert 'id' in response_data
    assert 'user_id' in response_data
    assert 'key' in response_data
    assert 'created_at' in response_data


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_get_key_by_name_not_found(service_client):
    data = {
        'name': 'name'
    }
    response = await service_client.post(
        GET_KEY_BY_NAME_PATH,
        json=data
    )
    assert response.status == 404
