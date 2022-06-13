#ifndef CLUSTERTESTER_H
#define CLUSTERTESTER_H

#include <QtTest>
#include "clusterManager.h"
#include "FileManipulation.h"

class clusterTester : public QObject
{
    Q_OBJECT

public:
    clusterTester();
    ~clusterTester();

private Q_SLOTS:
    void test_login();
    void test_login_data();
    void test_cluster_write();
    void test_cluster_write_data();
    void test_cluster_download();
    void test_cluster_download_data();
};

#endif
