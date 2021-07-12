#include "_public.h"
#include "_ooci.h"


int main()
{
  connection conn;

  conn.connecttodb("scott/tiger","Simplified Chinese_China.ZHS16GBK");

  sqlstatement stmt(&conn);

  if (stmt.execute("insert into TEST123(XXNAME,XXCLASS,XXNUMBER,XXSCORE) VALUES('张三'，'三年级'，'3'，'60')")==0)
  {
    printf("执行SQL成功\n");
    conn.commit();
  }
  else
  {
    printf("执行SQL失败%s\n",stmt.m_cda.message);
    conn.rollback();
  }
}


