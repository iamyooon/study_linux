#ifndef DEMO_H
#define DEMO_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QProcess>

//#define DEMO_SCRIPT_STATIC_PATH "/home/geumran.lee/icp_work/src/qt5_qprocess_tutorial/demo_script.sh"
#define DEMO_SCRIPT_STATIC_PATH "/home/geumran.lee/icp/53icp_testsuit_script/run.sh"
//#define DEMO_SCRIPT_STATIC_PATH "/home/geumran.lee/icp/53icp_testsuit_script/test.sh"
#define GET_DEV_ID_SCRIPT_PATH "/home/geumran.lee/icp/53icp_testsuit_script/bench_exec.sh dev_id"
//#define DEMO_SCRIPT_STATIC_PATH "/home/geumran.lee/icp/53icp_testsuit_script/bench_exec.sh"
class DEMO : public QObject
{
    Q_OBJECT
public:
    DEMO();
    QString dev_id_str;
    QString stdout_str;
    QString dev1_str, dev2_str;
    int run_test_script(void);
};

#endif // DEMO_H
