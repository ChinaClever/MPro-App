#include "jsonrpcobj.h"

bool JsonRpcObj::getString(mg_str r, int id, char *s)
{
    char buffer[10] = {0};
    //static char buf[256] = {0};
    sprintf(buffer , "$[%d]" , id);
    s = mg_json_get_str(r, buffer);
    //if(n > 0) qstrcpy(s, buf); else s[0] = '\0';
    //return n == -1 ?false:true;
    return s == nullptr ?false:true;
}

double JsonRpcObj::getNumber(mg_str r, int id)
{
    double res=0; char buffer[10] = {0};  sprintf(buffer , "$[%d]" , id);
    bool ret = mg_json_get_num(r, buffer, &res);
    if(!ret) qDebug() << "Error: JsonRpcObj Get Number" << ret;
    return res;
}

QVector<double> JsonRpcObj::getNumbers(mg_str r, int num)
{
    QVector<double> res;
    for(int i=0; i<num; ++i) {
        double ret = getNumber(r, i);
        res.append(ret);
    }
    return res;
}

/*
 *
int mjson_get_number(const char *s, int len, const char *path, double *v) {
  const char *p;
  int tok, n;
  if ((tok = mjson_find(s, len, path, &p, &n)) == MJSON_TOK_NUMBER) {
    if (v != NULL) *v = mystrtod(p, NULL);
  }
  return tok == MJSON_TOK_NUMBER ? 1 : 0;
}
int mjson_get_string(const char *s, int len, const char *path, char *to,
                     int n) {
  const char *p;
  int sz;
  if (mjson_find(s, len, path, &p, &sz) != MJSON_TOK_STRING) return -1;
  return mjson_unescape(p + 1, sz - 2, to, n);
}*/
