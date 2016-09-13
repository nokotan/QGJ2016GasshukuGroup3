#pragma once

/// @file DebugHelper.h
/// DebugHelper.h --------------------
/// �f�o�b�O�⏕�֐��Ɨ񋓑̐錾�}�N��
/// ----------------------------------

// �f�o�b�O�⏕�֐� ------------

#ifdef _DEBUG
	/// <summary>�t�@�C���ƍs�ԍ���ǉ����ĕW���o�͂ɕ�������o�͂��܂��B</summary>
	#define DebugTrace(format, ...) {	\
		printf(format, __VA_ARGS__);	\
		printf("\t�ꏊ: %s(%d) \n\t%s\n\n", __FILE__, __LINE__, __FUNCSIG__); \
	}
	/// <summary>���������U�̂Ƃ��A�t�@�C���ƍs�ԍ���ǉ����ĕW���o�͂ɕ�������o�͂��܂��B</summary>
	#define AssertTrace(condition, format, ...) {	\
		if (!(condition))								\
			DebugTrace(format, __VA_ARGS__);		\
	}
#else
	/// <summary>�t�@�C���ƍs�ԍ���ǉ����ĕW���o�͂ɕ�������o�͂��܂��B</summary>
	#define DebugTrace(format, ...)
	/// <summary>���������^�̂Ƃ��A�t�@�C���ƍs�ԍ���ǉ����ĕW���o�͂ɕ�������o�͂��܂��B</summary>
	#define AssertTrace(condition, format, ...)
#endif

/// <summary>�ϐ��̖��O���擾���܂��B</summary>
#define GetVariableName(x) #x
/// <summary>�}�N���ϐ��̒l��\����������擾���܂��B</summary>
#define GetMacroValue(x) GetVariableName(x)




// enumdef ------------------------------------------------------------
// �񋓑� name ���쐬���A���̗v�f�����i�[���� name_Count ����`���܂��B
// --------------------------------------------------------------------

template<size_t size>
/// <summary>�񋓑̂�錾���镶����̗v�f�����擾���܂��B</summary>
/// <param name="str">�񋓑̂��`���镶����</param>
int CountComma(const char (&str)[size]) {
	int count = 0;

	for (int i = 0; str[i] != '\0' && i < size; i++) {
		if (str[i] == ',' || str[i + 1] == '\0')
			count++;
	}

	return count;
}

/// <summary>�񋓑� name ���쐬���A���̗v�f�����i�[���� name_Count ����`���܂��B</summary>
#define enumdef(name, ...)		\
	enum name { __VA_ARGS__ };	\
	const int name##_Count = CountComma(#__VA_ARGS__);

/// <summary>�N���X name ���ɗ񋓑̂��쐬���A���̗v�f�����i�[���� Count ����`���܂�</summary>
#define enumclassdef(name, ...)		\
	class name {					\
		public:						\
		enum Enum { __VA_ARGS__ };	\
		static const int Count;		\
	};								\
	const int name::Count = CountComma(#__VA_ARGS__);