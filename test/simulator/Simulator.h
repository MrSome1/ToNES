#ifndef _TONES_SIMULATOR_H_
#define _TONES_SIMULATOR_H_

#include <thread>

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QString>
#include <QTextDocument>

#include "ui_mainwindow.h"
#include "MotherBoard.h"

namespace tones {

class Simulator : public QMainWindow, public OutputPanel
{
    Q_OBJECT

public:

    enum Status {
        Started,
        Paused,
        Resumed,
        Stopped,
        Invalid
    };

    Simulator(QWidget *parent=nullptr);
    ~Simulator();

    void onVideoDotRendered(int x, int y, uint32_t color) override;

    void onVideoFrameRendered() override;

    void onAudioOutput() override;

    void onCpuStepped(const MicroProcessor::Registers_t &regs) override;

Q_SIGNALS:

    void showFrame();

    void showCartridge();

public Q_SLOTS:

    void onOpen();

    void onStart();

    void onStop();

    void onPause();

    void onStep();

    void onReset();

    void onShowFrame();

    void onShowCartridge();

protected:

    void start();

    void stop();

    void setupView();

    void setupConnections();

    void changeStatus(Status s = Invalid);

    void showCurrentLine(uint16_t pc);

    void showPrgRom();

    void showChrRom();

    void drawPatternTable(int base, QImage &img);

private:

    Ui::MainWindow *_ui;

    std::thread _thread;

    CartridgePtr _card;

    MotherBoard _engine;

    /* Video Buffers */

    QImage _videoBuffer;

    QPixmap _videoFrame;

    /* Debuging Info */

    QString _cpuP;

    QTextDocument *_prom;

    QImage _limg, _rimg; // pattern tables
};

} // namespace tones

#endif // _TONES_SIMULATOR_H_
