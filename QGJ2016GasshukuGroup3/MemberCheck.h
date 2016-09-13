#pragma once

/// @file MemberCheck.h
/// MemberCheck.h ------------------------------------------------------
/// ����N���X�܂��͍\���̂����郁���o�[�ϐ����邢�̓����o�[�֐���
/// �����Ă��邩���e�X�g���A���̌��ʂ���K�؂ȏ������s����悤�ɂ��܂��B
/// �� ��{�I�ɂ͂��̃t�@�C����������K�v�͂���܂���B
///
/// �ŏI�X�V:
///			2016/ 7/25
/// --------------------------------------------------------------------

#include <type_traits>

/// <summary>�����o���`�F�b�N����֐����`���܂��B</summary>
#define CreateTestMember(FunctionName, MemberNameList)										\
	template <class TestType>																\
	static auto FunctionName(TestType&& obj) -> decltype(MemberNameList, std::true_type()); \
	template <class TestType>																\
	static std::false_type FunctionName(...)

/// <summary>�P�ꃁ���o�ւ̃|�C���^��Ԃ��܂��B</summary>
#define Member (MemberName)					&TestType::MemberName
/// <summary>���������o�ւ̃|�C���^��Ԃ��܂��B</summary>
#define Members(MemberName1, MemberName2)	&TestType::MemberName1, &TestType::MemberName2
/// <summary>�����o�ϐ��̊m�F���ʂ�Ԃ��܂��B</summary>
#define GetCheckResult(FunctionName) typename std::identity<decltype(FunctionName<Type>(std::declval<Type>()))>::type::value

template <class Type, class ArgType = void>
/**
* ����N���X�܂��͍\���̂�����̃����o�[�ϐ����邢�̓����o�[�֐��������Ă��邩���e�X�g���܂��B
* @tparam Type �����o�[���e�X�g����N���X�܂��͍\����
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
* ����N���X�܂��͍\���̂�����̃����o�[�ϐ����邢�̓����o�[�֐��������Ă��邩���e�X�g���܂��B
* @tparam Type �����o�[���e�X�g����N���X�܂��͍\����
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

// �O���ł͎g���Ȃ��悤�ɂ���
#undef CreateTestMemberVariable
#undef Member
#undef Members
#undef GetCheckResult

template < class Type, bool hasGetCollideRectange = CheckMember<Type>::HasGetCollideRectangle>
/** 
* �w�肵���N���X�������o�֐� GetCollideRectangle �������Ă���ꍇ�͂�����Ăяo���A
* �����łȂ��ꍇ�̓����o�ϐ� x, y, width, height ���� Rectangle_t ���쐬���܂��B
* �w�肵���N���X�������o�� GetCollideRectangle �� x, y, width, height �̗����������Ă��Ȃ��ꍇ�A
* �R���p�C���G���[���������܂��B
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
* �w�肵���N���X�������o�֐� GetCollideRectangle �������Ă���ꍇ�͂�����Ăяo���A
* �����łȂ��ꍇ�̓����o�ϐ� x, y, width, height ���� Rectangle_t ���쐬���܂��B
* �w�肵���N���X�������o�� GetCollideRectangle �� x, y, width, height �̗����������Ă��Ȃ��ꍇ�A
* �R���p�C���G���[���������܂��B
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
///<summary> �w�肵���N���X�������o�֐� OnCollideFromSide �������Ă���ꍇ�͂�����Ăяo���A�����łȂ��ꍇ�͉������܂���B</summary>
struct OnCollideFromSideCaller {
	static bool OnCollideFromSide(Type& obj, MapType& tile, int i, int j) {
		return obj.OnCollideFromSide(tile, i, j);
	}
};

template <class Type, class MapType>
///<summary> �w�肵���N���X�������o�֐� OnCollideFromSide �������Ă���ꍇ�͂�����Ăяo���A�����łȂ��ꍇ�͉������܂���B</summary>
struct OnCollideFromSideCaller <Type, MapType, false> {
	static bool OnCollideFromSide(Type& obj, MapType& tile, int i, int j) {
		return false;
	}
};

template <class Type, class MapType, bool hasOnCollideFromBottom = CheckMember<Type, MapType>::HasOnCollideFromBottom>
/// <summary>�w�肵���N���X�������o�֐� OnCollideFromBottom �������Ă���ꍇ�͂�����Ăяo���A�����łȂ��ꍇ�͉������܂���B</summary>
struct OnCollideFromBottomCaller {
	static bool OnCollideFromBottom(Type& obj, MapType& tile, int i, int j) {
		return obj.OnCollideFromBottom(tile, i, j);
	}
};

template <class Type, class MapType>
/// <summary>�w�肵���N���X�������o�֐� OnCollideFromBottom �������Ă���ꍇ�͂�����Ăяo���A�����łȂ��ꍇ�͉������܂���B</summary>
struct OnCollideFromBottomCaller <Type, MapType, false> {
	static bool OnCollideFromBottom(Type& obj, MapType& tile, int i, int j) {
		return false;
	}
};

template <class Type, class MapType, bool hasOnCollideFromTop = CheckMember<Type, MapType>::HasOnCollideFromTop>
/// <summary>�w�肵���N���X�������o�֐� OnCollideFromTop �������Ă���ꍇ�͂�����Ăяo���A�����łȂ��ꍇ�͉������܂���B</summary>
struct OnCollideFromTopCaller {
	static bool OnCollideFromTop(Type& obj, MapType& tile, int i, int j) {
		return obj.OnCollideFromTop(tile, i, j);
	}
};

template <class Type, class MapType>
/// <summary>�w�肵���N���X�������o�֐� OnCollideFromTop �������Ă���ꍇ�͂�����Ăяo���A�����łȂ��ꍇ�͉������܂���B</summary>
struct OnCollideFromTopCaller <Type, MapType, false> {
	static bool OnCollideFromTop(Type& obj, MapType& tile, int i, int j) {
#pragma message(__FILE__": warning : Type �� OnCollideFromTop")
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