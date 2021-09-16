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
	//����ǰ������
	Json::Value value;
	Json::Reader rd;
	if (!rd.parse(req.body, value))
	{
		rsp.set_content("", "text/html");
		rsp.status = 500;
		return;
	}
	//���ı�ת��λbase64
	Base64 base64;
	string strRst=base64.Encode(value["strtextdata"].asString());

	//�������л�
	Json::StyledWriter sw;
	Json::Value  wvalue;
	wvalue["base64Data"] = strRst;
	strRst = sw.write(wvalue);

	rsp.set_content(strRst, "txt/html");
	rsp.status = 200;
}
void Base642Str(const httplib::Request& req, httplib::Response& rsp)
{
	//����ǰ������
	Json::Value value;
	Json::Reader rd;
	if (!rd.parse(req.body, value))
	{
		rsp.set_content("", "text/html");
		rsp.status = 500;
		return;
	}
	//���ı�ת��λbase64
	Base64 base64;
	string strRst = base64.Decode(value["base64data"].asString());

	//�������л�
	Json::StyledWriter sw;
	Json::Value  wvalue;
	wvalue["strtext"] = strRst;
	strRst = sw.write(wvalue);

	rsp.set_content(strRst, "txt/html");
	rsp.status = 200;
}
void Pic2Base64(const httplib::Request& req, httplib::Response& rsp)
{
	//������req�л�ȡͼƬ����
	auto picFile = req.files;
	auto formData = picFile.find("strPicData")->second;//��ȡ��
	auto picData = formData.content;//��ȡ��������--�������ݾ���һ��ͼƬ
	//��ͼƬ���ݣ������Ƹ�ʽ���ݣ�ת����base64����
	Base64 base64;
	string strRst;
	strRst += "<img src='data:img/jpg;base64,";
	strRst += base64.Encode(picData);
	strRst += "'/>";
	//��ת���Ľ������һ����Ӧ������ֵ�ͻ���
	//��JSON����֯���ص�����
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