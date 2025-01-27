
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
    , _ui(new Ui::MainWindow)
    , _videoBuffer(360, 320, QImage::Format_RGB888)
    , _cpuP(CpuPValue)
    , _prom(nullptr)
    , _limg(128, 128, QImage::Format_Indexed8)
    , _rimg(128, 128, QImage::Format_Indexed8)
{
    _ui->setupUi(this);

    setupView();
    setupConnections();
    changeStatus();

    // TODO: Load logo
    //if (!_videoBuffer.load("/home/li/Pictures/fengling.jpeg"))
    //    LOG_DEBUG() << "Failed to load logo";
    onVideoFrameRendered();

    _engine.setOutputPanel(*this);
}

Simulator::~Simulator()
{
    onStop();
}

void Simulator::setupView()
{
    _ui->cpuPHeader->setText(CpuPHeader);
    _prom = _ui->programRom->document();
    _limg.setColorTable(Colors);
    _rimg.setColorTable(Colors);
}

void Simulator::setupConnections()
{
    connect(_ui->actionOpen, &QAction::triggered, this, &Simulator::onOpen);
    connect(_ui->startButton, &QPushButton::clicked, this, &Simulator::onStart);
    connect(_ui->stopButton, &QPushButton::clicked, this, &Simulator::onStop);
    connect(_ui->pauseButton, &QPushButton::clicked, this, &Simulator::onPause);
    connect(_ui->stepButton, &QPushButton::clicked, this, &Simulator::onStep);
    connect(_ui->resetButton, &QPushButton::clicked, this, &Simulator::onReset);

    connect(this, &Simulator::showFrame, this, &Simulator::onShowFrame);
    connect(this, &Simulator::showCartridge, this, &Simulator::onShowCartridge);
}

void Simulator::changeStatus(Status s)
{
    switch (s) {
    case Started:
        _ui->startButton->setEnabled(false);
        _ui->pauseButton->setEnabled(true);
        _ui->stopButton->setEnabled(true);
        _ui->stepButton->setEnabled(false);
        _ui->resetButton->setEnabled(false);
        break;
    case Stopped:
        _ui->startButton->setEnabled(true);
        _ui->pauseButton->setEnabled(false);
        _ui->stopButton->setEnabled(false);
        _ui->stepButton->setEnabled(true);
        _ui->resetButton->setEnabled(true);
        _ui->pauseButton->setText(Pause);
        break;
    case Paused:
        _ui->startButton->setEnabled(false);
        _ui->pauseButton->setEnabled(true);
        _ui->stopButton->setEnabled(true);
        _ui->stepButton->setEnabled(true);
        _ui->resetButton->setEnabled(true);
        _ui->pauseButton->setText(Resume);
        break;
    case Resumed:
        _ui->startButton->setEnabled(false);
        _ui->pauseButton->setEnabled(true);
        _ui->stopButton->setEnabled(true);
        _ui->resetButton->setEnabled(false);
        _ui->resetButton->setEnabled(false);
        _ui->pauseButton->setText(Pause);
        break;
    default:
        _ui->startButton->setEnabled(false);
        _ui->pauseButton->setEnabled(false);
        _ui->stopButton->setEnabled(false);
        _ui->stepButton->setEnabled(false);
        _ui->resetButton->setEnabled(false);
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
    _ui->statusBar->showMessage(filename);
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
    _ui->screen->setPixmap(_videoFrame);
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

    _ui->leftPatternTable->setPixmap(QPixmap::fromImage(_limg));
    _ui->rightPatternTable->setPixmap(QPixmap::fromImage(_rimg));

    _ui->leftPatternTable->setEnabled(status);
    _ui->rightPatternTable->setEnabled(status);

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

            p0 <<= 1;
            p1 <<= 1;

            img.setPixel(x, y, ((p1 & 0x100) >> 7) | ((p0 & 0x100) >> 8));
        }
    }
}

void Simulator::showCurrentLine(uint16_t pc)
{
    int line = pc - ReadOnlyMemory::RomLowerBankBase;
    QTextCursor cursor(_prom->findBlockByLineNumber(line));
    cursor.select(QTextCursor::LineUnderCursor);
    _ui->programRom->setTextCursor(cursor);
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

void Simulator::onVideoDotRendered(int x, int y, const RGB &color)
{
    _videoBuffer.setPixelColor(x, y, QColor::fromRgb(std::get<0>(color),
                                                     std::get<1>(color),
                                                     std::get<2>(color)));
}

void Simulator::onVideoFrameRendered()
{
    _videoFrame.convertFromImage(_videoBuffer);
    emit showFrame();
}

void Simulator::onAudioOutput()
{
    // TODO
}

void Simulator::onCpuStepped(const MicroProcessor::Registers_t &regs)
{
    _ui->cpuPC->setText(toHexString(regs.PC));
    _ui->cpuS->setText(toHexString(regs.S));

    _ui->cpuA->setText(toHexString(regs.A));
    _ui->cpuX->setText(toHexString(regs.X));
    _ui->cpuY->setText(toHexString(regs.Y));

    for (int i = 0, mask = 0x80; mask; mask >>= 1) {
        _cpuP[i] = regs.P & mask ? '1' : '0';
        i += CpuPSepLen;
    }
    _ui->cpuP->setText(_cpuP);

    showCurrentLine(regs.PC);
}

} // namespace tones
