
-- 上海天气APP软件的主用户，存放重要的数据表和业务参数表，数据量比较小
-- drop user shtqapp cascade;
create user shtqapp profile default identified by pwdidc default tablespace users account unlock;
grant connect to shtqapp;
grant dba to shtqapp;

-- 上海天气APP软件的主用户，存放数据量比较大的表。
-- drop user shappdata cascade;
create user shappdata profile default identified by pwdidc default tablespace users account unlock;
grant connect to shappdata;
grant dba to shappdata;




exit;
