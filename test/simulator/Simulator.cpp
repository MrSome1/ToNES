
#include "Simulator.h"

#include <QVector>
#include <QFileDialog>
#include <QTextBlock>
#include <QTextCursor>

#include "Device.h"
#include "Cartridge.h"
#include "Instruction.h"
#include "Log.h"

namespace tones {

const char *Pause  = "Pause";
const char *Resume = "Resume";

const char *CpuPHeader = "N    V    -    B    D    I    Z    C";
const char *CpuPValue  = "0    0    0    0    0    0    0    0";
const int CpuPSepLen = 5;

static const QVector<QRgb> Colors = {0xff000000, 0xff0000ff, 0xff00ff00, 0xffff0000};

const QLatin1Char HexPrefix('0');

inline QString toHexString(uint8_t byte)
{
    return QString("%1").arg(byte, 2, 16, HexPrefix);
}

inline QString toHexString(uint16_t num)
{
    return QString("%1").arg(num, 4, 16, HexPrefix);
}

Simulator::Simulator(QWidget *parent)
    : QMainWindow(parent)
    , _mainWindow(new Ui::MainWindow)
    , _ppuViewerDialog(new QDialog)
    , _ppuViewer(new Ui::PpuViewer)
    , _videoBuffer(360, 320, QImage::Format_RGB888)
    , _cpuP(CpuPValue)
    , _prom(nullptr)
    , _limg(128, 128, QImage::Format_Indexed8)
    , _rimg(128, 128, QImage::Format_Indexed8)
{
    setupView();
    setupConnections();
    changeStatus();

    _engine.setOutputPanel(*this);
}

Simulator::~Simulator()
{
    onStop();
    delete _mainWindow;
    delete _ppuViewer;
    delete _ppuViewerDialog;
}

void Simulator::setupView()
{
    _mainWindow->setupUi(this);
    _ppuViewer->setupUi(_ppuViewerDialog);

    // TODO: Load logo
    // _videoBuffer.load("/home/li/Pictures/fengling.jpeg")
    _videoFrame.convertFromImage(_videoBuffer);
    _mainWindow->screen->setPixmap(_videoFrame);

    _mainWindow->cpuPHeader->setText(CpuPHeader);
    _prom = _mainWindow->programRom->document();

    _limg.setColorTable(Colors);
    _rimg.setColorTable(Colors);

    _paletteLables = {
        _mainWindow->paletteB_0_0, _mainWindow->paletteB_0_1,  _mainWindow->paletteB_0_2,  _mainWindow->paletteB_0_3,
        _mainWindow->paletteB_1_0, _mainWindow->paletteB_1_1,  _mainWindow->paletteB_1_2,  _mainWindow->paletteB_1_3,
        _mainWindow->paletteB_2_0, _mainWindow->paletteB_2_1,  _mainWindow->paletteB_2_2,  _mainWindow->paletteB_2_3,
        _mainWindow->paletteB_3_0, _mainWindow->paletteB_3_1,  _mainWindow->paletteB_3_2,  _mainWindow->paletteB_3_3,
        _mainWindow->paletteS_0_0, _mainWindow->paletteS_0_1,  _mainWindow->paletteS_0_2,  _mainWindow->paletteS_0_3,
        _mainWindow->paletteS_1_0, _mainWindow->paletteS_1_1,  _mainWindow->paletteS_1_2,  _mainWindow->paletteS_1_3,
        _mainWindow->paletteS_2_0, _mainWindow->paletteS_2_1,  _mainWindow->paletteS_2_2,  _mainWindow->paletteS_2_3,
        _mainWindow->paletteS_3_0, _mainWindow->paletteS_3_1,  _mainWindow->paletteS_3_2,  _mainWindow->paletteS_3_3,
    };

    setupAllColors();
}

void Simulator::setupAllColors()
{
    _colorLables = {
        _ppuViewer->color_00, _ppuViewer->color_01, _ppuViewer->color_02, _ppuViewer->color_03,
        _ppuViewer->color_04, _ppuViewer->color_05, _ppuViewer->color_06, _ppuViewer->color_07,
        _ppuViewer->color_08, _ppuViewer->color_09, _ppuViewer->color_0A, _ppuViewer->color_0B,
        _ppuViewer->color_0C, _ppuViewer->color_0D, _ppuViewer->color_0E, _ppuViewer->color_0F,

        _ppuViewer->color_10, _ppuViewer->color_11, _ppuViewer->color_12, _ppuViewer->color_13,
        _ppuViewer->color_14, _ppuViewer->color_15, _ppuViewer->color_16, _ppuViewer->color_17,
        _ppuViewer->color_18, _ppuViewer->color_19, _ppuViewer->color_1A, _ppuViewer->color_1B,
        _ppuViewer->color_1C, _ppuViewer->color_1D, _ppuViewer->color_1E, _ppuViewer->color_1F,

        _ppuViewer->color_20, _ppuViewer->color_21, _ppuViewer->color_22, _ppuViewer->color_23,
        _ppuViewer->color_24, _ppuViewer->color_25, _ppuViewer->color_26, _ppuViewer->color_27,
        _ppuViewer->color_28, _ppuViewer->color_29, _ppuViewer->color_2A, _ppuViewer->color_2B,
        _ppuViewer->color_2C, _ppuViewer->color_2D, _ppuViewer->color_2E, _ppuViewer->color_2F,

        _ppuViewer->color_30, _ppuViewer->color_31, _ppuViewer->color_32, _ppuViewer->color_33,
        _ppuViewer->color_34, _ppuViewer->color_35, _ppuViewer->color_36, _ppuViewer->color_37,
        _ppuViewer->color_38, _ppuViewer->color_39, _ppuViewer->color_3A, _ppuViewer->color_3B,
        _ppuViewer->color_3C, _ppuViewer->color_3D, _ppuViewer->color_3E, _ppuViewer->color_3F,
    };

    for (size_t i = 0; i < 64; ++i) {
        QPalette p;
        p.setColor(QPalette::Background, 0xff000000 | ppu::Colors[i]);
        _colorLables[i]->setPalette(p);
    }
}

void Simulator::setupConnections()
{
    // Actions
    connect(_mainWindow->actionOpen, &QAction::triggered, this, &Simulator::onOpen);
    connect(_mainWindow->actionPpu, &QAction::triggered, _ppuViewerDialog, &QDialog::open);

    // Buttons
    connect(_mainWindow->startButton, &QPushButton::clicked, this, &Simulator::onStart);
    connect(_mainWindow->stopButton, &QPushButton::clicked, this, &Simulator::onStop);
    connect(_mainWindow->pauseButton, &QPushButton::clicked, this, &Simulator::onPause);
    connect(_mainWindow->stepButton, &QPushButton::clicked, this, &Simulator::onStep);
    connect(_mainWindow->resetButton, &QPushButton::clicked, this, &Simulator::onReset);

    connect(this, &Simulator::showFrame, this, &Simulator::onShowFrame);
    connect(this, &Simulator::showCartridge, this, &Simulator::onShowCartridge);
}

void Simulator::changeStatus(Status s)
{
    switch (s) {
    case Started:
        _mainWindow->startButton->setEnabled(false);
        _mainWindow->pauseButton->setEnabled(true);
        _mainWindow->stopButton->setEnabled(true);
        _mainWindow->stepButton->setEnabled(false);
        _mainWindow->resetButton->setEnabled(false);
        break;
    case Stopped:
        _mainWindow->startButton->setEnabled(true);
        _mainWindow->pauseButton->setEnabled(false);
        _mainWindow->stopButton->setEnabled(false);
        _mainWindow->stepButton->setEnabled(true);
        _mainWindow->resetButton->setEnabled(true);
        _mainWindow->pauseButton->setText(Pause);
        break;
    case Paused:
        _mainWindow->startButton->setEnabled(false);
        _mainWindow->pauseButton->setEnabled(true);
        _mainWindow->stopButton->setEnabled(true);
        _mainWindow->stepButton->setEnabled(true);
        _mainWindow->resetButton->setEnabled(true);
        _mainWindow->pauseButton->setText(Resume);
        break;
    case Resumed:
        _mainWindow->startButton->setEnabled(false);
        _mainWindow->pauseButton->setEnabled(true);
        _mainWindow->stopButton->setEnabled(true);
        _mainWindow->resetButton->setEnabled(false);
        _mainWindow->resetButton->setEnabled(false);
        _mainWindow->pauseButton->setText(Pause);
        break;
    default:
        _mainWindow->startButton->setEnabled(false);
        _mainWindow->pauseButton->setEnabled(false);
        _mainWindow->stopButton->setEnabled(false);
        _mainWindow->stepButton->setEnabled(false);
        _mainWindow->resetButton->setEnabled(false);
        break;
    }
}

void Simulator::onOpen()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select Rom:"),
                                                    "", tr("*.nes")); // "Roms (*.nes)"
    if (filename.isEmpty())
        return;

    _card = CartridgeFactory::createCartridge(filename.toStdString());

    changeStatus(Stopped);
    _mainWindow->statusBar->showMessage(filename);
    emit showCartridge();

    _engine.insert(_card);
}

void Simulator::onStart()
{
    start();
    changeStatus(Started);
}

void Simulator::onStop()
{
    stop();
    changeStatus(Stopped);
}

void Simulator::onPause()
{
    if (_engine.isRunning()) {
        _engine.pause();
        changeStatus(Paused);
    } else {
        _engine.resume();
        changeStatus(Resumed);
    }
}

void Simulator::onStep()
{
    _engine.step();
}

void Simulator::onReset()
{
    _engine.reset();
}

void Simulator::onShowFrame()
{
    _mainWindow->screen->setPixmap(_videoFrame);
}

void Simulator::onShowCartridge()
{
    showPrgRom();
    showChrRom();
}

void Simulator::showPrgRom()
{
    QString prom;

    int argv = 0;
    int addr = ReadOnlyMemory::RomLowerBankBase;

    for (auto byte : _card->prgRom()) {
        auto line = QString("%1 |  %2\n").arg(addr, 4, 16, HexPrefix);
        if (!argv) {
            auto *op = cpu::OperationSet[byte];
            argv = op->mode->operands;
            prom.append(line.arg(QString("%1  # %2, %3").arg(byte, 2, 16, HexPrefix)
                                                        .arg(QString(op->type->name))
                                                        .arg(QString(op->mode->name))));
        } else {
            --argv;
            prom.append(line.arg(byte, 2, 16, HexPrefix));
        }
        ++addr;
    }

    _prom->setPlainText(prom);
}

void Simulator::showChrRom()
{
    bool status = PatternTables::TotalSize == _card->chrRom().size();

    if (status) {
        drawPatternTable(PatternTables::TableLowerBankBase, _limg);
        drawPatternTable(PatternTables::TableUpperBankBase, _rimg);
    } else {
        // All black
        _limg.fill(0x00);
        _rimg.fill(0x00);
    }

    _mainWindow->leftPatternTable->setPixmap(QPixmap::fromImage(_limg.scaled(256, 256)));
    _mainWindow->rightPatternTable->setPixmap(QPixmap::fromImage(_rimg.scaled(256, 256)));

    _mainWindow->leftPatternTable->setEnabled(status);
    _mainWindow->rightPatternTable->setEnabled(status);

    // _limg.scaled(512, 512).save("left_patterns_table", "png");
    // _rimg.scaled(512, 512).save("right_patterns_table", "png");
}

void Simulator::drawPatternTable(int base, QImage &img)
{
    int addr, p0, p1;

    for (int y = 0; y <= 0x7f; ++y) {
        for (int x = 0; x <= 0x7f; ++x) {
            if (!(x & 0x07)) { // fine x
                // table base | coarse y | coarse x | fine y
                addr = base | (y & 0x78) << 5 | (x & 0x78) << 1 | (y & 0x07);
                p0 = _card->chrRom()[addr];        // plane 0
                p1 = _card->chrRom()[addr | 0x08]; // plane 1
            }

            img.setPixel(x, y, (p1 & 0x80) >> 6 | (p0 & 0x80) >> 7);

            p0 <<= 1;
            p1 <<= 1;
        }
    }
}

void Simulator::showPalettes()
{
    _engine.dumpPpuPalettes(_paletteColors);

    // QLabels of palettes set autoFillBackground=true already
    for (size_t i = 0; i < _paletteColors.size(); ++i) {
        QPalette p;
        uint32_t color = ppu::Colors[_paletteColors[i]/* & ppu::ColorPaletteMask*/];
        p.setColor(QPalette::Background, 0xff000000 | color);
        _paletteLables[i]->setPalette(p);
        _paletteLables[i]->setText(QString::number(_paletteColors[i], 16));
    }
}

void Simulator::showCurrentLine(uint16_t pc)
{
    int line = pc - ReadOnlyMemory::RomLowerBankBase;
    QTextCursor cursor(_prom->findBlockByLineNumber(line));
    cursor.select(QTextCursor::LineUnderCursor);
    _mainWindow->programRom->setTextCursor(cursor);
}

void Simulator::start()
{
    _thread = std::thread([this] () {
        _engine.start();
    });
}

void Simulator::stop()
{
    _engine.stop();
    if (_thread.joinable()) {
        _thread.join();
    }
}

void Simulator::onVideoDotRendered(int x, int y, uint32_t color)
{
    _videoBuffer.setPixel(x, y, 0xff000000 | color);
}

void Simulator::onVideoFrameRendered()
{
    showPalettes();
    _videoFrame.convertFromImage(_videoBuffer);
    emit showFrame();
}

void Simulator::onAudioOutput()
{
    // TODO
}

void Simulator::onRegistersChanged()
{
    // Show CPU Registers
    _engine.dumpCpuRegisters(_cpuRegisters);

    _mainWindow->cpuPC->setText(toHexString(_cpuRegisters.PC));
    _mainWindow->cpuS->setText(toHexString(_cpuRegisters.S));

    _mainWindow->cpuA->setText(toHexString(_cpuRegisters.A));
    _mainWindow->cpuX->setText(toHexString(_cpuRegisters.X));
    _mainWindow->cpuY->setText(toHexString(_cpuRegisters.Y));

    for (int i = 0, mask = 0x80; mask; mask >>= 1) {
        _cpuP[i] = _cpuRegisters.P & mask ? '1' : '0';
        i += CpuPSepLen;
    }
    _mainWindow->cpuP->setText(_cpuP);

    showCurrentLine(_cpuRegisters.PC);

    // TODO: Show PPU Registers
    _engine.dumpPpuRegisters(_ppuRegisters);
}

} // namespace tones
