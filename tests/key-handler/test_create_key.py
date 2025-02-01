import pytest


CREATE_KEY_PATH = '/v1/keys/create-key'

# Start the tests via `make test-debug` or `make test-release`
# Tests return null ----------------------------------------------------


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_create_key_success(service_client):
    data = {
        'user_id': '3f47acb2-9e5d-4c3a-8f1b-2d6e5c7a9b0e',
        'name': 'name',
        'key': 'key',
    }
    response = await service_client.post(
        CREATE_KEY_PATH,
        json=data
    )
    assert response.status == 200
    response_json = response.json()
    assert 'id' in response_json["data"]


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_create_key_duplicate_name(service_client):
    data = {
        'user_id': '3f47acb2-9e5d-4c3a-8f1b-2d6e5c7a9b0e',
        'name': 'Home WiFi',
        'key': 'key',
    }
    response = await service_client.post(
        CREATE_KEY_PATH,
        json=data
    )
    assert response.status == 409


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_create_key_missing_user_id_failure(service_client):
    data = {
        'name': 'name',
        'key': 'key',
    }
    response = await service_client.post(
        CREATE_KEY_PATH,
        json=data
    )
    assert response.status == 400


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_create_key_missing_name_failure(service_client):
    data = {
        'user_id': '3f47acb2-9e5d-4c3a-8f1b-2d6e5c7a9b0e',
        'key': 'key',
    }
    response = await service_client.post(
        CREATE_KEY_PATH,
        json=data
    )
    assert response.status == 400


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_create_key_missing_key_failure(service_client):
    data = {
        'user_id': '3f47acb2-9e5d-4c3a-8f1b-2d6e5c7a9b0e',
        'name': 'Home WiFi',
    }
    response = await service_client.post(
        CREATE_KEY_PATH,
        json=data
    )
    assert response.status == 400


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_create_key_invalid_user_id(service_client):
    data = {
        'user_id': 'user id',
        'name': 'name',
        'key': 'key',
    }
    response = await service_client.post(
        CREATE_KEY_PATH,
        json=data
    )
    assert response.status == 400


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_create_key_user_not_found(service_client):
    data = {
        'user_id': '3f47acb2-9e5d-4c3a-8f1b-2d6e5c7a9b0f',
        'name': 'name',
        'key': 'key',
    }
    response = await service_client.post(
        CREATE_KEY_PATH,
        json=data
    )
    assert response.status == 404
