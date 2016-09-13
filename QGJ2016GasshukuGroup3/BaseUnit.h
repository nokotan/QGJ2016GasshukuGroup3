#pragma once

/// @file BaseUnit.h 
/// BaseUnit.h -------------------------------
/// ゲーム中に使うオブジェクト（の基底クラス）
/// 最終更新:
///			2016/08/14
/// ------------------------------------------

#include "Shape.h"

/// <summary>マップタイルの設定</summary>
struct MapTile {
	// マップタイルとして使う構造体（またはクラス）をここに書いてください。
	// 例 マップタイルとして使う構造体の名前が MyMapTile の場合
	// typedef struct MyMapTile Type;
	//         ----------------
	//		   ここを書き換える
	typedef int Type;

	// マップタイルの一辺の長さをここに書いてください。
	static const int MapSize = 32;
	// static Type Empty;
private:
	// インスタンス生成禁止
	MapTile();
	MapTile(MapTile&);
	MapTile &operator=(MapTile&);
};


#include <xutility>
#include <memory>

template <class Type>
/// <summary>配列に対して安全に要素にアクセスする方法と反復子を用意します。</summary>
class DynamicArray {
	Type *Data;
	int Size;
public:
	/// <summary>
	/// 動的に割り当てた配列とその大きさから DynamicArray オブジェクトを初期化します。
	/// DynamicArray オブジェクトを使って要素を変更すると、元の配列にもその変更が適用されます。
	/// 渡された配列を DynamicArray 内部で自動的に解放することはないので注意が必要です。
	/// </summary>
	/// <param name="data">反復子を用意させたい配列</param>
	/// <param name="size">反復子を用意させたい配列の要素数</param>
	/// <remarks>
	/// 使用例:
	/// <code>
	/// int *pArray = new int[10] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	/// {
	///		DynamicArray&lt;int&gt; dArray { pArray, 10 };
	///		// 普通の配列みたいに要素を変更
	///		// pArray[0] が 10 に書き換わる。
	///		dArray[0] = 10;
	///		
	///		// range-based for も使える
	///		for (int& val : dArray) {
	///			printf("%d\n", val);
	///		}
	///	} 
	/// // スコープを抜けても、DynamicArray によって pArray は解放されないので、
	/// // プログラマが配列を解放しなければならない。 
	/// delete[] pArray;
	/// </code>
	/// </remarks>
	DynamicArray(Type* data, int size) :
		Data(data),
		Size(size) {
	}

	// __declspec(property) は、Visual C++ の拡張なので、Visual C++ のコンパイラの時だけメンバを追加
#ifdef _MSC_BUILD
	__declspec(property(get = size))
	/// <summary>要素数を取得します。</summary>
	size_t Length;
#endif

	/// <summary>要素数を取得します。</summary>
	inline size_t size() const {
		return Size;
	}

	/// <summary>内部の配列データを取得します。</summary>
	inline Type *data() const {
		return Data;
	}

	/// <summary>指定した要素の値への参照を取得します。</summary>
	/// <param name="index">アクセスする配列要素のインデックス。0 以上、要素数未満である必要があります。</param>
	inline Type &at(int index) {
		if (index >= 0 && index < Size) {
			return Data[index];
		} else {
			throw std::out_of_range("bad index!");
		}
	}

	/// <summary>指定した要素の値への参照を取得します。</summary>
	/// <param name="index">アクセスする配列要素のインデックス。0 以上、要素数未満である必要があります。</param>
	inline Type &Item(int index) {
		return at(index);
	}

	/// <summary>指定した要素の値への参照を取得します。</summary>
	/// <param name="index">アクセスする配列要素のインデックス。0 以上、要素数未満である必要があります。</param>
	inline Type &operator[](int index) {
		return at(index);
	}

	/// <summary>DynamicArray の要素を順方向に走査するイテレータを実装します。</summary>
	/// <remarks>
	/// 使用例:
	/// <code>
	/// int pArray = new int[10] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	/// DynamicArray dArray { pArray, 10 };
	///
	/// for (DynamicArray&lt;int&gt;::Iterator iter = dArray.begin(); iter != dArray.end(); iter++) {
	///		printf("%d\n", *iter);
	/// }
	/// </code>
	/// </remarks>
	class Iterator : public std::iterator<std::forward_iterator_tag, Type> {
		size_t Index;
		DynamicArray* Container;

		friend class DynamicArray<Type>;

		Iterator(DynamicArray *container) :
			Index(SIZE_MAX), Container(container) {
		}

		Iterator(DynamicArray *container, size_t index) :
			Index(index >= 0 && index < static_cast<size_t>(container->Size) ? index : SIZE_MAX), Container(container) {
		}
	public:
		/// <summary>イテレータをコピーします。</summary>
		Iterator(const Iterator& obj) = default;

		/// <summary>イテレータを１つ進めます</summary>
		/// <returns>１つ進めた後のイテレータ</returns>
		Iterator& operator++() {
			Index++;
			if (Index >= static_cast<size_t>(Container->Size)) Index = SIZE_MAX;
			return *this;
		}

		/// <summary>イテレータを１つ進めます</summary>
		/// <returns>１つ進める前のイテレータ</returns>
		Iterator operator++(int) {
			Iterator RetVal(*this);
			Index++;
			if (Index >= static_cast<size_t>(Container->Size)) Index = SIZE_MAX;
			return RetVal;
		}

		/// <summary>現在のイテレータの位置の値を取得します。</summary>
		Type &operator*() {
			static Type Dummy;
			return (Index != SIZE_MAX ? Container->Data[Index] : Dummy);
		}

		/// <summary>２つのイテレータが等しいかどうか確かめます。</summary>
		bool operator==(const Iterator& obj) {
			return !(*this != obj);
		}

		/// <summary>２つのイテレータが等しくないかどうか確かめます。</summary>
		bool operator!=(const Iterator& obj) {
			return (obj.Container != Container || obj.Index != Index);
		}
	};

	/// <summary>最初の要素を示すイテレータを取得します。</summary>
	/// <example>
	/// 使用例:
	/// class <see cref="Iterator">DynamicArray&lt;Type&gt::Iterator</see> を参照のこと。
	/// </example>
	Iterator begin() {
		return Iterator(this, 0);
	}

	/// <summary>最後の要素を示すイテレータを取得します。</summary>
	/// <example>
	/// 使用例:
	/// class <see cref="Iterator">DynamicArray&lt;Type&gt::Iterator</see> を参照のこと。
	/// </example>
	Iterator end() {
		return Iterator(this);
	}
};

template <class Type>
/// <summary>マップチップを管理するレイヤーを実装します。</summary>
class IMapTileLayor {
public:
	/// <summary>列数を取得します。</summary>
	virtual int Cols() = 0;
	/// <summary>行数を取得します。</summary>
	virtual int Rows() = 0;
	/// <summary>1つのタイルの大きさを取得します。</summary>
	virtual Pixel GetTileSize() { return static_cast<Pixel>(MapTile::MapSize); };
	/// <summary>X座標を取得します。</summary>
	virtual Pixel GetX() { return 0; };
	/// <summary>Y座標を取得します。</summary>
	virtual Pixel GetY() { return 0; };
	/// <summary>X軸方向の速度を取得します。</summary>
	virtual Pixel GetDeltaX() { return 0; };
	/// <summary>Y軸方向の速度を取得します。</summary>
	virtual Pixel GetDeltaY() { return 0; };

	/// <summary>指定した座標にマップチップを配置します。</summary>
	/// <param name="tile">配置するマップチップ</param>
	/// <param name="x">マップチップを配置する場所の列数</param>
	/// <param name="y">マップチップを配置する場所の行数</param>
	virtual void SetTile(Type& tile, int x, int y) = 0;
	/// <summary>指定した座標にあるマップチップを取得します。</summary>
	/// <param name="x">取得するマップチップの列数</param>
	/// <param name="y">取得するマップチップの行数</param>
	virtual Type &GetTile(int x, int y) = 0;
};

template <class MapType>
/// <summary>自由にサイズが変更できるタイルマップを実装します。</summary>
/// <example>
/// 使用例:
/// <code>
/// // int[120][30] の２次元配列を作成し、-1で埋める
/// Map&lt;int&gt; MyMap { 120, 30, -1 };
/// // [] を使って配列の内容を更新する。
/// MyMap[30][20] = 120;
/// // [] を使って配列の内容を取得する。
/// int val = MyMap[20][20];
/// 
/// printf("MyMap[20][20] = %d\n", val);
/// // 出力:
/// // MyMap[20][20] = -1
/// </code>
/// </example>
class Map : public IMapTileLayor<MapType> {
	MapType **Tiles;
	bool IsTilesCopyed;
	int Width, Height;

	// コピーは禁止
	Map(Map& right) {
		Create(right.Width, right.Height, right.Tiles);
	}

	Map& operator=(Map& right) {
		if (&right == this) {
			return *this;
		}

		Create(right.Width, right.Height, right.Tiles);
		return *this;
	};	
public:
	int X, Y, DeltaX, DeltaY;

	/// <summary>X座標を取得します。</summary>
	virtual Pixel GetX() { return X; };
	/// <summary>Y座標を取得します。</summary>
	virtual Pixel GetY() { return Y; };
	/// <summary>X軸方向の速度を取得します。</summary>
	virtual Pixel GetDeltaX() { return DeltaX; };
	/// <summary>Y軸方向の速度を取得します。</summary>
	virtual Pixel GetDeltaY() { return DeltaY; };

	/// <summary>空のタイルマップを作成します。</summary>
	Map() : Tiles(nullptr), Width(0), Height(0), IsTilesCopyed(false) {
		DefaultMap = *this;
	};

	/// <summary>指定した大きさのタイルマップを作成します。</summary>
	/// <param name="width">新しく作成する Map オブジェクトの幅</param>
	/// <param name="height">新しく作成する Map オブジェクトの高さ</param>
	Map(int width, int height) : Map() {
		Create(width, height);

		DefaultMap = *this;
	};

	/// <summary>指定した大きさのタイルマップを作成し、指定した値で埋めます。</summary>
	/// <param name="width">新しく作成する Map オブジェクトの幅</param>
	/// <param name="height">新しく作成する Map オブジェクトの高さ</param>
	Map(int width, int height, MapType& val) : Map() {
		Create(width, height);
		Fill(val);

		DefaultMap = *this;
	};

	/// <summary>指定した大きさのタイルマップを作成し、指定した値で埋めます。</summary>
	/// <param name="width">新しく作成する Map オブジェクトの幅</param>
	/// <param name="height">新しく作成する Map オブジェクトの高さ</param>
	Map(int width, int height, MapType&& val) : Map() {
		Create(width, height);
		Fill(val);

		DefaultMap = *this;
	};

	template <size_t width, size_t height>
	/// <summary>既存の２次元配列からタイルマップを作成します。</summary>
	/// <param name="tiles">マップタイルを作成するもとの２次元配列</param>
	/// <param name="forceclone">
	///	コピーを強制するかどうか。コピーを行わなければ渡された２次元配列とこのタイルマップの間で共有され、
	///	一方を変更するともう一方も変更されます。コピーを行うと渡された２次元配列とこのタイルマップは独立したもの
	///	となり、互いに影響を与えません。既定ではこのオプションは false に設定されます。
	/// </param>
	Map(MapType(&tiles)[width][height], bool forceclone = false) :
		Tiles(nullptr),
		Width(width),
		Height(height),
		IsTilesCopyed(true) {
		// 2次元配列をダブルポインタで渡すための下準備
		MapType** newtiles = new MapType*[width];

		for (size_t i = 0; i < width; i++) {
			newtiles[i] = tiles[i];
		}

		Create(width, height, newtiles, forceclone);

		delete[] newtiles;

		DefaultMap = *this;
	}

	~Map() {
		if (Tiles != nullptr) {
			if (!IsTilesCopyed) {
				delete[] Tiles[0];
			}

			delete[] Tiles;
		}
	};
 
	/// <summary>指定した大きさのタイルマップを作成します。</summary>
	/// <param name="width">新しく作成するタイルマップの幅</param>
	/// <param name="height">新しく作成するタイルマップの高さ</param>
	/// <param name="tiles">
	/// マップタイルのコピーもとの２次元配列。nullptr を渡すと、内部で新しいタイルマップを作成します。
	///	既定では、この引数に nullptr が設定されます。
	/// </param>
	/// <param name="forceclone">
	/// コピーを強制するかどうか。コピーを行わなければ tiles で渡された２次元配列とこのタイルマップの間で共有され、
	///	一方を変更するともう一方も変更されます。コピーを行うと tiles で渡された２次元配列とこのタイルマップは独立したもの
	///	となり、互いに影響を与えません。tiles に nullptr を渡した場合、このオプションは無視されます。
	///	既定ではこのオプションは false に設定されます。
	/// </param>
	void Create(int width, int height, MapType **source = nullptr, bool forceclone = false) {
		Width = width;
		Height = height;
		IsTilesCopyed = (source != nullptr && !forceclone);

		if (Tiles != nullptr) {
			// デストラクタだけ呼び出す。
			Map::~Map();
		}

		Tiles = new MapType*[width];

		if (IsTilesCopyed) {
			for (int i = 0; i < width; i++) {
				Tiles[i] = source[i];
			}
		} else {
			MapType *newtiles = new MapType[width * height];

			for (int i = 0; i < width; i++) {
				Tiles[i] = newtiles + (i * height);
			}
		}
	};

	/// <summary>現在のマップを指定した値で埋めます。</summary>
	void Fill(MapType& val) {
		if (Tiles != nullptr && Tiles[0] != nullptr) {
			for (auto& item : DynamicArray<MapType>(Tiles[0], Width * Height)) {
				item = val;
			}
		}
	}

	/// <summary>現在のマップを指定した値で埋めます。</summary>
	void Fill(MapType&& val) {
		if (Tiles != nullptr && Tiles[0] != nullptr) {
			for (auto& item : DynamicArray<MapType>(Tiles[0], Width * Height)) {
				item = val;
			}
		}
	}

	/// <summary>列数を取得します。</summary>
	int Cols() override { return Width; };
	/// <summary>行数を取得します。</summary>
	int Rows() override { return Height; };

	/// <summary>指定した場所にマップチップを配置します。</summary>
	/// <param name="tile">配置するマップチップ</param>
	/// <param name="x">マップチップを配置する場所の列数。0 以上幅未満である必要があります。</param>
	/// <param name="y">マップチップを配置する場所の行数。0 以上高さ未満である必要があります。</param>
	/// <remarks>
	/// x, y のいずれかが範囲外の場合、何もしません。
	/// </remarks>
	void SetTile(MapType& tile, int x, int y) override {
		if (x >= 0 && x < Width && y >= 0 && y < Height) {
			Tiles[x][y] = tile;
		} else {
			// Will throw.
		}
	};

	/// <summary>指定した場所にあるマップチップを取得します。</summary>
	/// <param name="x">マップチップを配置する場所の列数。0 以上幅未満である必要があります。</param>
	/// <param name="y">マップチップを配置する場所の行数。0 以上高さ未満である必要があります。</param>
	/// <returns>
	/// 指定した場所にあるマップチップ。x, y のいずれかが範囲外の場合、位置 (0, 0) にあるマップチップを返します。
	/// </returns>
	MapType &GetTile(int x, int y) override {
		if (x >= 0 && x < Width && y >= 0 && y < Height) {
			return Tiles[x][y];
		} else {
			return Tiles[0][0];
		}
	};

	/// <summary>指定した場所にあるマップチップを取得します。</summary>
	/// <param name="x">マップチップを配置する場所の列数。0 以上幅未満である必要があります。</param>
	/// <returns>指定した列数にあるマップチップ。x が範囲外の場合、空の配列を返します。</returns>
	/// </summary>
	/// <example>
	/// 使用例:
	/// <see cref="Map">Map&lt;MapType&gt;</see>を参照のこと。
	/// </example>
	DynamicArray<MapType> operator[](int x) {
		if (x >= 0 && x < Width) {
			return DynamicArray<MapType>(Tiles[x], Height);
		} else {
			return DynamicArray<MapType>(nullptr, 0);
		}
	};

	

	// インスタンス
	static Map DefaultMap;
};

template <class Type>
Map<Type> Map<Type>::DefaultMap = Map<Type>();

typedef Map<MapTile::Type> CMap;

template <class Type, size_t Width, size_t Height>
/// <summary>
/// 既存の２次元配列から、Map オブジェクトを作成します。渡された２次元配列はこのタイルマップの間で共有され、
///	一方を変更するともう一方も変更されます。
/// </summary>
/// <example>
/// 使用例:
/// <code>
/// int MapTiles[120][30];
/// MapTiles[20][20] = -1;
///
/// auto&amp;&amp; MyMap = CreateMap(MapTiles);
/// // これを書くのと同じ
///	// Map&lt;int&gt;&amp;&amp; MyMap { MapTiles };
///
/// // [] を使って配列の内容を更新する。
/// MyMap[30][20] = 120;
/// // [] を使って配列の内容を取得する。
/// int val = MyMap[20][20];
/// 
/// printf("MyMap[20][20] = %d\n", val);
/// // 出力:
/// // MyMap[20][20] = -1
/// </code>
/// </example>
Map<Type> CreateMap(Type (&map)[Width][Height]) {
	return Map<Type>(map);
}

template <class Type, size_t Width, size_t Height>
/// <summary>既存の２次元配列をタイルマップとして使うための関数を用意します。</summary>
/// <example>
/// 使用例:
/// <code>
/// int MapTiles[120][30];
/// MapTiles[20][20] = -1;
///
/// TemporaryMap&lt;int, 120, 30&gt; MyMap { MapTiles };
/// // このように初期化してもよい。
/// // auto&amp;&amp; MyMap = CreateTemporaryMap(MapTiles);
///
/// // [] を使って配列の内容を更新する。
/// MyMap[30][20] = 120;
/// // [] を使って配列の内容を取得する。
/// int val = MyMap[20][20];
/// 
/// printf("MyMap[20][20] = %d\n", val);
/// // 出力:
/// // MyMap[20][20] = -1
/// </code>
/// </example>
class TemporaryMap : public IMapTileLayor<Type> {
	Type (&Tiles)[Width][Height];
public:
	TemporaryMap(Type(&tiles)[Width][Height]) :
		Tiles(tiles) {
	}

	TemporaryMap(TemporaryMap&) = default;
	TemporaryMap& operator=(TemporaryMap&) = default;
	
	/// <summary>列数を取得します。</summary>
	int Cols() override { return Width; };
	/// <summary>行数を取得します。</summary>
	int Rows() override { return Height; };

	/// <summary>指定した場所にマップチップを配置します。</summary>
	/// <param name="tile">配置するマップチップ</param>
	/// <param name="x">マップチップを配置する場所の列数。0 以上幅未満である必要があります。</param>
	/// <param name="y">マップチップを配置する場所の行数。0 以上高さ未満である必要があります。</param>
	/// <remarks>
	/// x, y のいずれかが範囲外の場合、何もしません。
	/// </remarks>
	void SetTile(Type& tile, int x, int y) override {
		if (x >= 0 && x < Width && y >= 0 && y < Height) {
			Tiles[x][y] = tile;
		} else {
			// Will throw.
		}
	};

	/// <summary>指定した場所にあるマップチップを取得します。</summary>
	/// <param name="x">マップチップを配置する場所の列数。0 以上幅未満である必要があります。</param>
	/// <param name="y">マップチップを配置する場所の行数。0 以上高さ未満である必要があります。</param>
	/// <returns>
	/// 指定した場所にあるマップチップ。x, y のいずれかが範囲外の場合、位置 (0, 0) にあるマップチップを返します。
	/// </returns>
	Type &GetTile(int x, int y) override {
		if (x >= 0 && x < Width && y >= 0 && y < Height) {
			return Tiles[x][y];
		} else {
			return Tiles[0][0];
		}
	};

	/// <summary>指定した場所にあるマップチップを取得します。</summary>
	/// <param name="x">マップチップを配置する場所の列数。0 以上幅未満である必要があります。</param>
	/// <returns>指定した列数にあるマップチップ。x が範囲外の場合、空の配列を返します。</returns>
	/// <example>
	/// 使用例:
	/// <see cref="TemporaryMap">TemporaryMap&lt;Type, Width, Height&gt;</see>を参照のこと。
	/// </example>
	DynamicArray<Type> operator[](int x) {
		return DynamicArray<Type>(Tiles[x], Height);
	}
};

template <class Type, size_t Width, size_t Height>
/// <summary>
/// 既存の２次元配列から、TemporaryMap オブジェクトを作成します。渡された２次元配列はこのタイルマップの間で共有され、
///	一方を変更するともう一方も変更されます。
/// </summary>
/// <example>
/// 使用例:
/// <code>
/// int MapTiles[120][30];
/// MapTiles[20][20] = -1;
///
/// auto&& MyMap = CreateTemporaryMap(MapTiles);
/// // これを書くのと同じ
///	// TemporaryMap&lt;int, 120, 30&gt;&amp;&amp; MyMap { MapTiles };
///
/// // [] を使って配列の内容を更新する。
/// MyMap[30][20] = 120;
/// // [] を使って配列の内容を取得する。
/// int val = MyMap[20][20];
/// 
/// printf("MyMap[20][20] = %d\n", val);
/// // 出力:
/// // MyMap[20][20] = -1
/// </code>
/// </example>
TemporaryMap<Type, Width, Height> CreateTemporaryMap(Type(&map)[Width][Height]) {
	return TemporaryMap<Type, Width, Height>(map);
}