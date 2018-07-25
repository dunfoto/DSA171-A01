/*
 * =========================================================================================
 * Name        : processData.cpp
 * Description : student code for Assignment 1 - Data structures and Algorithms - Spring 2018
 * =========================================================================================
 */
#include "requestLib.h"
#include "dbLib.h"
#define NOTFOUND cout << "not found!" << endl
#define NONSTOP cout << "non stop!" << endl

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

bool checkLocation(VRecord& a, VRecord& b) {
    return distanceVR(a.y, a.x, b.y, b.x) < 0.005;
}

void getStopRecords(VRecord &data, void *list) {
    L1List<VRecord> *ls = (L1List<VRecord>*) list;
    if (ls -> isEmpty()) ls -> insertHead(data);
    else {
        if (ls -> getSize() == 1) {
            if (checkLocation(ls[0][0], data)){ 
                ls -> insertHead(data);
            } else {
                ls -> removeHead();
                ls -> insertHead(data);
            }
        } else {
            if (checkLocation(ls[0][0], data)) {
                ls -> insertHead(data);
            } else {
                if (checkLocation(ls[0][0], ls[0][1])) {
                    ls -> insertHead(data);
                } else {
                    ls -> removeHead();
                    ls -> insertHead(data);
                }
            }
        }
    }
}

void removeStopRecords(L1List<VRecord> *ls) {
    if (ls->isEmpty() || ls->getSize() == 1) {
        ls->clean();
        return;
    }
    if (!checkLocation(ls[0][0], ls[0][1])) {
        ls -> removeHead();
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
        delete ret;
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

    if (!l.isEmpty())
        cout << l.getSize() << endl;
    else NOTFOUND;
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

bool VMT(char *cmd, L1List<VRecord> &recList){
    if (!cmd) return false;
    VRecord r(cmd);
    L1List<VRecord> l;
    l.insertHead(r);
    recList.traverse(getRoD, &l);
    l.removeHead();

    int stopTime = 0;
    for (int i = 0; i < l.getSize() - 1; i++) {
        if (checkLocation(l[i], l[i+1])) {
            stopTime += l[i+1].timestamp - l[i].timestamp;
        }
    }
    if (!l.isEmpty()){
        int t = l[l.getSize() - 1].timestamp - l[0].timestamp - stopTime;
        cout << t << endl;
    }
    else NOTFOUND;
    return true;
}

bool VFS(char *cmd, L1List<VRecord> &recList){
    if (!cmd) return false;
    VRecord r(cmd);
    L1List<VRecord> l;
    l.insertHead(r);
    recList.traverse(getRoD, &l);
    l.removeHead();

    if (!l.isEmpty()){
        L1List<VRecord> result;
        l.traverse(getStopRecords, &result);
        removeStopRecords(&result);
        result.reverse();
        if (result.isEmpty()) NONSTOP;
        else cout << "(" << result[0].x << " " << result[0].y << ")" << endl;
    }
    else NOTFOUND;
    return true;
}

bool VLS(char *cmd, L1List<VRecord> &recList){
    if (!cmd) return false;
    VRecord r(cmd);
    L1List<VRecord> l;
    l.insertHead(r);
    recList.traverse(getRoD, &l);
    l.removeHead();

    if (!l.isEmpty()){
        L1List<VRecord> result;
        l.traverse(getStopRecords, &result);
        removeStopRecords(&result);
        result.reverse();
        if (result.isEmpty()) NONSTOP;
        else cout << "(" << result[result.getSize() - 2].x 
                    << " " << result[0].y << ")" << endl;
    }
    else NOTFOUND;
    return true;
}

bool VMS(char *cmd, L1List<VRecord> &recList){
    if (!cmd) return false;

    VRecord r(cmd);
    L1List<VRecord> l;
    l.insertHead(r);
    recList.traverse(getRoD, &l);
    l.removeHead();

    if (!l.isEmpty()){
        int j = -1;
        int crtTime = 0;
        int maxTime = -1;
        for (int i = 0; i < l.getSize() - 1; i++) {
            if (checkLocation(l[i], l[i+1])) {
                if (j == -1) j = i;
                if (checkLocation(l[j], l[i+1])) {
                    crtTime = l[i+1].timestamp - l[j].timestamp;
                    if (crtTime > maxTime) maxTime = crtTime;
                } else {
                    j = i;
                    crtTime = l[i+1].timestamp - l[j].timestamp;
                    if (crtTime > maxTime) maxTime = crtTime;
                }
            } else {
                j = -1;
                crtTime = 0;
            } 
        }
        if (maxTime != -1) cout << maxTime << endl;
        else NONSTOP;
    } else NONSTOP;
    return true;
}

bool MST(char *cmd, L1List<VRecord> &recList){
    if (cmd) {
        NOTFOUND;
        return true;
    }

    L1List<VRecord> ListUnit;
    recList.traverse(devices, &ListUnit);

    int maxTime = 0;
    for (int i = 0; i < ListUnit.getSize(); i++) {
        L1List<VRecord> l;
        l.insertHead(ListUnit[i]);
        recList.traverse(getRoD, &l);
        l.removeHead();

        if (!l.isEmpty()){
            int j = -1;
            int crtTime = 0;
            for (int i = 0; i < l.getSize() - 1; i++) {
                if (checkLocation(l[i], l[i+1])) {
                    if (j == -1) j = i;
                    if (checkLocation(l[j], l[i+1])) {
                        crtTime = l[i+1].timestamp - l[j].timestamp;
                        if (crtTime > maxTime) maxTime = crtTime;
                    } else {
                        j = i;
                        crtTime = l[i+1].timestamp - l[j].timestamp;
                        if (crtTime > maxTime) maxTime = crtTime;
                    }
                } else {
                    j = -1;
                    crtTime = 0;
                }
                
            }
        }
    }
    if (maxTime == 0) NONSTOP;
    else cout << maxTime << 's' << endl;

    return true;
}

bool CNR(char *cmd, L1List<VRecord> &recList){
    if (cmd) {
        NOTFOUND;
        return true;
    }
    cout << recList.getSize() << endl;
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

bool MTV(char *cmd, L1List<VRecord> &recList) {
    if (cmd) {
        NOTFOUND;
        return true;
    }

    L1List<VRecord> ListUnit;
    recList.traverse(devices, &ListUnit);

    int maxTime = -1;
    char    id[16];
    for (int i = 0; i < ListUnit.getSize(); i++) {
        VRecord crt = ListUnit[i];
        L1List<VRecord> l;
        l.insertHead(crt);
        recList.traverse(getRoD, &l);
        l.removeHead();

        if (!l.isEmpty()){
            int t = l[l.getSize() - 1].timestamp - l[0].timestamp;
            if (t > maxTime) {
                maxTime = t;
                strcpy(id, crt.id);
            }
        }
        else continue;
    }
    if (maxTime == -1) NONSTOP;
    else cout << id << endl;

    return true;
}

bool MVV(char *cmd, L1List<VRecord> &recList) {
    if (cmd) {
        NOTFOUND;
        return true;
    }

    L1List<VRecord> ListUnit;
    recList.traverse(devices, &ListUnit);

    char id[16];
    double maxV = -1;
    for (int i = 0; i < ListUnit.getSize(); i++) {
        VRecord crt = ListUnit[i];
        L1List<VRecord> l;
        l.insertHead(crt);
        recList.traverse(getRoD, &l);
        l.removeHead();
        if (l.isEmpty()) continue;
        else {
            double t = (double) l[l.getSize() - 1].timestamp - l[0].timestamp; 
            L1Item<VRecord> *_pCur = l.getHead();
            L1Item<VRecord> *_pNext = _pCur->pNext;
            double sumDistance = 0.0;
            while (_pNext){
                sumDistance += distanceVR(_pCur->data.y,_pCur->data.x,_pNext->data.y,_pNext->data.x);
                _pCur = _pNext;
                _pNext = _pCur->pNext;
            }
            double crtV = sumDistance / t;
            if (crtV > maxV) {
                maxV = crtV;
                strcpy(id, crt.id);
            }
        }
    }

    if (maxV == -1) NOTFOUND;
    else cout << id << endl;
    return true;
}

bool CNS(char *cmd, L1List<VRecord> &recList){
    if (cmd) {
        NOTFOUND;
        return true;
    }

    L1List<VRecord> ListUnit;
    recList.traverse(devices, &ListUnit);

    int counter = 0;
    for (int i = 0; i < ListUnit.getSize(); i++) {
        L1List<VRecord> l;
        l.insertHead(ListUnit[i]);
        recList.traverse(getRoD, &l);
        l.removeHead();

        if (!l.isEmpty()){
            L1List<VRecord> result;
            l.traverse(getStopRecords, &result);
            removeStopRecords(&result);
            if (result.isEmpty()) counter++;
        }
    }
    cout << counter << endl;

    return true;
}

bool CAS(char *cmd, L1List<VRecord> &recList) {
    if (cmd) {
        NOTFOUND;
        return true;
    }

    L1List<VRecord> ListUnit;
    recList.traverse(devices, &ListUnit);

    double distance = 0;
    for (int i = 0; i < ListUnit.getSize(); i++) {
        VRecord crt = ListUnit[i];
        L1List<VRecord> l;
        l.insertHead(crt);
        recList.traverse(getRoD, &l);
        l.removeHead();
        if (l.isEmpty()) continue;
        else {
            L1Item<VRecord> *_pCur = l.getHead();
            L1Item<VRecord> *_pNext = _pCur->pNext;
            while (_pNext){
                distance += distanceVR(_pCur->data.y,_pCur->data.x,_pNext->data.y,_pNext->data.x);
                _pCur = _pNext;
                _pNext = _pCur->pNext;
            }
        }
    }
    double p = (double) ListUnit.getSize() / 1000;
    double dis = (double) distance / p;
    cout << dis << " meter" << endl;
    return true;
}

bool LPV(char *cmd, L1List<VRecord> &recList) {
    if (cmd) {
        NOTFOUND;
        return true;
    }

    L1List<VRecord> ListUnit;
    recList.traverse(devices, &ListUnit);

    double maxDistance = 0;
    char id[16];
    for (int i = 0; i < ListUnit.getSize(); i++) {
        VRecord crt = ListUnit[i];
        L1List<VRecord> l;
        l.insertHead(crt);
        recList.traverse(getRoD, &l);
        l.removeHead();
        if (l.isEmpty()) continue;
        else {
            L1Item<VRecord> *_pCur = l.getHead();
            L1Item<VRecord> *_pNext = _pCur->pNext;
            double distance = 0.0;
            while (_pNext){
                distance += distanceVR(_pCur->data.y,_pCur->data.x,_pNext->data.y,_pNext->data.x);
                _pCur = _pNext;
                _pNext = _pCur->pNext;
            }
            if (maxDistance <= distance) {
                maxDistance = distance;
                strcpy(id, crt.id);
            }
        }
    }
    
    cout << id << endl;
    return true;
}

bool SPV(char *cmd, L1List<VRecord> &recList) {
    if (cmd) {
        NOTFOUND;
        return true;
    }

    L1List<VRecord> ListUnit;
    recList.traverse(devices, &ListUnit);

    double minDistance = 0;
    char id[16];
    for (int i = 0; i < ListUnit.getSize(); i++) {
        VRecord crt = ListUnit[i];
        L1List<VRecord> l;
        l.insertHead(crt);
        recList.traverse(getRoD, &l);
        l.removeHead();
        if (l.isEmpty()) continue;
        else {
            L1Item<VRecord> *_pCur = l.getHead();
            L1Item<VRecord> *_pNext = _pCur->pNext;
            double distance = 0.0;
            while (_pNext){
                distance += distanceVR(_pCur->data.y,_pCur->data.x,_pNext->data.y,_pNext->data.x);
                _pCur = _pNext;
                _pNext = _pCur->pNext;
            }
            if (minDistance >= distance) {
                minDistance = distance;
                strcpy(id, crt.id);
            }
        }
    }
    
    cout << id << endl;
    return true;
}

bool RVR(char *cmd, L1List<VRecord> &recList) {
    if (!cmd) {
        return false;
    }

    VRecord r(cmd);
    if (recList.removeAll(r)) {
        cout << "success!" << endl;
    } else {
        NOTFOUND;
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
        case VMTType: {
            string vfx = "VMT";
            strcpy(ret, vfx.data());
            return ret;
        }
        case VFSType: {
            string vfx = "VFS";
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
        case VLSType: {
            string lrv = "VLS";
            strcpy(ret,lrv.data());
            return ret;
        }
        case VMSType: {
            string lrv = "VMS";
            strcpy(ret,lrv.data());
            return ret;
        }
        case MSTType: {
            string lrv = "MST";
            strcpy(ret,lrv.data());
            return ret;
        }
        case CNRType: {
            string lrv = "CNR";
            strcpy(ret,lrv.data());
            return ret;
        }
        case MTVType: {
            string lrv = "MTV";
            strcpy(ret,lrv.data());
            return ret;
        }
        case MVVType: {
            string lrv = "MVV";
            strcpy(ret,lrv.data());
            return ret;
        }
        case CNSType: {
            string lrv = "CNS";
            strcpy(ret,lrv.data());
            return ret;
        }
        case CASType: {
            string lrv = "CAS";
            strcpy(ret,lrv.data());
            return ret;
        }
        case LPVType: {
            string lrv = "LPV";
            strcpy(ret,lrv.data());
            return ret;
        }
        case SPVType: {
            string lrv = "SPV";
            strcpy(ret,lrv.data());
            return ret;
        }
        case RVRType: {
            string lrv = "RVR";
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
    mCMD->registerCommand(getCmdLabel(VMTType), VMT);
    mCMD->registerCommand(getCmdLabel(VFSType), VFS);
    mCMD->registerCommand(getCmdLabel(VASType), VAS);
    mCMD->registerCommand(getCmdLabel(MRVType), MRV);
    mCMD->registerCommand(getCmdLabel(LRVType), LRV);
    mCMD->registerCommand(getCmdLabel(VLSType), VLS);
    mCMD->registerCommand(getCmdLabel(VMSType), VMS);
    mCMD->registerCommand(getCmdLabel(MSTType), MST);
    mCMD->registerCommand(getCmdLabel(CNRType), CNR);
    mCMD->registerCommand(getCmdLabel(MTVType), MTV);
    mCMD->registerCommand(getCmdLabel(MVVType), MVV);
    mCMD->registerCommand(getCmdLabel(CNSType), CNS);
    mCMD->registerCommand(getCmdLabel(CASType), CAS);
    mCMD->registerCommand(getCmdLabel(LPVType), LPV);
    mCMD->registerCommand(getCmdLabel(SPVType), SPV);
    mCMD->registerCommand(getCmdLabel(RVRType), RVR);
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
