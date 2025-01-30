INSERT INTO vpn_manager.user (username, first_name, last_name, email, phone_number)
VALUES
    ('jdoe', 'John', 'Doe', 'johndoe@example.com', '123-456-7890'),
    ('asmith', 'Alice', 'Smith', 'alice.smith@example.com', '987-654-3210'),
    ('bwayne', 'Bruce', 'Wayne', 'bruce.wayne@example.com', '456-789-1234'),
    ('cclark', 'Clark', 'Kent', 'clark.kent@example.com', '789-123-4567'),
    ('ddiana', 'Diana', 'Prince', 'diana.prince@example.com', '321-654-9870');

INSERT INTO vpn_manager.key (user_id, name, key)
VALUES
    ((SELECT id FROM vpn_manager.user WHERE username = 'jdoe'), 'Home WiFi', 'abc123key'),
    ((SELECT id FROM vpn_manager.user WHERE username = 'asmith'), 'Work VPN', 'xyz456key'),
    ((SELECT id FROM vpn_manager.user WHERE username = 'bwayne'), 'Batcave VPN', 'bat789key'),
    ((SELECT id FROM vpn_manager.user WHERE username = 'cclark'), 'Fortress VPN', 'krypton101key'),
    ((SELECT id FROM vpn_manager.user WHERE username = 'ddiana'), 'Amazon VPN', 'wonder102key');
