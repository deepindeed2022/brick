/*************************************************************************
	> File Name: gen_json.cpp
	> Author: caowenlong
	> Mail: caowenlong92@gmail.com
	> Created Time: 2019年07月31日
 ************************************************************************/
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <json11.hpp>
/// cv rectangle definition
typedef struct cv_float_rect_t {
    float left;   ///< 矩形最左边的坐标
    float top;    ///< 矩形最上边的坐标
    float right;  ///< 矩形最右边的坐标
    float bottom; ///< 矩形最下边的坐标
} cv_float_rect_t;

typedef struct IDCardImg {
	typedef struct IDCardLabelRect {
		std::string label;
		int number;
		std::string ops;
		bool out_target_boundary;
		cv_float_rect_t position;
		std::string source;
		bool valid;
	} IDCardLabelRect;
	int channel;
	int height;
	int width;
	int rotate;
	std::string id;
	std::string rawFilename;
	bool valid;
	int rect_number;
	std::vector<IDCardLabelRect> rects;
} IDCardImg;

const std::string _label_name = "calc";
json11::Json to_json(const IDCardImg& value);

int main(int argc, char *argv[]) {
    std::string rawFilename = "1.jpg";
    std::ofstream ferr;
    ferr.open("autogen_crop_idcard.log");
    IDCardImg st_idcard = {
        3,
        123,
        123,
        90,
        "akmdlfmasl19230u01josndnaldn",
        rawFilename,
        false,
        0,
        {}
    };
    std::string id = st_idcard.id;
    json11::Json st_idcard_json = to_json(st_idcard);
    std::ofstream json_out;
    std::string out_json_name = id + ".json";
    json_out.open(out_json_name, std::ios_base::out);
    std::string result;
    st_idcard_json.dump(result);
    json_out << result << std::endl;
    json_out.close();
    return 0;
}

json11::Json to_json(const cv_float_rect_t& value) {
	return json11::Json::object {
		{"left", std::to_string(value.left)},
		{"top",  std::to_string(value.top)},
		{"bottom",  std::to_string(value.bottom)},
		{"right",  std::to_string(value.right)}
	};
}

json11::Json to_json(const IDCardImg::IDCardLabelRect& value) {
	return json11::Json::object {
		{"label", value.label},
		{"number",  std::to_string(value.number)},
		{"ops", value.ops},
		{"out_target_boundary", value.out_target_boundary ? "true": "false"},
		{"position", to_json(value.position)},
		{"source", value.source},
		{"valid", value.valid ? "true": "false"}
	};
}

json11::Json to_json(const IDCardImg& value) {
	json11::Json::object obj;
    obj["idcard-validity"] = "true";
	obj["channel"] = std::to_string(value.channel);
	obj["width"] = std::to_string(value.width);
	obj["heigth"] = std::to_string(value.height);
	obj["rotate"] = std::to_string(value.rotate);
	obj["id"] = value.id;
	json11::Json::array rects;
	for(int i = 0; i < value.rect_number; i++) {
		rects.push_back(to_json(value.rects[i]));
	}
	obj["rects"] = json11::Json::object{{"idcard", rects}};
	obj["rawFilePath"] = "";
	obj["rawFilename"] = value.rawFilename;
	obj["result_version"] = "1.0";
	obj["valid"] = value.valid ? "true": "false";
	return obj;
}