#ifndef _TONES_SIMULATOR_H_
#define _TONES_SIMULATOR_H_

#include <thread>

#include <QMainWindow>
#include <QAction>
#include <QLabel>
#include <QLineEdit>
#include <QImage>
#include <QPixmap>

#include "MotherBoard.h"

namespace tones {

class Simulator : public QMainWindow, public OutputPanel
{
    Q_OBJECT

public:

    Simulator(QWidget *parent=nullptr);
    ~Simulator();

    void onVideoDotRendered(int x, int y, const RGB &color) override;

    void onVideoFrameRendered() override;

    void onAudioOutput() override;

    void onCpuStepped() override;

Q_SIGNALS:

    void frameRendred();

public Q_SLOTS:

    void onOpen();

    void onPause();

    void onResume();

    void onFrameRendred();

protected:

    /* UI Setup */

    void setupActions();

    void setupMenu();

    void setupTool();

    void setupView();

    /* */

    void start();

    void stop();

private:

    std::thread *_thread;

    tones::MotherBoard _engine;

    /* CPU Registers */
    QLineEdit _cpu_pc;
    QLineEdit _cpu_s;
    QLineEdit _cpu_a;
    QLineEdit _cpu_p;
    QLineEdit _cpu_x;
    QLineEdit _cpu_y;

    /* Video */

    QLabel _screen;

    QImage _videoBuffer;

    QPixmap _videoFrame;

    /* Actions */

    QAction *_openAction;

    QAction *_pauseAction;

    QAction *_resumeAction;
};

} // namespace tones

#endif // _TONES_SIMULATOR_H_
