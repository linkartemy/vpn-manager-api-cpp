import pytest

from testsuite.databases import pgsql


# Start the tests via `make test-debug` or `make test-release`
# Tests return null ----------------------------------------------------
@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_get_user_by_username_success(service_client):
    data = {
        'username': 'jdoe'
    }
    response = await service_client.post(
        '/v1/users/get-user-by-username',
        json=data
    )
    assert response.status == 200
    response_json = response.json()
    assert response_json['username'] == 'jdoe'
