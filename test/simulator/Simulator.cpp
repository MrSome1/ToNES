
#include "Simulator.h"

#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QWidget>
#include <QString>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "Cartridge.h"
#include "Log.h"

namespace tones {

Simulator::Simulator(QWidget *parent)
    : QMainWindow(parent)
    , _thread(nullptr)
    , _videoBuffer(360, 320, QImage::Format_RGB888)
{
    setupActions();
    // setupMenu();
    setupTool();
    setupView();
    _engine.setOutputPanel(*this);
}

Simulator::~Simulator()
{
    stop();
}

void Simulator::setupMenu()
{
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu *fileMenu = menuBar->addMenu(tr("File"));
    fileMenu->addAction(_openAction);

    QMenu *controlMenu = menuBar->addMenu(tr("Control"));
    controlMenu->addAction(_pauseAction);
    controlMenu->addAction(_resumeAction);
}

void Simulator::setupTool()
{
    QToolBar *toolBar = new QToolBar(this);
    addToolBar(toolBar);

    toolBar->addAction(_openAction);
    toolBar->addAction(_pauseAction);
    toolBar->addAction(_resumeAction);
}

void Simulator::setupView()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto grid = new QGridLayout();
    grid->addWidget(new QLabel("PC", this), 0, 0);
    grid->addWidget(&_cpu_pc, 0, 2);
    grid->addWidget(new QLabel("S", this), 0, 4);
    grid->addWidget(&_cpu_s, 0, 6);
    grid->addWidget(new QLabel("A", this), 1, 0);
    grid->addWidget(&_cpu_a, 1, 2);
    grid->addWidget(new QLabel("P", this), 1, 4);
    grid->addWidget(&_cpu_p, 1, 6);
    grid->addWidget(new QLabel("X", this), 2, 0);
    grid->addWidget(&_cpu_x, 2, 2);
    grid->addWidget(new QLabel("Y", this), 2, 4);
    grid->addWidget(&_cpu_y, 2, 6);

    auto hbox = new QHBoxLayout();
    hbox->addWidget(&_screen);
    hbox->setAlignment(Qt::AlignCenter);
    hbox->addLayout(grid);

    centralWidget->setLayout(hbox);

    // TODO: Load logo
    // if (!_videoBuffer.load("/Users/mrxu/Pictures/fengling.jpg"))
    //     LOG_DEBUG() << "Failed to load logo";
    onVideoFrameRendered();
}

void Simulator::setupActions()
{
    _openAction = new QAction(tr("Open"), this);
    // _openAction->setShortcut(QKeySequence::Open);
    // _openAction->setStatusTip(tr("Open a image."));
    // _openAction->setIcon(QIcon(":/images/open.png")); 

    _pauseAction = new QAction(tr("Pause"), this);
    _resumeAction = new QAction(tr("Resume"), this);

    connect(_openAction, &QAction::triggered, this, &Simulator::onOpen);
    connect(_pauseAction, &QAction::triggered, this, &Simulator::onPause);
    connect(_resumeAction, &QAction::triggered, this, &Simulator::onResume);

    connect(this, &Simulator::frameRendred, this, &Simulator::onFrameRendred);
}

void Simulator::start()
{
    _thread = new std::thread([this] () {
        _engine.start();
    });
}

void Simulator::stop()
{
    _engine.stop();
    if (_thread) {
        _thread->join();
        _thread = nullptr;
    }
}

void Simulator::onOpen()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select Rom:"),
                                                    ".", tr("*.nes")); // "Roms (*.nes)"
    if (filename.isEmpty())
        return;

    stop();

    auto card = tones::CartridgeFactory::createCartridge(filename.toStdString());
    _engine.insert(card);

    start();
}

void Simulator::onPause()
{
    _engine.pause();
}

void Simulator::onResume()
{
    _engine.resume();
}

void Simulator::onFrameRendred()
{
    _screen.setPixmap(_videoFrame);
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
    emit frameRendred();
}

void Simulator::onAudioOutput()
{

}

void Simulator::onCpuStepped()
{
    if (_engine.isRunning())
        return; // refresh too often will crash

    _cpu_pc.setText(QString::number(_registers.PC, 16));
    _cpu_s.setText(QString::number(_registers.S, 16));

    _cpu_a.setText(QString::number(_registers.A, 16));
    _cpu_p.setText(QString::number(_registers.P, 2));

    _cpu_x.setText(QString::number(_registers.X, 16));
    _cpu_y.setText(QString::number(_registers.Y, 16));

    // LOG_DEBUG() << std::hex
    //             << " A: 0x" <<  (int)registers.A
    //             << " X: 0x" <<  (int)registers.X
    //             << " Y: 0x" <<  (int)registers.Y
    //             << " S: 0x" <<  (int)registers.S
    //             << " P: 0x" <<  (int)registers.P
    //             << " PC: 0x" << (int)registers.PC
    //             << std::dec;
}

} // namespace tones
