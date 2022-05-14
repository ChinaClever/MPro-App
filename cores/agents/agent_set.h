#ifndef AGENT_SET_H
#define AGENT_SET_H

#include "agent_get.h"

class Agent_Set : public Agent_Get
{
    Q_OBJECT
public:
    explicit Agent_Set(QObject *parent = nullptr);

signals:

};

#endif // AGENT_SET_H
