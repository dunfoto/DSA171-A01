/*
 * =========================================================================================
 * Name        : processData.cpp
 * Description : student code for Assignment 1 - Data structures and Algorithms - Spring 2018
 * =========================================================================================
 */
#include "requestLib.h"
#include "dbLib.h"

#define CODELEN = 3

class CommandIfo {
public:
    unsigned bool (*op)(char*,L1List<VRecord>&);
    char *cmd;
    CommandIfo(char *c, bool (*inop)(char*,L1List<VRecord>&)) {
        cmd = c;
        op = inop;
    }

    CommandIfo(char *c) {
        op = NULL;
        cmd = c;
    }

    bool run(char *args, L1List<VRecord>& recList) {
        return op(args, recList);
    }

    bool operator==(CommandIfo c) {
        return isTrue(c.cmd);
    }

    bool isTrue(char *c) {
        return strcmp(cmd, c) == 0;
    }
};

class CommandManager {
public:
    CommandManager() {
        mCommand = new L1List<CommandIfo>();
    };
    ~CommandManager() {
        delete mCommand;
    };

    L1List<CommandIfo> *mCommand;

    void registerCommand(char *cmd, void (*inop)(char*,L1List<VRecord>&)) {
        mCommand->insertHead(CommandIfo(cmd, inop));
    }

    bool process(VRequest& resquest, L1List<VRecord>& recList) {
        CommandIfo p = getCommand(request);
        char *args = getArgs(request);
        if (p) {
            return p->run(args, recList);
        }
        return false;
    }
private:
    CommandIfo getCommand(VRequest& request) {
        char *cmd = request.getCmd();
        int idx = -1;
        CommandIfo tmp = CommandIfo(cmd);
        if (mCommand->find(tmp, idx)) {
            delete cmd;
            return mCommand[idx];
        }
        delete cmd;
    }

    char* getArgs(VRequest& request) {
        char *arg = request.getArgs();
        if (strlen(arg) == 0) {
            delete arg;
            return NULL;
        }
        return arg;
    }
};

bool initVGlobalData(void** pGData) {
    pGData = new CommandManager();

    //Register all command
    pGData->registerCommand("CNV", CNV);

    return true;
}
void releaseVGlobalData(void* pGData) {
    delete pGData;
}

bool processRequest(VRequest& request, L1List<VRecord>& recList, void* pGData) {
    return pGData->process(request, recList);
}
