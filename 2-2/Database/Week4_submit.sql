CREATE TABLE order_details (
	order_detail_id integer PRIMARY KEY,
	order_id integer NOT NULL,
	order_date date,
	quantity integer,
	notes character varying(200)
);

\d order_details
ALTER TABLE order_details ALTER COLUMN notes SET DEFAULT 'Standard shipping';

\d order_details
ALTER TABLE order_details ADD order_uuid uuid; 

\d order_details
ALTER TABLE order_details ADD CONSTRAINT quantitychk CHECK (quantity > 0);

\d order_details
ALTER TABLE order_details RENAME COLUMN notes TO memo;

\d order_details
ALTER TABLE order_details DROP order_id; 

\dt

DROP TABLE order_details;

\dt