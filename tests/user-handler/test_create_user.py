import pytest


CREATE_USER_PATH = '/v1/users/create-user'

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
        CREATE_USER_PATH,
        json=data
    )
    assert response.status == 200
    response_json = response.json()
    assert 'id' in response_json['data']


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
        CREATE_USER_PATH,
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
        CREATE_USER_PATH,
        json=data
    )
    assert response.status == 200
    response_json = response.json()
    assert 'id' in response_json['data']


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_create_user_missing_phone_number_success(service_client):
    data = {
        'username': 'user',
        'first_name': 'first',
        'last_name': 'last',
        'email': 'email@email.com',
    }
    response = await service_client.post(
        CREATE_USER_PATH,
        json=data
    )
    assert response.status == 200
    response_json = response.json()
    assert 'id' in response_json['data']


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_create_user_missing_first_name_failure(service_client):
    data = {
        'username': 'user',
        'last_name': 'last',
        'email': 'email@email.com',
        'phone_number': '+12345678901',
    }
    response = await service_client.post(
        CREATE_USER_PATH,
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
        CREATE_USER_PATH,
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
        CREATE_USER_PATH,
        json=data
    )
    assert response.status == 400


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
@pytest.mark.parametrize(
    'email',
    [
        'invalid email',
        'user@domain',
        'user@.com',
        '@domain.com',
        'user@domain..com',
        'user@domain,com',
        'user@domain com',
        'user@@domain.com',
        'userdomain.com',
        'user@domain.c',
        'user@-domain.com',
    ]
)
async def test_create_user_invalid_email(service_client, email):
    data = {
        'username': 'user',
        'first_name': 'first',
        'last_name': 'last',
        'email': email,
        'phone_number': '+12345678901',
    }
    response = await service_client.post(
        CREATE_USER_PATH,
        json=data
    )
    assert response.status == 400


@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
@pytest.mark.parametrize('phone_number', [
    '12345678901',
    '+1234567890',
    '+123456789012',
    '+12 3456789012a',
    '+1(234)5678',
    '+1-234-567-89',
    '+1-234-567-89012'
])
async def test_create_user_invalid_phone(service_client, phone_number):
    data = {
        'username': 'user',
        'first_name': 'first',
        'last_name': 'last',
        'email': 'email@email.com',
        'phone_number': phone_number,
    }
    response = await service_client.post(
        CREATE_USER_PATH,
        json=data
    )
    assert response.status == 400
