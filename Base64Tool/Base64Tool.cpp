#include"Base64.h"
#include"httplib.h"
#include"json.h"
#include<iostream>
#pragma comment(lib,".\\..\\Debug\\json_vc71_libmtd.lib")
using namespace std;
void TestJson()
{
	Json::Value value;
}
void Str2Base64(const httplib::Request& req, httplib::Response& rsp)
{
	//解析前端数据
	Json::Value value;
	Json::Reader rd;
	if (!rd.parse(req.body, value))
	{
		rsp.set_content("", "text/html");
		rsp.status = 500;
		return;
	}
	//将文本转换位base64
	Base64 base64;
	string strRst=base64.Encode(value["strtextdata"].asString());

	//进行序列化
	Json::StyledWriter sw;
	Json::Value  wvalue;
	wvalue["base64Data"] = strRst;
	strRst = sw.write(wvalue);

	rsp.set_content(strRst, "txt/html");
	rsp.status = 200;
}
void Base642Str(const httplib::Request& req, httplib::Response& rsp)
{
	//解析前端数据
	Json::Value value;
	Json::Reader rd;
	if (!rd.parse(req.body, value))
	{
		rsp.set_content("", "text/html");
		rsp.status = 500;
		return;
	}
	//将文本转换位base64
	Base64 base64;
	string strRst = base64.Decode(value["base64data"].asString());

	//进行序列化
	Json::StyledWriter sw;
	Json::Value  wvalue;
	wvalue["strtext"] = strRst;
	strRst = sw.write(wvalue);

	rsp.set_content(strRst, "txt/html");
	rsp.status = 200;
}
void Pic2Base64(const httplib::Request& req, httplib::Response& rsp)
{
	//从请求req中获取图片数据
	auto picFile = req.files;
	auto formData = picFile.find("strPicData")->second;//获取表单
	auto picData = formData.content;//获取表单的内容--表单的内容就是一张图片
	//将图片数据（二进制格式数据）转化成base64编码
	Base64 base64;
	string strRst;
	strRst += "<img src='data:img/jpg;base64,";
	strRst += base64.Encode(picData);
	strRst += "'/>";
	//将转化的结果构造一个响应包返回值客户端
	//用JSON来组织返回的数据
	Json::Value value;
	value["picbase64Data"] = strRst;

	Json::StyledWriter sw;
	strRst = sw.write(value);
	rsp.set_content("", "text/html");
	rsp.status = 200;
}
int main()
{
	//Base64 b;
	//std::string encode = b.Encode("hello");
	//cout << encode << endl;
	//std::string decode = b.Decode(encode);
	//cout << decode << endl;
	

	httplib::Server s;

	s.set_base_dir(".//..//Debug");
	s.Post("/str_2_base64", Str2Base64);
	s.Post("/base64_2_str", Base642Str);
	s.Post("/pic_2_base64", Pic2Base64);
	s.listen("127.0.0.1", 8000);

}