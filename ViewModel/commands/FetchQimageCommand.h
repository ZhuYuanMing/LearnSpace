//
// Created by px on 7/13/2019.
//

#ifndef FETCHQIMAGECOMMAND_H
#define FETCHQIMAGECOMMAND_H

#include "../../common/command.h"

class ViewModel;
class FetchQimageCommand : public  CommandBase{
private:
    ViewModel *PtrViewModel;
public:
    FetchQimageCommand(ViewModel *ptr);
    ~FetchQimageCommand()= default;
    virtual void Exec();
    virtual void SetParameters(const std::string path);
    virtual void SetParameters(int val);
};

#endif //FETCHQIMAGECOMMAND_H
