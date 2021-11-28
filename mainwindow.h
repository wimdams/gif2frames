#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#define QD qDebug() << __FILE__ << __LINE__
#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QImageReader>
#include <QLabel>
#include <QMessageBox>
#include <QGridLayout>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addFrames(QString imagePath);
    void convertImage(QImage image, QString path, quint32 i, quint32 delay);

private slots:
    void on_actionOpen_Gif_triggered();

private:
    QByteArray m_qbaRawOutput;
    Ui::MainWindow *ui;
    QImageReader *m_reader;
    QImage * m_frame;
    QString m_path;
    QGridLayout *m_layout;
};
#endif // MAINWINDOW_H
