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
    QByteArray str = "123456luozhiyong";
    QByteArray res = rsa_encode(str);
    QByteArray rtn = rsa_decode(res);
    qDebug() << (str == rtn ? true : false) << rtn;
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
