//
// Created by px on 7/14/2019.
//

#ifndef REFRESHTIMESTAMP_H
#define REFRESHTIMESTAMP_H

#include "../../common/command.h"

class ViewModel;
class RefreshTimeStamp : public  CommandBase{
private:
    int StampTime;
    ViewModel *PtrViewModel;
public:
    RefreshTimeStamp(ViewModel *ptr);
    ~RefreshTimeStamp()= default;
    virtual void Exec();
    virtual void SetParameters(int stamptime);
    virtual void SetParameters(const std::string str);
};


#endif //REFRESHTIMESTAMP_H
