/*
 * =========================================================================================
 * Name        : processData.cpp
 * Description : student code for Assignment 1 - Data structures and Algorithms - Spring 2018
 * =========================================================================================
 */
#include "requestLib.h"
#include "dbLib.h"

class CommandIfo {
public:
    bool (*op)(char*,L1List<VRecord>&);
    char *cmd;
    CommandIfo(char *c, bool (*inop)(char*,L1List<VRecord>&)) {
        cmd = c;
        op = inop;
    }

    CommandIfo(char *c) {
        op = NULL;
        cmd = c;
    }

    CommandIfo() : cmd(NULL), op(NULL) {}

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
private:
    L1List<CommandIfo> *mCommand = new L1List<CommandIfo>();

    CommandIfo getCommand(VRequest& request) {
        char *cmd = request.getCmd();
        int idx = -1;
        CommandIfo *tmp = new CommandIfo(cmd);
        if (mCommand->find(tmp, idx)) {
            delete cmd;
            return mCommand->at(idx);
        }
        delete cmd;
        return NULL;
    }

    char* getArgs(VRequest& request) {
        char *arg = request.getArgs();
        if (strlen(arg) == 0) {
            delete arg;
            return NULL;
        }
        return arg;
    }
public:
    CommandManager() {};
    ~CommandManager() {
        delete mCommand;
    };

    int getSize(){
        return mCommand->getSize();
    }

    void registerCommand(char *cmd, bool (*inop)(char*,L1List<VRecord>&)) {
        CommandIfo command = CommandIfo(cmd, inop);
        mCommand->insertHead(command);
    }

    bool process(VRequest& request, L1List<VRecord>& recList) {
        CommandIfo p = getCommand(request);
        char *args = getArgs(request);
        return p.run(args, recList);
    }
};



bool initVGlobalData(void** pGData) {
    CommandManager *mCMD = new CommandManager();
    *pGData = mCMD;
    //Register all command
    mCMD->registerCommand("CNV", CNV);
    
    CommandManager *m = (CommandManager*) pGData;

    return true;
}
void releaseVGlobalData(void* pGData) {
    CommandManager *mCMD = static_cast<CommandManager *>(pGData);
    delete mCMD;
}

bool processRequest(VRequest& request, L1List<VRecord>& recList, void* pGData) {
    CommandManager *mCMD = static_cast<CommandManager *>(pGData);
    return mCMD->process(request, recList);
}
