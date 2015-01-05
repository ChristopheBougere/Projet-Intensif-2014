--Script de création de la base de données du serveur

drop table if exists users cascade;
create table users (
       id    	   serial primary key,
       name  	   varchar,
       first_name  varchar,
       doctor_id   integer,
       supervisor_id integer
);

drop table if exists doctors;
create table doctors(
       id serial primary key,
       name varchar,
       first_name varchar,
       password varchar
);

drop table if exists alerts cascade ;
create table alerts(
       id serial primary key,
       user_id integer,
       alert_type_id integer,
       alert_date date,
       alert_time time
);

drop table if exists alert_types cascade;
create table alert_types (
       id integer primary key,
       label varchar
);

drop table if exists posologies cascade;
create table posologies (
       id serial primary key,
       user_id integer,
       drug_id integer,
       quantity integer,
       start_date date,
       end_date date
);

drop table if exists hours cascade;
create table hours (
       posology_id integer,
       posology_time time
);

drop table if exists drugs cascade;
create table drugs (
       id serial primary key,
       name varchar
);

drop table if exists supervisors cascade;
create table supervisors (
       id serial primary key
);


-- Clé étrangères
alter table users add constraint fk_doctor_id foreign key (doctor_id) references doctors(id);
alter table users add constraint fk_supervisor_id foreign key (supervisor_id) references supervisors(id);

alter table alerts add constraint fk_user_id foreign key (user_id) references users(id);
alter table alerts add constraint fk_alert_type_id foreign key (alert_type_id) references alert_types(id);

alter table posologies add constraint fk_user_id foreign key (user_id) references users(id);
alter table posologies add constraint fk_drug_id foreign key (drug_id) references drugs(id);

alter table hours add constraint fk_posology_id foreign key (posology_id) references posologies(id);



-- Insertions de champs constants
Insert into alert_types values 
       (1,'Drugs not taken')
;

Insert into drugs(name) values
       ('Acetate de cyproterone'),
       ('Afinitor'),
       ('Alimta'),
       ('Clairodermyl'),
       ('Enantone'),
       ('Hydroxyzine'),
       ('Noctyl'),
       ('Theralene'),
       ('Paracetamol'),
       ('Smecta'),
       ('Pregabaline'),
       ('Zophren'),
       ('Felbamate'),
       ('Atociban'),
       ('Modafinil'),
       ('Sophidone')
;

-- On donne la database a dbuser

ALTER DATABASE server_database OWNER TO dbuser;
alter table users owner to dbuser;
alter table users owner to dbuser;
alter table alerts owner to dbuser;
alter table alert_types owner to dbuser;
alter table posologies owner to dbuser;
alter table hours owner to dbuser;
alter table drugs owner to dbuser;
alter table supervisors owner to dbuser;