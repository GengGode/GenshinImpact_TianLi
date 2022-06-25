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
#define DLLAPI GENSHINIMPACTTIANLISQLITE_API

#else
#define DLLAPI
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

		void append(const char* name, int index, double x, double y, double z, int uuid, const char* msg);
	};
	struct DLLAPI  TextVector
	{
		char** text;
		int size;
		TextVector();
		~TextVector();
		const char* operator[](int i);
		void append(const char* str);
	};
	class  DLLAPI GenshinImpact_TianLi_Sqlite
	{
		SqliteImpl* impl = nullptr;
	public:
		GenshinImpact_TianLi_Sqlite();
		~GenshinImpact_TianLi_Sqlite();
		int OpenSqlite(const char* dbPath);
		int OpenSqlite(unsigned char *dbData,int size);
		int CloseSqlite();

		// ��ȡ���ҵ���
		int ReadCountry(TextVector& text);
		// ��ȡ�ù��ҵ����µ���������
		int ReadType(const char* country, TextVector& text);
		// ��ȡ�ù��ҵ��������µ�������Ʒ����
		int ReadItem(const char* country, const char* type, TextVector& text);
		// ��ȡ�ù��ҵ���������Ʒ�����µ�������Ʒ
		int ReadItems(const char* country, const char* type, const char* item, TextVector& text);
		int ReadItems(const char* country, const char* type, const char* item, ItemsVector& items);

	};

}
