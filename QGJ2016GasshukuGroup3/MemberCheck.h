#pragma once

/// @file MemberCheck.h
/// MemberCheck.h ------------------------------------------------------
/// あるクラスまたは構造体があるメンバー変数あるいはメンバー関数を
/// 持っているかをテストし、その結果から適切な処理を行えるようにします。
/// ※ 基本的にはこのファイルをいじる必要はありません。
///
/// 最終更新:
///			2016/ 7/25
/// --------------------------------------------------------------------

#include <type_traits>

/// <summary>メンバをチェックする関数を定義します。</summary>
#define CreateTestMember(FunctionName, MemberNameList)										\
	template <class TestType>																\
	static auto FunctionName(TestType&& obj) -> decltype(MemberNameList, std::true_type()); \
	template <class TestType>																\
	static std::false_type FunctionName(...)

/// <summary>単一メンバへのポインタを返します。</summary>
#define Member (MemberName)					&TestType::MemberName
/// <summary>複数メンバへのポインタを返します。</summary>
#define Members(MemberName1, MemberName2)	&TestType::MemberName1, &TestType::MemberName2
/// <summary>メンバ変数の確認結果を返します。</summary>
#define GetCheckResult(FunctionName) typename std::identity<decltype(FunctionName<Type>(std::declval<Type>()))>::type::value

template <class Type, class ArgType = void>
/**
* あるクラスまたは構造体が特定のメンバー変数あるいはメンバー関数を持っているかをテストします。
* @tparam Type メンバーをテストするクラスまたは構造体
*/
class CheckMember : public CheckMember<Type, void> {
	CreateTestMember(TestOnCollideFromSide,		obj.OnCollideFromSide(std::declval<ArgType&>(), 0, 0));
	CreateTestMember(TestOnCollideFromBottom,	obj.OnCollideFromBottom(std::declval<ArgType&>(), 0, 0));
	CreateTestMember(TestOnCollideFromTop,		obj.OnCollideFromTop(std::declval<ArgType&>(), 0, 0));
	CreateTestMember(TestOperatorFunctionCall,  obj(std::declval<ArgType>()));
public:
	static const bool HasOnCollideFromSide   = GetCheckResult(TestOnCollideFromSide);
	static const bool HasOnCollideFromBottom = GetCheckResult(TestOnCollideFromBottom);
	static const bool HasOnCollideFromTop = GetCheckResult(TestOnCollideFromTop);
	static const bool HasOperatorFunctionCall = GetCheckResult(TestOperatorFunctionCall);
};

template <class Type>
/** 
* あるクラスまたは構造体が特定のメンバー変数あるいはメンバー関数を持っているかをテストします。
* @tparam Type メンバーをテストするクラスまたは構造体
*/
class CheckMember <Type, void> {
	CreateTestMember(TestXAndY,					Members(x, y));
	CreateTestMember(TestWidthAndHeight,		Members(width, height));
	CreateTestMember(TestDxAndDy,				Members(dx, dy));
	CreateTestMember(TestEnable,				Member(Enable));
	
	CreateTestMember(TestGetCollideRectangle,	Member(GetCollideRectangle));

	CreateTestMember(TestMapTileIsEmpty,		  MapTileIsEmpty(std::declval<TestType>()));
	CreateTestMember(TestMapTileIsEmptyReference, MapTileIsEmpty(std::declval<TestType&>()));
	CreateTestMember(TestIsEmpty,				Member(IsEmpty));
public:
	static const bool HasXY					 = GetCheckResult(TestXAndY);
	static const bool HasWidthHeight		 = GetCheckResult(TestWidthAndHeight); 
	static const bool HasDxDy				 = GetCheckResult(TestDxAndDy); 
	static const bool HasEnable				 = GetCheckResult(TestEnable); 

	static const bool HasGetCollideRectangle = GetCheckResult(TestGetCollideRectangle);

	static const bool ExistMapTileIsEmpty	 = GetCheckResult(TestMapTileIsEmpty) || GetCheckResult(TestMapTileIsEmptyReference);
	static const bool HasIsEmpty			 = GetCheckResult(TestIsEmpty);
};

// 外側では使えないようにする
#undef CreateTestMemberVariable
#undef Member
#undef Members
#undef GetCheckResult

template < class Type, bool hasGetCollideRectange = CheckMember<Type>::HasGetCollideRectangle>
/** 
* 指定したクラスがメンバ関数 GetCollideRectangle を持っている場合はそれを呼び出し、
* そうでない場合はメンバ変数 x, y, width, height から Rectangle_t を作成します。
* 指定したクラスがメンバに GetCollideRectangle と x, y, width, height の両方を持っていない場合、
* コンパイルエラーが発生します。
*/
struct GetCollideRectangeCaller {
	static Rectangle_t GetCollideRectangle(Type& obj) {
		return obj.GetCollideRectangle();
	}
};

template <class Type>
/**
* @copydoc GetCollideRectangeCaller
*/
struct GetCollideRectangeCaller <Type, false> {
	static Rectangle_t GetCollideRectangle(Type& obj) {
		return Rectangle_t(static_cast<Pixel>(obj.x), static_cast<Pixel>(obj.y), static_cast<Pixel>(obj.width), static_cast<Pixel>(obj.height));
	}
};

template <class Type, 
		  bool hasGetCollideRectangle = CheckMember<Type>::HasGetCollideRectangle, 
		  bool hasxy = CheckMember<Type>::HasXY, 
		  bool haswidthheight = CheckMember<Type>::HasWidthHeight>
/**
* 指定したクラスがメンバ関数 GetCollideRectangle を持っている場合はそれを呼び出し、
* そうでない場合はメンバ変数 x, y, width, height から Rectangle_t を作成します。
* 指定したクラスがメンバに GetCollideRectangle と x, y, width, height の両方を持っていない場合、
* コンパイルエラーが発生します。
*/
struct MapTileGetCollideRectangeCaller {
	static Rectangle_t GetCollideRectangle(Type& obj, int x, int y) {
		return obj.GetCollideRectangle();
	}
};

template <class Type>
/**
* @copydoc GetCollideRectangeCaller
*/
struct MapTileGetCollideRectangeCaller <Type, false, true, true> {
	static Rectangle_t GetCollideRectangle(Type& obj, int x, int y) {
		return Rectangle_t(static_cast<Pixel>(obj.x), static_cast<Pixel>(obj.y), static_cast<Pixel>(obj.width), static_cast<Pixel>(obj.height));
	}
};

template <class Type>
/**
* @copydoc GetCollideRectangeCaller
*/
struct MapTileGetCollideRectangeCaller <Type, false, true, false> {
	static Rectangle_t GetCollideRectangle(Type& obj, int x, int y) {
		return Rectangle_t(static_cast<Pixel>(obj.x), static_cast<Pixel>(obj.y), MapTile::MapSize, MapTile::MapSize);
	}
};

template <class Type>
/**
* @copydoc GetCollideRectangeCaller
*/
struct MapTileGetCollideRectangeCaller <Type, false, false, true> {
	static Rectangle_t GetCollideRectangle(Type& obj, int x, int y) {
		return Rectangle_t(static_cast<Pixel>(x) * MapTile::MapSize, static_cast<Pixel>(y) * MapTile::MapSize, obj.width, obj.height);
	}
};

template <class Type>
/**
* @copydoc GetCollideRectangeCaller
*/
struct MapTileGetCollideRectangeCaller <Type, false, false, false> {
	static Rectangle_t GetCollideRectangle(Type& obj, int x, int y) {
		return Rectangle_t(static_cast<Pixel>(x * MapTile::MapSize), static_cast<Pixel>(y * MapTile::MapSize), static_cast<Pixel>(MapTile::MapSize), static_cast<Pixel>(MapTile::MapSize));
	}
};

template <class Type, class MapType, bool hasOnCollideFromSide = CheckMember<Type, MapType>::HasOnCollideFromSide>
///<summary> 指定したクラスがメンバ関数 OnCollideFromSide を持っている場合はそれを呼び出し、そうでない場合は何もしません。</summary>
struct OnCollideFromSideCaller {
	static bool OnCollideFromSide(Type& obj, MapType& tile, int i, int j) {
		return obj.OnCollideFromSide(tile, i, j);
	}
};

template <class Type, class MapType>
///<summary> 指定したクラスがメンバ関数 OnCollideFromSide を持っている場合はそれを呼び出し、そうでない場合は何もしません。</summary>
struct OnCollideFromSideCaller <Type, MapType, false> {
	static bool OnCollideFromSide(Type& obj, MapType& tile, int i, int j) {
		return false;
	}
};

template <class Type, class MapType, bool hasOnCollideFromBottom = CheckMember<Type, MapType>::HasOnCollideFromBottom>
/// <summary>指定したクラスがメンバ関数 OnCollideFromBottom を持っている場合はそれを呼び出し、そうでない場合は何もしません。</summary>
struct OnCollideFromBottomCaller {
	static bool OnCollideFromBottom(Type& obj, MapType& tile, int i, int j) {
		return obj.OnCollideFromBottom(tile, i, j);
	}
};

template <class Type, class MapType>
/// <summary>指定したクラスがメンバ関数 OnCollideFromBottom を持っている場合はそれを呼び出し、そうでない場合は何もしません。</summary>
struct OnCollideFromBottomCaller <Type, MapType, false> {
	static bool OnCollideFromBottom(Type& obj, MapType& tile, int i, int j) {
		return false;
	}
};

template <class Type, class MapType, bool hasOnCollideFromTop = CheckMember<Type, MapType>::HasOnCollideFromTop>
/// <summary>指定したクラスがメンバ関数 OnCollideFromTop を持っている場合はそれを呼び出し、そうでない場合は何もしません。</summary>
struct OnCollideFromTopCaller {
	static bool OnCollideFromTop(Type& obj, MapType& tile, int i, int j) {
		return obj.OnCollideFromTop(tile, i, j);
	}
};

template <class Type, class MapType>
/// <summary>指定したクラスがメンバ関数 OnCollideFromTop を持っている場合はそれを呼び出し、そうでない場合は何もしません。</summary>
struct OnCollideFromTopCaller <Type, MapType, false> {
	static bool OnCollideFromTop(Type& obj, MapType& tile, int i, int j) {
#pragma message(__FILE__": warning : Type の OnCollideFromTop")
		return obj.OnCollideFromTop(tile, i, j);
	}
};

template <class Type, bool = CheckMember<Type>::ExistMapTileIsEmpty>
struct MapTileIsEmptyCaller {
	static bool MapTileIsEmptyProxy(Type& obj) {
		return MapTileIsEmpty(obj);
	}
};

template <class Type>
struct MapTileIsEmptyCaller <Type, false> {
	static bool MapTileIsEmptyProxy(Type& obj) {
		return false;
	}
};

template <class Type, bool = CheckMember<Type>::HasEnable>
struct EnableGetter {
	static bool Enable(Type& obj) {
		return obj.Enable;
	}
};

template <class Type>
struct EnableGetter <Type, false> {
	static bool Enable(Type& obj) {
		return false;
	}
};

template <class Type, bool = CheckMember<Type>::HasIsEmpty>
struct IsEmptyCaller {
	static bool IsEmpty(Type& obj) {
		return obj.IsEmpty();
	}
};

template <class Type>
struct IsEmptyCaller <Type, false> {
	static bool IsEmpty(Type& obj) {
		return false;
	}
};