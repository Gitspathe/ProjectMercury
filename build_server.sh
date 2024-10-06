mkdir bin
cmake -B bin -DBUILD_SERVER=ON -DBUILD_CLIENT=OFF -DDEV_MODE=ON
cd bin
make
