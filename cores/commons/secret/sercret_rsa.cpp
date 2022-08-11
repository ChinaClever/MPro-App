/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "sercret_rsa.h"
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>

sRsaIt Sercret_Rsa::rsaCfg;
Sercret_Rsa::Sercret_Rsa()
{
    Sercret_TlsCert *cert = Sercret_TlsCert::bulid();
    rsaCfg.privKey = cert->privKey().toPem();
    rsaCfg.pubKey = cert->publicKey().toPem();
    rsaCfg.length = cert->publicKey().length();
}

void Sercret_Rsa::rsa_test()
{
    QByteArray str = "123456";
    QByteArray res = rsa_encode(str);
    QByteArray rtn = rsa_decode(res);
    qDebug() << (str == rtn ? true : false) << rtn << res.size();
    qDebug() << res.toBase64();
}

QByteArray Sercret_Rsa::rsa_encode(const QByteArray &rawData)
{
    QByteArray res; if(rsaCfg.pubKey.size()) {
        res = rsaPubEncrypt(rawData, rsaCfg.pubKey);
    }
    return res;
}

QByteArray Sercret_Rsa::rsa_decode(const QByteArray &rawData)
{
    QByteArray res; if(rsaCfg.privKey.size()) {
        res = rsaPriDecrypt(rawData, rsaCfg.privKey);
    }
    return res;
}

/*
@brief : 公钥加密
@para  : clear_text  -[i] 需要进行加密的明文
         pri_key     -[i] 私钥
@return: 加密后的数据
**/
QByteArray Sercret_Rsa::rsaPubEncrypt(const QByteArray &clear_text, const QByteArray &pub_key)
{
    QByteArray encrypt_text;
    BIO *keybio = BIO_new_mem_buf((unsigned char *)pub_key.constData(), -1);
    RSA* rsa = RSA_new();
    // 注意-----第1种格式的公钥
    //rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);
    // 注意-----第2种格式的公钥（这里以第二种格式为例）
    rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);

    // 获取RSA单次可以处理的数据块的最大长度
    int key_len = RSA_size(rsa);
    int block_len = key_len - 11;    // 因为填充方式为RSA_PKCS1_PADDING, 所以要在key_len基础上减去11

    // 申请内存：存贮加密后的密文数据
    char *sub_text = new char[key_len + 1];
    memset(sub_text, 0, key_len + 1);
    int ret = 0;
    int pos = 0;
    QByteArray sub_str;
    // 对数据进行分段加密（返回值是加密后数据的长度）
    while (pos < clear_text.size()) {
        sub_str = clear_text.mid(pos, block_len);
        memset(sub_text, 0, key_len + 1);
        ret = RSA_public_encrypt(sub_str.length(), (const unsigned char*)sub_str.constData(),
                                 (unsigned char*)sub_text, rsa, RSA_PKCS1_PADDING);
        if (ret >= 0) {
            encrypt_text.append(sub_text, ret);
        }
        pos += block_len;
    }

    // 释放内存
    BIO_free_all(keybio);
    RSA_free(rsa);
    delete[] sub_text;

    return encrypt_text;
}

/*
@brief : 私钥解密
@para  : cipher_text -[i] 加密的密文
         pub_key     -[i] 公钥
@return: 解密后的数据
**/
QByteArray Sercret_Rsa::rsaPriDecrypt(const QByteArray &cipher_text, const QByteArray&pri_key)
{
    QByteArray decrypt_text;
    RSA *rsa = RSA_new();
    BIO *keybio;
    keybio = BIO_new_mem_buf((unsigned char *)pri_key.constData(), -1);
    rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
    if (rsa == nullptr) {
        unsigned long err = ERR_get_error(); //获取错误号
        char err_msg[1024] = { 0 };
        ERR_error_string(err, err_msg); // 格式：error:errId:库:函数:原因
        printf("err msg: err:%ld, msg:%s\n", err, err_msg);
        return decrypt_text;
    }

    // 获取RSA单次处理的最大长度
    int key_len = RSA_size(rsa);
    char *sub_text = new char[key_len + 1];
    memset(sub_text, 0, key_len + 1);
    int ret = 0;
    QByteArray sub_str;
    int pos = 0;
    // 对密文进行分段解密
    while (pos < cipher_text.size()) {
        sub_str = cipher_text.mid(pos, key_len);
        memset(sub_text, 0, key_len + 1);
        ret = RSA_private_decrypt(sub_str.length(), (const unsigned char*)sub_str.constData(),
                                  (unsigned char*)sub_text, rsa, RSA_PKCS1_PADDING);
        if (ret >= 0) {
            decrypt_text.append(sub_text, ret);
            //printf("pos:%d, sub: %s\n", pos, sub_text);
            pos += key_len;
        }
    }

    // 释放内存
    delete[] sub_text;
    BIO_free_all(keybio);
    RSA_free(rsa);
    return decrypt_text;
}

/**


-----BEGIN PUBLIC KEY-----
MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAw02XT3+YuP4KJqxf+Fvr
JtNC2bCrM8Ksi3+dFcwIfNUeyfxIWy1r6E8sJq8eYK5h5BqHGUaLdbTwGwfw9JV+
MGNZeCDSh8TGRAUi9GT6m3GGIpWJ09PBnLItH2F18BXQxV/ZqIyXbJatz3eoFtVs
cHRVYfgCWHgVl9nYN5pD4UT5ZdfylS6DfvqvhF23HHw8tNSGUZ0AHIsuF1+S8QDT
t0OPkY0jZyFGNUEeTR7TPzDTRPj2S3NzsuZ5Q3ZIJD00E5b9kse4dS+o4uw9v0fz
BJ5TUafQjY6oMBE7ITko8Zef55vmezU8/TLzR1qmk4zbs+xdqJDvx+gd1Z8FuOAx
lnJS3mPmSBUH27czRSNJaHRwWZHUrvb3cUzEqTTOuCueCEKNgl/io9UVmWdYYdYi
LtyG/CdzF/8HoEpMUN5RfPPGSZOglss8d9TnsjxcM8HM3Q+cV84aP+bgtcoM4g+C
i916rcyIdZVBz8HIxPlhaVnTa6be1HT8e/DzupbbRJhM/Yv+9bSOQ5pVKcrrfQpt
1A9ghTqJ1uYY/GCtBgd/JpSvHJSjArcFYsdeC2Q/fY+Pss66Gc3pILqjpLoKpJSc
wxeiIr1f0on8pZBvTymt4zLoD2jXWbuuzWMJ/ISF2RQ0He9wQsQl52umrdQ1rD+w
r2nGRigCbHY6ZK128btSpJ0CAwEAAQ==
-----END PUBLIC KEY-----





-----BEGIN RSA PRIVATE KEY-----
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
-----END RSA PRIVATE KEY-----

*/
