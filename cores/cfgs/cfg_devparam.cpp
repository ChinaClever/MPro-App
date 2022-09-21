#include "cfg_devparam.h"
#include "log_core.h"

Cfg_devParam::Cfg_devParam()
{
    QString fn = Cfg_Obj::pathOfCfg("dev.ini");
    mDevCfg = new Cfg_Obj(fn);

}

void Cfg_devParam::uutInfoRead(sUutInfo &uut)
{
    Cfg_Obj *cfg = mDevCfg; QString g = "uut";
    QString str = cfg->readCfg("room", "", g).toString();
    qstrcpy(uut.room, str.toLatin1().data());

    str = cfg->readCfg("location", "", g).toString();
    qstrcpy(uut.location, str.toLatin1().data());

    str = cfg->readCfg("devName", "", g).toString();
    qstrcpy(uut.devName, str.toLatin1().data());

    str = cfg->readCfg("qrcode", "", g).toString();
    qstrcpy(uut.qrcode, str.toLatin1().data());

    str = cfg->readCfg("sn", "", g).toString();
    qstrcpy(uut.sn, str.toLatin1().data());
}

void Cfg_devParam::devNumRead(sDevNums &it)
{
    Cfg_Obj *cfg = mDevCfg; QString g = "devNums";
    it.slaveNum = cfg->readCfg("slaveNum", 0, g).toInt();
    it.boardNum = cfg->readCfg("boardNum", 3, g).toInt();
    it.lineNum = cfg->readCfg("lineNum", LINE_NUM, g).toInt();
    it.loopNum = cfg->readCfg("boardNum", LOOP_NUM/2, g).toInt();
    it.outputNum = cfg->readCfg("outputNum", OUTPUT_NUM/2, g).toInt();
    it.groupEn = cfg->readCfg("groupEn", 0, g).toInt();

    for(uint i=0; i<it.boardNum; ++i) {
        QString key = "boards_" + QString::number(i+1);
        it.boards[i] = cfg->readCfg(key, 8, g).toUInt();
    }

    for(uint i=0; i<it.loopNum; ++i) {
        QString key = "loopStarts" + QString::number(i+1);
        it.loopStarts[i] = cfg->readCfg(key, 8*i, g).toUInt();
        key = "loopEnds" + QString::number(i);
        it.loopEnds[i] = cfg->readCfg(key, 8*(i+1), g).toUInt();
    }
}

bool Cfg_devParam::initialParam()
{
    sDevData *dev = cm::masterDev();
    dev->cfg.param.isBreaker = 1;
    initDevInfo(dev->cfg.nums);
    return true;
}

void Cfg_devParam::initDevInfo(sDevNums &it)
{
    it.boardNum = 3;
    it.lineNum = LINE_NUM;
    it.loopNum = LOOP_NUM / 2;
    it.outputNum = OUTPUT_NUM / 2;
    for(uint i=0; i<it.loopNum; ++i) {
        it.loopEnds[i] = 8*(i+1);
        it.loopStarts[i] = 8*i;
    }
}


bool Cfg_devParam::writeParams()
{
    QFile file(Cfg_Com::pathOfCfg(CFG_PARAM_FN));
    bool ret = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(ret) {
        QByteArray array; ushort end = END_CRC;
        QDataStream in(&array, QIODevice::WriteOnly);
        sDevCfg *cfg = &(cm::masterDev()->cfg);
        in << cm::toByteArray(cfg->nums)
           << cm::toByteArray(cfg->param)
           << cm::toByteArray(cfg->uut) << end;
        file.write(array);
    }
    file.close();
    return ret;
}

bool Cfg_devParam::readParam(const QString &fn)
{
    bool ret = false; QFile file(Cfg_Com::pathOfCfg(fn));
    if(file.exists() && file.open(QIODevice::ReadOnly)) {
        QByteArray array = file.readAll();
        if(array.size()) {
            QByteArray nums, param, uut;
            QDataStream out(&array, QIODevice::ReadOnly);
            ushort end; out >> nums >> param >> uut >> end;
            if(end == END_CRC){
                sDevCfg *cfg = &cm::masterDev()->cfg;
                cfg->nums = cm::toStruct<sDevNums>(nums);
                cfg->param = cm::toStruct<sParameter>(param);
                cfg->uut = cm::toStruct<sUutInfo>(uut); ret = true;
            } else {
                sSysItem it; it.module = QStringLiteral("配置参数");
                it.content = QStringLiteral("设备配置参数读取异常:");
                it.content += file.errorString();
                Log_Core::bulid()->append(it);
                cout << it.module << it.content << Cfg_Com::pathOfCfg(fn);
            }
        }
    }file.close();

    return ret;
}
