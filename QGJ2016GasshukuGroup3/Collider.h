#pragma once

/// @file Collider.h
/// Collider.h -----------------------------
/// �����蔻����s�����߂̊֐���񋟂��܂��B
/// �g���₷�����d�����邽�߁A�����̏����̉��ς����ɂ����Ȃ��Ă��邽�߁A
/// ���ς���̂ł���΁A""���g�����Ƃ������߂��܂��B
/// �ŏI�X�V:
///			2016/08/14
/// ----------------------------------------
/// @remark
/// �g�p��:
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
///		// �^�C���}�b�v�Ƃ��Ďg���Q�����z��
///		const int MapTilesWidth = 20;
///		const int MapTilesHeight = 15;
///		int MapTiles[MapTilesWidth][MapTilesHeight];
/// 
///		// �^�C���}�b�v�� -1 (�����Ȃ�) �Ŗ��߂�
///		for (int i = 0; i &lt; MapTilesWidth; i++) {
///			for (int j = 0; j &lt; MapTilesHeight; j++) {
///				MapTiles[i][j] = -1;
///			
///				// ��ԉ��͏��Ƃ��� 0 �����Ă���
///				if (j == MapTilesHeight - 1) {
///					MapTiles[i][j] = 0;
///				}
///			}
///		}
///
///		Player player;
///		// player ��������
///		player.x = 0;
///		player.y = 384;
///		player.width = 32;
///		player.height = 64;
///		player.dx = 0;
///		player.dy = 0;
///
///		// ���C�����[�v
///		while (ProcessMessage() != -1 &amp;&amp; ClearDrawScreen() != -1) {
///			// ���͂ɉ����āA�v���C���[�̃X�s�[�h��ς���
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
///			// �d�͉����x��������B
///			player.dy += 1;
///
///			if (player.dy &gt; 10) {
///				player.dy = 10;
///			}
///
///			// �����蔻����s���B
///			CollisionCheck(player, MapTiles, 32, -1);
///
///			// �v���C���[�`��
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
/// <summary>���[�U�[��`�� MapTileIsEmpty �֐��AMapType �� Enable, IsEmpty �����o�̂������݂�����̂������Ăяo���֐��I�u�W�F�N�g���쐬���܂��B</summary>
struct CollisionCheckIsTileEmptyHelper {
	bool operator()(TileType& obj) {
		return MapTileIsEmptyCaller<TileType>::MapTileIsEmptyProxy(obj) ||
			EnableGetter<TileType>::Enable(obj) ||
			IsEmptyCaller<TileType>::IsEmpty(obj);
	}
};

template <class Type, class MapType, bool = true>
/// <summary>�����蔻��̑ΏۂƂȂ�I�u�W�F�N�g���K�؂Ɏ�������Ă��邩�ǂ������ׁA�܂��A�����蔻��̎�����񋟂��܂��B</summary>
struct CollisionCheckHelper {
	template <class IsTileEmpty>
	/// <summary>�����蔻��̎�����񋟂��܂��B</summary>
	/// <param name="obj">�����蔻����s���I�u�W�F�N�g�B</param>
	/// <param name="layor">�����蔻��̃t�B�[���h�ƂȂ� IMapTileLayor �I�u�W�F�N�g�B</param>
	/// <param name="EmptyCheck">��̃}�b�v�^�C�����ǂ������肷�邽�߂̊֐��܂��͊֐��I�u�W�F�N�g</param>

	static void CollisionCheck(Type& obj, IMapTileLayor<MapType>& layor, IsTileEmpty EmptyCheck) {
		Pixel newX = static_cast<Pixel>(obj.x + obj.dx);
		Pixel newDx = static_cast<Pixel>(obj.dx);
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
							newX = tilerect.X - rectX.Width;// -(rectX.X - newX);
							newDx = layor.GetDeltaX();
							obj.CollidedDirection |= Direction::Right;
						} else if (obj.dx - layor.GetDeltaX() < 0) {
							newX = tilerect.X + tilerect.Width;// -(rectX.X - newX);
							newDx = layor.GetDeltaX();
							obj.CollidedDirection |= Direction::Left;
						} else {
							obj.CollidedDirection |= Direction::LeftAndRight;
						}
					}

					// CollidedList.push_back(&tile);
				}
			}
		}

		if (!IsCanceled) {
			obj.x = static_cast<decltype(obj.x)>(newX);
			obj.dx = static_cast<decltype(obj.dx)>(newDx);
		}

		//for (MapType* val : CollidedList) {
		//	OnCollideFromSideCaller<Type, MapType>::OnCollideFromSide(obj, *val);
		//}

		Pixel newY = static_cast<Pixel>(obj.y + obj.dy);
		Pixel newDy = static_cast<Pixel>(obj.dy);
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
							newDy = layor.GetDeltaY();
							obj.FloorDeltaX = layor.GetDeltaX();
							obj.CollidedDirection |= Direction::Down;
						}

						// CollidedBottomList.push_back(&tile);
					} else if (obj.dy - layor.GetDeltaY() < 0) {
						if (!OnCollideFromTopCaller<Type, MapType>::OnCollideFromTop(obj, tile, i, j)) {
							newY = tilerect.Y + tilerect.Height;
							newDy = layor.GetDeltaY();
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
		obj.dy = static_cast<decltype(obj.dy)>(newDy);

	}


};

template <class Type, class MapType>
/// <summary>�����蔻��̑ΏۂƂȂ�I�u�W�F�N�g���K�؂Ɏ�������Ă��邩�ǂ������ׁA�܂��A�����蔻��̎�����񋟂��܂��B</summary>
struct CollisionCheckHelper<Type, MapType, false> {
	template <class IsTileEmpty>
	/// <summary>�����蔻��̎�����񋟂��܂��B</summary>
	/// <param name="obj">�����蔻����s���I�u�W�F�N�g�B</param>
	/// <param name="layor">�����蔻��̃t�B�[���h�ƂȂ� IMapTileLayor �I�u�W�F�N�g�B</param>
	/// <param name="EmptyCheck">��̃}�b�v�^�C�����ǂ������肷�邽�߂̊֐��܂��͊֐��I�u�W�F�N�g</param>
	static void CollisionCheck(Type& obj, IMapTileLayor<MapType>& layor, IsTileEmpty EmptyCheck);
};

#if defined(__BASE_FILE__)
/// @def �R���p�C�����̃\�[�X�t�@�C��
#	define MY_BASE_FILE __BASE_FILE__
#elif !defined(MY_BASE_FILE)
/// @def �R���p�C�����̃\�[�X�t�@�C��
#	define MY_BASE_FILE __FILE__
#endif

template <class Type, class MapType>
template <class IsTileEmpty>
void CollisionCheckHelper<Type, MapType, false>::CollisionCheck(Type& obj, IMapTileLayor<MapType>& layor, IsTileEmpty EmptyCheck) {
	
	DebugBreak();
#pragma message(MY_BASE_FILE"("GetMacroValue(__LINE__)",1): error : CollisionCheck �̑������ɂ́Ax, y, dx, dy, width, height���������\���̂��K�v�ł��B(�g�p��͏o�͂��m�F���Ă��������B)\n" \
	"�g�p��:\n" \
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
/// �n���ꂽ IMapTileLayor �I�u�W�F�N�g���ŁA�n���ꂽ�I�u�W�F�N�g�ɑ΂��Ă����蔻����s���܂��B
/// �}�b�v�^�C�����󂩂ǂ����́A���[�U�[��`�� MapTileIsEmpty �֐��AMapType �� Enable, IsEmpty �����o�̂���
/// ���݂�����̂��g�p����܂��B
/// �����蔻�肪���������ꍇ�́A���̂R�̂������Ȃ��Ƃ������`����Ă��邱�Ƃ��m�F���Ă��������B
/// </summary>
/// <param name="obj">
/// �����蔻����s���I�u�W�F�N�g�B���̃I�u�W�F�N�g�̓����o�Ƃ��āAx, y, width, height, dx, dy �������Ă���K�v������܂��B
/// ���̊֐����s���ƁA�I�u�W�F�N�g�� x, y �͓K�؂ɍX�V����A�}�b�v�^�C���ɂԂ������Ƃ��ɁA���̃I�u�W�F�N�g�� dx, dy �� 0 �ɐݒ肵�܂��B
/// �I�u�W�F�N�g�� width, height �� 0 �ł���Ƃ����蔻�肪���s����̂Œ��ӂ��Ă��������B
/// </param>
/// <param name="layor">�����蔻��̃t�B�[���h�ƂȂ� IMapTileLayor �I�u�W�F�N�g�B</param>
/// <example>
/// �g�p��:
/// ���ӁI
/// ���̃T���v���ł́A�֐����g�����߂̍ŏ����̕��������Љ�Ă��܂��B
/// �t�@�C�� "Collider.h" �Ɏ������T���v�����Q�l�ɂ��Ȃ���ǂ�ł��������B
/// <code>
/// // �}�b�v�^�C�����󂩂ǂ������肷��֐��B���̖��O�œ���̈����������Ă���ΌĂяo����܂��B
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
///		// ���ȊO -1 (�����Ȃ�) �Ŗ��߂�B
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
/// �n���ꂽ IMapTileLayor �I�u�W�F�N�g���ŁA�n���ꂽ�I�u�W�F�N�g�ɑ΂��Ă����蔻����s���܂��B
/// </summary>
/// <param name="obj">
/// �����蔻����s���I�u�W�F�N�g�B���̃I�u�W�F�N�g�̓����o�Ƃ��āAx, y, width, height, dx, dy �������Ă���K�v������܂��B
/// ���̊֐����s���ƁA�I�u�W�F�N�g�� x, y �͓K�؂ɍX�V����A�}�b�v�^�C���ɂԂ������Ƃ��ɁA���̃I�u�W�F�N�g�� dx, dy �� 0 �ɐݒ肵�܂��B
/// �I�u�W�F�N�g�� width, height �� 0 �ł���Ƃ����蔻�肪���s����̂Œ��ӂ��Ă��������B
/// </param>
/// <param name="layor">�����蔻��̃t�B�[���h�ƂȂ� IMapTileLayor �I�u�W�F�N�g�B</param>
/// <param name="EmptyTile">��̃}�b�v�^�C�����ǂ������肷�邽�߂̊֐��܂��͊֐��I�u�W�F�N�g</param>
/// <example>
/// �g�p��:
/// ���ӁI
/// ���̃T���v���ł́A�֐����g�����߂̍ŏ����̕��������Љ�Ă��܂��B
/// �t�@�C�� "Collider.h" �Ɏ������T���v�����Q�l�ɂ��Ȃ���ǂ�ł��������B
/// <code>
///	const int MapTilesWidth = 20;
///	const int MapTilesHeight = 15;
///
/// Map&lt;int&gt; MyMap { MapTilesWidth, MapTilesHeight };
///
/// // ���ȊO -1 (�����Ȃ�) �Ŗ��߂�B
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
/// �n���ꂽ IMapTileLayor �I�u�W�F�N�g���ŁA�n���ꂽ�I�u�W�F�N�g�ɑ΂��Ă����蔻����s���܂��B
/// </summary>
/// <param name="obj">
/// �����蔻����s���I�u�W�F�N�g�B���̃I�u�W�F�N�g�̓����o�Ƃ��āAx, y, width, height, dx, dy �������Ă���K�v������܂��B
/// ���̊֐����s���ƁA�I�u�W�F�N�g�� x, y �͓K�؂ɍX�V����A�}�b�v�^�C���ɂԂ������Ƃ��ɁA���̃I�u�W�F�N�g�� dx, dy �� 0 �ɐݒ肵�܂��B
/// �I�u�W�F�N�g�� width, height �� 0 �ł���Ƃ����蔻�肪���s����̂Œ��ӂ��Ă��������B
/// </param>
/// <param name="layor">�����蔻��̃t�B�[���h�ƂȂ� IMapTileLayor �I�u�W�F�N�g�B</param>
/// <param name="EmptyTile">��̃}�b�v�^�C���Ƃ��Ďg���l�B</param>
/// <example>
/// �g�p��:
/// ���ӁI
/// ���̃T���v���ł́A�֐����g�����߂̍ŏ����̕��������Љ�Ă��܂��B
/// �t�@�C�� "Collider.h" �Ɏ������T���v�����Q�l�ɂ��Ȃ���ǂ�ł��������B
/// <code>
///	const int MapTilesWidth = 20;
///	const int MapTilesHeight = 15;
///
/// Map&lt;int&gt; MyMap { MapTilesWidth, MapTilesHeight };
///
/// // ���ȊO -1 (�����Ȃ�) �Ŗ��߂�B
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
/// �n���ꂽ�Q�����z����^�C���}�b�v�Ƃ��Ĉ����A���̃^�C���}�b�v���œn���ꂽ�I�u�W�F�N�g�̂����蔻����s���܂��B
/// �}�b�v�^�C�����󂩂ǂ����́A���[�U�[��`�� MapTileIsEmpty �֐��AMapType �� Enable, IsEmpty �����o�̂���
/// ���݂�����̂��g�p����܂��B
/// �����蔻�肪���������ꍇ�́A���̂R�̂������Ȃ��Ƃ������`����Ă��邱�Ƃ��m�F���Ă��������B
/// </summary>
/// <param name="obj">
/// �����蔻����s���I�u�W�F�N�g�B���̃I�u�W�F�N�g�̓����o�Ƃ��āAx, y, width, height, dx, dy �������Ă���K�v������܂��B
/// ���̊֐����s���ƁA�I�u�W�F�N�g�� x, y �͓K�؂ɍX�V����A�}�b�v�^�C���ɂԂ������Ƃ��ɁA���̃I�u�W�F�N�g�� dx, dy �� 0 �ɐݒ肵�܂��B
/// �I�u�W�F�N�g�� width, height �� 0 �ł���Ƃ����蔻�肪���s����̂Œ��ӂ��Ă��������B
/// </param>
/// <param name="layor">�^�C���}�b�v�Ƃ��Ďg���Q�����z��B</param>
/// <param name="MapSize">�^�C���}�b�v�P�̕��B</param>
/// <example>
/// �g�p��:
/// ���ӁI
/// ���̃T���v���ł́A�֐����g�����߂̍ŏ����̕��������Љ�Ă��܂��B
/// �t�@�C�� "Collider.h" �Ɏ������T���v�����Q�l�ɂ��Ȃ���ǂ�ł��������B
/// <code>
/// // �}�b�v�^�C�����󂩂ǂ������肷��֐��B���̖��O�œ���̈����������Ă���ΌĂяo����܂��B
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
///		// ���ȊO -1 (�����Ȃ�) �Ŗ��߂�B
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
/// �n���ꂽ�Q�����z����^�C���}�b�v�Ƃ��Ĉ����A���̃^�C���}�b�v���œn���ꂽ�I�u�W�F�N�g�̂����蔻����s���܂��B
/// </summary>
/// <param name="obj">
/// �����蔻����s���I�u�W�F�N�g�B���̃I�u�W�F�N�g�̓����o�Ƃ��āAx, y, width, height, dx, dy �������Ă���K�v������܂��B
/// ���̊֐����s���ƁA�I�u�W�F�N�g�� x, y �͓K�؂ɍX�V����A�}�b�v�^�C���ɂԂ������Ƃ��ɁA���̃I�u�W�F�N�g�� dx, dy �� 0 �ɐݒ肵�܂��B
/// �I�u�W�F�N�g�� width, height �� 0 �ł���Ƃ����蔻�肪���s����̂Œ��ӂ��Ă��������B
/// </param>
/// <param name="layor">�^�C���}�b�v�Ƃ��Ďg���Q�����z��B</param>
/// <param name="MapSize">�^�C���}�b�v�P�̕��B</param>
/// <param name="EmptyTile">��̃}�b�v�^�C�����ǂ������肷�邽�߂̊֐��܂��͊֐��I�u�W�F�N�g</param>
/// <example>
/// �g�p��:
/// ���ӁI
/// ���̃T���v���ł́A�֐����g�����߂̍ŏ����̕��������Љ�Ă��܂��B
/// �t�@�C�� "Collider.h" �Ɏ������T���v�����Q�l�ɂ��Ȃ���ǂ�ł��������B
/// <code>
///	const int MapTilesWidth = 20;
///	const int MapTilesHeight = 15;
///
/// int MyMap[MapTilesWidth][MapTilesHeight];
///
/// // ���ȊO -1 (�����Ȃ�) �Ŗ��߂�B
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
/// �n���ꂽ�Q�����z����^�C���}�b�v�Ƃ��Ĉ����A���̃^�C���}�b�v���œn���ꂽ�I�u�W�F�N�g�̂����蔻����s���܂��B
/// </summary>
/// <param name="obj">
/// �����蔻����s���I�u�W�F�N�g�B���̃I�u�W�F�N�g�̓����o�Ƃ��āAx, y, width, height, dx, dy �������Ă���K�v������܂��B
/// ���̊֐����s���ƁA�I�u�W�F�N�g�� x, y �͓K�؂ɍX�V����A�}�b�v�^�C���ɂԂ������Ƃ��ɁA���̃I�u�W�F�N�g�� dx, dy �� 0 �ɐݒ肵�܂��B
/// �I�u�W�F�N�g�� width, height �� 0 �ł���Ƃ����蔻�肪���s����̂Œ��ӂ��Ă��������B
/// </param>
/// <param name="layor">�^�C���}�b�v�Ƃ��Ďg���Q�����z��B</param>
/// <param name="MapSize">�^�C���}�b�v�P�̕��B</param>
/// <param name="EmptyTile">��̃}�b�v�^�C���Ƃ��Ďg���l�B</param>
/// <example>
/// �g�p��:
/// ���ӁI
/// ���̃T���v���ł́A�֐����g�����߂̍ŏ����̕��������Љ�Ă��܂��B
/// �t�@�C�� "Collider.h" �Ɏ������T���v�����Q�l�ɂ��Ȃ���ǂ�ł��������B
/// <code>
///	const int MapTilesWidth = 20;
///	const int MapTilesHeight = 15;
///
/// int MyMap[MapTilesWidth][MapTilesHeight];
///
/// // ���ȊO -1 (�����Ȃ�) �Ŗ��߂�B
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