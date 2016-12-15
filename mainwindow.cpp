#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    video = new Video();
    ui->setupUi(this);
    move(0,0);
    QPixmap pix("sygnalizacja.png");
    ui->labelImage->setGeometry(ui->labelImageText->x()+0.5*ui->labelImageText->width()-0.5*pix.width(),ui->labelImageText->y()+ui->labelImageText->height()+3,pix.width(),pix.height());
    ui->labelImage->setPixmap(pix);
    ui->sliderVideo->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete video;
    delete ui;
}

void MainWindow::displayNextFrame(Selection lastSelection)
{
    //Wyświetlenie klatki wraz z zastosowaniem template matching gdy na poprzedniej klatce jest zaznaczenie
    if(lastSelection.getFrameNumber()==video->getCurrentFrame()-1 || lastSelection.getFrameNumber()==video->getCurrentFrame())
    {
        video->setPreviousFrame(video->getFrame());
        video->displayFrame();
//    imshow("previousframe",video->getPreviousFrame());
//    imshow("Framethis",video->getFrame());
        if(!video->templateMatching(video->getSelectionVector().back(),video->getPreviousFrame()))
        {
            QMessageBox msg;
            msg.setText("Zaznaczenie jest zbyt blisko krawedzi ekranu");
            msg.exec();

        }
    }
    //Wyświetlenie klatki bez zastosowania template matching gdy na poprzedniej klatce nie ma zaznaczenia
    else
    {
        video->addSelectionToVector(video->getSelection());
        video->setPreviousSelection(video->getSelection());
        video->setPreviousFrame(video->getFrame());
        video->displayFrame();
    }
}

void MainWindow::nextFrame()
{
    if(video->getSelectionVector().size()!=0)
    {
        if(video->selection.getStatus()==2)
        {
            video->addSelectionToVector(video->getSelection());
            displayNextFrame(video->getSelectionVector().back());
        }
        else
        {
            video->displayFrame();
        }
    }
    else
    {
        //Gdy wektor zaznaczeniem jest pusty, ale zaznoczono obszar na ekranie, wykonywany jest template matching dla kolejnej klatki
        if(video->selection.getStatus()==2)
        {
        video->addSelectionToVector(video->getSelection());
        video->setPreviousSelection(video->getSelection());
        video->setPreviousFrame(video->getFrame());
        video->displayFrame();
        video->templateMatching(video->getSelectionVector().back(),video->getPreviousFrame());
        }
        //Zwykłe wyśietlenie kolejnej klatki
        else
        {
            video->displayFrame();
        }
//        QMessageBox msg;
//        msg.setText("Zazanacz interesujący Cię ubszar lub kliknij przycisk Play");
//        msg.exec();
    }
}

void MainWindow::delay(int milliseconds)
{
    QTime dieTime= QTime::currentTime().addMSecs(milliseconds);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void MainWindow::on_buttonOpenVideo_clicked()
{
    if(ui->listWidget->selectedItems().size()!=0)
    {
        if(video->getSelectionVector().size()!=0)
        {
            QMessageBox msgBox;
            msgBox.setText("Czy chcesz zapisać zmiany do pliku?");
            msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
            msgBox.setDefaultButton(QMessageBox::Save);
            int ret = msgBox.exec();

            switch (ret)
            {
            case QMessageBox::Save:
                video->saveToFile();
                video->clearSelectionVector();
                break;
            case QMessageBox::Discard:
                video->clearSelectionVector();
                break;
            default:
                break;
            }
        }
    }
    else
    {
        QMessageBox msg;
        msg.setText("Zaznacz wideo z listy");
        msg.exec();
        return;
    }

    if(!video->openVideo(ui->listWidget->currentItem()->text()))
    {
        QMessageBox msg;
        msg.setText("Nie udalo sie otworzyc pliku");
        msg.exec();
    }
    else
    {

        ui->buttonPause->setEnabled(true);
        ui->buttonPlay->setEnabled(true);
        ui->buttonForward->setEnabled(true);
        ui->buttonBackward->setEnabled(true);
        ui->buttonSaveToFile->setEnabled(true);
        ui->buttonDeleteSelection->setEnabled(true);
        ui->sliderVideo->setMaximum(video->getNumberOfFrames());
        ui->sliderVideo->setValue(video->getCurrentFrame());
        video->play();

    }
}

void MainWindow::on_buttonPlay_clicked()
{

//    if(video->isPaused())
//    {
        video->play();
        ui->sliderVideo->setEnabled(false);
        ui->buttonMatching->setEnabled(false);
        ui->buttonBackward->setEnabled(false);
        ui->buttonForward->setEnabled(false);
//    }


}

void MainWindow::on_buttonAddFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Otworz plik video"), ".",tr("Video Files (*.avi *.mpg *.mp4)"));
    if(!filename.isEmpty())
    {
        ui->listWidget->addItem(filename);
    }
    ui->buttonOpenVideo->setEnabled(true);
//    if(!video->openVideo(filename))
//    {
//        QMessageBox msg;
//        msg.setText("Nie udalo sie otworzyc pliku");
//        msg.exec();
//    }
//    else
//    {
//    video->setFileNameVideo(filename);
//    ui->buttonPause->setEnabled(true);
//    ui->buttonPlay->setEnabled(true);
//    ui->sliderVideo->setMaximum(video->getNumberOfFrames());
//    video->play();
//    }
}

void MainWindow::on_buttonForward_clicked()
{

    nextFrame();
    ui->numberFrame->display(video->getCurrentFrame());

}

void MainWindow::on_buttonPause_clicked()
{
    if(video->isRunning())
        {
            video->pause();
        }
    ui->sliderVideo->setEnabled(true);
    ui->sliderVideo->setValue(video->getCurrentFrame());
    ui->buttonMatching->setEnabled(true);
    ui->buttonBackward->setEnabled(true);
    ui->buttonForward->setEnabled(true);
    matching=false;

}

void MainWindow::on_buttonBackward_clicked()
{
    int crrFrame = video->getCurrentFrame();
    video->setCurrentFrame(crrFrame-2);
    video->displayFrame();
    ui->numberFrame->display(video->getCurrentFrame());

}

void MainWindow::on_buttonRed_clicked()
{
    video->setCurrentColor("Red");
    cout<<"Czerwony klik!"<<endl;
}

void MainWindow::on_buttonYellow_clicked()
{
    video->setCurrentColor("Yellow");
    cout<<"Zolty klik!"<<endl;
}

void MainWindow::on_buttonGreen_clicked()
{
    video->setCurrentColor("Green");
    cout<<"Zielonyklik!"<<endl;
}

void MainWindow::on_buttonRedYellow_clicked()
{
    video->setCurrentColor("RedYellow");
    cout<<"Czerwonozloty klik!"<<endl;
}


void MainWindow::on_buttonSaveToFile_clicked()
{
    video->saveToFile();
    QMessageBox msg;
    msg.setText("Zapisywanie do pliku zakończone!");
    msg.exec();

}

void MainWindow::on_buttonDeleteSelection_clicked()
{
    video->selection.setStatus(0);
}

void MainWindow::on_sliderVideo_valueChanged(int value)
{
    video->setCurrentFrame(value);
    ui->numberFrame->display(video->getCurrentFrame());
}


void MainWindow::on_buttonMatching_clicked()
{
    if(video->getPreviousSelection().getFrameNumber()==video->getSelection().getFrameNumber()-1);
    {
    if(matching==true)
    {
        matching=false;
    }
    else
    {
        matching=true;
    }
    }

    while(matching==true)
    {
        nextFrame();
        delay(50);
        if(!video->checkSelections())
            matching=false;
    }
}
