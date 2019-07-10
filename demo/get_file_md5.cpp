/// 基于openssl中的md5算法获取文件的md5

#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
// ADD IN cmake `-lcurl -lcrypto /data/soft/anaconda3/lib/libcurl.a`
// TARGET_LINK_LIBRARIES(test_${sample_basename} pthread -lcurl -lcrypto /data/soft/anaconda3/lib/libcurl.a)
#include <openssl/md5.h>

typedef struct FileMeta {
    std::string name;
    int size;
    std::string md5_sign;
} FileMeta;

int get_file_md5(const std::string &file_name, FileMeta& meta)
{
    meta.name = file_name;
    meta.md5_sign.clear();
    std::ifstream file(file_name.c_str(), std::ifstream::binary);
    if (!file) {
        return -1;
    }
    MD5_CTX md5Context;
    MD5_Init(&md5Context);
    char buf[1024 * 16];
    while (file.good()) {
        file.read(buf, sizeof(buf));
        MD5_Update(&md5Context, buf, file.gcount());
    }
    meta.size = (int)file.gcount();

    unsigned char result[MD5_DIGEST_LENGTH];
    MD5_Final(result, &md5Context);
    char hex[35];
    memset(hex, 0, sizeof(hex));
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        sprintf(hex + i * 2, "%02x", result[i]);
    }
    hex[32] = '\0';
    meta.md5_sign = std::string(hex);
    return 0;
}

int main(int argc, char const *argv[]) {
    std::string fname;
    if(argc >= 2) {
        fname = argv[1];
    } else {
        fname = argv[0];
    }
    std::string md5_value;
    FileMeta meta;
    get_file_md5(fname, meta);
    std::cout << "file:" << meta.name << std::endl;
    std::cout << "  |-size:" << meta.size << std::endl;
    std::cout << "  |-md5:" << meta.md5_sign << std::endl;
    return 0;
}
