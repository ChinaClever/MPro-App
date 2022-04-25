/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "sercret_smx.h"
#include "SMX/stringutil.h"
#include "SMX/base64.h"
#include "SMX/sm2.h"
#include "SMX/sm3.h"
#include "SMX/sm4.h"
#include <random>

Sercret_Smx::Sercret_Smx()
{

}

Sercret_Smx *Sercret_Smx::bulid()
{
    static Sercret_Smx* sington = nullptr;
    if(sington == nullptr) {
        sington = new Sercret_Smx();
    }
    return sington;
}

QString Sercret_Smx::sm2_MakePubKey(const QByteArray &priKey)
{
    QString Q_PriKey = priKey;
    std::string S_PriKey = Q_PriKey.toStdString();

    // (byte)PriKey
    std::string PriKey_byte = HexStr2BYTE(S_PriKey);
    BYTE PriKey[32];
    for (uint i = 0; i < 32; ++i)
        PriKey[i] = PriKey_byte[i];

    // ����64�ֽڹ�Կ
    BYTE PubKey[64];
    EccMakeKey(PriKey, 32,PubKey, 64, 0);

    // ����16���ƹ�Կ�ַ���
    std::string PubKeyString(64, '0');
    for (uint i = 0; i < 64; ++i)
        PubKeyString[i] = PubKey[i];

    std::string PubKeyHexStr = BYTE2HexStr(PubKeyString);
    return QString::fromStdString(PubKeyHexStr);
}



// 签名
QString Sercret_Smx::sm2_Sign(const QByteArray &msg, const QByteArray &user,
                              const QByteArray &pubKey, const QByteArray &priKey)
{
    // 获取随机数组
    BYTE random[32];
    std::random_device rd;
    for (uint i = 0; i < 32; i++)
        random[i] = (BYTE)(rd() % 256);

    // 提取明文
    QString Q_Plain = msg;
    std::string S_Plain = utf8Togb18030(Q_Plain);

    // 计算摘要
    std::string hash1 = SM3::hash(S_Plain, S_Plain.size());

    QString Q_userid = user;
    std::string S_userid = Q_userid.toStdString();

    QString Q_pubkey = pubKey;
    std::string S0_pubkey = Q_pubkey.toStdString();
    std::string S_pubkey = HexStr2BYTE(S0_pubkey);

    std::string S_sm2_par_dig(128, '0');
    for (int i = 0; i < 128; i++) S_sm2_par_dig[i] = sm2_par_dig[i];

    int userid_bitlen = (S_userid.size() << 3);
    std::string s1 = "00";
    s1[0] = (BYTE) ((userid_bitlen >> 8) & 0xFF);
    s1[1] = (BYTE) (userid_bitlen & 0xFF);

    std::string s2 = s1 + S_userid + S_sm2_par_dig + S_pubkey;
    std::string hash2 = SM3::hash(s2, s2.size());

    // hash1
    std::string S_hash1 = HexStr2BYTE(hash1);

    // hash2
    std::string S_hash2 = HexStr2BYTE(hash2);

    // hash3
    std::string s3 = S_hash2 + S_hash1;
    std::string hash3 = SM3::hash(s3, s3.size());

    std::string hash3_BYTE = HexStr2BYTE(hash3);

    BYTE hash[32];

    for (uint i = 0; i < 32; i++) {
        hash[i] = hash3_BYTE[i];
    }

    // 私钥
    QString Q_PriKey = priKey;
    std::string S_PriKey = Q_PriKey.toStdString();
    std::string S_Prikey_BYTE = HexStr2BYTE(S_PriKey);
    BYTE PriKey[32];
    for (uint i = 0; i < 32; i++)
        PriKey[i] = S_Prikey_BYTE[i];

    // 计算签名
    BYTE sign[64];
    EccSign(hash, 32, random, 32, PriKey, 32, sign, 64);

    // base64
    std::string base64_sign = base64_encode(sign, 64);
    return QString::fromStdString(base64_sign);
}


// 验签
bool Sercret_Smx::sm2_Verify(const QByteArray &pubKey, const QByteArray &Sign,
                             const QString &plain, const QByteArray &user)
{
    // 获取公钥
    QString Q_PubKey = pubKey;
    std::string S_PubKey = Q_PubKey.toStdString();
    std::string S_Pubkey_BYTE = HexStr2BYTE(S_PubKey);
    BYTE PubKey[64];
    for (uint i = 0; i < 64; i++)
        PubKey[i] = S_Pubkey_BYTE[i];

    // 获取签名
    QString Q_Sign = Sign;
    std::string S_Sign = Q_Sign.toStdString();

    std::string S_sign = base64_decode(S_Sign);

    BYTE sign[64];

    for (uint i = 0; i < 64; i++)
        sign[i] = S_sign[i];

    /* -------------digest----------- */
    // 提取明文
    QString Q_Plain = plain;
    std::string S_Plain = utf8Togb18030(Q_Plain);

    // 计算摘要
    std::string hash1 = SM3::hash(S_Plain, S_Plain.size());

    QString Q_userid = user;
    std::string S_userid = Q_userid.toStdString();

    std::string S_sm2_par_dig(128, '0');
    for (int i = 0; i < 128; i++) S_sm2_par_dig[i] = sm2_par_dig[i];

    int userid_bitlen = (S_userid.size() << 3);
    std::string s1 = "00";
    s1[0] = (BYTE) ((userid_bitlen >> 8) & 0xFF);
    s1[1] = (BYTE) (userid_bitlen & 0xFF);

    std::string s2 = s1 + S_userid + S_sm2_par_dig + S_Pubkey_BYTE;
    std::string hash2 = SM3::hash(s2, s2.size());

    // hash1
    std::string S_hash1 = HexStr2BYTE(hash1);

    // hash2
    std::string S_hash2 = HexStr2BYTE(hash2);

    // hash3
    std::string s3 = S_hash2 + S_hash1;
    std::string hash3 = SM3::hash(s3, s3.size());

    std::string hash3_BYTE = HexStr2BYTE(hash3);

    BYTE hash[32];

    for (uint i = 0; i < 32; i++) {
        hash[i] = hash3_BYTE[i];
    }

    // 验签
    int isVerify = EccVerify(hash, 32, PubKey, 64, sign, 64);
    if(isVerify) return false;

    return true;
}


// 公钥加密
QString Sercret_Smx::sm2_Encrypt(const QByteArray &raw, const QByteArray &pubKey, uint PlainFlag)
{
    QString Q_Plain = raw;
    std::string S_Plain = Q_Plain.toStdString();

    // 明文处理
    BYTE *Plain;
    uint PlainLen = 0;
    if (PlainFlag == 0) {
        PlainLen = S_Plain.size();
        Plain = new BYTE[S_Plain.size()];
        for (uint i = 0; i < PlainLen; i++)
            Plain[i] = S_Plain[i];
    }
    else {
        PlainLen = S_Plain.size() / 2;
        Plain = new BYTE[PlainLen];
        std::string S_Plain_BYTE = HexStr2BYTE(S_Plain);
        for (uint i = 0; i < PlainLen; i++)
            Plain[i] = S_Plain_BYTE[i];
    }

    // 获取随机数组
    BYTE random[32];
    std::random_device rd;
    for (uint i = 0; i < 32; i++)
        random[i] = (BYTE)(rd() % 256);

    // 获取公钥
    QString Q_PubKey = pubKey;
    std::string S_PubKey = Q_PubKey.toStdString();
    std::string S_Pubkey_BYTE = HexStr2BYTE(S_PubKey);
    BYTE PubKey[64];
    for (uint i = 0; i < 64; i++)
        PubKey[i] = S_Pubkey_BYTE[i];

    // 秘文
    uint CipherLen = PlainLen + 96;
    BYTE *Cipher = new BYTE[CipherLen];

    // 加密
    EccEncrypt(Plain, PlainLen, random, 32, PubKey, 64, Cipher, CipherLen);

    std::string S_Cipher(CipherLen, '0');
    for (uint i = 0; i < CipherLen; i++)
        S_Cipher[i] = Cipher[i];

    // base64
    std::string result_base64 = base64_encode(reinterpret_cast<const BYTE*>
                                              (S_Cipher.c_str()), S_Cipher.length());
    delete Plain;

    return QString::fromStdString(result_base64);
}



// 私钥解密
QString Sercret_Smx::sm2_Decrypt(const QByteArray &raw, const QByteArray &priKey, uint PlainFlag)
{
    QString Q_Plain = raw;
    std::string S_Plain = Q_Plain.toStdString();

    // base64解码
    std::string S_Plain_debase = base64_decode(S_Plain);

    // 获取秘文
    uint CipherLen = S_Plain_debase.size();
    BYTE *Cipher = new BYTE[CipherLen];
    for (uint i = 0; i < CipherLen; i++)
        Cipher[i] = S_Plain_debase[i];

    // 获取私钥
    QString Q_PriKey = priKey;
    std::string S_PriKey = Q_PriKey.toStdString();
    std::string S_Prikey_BYTE = HexStr2BYTE(S_PriKey);
    BYTE PriKey[32];
    for (uint i = 0; i < 32; i++)
        PriKey[i] = S_Prikey_BYTE[i];

    // 生成明文串
    uint PlainLen = CipherLen-96;
    BYTE Plain[PlainLen];

    // decrypt
    EccDecrypt(Cipher, CipherLen, PriKey, 32, Plain, PlainLen);

    std::string Plain_BYTE(PlainLen, '0');
    for (uint i = 0; i < PlainLen; i++)
        Plain_BYTE[i] = Plain[i];
    std::string ret;
    if (PlainFlag == 0) {
        ret.resize(PlainLen);
        //result = Plain_BYTE;
        for (uint i = 0; i < PlainLen; i++)
            ret[i] = Plain_BYTE[i];
    }
    else {
        ret.resize(PlainLen * 2);
        ret = BYTE2HexStr(Plain_BYTE);
    }

    return QString::fromStdString(ret);
}


// 1次摘要
QString Sercret_Smx::sm3(const QByteArray &raw)
{
    QString rawText = raw;
    std::string rawText_str = utf8Togb18030(rawText);

    std::string hash1 = SM3::hash(rawText_str, rawText_str.size());
    return QString::fromStdString(hash1);
}


// 2次摘要
QString Sercret_Smx::sm3_2(const QByteArray &pubKey, const QByteArray &user)
{
    QString Q_userid = user;
    std::string S_userid = Q_userid.toStdString();

    QString Q_pubkey = pubKey;
    std::string S0_pubkey = Q_pubkey.toStdString();
    std::string S_pubkey = HexStr2BYTE(S0_pubkey);

    std::string S_sm2_par_dig(128, '0');
    for (int i = 0; i < 128; i++) S_sm2_par_dig[i] = sm2_par_dig[i];

    int userid_bitlen = (S_userid.size() << 3);
    std::string s1 = "00";
    s1[0] = (BYTE) ((userid_bitlen >> 8) & 0xFF);
    s1[1] = (BYTE) (userid_bitlen & 0xFF);

    std::string s2 = s1 + S_userid + S_sm2_par_dig + S_pubkey;

    std::string hash2 = SM3::hash(s2, s2.size());

    return QString::fromStdString(hash2);
}

// 3次摘要
QString Sercret_Smx::sm3_3(const QByteArray &hash1, const QByteArray &hash2)
{
    // hash1
    QString Q_hash1 = hash1;
    std::string S0_hash1 = Q_hash1.toStdString();
    std::string S_hash1 = HexStr2BYTE(S0_hash1);

    // hash2
    QString Q_hash2 = hash2;
    std::string S0_hash2 = Q_hash2.toStdString();
    std::string S_hash2 = HexStr2BYTE(S0_hash2);

    // hash3
    std::string s3 = S_hash2 + S_hash1;
    std::string hash3 = SM3::hash(s3, s3.size());

    return QString::fromStdString(hash3);
}



QString Sercret_Smx::sm4_crypt(const QByteArray &raw, const QByteArray &Key)
{
    // 随机对称密钥
    QString Q_key = Key;
    std::string S0_key = Q_key.toStdString();
    std::string S_key = HexStr2BYTE(S0_key);
    BYTE key[16] = { 0 };
    for (int i = 0; i < 16; i++) key[i] = S_key[i];

    // 明文
    QString Q_RawText = raw;
    std::string S_RawText = utf8Togb18030(Q_RawText);
    const uint rawl = S_RawText.size();
    BYTE input[rawl] = { 0 };
    for (uint i = 0; i < rawl; i++) input[i] = S_RawText[i];
    const unsigned int n = (rawl / 16 + 1) * 16;
    BYTE input2[n] = { 0 };
    BYTE output[n] = { 0 };

    // PKCS#7填充处理
    pkcs7(input, rawl, n, input2);

    //encrypt standard testing vector
    sm4_context ctx;
    ctx.mode = 0;
    for (uint i = 0; i < 32; i++) ctx.sk[i] = 0;

    BYTE iv[16] = { 0 };
    sm4_setkey_enc(&ctx, key);
    sm4_crypt_cbc(&ctx, 1, n, iv, input2, output);

    std::string secText(n, '0');
    for (uint i = 0; i < n; i++) secText[i] = output[i];

    // base64
    std::string Str = base64_encode(reinterpret_cast<const BYTE*>
                                    (secText.c_str()), secText.length());

    return QString::fromStdString(Str);
}



QString Sercret_Smx::sm4_decrypt(const QByteArray &raw, const QByteArray &Key)
{
    // 随机对称密钥
    QString Q_key = Key;
    std::string S0_key = Q_key.toStdString();
    std::string S_key = HexStr2BYTE(S0_key);
    BYTE key[16] = { 0 };
    for (int i = 0; i < 16; i++) key[i] = S_key[i];

    // 秘文(base64)
    QString Q_SecText = raw;
    std::string S_SecText = Q_SecText.toStdString();

    // debase64
    std::string S_SecText_debase = base64_decode(S_SecText);
    const uint n = S_SecText_debase.size();
    BYTE input[n] = { 0 };
    BYTE output[n] = { 0 };
    for (uint i = 0; i < n; i++) input[i] = S_SecText_debase[i];

    // decrypt
    BYTE iv[16] = { 0 };
    sm4_context ctx;
    ctx.mode = 0;
    for (uint i = 0; i < 32; i++) ctx.sk[i] = 0;
    sm4_setkey_dec(&ctx, key);
    sm4_crypt_cbc(&ctx, 0, n, iv, input, output);

    // 计算填充字节数
    uint l = n - output[n-1];
    std::string ret(l, '0');

    for (uint i = 0; i < l; i++) ret[i] = output[i];

    return QString::fromStdString(ret);
}



QString Sercret_Smx::utils_Unfold(const QByteArray &raw)
{
    QString Q_byteStr = raw;
    std::string S_byteStr = utf8Togb18030(Q_byteStr);

    std::string S_hexStr = BYTE2HexStr(S_byteStr);

    return QString::fromStdString(S_hexStr);
}



QString Sercret_Smx::utils_Fold(const QByteArray &raw)
{
    QString Q_hexStr = raw;
    std::string S_hexStr = utf8Togb18030(Q_hexStr);

    std::string S_byteStr = HexStr2BYTE(S_hexStr);

    return QString::fromStdString(S_byteStr);
}
