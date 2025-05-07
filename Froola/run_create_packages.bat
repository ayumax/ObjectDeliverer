cd /d %~dp0
Froola.exe plugin -n ObjectDeliverer -p ObjectDelivererTest -v UE_5_5 -u git@github.com:ayumax/ObjectDeliverer.git -b UE5.5 -t -c -o packages
Froola.exe plugin -n ObjectDeliverer -p ObjectDelivererTest -v UE_5_4 -u git@github.com:ayumax/ObjectDeliverer.git -b UE5.4 -t -c -o packages
Froola.exe plugin -n ObjectDeliverer -p ObjectDelivererTest -v UE_5_3 -u git@github.com:ayumax/ObjectDeliverer.git -b UE5.3 -t -c -o packages