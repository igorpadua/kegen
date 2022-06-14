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

    if (video != " " && legenda != " ")  {
        QMessageBox::warning(this, "Aviso", "Preencha todos os dados!");
        return;
    }

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

    if (legenda.right(4) == ".srt") {
        legenda.insert(0, "subtitles=");
    } else if (legenda.right(4) == ".ass") {
        legenda.insert(0, "ass=");
    } else {
        QMessageBox::warning(this, "Aviso", "Formato de legenda não suportado!");
        return;
    }

    const QString programa = "ffmpeg";
    const QStringList arguments = {"-i", video, "-vf", legenda, nomeFinal};

    qDebug() << legenda;
    qDebug() << arguments;

    processo->setProcessChannelMode(QProcess::MergedChannels);
    processo->start(programa, arguments);

}


void MainWindow::on_btnSaida_clicked()
{
    const QString filename = QFileDialog::getSaveFileName(this, "Salvar", QDir::homePath(), "Arquivo de vídeo (.mp4)");
    const QFile file(filename);
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

