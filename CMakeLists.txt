cmake_minimum_required(VERSION 3.12)
project(MiniPreimere)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOrCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


if(WIN32)
    include_directories(C:/CodeLibrary/opencv-3.4.6/build/install/include)
    include_directories(C:/CodeLibrary/ffmpeg-4.1.3-win64-dev/include)
    link_directories(C:/CodeLibrary/ffmpeg-4.1.3-win64-dev/lib)
    set(CMAKE_PREFIX_PATH C:/Qt/5.13.0/mingw73_64/lib/cmake  C:/CodeLibrary/opencv-3.4.6/build/install)
endif(WIN32)

if(UNIX)
    set(CMAKE_PREFIX_PATH /home/root/Qt/5.13.0/gcc_64/lib/cmake)
endif(UNIX)

# find and set Qt
find_package(Qt5Gui REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

# find and set openCV
find_package(OpenCV)
set(OpenCV_LIBS opencv_core  opencv_highgui opencv_imgproc)
# set ffmpeg

set(ffmpeg_LIBS avcodec avformat avutil avdevice swscale)


set(SOURCE_FILES ./App/app.cpp ./App/main.cpp ./View/MainWindow.cpp ./View/Notification/UpdateViewNotification.cpp  ./View/Notification/UpdateTimeStampNotification.cpp ./View/MySlider.cpp ./ViewModel/commands/FetchQimageCommand.cpp ./ViewModel/commands/OpenFileCommand.cpp ./ViewModel/commands/RefreshTimeStamp.cpp ./ViewModel/ViewModel.cpp  ./model/Model.cpp  ./model/EditableVideo.cpp)
#link

add_executable(MiniPremiere ${SOURCE_FILES})
target_link_libraries(MiniPremiere ${OpenCV_LIBS} avcodec avdevice avfilter avformat swresample avutil postproc swresample swscale Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Multimedia)
