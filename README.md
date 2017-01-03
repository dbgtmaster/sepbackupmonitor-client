Derzeit stehen nur Debian Pakete für Ubuntu 14 amd64, Ubuntu 16 amd64 sowie auch für Windows.
Ubuntu Pakete sollten auch mit Debian kompatibel sein.

Download: https://github.com/dbgtmaster/sepbackupmonitor-client/tree/master/Download-Packages
Zum Download auf das jeweilige Paket klicken und auf den "Download" Button.


Ubuntu
===
dpkg -i [paketname].dpkg

Falls die Installation aufgrund von Abhängigkeiten fehlschlägt, so kann mit folgendem Befehl die Abhängigkeiten installiert werden:
apt-get -f install

Gestartet wird der Client mit dem Befehl "sepbackupmonitor-client" oder in der Applikationssuche mit dem Namen "SEPBackupmonitor-Client"