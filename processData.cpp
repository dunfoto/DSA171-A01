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

    void print() {
        cout << cmd << endl;
    }

    CommandIfo() : cmd(NULL), op(NULL) {}

    bool run(char *args, L1List<VRecord>& recList) {
        return op(args, recList);
    }

    bool operator==(CommandIfo c) {
        bool ret =  isTrue(c.cmd);
        return ret;
    }

    bool isTrue(char *c) {
        return strcmp(cmd, c) == 0;
    }
};

class CommandManager {
private:
    CommandIfo AtrNULL;
    L1List<CommandIfo> mCommand;

    bool getCommand(VRequest& request, CommandIfo& info) {
        char *cmd = request.getCmd();
        int idx = -1;
        CommandIfo tmp(cmd);
        if (mCommand.find(tmp, idx)) {
            info = mCommand[idx];
            return true;
        }
        return false;
    }

    char* getArgs(VRequest& request) {
        char *arg = request.getArgs();
        if (strlen(arg) == 0) {
            return NULL;
        }
        return arg;
    }
public:
    CommandManager() {};
    ~CommandManager() {};

    int getSize(){
        return mCommand.getSize();
    }

    void registerCommand(char *cmd, bool (*inop)(char*,L1List<VRecord>&)) {
        CommandIfo command = CommandIfo(cmd, inop);
        mCommand.push_back(command);
    }

    bool process(VRequest& request, L1List<VRecord>& recList) {
        CommandIfo p;
        bool ret = getCommand(request, p);
        if (!ret) return false;
        char *args = getArgs(request);
        return p.run(args, recList);
    }
};

bool VFF(char *cmd, L1List<VRecord> &recList) {
    if (cmd != NULL)
        return false;
    else {
        if (recList.isEmpty()) 
            return false;
        else cout << recList[0].id << endl;
        return true;
    }
}

bool VFL(char *cmd, L1List<VRecord> &recList) {
    return true;
}

enum CmdType {
    CNVType, VFFType
};

char *getCmdLabel(CmdType type) {
    char * ret = new char[3];
    switch (type) {
        case CNVType: {
            string cnv = "CNV";
            strcpy(ret, cnv.data());
            return ret;
        }
        case VFFType: {
            string vff = "VFF";
            strcpy(ret, vff.data());
            return ret;
        }
    }
    return ret;
}

bool initVGlobalData(void** pGData) {
    CommandManager *mCMD = new CommandManager();
    *pGData = mCMD;
    //Register all command
    mCMD->registerCommand(getCmdLabel(CNVType), CNV);
    mCMD->registerCommand(getCmdLabel(VFFType), VFF);

    return true;
}
void releaseVGlobalData(void* pGData) {
    CommandManager *mCMD = static_cast<CommandManager *>(pGData);
    delete mCMD;
}

bool processRequest(VRequest& request, L1List<VRecord>& recList, void* pGData) {
    CommandManager *mCMD = (CommandManager *) pGData;
    cout << request.code << ": ";
    bool ret = mCMD->process(request, recList);
    if (!ret) cout << "not found!" << endl;
    return true;
}
