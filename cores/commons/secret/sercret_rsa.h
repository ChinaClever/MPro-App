#ifndef SERCRET_RSA_H
#define SERCRET_RSA_H
#include "RSA/qrsaencryption.h"
#include "sercret_aes.h"

class Sercret_Rsa : public Sercret_Aes
{
public:
    Sercret_Rsa();
    static Sercret_Rsa *bulid();
    bool generatePairKey(QByteArray &pubKey, QByteArray &privKey, const QByteArray& genesis,
                         QRSAEncryption::Rsa rsa=QRSAEncryption::Rsa::RSA_2048);

    QByteArray rsa_sign(QByteArray rawData, const QByteArray &privKey,
                           QRSAEncryption::Rsa rsa=QRSAEncryption::Rsa::RSA_2048);

    bool rsa_checkSign(const QByteArray &rawData, const QByteArray &pubKey,
                          QRSAEncryption::Rsa rsa=QRSAEncryption::Rsa::RSA_2048);

    QByteArray rsa_encode(const QByteArray &rawData, const QByteArray &pubKey,
                      QRSAEncryption::Rsa rsa=QRSAEncryption::Rsa::RSA_2048);

    QByteArray rsa_decode(const QByteArray &rawData, const QByteArray &privKey,
                      QRSAEncryption::Rsa rsa=QRSAEncryption::Rsa::RSA_2048);
};

#endif // SERCRET_RSA_H
