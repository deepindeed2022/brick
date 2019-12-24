#!/usr/bash
set -e
if [ ! -d build ]; then
    mkdir build
fi

cd build && cmake .. && make -j4
if [ ! -d test_image ]; then
    ln -s ../test_image test_image
fi
# test DFT
# ./test_discrete_fourier_transform test_image/imageTextR.png
# ./test_discrete_fourier_transform test_image/imageTextN.png

# test
./test_video_input_psnr_ssim test_image/Megamind.avi test_image/Megamind_bugy.avi  35 10
./test_background_subtraction -input=test_image/Megamind.avi -algo KNN
cd -