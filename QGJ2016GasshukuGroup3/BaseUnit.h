#pragma once

/// @file BaseUnit.h 
/// BaseUnit.h -------------------------------
/// �Q�[�����Ɏg���I�u�W�F�N�g�i�̊��N���X�j
/// �ŏI�X�V:
///			2016/08/14
/// ------------------------------------------

#include "Shape.h"

/// <summary>�}�b�v�^�C���̐ݒ�</summary>
struct MapTile {
	// �}�b�v�^�C���Ƃ��Ďg���\���́i�܂��̓N���X�j�������ɏ����Ă��������B
	// �� �}�b�v�^�C���Ƃ��Ďg���\���̖̂��O�� MyMapTile �̏ꍇ
	// typedef struct MyMapTile Type;
	//         ----------------
	//		   ����������������
	typedef int Type;

	// �}�b�v�^�C���̈�ӂ̒����������ɏ����Ă��������B
	static const int MapSize = 32;
	// static Type Empty;
private:
	// �C���X�^���X�����֎~
	MapTile();
	MapTile(MapTile&);
	MapTile &operator=(MapTile&);
};


#include <xutility>
#include <stdexcept>
#include <memory>

template <class Type>
/// <summary>�z��ɑ΂��Ĉ��S�ɗv�f�ɃA�N�Z�X������@�Ɣ����q��p�ӂ��܂��B</summary>
class DynamicArray {
	Type *Data;
	int Size;
public:
	/// <summary>
	/// ���I�Ɋ��蓖�Ă��z��Ƃ��̑傫������ DynamicArray �I�u�W�F�N�g�����������܂��B
	/// DynamicArray �I�u�W�F�N�g���g���ėv�f��ύX����ƁA���̔z��ɂ����̕ύX���K�p����܂��B
	/// �n���ꂽ�z��� DynamicArray �����Ŏ����I�ɉ�����邱�Ƃ͂Ȃ��̂Œ��ӂ��K�v�ł��B
	/// </summary>
	/// <param name="data">�����q��p�ӂ��������z��</param>
	/// <param name="size">�����q��p�ӂ��������z��̗v�f��</param>
	/// <remarks>
	/// �g�p��:
	/// <code>
	/// int *pArray = new int[10] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	/// {
	///		DynamicArray&lt;int&gt; dArray { pArray, 10 };
	///		// ���ʂ̔z��݂����ɗv�f��ύX
	///		// pArray[0] �� 10 �ɏ��������B
	///		dArray[0] = 10;
	///		
	///		// range-based for ���g����
	///		for (int& val : dArray) {
	///			printf("%d\n", val);
	///		}
	///	} 
	/// // �X�R�[�v�𔲂��Ă��ADynamicArray �ɂ���� pArray �͉������Ȃ��̂ŁA
	/// // �v���O���}���z���������Ȃ���΂Ȃ�Ȃ��B 
	/// delete[] pArray;
	/// </code>
	/// </remarks>
	DynamicArray(Type* data, int size) :
		Data(data),
		Size(size) {
	}

	// __declspec(property) �́AVisual C++ �̊g���Ȃ̂ŁAVisual C++ �̃R���p�C���̎����������o��ǉ�
#ifdef _MSC_BUILD
	__declspec(property(get = size))
	/// <summary>�v�f�����擾���܂��B</summary>
	size_t Length;
#endif

	/// <summary>�v�f�����擾���܂��B</summary>
	inline size_t size() const {
		return Size;
	}

	/// <summary>�����̔z��f�[�^���擾���܂��B</summary>
	inline Type *data() const {
		return Data;
	}

	/// <summary>�w�肵���v�f�̒l�ւ̎Q�Ƃ��擾���܂��B</summary>
	/// <param name="index">�A�N�Z�X����z��v�f�̃C���f�b�N�X�B0 �ȏ�A�v�f�������ł���K�v������܂��B</param>
	inline Type &at(int index) {
		if (index >= 0 && index < Size) {
			return Data[index];
		} else {
			throw std::out_of_range("bad index!");
		}
	}

	/// <summary>�w�肵���v�f�̒l�ւ̎Q�Ƃ��擾���܂��B</summary>
	/// <param name="index">�A�N�Z�X����z��v�f�̃C���f�b�N�X�B0 �ȏ�A�v�f�������ł���K�v������܂��B</param>
	inline Type &Item(int index) {
		return at(index);
	}

	/// <summary>�w�肵���v�f�̒l�ւ̎Q�Ƃ��擾���܂��B</summary>
	/// <param name="index">�A�N�Z�X����z��v�f�̃C���f�b�N�X�B0 �ȏ�A�v�f�������ł���K�v������܂��B</param>
	inline Type &operator[](int index) {
		return at(index);
	}

	/// <summary>DynamicArray �̗v�f���������ɑ�������C�e���[�^���������܂��B</summary>
	/// <remarks>
	/// �g�p��:
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
		/// <summary>�C�e���[�^���R�s�[���܂��B</summary>
		Iterator(const Iterator& obj) = default;

		/// <summary>�C�e���[�^���P�i�߂܂�</summary>
		/// <returns>�P�i�߂���̃C�e���[�^</returns>
		Iterator& operator++() {
			Index++;
			if (Index >= static_cast<size_t>(Container->Size)) Index = SIZE_MAX;
			return *this;
		}

		/// <summary>�C�e���[�^���P�i�߂܂�</summary>
		/// <returns>�P�i�߂�O�̃C�e���[�^</returns>
		Iterator operator++(int) {
			Iterator RetVal(*this);
			Index++;
			if (Index >= static_cast<size_t>(Container->Size)) Index = SIZE_MAX;
			return RetVal;
		}

		/// <summary>���݂̃C�e���[�^�̈ʒu�̒l���擾���܂��B</summary>
		Type &operator*() {
			static Type Dummy;
			return (Index != SIZE_MAX ? Container->Data[Index] : Dummy);
		}

		/// <summary>�Q�̃C�e���[�^�����������ǂ����m���߂܂��B</summary>
		bool operator==(const Iterator& obj) {
			return !(*this != obj);
		}

		/// <summary>�Q�̃C�e���[�^���������Ȃ����ǂ����m���߂܂��B</summary>
		bool operator!=(const Iterator& obj) {
			return (obj.Container != Container || obj.Index != Index);
		}
	};

	/// <summary>�ŏ��̗v�f�������C�e���[�^���擾���܂��B</summary>
	/// <example>
	/// �g�p��:
	/// class <see cref="Iterator">DynamicArray&lt;Type&gt::Iterator</see> ���Q�Ƃ̂��ƁB
	/// </example>
	Iterator begin() {
		return Iterator(this, 0);
	}

	/// <summary>�Ō�̗v�f�������C�e���[�^���擾���܂��B</summary>
	/// <example>
	/// �g�p��:
	/// class <see cref="Iterator">DynamicArray&lt;Type&gt::Iterator</see> ���Q�Ƃ̂��ƁB
	/// </example>
	Iterator end() {
		return Iterator(this);
	}
};

template <class Type>
/// <summary>�}�b�v�`�b�v���Ǘ����郌�C���[���������܂��B</summary>
class IMapTileLayor {
public:
	/// <summary>�񐔂��擾���܂��B</summary>
	virtual int Cols() = 0;
	/// <summary>�s�����擾���܂��B</summary>
	virtual int Rows() = 0;
	/// <summary>1�̃^�C���̑傫�����擾���܂��B</summary>
	virtual Pixel GetTileSize() { return static_cast<Pixel>(MapTile::MapSize); };
	/// <summary>X���W���擾���܂��B</summary>
	virtual Pixel GetX() { return 0; };
	/// <summary>Y���W���擾���܂��B</summary>
	virtual Pixel GetY() { return 0; };
	/// <summary>X�������̑��x���擾���܂��B</summary>
	virtual Pixel GetDeltaX() { return 0; };
	/// <summary>Y�������̑��x���擾���܂��B</summary>
	virtual Pixel GetDeltaY() { return 0; };

	/// <summary>�w�肵�����W�Ƀ}�b�v�`�b�v��z�u���܂��B</summary>
	/// <param name="tile">�z�u����}�b�v�`�b�v</param>
	/// <param name="x">�}�b�v�`�b�v��z�u����ꏊ�̗�</param>
	/// <param name="y">�}�b�v�`�b�v��z�u����ꏊ�̍s��</param>
	virtual void SetTile(Type& tile, int x, int y) = 0;
	/// <summary>�w�肵�����W�ɂ���}�b�v�`�b�v���擾���܂��B</summary>
	/// <param name="x">�擾����}�b�v�`�b�v�̗�</param>
	/// <param name="y">�擾����}�b�v�`�b�v�̍s��</param>
	virtual Type &GetTile(int x, int y) = 0;
};

template <class MapType>
/// <summary>���R�ɃT�C�Y���ύX�ł���^�C���}�b�v���������܂��B</summary>
/// <example>
/// �g�p��:
/// <code>
/// // int[120][30] �̂Q�����z����쐬���A-1�Ŗ��߂�
/// Map&lt;int&gt; MyMap { 120, 30, -1 };
/// // [] ���g���Ĕz��̓��e���X�V����B
/// MyMap[30][20] = 120;
/// // [] ���g���Ĕz��̓��e���擾����B
/// int val = MyMap[20][20];
/// 
/// printf("MyMap[20][20] = %d\n", val);
/// // �o��:
/// // MyMap[20][20] = -1
/// </code>
/// </example>
class Map : public IMapTileLayor<MapType> {
	MapType **Tiles;
	bool IsTilesCopyed;
	int Width, Height;

	// �R�s�[�͋֎~
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

	/// <summary>X���W���擾���܂��B</summary>
	virtual Pixel GetX() { return X; };
	/// <summary>Y���W���擾���܂��B</summary>
	virtual Pixel GetY() { return Y; };
	/// <summary>X�������̑��x���擾���܂��B</summary>
	virtual Pixel GetDeltaX() { return DeltaX; };
	/// <summary>Y�������̑��x���擾���܂��B</summary>
	virtual Pixel GetDeltaY() { return DeltaY; };

	/// <summary>��̃^�C���}�b�v���쐬���܂��B</summary>
	Map() : Tiles(nullptr), Width(0), Height(0), IsTilesCopyed(false) {
		//DefaultMap = *this;
	};

	/// <summary>�w�肵���傫���̃^�C���}�b�v���쐬���܂��B</summary>
	/// <param name="width">�V�����쐬���� Map �I�u�W�F�N�g�̕�</param>
	/// <param name="height">�V�����쐬���� Map �I�u�W�F�N�g�̍���</param>
	Map(int width, int height) : Map() {
		Create(width, height);

		//DefaultMap = *this;
	};

	/// <summary>�w�肵���傫���̃^�C���}�b�v���쐬���A�w�肵���l�Ŗ��߂܂��B</summary>
	/// <param name="width">�V�����쐬���� Map �I�u�W�F�N�g�̕�</param>
	/// <param name="height">�V�����쐬���� Map �I�u�W�F�N�g�̍���</param>
	Map(int width, int height, MapType& val) : Map() {
		Create(width, height);
		Fill(val);

		//DefaultMap = *this;
	};

	/// <summary>�w�肵���傫���̃^�C���}�b�v���쐬���A�w�肵���l�Ŗ��߂܂��B</summary>
	/// <param name="width">�V�����쐬���� Map �I�u�W�F�N�g�̕�</param>
	/// <param name="height">�V�����쐬���� Map �I�u�W�F�N�g�̍���</param>
	Map(int width, int height, MapType&& val) : Map() {
		Create(width, height);
		Fill(val);

		//DefaultMap = *this;
	};

	template <size_t width, size_t height>
	/// <summary>�����̂Q�����z�񂩂�^�C���}�b�v���쐬���܂��B</summary>
	/// <param name="tiles">�}�b�v�^�C�����쐬������Ƃ̂Q�����z��</param>
	/// <param name="forceclone">
	///	�R�s�[���������邩�ǂ����B�R�s�[���s��Ȃ���Γn���ꂽ�Q�����z��Ƃ��̃^�C���}�b�v�̊Ԃŋ��L����A
	///	�����ύX����Ƃ���������ύX����܂��B�R�s�[���s���Ɠn���ꂽ�Q�����z��Ƃ��̃^�C���}�b�v�͓Ɨ���������
	///	�ƂȂ�A�݂��ɉe����^���܂���B����ł͂��̃I�v�V������ false �ɐݒ肳��܂��B
	/// </param>
	Map(MapType(&tiles)[width][height], bool forceclone = false) :
		Tiles(nullptr),
		Width(width),
		Height(height),
		IsTilesCopyed(true) {
		// 2�����z����_�u���|�C���^�œn�����߂̉�����
		MapType** newtiles = new MapType*[width];

		for (size_t i = 0; i < width; i++) {
			newtiles[i] = tiles[i];
		}

		Create(width, height, newtiles, forceclone);

		delete[] newtiles;

		//DefaultMap = *this;
	}

	~Map() {
		if (Tiles != nullptr) {
			if (!IsTilesCopyed) {
				delete[] Tiles[0];
			}

			delete[] Tiles;
		}
	};
 
	/// <summary>�w�肵���傫���̃^�C���}�b�v���쐬���܂��B</summary>
	/// <param name="width">�V�����쐬����^�C���}�b�v�̕�</param>
	/// <param name="height">�V�����쐬����^�C���}�b�v�̍���</param>
	/// <param name="tiles">
	/// �}�b�v�^�C���̃R�s�[���Ƃ̂Q�����z��Bnullptr ��n���ƁA�����ŐV�����^�C���}�b�v���쐬���܂��B
	///	����ł́A���̈����� nullptr ���ݒ肳��܂��B
	/// </param>
	/// <param name="forceclone">
	/// �R�s�[���������邩�ǂ����B�R�s�[���s��Ȃ���� tiles �œn���ꂽ�Q�����z��Ƃ��̃^�C���}�b�v�̊Ԃŋ��L����A
	///	�����ύX����Ƃ���������ύX����܂��B�R�s�[���s���� tiles �œn���ꂽ�Q�����z��Ƃ��̃^�C���}�b�v�͓Ɨ���������
	///	�ƂȂ�A�݂��ɉe����^���܂���Btiles �� nullptr ��n�����ꍇ�A���̃I�v�V�����͖�������܂��B
	///	����ł͂��̃I�v�V������ false �ɐݒ肳��܂��B
	/// </param>
	void Create(int width, int height, MapType **source = nullptr, bool forceclone = false) {
		Width = width;
		Height = height;
		IsTilesCopyed = (source != nullptr && !forceclone);

		if (Tiles != nullptr) {
			// �f�X�g���N�^�����Ăяo���B
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

	/// <summary>���݂̃}�b�v���w�肵���l�Ŗ��߂܂��B</summary>
	void Fill(MapType& val) {
		if (Tiles != nullptr && Tiles[0] != nullptr) {
			for (auto& item : DynamicArray<MapType>(Tiles[0], Width * Height)) {
				item = val;
			}
		}
	}

	/// <summary>���݂̃}�b�v���w�肵���l�Ŗ��߂܂��B</summary>
	void Fill(MapType&& val) {
		if (Tiles != nullptr && Tiles[0] != nullptr) {
			for (auto& item : DynamicArray<MapType>(Tiles[0], Width * Height)) {
				item = val;
			}
		}
	}

	/// <summary>�񐔂��擾���܂��B</summary>
	int Cols() override { return Width; };
	/// <summary>�s�����擾���܂��B</summary>
	int Rows() override { return Height; };

	/// <summary>�w�肵���ꏊ�Ƀ}�b�v�`�b�v��z�u���܂��B</summary>
	/// <param name="tile">�z�u����}�b�v�`�b�v</param>
	/// <param name="x">�}�b�v�`�b�v��z�u����ꏊ�̗񐔁B0 �ȏ㕝�����ł���K�v������܂��B</param>
	/// <param name="y">�}�b�v�`�b�v��z�u����ꏊ�̍s���B0 �ȏ㍂�������ł���K�v������܂��B</param>
	/// <remarks>
	/// x, y �̂����ꂩ���͈͊O�̏ꍇ�A�������܂���B
	/// </remarks>
	void SetTile(MapType& tile, int x, int y) override {
		if (x >= 0 && x < Width && y >= 0 && y < Height) {
			Tiles[x][y] = tile;
		} else {
			// Will throw.
		}
	};

	/// <summary>�w�肵���ꏊ�ɂ���}�b�v�`�b�v���擾���܂��B</summary>
	/// <param name="x">�}�b�v�`�b�v��z�u����ꏊ�̗񐔁B0 �ȏ㕝�����ł���K�v������܂��B</param>
	/// <param name="y">�}�b�v�`�b�v��z�u����ꏊ�̍s���B0 �ȏ㍂�������ł���K�v������܂��B</param>
	/// <returns>
	/// �w�肵���ꏊ�ɂ���}�b�v�`�b�v�Bx, y �̂����ꂩ���͈͊O�̏ꍇ�A�ʒu (0, 0) �ɂ���}�b�v�`�b�v��Ԃ��܂��B
	/// </returns>
	MapType &GetTile(int x, int y) override {
		if (x >= 0 && x < Width && y >= 0 && y < Height) {
			return Tiles[x][y];
		} else {
			return Tiles[0][0];
		}
	};

	/// <summary>�w�肵���ꏊ�ɂ���}�b�v�`�b�v���擾���܂��B</summary>
	/// <param name="x">�}�b�v�`�b�v��z�u����ꏊ�̗񐔁B0 �ȏ㕝�����ł���K�v������܂��B</param>
	/// <returns>�w�肵���񐔂ɂ���}�b�v�`�b�v�Bx ���͈͊O�̏ꍇ�A��̔z���Ԃ��܂��B</returns>
	/// </summary>
	/// <example>
	/// �g�p��:
	/// <see cref="Map">Map&lt;MapType&gt;</see>���Q�Ƃ̂��ƁB
	/// </example>
	DynamicArray<MapType> operator[](int x) {
		if (x >= 0 && x < Width) {
			return DynamicArray<MapType>(Tiles[x], Height);
		} else {
			return DynamicArray<MapType>(nullptr, 0);
		}
	};

	

	// �C���X�^���X
	// static Map DefaultMap;
};

//template <class Type>
//Map<Type> Map<Type>::DefaultMap = Map<Type>();

typedef Map<MapTile::Type> CMap;

template <class Type, size_t Width, size_t Height>
/// <summary>
/// �����̂Q�����z�񂩂�AMap �I�u�W�F�N�g���쐬���܂��B�n���ꂽ�Q�����z��͂��̃^�C���}�b�v�̊Ԃŋ��L����A
///	�����ύX����Ƃ���������ύX����܂��B
/// </summary>
/// <example>
/// �g�p��:
/// <code>
/// int MapTiles[120][30];
/// MapTiles[20][20] = -1;
///
/// auto&amp;&amp; MyMap = CreateMap(MapTiles);
/// // ����������̂Ɠ���
///	// Map&lt;int&gt;&amp;&amp; MyMap { MapTiles };
///
/// // [] ���g���Ĕz��̓��e���X�V����B
/// MyMap[30][20] = 120;
/// // [] ���g���Ĕz��̓��e���擾����B
/// int val = MyMap[20][20];
/// 
/// printf("MyMap[20][20] = %d\n", val);
/// // �o��:
/// // MyMap[20][20] = -1
/// </code>
/// </example>
Map<Type> CreateMap(Type (&map)[Width][Height]) {
	return Map<Type>(map);
}

template <class Type, size_t Width, size_t Height>
/// <summary>�����̂Q�����z����^�C���}�b�v�Ƃ��Ďg�����߂̊֐���p�ӂ��܂��B</summary>
/// <example>
/// �g�p��:
/// <code>
/// int MapTiles[120][30];
/// MapTiles[20][20] = -1;
///
/// TemporaryMap&lt;int, 120, 30&gt; MyMap { MapTiles };
/// // ���̂悤�ɏ��������Ă��悢�B
/// // auto&amp;&amp; MyMap = CreateTemporaryMap(MapTiles);
///
/// // [] ���g���Ĕz��̓��e���X�V����B
/// MyMap[30][20] = 120;
/// // [] ���g���Ĕz��̓��e���擾����B
/// int val = MyMap[20][20];
/// 
/// printf("MyMap[20][20] = %d\n", val);
/// // �o��:
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
	
	/// <summary>�񐔂��擾���܂��B</summary>
	int Cols() override { return Width; };
	/// <summary>�s�����擾���܂��B</summary>
	int Rows() override { return Height; };

	/// <summary>�w�肵���ꏊ�Ƀ}�b�v�`�b�v��z�u���܂��B</summary>
	/// <param name="tile">�z�u����}�b�v�`�b�v</param>
	/// <param name="x">�}�b�v�`�b�v��z�u����ꏊ�̗񐔁B0 �ȏ㕝�����ł���K�v������܂��B</param>
	/// <param name="y">�}�b�v�`�b�v��z�u����ꏊ�̍s���B0 �ȏ㍂�������ł���K�v������܂��B</param>
	/// <remarks>
	/// x, y �̂����ꂩ���͈͊O�̏ꍇ�A�������܂���B
	/// </remarks>
	void SetTile(Type& tile, int x, int y) override {
		if (x >= 0 && x < Width && y >= 0 && y < Height) {
			Tiles[x][y] = tile;
		} else {
			// Will throw.
		}
	};

	/// <summary>�w�肵���ꏊ�ɂ���}�b�v�`�b�v���擾���܂��B</summary>
	/// <param name="x">�}�b�v�`�b�v��z�u����ꏊ�̗񐔁B0 �ȏ㕝�����ł���K�v������܂��B</param>
	/// <param name="y">�}�b�v�`�b�v��z�u����ꏊ�̍s���B0 �ȏ㍂�������ł���K�v������܂��B</param>
	/// <returns>
	/// �w�肵���ꏊ�ɂ���}�b�v�`�b�v�Bx, y �̂����ꂩ���͈͊O�̏ꍇ�A�ʒu (0, 0) �ɂ���}�b�v�`�b�v��Ԃ��܂��B
	/// </returns>
	Type &GetTile(int x, int y) override {
		if (x >= 0 && x < Width && y >= 0 && y < Height) {
			return Tiles[x][y];
		} else {
			return Tiles[0][0];
		}
	};

	/// <summary>�w�肵���ꏊ�ɂ���}�b�v�`�b�v���擾���܂��B</summary>
	/// <param name="x">�}�b�v�`�b�v��z�u����ꏊ�̗񐔁B0 �ȏ㕝�����ł���K�v������܂��B</param>
	/// <returns>�w�肵���񐔂ɂ���}�b�v�`�b�v�Bx ���͈͊O�̏ꍇ�A��̔z���Ԃ��܂��B</returns>
	/// <example>
	/// �g�p��:
	/// <see cref="TemporaryMap">TemporaryMap&lt;Type, Width, Height&gt;</see>���Q�Ƃ̂��ƁB
	/// </example>
	DynamicArray<Type> operator[](int x) {
		return DynamicArray<Type>(Tiles[x], Height);
	}
};

template <class Type, size_t Width, size_t Height>
/// <summary>
/// �����̂Q�����z�񂩂�ATemporaryMap �I�u�W�F�N�g���쐬���܂��B�n���ꂽ�Q�����z��͂��̃^�C���}�b�v�̊Ԃŋ��L����A
///	�����ύX����Ƃ���������ύX����܂��B
/// </summary>
/// <example>
/// �g�p��:
/// <code>
/// int MapTiles[120][30];
/// MapTiles[20][20] = -1;
///
/// auto&& MyMap = CreateTemporaryMap(MapTiles);
/// // ����������̂Ɠ���
///	// TemporaryMap&lt;int, 120, 30&gt;&amp;&amp; MyMap { MapTiles };
///
/// // [] ���g���Ĕz��̓��e���X�V����B
/// MyMap[30][20] = 120;
/// // [] ���g���Ĕz��̓��e���擾����B
/// int val = MyMap[20][20];
/// 
/// printf("MyMap[20][20] = %d\n", val);
/// // �o��:
/// // MyMap[20][20] = -1
/// </code>
/// </example>
TemporaryMap<Type, Width, Height> CreateTemporaryMap(Type(&map)[Width][Height]) {
	return TemporaryMap<Type, Width, Height>(map);
}