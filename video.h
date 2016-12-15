#ifndef VIDEO_H
#define VIDEO_H

#include "opencv/cv.h"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include "time.h"
#include <iostream>
#include <QString>
#include <QThread>
#include <QMutex>
#include <QImage>
#include <QWaitCondition>
#include "selection.h"
#include <set>
#include <map>

using namespace std;
using namespace cv;

class Video : public QThread
{
    Q_OBJECT

private:
    // Zmienna do nagrywania
    VideoCapture *capture;
    Selection  previousSelection;
    vector<Selection> selectionVector;
    bool paused;
    QWaitCondition condition;
    Mat frame;
    Mat previousFrame;
    QMutex mutex;
    Mat RGBframe;
    Mat mask;
    Mat mask1,mask2;
    Mat output;
    QImage image;
    int frameRate;
    string currentColor;
    QString fileNameVideo;

protected:
    void run();
    void sleep(int ms);
public:
    Video(QObject *parent=0);
    ~Video();
    Selection selection, selectionHelper;

    static void mouseCallback(int event, int x, int y, int flags, void *userdata);

    void mouseCallbackHelper(int event, int x, int y);

    static Scalar colorScalar(string name);

    bool openVideo(QString filename);
    void play();
    void pause();
    bool isPaused() const;
    void displayFrame();
    void displayFrameToSelect();
    bool templateMatching(Selection lastSelection, Mat lastFrame);
    void saveToFile();
    void readFile();
    QString getShortFileName(QString name);
    bool checkSelections();
    void clearSelectionVector();


    int getCurrentFrame();
    int getNumberOfFrames();
    void setCurrentFrame(int value);
    string getCurrentColor() const;
    void setCurrentColor(const string &value);
    vector<Selection> getSelectionVector() const;
    void setSelectionVector(const vector<Selection> &value);
    Selection getSelection() const;
    void setSelection(Selection value);
    void addSelectionToVector(Selection selectionToAdd);
    Selection getPreviousSelection() const;
    void setPreviousSelection(Selection value);
    Mat getFrame();
    void setFrame(Mat value);
    Mat getPreviousFrame();
    void setPreviousFrame(Mat value);
    QString getFileNameVideo() const;
    void setFileNameVideo(const QString &value);
};

#endif // VIDEO_H
