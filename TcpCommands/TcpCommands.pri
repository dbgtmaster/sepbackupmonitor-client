SOURCES += TcpCommands/main/connectTcp.cpp \
    TcpCommands/main/serverVersion.cpp \
    TcpCommands/main/userLogin.cpp \
    TcpCommands/loggedInUser/getUserColumn.cpp \
    TcpCommands/main/userLoginFactory.cpp \
    TcpCommands/main/serverVersionFactory.cpp \
    TcpCommands/main/connectTcpFactory.cpp \
    TcpCommands/loggedInUser/getUserColumnFactory.cpp \
    TcpCommands/system/getAllTcpCommandsFactory.cpp \
    TcpCommands/system/getAllTcpCommands.cpp \
    TcpCommands/system/getAllSystemGroupsFactory.cpp \
    TcpCommands/system/getAllSystemGroups.cpp \
    TcpCommands/system/saveTcpCommandsPermissionsFactory.cpp \
    TcpCommands/system/saveTcpCommandsPermissions.cpp \
    TcpCommands/administration/testLDAPGroupMembershipsFactory.cpp \
    TcpCommands/administration/testLDAPGroupMemberships.cpp \
    TcpCommands/administration/userGroupModFactory.cpp \
    TcpCommands/administration/userGroupMod.cpp \
    TcpCommands/administration/getAllUserGroupsFactory.cpp \
    TcpCommands/administration/getAllUserGroups.cpp \
    TcpCommands/administration/getUserGroupFactory.cpp \
    TcpCommands/administration/getUserGroup.cpp \
    TcpCommands/administration/userGroupDeleteFactory.cpp \
    TcpCommands/administration/userGroupDelete.cpp \
    TcpCommands/administration/userGroupToSystemGroupsFactory.cpp \
    TcpCommands/administration/userGroupToSystemGroups.cpp \
    TcpCommands/backupMonitor/getCustomersFactory.cpp \
    TcpCommands/backupMonitor/getCustomers.cpp \
    TcpCommands/customers/searchFactory.cpp \
    TcpCommands/customers/search.cpp \
    TcpCommands/customers/getCustomerFactory.cpp \
    TcpCommands/customers/getCustomer.cpp \
    TcpCommands/customers/modCustomerFactory.cpp \
    TcpCommands/customers/modCustomer.cpp \
    TcpCommands/customers/generateNewBackupMonitorKeyFactory.cpp \
    TcpCommands/customers/generateNewBackupMonitorKey.cpp \
    TcpCommands/backupMonitor/getJobsFactory.cpp \
    TcpCommands/backupMonitor/getJobs.cpp \
    TcpCommands/backupMonitor/getJobFactory.cpp \
    TcpCommands/backupMonitor/getJob.cpp \
    TcpCommands/backupMonitor/getJobProtocolFactory.cpp \
    TcpCommands/backupMonitor/getJobProtocol.cpp \
    TcpCommands/system/serverStatisticsFactory.cpp \
    TcpCommands/system/serverStatistics.cpp \
    TcpCommands/loggedInUser/hasTcpPermissionFactory.cpp \
    TcpCommands/loggedInUser/hasTcpPermission.cpp \
    TcpCommands/backupMonitor/getBackupStateOfDayFactory.cpp \
    TcpCommands/backupMonitor/getBackupStateOfDay.cpp \
    TcpCommands/notifications/createFactory.cpp \
    TcpCommands/notifications/create.cpp \
    TcpCommands/notifications/getLastMessagesFactory.cpp \
    TcpCommands/notifications/getLastMessages.cpp \
    TcpCommands/backupMonitor/getCustomerGroupsFactory.cpp \
    TcpCommands/backupMonitor/getCustomerGroups.cpp \
    TcpCommands/backupMonitor/customerGroupModFactory.cpp \
    TcpCommands/backupMonitor/customerGroupMod.cpp \
    TcpCommands/backupMonitor/customerGroupDeleteFactory.cpp \
    TcpCommands/backupMonitor/customerGroupDelete.cpp \
    TcpCommands/customers/customerEditableFactory.cpp \
    TcpCommands/customers/customerEditable.cpp \
    TcpCommands/customers/removeCustomerFactory.cpp \
    TcpCommands/customers/removeCustomer.cpp \
    TcpCommands/main/resetTimeoutFactory.cpp \
    TcpCommands/main/resetTimeout.cpp \
    TcpCommands/backupMonitor/getCustomerInterfaceAddressFactory.cpp \
    TcpCommands/backupMonitor/getCustomerInterfaceAddress.cpp \
    TcpCommands/backupMonitor/getLogFactory.cpp \
    TcpCommands/backupMonitor/getLog.cpp \
    TcpCommands/backupMonitor/createLogFactory.cpp \
    TcpCommands/backupMonitor/createLog.cpp

HEADERS += TcpCommands/main/connectTcp.h \
    TcpCommands/main/serverVersion.h \
    TcpCommands/main/userLogin.h \
    TcpCommands/loggedInUser/getUserColumn.h \
    TcpCommands/main/userLoginFactory.h \
    TcpCommands/main/serverVersionFactory.h \
    TcpCommands/main/connectTcpFactory.h \
    TcpCommands/loggedInUser/getUserColumnFactory.h \
    TcpCommands/system/getAllTcpCommandsFactory.h \
    TcpCommands/system/getAllTcpCommands.h \
    TcpCommands/system/getAllSystemGroupsFactory.h \
    TcpCommands/system/getAllSystemGroups.h \
    TcpCommands/system/saveTcpCommandsPermissionsFactory.h \
    TcpCommands/system/saveTcpCommandsPermissions.h \
    TcpCommands/administration/testLDAPGroupMembershipsFactory.h \
    TcpCommands/administration/testLDAPGroupMemberships.h \
    TcpCommands/administration/userGroupModFactory.h \
    TcpCommands/administration/userGroupMod.h \
    TcpCommands/administration/getAllUserGroupsFactory.h \
    TcpCommands/administration/getAllUserGroups.h \
    TcpCommands/administration/getUserGroupFactory.h \
    TcpCommands/administration/getUserGroup.h \
    TcpCommands/administration/userGroupDeleteFactory.h \
    TcpCommands/administration/userGroupDelete.h \
    TcpCommands/administration/userGroupToSystemGroupsFactory.h \
    TcpCommands/administration/userGroupToSystemGroups.h \
    TcpCommands/backupMonitor/getCustomersFactory.h \
    TcpCommands/backupMonitor/getCustomers.h \
    TcpCommands/customers/searchFactory.h \
    TcpCommands/customers/search.h \
    TcpCommands/customers/getCustomerFactory.h \
    TcpCommands/customers/getCustomer.h \
    TcpCommands/customers/modCustomerFactory.h \
    TcpCommands/customers/modCustomer.h \
    TcpCommands/customers/generateNewBackupMonitorKeyFactory.h \
    TcpCommands/customers/generateNewBackupMonitorKey.h \
    TcpCommands/backupMonitor/getJobsFactory.h \
    TcpCommands/backupMonitor/getJobs.h \
    TcpCommands/backupMonitor/getJobFactory.h \
    TcpCommands/backupMonitor/getJob.h \
    TcpCommands/backupMonitor/getJobProtocolFactory.h \
    TcpCommands/backupMonitor/getJobProtocol.h \
    TcpCommands/system/serverStatisticsFactory.h \
    TcpCommands/system/serverStatistics.h \
    TcpCommands/loggedInUser/hasTcpPermissionFactory.h \
    TcpCommands/loggedInUser/hasTcpPermission.h \
    TcpCommands/backupMonitor/getBackupStateOfDayFactory.h \
    TcpCommands/backupMonitor/getBackupStateOfDay.h \
    TcpCommands/notifications/createFactory.h \
    TcpCommands/notifications/create.h \
    TcpCommands/notifications/getLastMessagesFactory.h \
    TcpCommands/notifications/getLastMessages.h \
    TcpCommands/backupMonitor/getCustomerGroupsFactory.h \
    TcpCommands/backupMonitor/getCustomerGroups.h \
    TcpCommands/backupMonitor/customerGroupModFactory.h \
    TcpCommands/backupMonitor/customerGroupMod.h \
    TcpCommands/backupMonitor/customerGroupDeleteFactory.h \
    TcpCommands/backupMonitor/customerGroupDelete.h \
    TcpCommands/customers/customerEditableFactory.h \
    TcpCommands/customers/customerEditable.h \
    TcpCommands/customers/removeCustomerFactory.h \
    TcpCommands/customers/removeCustomer.h \
    TcpCommands/main/resetTimeoutFactory.h \
    TcpCommands/main/resetTimeout.h \
    TcpCommands/backupMonitor/getCustomerInterfaceAddressFactory.h \
    TcpCommands/backupMonitor/getCustomerInterfaceAddress.h \
    TcpCommands/backupMonitor/getLog.h \
    TcpCommands/backupMonitor/getLogFactory.h \
    TcpCommands/backupMonitor/createLog.h \
    TcpCommands/backupMonitor/createLogFactory.h

FORMS +=


































