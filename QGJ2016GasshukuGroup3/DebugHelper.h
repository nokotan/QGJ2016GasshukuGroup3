#pragma once

/// @file DebugHelper.h
/// DebugHelper.h --------------------
/// デバッグ補助関数と列挙体宣言マクロ
/// ----------------------------------

// デバッグ補助関数 ------------

#ifdef _DEBUG
	/// <summary>ファイルと行番号を追加して標準出力に文字列を出力します。</summary>
	#define DebugTrace(format, ...) {	\
		printf(format, __VA_ARGS__);	\
		printf("\t場所: %s(%d) \n\t%s\n\n", __FILE__, __LINE__, __FUNCSIG__); \
	}
	/// <summary>条件式が偽のとき、ファイルと行番号を追加して標準出力に文字列を出力します。</summary>
	#define AssertTrace(condition, format, ...) {	\
		if (!(condition))								\
			DebugTrace(format, __VA_ARGS__);		\
	}
#else
	/// <summary>ファイルと行番号を追加して標準出力に文字列を出力します。</summary>
	#define DebugTrace(format, ...)
	/// <summary>条件式が真のとき、ファイルと行番号を追加して標準出力に文字列を出力します。</summary>
	#define AssertTrace(condition, format, ...)
#endif

/// <summary>変数の名前を取得します。</summary>
#define GetVariableName(x) #x
/// <summary>マクロ変数の値を表す文字列を取得します。</summary>
#define GetMacroValue(x) GetVariableName(x)




// enumdef ------------------------------------------------------------
// 列挙体 name を作成し、その要素数を格納する name_Count も定義します。
// --------------------------------------------------------------------

template<size_t size>
/// <summary>列挙体を宣言する文字列の要素数を取得します。</summary>
/// <param name="str">列挙体を定義する文字列</param>
int CountComma(const char (&str)[size]) {
	int count = 0;

	for (int i = 0; str[i] != '\0' && i < size; i++) {
		if (str[i] == ',' || str[i + 1] == '\0')
			count++;
	}

	return count;
}

/// <summary>列挙体 name を作成し、その要素数を格納する name_Count も定義します。</summary>
#define enumdef(name, ...)		\
	enum name { __VA_ARGS__ };	\
	const int name##_Count = CountComma(#__VA_ARGS__);

/// <summary>クラス name 内に列挙体を作成し、その要素数を格納する Count も定義します</summary>
#define enumclassdef(name, ...)		\
	class name {					\
		public:						\
		enum Enum { __VA_ARGS__ };	\
		static const int Count;		\
	};								\
	const int name::Count = CountComma(#__VA_ARGS__);