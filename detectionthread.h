//
// Created by xsy on 18-7-18.
//

#ifndef CHESSAGENT_DETECTIONTHREAD_H
#define CHESSAGENT_DETECTIONTHREAD_H

#include <QObject>
#include <QThread>

class DetectionThread : public QThread {
protected:
    void run() override;

};


#endif //CHESSAGENT_DETECTIONTHREAD_H
