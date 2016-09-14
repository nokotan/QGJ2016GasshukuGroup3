#pragma once

/// @file Collider.h
/// Collider.h -----------------------------
/// あたり判定を行うための関数を提供します。
/// 使いやすさを重視するため、内部の処理の改変がしにくくなっているため、
/// 改変するのであれば、""を使うことをお勧めします。
/// 最終更新:
///			2016/08/14
/// ----------------------------------------
/// @remark
/// 使用例:
/// @code
/// #include "DxLib.h"
///	#include "Collider.h"
///
///	struct Player {
///		int x, y, width, height, dx, dy;
///	};
///
/// int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
///
///		ChangeWindowMode(TRUE);
///		
///		if (DxLib_Init() == -1) {
///			return 0;
///		}
///
///		SetDrawScreen(DX_SCREEN_BACK);		
///
///		// タイルマップとして使う２次元配列
///		const int MapTilesWidth = 20;
///		const int MapTilesHeight = 15;
///		int MapTiles[MapTilesWidth][MapTilesHeight];
/// 
///		// タイルマップを -1 (何もない) で埋める
///		for (int i = 0; i &lt; MapTilesWidth; i++) {
///			for (int j = 0; j &lt; MapTilesHeight; j++) {
///				MapTiles[i][j] = -1;
///			
///				// 一番下は床として 0 を入れておく
///				if (j == MapTilesHeight - 1) {
///					MapTiles[i][j] = 0;
///				}
///			}
///		}
///
///		Player player;
///		// player を初期化
///		player.x = 0;
///		player.y = 384;
///		player.width = 32;
///		player.height = 64;
///		player.dx = 0;
///		player.dy = 0;
///
///		// メインループ
///		while (ProcessMessage() != -1 &amp;&amp; ClearDrawScreen() != -1) {
///			// 入力に応じて、プレイヤーのスピードを変える
///			if (CheckHitKey(KEY_INPUT_LEFT)) {
///				player.dx = -2;
///			} else if (CheckHitKey(KEY_INPUT_RIGHT)) {
///				player.dx = 2;
///			} else {
///				player.dx = 0;
///			}
///
///			if (CheckHitKey(KEY_INPUT_SPACE) &amp;&amp; player.y == 384) {
///				player.dy = -20;
///			}
///
///			// 重力加速度を加える。
///			player.dy += 1;
///
///			if (player.dy &gt; 10) {
///				player.dy = 10;
///			}
///
///			// あたり判定を行う。
///			CollisionCheck(player, MapTiles, 32, -1);
///
///			// プレイヤー描画
///			DrawBox(player.x, player.y, player.x + player.width, player.y + player.height, GetColor(255, 255, 255), TRUE);
///
///			for (int i = 0; i &lt; MapTilesWidth; i++) {
///				for (int j = 0; j &lt; MapTilesHeight; j++) {
///					if (MapTiles[i][j] != -1) {
///						DrawBox(i * 32, j * 32, i * 32 + 32, j * 32 + 32, GetColor(0, 216, 0), TRUE);
///					}
///				}
///			}
///
///			ScreenFlip();
///		}
///
///		DxLib_End();
///		return 0;
/// }
/// @endcode

#include "BaseUnit.h"
#include "MemberCheck.h"
#include "DebugHelper.h"

#include <cmath>
#include <vector>

namespace Direction {
	enum Direction {
		None = 0,
		Left = 1,
		Right = 2,
		Up = 4,
		Down = 8,
		LeftAndRight = 3,
		UpAndDown = 12,
	};
}

template <class TileType>
/// <summary>ユーザー定義の MapTileIsEmpty 関数、MapType の Enable, IsEmpty メンバのうち存在するものだけを呼び出す関数オブジェクトを作成します。</summary>
struct CollisionCheckIsTileEmptyHelper {
	bool operator()(TileType& obj) {
		return MapTileIsEmptyCaller<TileType>::MapTileIsEmptyProxy(obj) ||
			EnableGetter<TileType>::Enable(obj) ||
			IsEmptyCaller<TileType>::IsEmpty(obj);
	}
};

template <class Type, class MapType, bool = true>
/// <summary>あたり判定の対象となるオブジェクトが適切に実装されているかどうか調べ、また、あたり判定の実装を提供します。</summary>
struct CollisionCheckHelper {
	template <class IsTileEmpty>
	/// <summary>あたり判定の実装を提供します。</summary>
	/// <param name="obj">あたり判定を行うオブジェクト。</param>
	/// <param name="layor">あたり判定のフィールドとなる IMapTileLayor オブジェクト。</param>
	/// <param name="EmptyCheck">空のマップタイルかどうか判定するための関数または関数オブジェクト</param>

	static void CollisionCheck(Type& obj, IMapTileLayor<MapType>& layor, IsTileEmpty EmptyCheck) {
		Pixel newX = static_cast<Pixel>(obj.x + obj.dx);
		Rectangle_t rectX = GetCollideRectangeCaller<Type>::GetCollideRectangle(obj);
		rectX.X += static_cast<Pixel>(obj.dx);
		rectX.Y += layor.GetDeltaY();

		int first_X = static_cast<int>(floor((rectX.X - layor.GetX()) / static_cast<double>(layor.GetTileSize())));
		int last_X = static_cast<int>((rectX.X + rectX.Width - 1 - layor.GetX()) / static_cast<double>(layor.GetTileSize()));
		int first_Y = static_cast<int>(floor((rectX.Y - layor.GetY()) / static_cast<double>(layor.GetTileSize())));
		int last_Y = static_cast<int>((rectX.Y + rectX.Height - 1 - layor.GetY()) / static_cast<double>(layor.GetTileSize()));
		// std::vector<MapType*> CollidedList;
		bool IsCanceled = false;
		int CollidedDirection = Direction::None;

		for (int i = first_X; i <= last_X; i++) {
			for (int j = first_Y; j <= last_Y; j++) {
				MapType& tile = layor.GetTile(i, j);
				Rectangle_t tilerect = MapTileGetCollideRectangeCaller<MapType>::GetCollideRectangle(tile, i, j);
				tilerect.X += layor.GetX();
				tilerect.Y += layor.GetY();

				bool is_empty = EmptyCheck(tile) && i >= 0 && i < layor.Cols();

				if (!is_empty && rectX.IntersectWith(tilerect)) {
					if (!OnCollideFromSideCaller<Type, MapType>::OnCollideFromSide(obj, tile, i, j)) {
						if (obj.dx - layor.GetDeltaX() > 0) {
							newX = tilerect.X - rectX.Width - (rectX.X - newX);
							obj.dx = layor.GetDeltaX();
							obj.CollidedDirection |= Direction::Right;
						} else if (obj.dx - layor.GetDeltaX() < 0) {
							newX = tilerect.X + tilerect.Width - (rectX.X - newX);
							obj.dx = layor.GetDeltaX();
							obj.CollidedDirection |= Direction::Left;
						}
					}

					// CollidedList.push_back(&tile);
				}
			}
		}

		if (!IsCanceled) {
			obj.x = static_cast<decltype(obj.x)>(newX);
		}

		//for (MapType* val : CollidedList) {
		//	OnCollideFromSideCaller<Type, MapType>::OnCollideFromSide(obj, *val);
		//}

		Pixel newY = static_cast<Pixel>(obj.y + obj.dy);
		Rectangle_t rectY = GetCollideRectangeCaller<Type>::GetCollideRectangle(obj);
		rectY.Y += static_cast<Pixel>(obj.dy);

		first_X = static_cast<int>(floor((rectY.X - layor.GetX()) / static_cast<double>(layor.GetTileSize())));
		last_X = static_cast<int>(static_cast<double>(rectY.X + rectY.Width - layor.GetX()) / static_cast<double>(layor.GetTileSize()));
		first_Y = static_cast<int>(floor((rectY.Y - layor.GetY()) / static_cast<double>(layor.GetTileSize())));
		last_Y = static_cast<int>(static_cast<double>(rectY.Y + rectY.Height - layor.GetY()) / static_cast<double>(layor.GetTileSize()));
		// std::vector<MapType*> CollidedBottomList, CollidedTopList;
		IsCanceled = false;

		for (int i = first_X; i <= last_X; i++) {
			for (int j = first_Y; j <= last_Y; j++) {
				MapType& tile = layor.GetTile(i, j);
				Rectangle_t tilerect = MapTileGetCollideRectangeCaller<MapType>::GetCollideRectangle(tile, i, j);
				tilerect.X += layor.GetX();
				tilerect.Y += layor.GetY();

				bool is_empty = EmptyCheck(tile);

				if (!is_empty && rectY.IntersectWith(tilerect)) {
					if (obj.dy - layor.GetDeltaY() > 0) {
						if (!OnCollideFromBottomCaller<Type, MapType>::OnCollideFromBottom(obj, tile, i, j)) {
							newY = tilerect.Y - rectY.Height;
							obj.dy = layor.GetDeltaY();
							obj.FloorDeltaX = layor.GetDeltaX();
							obj.CollidedDirection |= Direction::Down;
						}

						// CollidedBottomList.push_back(&tile);
					} else if (obj.dy - layor.GetDeltaY() < 0) {
						if (!OnCollideFromTopCaller<Type, MapType>::OnCollideFromTop(obj, tile, i, j)) {
							newY = tilerect.Y + tilerect.Height;
							obj.dy = layor.GetDeltaY();
							obj.CollidedDirection |= Direction::Up;
						}

						// CollidedTopList.push_back(&tile);
					}
				}
			}
		}

		//for (MapType* val : CollidedTopList) {
		//	OnCollideFromTopCaller<Type, MapType>::OnCollideFromTop(obj, *val);
		//}

		//for (MapType* val : CollidedBottomList) {
		//	OnCollideFromBottomCaller<Type, MapType>::OnCollideFromBottom(obj, *val);
		//}

		obj.y = static_cast<decltype(obj.y)>(newY);
	}


};

template <class Type, class MapType>
/// <summary>あたり判定の対象となるオブジェクトが適切に実装されているかどうか調べ、また、あたり判定の実装を提供します。</summary>
struct CollisionCheckHelper<Type, MapType, false> {
	template <class IsTileEmpty>
	/// <summary>あたり判定の実装を提供します。</summary>
	/// <param name="obj">あたり判定を行うオブジェクト。</param>
	/// <param name="layor">あたり判定のフィールドとなる IMapTileLayor オブジェクト。</param>
	/// <param name="EmptyCheck">空のマップタイルかどうか判定するための関数または関数オブジェクト</param>
	static void CollisionCheck(Type& obj, IMapTileLayor<MapType>& layor, IsTileEmpty EmptyCheck);
};

#if defined(__BASE_FILE__)
/// @def コンパイル中のソースファイル
#	define MY_BASE_FILE __BASE_FILE__
#elif !defined(MY_BASE_FILE)
/// @def コンパイル中のソースファイル
#	define MY_BASE_FILE __FILE__
#endif

template <class Type, class MapType>
template <class IsTileEmpty>
void CollisionCheckHelper<Type, MapType, false>::CollisionCheck(Type& obj, IMapTileLayor<MapType>& layor, IsTileEmpty EmptyCheck) {
	
	DebugBreak();
#pragma message(MY_BASE_FILE"("GetMacroValue(__LINE__)",1): error : CollisionCheck の第一引数には、x, y, dx, dy, width, heightを持った構造体が必要です。(使用例は出力を確認してください。)\n" \
	"使用例:\n" \
	"struct Player {\n" \
	"\tint x, y, width, height, dx, dy;\n" \
	"};\n" \
	"\n" \
	"int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {\n" \
	"\tint tiles[100][30];\n" \
	"\tPlayer player;\n" \
	"\tCollisionCheck(player, tiles);\n" \
	"\treturn 0;\n" \
	"};")
};

template <class Type, class MapType>
/// <summary>
/// 渡された IMapTileLayor オブジェクト中で、渡されたオブジェクトに対してあたり判定を行います。
/// マップタイルが空かどうかは、ユーザー定義の MapTileIsEmpty 関数、MapType の Enable, IsEmpty メンバのうち
/// 存在するものが使用されます。
/// あたり判定がおかしい場合は、この３つのうち少なくとも一つが定義されていることを確認してください。
/// </summary>
/// <param name="obj">
/// あたり判定を行うオブジェクト。このオブジェクトはメンバとして、x, y, width, height, dx, dy を持っている必要があります。
/// この関数を行うと、オブジェクトの x, y は適切に更新され、マップタイルにぶつかったときに、そのオブジェクトの dx, dy を 0 に設定します。
/// オブジェクトの width, height が 0 であるとあたり判定が失敗するので注意してください。
/// </param>
/// <param name="layor">あたり判定のフィールドとなる IMapTileLayor オブジェクト。</param>
/// <example>
/// 使用例:
/// 注意！
/// このサンプルでは、関数を使うための最小限の部分だけ紹介しています。
/// ファイル "Collider.h" に示したサンプルも参考にしながら読んでください。
/// <code>
/// // マップタイルが空かどうか判定する関数。この名前で特定の引数を持っていれば呼び出されます。
/// bool MapTileIsEmpty(int tileid) {
///		return tileid = -1;
/// }
///
/// int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
///		const int MapTilesWidth = 20;
///		const int MapTilesHeight = 15;
///
///		Map&lt;int&gt; MyMap { MapTilesWidth, MapTilesHeight };
///
///		// 床以外 -1 (何もない) で埋める。
///		for (int x = 0; x &lt; MapTilesWidth; x++) {
///			for (int y = 0; y &lt; MapTilesHeight - 1; y++) {
///				MyMap[x][y] = -1;
///			}
///		
///			MyMap[x][MapTilesHeight - 1] = 0;
///		}
///	
///		struct Player {
///			int x, y, width, height, dx, dy;
///		};
/// 
///		Player player;
///		CollisionCheck(player, MyMap);
///		return 0;
/// }
/// </code>
/// </example>
void CollisionCheck(Type& obj, IMapTileLayor<MapType>& layor) {
	CollisionCheckHelper<Type, MapType, CheckMember<Type>::HasXY && CheckMember<Type>::HasWidthHeight && CheckMember<Type>::HasDxDy>
		::CollisionCheck(obj, layor, CollisionCheckIsTileEmptyHelper<MapType>());
}

template <class Type, class MapType, class TypeEmpty>
/// <summary>
/// 渡された IMapTileLayor オブジェクト中で、渡されたオブジェクトに対してあたり判定を行います。
/// </summary>
/// <param name="obj">
/// あたり判定を行うオブジェクト。このオブジェクトはメンバとして、x, y, width, height, dx, dy を持っている必要があります。
/// この関数を行うと、オブジェクトの x, y は適切に更新され、マップタイルにぶつかったときに、そのオブジェクトの dx, dy を 0 に設定します。
/// オブジェクトの width, height が 0 であるとあたり判定が失敗するので注意してください。
/// </param>
/// <param name="layor">あたり判定のフィールドとなる IMapTileLayor オブジェクト。</param>
/// <param name="EmptyTile">空のマップタイルかどうか判定するための関数または関数オブジェクト</param>
/// <example>
/// 使用例:
/// 注意！
/// このサンプルでは、関数を使うための最小限の部分だけ紹介しています。
/// ファイル "Collider.h" に示したサンプルも参考にしながら読んでください。
/// <code>
///	const int MapTilesWidth = 20;
///	const int MapTilesHeight = 15;
///
/// Map&lt;int&gt; MyMap { MapTilesWidth, MapTilesHeight };
///
/// // 床以外 -1 (何もない) で埋める。
/// for (int x = 0; x &lt; MapTilesWidth; x++) {
///		for (int y = 0; y &lt; MapTilesHeight - 1; y++) {
///			MyMap[x][y] = -1;
///		}
///		
///		MyMap[x][MapTilesHeight - 1] = 0;
///	}
///	
///	struct Player {
///		int x, y, width, height, dx, dy;
///	};
/// 
/// Player player;
/// CollisionCheck(player, MyMap, [](int tileid) -&gt; bool { return tileid == -1; });
/// </code>
/// </example>
void CollisionCheck(Type& obj, IMapTileLayor<MapType>& layor, TypeEmpty EmptyTile) {
	CollisionCheckHelper<Type, MapType, CheckMember<Type>::HasXY && CheckMember<Type>::HasWidthHeight && CheckMember<Type>::HasDxDy>
		::CollisionCheck(obj, layor, EmptyTile);
}

template <class Type, class MapType>
/// <summary>
/// 渡された IMapTileLayor オブジェクト中で、渡されたオブジェクトに対してあたり判定を行います。
/// </summary>
/// <param name="obj">
/// あたり判定を行うオブジェクト。このオブジェクトはメンバとして、x, y, width, height, dx, dy を持っている必要があります。
/// この関数を行うと、オブジェクトの x, y は適切に更新され、マップタイルにぶつかったときに、そのオブジェクトの dx, dy を 0 に設定します。
/// オブジェクトの width, height が 0 であるとあたり判定が失敗するので注意してください。
/// </param>
/// <param name="layor">あたり判定のフィールドとなる IMapTileLayor オブジェクト。</param>
/// <param name="EmptyTile">空のマップタイルとして使う値。</param>
/// <example>
/// 使用例:
/// 注意！
/// このサンプルでは、関数を使うための最小限の部分だけ紹介しています。
/// ファイル "Collider.h" に示したサンプルも参考にしながら読んでください。
/// <code>
///	const int MapTilesWidth = 20;
///	const int MapTilesHeight = 15;
///
/// Map&lt;int&gt; MyMap { MapTilesWidth, MapTilesHeight };
///
/// // 床以外 -1 (何もない) で埋める。
/// for (int x = 0; x &lt; MapTilesWidth; x++) {
///		for (int y = 0; y &lt; MapTilesHeight - 1; y++) {
///			MyMap[x][y] = -1;
///		}
///		
///		MyMap[x][MapTilesHeight - 1] = 0;
///	}
///	
///	struct Player {
///		int x, y, width, height, dx, dy;
///	};
/// 
/// Player player;
/// CollisionCheck(player, MyMap, -1);
/// </code>
/// </example>
void CollisionCheck(Type& obj, IMapTileLayor<MapType>& layor, MapType EmptyTile) {
	CollisionCheckHelper<Type, MapType, CheckMember<Type>::HasXY && CheckMember<Type>::HasWidthHeight && CheckMember<Type>::HasDxDy>
		::CollisionCheck(obj, layor, [&](MapType& obj) -> bool { return obj == EmptyTile; });
}

template <class Type, class MapType, size_t width, size_t height>
/// <summary>
/// 渡された２次元配列をタイルマップとして扱い、そのタイルマップ中で渡されたオブジェクトのあたり判定を行います。
/// マップタイルが空かどうかは、ユーザー定義の MapTileIsEmpty 関数、MapType の Enable, IsEmpty メンバのうち
/// 存在するものが使用されます。
/// あたり判定がおかしい場合は、この３つのうち少なくとも一つが定義されていることを確認してください。
/// </summary>
/// <param name="obj">
/// あたり判定を行うオブジェクト。このオブジェクトはメンバとして、x, y, width, height, dx, dy を持っている必要があります。
/// この関数を行うと、オブジェクトの x, y は適切に更新され、マップタイルにぶつかったときに、そのオブジェクトの dx, dy を 0 に設定します。
/// オブジェクトの width, height が 0 であるとあたり判定が失敗するので注意してください。
/// </param>
/// <param name="layor">タイルマップとして使う２次元配列。</param>
/// <param name="MapSize">タイルマップ１つの幅。</param>
/// <example>
/// 使用例:
/// 注意！
/// このサンプルでは、関数を使うための最小限の部分だけ紹介しています。
/// ファイル "Collider.h" に示したサンプルも参考にしながら読んでください。
/// <code>
/// // マップタイルが空かどうか判定する関数。この名前で特定の引数を持っていれば呼び出されます。
/// bool MapTileIsEmpty(int tileid) {
///		return tileid = -1;
/// }
///
/// int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
///		const int MapTilesWidth = 20;
///		const int MapTilesHeight = 15;
///
///		int MyMap[MapTilesWidth][MapTilesHeight];
///
///		// 床以外 -1 (何もない) で埋める。
///		for (int x = 0; x &lt; MapTilesWidth; x++) {
///			for (int y = 0; y &lt; MapTilesHeight - 1; y++) {
///				MyMap[x][y] = -1;
///			}
///		
///			MyMap[x][MapTilesHeight - 1] = 0;
///		}
///	
///		struct Player {
///			int x, y, width, height, dx, dy;
///		};
/// 
///		Player player;
///		CollisionCheck(player, MyMap, 32);
///		return 0;
/// }
/// </code>
/// </example>
void CollisionCheck(Type& obj, MapType(&layor)[width][height], int MapSize) {
	CollisionCheckHelper<Type, MapType, CheckMember<Type>::HasXY && CheckMember<Type>::HasWidthHeight && CheckMember<Type>::HasDxDy>
		::CollisionCheck(obj, TemporaryMap<MapType, width, height>(layor), CollisionCheckIsTileEmptyHelper<MapType>());
}

template <class Type, class MapType, size_t width, size_t height, class TypeEmpty>
/// <summary>
/// 渡された２次元配列をタイルマップとして扱い、そのタイルマップ中で渡されたオブジェクトのあたり判定を行います。
/// </summary>
/// <param name="obj">
/// あたり判定を行うオブジェクト。このオブジェクトはメンバとして、x, y, width, height, dx, dy を持っている必要があります。
/// この関数を行うと、オブジェクトの x, y は適切に更新され、マップタイルにぶつかったときに、そのオブジェクトの dx, dy を 0 に設定します。
/// オブジェクトの width, height が 0 であるとあたり判定が失敗するので注意してください。
/// </param>
/// <param name="layor">タイルマップとして使う２次元配列。</param>
/// <param name="MapSize">タイルマップ１つの幅。</param>
/// <param name="EmptyTile">空のマップタイルかどうか判定するための関数または関数オブジェクト</param>
/// <example>
/// 使用例:
/// 注意！
/// このサンプルでは、関数を使うための最小限の部分だけ紹介しています。
/// ファイル "Collider.h" に示したサンプルも参考にしながら読んでください。
/// <code>
///	const int MapTilesWidth = 20;
///	const int MapTilesHeight = 15;
///
/// int MyMap[MapTilesWidth][MapTilesHeight];
///
/// // 床以外 -1 (何もない) で埋める。
/// for (int x = 0; x &lt; MapTilesWidth; x++) {
///		for (int y = 0; y &lt; MapTilesHeight - 1; y++) {
///			MyMap[x][y] = -1;
///		}
///		
///		MyMap[x][MapTilesHeight - 1] = 0;
///	}
///	
///	struct Player {
///		int x, y, width, height, dx, dy;
///	};
/// 
/// Player player;
/// CollisionCheck(player, MyMap, 32, [](int tileid) -&gt; bool { return tileid == -1; });
/// </code>
/// </example>
void CollisionCheck(Type& obj, MapType (&layor)[width][height], int MapSize, TypeEmpty EmptyTile) {
	CollisionCheckHelper<Type, MapType, CheckMember<Type>::HasXY && CheckMember<Type>::HasWidthHeight && CheckMember<Type>::HasDxDy>
		::CollisionCheck(obj, TemporaryMap<MapType, width, height>(layor), EmptyTile);
} 

template <class Type, class MapType, size_t width, size_t height>
/// <summary>
/// 渡された２次元配列をタイルマップとして扱い、そのタイルマップ中で渡されたオブジェクトのあたり判定を行います。
/// </summary>
/// <param name="obj">
/// あたり判定を行うオブジェクト。このオブジェクトはメンバとして、x, y, width, height, dx, dy を持っている必要があります。
/// この関数を行うと、オブジェクトの x, y は適切に更新され、マップタイルにぶつかったときに、そのオブジェクトの dx, dy を 0 に設定します。
/// オブジェクトの width, height が 0 であるとあたり判定が失敗するので注意してください。
/// </param>
/// <param name="layor">タイルマップとして使う２次元配列。</param>
/// <param name="MapSize">タイルマップ１つの幅。</param>
/// <param name="EmptyTile">空のマップタイルとして使う値。</param>
/// <example>
/// 使用例:
/// 注意！
/// このサンプルでは、関数を使うための最小限の部分だけ紹介しています。
/// ファイル "Collider.h" に示したサンプルも参考にしながら読んでください。
/// <code>
///	const int MapTilesWidth = 20;
///	const int MapTilesHeight = 15;
///
/// int MyMap[MapTilesWidth][MapTilesHeight];
///
/// // 床以外 -1 (何もない) で埋める。
/// for (int x = 0; x &lt; MapTilesWidth; x++) {
///		for (int y = 0; y &lt; MapTilesHeight - 1; y++) {
///			MyMap[x][y] = -1;
///		}
///		
///		MyMap[x][MapTilesHeight - 1] = 0;
///	}
///	
///	struct Player {
///		int x, y, width, height, dx, dy;
///	};
/// 
/// Player player;
/// CollisionCheck(player, MyMap, 32, -1);
/// </code>
/// </example>
void CollisionCheck(Type& obj, MapType(&layor)[width][height], int MapSize, MapType EmptyTile) {
	CollisionCheckHelper<Type, MapType, CheckMember<Type>::HasXY && CheckMember<Type>::HasWidthHeight && CheckMember<Type>::HasDxDy>
		::CollisionCheck(obj, TemporaryMap<MapType, width, height>(layor), [&](MapType& obj) -> bool { return obj == EmptyTile; });
}