CREATE TABLE trade(
    id bigserial PRIMARY KEY,
    source_account_id bigint NOT NULL,
    dest_account_id bigint NOT NULL,
    money bigint NOT NULL,
    status int NOT NULL,
    modified_time timestamp NOT NULL,
    created_time timestamp NOT NULL
);
CREATE INDEX source_id_idx ON trade(source_account_id);
CREATE INDEX dest_id_idx ON trade(dest_account_id);