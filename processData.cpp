/*
 * =========================================================================================
 * Name        : processData.cpp
 * Description : student code for Assignment 1 - Data structures and Algorithms - Spring 2018
 * =========================================================================================
 */
#include "requestLib.h"
#include "dbLib.h"

#define CODELEN = 3

void CNV_addunit(VRecord &data,void *list){
    L1List<VRecord>* l = (L1List<VRecord>*) list;
    if (!l->exist(data)) l->insertHead(data);
}

bool CNV(char *args,L1List<VRecord> &recList){
    if (args){
        cout << "CNV:Failed";
        return false;
    }
    L1List<VRecord> *ListUnit = new L1List<VRecord>();
    recList.traverse(CNV_addunit,ListUnit);
    cout << ListUnit->getSize();
    return true;
}

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
    L1List<CommandIfo*> *mCommand = new L1List<CommandIfo*>();

    CommandIfo* getCommand(VRequest& request) {
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

    void registerCommand(char *cmd, bool (*inop)(char*,L1List<VRecord>&)) {
        CommandIfo *command = new CommandIfo(cmd, inop);
        mCommand->insertHead(command);
    }

    bool process(VRequest& request, L1List<VRecord>& recList) {
        CommandIfo *p = getCommand(request);
        char *args = getArgs(request);
        if (p) {
            return p->run(args, recList);
        }
        return false;
    }
};

bool CNV(char *cmd, L1List<VRecord> &recList) {
    
}

bool initVGlobalData(void** pGData) {
    CommandManager *mCMD = new CommandManager();
    pGData = (void**) &mCMD;

    //Register all command
    mCMD->registerCommand("CNV", CNV);

    return true;
}
void releaseVGlobalData(void* pGData) {
    delete pGData;
}

bool processRequest(VRequest& request, L1List<VRecord>& recList, void* pGData) {
    CommandManager *mCMD = (CommandManager*) pGData;
    return mCMD->process(request, recList);
}
