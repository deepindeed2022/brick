#include <glog/logging.h>
#include <iostream>
#include <gflags/gflags.h>
DEFINE_string(confPath, "test.conf", "program configure file.");
DEFINE_int32(port, 1111, "program listen port");
DEFINE_bool(debug, true, "run debug mode");
using namespace std;
int main(int argc, char **argv) {

    // logging demo, google::COUNTER表示log级别
    google::InitGoogleLogging(argv[0]);
    FLAGS_stderrthreshold = google::INFO;
    FLAGS_colorlogtostderr = true;
    for (int i = 1; i <= 100; i++)
    {
        // LOG(INFO) << "LOG(INFO)  google::COUNTER=" << google::COUNTER << "  i=" << i;
        LOG_IF(INFO, i == 100) << "LOG_IF(INFO,i==100)  google::COUNTER=" << google::COUNTER << "  i=" << i;
        LOG_EVERY_N(INFO, 10) << "LOG_EVERY_N(INFO,10)  google::COUNTER=" << google::COUNTER << "  i=" << i;
        LOG_IF_EVERY_N(WARNING, (i > 50), 10) << "LOG_IF_EVERY_N(INFO,(i>50),10)  google::COUNTER=" << google::COUNTER << "  i=" << i;
        LOG_FIRST_N(ERROR, 5) << "LOG_FIRST_N(INFO,5)  google::COUNTER=" << google::COUNTER << "  i=" << i;
    }
    google::ShutdownGoogleLogging();

    // google::ParseCommandLineFlags(&argc, &argv, false);//这里可以试一下第三个参数是true和false的区别
    google::ParseCommandLineFlags(&argc, &argv, true);
    std::cout << "argc= " << argc << std::endl;
    for (int i = 0; i < argc; ++i)
        std::cout << "argv[" << i << "]:" << argv[i] << std::endl;
    std::cout << "confPath = " << FLAGS_confPath << std::endl;
    std::cout << "port = " << FLAGS_port << std::endl;

    if (FLAGS_debug) {
        std::cout << "this is a debug mode..." << std::endl;
    } else {
        std::cout << "this is a nodebug mode...." << std::endl;
    }
    std::cout << "succuss" << std::endl;
    google::ShutDownCommandLineFlags();
    return 0;
}
