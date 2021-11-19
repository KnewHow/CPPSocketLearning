CREATE TABLE customer (
    id bigserial PRIMARY KEY,
    username varchar(255) NOT NULL,
    password varchar(255) NOT NULL,
    sex      smallint    NOT NULL,
    age      bigint      NOT NULL,
    modified_time timestamp NOT NULL,
    created_time timestamp NOT NULL
);
CREATE INDEX username_idx ON customer(username);
CREATE INDEX username_and_password_idx ON customer(username, password);