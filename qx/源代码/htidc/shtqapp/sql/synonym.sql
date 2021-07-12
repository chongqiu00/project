-- sqlplus shtqapp/pwdidc @/htidc/shtqapp/sql/synonym.sql

drop synonym T_USERLOG;
create synonym T_USERLOG for shappdata.T_USERLOG;

drop synonym SEQ_USERLOG;
create synonym SEQ_USERLOG for shappdata.SEQ_USERLOG;

exit;
