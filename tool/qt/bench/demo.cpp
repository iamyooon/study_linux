#include "demo.h"

DEMO::DEMO()
{

}

int DEMO::run_test_script()
{
    qDebug() << "[QT5 DBG] Get device id";

    QProcess get_dev_script, demo_script;

    //get_dev_script.QProcess::start("/home/geumran.lee/icp/53icp_testsuit_script/test.sh",QStringList(""));
    // not work
    // get_dev_script.start("/home/geumran.lee/icp/53icp_testsuit_script/get_dev_id.sh");
    // not work
    //get_dev_script.QProcess::start("/home/geumran.lee/icp/53icp_testsuit_script/bench_exec.sh dev_id",QStringList(""));
    // work
    get_dev_script.start("/home/geumran.lee/icp/53icp_testsuit_script/bench_exec.sh dev_id");

    if (!get_dev_script.waitForStarted())
        return -1;

    if (!get_dev_script.waitForFinished())
        return -1;

    QByteArray dev_ids = get_dev_script.readAll();

    this->dev_id_str = QString(dev_ids);

    QStringList list = dev_id_str.split(QLatin1Char('\n'), (QString::SplitBehavior)1);

    //for(int i=0; i < list.length(); i++)
    //    qDebug() << list.at(i);
    qDebug() << list.at(0);
    qDebug() << list.at(1);
    QString dev1_id = list.at(0);
    QString dev2_id = list.at(1);
    //qDebug() <<  dev_ids;
    //qDebug() <<  dev_id_str;

    qDebug() << "[QT5 DBG] start bench test";

    demo_script.start("/home/geumran.lee/icp/53icp_testsuit_script/bench_exec.sh");

    if (!demo_script.waitForStarted())
        return -1;

    if (!demo_script.waitForFinished())
        return -1;

    QByteArray get_stdout = demo_script.readAll();

    this->stdout_str = QString(get_stdout);

    qDebug() <<  stdout_str;

    qDebug() << "[QT5 DBG] print test result";

    QStringList dev1_option;
    dev1_option << "read" << dev1_id;
    //qDebug() << " " << dev1_option;

    demo_script.start("/home/geumran.lee/icp/53icp_testsuit_script/bench_exec.sh", dev1_option);

    if (!demo_script.waitForStarted())
        return -1;

    if (!demo_script.waitForFinished())
        return -1;

    QByteArray dev1_log = demo_script.readAll();

    this->dev1_str = QString(dev1_log);

    qDebug() <<  dev1_str;

    QStringList dev2_option;
    dev2_option << "read" << dev2_id;
    //qDebug() << " " << dev2_option;

    demo_script.start("/home/geumran.lee/icp/53icp_testsuit_script/bench_exec.sh",dev2_option);

    if (!demo_script.waitForStarted())
        return -1;

    if (!demo_script.waitForFinished())
        return -1;

    QByteArray dev2_log = demo_script.readAll();

    this->dev2_str = QString(dev2_log);

    qDebug() <<  dev2_str;

    qDebug() << "[QT5 DBG] run_test_script End ";

    return 0;
}
