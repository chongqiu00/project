#include "_public.h"
#include "_ooci.h"


int main()
{
  connection conn;

  conn.connecttodb("scott/tiger","Simplified Chinese_China.ZHS16GBK");

  sqlstatement stmt(&conn);

  if (stmt.execute("insert into TEST123(XXNAME,XXCLASS,XXNUMBER,XXSCORE) VALUES('����'��'���꼶'��'3'��'60')")==0)
  {
    printf("ִ��SQL�ɹ�\n");
    conn.commit();
  }
  else
  {
    printf("ִ��SQLʧ��%s\n",stmt.m_cda.message);
    conn.rollback();
  }
}


