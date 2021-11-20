CREATE TABLE customer (
    id bigserial PRIMARY KEY,
    phone_number char(11) NOT NULL UNIQUE,
    password varchar(255) NOT NULL,
    username varchar(255),
    sex      smallint    NOT NULL,
    age      bigint      NOT NULL,
    status   int         NOT NULL, -- 0 normal -1 deleted
    modified_time timestamp NOT NULL,
    created_time timestamp NOT NULL
);