#!/bin/sh
cur_dir=$(pwd)
protoc --cpp_out=. ${cur_dir}/samples/protobuf/addressbook.proto

if [[ ! -d ${cur_dir}/build ]]; then 
    mkdir -p ${cur_dir}/build
fi
cd ${cur_dir}/build && cmake .. && make -j4
echo "## start test gen_file_md5"
./test_get_file_md5


echo "## start test list_files_in_dir"
./test_list_files_in_dir ${cur_dir}/samples ./ .cpp

echo "## start test_sample_zlib"
./test_sample_zlib

# echo "## start test_sample_fstream"
# ./test_sample_fstream

echo "## start test_sample_eigen"
./test_sample_eigen

echo "## start test_sample_curl"
./test_sample_curl

echo "## start test_gen_json"
./test_gen_json

cd -