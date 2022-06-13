#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Criando o processo
    processo = new QProcess(this);

    connect(processo, SIGNAL(started()), this, SLOT(processStarted()));
    connect(processo, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
    connect(processo, SIGNAL(finished(int)), this, SLOT(encodingFinished()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processStarted() {
    qDebug() << "processStarted()";
}

void MainWindow::readyReadStandardOutput()
{
    qDebug() << "readyReadStandardOutput()";
    mOutputString.append(processo->readAllStandardOutput());
    ui->textBrowser->setText(mOutputString);

    QTextCursor textCursor = ui->textBrowser->textCursor();
    textCursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
    ui->textBrowser->setTextCursor(textCursor);
}

void MainWindow::on_btnVideo_clicked()
{   
    filename = QFileDialog::getOpenFileName(this, "Abrir", QDir::homePath(), "Arquivos de vídeo (.mp4)");
    file.setFileName(filename);
    localfilename = filename;
    ui->lineVideo->clear();
    ui->lineVideo->insert(filename);
}


void MainWindow::on_btnLeg_clicked()
{
    filename = QFileDialog::getOpenFileName(this, "Abrir", QDir::homePath(), "Arquivos de legenda (.ass)");
    file.setFileName(filename);
    localfilename = filename;

    ui->lineLeg->clear();
    ui->lineLeg->insert(filename);
}


void MainWindow::on_btnRender_clicked()
{
    QString video = ui->lineVideo->text();
    QString legenda = ui->lineLeg->text();
    QString nomeFinal = ui->lineSaida->text();


    if (video != "" && legenda != "")  {

        if (QString::compare("file://", video.left(6))) {
            video.remove("file://");
        }

        if (QString::compare("file://", video.left(6))) {
            legenda.remove("file://");
        }

        if (nomeFinal.isEmpty()) {
            nomeFinal = video;
            nomeFinal.insert(nomeFinal.lastIndexOf('/') + 1, "FINALIZADO_");
        }

        QString programa = "ffmpeg";
        QStringList arguments;


        if (legenda.right(3) == "srt") {
            legenda.insert(0, "subtitles=");
            arguments <<  "-i" << video << "-vf" << legenda << nomeFinal;
        } else {
            legenda.insert(0, "ass=");
            arguments << "-vaapi_device" << "/dev/dri/renderD128" <<  "-i" << video << "-vf" << legenda << "-vf" << "format=nv12,hwupload" << "-c:v" << "hevc_vaapi" << "-f" << "mp4" << "-rc_mode" << "1" << "-qp" << "25" << nomeFinal;
        }

        qDebug() << legenda;

        qDebug() << arguments;

        processo->setProcessChannelMode(QProcess::MergedChannels);
        processo->start(programa, arguments);
    } else {
        QMessageBox::warning(this, "Aviso", "Preencha todos os dados!");
    }

}


void MainWindow::on_btnSaida_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Salvar", QDir::homePath(), "Arquivo de vídeo (.mp4)");
    QFile file(filename);
    localfilename = filename;

    ui->lineSaida->clear();
    ui->lineSaida->insert(filename);
}

void MainWindow::on_btnCancelar_clicked()
{

    processo->kill();

    QString nomeFinal = ui->lineSaida->text();

    if (nomeFinal.isEmpty()) {
        QString video = ui->lineVideo->text();

        if (QString::compare("file://", video.left(6))) {
            video.remove("file://");
        }

        nomeFinal = video;
        nomeFinal.insert(nomeFinal.lastIndexOf('/') + 1, "FINALIZADO_");
    }
    QString remover = "rm \"" + nomeFinal + "\"";
    system(qPrintable(remover));
    ui->textBrowser->setText("Cancelado com sucesso!");
}

