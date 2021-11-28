#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_layout = new QGridLayout(ui->scrollWidget);
    m_path = QDir::homePath();
}

void MainWindow::on_actionOpen_Gif_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open Gif"), m_path, tr("Gif Files (*.gif)"));
    if( !QFile::exists(fileName) )
        return;
    QDir dir;
    m_path = dir.absoluteFilePath (fileName);
    addFrames(fileName);
}


void MainWindow::addFrames(QString imagePath)
{
    QD << ui->scrollWidget->children();
    foreach(QObject * o,ui->scrollWidget->children()){
        if(o->inherits("QLabel")){
            QLabel * del = static_cast<QLabel*>(o);
            delete del;
        }
    }
    m_reader = new QImageReader(imagePath);
    m_frame = new QImage(m_reader->size(),QImage::Format_RGB32);

    int i = 0;
    while(m_reader->read(m_frame)) {
        QLabel * newLbl = new QLabel(ui->scrollWidget);
        m_layout->addWidget(newLbl,i/5,i%5); i++;
        newLbl->setPixmap(QPixmap::fromImage(*m_frame));
        quint32 delay =  m_reader->nextImageDelay();
        convertImage(*m_frame, imagePath, i,delay);
    }
    delete m_reader;
    delete m_frame;
}

void MainWindow::convertImage(QImage image, QString imageFile, quint32 index, quint32 delay){
    m_qbaRawOutput.clear();
    //Over de pixels lopen
    for ( int y = 0; y < image.height(); y++ ){
        for ( int x = 0; x < image.width(); x++ ){
            // ARGB1555
            QRgb simColor = image.pixel(x,y);
            int color = ((qAlpha(simColor)?0x8000:0) | (qRed(simColor) & 0xF8) << 7) | ((qGreen(simColor) & 0xF8) << 2) | ((qBlue(simColor) & 0xF8) >> 3);
            //LittleEndian
            m_qbaRawOutput.append(color & 0x000000FF);
            m_qbaRawOutput.append((color & 0x0000FF00)>>8);
        }
    }
    QString outputFileName = imageFile;
    outputFileName.chop(outputFileName.size()-outputFileName.lastIndexOf("."));
    outputFileName.append(QString("#%1#%2x%3@%4.raw")
                          .arg(index)
                          .arg(image.size().width())
                          .arg(image.size().height())
                          .arg(delay));
    QFile outputFile(outputFileName);
    if ( !outputFile.open(QFile::Truncate|QFile::ReadWrite)){
        QMessageBox::warning(this, tr("Oops"),
                   tr("An error occured whilst trying to save the raw file"),
                                QMessageBox::Ok ,
                                QMessageBox::Ok);
        return;
    }
    outputFile.write(m_qbaRawOutput);
    outputFile.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
