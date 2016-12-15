#include "video.h"

int Video::getCurrentFrame()
{
    return capture->get(CV_CAP_PROP_POS_FRAMES);
}

int Video::getNumberOfFrames()
{
    return capture->get(CV_CAP_PROP_FRAME_COUNT);
}

void Video::setCurrentFrame(int value)
{
    capture->set(CV_CAP_PROP_POS_FRAMES,value);
}

void Video::mouseCallbackHelper(int event, int x, int y)
{
    Point currentPoint = Point(x,y);

    if(event == EVENT_LBUTTONDOWN)
    {
        selection.setLpm(currentPoint);
        selection.setStatus(1);
        cout<<"Wcisniety przycisk"<<endl;
    }

    else if(event == EVENT_LBUTTONUP)
    {
        selection.setStatus(2);
        selection.setSelectionStable(selection.getSelectionTemporary());
        cout<<"Przycisk zwolniony"<<endl;
    }
    selection.setCornerLeft(Point(min(selection.getLpm().x,currentPoint.x),min(selection.getLpm().y,currentPoint.y)));
    selection.setCornerRight(Point(max(selection.getLpm().x,currentPoint.x),max(selection.getLpm().y,currentPoint.y)));

    selection.setSelectionTemporary(Rect(selection.getCornerLeft().x,selection.getCornerLeft().y,selection.getCornerRight().x-selection.getCornerLeft().x,2*(selection.getCornerRight().x-selection.getCornerLeft().x)));
    return;
}

Scalar Video::colorScalar(string name)
{
    Scalar color;
if(name=="Red")
{
        color = Scalar(1,1,255);
}
else if(name=="Green")
{
        color = Scalar(1,255,1);
}
else if(name=="Yellow")
{
        color = Scalar(1,200,200);
}
else if(name=="RedYellow")
{
        color = Scalar(1,200,200);
}
else
        cout<<"Blad";

    return color;

}

string Video::getCurrentColor() const
{
    return currentColor;
}

void Video::setCurrentColor(const string &value)
{
    currentColor = value;
    selection.setColor(value);
}

vector<Selection> Video::getSelectionVector() const
{
    return selectionVector;
}

void Video::setSelectionVector(const vector<Selection> &value)
{
    selectionVector = value;
}

Selection Video::getSelection() const
{
    return selection;
}

void Video::setSelection(Selection value)
{
    selection = value;
}

void Video::addSelectionToVector(Selection selectionToAdd)
{
    selectionVector.push_back(selectionToAdd);
}

Selection Video::getPreviousSelection() const
{
    return previousSelection;
}

void Video::setPreviousSelection(Selection value)
{
    previousSelection = value;
}

Mat Video::getFrame()
{
    return frame;
}

void Video::setFrame(Mat value)
{
    frame = value;
}

Mat Video::getPreviousFrame()
{
    return previousFrame;
}

void Video::setPreviousFrame(Mat value)
{
    value.copyTo(previousFrame);
}

QString Video::getFileNameVideo() const
{
    return fileNameVideo;
}

void Video::setFileNameVideo(const QString &value)
{
    fileNameVideo = value;
}

void Video::run()
{
    int delay =1000/frameRate;
    
    while(!paused)
    {
        displayFrame();
        this->sleep(delay);
        
    }
    while(paused)
    {
        displayFrameToSelect();
        this->sleep(delay);
    }
}

void Video::sleep(int ms)
{
    struct timespec t = {ms/1000, (ms%1000)*1000000};
    nanosleep(&t,NULL);
}

Video::Video(QObject *parent)
    :QThread(parent)
{

    Selection previousSelection;
    paused=true;
}

Video::~Video()
{
    mutex.lock();
    paused = true;
    capture->release();
    destroyAllWindows();
    delete capture;
    condition.wakeOne();
    mutex.unlock();
    wait();
}

void Video::mouseCallback(int event, int x, int y, int flags, void *userdata)
{
    Video *self = static_cast<Video*>(userdata);
    self->mouseCallbackHelper(event, x, y);
}


void Video::play()
{

    if(isPaused())
    {
        paused = false;
    }
    start(LowPriority);
}

void Video::pause()
{
    if(!isPaused())
    {
    capture->read(frame);
    }
    paused=true;
    if(!isRunning())
    {
        start(LowPriority);
    }
}

bool Video::isPaused() const
{

    return this->paused;
}

void Video::displayFrame()
{
    if(!capture->read(frame))
    {
        paused=true;
    }
    else
    {
        output=frame.clone();
        mask2 = Mat(frame.rows,frame.cols,CV_8UC1);
        for(auto &elem : selectionVector)
        {
            if(elem.getFrameNumber()== getCurrentFrame())
            {
                rectangle(mask2,elem.getSelectionStable(),colorScalar(elem.getColor()),-1);
                add(output,colorScalar(elem.getColor()),output,mask2);
                if(elem.getColor()=="RedYellow")
                {
                    rectangle(mask1,elem.getSelectionStable(),Scalar(1,1,255),2);
                    output.setTo(Scalar(1,1,255), mask1 );
                }
            }
        }
        imshow("Filmik",output);
    }
}

void Video::displayFrameToSelect()
{

    int delay =1000/frameRate;

    mask = Mat(frame.rows,frame.cols,CV_8UC1);
    mask1 = Mat(frame.rows,frame.cols,CV_8UC1);
    mask2 = Mat(frame.rows,frame.cols,CV_8UC1);
    if(selection.getStatus()!=0)
    {
    while(paused==true)
    {
        output=frame.clone();
        mask.setTo(0);
        mask1.setTo(0);
        mask2.setTo(0);


        if(selection.getStatus()==1){
            rectangle(mask, selection.getSelectionTemporary(),colorScalar(this->getCurrentColor()), 1 );
            output.setTo( colorScalar(this->getCurrentColor()), mask );
        }
        else if(selection.getStatus()==2)
        {

            rectangle(mask,selection.getSelectionStable(),colorScalar(this->getCurrentColor()),-1);
            add(output,colorScalar(this->getCurrentColor()),output,mask);
            if(this->getCurrentColor()=="RedYellow")
            {
                rectangle(mask1,selection.getSelectionStable(),Scalar(1,1,255),2);
                output.setTo(Scalar(1,1,255), mask1 );
            }
            selection.setFrameNumber(this->getCurrentFrame());
        }

        for(auto &elem : selectionVector)
        {
            if(elem.getFrameNumber()== getCurrentFrame())
            {
                rectangle(mask2,elem.getSelectionStable(),colorScalar(elem.getColor()),-1);
                add(output,colorScalar(elem.getColor()),output,mask2);
                if(elem.getColor()=="RedYellow")
                {
                    rectangle(mask1,selection.getSelectionStable(),Scalar(1,1,255),2);
                    output.setTo(Scalar(1,1,255), mask1 );
                }
            }
        }
        imshow("Filmik",output);
        this->sleep(delay);
    }
    }

}

bool Video::templateMatching(Selection lastSelection, Mat lastFrame)
{
    Mat imageToTemplateMatching = frame;
    int match_method = CV_TM_SQDIFF_NORMED;
    Rect rectToTemplateMatching = lastSelection.getSelectionStable();
    rectToTemplateMatching-=Point(20,40);
    rectToTemplateMatching+=Size(40,80);
    if(rectToTemplateMatching.x<=0||rectToTemplateMatching.y<=0)
    {
        return false;
    }
    imageToTemplateMatching = imageToTemplateMatching(rectToTemplateMatching);
//    frame.copyTo(imageToTemplateMatching);

    Mat lastFrameCopy;
    lastFrame.copyTo(lastFrameCopy);
    Mat template1 = lastFrameCopy(lastSelection.getSelectionStable());
//    Mat templateBigger,templateSmaller;
//    resize(template1,templateBigger,Size(),1.05,1.05);
//    resize(template1,templateSmaller,Size(),0.95,0.95);

//    Mat resultSmaller,resultBigger;
    Mat result;
    /// Create the result matrix
    int result_cols =  imageToTemplateMatching.cols - template1.cols + 1;
    int result_rows = imageToTemplateMatching.rows - template1.rows + 1;

    result.create( result_cols, result_rows, CV_32FC1 );
//    result.copyTo(resultBigger);
//    result.copyTo(resultSmaller);

    /// Do the Matching and Normalize
    matchTemplate( imageToTemplateMatching, template1, result, match_method );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
//    matchTemplate( imageToTemplateMatching, templateBigger, resultBigger, match_method );
//    normalize( resultBigger, resultBigger, 0, 1, NORM_MINMAX, -1, Mat() );
//    matchTemplate( imageToTemplateMatching, templateSmaller, resultSmaller, match_method );
//    normalize( resultSmaller, resultSmaller, 0, 1, NORM_MINMAX, -1, Mat() );

    /// Localizing the best match with minMaxLoc
    double minVal; double maxVal; Point minLoc; Point maxLoc;
//    double minValBig; double maxValBig; Point minLocBig; Point maxLocBig;
//    double minValSmall; double maxValSmall; Point minLocSmall; Point maxLocSmall;
    Point matchLoc;

    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
//    minMaxLoc( resultBigger, &minValBig, &maxValBig, &minLocBig, &maxLocBig, Mat() );
//    minMaxLoc( resultSmaller, &minValSmall, &maxValSmall, &minLocSmall, &maxLocSmall, Mat() );

//    map<double,Point> minimumValues;
//    map<double,Point> maximumValues;
//    minimumValues[minVal]= minLoc;
//    minimumValues[minValBig]=minLocBig;
//    minimumValues[minValSmall]=minLocSmall;
//    maximumValues[maxVal]=maxLoc;
//    maximumValues[maxValBig]=maxLocBig;
//    maximumValues[maxValSmall]=maxLocSmall;


    /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
    {
//        auto it1= minimumValues.begin();
//        Point minimum = it1->second;
//        matchLoc = minimum;
        matchLoc=minLoc;
    }
    else
    {
//        auto it2= prev(maximumValues.end());
//        Point maximum = it2->second;
//        matchLoc = maximum;
        matchLoc=maxLoc;
    }

    matchLoc.x+=rectToTemplateMatching.x;
    matchLoc.y+=rectToTemplateMatching.y;
    selection.setCornerLeft(matchLoc);
    selection.setCornerRight(Point( matchLoc.x + template1.cols , matchLoc.y + template1.rows ));
    selection.setColor(currentColor);
    selection.setFrameNumber(getCurrentFrame());
    selection.setSelectionStable(Rect(selection.getCornerLeft(),selection.getCornerRight()));
    setPreviousSelection(selection);
//    rectangle( imageToTemplateMatching, matchLoc, Point( matchLoc.x + template1.cols , matchLoc.y + template1.rows ), Scalar::all(0), 2, 8, 0 );
    rectangle( result, matchLoc, Point( matchLoc.x + template1.cols , matchLoc.y + template1.rows ), Scalar::all(0), 2, 8, 0 );
//    imshow("Result",result);
//    imshow("imagetotempla",imageToTemplateMatching);
    return true;
}

void Video::saveToFile()
{
    QString filenameStorage = getShortFileName(getFileNameVideo());
    filenameStorage.append(".yml");
    FileStorage fs(filenameStorage.toStdString(),FileStorage::WRITE);
    Selection slc;
    filenameStorage.chop(4);
    fs<<filenameStorage.toStdString()<<"[";
    for(int i=0;i<selectionVector.size();++i)
    {
        slc=selectionVector.at(i);
        cout<<slc.getFrameNumber()<<": "<<slc.getColor()<<' '<<slc.getSelectionStable().x<<endl;
        fs<<"{:"<<"Frame number"<<slc.getFrameNumber()<<"Color"<<slc.getColor()<<"x"<<slc.getSelectionStable().x<<"y"<<slc.getSelectionStable().y<<"width"<<slc.getSelectionStable().width<<"}";
    }
    fs<<"]";
    fs.release();
    cout<<"Zapisywanie udane"<<endl;


}

void Video::readFile()
{
    QString fileName = getFileNameVideo();
    fileName = getShortFileName(fileName);
    fileName.append(".yml");
    FileStorage fs2(fileName.toStdString(), FileStorage::READ);
    if(!fs2.isOpened())
    {
        cout<<"Nie udalo sie otworzyc pliku";
    }
    else
    {
    fileName.chop(4);
    FileNode videoData=fs2[fileName.toStdString()];
    FileNodeIterator it = videoData.begin();
    FileNodeIterator it_end=videoData.end();
    for(;it!=it_end;++it)
    {
        Point cL,cP;
        cL.x=(int)(*it)["x"];
        cL.y=(int)(*it)["y"];
        int width=(int)(*it)["width"];
        cP.x=cL.x+width;
        cP.y=cL.y+2*width;
        string colour=(string)(*it)["Color"];
        int frameNumber = (int)(*it)["Frame number"];
        Selection selectionFromFile(cL,cP,colour,frameNumber);
        addSelectionToVector(selectionFromFile);
    }
    setCurrentFrame((int)(*(--it_end))["Frame number"]);
    }

}

QString Video::getShortFileName(QString name)
{
    QString filenameStorage = name;
    QStringList StringList = filenameStorage.split('/');
    filenameStorage=StringList.last();
    filenameStorage.chop(4);
    return filenameStorage;
}

bool Video::checkSelections()
{
    Point prevL=previousSelection.getCornerLeft();
    Point curL=selection.getCornerLeft();
    int pixels=3;

    if(prevL.x<curL.x-pixels||prevL.x>curL.x+pixels||prevL.y<curL.y-pixels||prevL.y>curL.y+pixels)
        return false;
    else
        return true;

}

void Video::clearSelectionVector()
{
    selectionVector.clear();
}



bool Video::openVideo(QString filename)
{
    string filename1 = filename.toStdString();
    capture = new VideoCapture(filename1);
    if(capture->isOpened())
    {
        namedWindow("Filmik");
        setMouseCallback("Filmik",mouseCallback,this);
        frameRate =(int) capture->get(CV_CAP_PROP_FPS);
        setFileNameVideo(filename);
        readFile();
        return true;
    }
    else
        return false;
}
