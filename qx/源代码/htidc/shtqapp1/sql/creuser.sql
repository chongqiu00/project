
-- �Ϻ�����APP��������û��������Ҫ�����ݱ��ҵ��������������Ƚ�С
-- drop user shtqapp cascade;
create user shtqapp profile default identified by pwdidc default tablespace users account unlock;
grant connect to shtqapp;
grant dba to shtqapp;

-- �Ϻ�����APP��������û�������������Ƚϴ�ı�
-- drop user shappdata cascade;
create user shappdata profile default identified by pwdidc default tablespace users account unlock;
grant connect to shappdata;
grant dba to shappdata;




exit;
