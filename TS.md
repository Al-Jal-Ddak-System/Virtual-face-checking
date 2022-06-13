## 1. Vim edit error

`sudo apt-get install vim-gui-common`  
`sudo apt-get install vim-runtime`  
`vim ~/.vimrc`
```
" ~/.vimrc

set number
set tabstop=4
set ignorecase
set hlsearch
set fileencodings=utf-8,euc-kr
set bs=indent,eol,start
set ruler
set title
set showmatch
set nowrap
set wmnu
syntax on
```

## 2. Install OpenCV 4.5.1 on RPI

1. opencv 가 이미 설치되어있다면 삭제

```
$ pkg-config --modversion opencv
$ sudo apt-get purge  libopencv* python-opencv
$ sudo apt-get autoremove
```

2. 업데이트

```
$ sudo apt update
$ sudo apt upgrade
```

3. 컴파일에 필요한 관련 라이브러리 설치

```
$ sudo apt install build-essential cmake
$ sudo apt install libjpeg-dev libtiff5-dev libjasper-dev libpng-dev
$ sudo apt install libavcodec-dev libavformat-dev libswscale-dev libxvidcore-dev libx264-dev libxine2-dev
$ sudo apt install libv4l-dev v4l-utils
$ sudo apt install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly
$ sudo apt install libgtk2.0-dev
$ sudo apt install mesa-utils libgl1-mesa-dri libgtkgl2.0-dev libgtkglext1-dev
$ sudo apt install libatlas-base-dev gfortran libeigen3-dev
$ sudo apt install python3-dev python3-numpy
```

<hr/>

3-1. 가상환경 설정

```
$ sudo pip install virtualenv virtualenvwrapper
$ sudo rm -rf ~/.cache/pip
```

```
$ echo -e "\n# virtualenv and virtualenvwrapper" >> ~/.profile
$ echo "export WORKON_HOME=$HOME/.virtualenvs" >> ~/.profile
$ echo "export VIRTUALENVWRAPPER_PYTHON=/usr/bin/python3" >> ~/.profile
$ echo "source /usr/local/bin/virtualenvwrapper.sh" >> ~/.profile
```

```
$ source ~/.profile
```

```
$ mkvirtualenv cv -p python3
```

가상환경은 아래 명령어를 통해 진입할 수 있다.

```
$ source ~/.profile
$ workon cv
```

<hr/>

4. opencv 설치

```
$ mkdir opencv
$ cd opencv
$ wget -O opencv.zip https://github.com/opencv/opencv/archive/4.5.1.zip
$ unzip opencv.zip
$ wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.5.1.zip
$ unzip opencv_contrib.zip
```

5. 컴파일

```
$ cd opencv-4.5.1
$ mkdir build
$ cd build
$ -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-4.5.1/modules
$ cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=OFF -D WITH_IPP=OFF -D WITH_1394=OFF -D BUILD_WITH_DEBUG_INFO=OFF -D BUILD_DOCS=OFF -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON -D BUILD_EXAMPLES=OFF -D BUILD_TESTS=OFF -D BUILD_PERF_TESTS=OFF -D ENABLE_NEON=ON -D ENABLE_VFPV3=ON -D WITH_QT=OFF -D WITH_GTK=ON -D WITH_OPENGL=ON -D OPENCV_ENABLE_NONFREE=ON -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-4.5.1/modules -D WITH_V4L=ON -D WITH_FFMPEG=ON -D WITH_XINE=ON -D ENABLE_PRECOMPILED_HEADERS=OFF -D BUILD_NEW_PYTHON_SUPPORT=ON -D OPENCV_GENERATE_PKGCONFIG=ON ../
```

6. 메모리 확장

```
$ sudo vi /etc/dphys-swapfile
```

CONF_SWAPSIZE=100 -> CONF_SWAPSIZE=2048 고쳐서 저장

```
$ sudo /etc/init.d/dphys-swapfile restart
```

7. 다시 컴파일

```
$ make -j4
```

j4 명령어를 이용해 쿼드코어로 컴파일하면 대략 70분 정도가 소요됨. 싱글코어 이용시 4시간 정도 걸림.

8. 설치

```
$ sudo make install
$ sudo ldconfig
```

9. 확인

```
$ python3
>>> import cv2
>>> cv2.__version__
'4.5.1'
>>>
```

9-1. 가상환경에 설치했다면 심볼릭 링킹을 해준다.

```
$ cd ~/.virtualenv/cv/lib/python3.9/site-package/
$ ln -s /user/local/lib/python3.9/site-package/cv2.so cv2.so
```

## 3. Install Tensorflow on RPI
