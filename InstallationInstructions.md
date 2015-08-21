Get the source code from SVN by

svn checkout http://qtdownloadmanager.googlecode.com/svn/trunk/ qtdownloadmanager



U need to install qmake and qtlibraries to compile the code



Then issue the following commands:

cd qtdownloadmanager/QtDownloadManager

qmake

make

sudo make install



This will install QDM in /opt/QDM if u want to install the program somewhere else edit the QtDownloadManager.pro file accordingly.


If you dont want to compile.

There are also binaries shared. just place the QDM folder to anywhere on the system and launch the application using the QDM.sh script


U might have to change the permissions of the QDM.sh script


sudo chmod +x QDM.sh