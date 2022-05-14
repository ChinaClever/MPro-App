#include "agent_core.h"

Agent_Core::Agent_Core(QObject *parent)
    : Agent_Trap{parent}
{

}

Agent_Core *Agent_Core::bulid(QObject *parent)
{
    static Agent_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Agent_Core(parent);
    }
    return sington;
}
