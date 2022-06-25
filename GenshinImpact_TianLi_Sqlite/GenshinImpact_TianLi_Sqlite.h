#pragma once
// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方式。此 DLL 中的所有文件都是用命令行上定义的 GENSHINIMPACT_TIANLI_SQLITE_DLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// GENSHINIMPACT_TIANLI_SQLITE_DLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
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

		// 读取国家地区
		int ReadCountry(TextVector& text);
		// 读取该国家地区下的所有类型
		int ReadType(const char* country, TextVector& text);
		// 读取该国家地区类型下的所有物品种类
		int ReadItem(const char* country, const char* type, TextVector& text);
		// 读取该国家地区类型物品种类下的所有物品
		int ReadItems(const char* country, const char* type, const char* item, TextVector& text);
		int ReadItems(const char* country, const char* type, const char* item, ItemsVector& items);

	};

}
