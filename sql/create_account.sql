CREATE TABLE account(
    id bigserial PRIMARY KEY,
    customer_id bigint NOT NULL,
    money bigint NOT NULL,
    type int NOT NULL,
    status int NOT NULL,
    modified_time timestamp NOT NULL,
    created_time timestamp NOT NULL
);
CREATE INDEX customer_id_idx ON account(customer_id);