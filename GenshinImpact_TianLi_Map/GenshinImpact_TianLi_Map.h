#pragma once
struct MapBlock 
{
	
};

//
//class object
//{
//public:
//	double x;
//	double y;
//	int id;
//	
//};
//class objects
//{
//public:
//	objects filte(double x, double y, double r = 200);
//	
//	object& at(int index);
//	int size();
//	
//	class iterator
//	{
//	public:
//		iterator(objects* objects, int index)
//		{
//			this->objects = objects;
//			this->index = index;
//		}
//		iterator& operator++()
//		{
//			index++;
//			return *this;
//		}
//		iterator operator++(int)
//		{
//			iterator tmp = *this;
//			index++;
//			return tmp;
//		}
//		bool operator==(const iterator& rhs)
//		{
//			return index == rhs.index;
//		}
//		bool operator!=(const iterator& rhs)
//		{
//			return index != rhs.index;
//		}
//		object& operator*()
//		{
//			return objects->at(index);
//		}
//		object* operator->()
//		{
//			return &objects->at(index);
//		}
//	private:
//		objects* objects;
//		int index;
//	};
//	// 实现for迭代
//	objects::iterator begin()
//	{
//		return objects::iterator(this, 0);
//	}
//	objects::iterator end()
//	{
//		return objects::iterator(this, this->size());
//	}
//	
//};
namespace TianLi
{
	class object
	{
	public:
		double x;
		double y;
		int id;
	};
	class objects
	{
		object* _objects;
		int _size;
	public:
		objects& filte(double x, double y, double r)
		{
			return *this;
		}
		object& at(int index)
		{
			return _objects[index];
		}
		int size()
		{
			return _size;
		}
		class iterator
		{
		public:
			iterator(objects* objects, int index)
			{
				this->objects = objects;
				this->index = index;
			}
			iterator& operator++()
			{
				index++;
				return *this;
			}
			iterator operator++(int)
			{
				iterator tmp = *this;
				index++;
				return tmp;
			}
			bool operator==(const iterator& rhs)
			{
				return index == rhs.index;
			}
			bool operator!=(const iterator& rhs)
			{
				return index != rhs.index;
			}
			object& operator*()
			{
				return objects->at(index);
			}
			object* operator->()
			{
				return &objects->at(index);
			}
		private:
			objects* objects;
			int index;
		};
		// 实现for迭代
		objects::iterator begin()
		{
			return objects::iterator(this, 0);
		}
		objects::iterator end()
		{
			return objects::iterator(this, this->size());
		}
	};
	
}

class GenshinImpact_TianLi_Map
{
//	int X = 0;
//	int Y = 0;
//	int x0 = 0;
//	int y0 = 0;
//	int x1 = 0;
//	int y1 = 0;
//	int dx = 0;
//	int dy = 0;
//public:
//
//	Point p0 = Point(0, 0);
//	Point p1 = Point(0, 0);
//	double value = 0.0;
//	double scale = 1.0;
//
//	//左键单击标志
//	bool bLCD = false;
//	bool bLCU = false;
//	//中键单击
//	bool bMCD = false;
//	bool bMCU = false;
//	Point2d zerosMinMap;
//	Point offGiMinMap;
//	void setMouseLeftDownPos(int x, int y);
//	void setMouseLeftUpPos(int x, int y);
//	void setMouseLeftMovePos(int x, int y);
//
//	void setMouseMidDownPos(int x, int y);
//	void setMouseMidUpPos(int x, int y);
//	void setMouseMidMovePos(int x, int y);
//
//	void normalizationZerosMinMap(Rect rangeRect);
	TianLi::objects& search(const char* name, double x = 0, double y = 0, double r = 0);
};

