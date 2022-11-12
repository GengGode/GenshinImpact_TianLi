#pragma once
// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼��ʽ���� DLL �е������ļ��������������϶���� GENSHINIMPACT_TIANLI_SQLITE_DLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ���Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// GENSHINIMPACT_TIANLI_SQLITE_DLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
// GENSHINIMPACTTIANLISQLITE_EXPORTS
// _WINDOWS
// _USRDLL
#ifndef _LIB
#ifdef GENSHINIMPACTTIANLISQLITE_EXPORTS
#define GENSHINIMPACTTIANLISQLITE_API __declspec(dllexport)
#else
//#define GENSHINIMPACTTIANLISQLITE_API __declspec(dllimport)
#define GENSHINIMPACTTIANLISQLITE_API /* No thing */
#endif

#define APICALL __stdcall
#ifndef DLLAPI
#define DLLAPI GENSHINIMPACTTIANLISQLITE_API
#endif // DLLAPI

#else
#ifndef DLLAPI
#define DLLAPI
#endif // DLLAPI
#endif
class SqliteImpl;
extern "C"
{

	struct DLLAPI  Item
	{
		char* name;
		int index;
		double x;
		double y;
		double z;
		int uuid;
		char* msg;

		Item(const char* name, int index, double x, double y, double z, int uuid, const char* msg);
		~Item();
	};
	struct DLLAPI  ItemsVector
	{
		char** names;
		int* indexs;
		double* xs;
		double* ys;
		double* zs;
		int* uuids;
		char** msgs;

		int size;

		ItemsVector();
		~ItemsVector();
		Item operator[](int i);

		void Append(const char* name, int index, double x, double y, double z, int uuid, const char* msg);
	};
	struct DLLAPI  TextVector
	{
		char** text;
		int size;
		TextVector();
		~TextVector();
		const char* operator[](int i);
		void Append(const char* str);
	};
	class  DLLAPI GenshinImpact_TianLi_Sqlite
	{
	public:
		SqliteImpl* impl = nullptr;
	public:
		GenshinImpact_TianLi_Sqlite();
		~GenshinImpact_TianLi_Sqlite();
		int OpenSqlite(const char* dbPath);
		int OpenSqlite(unsigned char *dbData,int size);
		int CloseSqlite();
	public:
		//std::vector<std::pair<std::string, int>> get_area_map();
	public:
		// ��ȡ���ҵ���
		int ReadCountry(TextVector& text);
		// ��ȡ�ù��ҵ����µ���������
		int ReadType(const char* area, TextVector& text);
		// ��ȡ�ù��ҵ��������µ�������Ʒ����
		int ReadItem(const char* area, const char* type, TextVector& text);
		// ��ȡ�ù��ҵ���������Ʒ�����µ�������Ʒ
		int ReadItems(const char* area, const char* type, const char* item, TextVector& text);
		int ReadItems(const char* area, const char* type, const char* item, ItemsVector& items);
		
		//===== ��ȡͼƬdata���
	public:
		// �������ƻ�ȡ���Ͷ�Ӧͼ������
		int GetTypeImage(const char* name, unsigned char*& data, int& size);
		// �������ƻ�ȡ��Ʒ��Ӧͼ������
		int GetItemImage(const char* name, unsigned char*& data, int& size);
	
	};

}

#include<map>
#include<vector>
#include<string>
struct Tag
{
	enum TagType 
	{
		Area,
		Type,
		Item,
		Object
	};
	TagType tag_type;
	int id;
	std::string name;
	std::string icon_tag;

	Tag() = default;
	~Tag() = default;
	
};
namespace v1
{
	std::map<std::pair<int, std::string>, std::vector<std::pair<int, std::string>> > get_area_map(GenshinImpact_TianLi_Sqlite* sqlite);
	std::map<std::pair<std::pair<int, std::string>, std::string>, std::vector<std::pair<std::pair<int, std::string>, std::string>> >  get_type_map(GenshinImpact_TianLi_Sqlite* sqlite);

}
std::map<Tag, std::vector<Tag> > get_area_group_map(GenshinImpact_TianLi_Sqlite* sqlite);
std::map<Tag, std::vector<Tag> > get_type_group_map(GenshinImpact_TianLi_Sqlite* sqlite);
