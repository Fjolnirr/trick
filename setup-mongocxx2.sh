cd `dirname $0`
SCRIPTDIR=`pwd`
cd -

sudo git config --global http.sslVerify false
git clone https://github.com/mongodb/mongo-cxx-driver.git --branch releases/stable
cd mongo-cxx-driver/build

sudo cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DBSONCXX_POLY_USE_MNMLSTC=1 \
    -DCMAKE_INSTALL_PREFIX=/usr/local

sudo make
sudo make install

cd $SCRIPTDIR
sudo rm -rf mongo-c-driver-1.25.0*