//
// Created by px on 7/14/2019.
//

#include "./RefreshTimeStamp.h"
#include "../ViewModel.h"

RefreshTimeStamp::RefreshTimeStamp(ViewModel *ptr) {
    this->PtrViewModel = ptr;
}
void RefreshTimeStamp::SetParameters(int stamptime) {
    StampTime = stamptime;
}
void RefreshTimeStamp::SetParameters(const std::string str){

}
void RefreshTimeStamp::Exec() {
    PtrViewModel->ExecRefreshTimeStampCommand(StampTime);
}
