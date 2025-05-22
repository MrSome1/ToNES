#ifndef _TONES_SIMULATOR_H_
#define _TONES_SIMULATOR_H_

#include <array>
#include <thread>

#include <QDialog>
#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QString>
#include <QTextDocument>

#include "ui_mainwindow.h"
#include "ui_ppuviewer.h"
#include "ui_memoryviewer.h"

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
    ~Simulator() override;

    void onVideoDotRendered(int x, int y, uint32_t color) override;

    void onVideoFrameRendered() override;

    void onAudioOutput() override;

    void onRegistersChanged() override;

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

    void setupMainView();

    void setupPpuView();

    void setupMemoryView();

    void changeStatus(Status s = Invalid);

    void showCurrentLine(uint16_t pc);

    void showPrgRom();

    void showChrRom();

    void showRendering();

    void showPalettes();

    void showMemoryMap();

    void drawMemoryMap(QString name);

    void drawMemoryHex(uint8_t *base, int size);

    void drawPatternTable(int base, QImage &img);

    void drawNameTable(const uint8_t *vram, int nbase, int pbase, QImage &img);

private:

    std::thread _thread;

    CartridgePtr _card;

    MotherBoard _engine;

    /* UI classes */

    Ui::MainWindow *_mainWindow;

    QDialog *_ppuViewerDialog;
    Ui::PpuViewer *_ppuViewer;

    QDialog *_memViewerDialog;
    Ui::MemoryViewer *_memViewer;

    /* Video Buffers */

    QImage _videoBuffer;

    QPixmap _videoFrame;

    /* Debuging Info */

    QString _cpuP;

    MicroProcessor::Registers _cpuRegisters;

    QTextDocument *_prom;

    PictureProcessingUnit::Registers _ppuRegisters;

    QImage _limg, _rimg; // pattern tables

    QVector<QLabel*> _colorLables;
    QVector<QLabel*> _paletteLables;
    std::array<uint8_t, ppu::Palettes::PalettesSize> _paletteColors;
};

} // namespace tones

#endif // _TONES_SIMULATOR_H_
