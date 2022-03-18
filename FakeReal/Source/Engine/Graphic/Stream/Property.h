#pragma once
#include "../Object.h"
#include "Rtti.h"
#include "Stream.h"
#include <unordered_map>
#include <assert.h>
namespace FakeReal
{
	template < class T > inline bool operator < (const T& v1, const T& v2)
	{
		return false;
	}
	template < class T > inline bool operator > (const T& v1, const T& v2)
	{
		return false;
	}

	class Property
	{
	public:
		enum class PropertyType
		{
			Enum,
			Data,//数据的指针
			VArray,//vector
			Map,//map,unordered_map...
			Value
		};

		enum //属性标记
		{
			F_SAVE_LOAD = 0x01,
			F_COPY = 0x02,
			F_CLONE = 0x04,
			F_SAVE_LOAD_COPY = 0x03,
			F_SAVE_LOAD_CLONE = 0x05,
			F_REFLECT_NAME = 0x08,
			F_MAX
		};
	public:
		Property(Rtti* pOwner, const std::string& name, unsigned int uiOffset, unsigned int uiFlag)
		{
			m_pOwner = pOwner;
			mName = name;
			m_uiOffset = uiOffset;
			m_uiFlag = uiFlag;
		}
		Property()
		{
			m_pOwner = nullptr;
			mName = "";
			m_uiOffset = 0;
			m_uiFlag = 0;
		}
		virtual ~Property()
		{

		}

		virtual void* GetValueAddress(const void* pObj) const
		{
			return (void*)(((unsigned char*)pObj) + m_uiOffset);
		}

		std::string& GetName() { return mName; }

		Rtti* GetRtti() const
		{
			return m_pOwner;
		}

		void SetOwner(Rtti* pOwner)
		{
			m_pOwner = pOwner;
		}
		unsigned int GetFlag()const
		{
			return m_uiFlag;
		}
		void SetFlag(unsigned int uiFlag)
		{
			m_uiFlag = uiFlag;
		}

	public:
		virtual PropertyType GetType() const = 0;
		virtual bool Archive(Stream& stream, void* pObj) = 0;
		virtual void CloneData(void* pSrcObj, void* pDestObj, std::unordered_map<Object*, Object*>& CloneMap) = 0;
		virtual void CopyData(void* pSrcObj, void* pDestObj) = 0;
	protected:
		Rtti* m_pOwner;
		std::string mName;
		unsigned int m_uiOffset;//属性地址偏移量
		unsigned int m_uiFlag;//属性标记
	};

	//枚举类型
	template<class T>
	class EnumProperty : public Property
	{
	public:
		EnumProperty()
		{

		}
		EnumProperty(
			Rtti* pOwner,
			const std::string& name,
			const std::string& enumName,
			unsigned int uiOffset,
			unsigned int uiFlag
		)
			: Property(pOwner, name, uiOffset, uiFlag)
		{
			mEnumName = enumName;
		}
		virtual ~EnumProperty()
		{
		}
		virtual bool SetValue(void* pObj, unsigned int data) const
		{
			*(unsigned int*)(((unsigned char*)pObj) + m_uiOffset) = data;
			return true;
		}

		virtual bool GetValue(void* pObj, unsigned int& outData) const
		{
			outData = *(unsigned int*)(((unsigned char*)pObj) + m_uiOffset);
			return true;
		}
		virtual bool GetValue(const void* pObj, unsigned int& outData) const
		{
			outData = *(const unsigned int*)(((const char*)pObj) + m_uiOffset);
			return true;
		}
		virtual unsigned int& Value(void* pObj) const
		{
			return *(unsigned int*)(((const char*)pObj) + m_uiOffset);
		}
		virtual Property* GetInstance()
		{
			return new EnumProperty();
		}

		const std::string& GetEnumName() const { return mEnumName; }

	public:
		virtual PropertyType GetType() const
		{
			return PropertyType::Enum;
		}
		virtual bool Archive(Stream& stream, void* pObj)
		{
			stream.Archive(Value(pObj));
			return true;
		}

		virtual void CloneData(void* pSrcObj, void* pDestObj, std::unordered_map<Object*, Object*>& CloneMap)
		{
			Value(pDestObj) = Value(pSrcObj);
		}
		virtual void CopyData(void* pSrcObj, void* pDestObj)
		{
			Value(pDestObj) = Value(pSrcObj);
		}
	private:
		std::string mEnumName;
	};

	//数据块指针类型
	template<class T, typename NumType>
	class DataProperty : public Property
	{
	public:
		DataProperty()
		{

		}
		DataProperty(
			Rtti* pOwner,
			const std::string& name,
			unsigned int uiOffset,
			unsigned int uiDataNum,
			bool isDynamicCreate
		)
			: Property(pOwner, name, uiOffset, Property::F_SAVE_LOAD_CLONE)
		{
			m_uiDataNum = uiDataNum;
			m_bIsDynamicCreate = isDynamicCreate;
		}
		DataProperty(
			Rtti* pOwner,
			const std::string& name,
			unsigned int uiOffset,
			unsigned int uiNumOffset
		)
			: Property(pOwner, name, uiOffset, Property::F_SAVE_LOAD_CLONE)
		{
			m_uiDataNum = 0;
			m_uiNumOffset = uiNumOffset;
			m_bIsDynamicCreate = true;
		}
		virtual ~DataProperty()
		{

		}
		virtual Property* GetInstance()
		{
			return new DataProperty<T, NumType>();
		}

	public:
		virtual PropertyType GetType() const
		{
			return PropertyType::Data;
		}

		virtual bool Archive(Stream &Stream, void* pObj)
		{
			unsigned int uiStreamFlag = Stream.GetStreamFlag();
			if (uiStreamFlag == Stream::AT_SAVE)
			{
				T * ValueAddres = *(T**)GetValueAddress(pObj);

				if (m_uiDataNum > 0)
				{
					Stream.Write(ValueAddres, m_uiDataNum * sizeof(T));
				}
				else
				{
					void * NumOffSet = (void*)(((unsigned char*)pObj) + m_uiNumOffset);
					NumType uiNum = *(NumType*)NumOffSet;
					Stream.Write(NumOffSet, sizeof(NumType));
					Stream.Write(ValueAddres, uiNum * sizeof(T));
				}
			}
			else if (uiStreamFlag == Stream::AT_LOAD)
			{
				T** Temp = (T**)GetValueAddress(pObj);
				if (m_uiDataNum > 0)
				{

					if (m_bIsDynamicCreate)
					{
						*Temp = new T[m_uiDataNum];
						Stream.Read((void *)(*Temp), m_uiDataNum * sizeof(T));

					}
					else
					{
						Stream.Read((void *)(*Temp), m_uiDataNum * sizeof(T));
					}

				}
				else
				{
					void * NumOffSet = (void*)(((unsigned char*)pObj) + m_uiNumOffset);
					Stream.Read(NumOffSet, sizeof(NumType));
					NumType uiNum = *(NumType*)NumOffSet;
					*Temp = new T[uiNum];
					Stream.Read((void *)(*Temp), uiNum * sizeof(T));
				}
			}
			else if (uiStreamFlag == Stream::AT_SIZE)
			{
				if (m_uiDataNum > 0)
				{
					Stream.AddBufferSize(m_uiDataNum * sizeof(T));
				}
				else
				{
					void * NumOffSet = (void*)(((unsigned char*)pObj) + m_uiNumOffset);
					NumType uiNum = *(NumType*)NumOffSet;
					Stream.AddBufferSize(sizeof(NumType));
					Stream.AddBufferSize(uiNum * sizeof(T));
				}
			}
			return true;
		}

		virtual void CloneData(void* pSrcObj, void* pDestObj, std::unordered_map<Object*, Object*>& CloneMap)
		{
			T * SrcValueAddres = *(T**)GetValueAddress(pSrcObj);

			if (m_uiDataNum > 0)
			{
				T** Temp = (T**)GetValueAddress(pDestObj);
				if (m_bIsDynamicCreate)
				{
					*Temp = new T[m_uiDataNum];
					memcpy_s((void*)(*Temp), m_uiDataNum * sizeof(T), (void*)SrcValueAddres, m_uiDataNum * sizeof(T));
				}
				else
				{
					memcpy_s((void*)(*Temp), m_uiDataNum * sizeof(T), (void*)SrcValueAddres, m_uiDataNum * sizeof(T));
				}
			}
			else
			{
				T** Temp = (T**)GetValueAddress(pDestObj);

				void * SrcNumOffSet = (void*)(((unsigned char*)pSrcObj) + m_uiNumOffset);
				void * DestNumOffSet = (void*)(((unsigned char*)pDestObj) + m_uiNumOffset);
				*(NumType*)DestNumOffSet = *(NumType*)SrcNumOffSet;
				NumType uiNum = *(NumType*)SrcNumOffSet;

				*Temp = new T[uiNum];
				memcpy_s((void*)(*Temp), uiNum * sizeof(T), (void*)SrcValueAddres, uiNum * sizeof(T));
			}
		}
		virtual void CopyData(void* pSrcObj, void* pDestObj)
		{
			assert(0);
		}
	private:
		unsigned int m_uiDataNum;//当不为0时，表示数据的长度
		unsigned int m_uiNumOffset;//属性的偏移量
		bool m_bIsDynamicCreate;//是否为动态创建的数据（不定长数组，指针表示的定长数组）
	};

	//范围限制的属性的基类
	template<class T>
	class RangeProperty : public Property
	{
	public:
		RangeProperty()
		{
		}
		RangeProperty(Rtti* pOwner, const std::string& name, unsigned int uiOffset, unsigned int uiFlag,
			bool isRange = false, T low = T(), T high = T(), T step = T())
			: Property(pOwner, name, uiOffset, uiFlag)
		{
			mLowRange = low;
			mHighRange = high;
			mStep = step;
			m_bRange = isRange;
		}
		virtual ~RangeProperty()
		{

		}
	protected:
		T mLowRange;
		T mHighRange;
		T mStep;
		bool m_bRange;
	};

	template<class T>
	class ValueProperty : public RangeProperty<T>
	{
	public:
		ValueProperty()
		{

		}
		ValueProperty(Rtti* pOwner, const std::string& name, unsigned int uiOffset, unsigned int uiFlag,
			bool isRange = false, T low = T(), T high = T(), T step = T())
			: RangeProperty<T>(pOwner, name, uiOffset, uiFlag, isRange, low, high, step)
		{

		}
		virtual ~ValueProperty()
		{

		}
		virtual bool SetValue(void* pObj, T& data) const
		{
			//若构造的类为模板类，那么派生类不可以直接使用继承到的基类数据和方法，需要通过this指针使用。
			//否则，在使用一些较新的编译器时，会报“找不到标识符”错误。
			if (data > this->mHighRange || data < this->mLowRange)
			{
				return false;
			}
			*(T*)(((unsigned char*)pObj) + this->m_uiOffset) = data;
			return true;
		}

		virtual bool GetValue(void* pObj, T& outData) const
		{
			outData = *(T*)(((unsigned char*)pObj) + this->m_uiOffset);
			return true;
		}
		virtual bool GetValue(const void* pObj, T& outData) const
		{
			outData = *(const T*)(((const char*)pObj) + this->m_uiOffset);
			return true;
		}
		virtual T& Value(void* pObj) const
		{
			return *(T*)(((const char*)pObj) + this->m_uiOffset);
		}
		virtual Property* GetInstance()
		{
			return new ValueProperty<T>();
		}

	public:
		virtual Property::PropertyType GetType() const
		{
			return Property::PropertyType::Value;
		}

		virtual bool Archive(Stream& stream, void* pObj)
		{
			stream.Archive(Value(pObj));
			return true;
		}

		virtual void CloneData(void* pSrcObj, void* pDestObj, std::unordered_map<Object*, Object*>& CloneMap)
		{
			Copy(Value(pDestObj), Value(pSrcObj), CloneMap);
		}
		virtual void CopyData(void* pSrcObj, void* pDestObj)
		{
			Value(pDestObj) = Value(pSrcObj);
		}
	};

	//vector类型属性
	template<class VArrayType, typename T>
	class VArrayProperty : public RangeProperty<T>
	{
	public:
		VArrayProperty()
		{

		}
		VArrayProperty(Rtti* pOwner, const std::string& name, unsigned int uiOffset, unsigned int uiFlag,
			bool isRange = false, T low = T(), T high = T(), T step = T())
			: RangeProperty<T>(pOwner, name, uiOffset, uiFlag, isRange, low, high, step)
		{

		}
		virtual ~VArrayProperty()
		{

		}
		VArrayType& GetContainer(void* pObj) const
		{
			return (*(VArrayType*)(((unsigned char*)pObj) + this->m_uiOffset));
		}
		bool AddElement(void* pObj, unsigned int uiIndex, T& e)
		{
			GetContainer(pObj).emplace_back(e);
			return true;
		}
		bool Erase(void* pObj, T& e)
		{
			VArrayType& container = GetContainer(pObj);
			auto itr = std::find(container.begin(), container.end(), e);
			if (itr != container.end())
			{
				container.erase(itr);
				return true;
			}
			return false;
		}
		virtual bool SetValue(void* pObj, size_t uiIndex, T& vlue)
		{
			if (vlue < this->mLowRange || vlue > this->mHighRange)
			{
				return false;
			}
			VArrayType& container = GetContainer(pObj);
			if (uiIndex < 0 || uiIndex >= container.size())
			{
				return false;
			}
			container[uiIndex] = vlue;
			return true;
		}
		virtual bool GetValue(void* pObj, size_t uiIndex, T& value) const
		{
			VArrayType& container = GetContainer(pObj);
			if (uiIndex < 0 || uiIndex >= container.size())
			{
				return false;
			}
			value = container[uiIndex];
			return true;
		}
		virtual Property* GetInstance()
		{
			return new VArrayProperty<VArrayType, T>();
		}

	public:
		virtual Property::PropertyType GetType() const
		{
			return Property::PropertyType::VArray;
		}

		virtual bool Archive(Stream& stream, void* pObj)
		{
			stream.Archive(GetContainer(pObj));
			return true;
		}

		virtual void CloneData(void* pSrcObj, void* pDestObj, std::unordered_map<Object*, Object*>& CloneMap)
		{
			Copy(GetContainer(pDestObj), GetContainer(pSrcObj), CloneMap);
		}
		virtual void CopyData(void* pSrcObj, void* pDestObj)
		{
			GetContainer(pDestObj) = GetContainer(pSrcObj);
		}
	};

	//UnMap类型属性
	template<class MapType, typename KEY, typename VALUE>
	class MapProperty : public RangeProperty<VALUE>
	{
	public:
		MapProperty()
		{

		}
		MapProperty(Rtti* pOwner, const std::string& name, unsigned int uiOffset, unsigned int uiFlag,
			bool isRange = false, VALUE low = VALUE(), VALUE high = VALUE(), VALUE step = VALUE())
			: RangeProperty<VALUE>(pOwner, name, uiOffset, uiFlag, isRange, low, high, step)
		{

		}
		virtual ~MapProperty()
		{

		}
		MapType& GetContainer(void* pObj) const
		{
			return (*(MapType*)(((unsigned char*)pObj) + this->m_uiOffset));
		}
		bool AddElement(void* pObj, KEY& key, VALUE& value)
		{
			GetContainer(pObj).emplace(key, value);
			return true;
		}
		bool Erase(void* pObj, KEY& key)
		{
			MapType& container = GetContainer(pObj);
			auto itr = container.find(key);
			if (itr != container.end())
			{
				container.erase(itr);
				return true;
			}
			return false;
		}
		virtual bool SetValue(void* pObj, KEY& key, VALUE& value)
		{
			if (value < this->mLowRange || value > this->mHighRange)
			{
				return false;
			}
			MapType& container = GetContainer(pObj);
			container[key] = value;
			return true;
		}
		virtual bool GetValue(void* pObj, KEY& key, VALUE& value) const
		{
			MapType& container = GetContainer(pObj);
			auto itr = container.find(key);
			if (itr != container.end())
			{
				value = itr->second;
				return true;
			}
			return false;
		}
		virtual Property* GetInstance()
		{
			return new MapProperty<MapType, KEY, VALUE>();
		}

	public:
		virtual Property::PropertyType GetType() const
		{
			return Property::PropertyType::Map;
		}

		virtual bool Archive(Stream& stream, void* pObj)
		{
			stream.Archive(GetContainer(pObj));
			return true;
		}

		virtual void CloneData(void* pSrcObj, void* pDestObj, std::unordered_map<Object*, Object*>& CloneMap)
		{
			Copy(GetContainer(pDestObj), GetContainer(pSrcObj), CloneMap);
		}
		virtual void CopyData(void* pSrcObj, void* pDestObj)
		{
			GetContainer(pDestObj) = GetContainer(pSrcObj);
		}
	};

	/*****************************************************************************************************/

	//构造器
	template<class T, typename NumType>
	struct DataPropertyCreator
	{
		Property* CreateProperty(
			Rtti* pOwner,
			std::string& name,
			unsigned int uiOffset,
			unsigned int uiDataNum,
			bool isDynamicCreate
		)
		{
			return new DataProperty<T, NumType>(pOwner, name, uiOffset, uiDataNum, isDynamicCreate);
		}

		Property* CreateProperty(
			Rtti* pOwner,
			const std::string& name,
			unsigned int uiOffset,
			unsigned int uiNumOffset
		)
		{
			return new DataProperty<T, NumType>(pOwner, name, uiOffset, uiNumOffset);
		}
	};

	template<class T>
	struct AutoPropertyCreator
	{
		Property* CreateProperty(Rtti* pOwner, const std::string& name, unsigned int uiOffset, unsigned int uiFlag)
		{
			return new ValueProperty<T>(pOwner, name, uiOffset, uiFlag);
		}
		Property* CreateProperty(Rtti* pOwner, const std::string& name, unsigned int uiOffset, unsigned int uiFlag,
			T low, T high, T step)
		{
			return new ValueProperty<T>(pOwner, name, uiOffset, uiFlag, true, low, high, step);
		}
	};

	template<class T>
	struct AutoPropertyCreator<std::vector<T>>
	{
		Property* CreateProperty(Rtti* pOwner, const std::string& name, unsigned int uiOffset, unsigned int uiFlag)
		{
			return new VArrayProperty<std::vector<T>, T>(pOwner, name, uiOffset, uiFlag);
		}
		Property* CreateProperty(Rtti* pOwner, const std::string& name, unsigned int uiOffset, unsigned int uiFlag,
			T low, T high, T step)
		{
			return new VArrayProperty<std::vector<T>, T>(pOwner, name, uiOffset, uiFlag, true, low, high, step);
		}
	};

	template<typename KEY, typename VALUE>
	struct AutoPropertyCreator<std::unordered_map<KEY, VALUE>>
	{
		Property* CreateProperty(Rtti* pOwner, const std::string& name, unsigned int uiOffset, unsigned int uiFlag)
		{
			return new MapProperty<std::unordered_map<KEY, VALUE>, KEY, VALUE>(pOwner, name, uiOffset, uiFlag);
		}
		Property* CreateProperty(Rtti* pOwner, const std::string& name, unsigned int uiOffset, unsigned int uiFlag,
			VALUE low, VALUE high, VALUE step)
		{
			return new MapProperty<std::unordered_map<KEY, VALUE>, KEY, VALUE>(pOwner, name, uiOffset, uiFlag, true, low, high, step);
		}
	};


	//构造器管理类
	class PropertyCreator
	{
	public:
		template<class ValueType, class NumType>
		static DataPropertyCreator<ValueType, NumType>& GetDataPropertyCreator(
			ValueType* & valueTypeRef,
			NumType& numTypeRef
		)
		{
			static DataPropertyCreator<ValueType, NumType> dpc;
			return dpc;
		}

		template<class ValueType>
		static AutoPropertyCreator<ValueType>& GetAutoPropertyCreator(ValueType& valueTypeRef)
		{
			static AutoPropertyCreator<ValueType> apc;
			return apc;
		}

		template<class ValueType>
		static Property* CreateEnumProperty(ValueType& valueTypeRef, Rtti* pOwner, const std::string& name, const std::string& enumName,
			unsigned int uiOffset, unsigned int uiFlag)
		{
			return new EnumProperty<ValueType>(pOwner, name, enumName, uiOffset, uiFlag);
		}
	};

#define BEGIN_ADD_ROOT_PROPERTY(classname)\
	bool classname::TerminalProperty()\
	{\
		ms_Type.ClearProperty();\
		return true;\
	}\
	bool classname::InitialProperty(Rtti* pRtti)\
	{\
		classname* ptr = nullptr;\
		Property* activeProperty = nullptr;\
		if (!pRtti)\
		{\
			pRtti = &ms_Type;\
		}

#define BEGIN_ADD_PROPERTY(classname, baseclassname)\
	bool classname::TerminalProperty()\
	{\
		ms_Type.ClearProperty();\
		return true;\
	}\
	bool classname::InitialProperty(Rtti* pRtti)\
	{\
		classname* ptr = nullptr;\
		Property* activeProperty = nullptr;\
		Rtti* pRttiTemp = pRtti;\
		if (!pRtti)\
		{\
			pRtti = &ms_Type;\
		}\
		baseclassname::InitialProperty(pRtti);

#define END_ADD_PROPERTY\
		return true;\
	}

#define REGISTER_PROPERTY(varName, reflectName, propertyFlag)\
	activeProperty = PropertyCreator::GetAutoPropertyCreator(ptr->varName).CreateProperty(\
			pRtti, reflectName, (size_t)((char*)&(ptr->varName) - (char*)ptr), propertyFlag);\
	assert(activeProperty);\
	pRtti->AddProperty(activeProperty);

#define REGISTER_PROPERTY_RANGE(varName, reflectName, propertyFlag, lowValue, highValue, stepValue)\
	activeProperty = PropertyCreator::GetAutoPropertyCreator(ptr->varName).CreateProperty(\
			pRtti, reflectName, (size_t)((char*)&(ptr->varName) - (char*)ptr), propertyFlag, lowValue, highValue, stepValue);\
	assert(activeProperty);\
	pRtti->AddProperty(activeProperty);

#define REGISTER_DATA_PROPERTY(varName, varNumName, reflectName)\
	activeProperty = PropertyCreator::GetDataPropertyCreator(ptr->varName, ptr->varNumName).CreateProperty(\
		pRtti, reflectName, (unsigned int)((char*)&(ptr->varName) - (char*)ptr), (unsigned int)((char*)&(ptr->varNumName) - (char*)ptr));\
	assert(activeProperty);\
	pRtti->AddProperty(activeProperty);

#define REGISTER_FIXED_DATA_PROPERTY(varName, Num, reflectName, isDynamicCreate)\
	unsigned int NumType = 0;\
	activeProperty = PropertyCreator::GetDataPropertyCreator(ptr->varName, ptr->NumType).CreateProperty(\
		pRtti, reflectName, (size_t)((char*)&(ptr->varName) - (char*)ptr), Num, isDynamicCreate);\
	assert(activeProperty);\
	pRtti->AddProperty(activeProperty);

#define REGISTER_ENUM_PROPERTY(varName, enumName, reflectName, propertyFlag)\
	{\
		activeProperty = PropertyCreator::CreateEnumProperty(\
			ptr->varName, pRtti, reflectName, enumName, (size_t)((char*)&(ptr->varName) - (char*)ptr), propertyFlag);\
		pRtti->AddProperty(activeProperty);\
	}

#define BEGIN_ADD_ENUM \
	if(!pRttiTemp) \
	{

#define END_ADD_ENUM  }

}