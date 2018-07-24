/*
 * =========================================================================================
 * Name        : processData.cpp
 * Description : student code for Assignment 1 - Data structures and Algorithms - Spring 2018
 * =========================================================================================
 */
#include "requestLib.h"
#include "dbLib.h"
#define NOTFOUND cout << "not found!" << endl

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

/**
 * =============================================================================
 *
 * IMPLEMENT ALL FEATURE
 * 
 * ============================================================================= 
 * */
void devices(VRecord &data, void *list){
    L1List<VRecord>* l = (L1List<VRecord>*) list;
    if (!l->exist(data)) l->insertHead(data);
}

void devices_resetX(VRecord &data, void *list){
    L1List<VRecord>* l = (L1List<VRecord>*) list;
    int idx = -1;
    if (!l->find(data,idx)){
        l->insertHead(data);
        l->at(0).x = 1.0;
    }
    else {
        l->at(idx).x += 1.0;
    }
}

void findDevice(VRecord &data, void *cmp) {
    VRecord *r = (VRecord *) cmp;
    if (*r == data) {
        r->x = data.x;
        r->y = data.y;
        r->timestamp = data.timestamp;
    }
}

void getRoD(VRecord &data, void *list) {
    L1List<VRecord>* l = (L1List<VRecord>*) list;
    if (data == l->at(0)) {
        l->push_back(data);
    }
}

void getMax(VRecord &data, void *list){
    L1List<VRecord> *l = (L1List<VRecord>*) list;
    if (data.x >= l->at(0).x){
        l->removeHead();
        l->insertHead(data);
    }
}

void getMin(VRecord &data, void *list){
    L1List<VRecord> *l = (L1List<VRecord>*) list;
    if (data.x <= l->at(0).x){
        l->removeHead();
        l->insertHead(data);
    }
}

bool CNV(char *args, L1List<VRecord> &recList){
    if (args){
        NOTFOUND;
        return true;
    }
    L1List<VRecord> ListUnit;
    recList.traverse(devices, &ListUnit);
    cout << ListUnit.getSize() << endl;
    return true;
}

bool VFF(char *cmd, L1List<VRecord> &recList) {
    if (cmd != NULL)
        NOTFOUND;
    else {
        if (recList.isEmpty()) NOTFOUND;
        else cout << recList[0].id << endl;
    }
    return true;
}

bool VFL(char *cmd, L1List<VRecord> &recList) {
    if (cmd) NOTFOUND;
    L1List<VRecord> l;
    recList.traverse(devices, &l);
    l.reverse();
    if (!l.isEmpty()) {
        cout << l[l.getSize() - 1].id << endl;
    } else NOTFOUND;
    return true;
}

bool VLY(char *cmd, L1List<VRecord> &recList){
    if (!cmd) return false;
    VRecord r(cmd);
    r.y = -1;
    recList.traverse(findDevice, &r);
    if (r.y != -1)
        cout << r.y << endl;
    else NOTFOUND;
    return true;
}

bool VLX(char *cmd, L1List<VRecord> &recList){
    if (!cmd) return false;
    VRecord r(cmd);
    r.x = -1;
    recList.traverse(findDevice, &r);
    if (r.x != -1)
        cout << r.x << endl;
    else NOTFOUND;
    return true;
}

void getMaxRoD(VRecord &data, void *list){}

bool VFY(char *cmd, L1List<VRecord> &recList) {
    if (!cmd) return false;
    L1List<VRecord> l;
    recList.traverse(devices, &l);
    VRecord r(cmd);
    r.y = -1;
    l.traverse(findDevice, &r);
    if (r.y != -1)
    cout << r.y << endl;
    else NOTFOUND;
    return true;
}

bool VFX(char *cmd, L1List<VRecord> &recList) {
    if (!cmd) return false;
    L1List<VRecord> l;
    recList.traverse(devices, &l);
    VRecord r(cmd);
    r.x = -1;
    l.traverse(findDevice, &r);
    if (r.x != -1)
    cout << r.x << endl;
    else NOTFOUND;
    return true;
}

bool VFT(char *cmd, L1List<VRecord> &recList) {
    if (!cmd) return false;
    L1List<VRecord> l;
    recList.traverse(devices, &l);
    VRecord r(cmd);
    r.timestamp = -1;
    l.traverse(findDevice, &r);
    if (r.timestamp != -1){
        char *ret = new char[100];
        strPrintTime(ret, r.timestamp);
        cout << ret << endl;
    } else NOTFOUND;
    return true;
}

bool VLT(char *cmd, L1List<VRecord> &recList) {
    if (!cmd) return false;
    VRecord r(cmd);
    r.timestamp = -1;
    recList.traverse(findDevice, &r);
    if (r.timestamp != -1){
        char *ret = new char[100];
        strPrintTime(ret, r.timestamp);
        cout << ret << endl;
    } else NOTFOUND;
    return true;
}

bool VCR(char *cmd, L1List<VRecord> &recList) {
    if (!cmd) return false;
    VRecord r(cmd);
    L1List<VRecord> l;
    l.insertHead(r);
    recList.traverse(getRoD, &l);
    l.removeHead();

    cout << l.getSize() << endl;
    return true;
}

bool VCL(char *cmd, L1List<VRecord> &recList){
    if (!cmd) return false;
    VRecord r(cmd);
    L1List<VRecord> l;
    l.insertHead(r);
    recList.traverse(getRoD, &l);
    l.removeHead();
    if (l.isEmpty()) NOTFOUND;
    else {
        L1Item<VRecord> *_pCur = l.getHead();
        L1Item<VRecord> *_pNext = _pCur->pNext;
        double sumDistance = 0.0;
        while (_pNext){
            sumDistance += distanceVR(_pCur->data.y,_pCur->data.x,_pNext->data.y,_pNext->data.x);
            _pCur = _pNext;
            _pNext = _pCur->pNext;
        }
        cout << sumDistance << endl;
    }
    return true;

}

bool VAS(char *cmd, L1List<VRecord> &recList){
    if (!cmd) return false;
    VRecord r(cmd);
    L1List<VRecord> l;
    l.insertHead(r);
    recList.traverse(getRoD, &l);
    l.removeHead();
    if (l.isEmpty()) NOTFOUND;
    else {
        L1Item<VRecord> *_pCur = l.getHead();
        L1Item<VRecord> *_pNext = _pCur->pNext;
        double sumDistance = 0.0;
        while (_pNext){
            sumDistance += distanceVR(_pCur->data.y,_pCur->data.x,_pNext->data.y,_pNext->data.x);
            _pCur = _pNext;
            _pNext = _pCur->pNext;
        }
        cout << (sumDistance / (double)(l.getSize() - 1)) * 1000<< " meter" << endl;
    }
    return true;
}

bool MRV(char *cmd, L1List<VRecord> &recList){
    if (cmd){
        NOTFOUND;
        return true;
    }
    else {
        L1List<VRecord> ListUnit;
        recList.traverse(devices_resetX, &ListUnit);
        L1List<VRecord> l;
        l.insertHead(ListUnit.getHead()->data);
        ListUnit.traverse(getMax,&l);

        cout << l.at(0).id << endl;
    }
    return true;

}

bool LRV(char *cmd, L1List<VRecord> &recList){
    if (cmd){
        NOTFOUND;
        return true;
    }
    else {
        L1List<VRecord> ListUnit;
        recList.traverse(devices_resetX, &ListUnit);
        L1List<VRecord> l;
        l.insertHead(ListUnit.getHead()->data);
        ListUnit.traverse(getMin,&l);

        cout << l.at(0).id << endl;
    }
    return true;
}

enum CmdType {
    CNVType, 
    VFFType, VFLType, 
    VFYType, VFXType,
    VLYType, VLXType,
    VFTType, VLTType,
    VCRType, VCLType,
    VMTType,
    VFSType, VLSType,
    VMSType,
    VASType,
    MSTType,
    CNRType,
    MRVType, LRVType,
    MTVType,
    MVVType,
    CNSType,
    CASType,
    LPVType, SPVType,
    RVRType
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
        case VFLType: {
            string vfl = "VFL";
            strcpy(ret, vfl.data());
            return ret;
        }
        case VLYType:{
            string vly = "VLY";
            strcpy(ret,vly.data());
            return ret;
        }
        case VLXType:{
            string vlx = "VLX";
            strcpy(ret,vlx.data());
            return ret;
        }
        case VFYType: {
            string vfy = "VFY";
            strcpy(ret, vfy.data());
            return ret;
        }
        case VFXType: {
            string vfx = "VFX";
            strcpy(ret, vfx.data());
            return ret;
        }
        case VCLType: {
            string vcl = "VCL";
            strcpy(ret, vcl.data());
            return ret;
        }
            case VFTType: {
            string vfx = "VFT";
            strcpy(ret, vfx.data());
            return ret;
        }
        case VLTType: {
            string vfx = "VLT";
            strcpy(ret, vfx.data());
            return ret;
        }
        case VCRType: {
            string vfx = "VCR";
            strcpy(ret, vfx.data());
            return ret;
        }
        case VASType: {
            string vas = "VAS";
            strcpy(ret, vas.data());
            return ret;
        }
        case MRVType: {
            string mrv = "MRV";
            strcpy(ret, mrv.data());
            return ret;
        }
        case LRVType: {
            string lrv = "LRV";
            strcpy(ret,lrv.data());
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
    mCMD->registerCommand(getCmdLabel(VFLType), VFL);
    mCMD->registerCommand(getCmdLabel(VLYType), VLY);
    mCMD->registerCommand(getCmdLabel(VLXType), VLX);
    mCMD->registerCommand(getCmdLabel(VCLType), VCL);
    mCMD->registerCommand(getCmdLabel(VFYType), VFY);
    mCMD->registerCommand(getCmdLabel(VFXType), VFX);
    mCMD->registerCommand(getCmdLabel(VFTType), VFT);
    mCMD->registerCommand(getCmdLabel(VLTType), VLT);
    mCMD->registerCommand(getCmdLabel(VCRType), VCR);
    mCMD->registerCommand(getCmdLabel(VASType), VAS);
    mCMD->registerCommand(getCmdLabel(MRVType), MRV);
    mCMD->registerCommand(getCmdLabel(LRVType), LRV);
    return true;
}
void releaseVGlobalData(void* pGData) {
    CommandManager *mCMD = static_cast<CommandManager *>(pGData);
    delete mCMD;
}

bool processRequest(VRequest& request, L1List<VRecord>& recList, void* pGData) {
    CommandManager *mCMD = (CommandManager *) pGData;
    cout << request.code << ": ";
    return mCMD->process(request, recList);
}
