INSERT INTO vpn_manager.user (id, username, first_name, last_name, email, phone_number)
VALUES
    ('3f47acb2-9e5d-4c3a-8f1b-2d6e5c7a9b0e', 'jdoe', 'John', 'Doe', 'johndoe@example.com', '123-456-7890'),
    ('a1b2c3d4-e5f6-7a8b-9c0d-e1f2a3b4c5d6', 'asmith', 'Alice', 'Smith', 'alice.smith@example.com', '987-654-3210'),
    ('f7e6d5c4-b3a2-1f0e-9d8c-7b6a5f4e3d2c', 'bwayne', 'Bruce', 'Wayne', 'bruce.wayne@example.com', '456-789-1234'),
    ('01234567-89ab-cdef-0123-456789abcdef', 'cclark', 'Clark', 'Kent', 'clark.kent@example.com', '789-123-4567'),
    ('fedcba98-7654-3210-fedc-ba9876543210', 'ddiana', 'Diana', 'Prince', 'diana.prince@example.com', '321-654-9870');

INSERT INTO vpn_manager.key (id, user_id, name, key)
VALUES
    ('1a2b3c4d-5e6f-7a8b-9c0d-1e2f3a4b5c6d', (SELECT id FROM vpn_manager.user WHERE username = 'jdoe'), 'Home WiFi', 'abc123key'),
    ('2b3c4d5e-6f7a-8b9c-0d1e-2f3a4b5c6d7e', (SELECT id FROM vpn_manager.user WHERE username = 'asmith'), 'Work VPN', 'xyz456key'),
    ('3c4d5e6f-7a8b-9c0d-1e2f-3a4b5c6d7e8f', (SELECT id FROM vpn_manager.user WHERE username = 'bwayne'), 'Batcave VPN', 'bat789key'),
    ('4d5e6f7a-8b9c-0d1e-2f3a-4b5c6d7e8f9f', (SELECT id FROM vpn_manager.user WHERE username = 'cclark'), 'Fortress VPN', 'krypton101key'),
    ('5e6f7a8b-9c0d-1e2f-3a4b-5c6d7e8f9f0a', (SELECT id FROM vpn_manager.user WHERE username = 'ddiana'), 'Amazon VPN', 'wonder102key');
