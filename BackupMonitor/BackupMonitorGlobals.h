#ifndef BACKUPMONITORGLOBALS_H
#define BACKUPMONITORGLOBALS_H

// Wenn der letzete Sync laenger als X Sekunden her ist, wird Kunde rot markiert.
#define SECONDS_SYNC_INVALID 14400

enum BACKUPMONITOR_JOB_TYPE {
    JOB = 0,
    GROUP = 1,
    MIGRATION_JOB = 2,
    MIGRATION_JOB_GROUP = 3,
    PROTOCOL = 4     // Tagesprotokoll
};

#endif // BACKUPMONITORGLOBALS_H
