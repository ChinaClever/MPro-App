//
// Created by LiuCheng on 2021/1/29.
//

#ifndef LIB_SM4_H
#define LIB_SM4_H

#include <QtCore>

// C++封装的国密SM4加解密, 支持ECB和CBC模式, PKCS7Padding补全
class Sm4 {
public:
    enum Type{
        ECB,
        CBC
    };
public:
    explicit Sm4();
    ~Sm4();
    void setKey(const QByteArray &k);
    void setIv(const QByteArray &i);
    void setType(Type t = Type::ECB);

    QByteArray encrypt(const QByteArray &data);
    QByteArray decrypt(const QByteArray &data);

private:
    QByteArray key;
    QByteArray iv;
    Type type;
};

#endif //LIB_SM4_H
