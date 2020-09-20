CREATE DATABASE  IF NOT EXISTS blog_system charset=utf8 collate utf8_general_ci;
use blog_system;

drop table if exists table_tag;
create table table_tag(
    tag_id int primary key auto_increment,
    tag_name varchar(30) unique
);

drop table if exists table_user;
create table table_user(
    user_id int primary key auto_increment,
    user_name varchar(30) unique
);

drop table if exists table_blog;
create table table_blog(
    blog_id int primary key auto_increment,
    user_id int,
    tag_id int,
    title varchar(255),
    content text,
    ctime datetime,
    foreign key (user_id) references table_user(user_id),
    foreign key (tag_id) references table_tag(tag_id)
);

