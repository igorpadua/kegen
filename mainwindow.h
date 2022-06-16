#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void btnVideoClicked();

    void btnLegClicked();

    void btnRenderClicked();

    void btnSaidaClicked();

    void btnCancelarClicked();

    void readyReadStandardOutput();

    void processStarted();

    void encodingFinished();


private:
    Ui::MainWindow *ui;
    QString localfilename;
    QString filename;
    QFile file;
    QProcess processo;
    QString mOutputString;

    QString nomeFinal();
};
#endif // MAINWINDOW_H
