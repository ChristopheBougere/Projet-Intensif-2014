-- Script d'insertion de valeurs pour test en base de données

insert into doctors(id,name, first_name, password) values(1, 'House', 'Gregory', md5('vicodine'));

insert into users(id,name,first_name,doctor_id) values (1,'Papin','Jean',1);

insert into posologies(id, user_id, drug_id, start_date, end_date) values (1,1,1,date '2015-01-01',date '2015-01-31');

insert into hours values (1,2,time '08:30'),(1,2, time '12:30'),(1,3,'16:30'),(1,1,time '20:30');
