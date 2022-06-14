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
    void on_btnVideo_clicked();

    void on_btnLeg_clicked();

    void on_btnRender_clicked();

    void on_btnSaida_clicked();

    void on_btnCancelar_clicked();

    void processStarted();

    void readyReadStandardOutput();

private:
    Ui::MainWindow *ui;
    QString localfilename;
    QString filename;
    QFile file;
    QProcess *processo;
    QString mOutputString;
};
#endif // MAINWINDOW_H
