import pytest

from testsuite.databases import pgsql


# Start the tests via `make test-debug` or `make test-release`
# Tests return null ----------------------------------------------------
@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_user_common_null(service_client):
    params = {
        'user_id': '2',
        'item_id': 'common_25'
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[]'

@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_user_receipt_null(service_client):
    params = {
        'user_id': '1',
        'item_id': 'receipt_62'
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[]'

@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_doctor_null(service_client):
    params = {
        'user_id': '61',
        'item_id': ['common_12', 'receipt_25', 'special_16']
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[]'
#------------------------------------------------------------------------------------------

# Tests return WRONG_CATEGORY -------------------------------------------------------------
@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_user_wrong_category(service_client):
    params = {
        'user_id': '2',
        'item_id': 'sdfds_25'
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[{"item_id":"sdfds_25","problem":"WRONG_CATEGORY"}]'

@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_doctor_wrong_category(service_client):
    params = {
        'user_id': '62',
        'item_id': 'sdfds_25'
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[{"item_id":"sdfds_25","problem":"WRONG_CATEGORY"}]'

@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_error_user_wrong_category(service_client):
    params = {
        'user_id': '150',
        'item_id': 'sdfds_25'
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[{"item_id":"sdfds_25","problem":"WRONG_CATEGORY"}]'
#------------------------------------------------------------------------------------------

# Tests return INCORRECT_ITEM_ID ----------------------------------------------------------
@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_error_user_incorrect_item_id(service_client):
    params = {
        'user_id': '150',
        'item_id': ['common_025', 'common_25fds', 'common_fds25']
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[{"item_id":"common_025","problem":"INCORRECT_ITEM_ID"},{"item_id":"common_25fds","problem":"INCORRECT_ITEM_ID"},{"item_id":"common_fds25","problem":"INCORRECT_ITEM_ID"}]'

@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_user_incorrect_item_id(service_client):
    params = {
        'user_id': '2',
        'item_id': ['common_025', 'common_25fds', 'common_fds25']
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[{"item_id":"common_025","problem":"INCORRECT_ITEM_ID"},{"item_id":"common_25fds","problem":"INCORRECT_ITEM_ID"},{"item_id":"common_fds25","problem":"INCORRECT_ITEM_ID"}]'

@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_doctor_incorrect_item_id(service_client):
    params = {
        'user_id': '62',
        'item_id': ['common_025', 'common_25fds', 'common_fds25']
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[{"item_id":"common_025","problem":"INCORRECT_ITEM_ID"},{"item_id":"common_25fds","problem":"INCORRECT_ITEM_ID"},{"item_id":"common_fds25","problem":"INCORRECT_ITEM_ID"}]'
#------------------------------------------------------------------------------------------

# Tests return ITEM_NOT_FOUND -------------------------------------------------------------
@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_user_item_not_found(service_client):
    params = {
        'user_id': '2',
        'item_id': ['common_999', 'receipt_999', 'special_999']
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[{"item_id":"common_999","problem":"ITEM_NOT_FOUND"},{"item_id":"receipt_999","problem":"ITEM_NOT_FOUND"},{"item_id":"special_999","problem":"ITEM_NOT_FOUND"}]'

@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_error_user_item_not_found(service_client):
    params = {
        'user_id': '150',
        'item_id': ['common_999', 'receipt_999', 'special_999']
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[{"item_id":"common_999","problem":"ITEM_NOT_FOUND"},{"item_id":"receipt_999","problem":"ITEM_NOT_FOUND"},{"item_id":"special_999","problem":"ITEM_NOT_FOUND"}]'

@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_doctor_item_not_found(service_client):
    params = {
        'user_id': '62',
        'item_id': ['common_999', 'receipt_999', 'special_999']
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[{"item_id":"common_999","problem":"ITEM_NOT_FOUND"},{"item_id":"receipt_999","problem":"ITEM_NOT_FOUND"},{"item_id":"special_999","problem":"ITEM_NOT_FOUND"}]'
#------------------------------------------------------------------------------------------

# Tests return NO_USER --------------------------------------------------------------------
@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_error_user_common(service_client):
    params = {
        'user_id': '150',
        'item_id': 'common_24'
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[{"item_id":"common_24","problem":"NO_USER"}]'
#------------------------------------------------------------------------------------------

# Tests return NO_USER_NO_RECEIPT ---------------------------------------------------------
@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_error_user_common(service_client):
    params = {
        'user_id': '150',
        'item_id': 'receipt_24'
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[{"item_id":"receipt_24","problem":"NO_USER_NO_RECEIPT"}]'
#------------------------------------------------------------------------------------------

# Tests return NO_USER_SPECIAL_ITEM -------------------------------------------------------
@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_error_user_common(service_client):
    params = {
        'user_id': '150',
        'item_id': 'special_12'
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[{"item_id":"special_12","problem":"NO_USER_SPECIAL_ITEM"}]'
#------------------------------------------------------------------------------------------

# Tests return NO_RECEIPT -----------------------------------------------------------------
@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_user_no_receipt(service_client):
    params = {
        'user_id': '1',
        'item_id': 'receipt_4'
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[{"item_id":"receipt_4","problem":"NO_RECEIPT"}]'
#------------------------------------------------------------------------------------------

# Tests return ITEM_IS_SPECIAL ------------------------------------------------------------
@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_user_special_item(service_client):
    params = {
        'user_id': '1',
        'item_id': 'special_12'
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[{"item_id":"special_12","problem":"ITEM_IS_SPECIAL"}]'
#------------------------------------------------------------------------------------------

# Tests return ITEM_SPECIAL_WRONG_SPECIFIC ------------------------------------------------
@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_doctor_special_item_wrong_specific(service_client):
    params = {
        'user_id': '61',
        'item_id': 'special_13'
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[{"item_id":"special_13","problem":"ITEM_SPECIAL_WRONG_SPECIFIC"}]'
#------------------------------------------------------------------------------------------
@pytest.mark.pgsql('db-1', files=['initial_data.sql'])
async def test_user_special(service_client):
    params = {
        'user_id': '2',
        'item_id': ['special_12', 'speciaL_25']
    }
    response = await service_client.get(
        '/check',
        params=params
    )
    assert response.status == 200
    assert response.text == '[{"item_id":"special_12","problem":"ITEM_IS_SPECIAL"},{"item_id":"special_25","problem":"ITEM_IS_SPECIAL"}]'
