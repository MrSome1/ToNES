#ifndef _TONES_SIMULATOR_H_
#define _TONES_SIMULATOR_H_

#include <thread>

#include <QMainWindow>
#include <QAction>
#include <QLabel>
#include <QImage>

#include "MotherBoard.h"

namespace tones {

class Simulator : public  QMainWindow
{
    Q_OBJECT

public:

    Simulator(QWidget *parent=nullptr);
    ~Simulator();

public Q_SLOTS:

    void onOpen();

    void onPause();

    void onResume();

protected:

    /* UI Setup */

    void setupActions();

    void setupMenu();

    void setupTool();

    void setupView();

    /* */

    void setupEngine();

    void start();

    void stop();

    void refresh();

private:

    std::thread *_thread;

    tones::MotherBoard _engine;

    /* Video */

    QLabel _screen;

    QImage _image;

    /* Actions */

    QAction *_openAction;

    QAction *_pauseAction;

    QAction *_resumeAction;
};

} // namespace tones

#endif // _TONES_SIMULATOR_H_
