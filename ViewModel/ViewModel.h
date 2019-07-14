#ifndef VIEWMODEL_H
#define VIEWMODEL_H 

#include "../model/Model.h"
#include "../common/command.h"
#include "../common/notification.h"
#include <opencv2/opencv.hpp>

class ViewModel {
private:
    std::shared_ptr <int>VideoFps;
	std::shared_ptr <int>VideoDuration;
	std::shared_ptr<QImage>q_image;
	std::shared_ptr<Model> model;
	std::shared_ptr<cv::Mat> cv_image;
	std::shared_ptr<EditableVideo> testVideo;
	std::shared_ptr<CommandBase> opFileCommand;
    std::shared_ptr<CommandBase> fQimageCommand;
    std::shared_ptr<CommandBase> refreshTimeStampCommand;
//    std::shared_ptr<CommandBase> frameRateCommand;
	std::shared_ptr<Notification> notification;
    std::shared_ptr<Notification> notification_jump;
public:
	ViewModel();
	inline void Bind(std::shared_ptr<Model> &newmodel){
	    this->model = newmodel;
	}

	inline std::shared_ptr<QImage> GetQImage(){
		return q_image;
	}

    inline std::shared_ptr<int> GetVideoFps(){
        return VideoFps;
    }

    inline std::shared_ptr<int> GetVideoDuration(){
		return VideoDuration;
	}

	//command openfile
	inline std::shared_ptr<CommandBase> GetOpenFileCommand(){
        return std::static_pointer_cast<CommandBase>(opFileCommand);
    }
    inline void ExecOpenFileCommand(std::string &path){
		testVideo = model->openFile(path);
        *VideoFps = testVideo->GetFps();
		*VideoDuration = testVideo->GetDuration();
	}

    //command FetcgQimage
    inline std::shared_ptr<CommandBase> GetFetchQimageCommand(){
        return std::static_pointer_cast<CommandBase>(fQimageCommand);
    }
    inline void ExecFetchQimageCommand(){
		cv_image = testVideo->getNextImage();
		cv::Mat cvimage_temp;
		cv::Mat *normalptr = cv_image.get();
		cv::namedWindow("Video", CV_WINDOW_AUTOSIZE);
		cv::imshow("Video", *cv_image);
		cv::cvtColor(*normalptr, cvimage_temp, CV_BGR2RGB);
        QImage qimage_temp((uchar*) cvimage_temp.data, cvimage_temp.cols, cvimage_temp.rows, cvimage_temp.step, QImage::Format_RGB888);
		qimage_temp.bits();
        *q_image = qimage_temp;
		notification->Exec();
	}

	//command RefreshTimeStamp
	inline std::shared_ptr<CommandBase> GetRefreshTimeStampCommand(){
        return std::static_pointer_cast<CommandBase>(refreshTimeStampCommand);
	}
	inline void ExecRefreshTimeStampCommand(int time){
		testVideo->seekImage(time);
        notification_jump->Exec();
	}

	inline void SetUpdateViewNotification(std::shared_ptr<Notification> notification){
		this->notification = notification;
	}

    inline void SetUpdateTimeStampNotification(std::shared_ptr<Notification> notification_jump){
        this->notification_jump = notification_jump;
    }
};

#endif
