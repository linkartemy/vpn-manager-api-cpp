import pytest

from testsuite.databases import pgsql


# Start the tests via `make test-debug` or `make test-release`
# Tests return null ----------------------------------------------------
@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_create_user_success(service_client):
    data = {
        'username': 'user',
        'first_name': 'first',
        'last_name': 'last',
        'email': 'email@email.com',
        'phone_number': '+12345678901',
    }
    response = await service_client.post(
        '/v1/users/create-user',
        json=data
    )
    assert response.status == 200
    response_json = response.json()
    assert 'id' in response_json


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_create_user_duplicate_username(service_client):
    data = {
        'username': 'jdoe',
        'first_name': 'first',
        'last_name': 'last',
        'email': 'email@email.com',
        'phone_number': '+12345678901',
    }
    response = await service_client.post(
        '/v1/users/create-user',
        json=data
    )
    assert response.status == 409


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_create_user_missing_email_success(service_client):
    data = {
        'username': 'user',
        'first_name': 'first',
        'last_name': 'last',
        'phone_number': '+12345678901',
    }
    response = await service_client.post(
        '/v1/users/create-user',
        json=data
    )
    assert response.status == 200
    response_json = response.json()
    assert 'id' in response_json


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_create_user_missing_phone_number_success(service_client):
    data = {
        'username': 'user',
        'first_name': 'first',
        'last_name': 'last',
        'email': 'email@email.com',
    }
    response = await service_client.post(
        '/v1/users/create-user',
        json=data
    )
    assert response.status == 200
    response_json = response.json()
    assert 'id' in response_json


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_create_user_missing_first_name_failure(service_client):
    data = {
        'username': 'user',
        'last_name': 'last',
        'email': 'email@email.com',
        'phone_number': '+12345678901',
    }
    response = await service_client.post(
        '/v1/users/create-user',
        json=data
    )
    assert response.status == 400


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_create_user_missing_last_name_failure(service_client):
    data = {
        'username': 'user',
        'first_name': 'first',
        'email': 'email@email.com',
        'phone_number': '+12345678901',
    }
    response = await service_client.post(
        '/v1/users/create-user',
        json=data
    )
    assert response.status == 400


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_create_user_missing_username_failure(service_client):
    data = {
        'first_name': 'first',
        'last_name': 'last',
        'email': 'email@email.com',
        'phone_number': '+12345678901',
    }
    response = await service_client.post(
        '/v1/users/create-user',
        json=data
    )
    assert response.status == 400
