
#include "Simulator.h"

#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "Cartridge.h"
#include "Log.h"

namespace tones {

Simulator::Simulator(QWidget *parent)
    : QMainWindow(parent)
    , _thread(nullptr)
    , _image(360, 320, QImage::Format_RGB888)
{
    setupActions();
    // setupMenu();
    setupTool();
    setupView();
    setupEngine();
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

    // TODO: Load logo
    // if (!_image.load("/home/li/Pictures/fengling.jpeg"))
    //     LOG_DEBUG() << "Failed to load logo";
    refresh();

    auto hbox = new QHBoxLayout();
    hbox->addWidget(&_screen);
    hbox->setAlignment(Qt::AlignCenter);

    centralWidget->setLayout(hbox);
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
}

void Simulator::setupEngine()
{
    _engine.setVideoOut([this] (int x, int y, const tones::RGB &rgb) {
        _image.setPixelColor(x, y, QColor::fromRgb(std::get<0>(rgb),
                                                   std::get<1>(rgb),
                                                   std::get<2>(rgb)));
    });

    _engine.setFrameEnd([this] () {
        refresh();
    });

    // _engine.setCpuViewer([] (tones::MicroProcessor::Registers_t &regs) {
    //     // TODO
    //     LOG_DEBUG() << std::hex
    //                 << " A: 0x" <<  (int)regs.A
    //                 << " X: 0x" <<  (int)regs.X
    //                 << " Y: 0x" <<  (int)regs.Y
    //                 << " S: 0x" <<  (int)regs.S
    //                 << " P: 0x" <<  (int)regs.P
    //                 << " PC: 0x" << (int)regs.PC
    //                 << std::dec;
    // });
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
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Select Rom:"),
                                                    ".",
                                                    tr("*.nes")); // "Roms (*.nes)"
    if (filename.isEmpty())
        return;

    stop();

    auto card = tones::CartridgeFactory::createCartridge(filename.toStdString());
    _engine.insert(card);
    _engine.reset();

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

void Simulator::refresh()
{
    _screen.setPixmap(QPixmap::fromImage(_image));
}

} // namespace tones
