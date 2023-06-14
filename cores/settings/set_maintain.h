#ifndef SET_MAINTAIN_H
#define SET_MAINTAIN_H
#include "set_updater.h"

class Set_Maintain : public Set_Updater
{
public:
    Set_Maintain();

    bool syscmd(int fc);
    void factoryRestore();
    QString backups(int fc);
    bool restores(int fc, const QVariant &v);

private:
    void clearLogs();
    QString batchBackup();
    QString profileBackup();
    bool restory(const QString &fn);
    bool profileRestore(const QString &fn);
    bool batchRestore(const QString &fn);

private:
    std::thread *m_thread;
};

#endif // SET_MAINTAIN_H
