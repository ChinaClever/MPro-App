sudo service snmpd stop
sudo snmpd -f -Lo -C -c snmpd.conf &
sudo ./core


sudo snmptrapd -f -Lo -m ALL -C -c snmptrapd.conf  &
