/**
  * UPDATE:
  * customers rename int to sync_id char very
  * add id serial not null
  *
  * Sync software anpassen!
  **/

#include <QApplication>
#include <TcpMainThread.h>
#include <LoginWindow/LoginWindow.h>

#include <QTcpSocket>
#include <TcpCommandsHelper.h>
#include <QDataStream>

#include <TcpCommandFactories.h>
#include "TcpCommands/main/userLoginFactory.h"
#include "TcpCommands/main/serverVersionFactory.h"
#include "TcpCommands/main/connectTcpFactory.h"
#include "TcpCommands/main/resetTimeoutFactory.h"
#include "TcpCommands/loggedInUser/getUserColumnFactory.h"
#include "TcpCommands/loggedInUser/hasTcpPermissionFactory.h"
#include "TcpCommands/system/getAllTcpCommandsFactory.h"
#include "TcpCommands/system/getAllSystemGroupsFactory.h"
#include "TcpCommands/system/saveTcpCommandsPermissionsFactory.h"
#include "TcpCommands/system/serverStatisticsFactory.h"
#include "TcpCommands/administration/testLDAPGroupMembershipsFactory.h"
#include "TcpCommands/administration/userGroupModFactory.h"
#include "TcpCommands/administration/getAllUserGroupsFactory.h"
#include "TcpCommands/administration/getUserGroupFactory.h"
#include "TcpCommands/administration/userGroupDeleteFactory.h"
#include "TcpCommands/administration/userGroupToSystemGroupsFactory.h"
#include "TcpCommands/backupMonitor/getCustomersFactory.h"
#include "TcpCommands/customers/searchFactory.h"
#include "TcpCommands/customers/getCustomerFactory.h"
#include "TcpCommands/customers/modCustomerFactory.h"
#include "TcpCommands/customers/generateNewBackupMonitorKeyFactory.h"
#include "TcpCommands/customers/customerEditableFactory.h"
#include "TcpCommands/customers/removeCustomerFactory.h"
#include "TcpCommands/backupMonitor/getJobsFactory.h"
#include "TcpCommands/backupMonitor/getJobFactory.h"
#include "TcpCommands/backupMonitor/getJobProtocolFactory.h"
#include "TcpCommands/backupMonitor/getBackupStateOfDayFactory.h"
#include "TcpCommands/backupMonitor/getCustomerGroupsFactory.h"
#include "TcpCommands/backupMonitor/customerGroupModFactory.h"
#include "TcpCommands/backupMonitor/customerGroupDeleteFactory.h"
#include "TcpCommands/backupMonitor/getCustomerInterfaceAddressFactory.h"
#include "TcpCommands/backupMonitor/getLogFactory.h"
#include "TcpCommands/backupMonitor/createLogFactory.h"

#include <iostream>

#include "BackupMonitor/BackupMonitorLogWidget.h"

int main(int argc, char **argv) {

    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    qRegisterMetaType<TcpCommandsHelper::userLoginResponse>("TcpCommandsHelper::userLoginResponse");
    QApplication app(argc, argv);

    app.setOrganizationName("Siedl Networks GmbH");
    app.setOrganizationDomain("www.siedl.net");

    app.setApplicationName("SEP-Backupmonitor");

    // Die wichtigsten TcpCommandFactories registrieren
    // (welche für den Loginvorgang beötigt werden):
    TcpCommandFactories *tcpFacts = TcpCommandFactories::instance();
    tcpFacts->registerFactory( new TcpCommandFactory_main_userLogin );
    tcpFacts->registerFactory( new TcpCommandFactory_main_serverVersion );
    tcpFacts->registerFactory( new TcpCommandFactory_main_connectTcp );
    tcpFacts->registerFactory( new TcpCommandFactory_main_resetTimeout );
    tcpFacts->registerFactory( new TcpCommandFactory_loggedInUser_getUserColumn );
    tcpFacts->registerFactory( new TcpCommandFactory_loggedInUser_hasTcpPermission );
    tcpFacts->registerFactory( new TcpCommandFactory_system_getAllTcpCommands );
    tcpFacts->registerFactory( new TcpCommandFactory_system_getAllSystemGroups );
    tcpFacts->registerFactory( new TcpCommandFactory_system_saveTcpCommandsPermissions );
    tcpFacts->registerFactory( new TcpCommandFactory_system_serverStatistics );
    tcpFacts->registerFactory( new TcpCommandFactory_administration_testLDAPGroupMemberships );
    tcpFacts->registerFactory( new TcpCommandFactory_administration_userGroupMod );
    tcpFacts->registerFactory( new TcpCommandFactory_administration_getAllUserGroups );
    tcpFacts->registerFactory( new TcpCommandFactory_administration_getUserGroup );
    tcpFacts->registerFactory( new TcpCommandFactory_administration_userGroupDelete );
    tcpFacts->registerFactory( new TcpCommandFactory_administration_userGroupToSystemGroups );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_getCustomers );
    tcpFacts->registerFactory( new TcpCommandFactory_customers_search );
    tcpFacts->registerFactory( new TcpCommandFactory_customers_getCustomer );
    tcpFacts->registerFactory( new TcpCommandFactory_customers_modCustomer );
    tcpFacts->registerFactory( new TcpCommandFactory_customers_generateNewBackupMonitorKey );
    tcpFacts->registerFactory( new TcpCommandFactory_customers_customerEditable );
    tcpFacts->registerFactory( new TcpCommandFactory_customers_removeCustomer );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_getJobs );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_getJob );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_getJobProtocol );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_getBackupStateOfDay );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_getCustomerGroups );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_customerGroupMod );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_customerGroupDelete );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_getCustomerInterfaceAddress );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_getLog );
    tcpFacts->registerFactory( new TcpCommandFactory_backupMonitor_createLog );


    LoginWindow login;
    login.show();

    return app.exec();
}
