#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&processo, &QProcess::started, this, &MainWindow::processStarted);
    connect(&processo, &QProcess::readyReadStandardOutput, this, &MainWindow::readyReadStandardOutput);
    connect(&processo, &QProcess::readChannelFinished, this, &MainWindow::encodingFinished);

    connect(ui->btnVideo, &QPushButton::clicked, this, &MainWindow::btnVideoClicked);
    connect(ui->btnLeg, &QPushButton::clicked, this, &MainWindow::btnLegClicked);
    connect(ui->btnSaida, &QPushButton::clicked, this, &MainWindow::btnSaidaClicked);
    connect(ui->btnRender, &QPushButton::clicked, this, &MainWindow::btnRenderClicked);
    connect(ui->btnCancelar, &QPushButton::clicked, this, &MainWindow::btnCancelarClicked);
}

void MainWindow::processStarted() {
    qDebug() << "processStarted()";
}

void MainWindow::readyReadStandardOutput()
{
    qDebug() << "readyReadStandardOutput()";
    mOutputString.append(processo.readAllStandardOutput());
    ui->textBrowser->setText(mOutputString);

    QTextCursor textCursor = ui->textBrowser->textCursor();
    textCursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor,1);
    ui->textBrowser->setTextCursor(textCursor);
}

void MainWindow::encodingFinished() {
    qDebug() << "encodingFinished()";
}

QString MainWindow::nomeFinal() {

    QString arquivoSaida = ui->lineVideo->text();

    if (QString::compare("file://", arquivoSaida.left(6))) {
        arquivoSaida.remove("file://");
    }

    arquivoSaida.insert(arquivoSaida.lastIndexOf('/') + 1, "FINALIZADO_");

    return arquivoSaida;
}

void MainWindow::btnVideoClicked()
{   
    filename = QFileDialog::getOpenFileName(this, tr("Abrir arquivo"), QDir::homePath(), tr("Arquivos de vídeo (*.mp4 *.mkv)"));
    file.setFileName(filename);
    localfilename = filename;

    ui->lineVideo->clear();
    ui->lineVideo->insert(filename);
}


void MainWindow::btnLegClicked()
{
    filename = QFileDialog::getOpenFileName(this, tr("Abrir arquivo"), QDir::homePath(), tr("Arquivos de legenda (*.ass *.srt)"));
    file.setFileName(filename);
    localfilename = filename;

    ui->lineLeg->clear();
    ui->lineLeg->insert(filename);
}

void MainWindow::btnSaidaClicked()
{
    filename = QFileDialog::getSaveFileName(this, tr("Salvar arquivo"), QDir::homePath(), tr("*.mp4(*.mp4)"));
    file.setFileName(filename);
    localfilename = filename;

    ui->lineSaida->clear();
    ui->lineSaida->insert(filename);
}

void MainWindow::btnRenderClicked()
{
    QString video = ui->lineVideo->text();
    QString legenda = ui->lineLeg->text();
    QString arquivoSaida = ui->lineSaida->text();

    if (video == "" && legenda == "")  {
        QMessageBox::warning(this, "Aviso", "Preencha todos os dados!");
        return;
    }

    if (QString::compare("file://", video.left(6))) {
        video.remove("file://");
    }

    if (QString::compare("file://", video.left(6))) {
        legenda.remove("file://");
    }

    if (arquivoSaida.isEmpty()) {
        arquivoSaida = nomeFinal();
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
    const QStringList arguments = {"-i", video, "-vf", legenda, arquivoSaida};

    qDebug() << legenda;
    qDebug() << arguments;

    processo.setProcessChannelMode(QProcess::MergedChannels);
    processo.start(programa, arguments);

}

void MainWindow::btnCancelarClicked()
{   
    processo.kill();

    QString arquivoSaida = ui->lineSaida->text();

    if (arquivoSaida.isEmpty()) {
        arquivoSaida = nomeFinal();
    }

    ui->textBrowser->setText("Cancelado com sucesso!");

    if (!QFile::exists(arquivoSaida)) {
            return;
    }

    QFile::remove(arquivoSaida);

}

