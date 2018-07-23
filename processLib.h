#include "requestLib.h"
#include "dbLib.h"

class CommandInfo {
    Command cmd;
    CommandInfo(Command c) : cmd(c) {};
    void process(string arg, L1List<VRecord>& recList, void* pGData) {
        cmd.process(argm recList, pGData);
    }
}

class Command {
    virtual void process(string arg, L1List<VRecord>& recList, void* pGData);
}

class NoArgCommand : public Command {
    virtual void process(L1List<VRecord>& recList, void* pGData);
    void process(string arg, L1List<VRecord>& recList, void* pGData) {
        process(recList, pGData);
    }
}
