import pytest


GET_KEY_BY_ID_PATH = '/v1/keys/get-key-by-id'

# Start the tests via `make test-debug` or `make test-release`
# Tests return null ----------------------------------------------------


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_get_key_by_id_success(service_client):
    data = {
        'id': '1a2b3c4d-5e6f-7a8b-9c0d-1e2f3a4b5c6d'
    }
    response = await service_client.post(
        GET_KEY_BY_ID_PATH,
        json=data
    )
    assert response.status == 200
    response_data = response.json()["data"]
    assert response_data['id'] == '1a2b3c4d-5e6f-7a8b-9c0d-1e2f3a4b5c6d'
    assert 'user_id' in response_data
    assert 'name' in response_data
    assert 'key' in response_data
    assert 'created_at' in response_data


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_get_key_by_id_not_found(service_client):
    data = {
        'id': '1a2b3c4d-5e6f-7a8b-9c0d-1e2f3a4b5c6a'
    }
    response = await service_client.post(
        GET_KEY_BY_ID_PATH,
        json=data
    )
    assert response.status == 404


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_get_key_by_id_invalid_id(service_client):
    data = {
        'id': '1a2b3c4d'
    }
    response = await service_client.post(
        GET_KEY_BY_ID_PATH,
        json=data
    )
    assert response.status == 400
