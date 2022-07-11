sudo service snmpd stop
sudo snmpd -f -Lo -C -c snmpd.conf &
sudo ./core


sudo snmptrapd -f -Lo -m ALL -C -c snmptrapd.conf  &

sudo mv /etc/snmp/snmpd.conf /etc/snmp/snmpd.conf_back
sudo cp snmpd.conf /etc/snmp/


snmpwalk -v 2c -c public 192.168.1.117:2161 1.3.6.1.4.1.30966.11

snmpget -v 2c -c public 192.168.1.117:2161 1.3.6.1.4.1.30966.11.8.0.0.1

snmpset -v 2c -c private 192.168.1.117:2161 1.3.6.1.4.1.30966.11.8.0.0.1 s "room"



